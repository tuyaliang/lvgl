/**
 * @file lv_style.h
 *
 */

#ifndef LV_STYLE_H
#define LV_STYLE_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdbool.h>
#include "../lv_font/lv_font.h"
#include "../lv_misc/lv_color.h"
#include "../lv_misc/lv_area.h"
#include "../lv_misc/lv_anim.h"
#include "../lv_misc/lv_types.h"
#include "../lv_draw/lv_draw_blend.h"

/*********************
 *      DEFINES
 *********************/
#define LV_RADIUS_CIRCLE (LV_COORD_MAX) /**< A very big radius to always draw as circle*/
#define LV_STYLE_DEGUG_SENTINEL_VALUE 0x12345678

LV_EXPORT_CONST_INT(LV_RADIUS_CIRCLE);

/**********************
 *      TYPEDEFS
 **********************/

/*Border types (Use 'OR'ed values)*/
enum {
    LV_BORDER_PART_NONE     = 0x00,
    LV_BORDER_PART_BOTTOM   = 0x01,
    LV_BORDER_PART_TOP      = 0x02,
    LV_BORDER_PART_LEFT     = 0x04,
    LV_BORDER_PART_RIGHT    = 0x08,
    LV_BORDER_PART_FULL     = 0x0F,
    LV_BORDER_PART_INTERNAL = 0x10, /**< FOR matrix-like objects (e.g. Button matrix)*/
};
typedef uint8_t lv_border_part_t;

enum {
    LV_GRAD_DIR_NONE,
    LV_GRAD_DIR_VER,
    LV_GRAD_DIR_HOR,
};

typedef uint8_t lv_grad_dir_t;

#define LV_STYLE_PROP_INIT(name, id, attr)  name = (id | ((attr) << 8))

#define LV_STYLE_ID_MASK 0x00FF

#define LV_STYLE_ATTR_TYPE_OPA      (0 << 0)
#define LV_STYLE_ATTR_TYPE_VALUE    (1 << 0)
#define LV_STYLE_ATTR_TYPE_COLOR    (2 << 0)
#define LV_STYLE_ATTR_TYPE_PTR      (3 << 0)
#define LV_STYLE_ATTR_INHERIT       (1 << 3)

#define LV_STYLE_ATTR_FLAGS_MASK    (0x7 << 5)

typedef union {
    struct {
        uint8_t type        :3;
        uint8_t inherit     :1; /*1: The property can be inherited*/
        uint8_t cached      :1; /*1: Not a real property of this style  just cached from an other style*/
        uint8_t reserved    :3;
    }bits;
    uint8_t full;
}lv_style_attr_t;

enum {
    LV_STYLE_PROP_INIT(LV_STYLE_RADIUS,             0x01, LV_STYLE_ATTR_TYPE_VALUE),
    LV_STYLE_PROP_INIT(LV_STYLE_PAD_TOP,            0x04, LV_STYLE_ATTR_TYPE_VALUE),
    LV_STYLE_PROP_INIT(LV_STYLE_PAD_BOTTOM,         0x05, LV_STYLE_ATTR_TYPE_VALUE),
    LV_STYLE_PROP_INIT(LV_STYLE_PAD_LEFT,           0x06, LV_STYLE_ATTR_TYPE_VALUE),
    LV_STYLE_PROP_INIT(LV_STYLE_PAD_RIGHT,          0x07, LV_STYLE_ATTR_TYPE_VALUE),
    LV_STYLE_PROP_INIT(LV_STYLE_BG_COLOR,           0x10, LV_STYLE_ATTR_TYPE_COLOR),
    LV_STYLE_PROP_INIT(LV_STYLE_BG_OPA,             0x11, LV_STYLE_ATTR_TYPE_OPA),
    LV_STYLE_PROP_INIT(LV_STYLE_BG_CLIP_CORNER,     0x12, LV_STYLE_ATTR_TYPE_VALUE),
    LV_STYLE_PROP_INIT(LV_STYLE_BG_BLEND_MODE,      0x13, LV_STYLE_ATTR_TYPE_VALUE),
    LV_STYLE_PROP_INIT(LV_STYLE_BORDER_COLOR,       0x20, LV_STYLE_ATTR_TYPE_COLOR),
    LV_STYLE_PROP_INIT(LV_STYLE_BORDER_OPA,         0x21, LV_STYLE_ATTR_TYPE_OPA),
    LV_STYLE_PROP_INIT(LV_STYLE_BORDER_WIDTH,       0x22, LV_STYLE_ATTR_TYPE_VALUE),
    LV_STYLE_PROP_INIT(LV_STYLE_BORDER_PART,        0x23, LV_STYLE_ATTR_TYPE_VALUE),
    LV_STYLE_PROP_INIT(LV_STYLE_BORDER_BLEND_MODE,  0x24, LV_STYLE_ATTR_TYPE_VALUE),
    LV_STYLE_PROP_INIT(LV_STYLE_SHADOW_WIDTH,       0x30, LV_STYLE_ATTR_TYPE_VALUE),
    LV_STYLE_PROP_INIT(LV_STYLE_SHADOW_COLOR,       0x31, LV_STYLE_ATTR_TYPE_COLOR),
    LV_STYLE_PROP_INIT(LV_STYLE_SHADOW_OFFSET_X,    0x32, LV_STYLE_ATTR_TYPE_VALUE),
    LV_STYLE_PROP_INIT(LV_STYLE_SHADOW_OFFSET_Y,    0x33, LV_STYLE_ATTR_TYPE_VALUE),
    LV_STYLE_PROP_INIT(LV_STYLE_SHADOW_SPREAD,      0x34, LV_STYLE_ATTR_TYPE_VALUE),
    LV_STYLE_PROP_INIT(LV_STYLE_SHADOW_OPA,         0x35, LV_STYLE_ATTR_TYPE_OPA),
    LV_STYLE_PROP_INIT(LV_STYLE_TEXT_COLOR,         0x40, LV_STYLE_ATTR_TYPE_COLOR),
    LV_STYLE_PROP_INIT(LV_STYLE_LINE_COLOR,         0x50, LV_STYLE_ATTR_TYPE_COLOR),
    LV_STYLE_PROP_INIT(LV_STYLE_IMG_COLOR,          0x60, LV_STYLE_ATTR_TYPE_COLOR),
};

typedef uint16_t lv_style_property_t;

typedef struct {
    uint8_t * map;
    uint16_t used_groups;
    uint16_t size;
}lv_style_t;

typedef int16_t lv_style_value_t;

#if LV_USE_ANIMATION
/** Data structure for style animations. */
typedef struct
{
    lv_style_t style_start; /*Save not only pointers because can be same as 'style_anim' then it
                               will be modified too*/
    lv_style_t style_end;
    lv_style_t * style_anim;
    lv_anim_ready_cb_t ready_cb;
} lv_style_anim_dsc_t;
#endif

/**********************
 * GLOBAL PROTOTYPES
 **********************/


/**
 *  Init. the built-in styles
 */
void lv_style_built_in_init(void);

void lv_style_init(lv_style_t * style);

/**
 * Copy a style to an other
 * @param dest pointer to the destination style
 * @param src pointer to the source style
 */
void lv_style_copy(lv_style_t * dest, const lv_style_t * src);

/**
 * Mix two styles according to a given ratio
 * @param start start style
 * @param end end style
 * @param res store the result style here
 * @param ratio the ratio of mix [0..256]; 0: `start` style; 256: `end` style
 */
void lv_style_mix(const lv_style_t * start, const lv_style_t * end, lv_style_t * res, uint16_t ratio);

void lv_style_set_value(lv_style_t * style, lv_style_property_t prop, lv_style_value_t value);
void lv_style_set_color(lv_style_t * style, lv_style_property_t prop, lv_color_t color);
void lv_style_set_opa(lv_style_t * style, lv_style_property_t prop, lv_opa_t opa);

lv_res_t lv_style_get_value(const lv_style_t * style, lv_style_property_t prop, lv_style_value_t * res);
lv_res_t lv_style_get_opa(const lv_style_t * style, lv_style_property_t prop, lv_opa_t * res);
lv_res_t lv_style_get_color(const lv_style_t * style, lv_style_property_t prop, lv_color_t * res);

#if LV_USE_ANIMATION

/**
 * Initialize an animation variable.
 * E.g.:
 * lv_anim_t a;
 * lv_style_anim__init(&a);
 * lv_style_anim_set_...(&a);
 * lv_style_anim_create(&a);
 * @param a pointer to an `lv_anim_t` variable to initialize
 */
void lv_style_anim_init(lv_anim_t * a);

/**
 *
 * @param a pointer to an initialized `lv_anim_t` variable
 * @param to_anim pointer to the style to animate
 * @param start pointer to a style to animate from (start value)
 * @param end pointer to a style to animate to (end value)
 */
void lv_style_anim_set_styles(lv_anim_t * a, lv_style_t * to_anim, const lv_style_t * start, const lv_style_t * end);

/**
 * Set the duration and delay of an animation
 * @param a pointer to an initialized `lv_anim_t` variable
 * @param duration duration of the animation in milliseconds
 * @param delay delay before the animation in milliseconds
 */
static inline void lv_style_anim_set_time(lv_anim_t * a, uint16_t duration, int16_t delay)
{
    lv_anim_set_time(a, duration, delay);
}

/**
 * Set a function call when the animation is ready
 * @param a pointer to an initialized `lv_anim_t` variable
 * @param ready_cb a function call when the animation is ready
 */
static inline void lv_style_anim_set_ready_cb(lv_anim_t * a, lv_anim_ready_cb_t ready_cb)
{
    lv_style_anim_dsc_t * dsc = (lv_style_anim_dsc_t *)a->var;
    dsc->ready_cb             = ready_cb;
}

/**
 * Make the animation to play back to when the forward direction is ready
 * @param a pointer to an initialized `lv_anim_t` variable
 * @param wait_time time in milliseconds to wait before starting the back direction
 */
static inline void lv_style_anim_set_playback(lv_anim_t * a, uint16_t wait_time)
{
    lv_anim_set_playback(a, wait_time);
}

/**
 * Disable playback. (Disabled after `lv_anim_init()`)
 * @param a pointer to an initialized `lv_anim_t` variable
 */
static inline void lv_style_anim_clear_playback(lv_anim_t * a)
{
    lv_anim_clear_playback(a);
}

/**
 * Make the animation to start again when ready.
 * @param a pointer to an initialized `lv_anim_t` variable
 * @param wait_time time in milliseconds to wait before starting the animation again
 */
static inline void lv_style_anim_set_repeat(lv_anim_t * a, uint16_t wait_time)
{
    lv_anim_set_repeat(a, wait_time);
}

/**
 * Disable repeat. (Disabled after `lv_anim_init()`)
 * @param a pointer to an initialized `lv_anim_t` variable
 */
static inline void lv_style_anim_clear_repeat(lv_anim_t * a)
{
    lv_anim_clear_repeat(a);
}

/**
 * Create an animation
 * @param a an initialized 'anim_t' variable. Not required after call.
 */
static inline void lv_style_anim_create(lv_anim_t * a)
{
    lv_anim_create(a);
}

#endif

/*************************
 *    GLOBAL VARIABLES
 *************************/
//extern lv_style_t lv_style_scr;
//extern lv_style_t lv_style_transp;
//extern lv_style_t lv_style_transp_fit;
//extern lv_style_t lv_style_transp_tight;
//extern lv_style_t lv_style_plain;
//extern lv_style_t lv_style_plain_color;
//extern lv_style_t lv_style_pretty;
//extern lv_style_t lv_style_pretty_color;
//extern lv_style_t lv_style_btn_rel;
//extern lv_style_t lv_style_btn_pr;
//extern lv_style_t lv_style_btn_tgl_rel;
//extern lv_style_t lv_style_btn_tgl_pr;
//extern lv_style_t lv_style_btn_ina;

/**********************
 *      MACROS
 **********************/

/**
 * Create and initialize a `static` style
 * Example:
 *     LV_STYLE_CREATE(my_style, &lv_style_plain);
 *   is equivalent to
 *     static lv_style_t my_style;
 *     lv_style_copy(my_style, &lv_style_plain);
 *
 * If the style to copy is `NULL` `lv_style_plain` will be used.
 */
#define LV_STYLE_CREATE(name, copy_p) static lv_style_t name; lv_style_copy(&name, copy_p == NULL ? &lv_style_plain : copy_p);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_STYLE_H*/
