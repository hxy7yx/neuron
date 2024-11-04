#ifndef _NEU_PLUGIN_PROFIBUS_POINT_H_
#define _NEU_PLUGIN_PROFIBUS_POINT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include <neuron.h>

#include "profibus.h"

typedef struct modbus_point {
    uint8_t       slave_id;
    modbus_area_e area;
    uint16_t      start_address;
    uint16_t      n_register;

    neu_type_e                type;
    neu_datatag_addr_option_u option;
    char                      name[NEU_TAG_NAME_LEN];
} modbus_point_t;

typedef struct modbus_point_write {
    modbus_point_t point;
    neu_value_u    value;
} modbus_point_write_t;

int modbus_tag_to_point(const neu_datatag_t *tag, modbus_point_t *point,
                        modbus_address_base address_base);
int modbus_write_tag_to_point(const neu_plugin_tag_value_t *tag,
                              modbus_point_write_t *        point,
                              modbus_address_base           address_base);

typedef struct modbus_read_cmd {
    uint8_t       slave_id;
    modbus_area_e area;
    uint16_t      start_address;
    uint16_t      n_register;

    UT_array *tags; // modbus_point_t ptr;
} modbus_read_cmd_t;

typedef struct modbus_read_cmd_sort {
    uint16_t           n_cmd;
    modbus_read_cmd_t *cmd;
} modbus_read_cmd_sort_t;

typedef struct modbus_write_cmd {
    uint8_t       slave_id;
    modbus_area_e area;
    uint16_t      start_address;
    uint16_t      n_register;
    uint8_t       n_byte;
    uint8_t *     bytes;

    UT_array *tags;
} modbus_write_cmd_t;

typedef struct modbus_write_cmd_sort {
    uint16_t            n_cmd;
    modbus_write_cmd_t *cmd;
} modbus_write_cmd_sort_t;

modbus_read_cmd_sort_t * modbus_tag_sort(UT_array *tags, uint16_t max_byte);
modbus_write_cmd_sort_t *modbus_write_tags_sort(UT_array *       tags,
                                                modbus_endianess endianess);
void                     modbus_tag_sort_free(modbus_read_cmd_sort_t *cs);

void modbus_convert_endianess(neu_value_u *value, modbus_endianess endianess);

#ifdef __cplusplus
}
#endif

#endif