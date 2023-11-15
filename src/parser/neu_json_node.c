/**
 * NEURON IIoT System for Industry 4.0
 * Copyright (C) 2020-2022 EMQ Technologies Co., Ltd All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 **/

/*
 * DO NOT EDIT THIS FILE MANUALLY!
 * It was automatically generated by `json-autotype`.
 */

#include <stdlib.h>
#include <string.h>

#include <jansson.h>

#include "json/json.h"

#include "neu_json_node.h"

int neu_json_encode_add_node_req(void *json_object, void *param)
{
    int                      ret = 0;
    neu_json_add_node_req_t *req = param;

    neu_json_elem_t req_elems[] = { {
                                        .name      = "plugin",
                                        .t         = NEU_JSON_STR,
                                        .v.val_str = req->plugin,
                                    },
                                    {
                                        .name      = "name",
                                        .t         = NEU_JSON_STR,
                                        .v.val_str = req->name,
                                    } };

    ret = neu_json_encode_field(json_object, req_elems,
                                NEU_JSON_ELEM_SIZE(req_elems));

    return ret;
}

int neu_json_decode_add_node_req(char *buf, neu_json_add_node_req_t **result)
{
    int                      ret      = 0;
    void *                   json_obj = NULL;
    neu_json_add_node_req_t *req = calloc(1, sizeof(neu_json_add_node_req_t));
    if (req == NULL) {
        return -1;
    }

    json_obj = neu_json_decode_new(buf);

    neu_json_elem_t req_elems[] = { {
                                        .name = "plugin",
                                        .t    = NEU_JSON_STR,
                                    },
                                    {
                                        .name = "name",
                                        .t    = NEU_JSON_STR,
                                    } };
    ret = neu_json_decode_by_json(json_obj, NEU_JSON_ELEM_SIZE(req_elems),
                                  req_elems);
    if (ret != 0) {
        goto decode_fail;
    }

    req->plugin = req_elems[0].v.val_str;
    req->name   = req_elems[1].v.val_str;

    *result = req;
    goto decode_exit;

decode_fail:
    free(req);
    ret = -1;

decode_exit:
    if (json_obj != NULL) {
        neu_json_decode_free(json_obj);
    }
    return ret;
}

void neu_json_decode_add_node_req_free(neu_json_add_node_req_t *req)
{

    free(req->plugin);
    free(req->name);

    free(req);
}

int neu_json_encode_del_node_req(void *json_object, void *param)
{
    int                      ret = 0;
    neu_json_del_node_req_t *req = param;

    neu_json_elem_t req_elems[] = { {
        .name      = "name",
        .t         = NEU_JSON_STR,
        .v.val_str = req->name,
    } };

    ret = neu_json_encode_field(json_object, req_elems,
                                NEU_JSON_ELEM_SIZE(req_elems));

    return ret;
}

int neu_json_decode_del_node_req(char *buf, neu_json_del_node_req_t **result)
{
    int                      ret      = 0;
    void *                   json_obj = NULL;
    neu_json_del_node_req_t *req = calloc(1, sizeof(neu_json_del_node_req_t));
    if (req == NULL) {
        return -1;
    }

    json_obj = neu_json_decode_new(buf);

    neu_json_elem_t req_elems[] = { {
        .name = "name",
        .t    = NEU_JSON_STR,
    } };
    ret = neu_json_decode_by_json(json_obj, NEU_JSON_ELEM_SIZE(req_elems),
                                  req_elems);
    if (ret != 0) {
        goto decode_fail;
    }

    req->name = req_elems[0].v.val_str;

    *result = req;
    goto decode_exit;

decode_fail:
    free(req);
    ret = -1;

decode_exit:
    if (json_obj != NULL) {
        neu_json_decode_free(json_obj);
    }
    return ret;
}

void neu_json_decode_del_node_req_free(neu_json_del_node_req_t *req)
{
    free(req->name);
    free(req);
}

int neu_json_encode_get_node_state_resp(void *json_object, void *param)
{
    int                             ret = 0;
    neu_json_get_node_state_resp_t *resp =
        (neu_json_get_node_state_resp_t *) param;
    int array_size = resp->is_driver ? 5 : 6;

    neu_json_elem_t resp_elems[] = { {
                                         .name      = "running",
                                         .t         = NEU_JSON_INT,
                                         .v.val_int = resp->running,
                                     },
                                     {
                                         .name      = "link",
                                         .t         = NEU_JSON_INT,
                                         .v.val_int = resp->link,
                                     },
                                     {
                                         .name      = "rtt",
                                         .t         = NEU_JSON_INT,
                                         .v.val_int = resp->rtt,
                                     },
                                     {
                                         .name      = "log_level",
                                         .t         = NEU_JSON_STR,
                                         .v.val_str = resp->log_level,
                                     },
                                     {
                                         .name      = "neuron_core",
                                         .t         = NEU_JSON_STR,
                                         .v.val_str = resp->core_level,
                                     },
                                     {
                                         .name      = "sub_group_count",
                                         .t         = NEU_JSON_INT,
                                         .v.val_int = resp->sub_group_count,
                                     } };

    ret = neu_json_encode_field(json_object, resp_elems, array_size);

    return ret;
}

int neu_json_encode_get_nodes_state_resp(void *json_object, void *param)
{
    int                              ret = 0;
    neu_json_get_nodes_state_resp_t *resp =
        (neu_json_get_nodes_state_resp_t *) param;

    void *                      node_array = neu_json_array();
    neu_json_get_nodes_state_t *p_node     = resp->nodes;
    for (int i = 0; i < resp->n_node; i++) {
        int             array_size   = p_node->is_driver ? 5 : 6;
        neu_json_elem_t node_elems[] = { {
                                             .name      = "node",
                                             .t         = NEU_JSON_STR,
                                             .v.val_str = p_node->name,
                                         },
                                         {
                                             .name      = "running",
                                             .t         = NEU_JSON_INT,
                                             .v.val_int = p_node->running,
                                         },
                                         {
                                             .name      = "link",
                                             .t         = NEU_JSON_INT,
                                             .v.val_int = p_node->link,
                                         },
                                         {
                                             .name      = "rtt",
                                             .t         = NEU_JSON_INT,
                                             .v.val_int = p_node->rtt,
                                         },
                                         {
                                             .name      = "log_level",
                                             .t         = NEU_JSON_STR,
                                             .v.val_str = p_node->log_level,
                                         },
                                         {
                                             .name = "sub_group_count",
                                             .t    = NEU_JSON_INT,
                                             .v.val_int =
                                                 p_node->sub_group_count,
                                         } };

        node_array = neu_json_encode_array(node_array, node_elems, array_size);
        p_node++;
    }

    neu_json_elem_t resp_elems[] = { {
                                         .name         = "states",
                                         .t            = NEU_JSON_OBJECT,
                                         .v.val_object = node_array,
                                     },
                                     {
                                         .name      = "neuron_core",
                                         .t         = NEU_JSON_STR,
                                         .v.val_str = resp->core_level,
                                     } };
    ret = neu_json_encode_field(json_object, resp_elems,
                                NEU_JSON_ELEM_SIZE(resp_elems));

    return ret;
}

int neu_json_encode_get_nodes_resp(void *json_object, void *param)
{
    int                        ret  = 0;
    neu_json_get_nodes_resp_t *resp = (neu_json_get_nodes_resp_t *) param;

    // accumulate data in nodes array if any
    void *node_array = json_object_get(json_object, "nodes");
    if (node_array) {
        json_incref(node_array);
    } else {
        node_array = json_array();
    }

    neu_json_get_nodes_resp_node_t *p_node = resp->nodes;
    for (int i = 0; i < resp->n_node; i++) {
        neu_json_elem_t node_elems[] = {
            {
                .name      = "plugin",
                .t         = NEU_JSON_STR,
                .v.val_str = p_node->plugin,
            },
            {
                .name      = "name",
                .t         = NEU_JSON_STR,
                .v.val_str = p_node->name,
            },
        };
        node_array = neu_json_encode_array(node_array, node_elems,
                                           NEU_JSON_ELEM_SIZE(node_elems));
        p_node++;
    }

    neu_json_elem_t resp_elems[] = { {
        .name         = "nodes",
        .t            = NEU_JSON_OBJECT,
        .v.val_object = node_array,
    } };
    ret = neu_json_encode_field(json_object, resp_elems,
                                NEU_JSON_ELEM_SIZE(resp_elems));

    return ret;
}

int neu_json_decode_get_nodes_resp(char *                      buf,
                                   neu_json_get_nodes_resp_t **result)
{
    json_t *json_obj = neu_json_decode_new(buf);
    if (NULL == json_obj) {
        return -1;
    }

    int ret = neu_json_decode_get_nodes_resp_json(json_obj, result);
    neu_json_decode_free(json_obj);
    return ret;
}

int neu_json_decode_get_nodes_resp_json(void *                      json_obj,
                                        neu_json_get_nodes_resp_t **result)
{
    int                        ret        = 0;
    json_t *                   node_array = NULL;
    neu_json_get_nodes_resp_t *resp       = NULL;
    int                        i          = 0;

    node_array = json_object_get(json_obj, "nodes");
    if (!json_is_array(node_array)) {
        goto decode_fail;
    }

    resp = calloc(1, sizeof(*resp));
    if (resp == NULL) {
        goto decode_fail;
    }

    resp->n_node = json_array_size(node_array);
    if (resp->n_node <= 0) {
        goto decode_fail;
    }

    resp->nodes = calloc(resp->n_node, sizeof(neu_json_get_nodes_resp_node_t));
    if (NULL == resp->nodes) {
        goto decode_fail;
    }

    for (i = 0; i < resp->n_node; i++) {
        neu_json_elem_t tag_elems[] = {
            {
                .name = "name",
                .t    = NEU_JSON_STR,
            },
            {
                .name = "plugin",
                .t    = NEU_JSON_STR,
            },
        };

        ret = neu_json_decode_by_json(json_array_get(node_array, i),
                                      NEU_JSON_ELEM_SIZE(tag_elems), tag_elems);

        // set the fields before check for easy clean up on error
        resp->nodes[i].name   = tag_elems[0].v.val_str;
        resp->nodes[i].plugin = tag_elems[1].v.val_str;

        if (ret != 0) {
            goto decode_fail;
        }
    }

    *result = resp;
    goto decode_exit;

decode_fail:
    neu_json_decode_get_nodes_resp_free(resp);
    ret = -1;

decode_exit:
    return ret;
}

void neu_json_decode_get_nodes_resp_free(neu_json_get_nodes_resp_t *resp)
{
    if (resp) {
        for (int i = 0; i < resp->n_node; ++i) {
            free(resp->nodes[i].name);
            free(resp->nodes[i].plugin);
        }
        free(resp->nodes);
        free(resp);
    }
}

int neu_json_encode_update_node_req(void *json_object, void *param)
{
    int                         ret = 0;
    neu_json_update_node_req_t *req = param;

    neu_json_elem_t req_elems[] = { {
                                        .name      = "name",
                                        .t         = NEU_JSON_STR,
                                        .v.val_str = req->name,
                                    },
                                    {
                                        .name      = "new_name",
                                        .t         = NEU_JSON_INT,
                                        .v.val_str = req->new_name,
                                    } };

    ret = neu_json_encode_field(json_object, req_elems,
                                NEU_JSON_ELEM_SIZE(req_elems));

    return ret;
}

int neu_json_decode_update_node_req(char *                       buf,
                                    neu_json_update_node_req_t **result)
{
    int                         ret      = 0;
    void *                      json_obj = NULL;
    neu_json_update_node_req_t *req =
        calloc(1, sizeof(neu_json_node_ctl_req_t));
    if (req == NULL) {
        return -1;
    }

    json_obj = neu_json_decode_new(buf);
    if (NULL == json_obj) {
        goto error;
    }

    neu_json_elem_t req_elems[] = { {
                                        .name = "name",
                                        .t    = NEU_JSON_STR,
                                    },
                                    {
                                        .name = "new_name",
                                        .t    = NEU_JSON_STR,
                                    } };
    ret = neu_json_decode_by_json(json_obj, NEU_JSON_ELEM_SIZE(req_elems),
                                  req_elems);
    if (ret != 0) {
        goto error;
    }

    req->name     = req_elems[0].v.val_str;
    req->new_name = req_elems[1].v.val_str;

    *result = req;
    neu_json_decode_free(json_obj);
    return 0;

error:
    neu_json_decode_update_node_req_free(req);
    neu_json_decode_free(json_obj);
    return -1;
}

void neu_json_decode_update_node_req_free(neu_json_update_node_req_t *req)
{
    if (req) {
        free(req->name);
        free(req->new_name);
        free(req);
    }
}

int neu_json_encode_node_ctl_req(void *json_object, void *param)
{
    int                      ret = 0;
    neu_json_node_ctl_req_t *req = param;

    neu_json_elem_t req_elems[] = { {
                                        .name      = "node",
                                        .t         = NEU_JSON_STR,
                                        .v.val_str = req->node,
                                    },
                                    {
                                        .name      = "cmd",
                                        .t         = NEU_JSON_INT,
                                        .v.val_int = req->cmd,
                                    } };

    ret = neu_json_encode_field(json_object, req_elems,
                                NEU_JSON_ELEM_SIZE(req_elems));

    return ret;
}

int neu_json_decode_node_ctl_req(char *buf, neu_json_node_ctl_req_t **result)
{
    int                      ret      = 0;
    void *                   json_obj = NULL;
    neu_json_node_ctl_req_t *req = calloc(1, sizeof(neu_json_node_ctl_req_t));
    if (req == NULL) {
        return -1;
    }

    json_obj = neu_json_decode_new(buf);

    neu_json_elem_t req_elems[] = { {
                                        .name = "node",
                                        .t    = NEU_JSON_STR,
                                    },
                                    {
                                        .name = "cmd",
                                        .t    = NEU_JSON_INT,
                                    } };
    ret = neu_json_decode_by_json(json_obj, NEU_JSON_ELEM_SIZE(req_elems),
                                  req_elems);
    if (ret != 0) {
        goto decode_fail;
    }

    req->node = req_elems[0].v.val_str;
    req->cmd  = req_elems[1].v.val_int;

    *result = req;
    goto decode_exit;

decode_fail:
    free(req);
    ret = -1;

decode_exit:
    if (json_obj != NULL) {
        neu_json_decode_free(json_obj);
    }
    return ret;
}

void neu_json_decode_node_ctl_req_free(neu_json_node_ctl_req_t *req)
{
    free(req->node);
    free(req);
}

int neu_json_encode_node_setting_req(void *json_object, void *param)
{
    int                          ret = 0;
    neu_json_node_setting_req_t *req = param;

    neu_json_elem_t req_elems[] = { {
        .name      = "node",
        .t         = NEU_JSON_STR,
        .v.val_str = req->node,
    } };

    ret = neu_json_encode_field(json_object, req_elems,
                                NEU_JSON_ELEM_SIZE(req_elems));

    if (0 == ret) {
        ret = neu_json_load_key(json_object, "params", req->setting, true);
    }

    return ret;
}

int neu_json_decode_node_setting_req(char *                        buf,
                                     neu_json_node_setting_req_t **result)
{
    int                          ret      = 0;
    void *                       json_obj = NULL;
    neu_json_node_setting_req_t *req =
        calloc(1, sizeof(neu_json_node_setting_req_t));
    if (req == NULL) {
        return -1;
    }

    json_obj = neu_json_decode_new(buf);

    neu_json_elem_t req_elems[] = { {
        .name = "node",
        .t    = NEU_JSON_STR,
    } };
    ret = neu_json_decode_by_json(json_obj, NEU_JSON_ELEM_SIZE(req_elems),
                                  req_elems);
    if (ret != 0) {
        goto decode_fail;
    }

    ret = neu_json_dump_key(json_obj, "params", &req->setting, true);
    if (0 != ret) {
        goto decode_fail;
    }

    req->node = req_elems[0].v.val_str;

    *result = req;
    goto decode_exit;

decode_fail:
    free(req->node);
    free(req->setting);
    free(req);
    ret = -1;

decode_exit:
    if (json_obj != NULL) {
        neu_json_decode_free(json_obj);
    }
    return ret;
}

void neu_json_decode_node_setting_req_free(neu_json_node_setting_req_t *req)
{
    free(req->node);
    free(req->setting);
    free(req);
}

int neu_json_encode_get_node_setting_resp(void *json_object, void *param)
{
    int                               ret  = 0;
    neu_json_get_node_setting_resp_t *resp = param;

    neu_json_elem_t resp_elems[] = { {
        .name      = "node",
        .t         = NEU_JSON_STR,
        .v.val_str = resp->node,
    } };

    ret = neu_json_encode_field(json_object, resp_elems,
                                NEU_JSON_ELEM_SIZE(resp_elems));

    if (0 == ret) {
        ret = neu_json_load_key(json_object, "params", resp->setting, true);
    }

    return ret;
}
