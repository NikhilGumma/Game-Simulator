#include <pebble.h>
#include "roll.h"
#include "menu.h"
#include "score.h"


#define NUM_MENU_SECTIONS 2
#define NUM_FIRST_MENU_ITEMS 1
#define NUM_SECOND_MENU_ITEMS 2

static Window *s_main_window;
static SimpleMenuLayer *s_simple_menu_layer;
static SimpleMenuSection s_menu_sections[NUM_MENU_SECTIONS];
static SimpleMenuItem s_first_menu_items[NUM_FIRST_MENU_ITEMS];
static SimpleMenuItem s_second_menu_items[NUM_SECOND_MENU_ITEMS];
static GBitmap *s_menu_icon_image;
static GBitmap *s_menu_score_image;

static bool s_special_flag = false;
static int s_hit_count = 0;

//void handle_init_player();
//void handle_deinit_player();
//static Window *player_list_window;

static void menu_select_callback(int index, void *ctx) {

  push_list();
  
}

static void menu_select_callback2(int index, void *ctx) {

  push_roll();

}

static void special_select_callback(int index, void *ctx) {

  push_history();
}

static void main_window_load(Window *window) {
  s_menu_icon_image = gbitmap_create_with_resource(RESOURCE_ID_dice2);
  s_menu_score_image = gbitmap_create_with_resource(RESOURCE_ID_score);
   
  // an int as such to easily change the order of menu items later
  int num_a_items = 0;

  s_first_menu_items[num_a_items++] = (SimpleMenuItem) {
    .icon=s_menu_score_image, 
    .title = "Score Counter",
    .subtitle = "Up to 6 players,",
    .callback = menu_select_callback,
    
  };
  s_second_menu_items[0] = (SimpleMenuItem) {
    .icon=s_menu_icon_image,
    .title = "Dice Simulator",
    .callback = menu_select_callback2,
  };

  s_second_menu_items[1] = (SimpleMenuItem) {
    .title = "Previous Dice Rolls",
    .callback = special_select_callback,
  };
  

  s_menu_sections[0] = (SimpleMenuSection) {
    .num_items = NUM_FIRST_MENU_ITEMS,
    .items = s_first_menu_items,
  };
  s_menu_sections[1] = (SimpleMenuSection) {
    .num_items = NUM_SECOND_MENU_ITEMS,
    .items = s_second_menu_items,
  };
  
//gBitmap_set_bounds(s_menu_icon_image,
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);

  s_simple_menu_layer = simple_menu_layer_create(bounds, window, s_menu_sections, NUM_MENU_SECTIONS, NULL);

  layer_add_child(window_layer, simple_menu_layer_get_layer(s_simple_menu_layer));
}

void main_window_unload(Window *window) {
  simple_menu_layer_destroy(s_simple_menu_layer);
  gbitmap_destroy(s_menu_icon_image);
}

static void init() {
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  window_stack_push(s_main_window, true);
}

static void deinit() {
  window_destroy(s_main_window);
}

int main(void) {
  init();
  handle_init_player();
  init_roll();
  app_event_loop();
  handle_deinit_player();
  deinit_roll();
  deinit();
}