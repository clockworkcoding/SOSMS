
#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
#include <stdlib.h>
#include <string.h>

#define MY_UUID { 0x90, 0x68, 0xCE, 0xBD, 0x2E, 0x92, 0x4D, 0x2A, 0xAC, 0x83, 0xBE, 0xE1, 0x0A, 0xF0, 0xD9, 0x93 }
PBL_APP_INFO(MY_UUID, "SOSMS", "Clockwork Coding", 0, 1, RESOURCE_ID_IMAGE_PLACEHOLDER_ICON,APP_INFO_STANDARD_APP);


Window window;

TextLayer textLayer;

char message[160]="";
char letter[8]="";
char morse[][8] = {"i\0","ii\0","iii\0","iiii\0","iiiii\0","iiiit\0","iiit\0","iiitiit\0","iiitt\0","iit\0","iiti\0","iittii\0","iittit\0","iittt\0","it\0","iti\0","itii\0","itiii\0","itiiti\0","ititi\0","ititit\0","itt\0","itti\0","ittiti\0","ittt\0","itttt\0","itttti\0","t\0","ti\0","tii\0","tii\0","tiiii\0","tiiiit\0","tiiit\0","tiit\0","tiiti\0","tit\0","titi\0","tititi\0","tititt\0","titt\0","titti\0","tittit\0","tt\0","tti\0","ttii\0","ttiii\0","ttiitt\0","ttit\0","ttt\0","tttii\0","tttiii\0","tttti\0","ttttt\0"};
char characters[][2] = {"E\0","I\0","S\0","H\0","5\0","4\0","V\0","$\0","3\0","U\0","F\0","?\0","_\0","2\0","A\0","R\0","L\0","&\0","\"\0","+\0",".\0","W\0","P\0","@\0","J\0","1\0","'\0","T\0","N\0","B\0","D\0","6\0","-\0","=\0","X\0","/\0","K\0","C\0",";\0","!\0","Y\0","(\0",")\0","M\0","G\0","Z\0","7\0",",\0","Q\0","O\0","8\0",":\0","9\0","0\0"," \0"};
int loc = -1;
int length = 0;

// Modify these common button handlers

void up_single_click_handler(ClickRecognizerRef recognizer, Window *window) {
  (void)recognizer;
  (void)window;

	loc =0;
	bool match = false;
	strcat(letter,"\0");
	for(int i = 0;i<54;i++)
	{
		if(letter == morse[i])
		{
			strcat(message,characters[i]);
			match = true;
			break;
		}
	}
	if(!match)
	{
		strcat(message,characters[54]);
	}
	strcpy(letter,"");
  text_layer_set_text(&textLayer, message);
}


void down_single_click_handler(ClickRecognizerRef recognizer, Window *window) {
  (void)recognizer;
  (void)window;

}


void select_single_click_handler(ClickRecognizerRef recognizer, Window *window) {
  (void)recognizer;
  (void)window;

	loc ++;
	strcat(letter,"i");
  	
  text_layer_set_text(&textLayer, letter);
}


void select_long_click_handler(ClickRecognizerRef recognizer, Window *window) {
  (void)recognizer;
  (void)window;
	loc++;
	strcat(letter,"t");

  text_layer_set_text(&textLayer, letter);
}


// This usually won't need to be modified

void click_config_provider(ClickConfig **config, Window *window) {
  (void)window;

  config[BUTTON_ID_SELECT]->click.handler = (ClickHandler) select_single_click_handler;

  config[BUTTON_ID_SELECT]->long_click.handler = (ClickHandler) select_long_click_handler;

  config[BUTTON_ID_UP]->click.handler = (ClickHandler) up_single_click_handler;
  config[BUTTON_ID_UP]->click.repeat_interval_ms = 100;

  config[BUTTON_ID_DOWN]->click.handler = (ClickHandler) down_single_click_handler;
  config[BUTTON_ID_DOWN]->click.repeat_interval_ms = 100;
}


// Standard app initialisation

void handle_init(AppContextRef ctx) {
  (void)ctx;

  window_init(&window, "S.O.S.M.S.");
  window_stack_push(&window, true /* Animated */);

  text_layer_init(&textLayer, window.layer.frame);
  text_layer_set_text(&textLayer, "Hello World");
  text_layer_set_font(&textLayer, fonts_get_system_font(FONT_KEY_GOTHAM_30_BLACK));
  layer_add_child(&window.layer, &textLayer.layer);

	
  // Attach our desired button functionality
  window_set_click_config_provider(&window, (ClickConfigProvider) click_config_provider);
}


void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init
  };
  app_event_loop(params, &handlers);
}
