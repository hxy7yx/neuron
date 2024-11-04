#include <stdlib.h>

#include <neuron.h>

#include "errcodes.h"

#include "profibus_point.h"
#include "profibus_req.h"
#include "profibus_stack.h"

static neu_plugin_t *driver_open(void);

static int driver_close(neu_plugin_t *plugin);
static int driver_init(neu_plugin_t *plugin, bool load);
static int driver_uninit(neu_plugin_t *plugin);
static int driver_start(neu_plugin_t *plugin);
static int driver_stop(neu_plugin_t *plugin);
static int driver_config(neu_plugin_t *plugin, const char *config);
static int driver_request(neu_plugin_t *plugin, neu_reqresp_head_t *head,
                          void *data);

static int driver_validate_tag(neu_plugin_t *plugin, neu_datatag_t *tag);
static int driver_group_timer(neu_plugin_t *plugin, neu_plugin_group_t *group);
static int driver_write(neu_plugin_t *plugin, void *req, neu_datatag_t *tag,
                        neu_value_u value);

static const neu_plugin_intf_funs_t plugin_intf_funs = {
    .open    = driver_open,
    .close   = driver_close,
    .init    = driver_init,
    .uninit  = driver_uninit,
    .start   = driver_start,
    .stop    = driver_stop,
    .setting = driver_config,
    .request = driver_request,

    .driver.validate_tag  = driver_validate_tag,
    .driver.group_timer   = driver_group_timer,
    .driver.group_sync    = driver_group_timer,
    .driver.write_tag     = driver_write,
    .driver.tag_validator = NULL,
    .driver.write_tags    = NULL,
    .driver.test_read_tag = NULL,
    .driver.add_tags      = NULL,
    .driver.load_tags     = NULL,
    .driver.del_tags      = NULL,
};

const neu_plugin_module_t neu_plugin_module = {
    .version     = NEURON_PLUGIN_VER_1_0,
    .schema      = "profibus",
    .module_name = "PROFIBUS",
    .module_descr =
        "This plugin is used to connect devices using the PROFIBUS protocol. ",
    .module_descr_zh =
        "该插件用于连接使用 PROFIBUS 协议的设备。",
    .intf_funs = &plugin_intf_funs,
    .kind      = NEU_PLUGIN_KIND_SYSTEM,
    .type      = NEU_NA_TYPE_DRIVER,
    .display   = true,
    .single    = false,
};

static neu_plugin_t *driver_open(void)
{
    neu_plugin_t *plugin = calloc(1, sizeof(neu_plugin_t));

    neu_plugin_common_init(&plugin->common);

    return plugin;
}

static int driver_close(neu_plugin_t *plugin)
{
    free(plugin);

    return 0;
}

static int driver_init(neu_plugin_t *plugin, bool load)
{
    (void) load;
    plugin->events   = neu_event_new();
    plugin->stack    = profibus_stack_create((void *) plugin,
                                        profibus_send_msg, profibus_value_handle,
                                        profibus_write_resp);

    plog_notice(plugin, "%s init success", plugin->common.name);
    return 0;
}

static int driver_uninit(neu_plugin_t *plugin)
{
    plog_notice(plugin, "%s uninit start", plugin->common.name);
    if (plugin->conn != NULL) {
        neu_conn_destory(plugin->conn);
    }

    if (plugin->stack) {
        profibus_stack_destroy(plugin->stack);
    }

    neu_event_close(plugin->events);

    plog_notice(plugin, "%s uninit success", plugin->common.name);

    return 0;
}

static int driver_start(neu_plugin_t *plugin)
{
    neu_conn_start(plugin->conn);
    plog_notice(plugin, "%s start success", plugin->common.name);
    return 0;
}

static int driver_stop(neu_plugin_t *plugin)
{
    neu_conn_stop(plugin->conn);
    plog_notice(plugin, "%s stop success", plugin->common.name);
    return 0;
}

static int driver_config(neu_plugin_t *plugin, const char *config)
{
    int              ret       = 0;
    char *           err_param = NULL;
    neu_conn_param_t param     = { 0 };

    neu_json_elem_t timeout  = { .name = "timeout", .t = NEU_JSON_INT };
    neu_json_elem_t interval = { .name = "interval", .t = NEU_JSON_INT };

    neu_json_elem_t device = { .name = "device", .t = NEU_JSON_STR };
    neu_json_elem_t stop   = { .name = "stop", .t = NEU_JSON_INT };
    neu_json_elem_t parity = { .name = "parity", .t = NEU_JSON_INT };
    neu_json_elem_t baud   = { .name = "baud", .t = NEU_JSON_INT };
    neu_json_elem_t data   = { .name = "data", .t = NEU_JSON_INT };

    ret = neu_parse_param((char *) config, &err_param, 2, &timeout,
                          &interval);
    if (ret != 0) {
        plog_error(plugin, "config: %s, decode interval & timeout error: %s",
                   config, err_param);
        free(err_param);
        return -1;
    }

    if (timeout.v.val_int <= 0) {
        plog_error(plugin, "config: %s, set timeout error: %s", config,
                   err_param);
        free(err_param);
        return -1;
    }

    plugin->interval = interval.v.val_int;

    ret = neu_parse_param((char *) config, &err_param, 5, &device, &stop,
                          &parity, &baud, &data);

    if (ret != 0) {
        plog_error(plugin, "config: %s, decode error: %s", config, err_param);
        free(err_param);
        return -1;
    }

    param.log              = plugin->common.log;

        param.type = NEU_CONN_TTY_CLIENT;

        param.params.tty_client.device  = device.v.val_str;
        param.params.tty_client.baud    = baud.v.val_int;
        param.params.tty_client.data    = data.v.val_int;
        param.params.tty_client.parity  = parity.v.val_int;
        param.params.tty_client.stop    = stop.v.val_int;
        param.params.tty_client.timeout = timeout.v.val_int;

        plugin->is_serial = true;
        plog_notice(plugin,
                    "config: device: %s, baud: %" PRId64 ", data: %" PRId64
                    ", parity: %" PRId64 ", stop: %" PRId64 "",
                    device.v.val_str, baud.v.val_int, data.v.val_int,
                    parity.v.val_int, stop.v.val_int);

    if (plugin->conn != NULL) {
        plugin->conn = neu_conn_reconfig(plugin->conn, &param);
    } else {
        plugin->common.link_state = NEU_NODE_LINK_STATE_DISCONNECTED;
        plugin->conn =
            neu_conn_new(&param, (void *) plugin, profibus_conn_connected,
                         profibus_conn_disconnected);
    }

    free(device.v.val_str);

    return 0;
}

static int driver_request(neu_plugin_t *plugin, neu_reqresp_head_t *head,
                          void *data)
{
    (void) plugin;
    (void) head;
    (void) data;
    return 0;
}

static int driver_validate_tag(neu_plugin_t *plugin, neu_datatag_t *tag)
{
    modbus_point_t point = { 0 };

    int ret = modbus_tag_to_point(tag, &point, plugin->address_base);
    if (ret == 0) {
        plog_notice(
            plugin,
            "validate tag success, name: %s, address: %s, type: %d, slave id: "
            "%d, start address: %d, n register: %d, area: %s",
            tag->name, tag->address, tag->type, point.slave_id,
            point.start_address, point.n_register,
            modbus_area_to_str(point.area));
    } else {
        plog_error(
            plugin,
            "validate tag error, name: %s, address: %s, type: %d, slave id: "
            "%d, start address: %d, n register: %d, area: %s",
            tag->name, tag->address, tag->type, point.slave_id,
            point.start_address, point.n_register,
            modbus_area_to_str(point.area));
    }
    return ret;
}

static int driver_group_timer(neu_plugin_t *plugin, neu_plugin_group_t *group)
{
    return profibus_group_timer(plugin, group, 0xfa);
}

static int driver_write(neu_plugin_t *plugin, void *req, neu_datatag_t *tag,
                        neu_value_u value)
{
    return profibus_write_tag(plugin, req, tag, value);
}