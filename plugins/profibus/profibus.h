#ifndef _NEU_M_PLUGIN_PROFIBUS_H_
#define _NEU_M_PLUGIN_PROFIBUS_H_

#include <stdbool.h>
#include <stdint.h>

#include <neuron.h>

struct profibus_awake {
    uint8_t sd;
    uint8_t da;
    uint8_t sa;
    uint8_t fc;
    uint8_t fcs;
    uint8_t ed;
} __attribute__((packed));

void profibus_awake_wrap(neu_protocol_pack_buf_t *buf,
                         uint8_t da, uint8_t sa);

int profibus_awake_res(neu_protocol_unpack_buf_t *buf,
                       struct profibus_awake *    out_awake);

struct profibus_diag_req {
    uint8_t sd_1;
    uint8_t le;
    uint8_t ler;
    uint8_t sd_2;
    uint8_t da;
    uint8_t sa;
    uint8_t fc;
    uint8_t dsap;
    uint8_t ssap;
    uint8_t fcs;
    uint8_t ed;
} __attribute__((packed));

struct profibus_diag_res {
    uint8_t  sd_1;
    uint8_t  le;
    uint8_t  ler;
    uint8_t  sd_2;
    uint8_t  da;
    uint8_t  sa;
    uint8_t  fc;
    uint8_t  dsap;
    uint8_t  ssap;
    uint16_t res;
    uint16_t master_id;
    uint16_t device_id;
    uint8_t  fcs;
    uint8_t  ed;
} __attribute__((packed));

void profibus_diag_wrap(neu_protocol_pack_buf_t *buf,
                         uint8_t da, uint8_t sa);
int profibus_diag_res(neu_protocol_unpack_buf_t *buf,
                      struct profibus_diag_res * out_diag);

struct profibus_param {
    uint8_t  sd_1;
    uint8_t  le;
    uint8_t  ler;
    uint8_t  sd_2;
    uint8_t  da;
    uint8_t  sa;
    uint8_t  fc;
    uint8_t  dsap;
    uint8_t  ssap;
    uint8_t  req;
    uint16_t param;
    uint8_t  min_interval;
    uint8_t  group_id;
    uint16_t device_id;
    uint8_t  fcs;
    uint8_t  ed;
} __attribute__((packed));

void profibus_param_req(neu_protocol_pack_buf_t *buf,
                         uint8_t da, uint8_t sa, uint16_t device_id);

struct profibus_config_req {
    uint8_t  sd_1;
    uint8_t  le;
    uint8_t  ler;
    uint8_t  sd_2;
    uint8_t  da;
    uint8_t  sa;
    uint8_t  fc;
    uint8_t  dsap;
    uint8_t  ssap;
    uint16_t config;
    uint8_t  fcs;
    uint8_t  ed;
} __attribute__((packed));

void profibus_config_req(neu_protocol_pack_buf_t *buf,
                         uint8_t da, uint8_t sa, uint16_t config);

bool profibus_ed_res(neu_protocol_unpack_buf_t *buf);

struct profibus_data {
    uint8_t         sd_1;
    uint8_t         le;
    uint8_t         ler;
    uint8_t         sd_2;
    uint8_t         da;
    uint8_t         sa;
    uint8_t         fc;
} __attribute__((packed));

void profibus_data_req(neu_protocol_pack_buf_t *buf,
                       uint8_t da, uint8_t sa,
                       uint8_t *out_data, uint8_t out_len);

int profibus_data_res(neu_protocol_unpack_buf_t *buf,
                      uint8_t da, uint8_t sa,
                      uint8_t *in_data, uint8_t in_len);

#endif