#pragma once
#define PBL_RESOURCES

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

struct Layer;
struct AccelData;
struct Animation;
struct ScrollLayer;
struct MenuLayer;
struct NumberWindow;
struct GContext;
typedef struct GContext GContext;
struct TextLayout;
typedef struct TextLayout TextLayout;
#define ARRAY_LENGTH(array) (sizeof((array))/sizeof((array)[0]))
#define IS_SIGNED(var) (((__typeof__(var)) - 1) < 0)
#define TupletBytes(_key, _data, _length) \
((const Tuplet) { .type = TUPLE_BYTE_ARRAY, .key = _key, .bytes = { .data = _data, .length = _length }})
#define TupletCString(_key, _cstring) \
((const Tuplet) { .type = TUPLE_CSTRING, .key = _key, .cstring = { .data = _cstring, .length = _cstring ? strlen(_cstring) + 1 : 0 }})
#define TupletInteger(_key, _integer) \
((const Tuplet) { .type = IS_SIGNED(_integer) ? TUPLE_INT : TUPLE_UINT, .key = _key, .integer = { .storage = _integer, .width = sizeof(_integer) }})
#define GPoint(x, y) ((GPoint){(x), (y)})
#define GPointZero GPoint(0, 0)
#define GSize(w, h) ((GSize){(w), (h)})
#define GSizeZero GSize(0, 0)
#define GRect(x, y, w, h) ((GRect){{(x), (y)}, {(w), (h)}})
#define GRectZero GRect(0, 0, 0, 0)
#define TRIG_MAX_RATIO 0xffff
#define TRIG_MAX_ANGLE 0x10000
#define ANIMATION_NORMALIZED_MIN 0
#define ANIMATION_NORMALIZED_MAX 65535
#define MENU_CELL_BASIC_HEADER_HEIGHT ((const int16_t) 16)
#define MENU_INDEX_NOT_FOUND ((const uint16_t) ~0)
#define MenuIndex(section, row) ((MenuIndex){ (section), (row) })
#define ACTION_BAR_WIDTH 20
#define NUM_ACTION_BAR_ITEMS 3
#define ANIMATION_DURATION_INFINITE ((uint32_t) ~0)

typedef struct AccelData
{
  int16_t x;
  int16_t y;
  int16_t z;
} AccelData;
typedef struct ListNode
{
  struct ListNode *next;
  struct ListNode *prev;
} ListNode;
typedef enum {AnimationCurveLinear = 0, AnimationCurveEaseIn = 1, AnimationCurveEaseOut = 2, AnimationCurveEaseInOut = 3, NumAnimationCurve = 4} AnimationCurve;
typedef void (*AnimationSetupImplementation)(struct Animation *animation);
typedef void (*AnimationUpdateImplementation)(struct Animation *animation, const uint32_t time_normalized);
typedef void (*AnimationTeardownImplementation)(struct Animation *animation);
typedef struct AnimationImplementation
{
  AnimationSetupImplementation setup;
  AnimationUpdateImplementation update;
  AnimationTeardownImplementation teardown;
} AnimationImplementation;
typedef void (*AnimationStartedHandler)(struct Animation *animation, void *context);
typedef void (*AnimationStoppedHandler)(struct Animation *animation, bool finished, void *context);
typedef struct AnimationHandlers
{
  AnimationStartedHandler started;
  AnimationStoppedHandler stopped;
} AnimationHandlers;
typedef struct Animation
{
  ListNode list_node;
  const struct AnimationImplementation *implementation;
  AnimationHandlers handlers;
  void *context;
  uint32_t abs_start_time_ms;
  uint32_t delay_ms;
  uint32_t duration_ms;
  AnimationCurve curve : 3;
  bool is_completed : 1;
} Animation;
typedef void *AppTaskContextRef;
typedef uint32_t AppTimerHandle;
typedef enum {BUTTON_ID_BACK = 0, BUTTON_ID_UP, BUTTON_ID_SELECT, BUTTON_ID_DOWN, NUM_BUTTONS} ButtonId;
typedef enum GColor {GColorClear = ~0, GColorBlack = 0, GColorWhite = 1} GColor;
typedef struct GPoint
{
  int16_t x;
  int16_t y;
} GPoint;
typedef struct GPath
{
  int num_points;
  GPoint *points;
  int32_t rotation;
  GPoint offset;
} GPath;
typedef struct GPathInfo
{
  int num_points;
  GPoint *points;
} GPathInfo;
typedef struct GSize
{
  int16_t w;
  int16_t h;
} GSize;
typedef struct GRect
{
  GPoint origin;
  GSize size;
} GRect;
typedef struct GBitmap
{
  void *addr;
  uint16_t row_size_bytes;
  uint16_t info_flags;
  GRect bounds;
} GBitmap;
typedef enum {GCompOpAssign, GCompOpAssignInverted, GCompOpOr, GCompOpAnd, GCompOpClear} GCompOp;
typedef struct 
{
  GRect clip_box;
  GRect drawing_box;
  GColor stroke_color : 2;
  GColor fill_color : 2;
  GColor text_color : 2;
  GCompOp compositing_mode : 3;
} GDrawState;
typedef enum {GCornerNone = 0, GCornerTopLeft = 1 << 0, GCornerTopRight = 1 << 1, GCornerBottomLeft = 1 << 2, GCornerBottomRight = 1 << 3, GCornersAll = ((GCornerTopLeft | GCornerTopRight) | GCornerBottomLeft) | GCornerBottomRight, GCornersTop = GCornerTopLeft | GCornerTopRight, GCornersBottom = GCornerBottomLeft | GCornerBottomRight, GCornersLeft = GCornerTopLeft | GCornerBottomLeft, GCornersRight = GCornerTopRight | GCornerBottomRight} GCornerMask;
typedef void *AppContextRef;
typedef struct 
{
  int tm_sec;
  int tm_min;
  int tm_hour;
  int tm_mday;
  int tm_mon;
  int tm_year;
  int tm_wday;
  int tm_yday;
  int tm_isdst;
} PblTm;
typedef enum {SECOND_UNIT = 1 << 0, MINUTE_UNIT = 1 << 1, HOUR_UNIT = 1 << 2, DAY_UNIT = 1 << 3, MONTH_UNIT = 1 << 4, YEAR_UNIT = 1 << 5} TimeUnits;
typedef void *ClickRecognizerRef;
typedef void (*ClickHandler)(ClickRecognizerRef recognizer, void *context);
typedef enum {TUPLE_BYTE_ARRAY = 0, TUPLE_CSTRING = 1, TUPLE_UINT = 2, TUPLE_INT = 3} TupleType;
typedef struct __attribute__((__packed__))
{
  uint32_t key;
  TupleType type : 8;
  uint16_t length;
  union 
  {
    uint8_t data[0];
    char cstring[0];
    uint8_t uint8;
    uint16_t uint16;
    uint32_t uint32;
    int8_t int8;
    int16_t int16;
    int32_t int32;
  } value[];
} Tuple;
typedef struct Tuplet
{
  TupleType type;
  uint32_t key;
  union 
  {
    struct 
    {
      const uint8_t *data;
      const uint16_t length;
    } bytes;
    struct 
    {
      const char *data;
      const uint16_t length;
    } cstring;
    struct 
    {
      uint32_t storage;
      const uint16_t width;
    } integer;
  };
} Tuplet;
typedef struct __attribute__((__packed__))
{
  uint8_t count;
  Tuple head[];
} Dictionary;
typedef struct 
{
  Dictionary *dictionary;
  const void *end;
  Tuple *cursor;
} DictionaryIterator;
typedef enum {APP_MSG_OK = 0, APP_MSG_SEND_TIMEOUT = 1 << 1, APP_MSG_SEND_REJECTED = 1 << 2, APP_MSG_NOT_CONNECTED = 1 << 3, APP_MSG_APP_NOT_RUNNING = 1 << 4, APP_MSG_INVALID_ARGS = 1 << 5, APP_MSG_BUSY = 1 << 6, APP_MSG_BUFFER_OVERFLOW = 1 << 7, APP_MSG_ALREADY_RELEASED = 1 << 9, APP_MSG_CALLBACK_ALREADY_REGISTERED = 1 << 10, APP_MSG_CALLBACK_NOT_REGISTERED = 1 << 11} AppMessageResult;
typedef struct AppMessageCallbacks
{
  void (*out_sent)(DictionaryIterator *sent, void *context);
  void (*out_failed)(DictionaryIterator *failed, AppMessageResult reason, void *context);
  void (*in_received)(DictionaryIterator *received, void *context);
  void (*in_dropped)(void *context, AppMessageResult reason);
} AppMessageCallbacks;
typedef struct AppMessageCallbacksNode
{
  ListNode node;
  void *context;
  AppMessageCallbacks callbacks;
} AppMessageCallbacksNode;
typedef struct 
{
  struct buffer_sizes
  {
    uint16_t inbound;
    uint16_t outbound;
  } buffer_sizes;
  AppMessageCallbacksNode default_callbacks;
} PebbleAppMessagingInfo;
typedef struct 
{
  void (*callback)(void *data);
  void *data;
} PebbleCallbackEvent;
typedef struct 
{
  PblTm *tick_time;
  TimeUnits units_changed;
} PebbleTickEvent;
typedef struct 
{
  struct Window *window;
  struct GContext *ctx;
} PebbleRenderEvent;
typedef struct 
{
  ButtonId button_id;
} PebbleButtonEvent;
typedef void (*PebbleAppInitEventHandler)(AppContextRef app_ctx);
typedef void (*PebbleAppButtonEventHandler)(AppContextRef app_ctx, PebbleButtonEvent *event);
typedef void (*PebbleAppRenderEventHandler)(AppContextRef app_ctx, PebbleRenderEvent *event);
typedef void (*PebbleAppDeinitEventHandler)(AppContextRef app_ctx);
typedef void (*PebbleAppTimerHandler)(AppContextRef app_ctx, AppTimerHandle handle, uint32_t cookie);
typedef void (*PebbleAppTickHandler)(AppContextRef app_ctx, PebbleTickEvent *event);
typedef struct 
{
  PebbleAppTickHandler tick_handler;
  TimeUnits tick_units;
} PebbleAppTickInfo;
typedef struct 
{
  struct buttons
  {
    PebbleAppButtonEventHandler up;
    PebbleAppButtonEventHandler down;
  } buttons;
} PebbleAppInputHandlers;
typedef struct 
{
  PebbleAppInitEventHandler init_handler;
  PebbleAppDeinitEventHandler deinit_handler;
  PebbleAppRenderEventHandler render_handler;
  PebbleAppInputHandlers input_handlers;
  PebbleAppTickInfo tick_info;
  PebbleAppTimerHandler timer_handler;
  PebbleAppMessagingInfo messaging_info;
} PebbleAppHandlers;
typedef void (*LayerUpdateProc)(struct Layer *layer, GContext *ctx);
typedef struct Layer
{
  GRect bounds;
  GRect frame;
  bool clips : 1;
  bool hidden : 1;
  struct Layer *next_sibling;
  struct Layer *parent;
  struct Layer *first_child;
  struct Window *window;
  LayerUpdateProc update_proc;
} Layer;
typedef void *GFont;
typedef enum {GTextOverflowModeWordWrap, GTextOverflowModeTrailingEllipsis} GTextOverflowMode;
typedef enum {GTextAlignmentLeft, GTextAlignmentCenter, GTextAlignmentRight} GTextAlignment;
typedef enum GAlign {GAlignCenter, GAlignTopLeft, GAlignTopRight, GAlignTop, GAlignLeft, GAlignBottom, GAlignRight, GAlignBottomRight, GAlignBottomLeft} GAlign;
typedef TextLayout *GTextLayoutCacheRef;
typedef struct TextLayer
{
  Layer layer;
  const char *text;
  GFont font;
  GTextLayoutCacheRef layout_cache;
  GColor text_color : 2;
  GColor background_color : 2;
  GTextOverflowMode overflow_mode : 2;
  GTextAlignment text_alignment : 2;
  bool should_cache_layout : 1;
} TextLayer;
typedef void (*WindowButtonEventHandler)(AppContextRef app_ctx, struct Window *window, PebbleButtonEvent *event);
typedef struct WindowInputHandlers
{
  struct 
  {
    WindowButtonEventHandler up;
    WindowButtonEventHandler down;
  } buttons;
} WindowInputHandlers;
typedef void (*WindowHandler)(struct Window *window);
typedef struct WindowHandlers
{
  WindowHandler load;
  WindowHandler appear;
  WindowHandler disappear;
  WindowHandler unload;
} WindowHandlers;
typedef struct ClickConfig
{
  void *context;
  struct click
  {
    ClickHandler handler;
    uint16_t repeat_interval_ms;
  } click;
  struct multi_click
  {
    uint8_t min;
    uint8_t max;
    bool last_click_only;
    ClickHandler handler;
    uint16_t timeout;
  } multi_click;
  struct long_click
  {
    uint16_t delay_ms;
    ClickHandler handler;
    ClickHandler release_handler;
  } long_click;
  struct raw
  {
    ClickHandler up_handler;
    ClickHandler down_handler;
    void *context;
  } raw;
} ClickConfig;
typedef void (*ClickConfigProvider)(ClickConfig **array_of_ptrs_to_click_configs_to_setup, void *context);
typedef struct Window
{
  Layer layer;
  const GBitmap *status_bar_icon;
  WindowInputHandlers input_handlers;
  WindowHandlers window_handlers;
  ClickConfigProvider click_config_provider;
  void *click_config_context;
  void *user_data;
  GColor background_color : 2;
  bool is_render_scheduled : 1;
  bool on_screen : 1;
  bool is_loaded : 1;
  bool overrides_back_button : 1;
  bool is_fullscreen : 1;
  const char *debug_name;
} Window;
typedef struct BitmapLayer
{
  Layer layer;
  const GBitmap *bitmap;
  GColor background_color : 2;
  GAlign alignment : 4;
  GCompOp compositing_mode : 3;
} BitmapLayer;
typedef struct 
{
  Layer layer;
  GBitmap *bitmap;
  GColor corner_clip_color;
  int32_t rotation;
  GPoint src_ic;
  GPoint dest_ic;
  GCompOp compositing_mode;
} RotBitmapLayer;
typedef struct 
{
  Layer layer;
  RotBitmapLayer white_layer;
  RotBitmapLayer black_layer;
} RotBmpPairLayer;
typedef struct 
{
  uint8_t *data;
  GBitmap bmp;
  BitmapLayer layer;
} BmpContainer;
typedef struct 
{
  uint8_t *data;
  GBitmap bmp;
  RotBitmapLayer layer;
} RotBmpContainer;
typedef struct 
{
  uint8_t *white_data;
  uint8_t *black_data;
  GBitmap white_bmp;
  GBitmap black_bmp;
  RotBmpPairLayer layer;
} RotBmpPairContainer;
typedef struct 
{
  uint32_t crc;
  uint32_t timestamp;
  char friendly_version[16];
} ResBankVersion;
typedef const ResBankVersion *ResVersionHandle;
typedef const void *ResHandle;
typedef struct 
{
  const uint32_t *durations;
  int num_segments;
} VibePattern;
typedef struct PropertyAnimation
{
  Animation animation;
  struct 
  {
    union 
    {
      GRect grect;
      GPoint gpoint;
      int16_t int16;
    } to;
    union 
    {
      GRect grect;
      GPoint gpoint;
      int16_t int16;
    } from;
  } values;
  void *subject;
} PropertyAnimation;
typedef struct InverterLayer
{
  Layer layer;
} InverterLayer;
typedef struct 
{
  uint8_t *data;
  GBitmap bmp;
} HeapBitmap;
typedef void (*ScrollLayerCallback)(struct ScrollLayer *scroll_layer, void *context);
typedef struct ScrollLayerCallbacks
{
  ClickConfigProvider click_config_provider;
  ScrollLayerCallback content_offset_changed_handler;
} ScrollLayerCallbacks;
typedef struct ScrollLayer
{
  Layer layer;
  Layer content_sublayer;
  Layer shadow_sublayer;
  PropertyAnimation animation;
  ScrollLayerCallbacks callbacks;
  void *context;
} ScrollLayer;
typedef struct MenuIndex
{
  uint16_t section;
  uint16_t row;
} MenuIndex;
typedef void (*MenuLayerSelectionChangedCallback)(struct MenuLayer *menu_layer, MenuIndex new_index, MenuIndex old_index, void *callback_context);
typedef void (*MenuLayerSelectCallback)(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context);
typedef void (*MenuLayerDrawRowCallback)(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *callback_context);
typedef void (*MenuLayerDrawHeaderCallback)(GContext *ctx, const Layer *cell_layer, uint16_t section_index, void *callback_context);
typedef int16_t (*MenuLayerGetHeaderHeightCallback)(struct MenuLayer *menu_layer, uint16_t section_index, void *callback_context);
typedef int16_t (*MenuLayerGetCellHeightCallback)(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context);
typedef uint16_t (*MenuLayerGetNumberOfRowsInSectionsCallback)(struct MenuLayer *menu_layer, uint16_t section_index, void *callback_context);
typedef uint16_t (*MenuLayerGetNumberOfSectionsCallback)(struct MenuLayer *menu_layer, void *callback_context);
typedef struct MenuLayerCallbacks
{
  MenuLayerGetNumberOfSectionsCallback get_num_sections;
  MenuLayerGetNumberOfRowsInSectionsCallback get_num_rows;
  MenuLayerGetCellHeightCallback get_cell_height;
  MenuLayerGetHeaderHeightCallback get_header_height;
  MenuLayerDrawRowCallback draw_row;
  MenuLayerDrawHeaderCallback draw_header;
  MenuLayerSelectCallback select_click;
  MenuLayerSelectCallback select_long_click;
  MenuLayerSelectionChangedCallback selection_changed;
} MenuLayerCallbacks;
typedef struct MenuCellSpan
{
  int16_t y;
  int16_t h;
  MenuIndex index;
} MenuCellSpan;
typedef enum {MenuRowAlignNone, MenuRowAlignCenter, MenuRowAlignTop, MenuRowAlignBottom} MenuRowAlign;
typedef struct MenuLayer
{
  ScrollLayer scroll_layer;
  InverterLayer inverter;
  struct 
  {
    MenuCellSpan cursor;
  } cache;
  MenuCellSpan selection;
  MenuLayerCallbacks callbacks;
  void *callback_context;
} MenuLayer;
typedef void (*SimpleMenuLayerSelectCallback)(int index, void *context);
typedef struct 
{
  const char *title;
  const char *subtitle;
  GBitmap *icon;
  SimpleMenuLayerSelectCallback callback;
} SimpleMenuItem;
typedef struct 
{
  const char *title;
  const SimpleMenuItem *items;
  uint32_t num_items;
} SimpleMenuSection;
typedef struct 
{
  MenuLayer menu;
  const SimpleMenuSection *sections;
  int num_sections;
  void *callback_context;
} SimpleMenuLayer;
typedef int32_t (*AnimationTimingFunction)(uint32_t time_normalized);
typedef struct ActionBarLayer
{
  Layer layer;
  const struct GBitmap *icons[3];
  struct Window *window;
  void *context;
  ClickConfigProvider click_config_provider;
  unsigned is_highlighted : 3;
  GColor background_color : 2;
} ActionBarLayer;
typedef void (*NumberWindowCallback)(struct NumberWindow *number_window, void *context);
typedef struct 
{
  NumberWindowCallback incremented;
  NumberWindowCallback decremented;
  NumberWindowCallback selected;
} NumberWindowCallbacks;
typedef struct NumberWindow
{
  Window window;
  ActionBarLayer action_bar;
  TextLayer value_label;
  TextLayer value_output;
  char value_output_buffer[12];
  int value;
  int max_val;
  int min_val;
  int step_size;
  bool is_wrapping_enabled;
  NumberWindowCallbacks callbacks;
  void *callback_context;
} NumberWindow;
typedef GPoint GPointReturn;
typedef GRect GRectReturn;
typedef void (*Int16Setter)(void *subject, int16_t int16);
typedef void (*GPointSetter)(void *subject, GPoint gpoint);
typedef void (*GRectSetter)(void *subject, GRect grect);
typedef int16_t (*Int16Getter)(void *subject);
typedef GPointReturn (*GPointGetter)(void *subject);
typedef GRectReturn (*GRectGetter)(void *subject);
typedef struct PropertyAnimationAccessors
{
  union 
  {
    Int16Setter int16;
    GPointSetter gpoint;
    GRectSetter grect;
  } setter;
  union 
  {
    Int16Getter int16;
    GPointGetter gpoint;
    GRectGetter grect;
  } getter;
} PropertyAnimationAccessors;
typedef struct PropertyAnimationImplementation
{
  AnimationImplementation base;
  PropertyAnimationAccessors accessors;
} PropertyAnimationImplementation;
typedef enum {DICT_OK = 0, DICT_NOT_ENOUGH_STORAGE = 1 << 1, DICT_INVALID_ARGS = 1 << 2, DICT_INTERNAL_INCONSISTENCY = 1 << 3} DictionaryResult;
typedef void (*AppSyncTupleChangedCallback)(const uint32_t key, const Tuple *new_tuple, const Tuple *old_tuple, void *context);
typedef void (*AppSyncErrorCallback)(DictionaryResult dict_error, AppMessageResult app_message_error, void *context);
typedef struct AppSync
{
  DictionaryIterator current_iter;
  union 
  {
    Dictionary *current;
    uint8_t *buffer;
  };
  uint16_t buffer_size;
  struct 
  {
    AppSyncTupleChangedCallback value_changed;
    AppSyncErrorCallback error;
    void *context;
  } callback;
  AppMessageCallbacksNode app_message_cb_node;
} AppSync;
typedef void (*DictionarySerializeCallback)(const uint8_t * const data, const uint16_t size, void *context);
typedef void (*DictionaryKeyUpdatedCallback)(const uint32_t key, const Tuple *new_tuple, const Tuple *old_tuple, void *context);

void animation_init(struct Animation *animation);
void animation_set_delay(struct Animation *animation, uint32_t delay_ms);
void animation_set_duration(struct Animation *animation, uint32_t duration_ms);
void animation_set_curve(struct Animation *animation, AnimationCurve curve);
void animation_set_handlers(struct Animation *animation, AnimationHandlers callbacks, void *context);
void animation_set_implementation(struct Animation *animation, const AnimationImplementation *implementation);
void *animation_get_context(struct Animation *animation);
void animation_schedule(struct Animation *animation);
void animation_unschedule(struct Animation *animation);
void animation_unschedule_all(void);
bool animation_is_scheduled(struct Animation *animation);
AppTimerHandle app_timer_send_event(AppContextRef app_ctx, uint32_t timeout_ms, uint32_t cookie);
bool app_timer_cancel_event(AppContextRef app_ctx_ref, AppTimerHandle handle);
void app_event_loop(AppTaskContextRef app_task_ctx, PebbleAppHandlers *handlers);
bool bmp_init_container(int resource_id, BmpContainer *c);
void bmp_deinit_container(BmpContainer *c);
int32_t cos_lookup(int32_t angle);
GFont fonts_get_system_font(const char *font_key);
GFont fonts_load_custom_font(ResHandle resource);
void fonts_unload_custom_font(GFont font);
void graphics_context_set_stroke_color(GContext *ctx, GColor color);
void graphics_context_set_fill_color(GContext *ctx, GColor color);
void graphics_context_set_text_color(GContext *ctx, GColor color);
void graphics_context_set_compositing_mode(GContext *ctx, GCompOp mode);
void graphics_draw_pixel(GContext *ctx, GPoint point);
void graphics_draw_line(GContext *ctx, GPoint p0, GPoint p1);
void graphics_fill_rect(GContext *ctx, GRect rect, uint8_t corner_radius, GCornerMask corner_mask);
void graphics_draw_circle(GContext *ctx, GPoint p, int radius);
void graphics_fill_circle(GContext *ctx, GPoint p, int radius);
void graphics_draw_round_rect(GContext *ctx, GRect rect, int radius);
void get_time(PblTm *time);
void gpath_init(GPath *path, const GPathInfo *init);
void gpath_move_to(GPath *path, GPoint point);
void gpath_rotate_to(GPath *path, int32_t angle);
void gpath_draw_outline(GContext *ctx, GPath *path);
void gpath_draw_filled(GContext *ctx, GPath *path);
GPoint grect_center_point(GRect *rect);
void layer_mark_dirty(Layer *layer);
void layer_remove_from_parent(Layer *child);
void layer_add_child(Layer *parent, Layer *child);
GRect layer_get_frame(Layer *layer);
void layer_set_frame(Layer *layer, GRect frame);
void layer_set_hidden(Layer *layer, bool hidden);
void layer_init(Layer *layer, GRect frame);
void light_enable(bool enable);
void light_enable_interaction(void);
void psleep(int millis);
void resource_init_current_app(ResVersionHandle version);
ResHandle resource_get_handle(uint32_t file_id);
size_t resource_load(ResHandle h, uint8_t *buffer, size_t max_length);
size_t resource_load_byte_range(ResHandle h, uint32_t start_bytes, uint8_t *data, size_t num_bytes);
size_t resource_size(ResHandle h);
void rotbmp_deinit_container(RotBmpContainer *c);
bool rotbmp_init_container(int resource_id, RotBmpContainer *c);
void rotbmp_pair_deinit_container(RotBmpPairContainer *c);
bool rotbmp_pair_init_container(int white_resource_id, int black_resource_id, RotBmpPairContainer *c);
void rotbmp_pair_layer_set_src_ic(RotBmpPairLayer *pair, GPoint ic);
void rotbmp_pair_layer_set_angle(RotBmpPairLayer *pair, int32_t angle);
void window_init(Window *window, const char *debug_name);
void window_stack_push(Window *window, bool animated);
void window_set_click_config_provider(Window *window, ClickConfigProvider click_config_provider);
void window_set_background_color(Window *window, GColor background_color);
void window_render(Window *window, GContext *ctx);
void window_set_fullscreen(Window *window, bool enabled);
int32_t sin_lookup(int32_t angle);
void string_format_time(char *ptr, size_t maxsize, const char *format, const PblTm *timeptr);
void text_layer_init(TextLayer *text_layer, GRect frame);
const char *text_layer_get_text(TextLayer *text_layer);
void text_layer_set_text(TextLayer *text_layer, const char *text);
void text_layer_set_font(TextLayer *text_layer, GFont font);
void text_layer_set_text_color(TextLayer *text_layer, GColor color);
void text_layer_set_background_color(TextLayer *text_layer, GColor color);
void vibes_double_pulse(void);
void vibes_enqueue_custom_pattern(VibePattern pattern);
void vibes_long_pulse(void);
void vibes_short_pulse(void);
GContext *app_get_current_graphics_context(void);
bool clock_is_24h_style(void);
void property_animation_init_layer_frame(struct PropertyAnimation *property_animation, struct Layer *layer, GRect *from_frame, GRect *to_frame);
void text_layer_set_text_alignment(TextLayer *text_layer, GTextAlignment text_alignment);
void graphics_draw_bitmap_in_rect(GContext *ctx, const GBitmap *bitmap, GRect rect);
void graphics_text_draw(GContext *ctx, const char *text, const GFont font, const GRect box, const GTextOverflowMode overflow_mode, const GTextAlignment alignment, const GTextLayoutCacheRef layout);
void layer_set_bounds(Layer *layer, GRect bounds);
GRect layer_get_bounds(Layer *layer);
void layer_set_update_proc(Layer *layer, LayerUpdateProc update_proc);
struct Window *layer_get_window(Layer *layer);
void layer_remove_child_layers(Layer *parent);
void layer_insert_below_sibling(Layer *layer_to_insert, Layer *below_sibling_layer);
void layer_insert_above_sibling(Layer *layer_to_insert, Layer *above_sibling_layer);
bool layer_get_hidden(Layer *layer);
void layer_set_clips(Layer *layer, bool clips);
bool layer_get_clips(Layer *layer);
GSize text_layer_get_max_used_size(GContext *ctx, TextLayer *text_layer);
void text_layer_set_size(TextLayer *text_layer, const GSize max_size);
void text_layer_set_overflow_mode(TextLayer *text_layer, GTextOverflowMode line_mode);
GSize graphics_text_layout_get_max_used_size(GContext *ctx, const char *text, const GFont font, const GRect box, const GTextOverflowMode overflow_mode, const GTextAlignment alignment, GTextLayoutCacheRef layout);
void inverter_layer_init(InverterLayer *inverter, GRect frame);
void bitmap_layer_init(BitmapLayer *bitmap_layer, GRect frame);
void bitmap_layer_set_bitmap(BitmapLayer *bitmap_layer, const GBitmap *bitmap);
void bitmap_layer_set_alignment(BitmapLayer *bitmap_layer, GAlign alignment);
void bitmap_layer_set_background_color(BitmapLayer *bitmap_layer, GColor color);
void bitmap_layer_set_compositing_mode(BitmapLayer *bitmap_layer, GCompOp mode);
bool heap_bitmap_init(HeapBitmap *hb, int resource_id);
void heap_bitmap_deinit(HeapBitmap *hb);
ButtonId click_recognizer_get_button_id(ClickRecognizerRef recognizer);
uint8_t click_number_of_clicks_counted(ClickRecognizerRef recognizer);
void menu_cell_basic_draw(GContext *ctx, const Layer *cell_layer, const char *title, const char *subtitle, GBitmap *icon);
void menu_cell_title_draw(GContext *ctx, const Layer *cell_layer, const char *title);
void menu_cell_basic_header_draw(GContext *ctx, const Layer *cell_layer, const char *title);
void menu_layer_init(MenuLayer *menu_layer, GRect frame);
Layer *menu_layer_get_layer(MenuLayer *menu_layer);
void menu_layer_set_callbacks(MenuLayer *menu_layer, void *callback_context, MenuLayerCallbacks callbacks);
void menu_layer_set_click_config_onto_window(MenuLayer *menu_layer, struct Window *window);
void menu_layer_set_selected_next(MenuLayer *menu_layer, bool up, MenuRowAlign scroll_align, bool animated);
void menu_layer_set_selected_index(MenuLayer *menu_layer, MenuIndex index, MenuRowAlign scroll_align, bool animated);
void menu_layer_reload_data(MenuLayer *menu_layer);
int16_t menu_index_compare(MenuIndex *a, MenuIndex *b);
void scroll_layer_init(ScrollLayer *scroll_layer, GRect frame);
void scroll_layer_add_child(ScrollLayer *scroll_layer, Layer *child);
void scroll_layer_set_click_config_onto_window(ScrollLayer *scroll_layer, struct Window *window);
void scroll_layer_set_callbacks(ScrollLayer *scroll_layer, ScrollLayerCallbacks callbacks);
void scroll_layer_set_context(ScrollLayer *scroll_layer, void *context);
void scroll_layer_set_content_offset(ScrollLayer *scroll_layer, GPoint offset, bool animated);
GPoint scroll_layer_get_content_offset(ScrollLayer *scroll_layer);
void scroll_layer_set_content_size(ScrollLayer *scroll_layer, GSize size);
GSize scroll_layer_get_content_size(ScrollLayer *scroll_layer);
void scroll_layer_set_frame(ScrollLayer *scroll_layer, GRect frame);
void scroll_layer_scroll_up_click_handler(ClickRecognizerRef recognizer, ScrollLayer *scroll_layer);
void scroll_layer_scroll_down_click_handler(ClickRecognizerRef recognizer, ScrollLayer *scroll_layer);
void simple_menu_layer_init(SimpleMenuLayer *simple_menu, GRect frame, Window *window, const SimpleMenuSection *sections, int num_sections, void *callback_context);
Layer *simple_menu_layer_get_layer(SimpleMenuLayer *simple_menu);
int simple_menu_layer_get_selected_index(SimpleMenuLayer *simple_menu);
void simple_menu_layer_set_selected_index(SimpleMenuLayer *simple_menu, int index, bool animated);
void window_deinit(Window *window);
void window_set_click_config_provider_with_context(Window *window, ClickConfigProvider click_config_provider, void *context);
ClickConfigProvider window_get_click_config_provider(Window *window);
void window_set_window_handlers(Window *window, WindowHandlers handlers);
struct Layer *window_get_root_layer(Window *window);
bool window_get_fullscreen(Window *window);
void window_set_status_bar_icon(Window *window, const GBitmap *icon);
bool window_is_loaded(Window *window);
Window *window_stack_pop(bool animated);
void window_stack_pop_all(const bool animated);
bool window_stack_contains_window(Window *window);
Window *window_stack_get_top_window(void);
Window *window_stack_remove(Window *window, bool animated);
void property_animation_init(struct PropertyAnimation *property_animation, const struct PropertyAnimationImplementation *implementation, void *subject, void *from_value, void *to_value);
void property_animation_update_int16(struct PropertyAnimation *property_animation, const uint32_t time_normalized);
void property_animation_update_gpoint(struct PropertyAnimation *property_animation, const uint32_t time_normalized);
void property_animation_update_grect(struct PropertyAnimation *property_animation, const uint32_t time_normalized);
AppMessageResult app_message_register_callbacks(AppMessageCallbacksNode *callbacks_node);
AppMessageResult app_message_deregister_callbacks(AppMessageCallbacksNode *callbacks_node);
AppMessageResult app_message_out_get(DictionaryIterator **iter_out);
AppMessageResult app_message_out_send(void);
AppMessageResult app_message_out_release(void);
void app_sync_init(struct AppSync *s, uint8_t *buffer, const uint16_t buffer_size, const Tuplet * const keys_and_initial_values, const uint8_t count, AppSyncTupleChangedCallback tuple_changed_callback, AppSyncErrorCallback error_callback, void *context);
void app_sync_deinit(struct AppSync *s);
AppMessageResult app_sync_set(struct AppSync *s, const Tuplet * const keys_and_values_to_update, const uint8_t count);
const Tuple *app_sync_get(const struct AppSync *s, const uint32_t key);
uint32_t dict_calc_buffer_size(const uint8_t tuple_count, ...);
DictionaryResult dict_write_begin(DictionaryIterator *iter, uint8_t * const buffer, const uint16_t size);
DictionaryResult dict_write_data(DictionaryIterator *iter, const uint32_t key, const uint8_t * const data, const uint16_t size);
DictionaryResult dict_write_cstring(DictionaryIterator *iter, const uint32_t key, const char * const cstring);
DictionaryResult dict_write_int(DictionaryIterator *iter, const uint32_t key, const void *integer, const uint8_t width_bytes, const bool is_signed);
DictionaryResult dict_write_uint8(DictionaryIterator *iter, const uint32_t key, const uint8_t value);
DictionaryResult dict_write_uint16(DictionaryIterator *iter, const uint32_t key, const uint16_t value);
DictionaryResult dict_write_uint32(DictionaryIterator *iter, const uint32_t key, const uint32_t value);
DictionaryResult dict_write_int8(DictionaryIterator *iter, const uint32_t key, const int8_t value);
DictionaryResult dict_write_int16(DictionaryIterator *iter, const uint32_t key, const int16_t value);
DictionaryResult dict_write_int32(DictionaryIterator *iter, const uint32_t key, const int32_t value);
uint32_t dict_write_end(DictionaryIterator *iter);
Tuple *dict_read_begin_from_buffer(DictionaryIterator *iter, const uint8_t * const buffer, const uint16_t size);
Tuple *dict_read_next(DictionaryIterator *iter);
Tuple *dict_read_first(DictionaryIterator *iter);
DictionaryResult dict_serialize_tuplets(DictionarySerializeCallback callback, void *context, const uint8_t tuplets_count, const Tuplet * const tuplets);
DictionaryResult dict_serialize_tuplets_to_buffer(const uint8_t tuplets_count, const Tuplet * const tuplets, uint8_t *buffer, uint32_t *size_in_out);
DictionaryResult dict_serialize_tuplets_to_buffer_with_iter(const uint8_t tuplets_count, const Tuplet * const tuplets, DictionaryIterator *iter, uint8_t *buffer, uint32_t *size_in_out);
DictionaryResult dict_write_tuplet(DictionaryIterator *iter, const Tuplet * const tuplet);
uint32_t dict_calc_buffer_size_from_tuplets(const uint8_t tuplets_count, const Tuplet * const tuplets);
DictionaryResult dict_merge(DictionaryIterator *dest, uint32_t *dest_max_size_in_out, DictionaryIterator *source, const bool update_existing_keys_only, const DictionaryKeyUpdatedCallback key_callback, void *context);
Tuple *dict_find(const DictionaryIterator *iter, const uint32_t key);
void action_bar_layer_init(ActionBarLayer *action_bar);
void action_bar_layer_set_context(ActionBarLayer *action_bar, void *context);
void action_bar_layer_set_click_config_provider(ActionBarLayer *action_bar, ClickConfigProvider click_config_provider);
void action_bar_layer_set_icon(ActionBarLayer *action_bar, ButtonId button_id, const GBitmap *icon);
void action_bar_layer_clear_icon(ActionBarLayer *action_bar, ButtonId button_id);
void action_bar_layer_add_to_window(ActionBarLayer *action_bar, struct Window *window);
void action_bar_layer_remove_from_window(ActionBarLayer *action_bar);
void action_bar_layer_set_background_color(ActionBarLayer *action_bar, GColor background_color);
void number_window_init(NumberWindow *numberwindow, const char *label, NumberWindowCallbacks callbacks, void *callback_context);
void number_window_set_label(NumberWindow *numberwindow, const char *label);
void number_window_set_max(NumberWindow *numberwindow, int max);
void number_window_set_min(NumberWindow *numberwindow, int min);
void number_window_set_value(NumberWindow *numberwindow, int value);
void number_window_set_step_size(NumberWindow *numberwindow, int step);
int number_window_get_value(NumberWindow *numberwindow);
void clock_copy_time_string(char *buffer, uint8_t size);
void gbitmap_init_as_sub_bitmap(GBitmap *sub_bitmap, const GBitmap *base_bitmap, GRect sub_rect);
void gbitmap_init_with_data(GBitmap *bitmap, const uint8_t *data);