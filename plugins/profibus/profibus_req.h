#ifndef _NEU_M_PLUGIN_PROFIBUS_REQ_H_
#define _NEU_M_PLUGIN_PROFIBUS_REQ_H_

#include <neuron.h>

#include "profibus_stack.h"

struct neu_plugin {
    neu_plugin_common_t common;

    neu_conn_t *    conn;
    modbus_stack_t *stack;

    void *   plugin_group_data;
    uint16_t cmd_idx;

    neu_event_io_t *tcp_server_io;
    bool            is_server;
    bool            is_serial;
    int             client_fd;
    neu_events_t *  events;

    modbus_protocol_e   protocol;
    modbus_endianess    endianess;
    modbus_address_base address_base;

    uint16_t interval;
    uint16_t retry_interval;
    uint16_t max_retries;
    uint16_t check_header;
    bool     degradation;
    uint16_t degrade_cycle;
    uint16_t degrade_time;
};

void modbus_conn_connected(void *data, int fd);
void modbus_conn_disconnected(void *data, int fd);
void modbus_tcp_server_listen(void *data, int fd);
void modbus_tcp_server_stop(void *data, int fd);
int  modbus_tcp_server_io_callback(enum neu_event_io_type type, int fd,
                                   void *usr_data);

int modbus_group_timer(neu_plugin_t *plugin, neu_plugin_group_t *group,
                       uint16_t max_byte);
int modbus_send_msg(void *ctx, uint16_t n_byte, uint8_t *bytes);
int modbus_value_handle(void *ctx, uint8_t slave_id, uint16_t n_byte,
                        uint8_t *bytes, int error, void *trace);
int modbus_write(neu_plugin_t *plugin, void *req, neu_datatag_t *tag,
                 neu_value_u value, bool response);
int modbus_write_tag(neu_plugin_t *plugin, void *req, neu_datatag_t *tag,
                     neu_value_u value);
int modbus_write_tags(neu_plugin_t *plugin, void *req, UT_array *tags);
int modbus_write_resp(void *ctx, void *req, int error);
int modbus_test_read_tag(neu_plugin_t *plugin, void *req, neu_datatag_t tag);
int modbus_value_handle_test(neu_plugin_t *plugin, void *req,
                             modbus_point_t *point, uint16_t n_byte,
                             uint8_t *bytes);

#endif
