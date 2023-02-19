// SquareLine LVGL GENERATED FILE
// EDITOR VERSION: SquareLine Studio 1.1.1
// LVGL VERSION: 8.2.0
// PROJECT: SquareLine_Project


#include "ui.h"
#include "ui_helpers.h"
#include "ui_comp.h"

uint32_t LV_EVENT_GET_COMP_CHILD;

typedef struct {
    uint32_t child_idx;
    lv_obj_t * child;
} ui_comp_get_child_t;

lv_obj_t * ui_comp_get_child(lv_obj_t * comp, uint32_t child_idx)
{
    ui_comp_get_child_t info;
    info.child = NULL;
    info.child_idx = child_idx;
    lv_event_send(comp, LV_EVENT_GET_COMP_CHILD, &info);
    return info.child;
}

void get_component_child_event_cb(lv_event_t * e)
{
    lv_obj_t ** c = lv_event_get_user_data(e);
    ui_comp_get_child_t * info = lv_event_get_param(e);
    info->child = c[info->child_idx];
}

void del_component_child_event_cb(lv_event_t * e)
{
    lv_obj_t ** c = lv_event_get_user_data(e);
    lv_free(c);
}


// COMPONENT arc

lv_obj_t * ui_arc_create(lv_obj_t * comp_parent)
{

    lv_obj_t * cui_arc;
    cui_arc = lv_arc_create(comp_parent);
    lv_obj_set_width(cui_arc, 150);
    lv_obj_set_height(cui_arc, 150);
    lv_obj_set_x(cui_arc, -25);
    lv_obj_set_y(cui_arc, -14);
    lv_obj_set_align(cui_arc, LV_ALIGN_CENTER);
    lv_arc_set_value(cui_arc, 0);

    lv_obj_t ** children = lv_malloc(sizeof(lv_obj_t *) * _UI_COMP_ARC_NUM);
    children[UI_COMP_ARC_ARC] = cui_arc;
    lv_obj_add_event_cb(cui_arc, get_component_child_event_cb, LV_EVENT_GET_COMP_CHILD, children);
    lv_obj_add_event_cb(cui_arc, del_component_child_event_cb, LV_EVENT_DELETE, children);
    return cui_arc;
}

