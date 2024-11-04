#include <assert.h>
#include <neuron.h>

#include "profibus_req.h"
#include "profibus_stack.h"

struct profibus_stack {
    void *                    ctx;
    profibus_stack_send       send_fn;
    profibus_stack_value      value_fn;
    profibus_stack_write_resp write_resp;

    uint8_t *buf;
    uint16_t buf_size;

    uint8_t *out_data; // out data
};

profibus_stack_t *profibus_stack_create(void *ctx,
                                        profibus_stack_send       send_fn,
                                        profibus_stack_value      value_fn,
                                        profibus_stack_write_resp write_resp)
{
    profibus_stack_t *stack = calloc(1, sizeof(profibus_stack_t));

    stack->ctx        = ctx;
    stack->send_fn    = send_fn;
    stack->value_fn   = value_fn;
    stack->write_resp = write_resp;

    stack->buf_size = 256;
    stack->buf      = calloc(stack->buf_size, 1);

    return stack;
}

void profibus_stack_destroy(profibus_stack_t *stack)
{
    free(stack->buf);
    free(stack);
}

int profibus_stack_recv(profibus_stack_t *stack,
                        neu_protocol_unpack_buf_t *buf)
{
    struct modbus_header header   = { 0 };
    struct modbus_code   code     = { 0 };
    int                  ret      = 0;
    int64_t              ts_start = neu_time_ns();

    if (stack->protocol == MODBUS_PROTOCOL_TCP) {
        ret = modbus_header_unwrap(buf, &header);
        if (ret < 0) {
            plog_warn((neu_plugin_t *) stack->ctx, "try modbus rtu driver");
        }
        if (ret <= 0) {
            return -1;
        }

        neu_plugin_t *plugin = (neu_plugin_t *) stack->ctx;
        if (plugin->check_header && header.seq + 1 != stack->read_seq &&
            header.seq + 1 != stack->write_seq) {
            return -1;
        }
    }

    ret = modbus_code_unwrap(buf, &code);
    if (ret <= 0) {
        return -1;
    }

    if (code.slave_id != slave_id) {
        return -1;
    }

    switch (code.function) {
    case MODBUS_READ_COIL:
    case MODBUS_READ_INPUT:
    case MODBUS_READ_HOLD_REG:
    case MODBUS_READ_INPUT_REG: {
        neu_otel_trace_ctx trace = NULL;
        neu_otel_scope_ctx scope = NULL;
        if (neu_otel_data_is_started()) {
            trace = neu_otel_find_trace((void *) (intptr_t) stack->read_seq);
            if (trace) {
                char new_span_id[36] = { 0 };
                neu_otel_new_span_id(new_span_id);
                scope =
                    neu_otel_add_span2(trace, "driver cmd recv", new_span_id);
                neu_otel_scope_add_span_attr_int(scope, "thread id",
                                                 (int64_t)(pthread_self()));
                neu_otel_scope_set_span_start_time(scope, ts_start);
                neu_otel_scope_set_span_end_time(scope, neu_time_ns());
            }
        }

        struct modbus_data data  = { 0 };
        uint8_t *          bytes = NULL;
        ret                      = modbus_data_unwrap(buf, &data);
        if (ret <= 0) {
            return -1;
        }

        switch (stack->protocol) {
        case MODBUS_PROTOCOL_TCP:
            if (data.n_byte == 0xff) {
                bytes = neu_protocol_unpack_buf(buf,
                                                header.len -
                                                    sizeof(struct modbus_code) -
                                                    sizeof(struct modbus_data));
                if (bytes == NULL) {
                    return -1;
                }
                stack->value_fn(stack->ctx, code.slave_id,
                                header.len - sizeof(struct modbus_code) -
                                    sizeof(struct modbus_data),
                                bytes, 0, (void *) (intptr_t) stack->read_seq);
            } else {
                bytes = neu_protocol_unpack_buf(buf, data.n_byte);
                if (bytes == NULL) {
                    return -1;
                }
                stack->value_fn(stack->ctx, code.slave_id, data.n_byte, bytes,
                                0, (void *) (intptr_t) stack->read_seq);
            }
            break;
        case MODBUS_PROTOCOL_RTU:
            bytes = neu_protocol_unpack_buf(buf, data.n_byte);
            if (bytes == NULL) {
                return -1;
            }
            stack->value_fn(stack->ctx, code.slave_id, data.n_byte, bytes, 0,
                            (void *) (intptr_t) stack->read_seq);
            break;
        }

        break;
    }
    case MODBUS_WRITE_S_COIL:
    case MODBUS_WRITE_M_HOLD_REG:
    case MODBUS_WRITE_M_COIL: {
        struct modbus_address address = { 0 };
        ret                           = modbus_address_unwrap(buf, &address);
        if (ret <= 0) {
            return -1;
        }
        break;
    }
    case MODBUS_READ_COIL_ERR:
        return MODBUS_DEVICE_ERR;
    case MODBUS_READ_INPUT_ERR:
        return MODBUS_DEVICE_ERR;
    case MODBUS_READ_HOLD_REG_ERR:
        return MODBUS_DEVICE_ERR;
    case MODBUS_READ_INPUT_REG_ERR:
        return MODBUS_DEVICE_ERR;
    case MODBUS_WRITE_S_COIL_ERR:
        return MODBUS_DEVICE_ERR;
    case MODBUS_WRITE_S_HOLD_REG_ERR:
        return MODBUS_DEVICE_ERR;
    case MODBUS_WRITE_M_HOLD_REG_ERR:
        return MODBUS_DEVICE_ERR;
    case MODBUS_WRITE_M_COIL_ERR:
        return MODBUS_DEVICE_ERR;
    case MODBUS_WRITE_S_HOLD_REG:
        break;
    default:
        return -1;
    }

    if (stack->protocol == MODBUS_PROTOCOL_RTU) {
        struct modbus_crc crc = { 0 };
        ret                   = modbus_crc_unwrap(buf, &crc);
        if (ret <= 0) {
            return -1;
        }
    }

    return neu_protocol_unpack_buf_used_size(buf);
}

int profibus_stack_send(profibus_stack_t *stack)
{
  static __thread uint8_t                 buf[16] = { 0 };
    static __thread neu_protocol_pack_buf_t pbuf    = { 0 };
    int                                     ret     = 0;
    *response_size                                  = 0;
    modbus_action_e m_action                        = MODBUS_ACTION_DEFAULT;

    neu_protocol_pack_buf_init(&pbuf, buf, sizeof(buf));

    if (stack->protocol == MODBUS_PROTOCOL_RTU) {
        modbus_crc_wrap(&pbuf);
    }
    modbus_address_wrap(&pbuf, start_address, n_reg, m_action);

    switch (area) {
    case MODBUS_AREA_COIL:
        modbus_code_wrap(&pbuf, slave_id, MODBUS_READ_COIL);
        *response_size += n_reg / 8 + ((n_reg % 8) > 0 ? 1 : 0);
        break;
    case MODBUS_AREA_INPUT:
        modbus_code_wrap(&pbuf, slave_id, MODBUS_READ_INPUT);
        *response_size += n_reg / 8 + ((n_reg % 8) > 0 ? 1 : 0);
        break;
    case MODBUS_AREA_INPUT_REGISTER:
        modbus_code_wrap(&pbuf, slave_id, MODBUS_READ_INPUT_REG);
        *response_size += n_reg * 2;
        break;
    case MODBUS_AREA_HOLD_REGISTER:
        modbus_code_wrap(&pbuf, slave_id, MODBUS_READ_HOLD_REG);
        *response_size += n_reg * 2;
        break;
    }

    *response_size += sizeof(struct modbus_code);
    *response_size += sizeof(struct modbus_data);

    switch (stack->protocol) {
    case MODBUS_PROTOCOL_TCP:
        modbus_header_wrap(&pbuf, stack->read_seq++);
        *response_size += sizeof(struct modbus_header);
        break;
    case MODBUS_PROTOCOL_RTU:
        modbus_crc_set(&pbuf);
        *response_size += 2;
        break;
    }

    ret = stack->send_fn(stack->ctx, neu_protocol_pack_buf_used_size(&pbuf),
                         neu_protocol_pack_buf_get(&pbuf));
    if (ret <= 0 && !is_test) {
        stack->value_fn(stack->ctx, 0, 0, NULL, NEU_ERR_PLUGIN_DISCONNECTED);
        plog_warn((neu_plugin_t *) stack->ctx, "send read req fail, %hhu!%hu",
                  slave_id, start_address);
    }

    return ret;
}