#include <pebble.h>
#include "score.h"
#include "menu.h"
//#include "roll.c"
//https://github.com/CACE4/Probability-simulator
//#include <pebble.h>

#define NUM_MENU_SECTIONS 1
#define NUM_PLAYERS 6
  
Window *score_window;
static Window *player_list_window;
TextLayer *player_layer;
TextLayer *score_layer;
TextLayer *score_player_layer;
static GFont elegant_font;
static GFont roboto_font;
SimpleMenuLayer *player_list;
SimpleMenuSection s_menu_sections[NUM_MENU_SECTIONS];
SimpleMenuItem s_player_menu_items[NUM_PLAYERS];

int score[6];

int sindex = 0;

void push_list(){
  window_stack_push(player_list_window, true);
}

static void redrawScore(void){
  
  // redraw player
  static char buffer2[25];
  snprintf(buffer2, sizeof(buffer2), "Player %d", sindex + 1);
  text_layer_set_text(score_player_layer, buffer2);
  
  // redraw score
  static char buffer[256];
  snprintf(buffer, sizeof(buffer), "Score: %d", score[sindex]);
  text_layer_set_text(score_layer, buffer);
}

static void menu_select_callback(int index, void *ctx) {
  sindex = index;
  window_stack_push(score_window, true);
  redrawScore();
  //s_player_menu_items[index].subtitle = "You've hit select here!";
  //layer_mark_dirty(simple_menu_layer_get_layer(player_list));
}

static void resetScores(void){
  for (int i = 0; i < 6; i++)
  {
    score[i] = 0;
  }
}

static void redrawPlayers(void){
  int i;
  for (i = 0; i < 6; i++)
  {
    
    // Player Name Handler
    static char namebuffer[6][10];
    snprintf(namebuffer[i], sizeof(namebuffer[i]), "Player %d", i + 1);
    
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Loop index now %d", i);
    printf("Address: %p", namebuffer[i]);
    
    // Player Score Handler
    static char scorebuffer[6][14];
    snprintf(scorebuffer[i], sizeof(scorebuffer[i]), "Score: %d", score[i]);
    
    
    
    s_player_menu_items[i] = (SimpleMenuItem) {
      
      .title = namebuffer[i], // buffer
      .subtitle = scorebuffer[i],
      .callback = menu_select_callback,
      
    };
    
    APP_LOG(APP_LOG_LEVEL_DEBUG, "%s", s_player_menu_items[i].title);
}
}

static void up_click_handler_score(ClickRecognizerRef recognizer, void *context) {
  score[sindex]++;
  redrawScore();
}

static void down_click_handler_score(ClickRecognizerRef recognizer, void *context) {
  score[sindex]--;
  redrawScore();
}

static void select_click_handler_score(ClickRecognizerRef recognizer, void *context) {
  // Does nothing yet
  redrawScore();
}

static void select_long_click_handler_score(ClickRecognizerRef recognizer, void *context){
  score[sindex] = 0;
  redrawScore();
}

/*static void back_click_handler_player(ClickRecognizerRef recognizer, void *context){
  // Need to find a better handler to run redraw players when the main player list is loaded.
  if(window_stack_get_top_window() == score_window){
    window_stack_push(player_list_window, true);
    redrawPlayers();
  }
}*/

static void click_config_provider_score(void *context) {
  // Register the ClickHandlers
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler_score);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler_score);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler_score);
  window_long_click_subscribe(BUTTON_ID_SELECT, 0, select_long_click_handler_score, NULL);
//  window_single_click_subscribe(BUTTON_ID_BACK, back_click_handler_player);
}

void player_list_window_appear(){
  redrawPlayers();
}

void player_list_window_load(){
  s_menu_sections[0] = (SimpleMenuSection) {
    .title = "Players",
    .num_items = NUM_PLAYERS,
    .items = s_player_menu_items,
  };
  Layer *window_layer = window_get_root_layer(player_list_window);
  GRect bounds = layer_get_frame(window_layer);

  player_list = simple_menu_layer_create(bounds, player_list_window, s_menu_sections, NUM_MENU_SECTIONS, NULL);

  layer_add_child(window_get_root_layer(player_list_window), simple_menu_layer_get_layer(player_list)); 
}

void player_list_window_unload(){
  simple_menu_layer_destroy(player_list);
}

void score_window_load(){
  window_set_click_config_provider(score_window, click_config_provider_score);
  
  
  // Player Name Layer
  score_player_layer = text_layer_create(GRect(0, 0, 144, 30)); // Top 30 y
  text_layer_set_text_alignment(score_player_layer, GTextAlignmentCenter);
  text_layer_set_text_color(score_player_layer, GColorBlack);
  text_layer_set_font(score_player_layer, roboto_font);
  
  
  // Player Score Layer
  score_layer = text_layer_create(GRect(0, 30, 144, 118)); // The rest
  text_layer_set_text_alignment(score_layer, GTextAlignmentCenter);
  text_layer_set_text_color(score_layer, GColorBlack);
  text_layer_set_font(score_layer, elegant_font);
  
  layer_add_child(window_get_root_layer(score_window), text_layer_get_layer(score_player_layer)); // Add player name to window
  layer_add_child(window_get_root_layer(score_window), text_layer_get_layer(score_layer)); // Add layer to window
}

void score_window_unload(){
  
}

void handle_init_player() {
  // Fonts
  elegant_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ELEGANT_LUX_18));
  roboto_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_REGULAR_18));
  
  // Score Window
  score_window = window_create();
  window_set_window_handlers(score_window, (WindowHandlers){
    .load = score_window_load,
    .unload = score_window_unload,
    //.appear = player_list_window_appear,
  });

  
  // Player Window
  player_list_window = window_create();
  window_set_window_handlers(player_list_window, (WindowHandlers){
    .load = player_list_window_load,
    .unload = player_list_window_unload,
    .appear = player_list_window_appear,
  });
  
  // Maintanence thingys
  resetScores();
  redrawPlayers();
  //redrawScore();
}

void handle_deinit_player() {
  text_layer_destroy(score_layer);
  window_destroy(score_window);
  window_destroy(player_list_window);
}

/*int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}*/
