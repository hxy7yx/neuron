#ifndef _NEU_M_PLUGIN_PROFIBUS_STACK_H_
#define _NEU_M_PLUGIN_PROFIBUS_STACK_H_

#include <stdint.h>

#include <neuron.h>

#include "profibus.h"
#include "profibus_point.h"

typedef struct profibus_stack profibus_stack_t;

typedef int (*profibus_stack_send)(void *ctx);
typedef int (*profibus_stack_value)(void *ctx);
typedef int (*profibus_stack_write_resp)(void *ctx, void *req, int error);

profibus_stack_t *profibus_stack_create(void *ctx,
                                        profibus_stack_send       send_fn,
                                        profibus_stack_value      value_fn,
                                        profibus_stack_write_resp write_resp);
void              profibus_stack_destroy(profibus_stack_t *stack);

int profibus_stack_recv(profibus_stack_t *stack,
                        neu_protocol_unpack_buf_t *buf);

int  profibus_stack_send(profibus_stack_t *profibus_stack_value);

#endif