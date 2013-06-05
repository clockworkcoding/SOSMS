
#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

#define MY_UUID { 0x90, 0x68, 0xCE, 0xBD, 0x2E, 0x92, 0x4D, 0x2A, 0xAC, 0x83, 0xBE, 0xE1, 0x0A, 0xF0, 0xD9, 0x93 }
PBL_APP_INFO(MY_UUID, "SOSMS", "Clockwork Coding", 0, 2, RESOURCE_ID_IMAGE_PLACEHOLDER_ICON,APP_INFO_STANDARD_APP);

#define MAX_MORSE_LENGTH 	6
#define BUFFER_LENGTH 		50

Window window;

TextLayer textLayer;
TextLayer morseCodeLayer;
TextLayer msgLengthLayer;
TextLayer letterPreviewLayer;
TextLayer sendStatusLayer;

char sendStatus[8]="";
char message[160]="";
char text_buffer[BUFFER_LENGTH +1]="";


char morseLetter[9]=""; //dots and dashes entered
char letter[9]=""; // dots and dashes translated to 'i' and 't'

char letterPreview[2]="";
char msgLength[7]="";

char morse[][8] = {"i\0","ii\0","iii\0","iiii\0","iiiii\0","iiiit\0","iiit\0","iiitiit\0","iiitt\0","iit\0","iiti\0","iittii\0","iittit\0","iittt\0","it\0","iti\0","itii\0","itiii\0","itiiti\0","ititi\0","ititit\0","itt\0","itti\0","ittiti\0","ittt\0","itttt\0","itttti\0","t\0","ti\0","tiii\0","tii\0","tiiii\0","tiiiit\0","tiiit\0","tiit\0","tiiti\0","tit\0","titi\0","tititi\0","tititt\0","titt\0","titti\0","tittit\0","tt\0","tti\0","ttii\0","ttiii\0","ttiitt\0","ttit\0","ttt\0","tttii\0","tttiii\0","tttti\0","ttttt\0"};
char characters[][2] = {"E\0","I\0","S\0","H\0","5\0","4\0","V\0","$\0","3\0","U\0","F\0","?\0","_\0","2\0","A\0","R\0","L\0","&\0","\"\0","+\0",".\0","W\0","P\0","@\0","J\0","1\0","'\0","T\0","N\0","B\0","D\0","6\0","-\0","=\0","X\0","/\0","K\0","C\0",";\0","!\0","Y\0","(\0",")\0","M\0","G\0","Z\0","7\0",",\0","Q\0","O\0","8\0",":\0","9\0","0\0"," \0"};
char letters[] = "EISH54V$3UF?_2ARL&\"+.WP@J1'TNBD6-=X/KC;!Y()MGZ7,QO8:90 ";

int length = 0;



void reverse(char s[])
{
    int i, j;
    char c;

    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}
void itoa(int n, char s[])
{
    int i, sign;

    if ((sign = n) < 0)  /* record sign */
        n = -n;          /* make n positive */
    i = 0;
    do {       /* generate digits in reverse order */
        s[i++] = n % 10 + '0';   /* get next digit */
    } while ((n /= 10) > 0);     /* delete it */
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}

int findLetter()
{
	strcat(letter,"\0");
	for(int i = 0;i<54;i++)
	{
		if(strncmp(letter, morse[i],7)==0)
		{
			return i;
		}
	}
	return 54;//space
}

void updateMessage()
{
	itoa(length,msgLength);
	strcat(msgLength,"/160");
	text_layer_set_text(&msgLengthLayer, msgLength);
	if(length > BUFFER_LENGTH)
	{ 
		memcpy( text_buffer, &message[length-BUFFER_LENGTH], BUFFER_LENGTH );
		text_buffer[BUFFER_LENGTH] = '\0';
	}
	else
	{
		strcpy(text_buffer,message);
	}
  	text_layer_set_text(&textLayer, text_buffer);
}

void updateMorse()
{
	letterPreview[0] = letters[findLetter()];
	text_layer_set_text(&letterPreviewLayer, letterPreview);
  	text_layer_set_text(&morseCodeLayer, morseLetter);
}

void resetMorse()
{
	strcpy(letter,"");
	strcpy(morseLetter,"");
	updateMorse();
}
// Modify these common button handlers

void select_single_click_handler(ClickRecognizerRef recognizer, Window *window) {
  (void)recognizer;
  (void)window;
	if(length<160)
	{ 
		message[length++] =letters[findLetter()];
		resetMorse();
		updateMessage();
	}
}

void select_long_click_handler(ClickRecognizerRef recognizer, Window *window) {
	//send message to phone
	strcpy(sendStatus,"Sending");
	text_layer_set_text(&sendStatusLayer, sendStatus);
	
	message[length+1]='\0';
	// CString + key:
	static const uint32_t SOME_STRING_KEY = 0x02;
	const char * messageOut = message;
	// Calculate the buffer size that is needed for the final Dictionary:
	const uint8_t key_count = 1;
	const uint32_t size = dict_calc_buffer_size(key_count, strlen(messageOut) + 1);
	// Stack-allocated buffer in which to create the Dictionary:
	uint8_t buffer[size];
	// Iterator variable, keeps the state of the creation serialization process:
	DictionaryIterator *iter2;
	
	DictionaryIterator **iter = &iter2;
	
	app_message_out_get(iter);	
	
	// Begin:
//	dict_write_begin(*iter, buffer, sizeof(buffer));
	
	// Write the CString:
	dict_write_cstring(*iter, SOME_STRING_KEY, messageOut);
	// End:
	const uint32_t final_size = dict_write_end(*iter);
	
	app_message_out_send();
	app_message_out_release();
}


void up_single_click_handler(ClickRecognizerRef recognizer, Window *window) {
  (void)recognizer;
  (void)window;
	
	if (strlen(morseLetter) > 0)
	{
		letter[strlen(morseLetter)-1]='\0';
		morseLetter[strlen(morseLetter)-1]='\0';
		updateMorse();
	}

}

void up_long_click_handler(ClickRecognizerRef recognizer, Window *window) {
  (void)recognizer;
  (void)window;
	
	if (length > 0)
	{
		length--;
		message[length]='\0';
		updateMessage();
	}

}

void down_single_click_handler(ClickRecognizerRef recognizer, Window *window) {
  (void)recognizer;
  (void)window;

	if(strlen(morseLetter) >= MAX_MORSE_LENGTH)
	{
		resetMorse();
	}
	else
	{
		strcat(letter,"i");
		strcat(morseLetter,".");
	}
	updateMorse();
}


void down_long_click_handler(ClickRecognizerRef recognizer, Window *window) {
  (void)recognizer;
  (void)window;
	if(strlen(morseLetter) >= MAX_MORSE_LENGTH)
	{
		resetMorse();
	}
	else
	{
		strcat(letter,"t");
		strcat(morseLetter,"-");
	}
	updateMorse();
}

void my_out_sent_handler(DictionaryIterator *sent, void *context) {
  // outgoing message was delivered
	strcpy(sendStatus,"Sent");
	text_layer_set_text(&sendStatusLayer, sendStatus);
}
void my_out_fail_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
  // outgoing message failed
	strcpy(sendStatus,"Failed");
	text_layer_set_text(&sendStatusLayer, sendStatus);
}
void my_in_rcv_handler(DictionaryIterator *received, void *context) {
  // incoming message received
}
void my_in_drp_handler(void *context, AppMessageResult reason) {
  // incoming message dropped
}


// This usually won't need to be modified

void click_config_provider(ClickConfig **config, Window *window) {
  (void)window;

  	config[BUTTON_ID_SELECT]->click.handler = (ClickHandler) select_single_click_handler;

  	config[BUTTON_ID_SELECT]->long_click.handler = (ClickHandler) select_long_click_handler;

  	config[BUTTON_ID_UP]->click.handler = (ClickHandler) up_single_click_handler;
	
  	config[BUTTON_ID_UP]->long_click.handler = (ClickHandler) up_long_click_handler;

  	config[BUTTON_ID_DOWN]->click.handler = (ClickHandler) down_single_click_handler;
	
	config[BUTTON_ID_DOWN]->long_click.handler = (ClickHandler) down_long_click_handler;
}


// Standard app initialisation

void handle_init(AppContextRef ctx) {
  (void)ctx;

  window_init(&window, "SOSMS");
  window_stack_push(&window, true /* Animated */);

  text_layer_init(&textLayer, GRect(2, 0, 140, 80));
  text_layer_set_text(&textLayer, "_");
  text_layer_set_background_color(&textLayer, GColorClear);
  text_layer_set_text_color(&textLayer, GColorBlack);
  text_layer_set_font(&textLayer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  layer_add_child(&window.layer, &textLayer.layer);

  
  text_layer_init(&morseCodeLayer,GRect(10, 113, 100, 40));
  text_layer_set_text(&morseCodeLayer, "... -- ...");
  text_layer_set_font(&morseCodeLayer, fonts_get_system_font(FONT_KEY_GOTHAM_30_BLACK));
  text_layer_set_background_color(&morseCodeLayer, GColorClear);
  text_layer_set_text_color(&morseCodeLayer, GColorBlack);
  layer_add_child(&window.layer, &morseCodeLayer.layer);
  

  text_layer_init(&sendStatusLayer, GRect(10, 80, 49, 30));
  text_layer_set_text(&sendStatusLayer, "");
  text_layer_set_background_color(&sendStatusLayer, GColorClear);
  text_layer_set_text_color(&sendStatusLayer, GColorBlack);
  text_layer_set_font(&sendStatusLayer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  layer_add_child(&window.layer, &sendStatusLayer.layer);
	
	
  text_layer_init(&msgLengthLayer, GRect(90, 80, 49, 30));
  text_layer_set_text(&msgLengthLayer, "0/160");
  text_layer_set_background_color(&msgLengthLayer, GColorClear);
  text_layer_set_text_color(&msgLengthLayer, GColorBlack);
  text_layer_set_font(&msgLengthLayer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_text_alignment(&msgLengthLayer, GTextAlignmentRight);
  layer_add_child(&window.layer, &msgLengthLayer.layer);
	
  text_layer_init(&letterPreviewLayer,GRect(120, 123, 144-40, 30));
  text_layer_set_text(&letterPreviewLayer, " ");
  text_layer_set_font(&letterPreviewLayer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_background_color(&letterPreviewLayer, GColorClear);
  text_layer_set_text_color(&letterPreviewLayer, GColorBlack);
  layer_add_child(&window.layer, &letterPreviewLayer.layer);

  // Attach our desired button functionality
  window_set_click_config_provider(&window, (ClickConfigProvider) click_config_provider);
}


void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
  
		.messaging_info = {
			.buffer_sizes = {
				.inbound = 124,
				.outbound = 256,
			},
			.default_callbacks.callbacks = {
				.out_sent = my_out_sent_handler,
				.out_failed = my_out_fail_handler,
				.in_received = my_in_rcv_handler,
				.in_dropped = my_in_drp_handler,
			}
	}
		};
  app_event_loop(params, &handlers);
}
