#include <assert.h>
#include <netinet/in.h>

#include <neuron.h>

#include "profibus.h"

static uint8_t calfcs(uint8_t *buf, int len)
{
    uint8_t fcs = 0;

    for (int i = 0; i < len; i++) {
        fcs += buf[i];
    }

    return fcs;
}

void profibus_awake_req(neu_protocol_pack_buf_t *buf,
                         uint8_t da, uint8_t sa)
{
    assert(neu_protocol_pack_buf_unused_size(buf) >=
           sizeof(struct profibus_awake));
    struct profibus_awake *req =
        (struct profibus_awake *) neu_protocol_pack_buf(
            buf, sizeof(struct profibus_awake));

    req->sd  = 0x10;
    req->da  = da;
    req->sa  = sa;
    req->fc  = 0x49;
    req->fcs = calfcs((uint8_t *) req + 1, 3);
    req->ed  = 0x16;
}

int profibus_awake_res(neu_protocol_unpack_buf_t *buf,
                       struct profibus_awake *    out_awake)
{
    struct profibus_awake *res =
        (struct profibus_awake *) neu_protocol_unpack_buf(
            buf, sizeof(struct profibus_awake));

    if (res == NULL) {
        return 0;
    }

    *out_awake = *res;

    return sizeof(struct profibus_awake);
}

void profibus_diag_req(neu_protocol_pack_buf_t *buf,
                         uint8_t da, uint8_t sa)
{
    assert(neu_protocol_pack_buf_unused_size(buf) >=
           sizeof(struct profibus_diag_req));
    struct profibus_diag_req *req =
        (struct profibus_diag_req *) neu_protocol_pack_buf(
            buf, sizeof(struct profibus_diag_req));

    req->sd_1 = 0x68;
    req->le   = 0x05;
    req->ler  = 0x05;
    req->sd_2 = 0x68;
    req->da   = 0x80 + da;
    req->sa   = 0x80 + sa;
    req->fc   = 0x6D;
    req->dsap = 0x3C;
    req->ssap = 0x3E;
    req->fcs  = calfcs((uint8_t *) req + 4, 5);
    req->ed   = 0x16;
}

int profibus_diag_res(neu_protocol_unpack_buf_t *buf,
                      struct profibus_diag_res * out_diag)
{
    struct profibus_diag_res *res =
        (struct profibus_diag_res *) neu_protocol_unpack_buf(
            buf, sizeof(struct profibus_diag_res));

    if (res == NULL) {
        return 0;
    }

    *out_awake = *res;

    return sizeof(struct profibus_diag_res);
}

void profibus_param_req(neu_protocol_pack_buf_t *buf,
                         uint8_t da, uint8_t sa, uint16_t device_id)
{
    assert(neu_protocol_pack_buf_unused_size(buf) >=
           sizeof(struct profibus_param));
    struct profibus_param *req =
        (struct profibus_param *) neu_protocol_pack_buf(
            buf, sizeof(struct profibus_param));

    req->sd_1         = 0x68;
    req->le           = 0x0C;
    req->ler          = 0x0C;
    req->sd_2         = 0x68;
    req->da           = 0x80 + da;
    req->sa           = 0x80 + sa;
    req->fc           = 0x5D;
    req->dsap         = 0x3D;
    req->ssap         = 0x3E;
    req->req          = 0x48;
    req->param        = 0x0A0A;
    req->min_interval = 0x0B;
    req->group_id     = 0x00;
    req->device_id    = htons(device_id);
    req->fcs          = calfcs((uint8_t *) req + 4, 12);
    req->ed           = 0x16;
}

void profibus_config_req(neu_protocol_pack_buf_t *buf,
                         uint8_t da, uint8_t sa, uint16_t config)
{
    assert(neu_protocol_pack_buf_unused_size(buf) >=
           sizeof(struct profibus_config_req));
    struct profibus_config_req *req =
        (struct profibus_config_req *) neu_protocol_pack_buf(
            buf, sizeof(struct profibus_config_req));

    req->sd_1   = 0x68;
    req->le     = 0x07;
    req->ler    = 0x07;
    req->sd_2   = 0x68;
    req->da     = 0x80 + da;
    req->sa     = 0x80 + sa;
    req->fc     = 0x7D;
    req->dsap   = 0x3E;
    req->ssap   = 0x3E;
    req->config = htons(config);
    req->fcs    = calfcs((uint8_t *) req + 4, 7);
    req->ed     = 0x16;
}

bool profibus_ed_res(neu_protocol_unpack_buf_t *buf)
{
    uint8_t *res = (uint8_t *) neu_protocol_unpack_buf(buf, 1);
    return res != NULL && *res == 0xE5;
}

void profibus_data_req(neu_protocol_pack_buf_t *buf,
                       uint8_t da, uint8_t sa,
                       uint8_t *out_data, uint8_t out_len)
{
    assert(neu_protocol_pack_buf_unused_size(buf) >=
           sizeof(struct profibus_data) + out_len + 2);
    struct profibus_data *req =
        (struct profibus_data *) neu_protocol_pack_buf(
            buf, sizeof(struct profibus_data));

    req->sd_1   = 0x68;
    req->le     = 0x13;
    req->ler    = 0x13;
    req->sd_2   = 0x68;
    req->da     = da;
    req->sa     = sa;
    req->fc     = 0x7D;

    uint8_t *data = neu_protocol_pack_buf(buf, out_len + 2);

    memcpy(data, out_data, out_len);

    data[out_len]     = calfcs((uint8_t *) req + 4, 3 + out_len);
    data[out_len + 1] = 0x16;
}

int profibus_data_res(neu_protocol_unpack_buf_t *buf,
                      uint8_t da, uint8_t sa,
                      uint8_t *in_data, uint8_t in_len)
{
    struct profibus_data *mdata = (struct profibus_data *) neu_protocol_unpack_buf(
        buf, sizeof(struct profibus_data));
    if (mdata == NULL || da != mdata->sa || sa != mdata->da) {
        return 0;
    }

    uint8_t *data = (uint8_t *) neu_protocol_unpack_buf(
        buf, in_len);
    if (data == NULL) {
        return 0;
    }

    memcpy(in_data, data, in_len);

    uint8_t *res = (uint8_t *) neu_protocol_unpack_buf(
        buf, 2);
    if (res == NULL) {
        return 0;
    }

    return in_len;
}