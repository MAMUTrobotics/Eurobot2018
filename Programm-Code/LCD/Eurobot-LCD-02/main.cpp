#include "main.h"
#include "menu.h"

u16 counter = 0;
u16 counter_s = 0;
uint8_t stunden, minuten, sekunden;			
char text_buffer[100];
u8 speed_update = 1;

var test1(v_u_char);
var test2(v_s_char);
var test3(v_bool);
var test4(v_team);
char test1_ee EEMEM;
char test2_ee EEMEM;
char test3_ee EEMEM;
char test4_ee EEMEM;

var anzeige_counter(v_bool);

// -------------------------16-Bit Register Definition----------------------
uint16_t var_u_int[1];	//Count U_INT
var test5(v_u_int);

int16_t var_s_int[2];	//Count INT
var test6(v_s_int);
var punkte(v_s_int);
int16_t punkte_ee EEMEM;

// -------------------------16-Bit Register Initialisierung-----------------
void init_vars(){
	//U_INT
	test5.value = 0;
	//INT
	test6.value = 0;
	#define punkte_array_num 1
	punkte.value = punkte_array_num;
}

int16_t get_i(var val){
	return var_s_int[val.value];
}
uint16_t get_u(var val){
	return var_u_int[val.value];
}

var Stunden(v_u_char_ro);
var Minuten(v_u_char_ro);
var Sekunden(v_u_char_ro);
var * last_value;
const Menu::Item_t *active_menu_item;

var Start(v_bool_ro);

// -------------------------EEPROM-Mapping----------------------------------
void load_ee(){
	test1.value = eeprom_read_byte ((uint8_t*)&test1_ee);
	test2.value = eeprom_read_byte ((uint8_t*)&test2_ee);
	test3.value = eeprom_read_byte ((uint8_t*)&test3_ee);
	test4.value = eeprom_read_byte ((uint8_t*)&test4_ee);
	var_s_int[punkte_array_num] = eeprom_read_word((uint16_t*)&punkte_ee);

}
void save_ee(){
	eeprom_update_byte((uint8_t*)&test1_ee, test1.value);
	eeprom_update_byte((uint8_t*)&test2_ee, test2.value);
	eeprom_update_byte((uint8_t*)&test3_ee, test3.value);
	eeprom_update_byte((uint8_t*)&test4_ee, test4.value);
	eeprom_update_word((uint16_t*)&punkte_ee, (uint16_t)var_s_int[punkte_array_num]);
}
// ----------------------------------------------------------------------------
Menu::Engine *engine;
bool updateMenu = false;
// ----------------------------------------------------------------------------

// -------------------------I2C-Register-Mapping-------------------------------
void init_pointer(){
	txbuffer[0] = (volatile uint8_t *)&stunden;
	txbuffer[1] = (volatile uint8_t *)&minuten;
	txbuffer[2] = (volatile uint8_t *)&sekunden;
	txbuffer[3] = (volatile uint8_t *)&test1.value;
	txbuffer[4] = (volatile uint8_t *)&test2.value;
	txbuffer[5] = (volatile uint8_t *)&test3.value;
	txbuffer[6] = (volatile uint8_t *)&test4.value;
	txbuffer[7] = (volatile uint8_t *)&Start.value;
	txbuffer[8] = (volatile uint8_t *)&PIND;

	rxbuffer[0] = (volatile uint8_t *)&updateMenu;
	rxbuffer[3] = (volatile uint8_t *)&test1.value;
	rxbuffer[4] = (volatile uint8_t *)&test2.value;
	rxbuffer[5] = (volatile uint8_t *)&test3.value;
	rxbuffer[6] = (volatile uint8_t *)&test4.value;
	rxbuffer[7] = (volatile uint8_t *)&Start.value;
}

// ----------------------------------------------------------------------------
// helpers

#define clampValue(val, lo, hi) if (val > hi) val = hi; if (val < lo) val = lo;
#define maxValue(a, b) ((a > b) ? a : b)
#define minValue(a, b) ((a < b) ? a : b)

void * operator new(size_t size){
	return malloc(size);
}

void operator delete(void * ptr){
	free(ptr);
}

// ----------------------------------------------------------------------------


namespace State {
	typedef enum SystemMode_e {
		None      = 0,
		Default   = (1<<0),
		Settings  = (1<<1),
		Edit      = (1<<2)
	} SystemMode;
};

uint8_t systemState = State::Default;
uint8_t previousSystemState = State::None;

// ----------------------------------------------------------------------------

bool menuExit(const Menu::Action_t a) {
	systemState = State::Default;
	clear_buffer(buffer);
	speed_update = 1;
	return true;
}
bool menuExit2(const Menu::Action_t a) {
	if (a == Menu::actionDisplay) {
		systemState = State::Default;
		clear_buffer(buffer);
		speed_update = 1;
	}	
	return true;
}

bool menuDummy(const Menu::Action_t a) {
	return true;
}

bool menuBack(const Menu::Action_t a) {
	if (a == Menu::actionDisplay) {
		engine->navigate(engine->getParent(engine->getParent()));
	}
	return true;
}

bool menuEdit(const Menu::Action_t a) {
	if (a == Menu::actionDisplay) {
		systemState = State::Edit;
		speed_update = 1;
	}
	return true;
}

bool menuSave(const Menu::Action_t a) {
	if (a == Menu::actionDisplay) {
		save_ee();
	}
	return true;
}


// ----------------------------------------------------------------------------

uint8_t menuItemsVisible = 8;
uint8_t menuItemHeight = 8;

void renderMenuItem(const Menu::Item_t *menu_item, uint8_t pos) {
	//ScopedTimer tm("  render menuitem");

	cursor_y = pos;
	cursor_x = 0;
	if(engine->currentItem == menu_item){
		active_menu_item = menu_item;
		printf(">>");
	}
	cursor_x = 12;
	printf("%s",engine->getLabel(menu_item));
	// mark items that have children
	if (engine->getChild(menu_item) != &Menu::NullItem) {
		cursor_x = 110;
		printf(" >>");
	}else if (engine->getValue(menu_item) != NULL) {
		cursor_x = 86;//92;
		if(systemState == State::Edit && engine->currentItem == menu_item){
			printf(">");
			last_value = engine->getValue(menu_item);
		}else{
			printf(" ");
		}
		switch((*engine->getValue(menu_item)).type){
			case v_u_char:
			case v_u_char_ro:
				printf("%4u", (*engine->getValue(menu_item)).value);
				break;
			case v_s_char:
			case v_s_char_ro:
				char num;
				num = (*engine->getValue(menu_item)).value;
				if(num<128)
					printf("%4i", num);
				else
					printf("%4i", num-256);
				break;
			case v_bool:
			case v_bool_ro: //read-only
				if((*engine->getValue(menu_item)).value)
					printf("true");
				else
					printf("false");
				break;
			case v_team:
				//Show the team color
				if((*engine->getValue(menu_item)).value)
					printf("green");
				else
					printf("orange");
				break;
			case v_u_int:
				printf("%6u", var_u_int[(*engine->getValue(menu_item)).value]);
				break;
			case v_s_int:
				printf("%6i", var_s_int[(*engine->getValue(menu_item)).value]);
				break;
		}
		
	}
}

// ----------------------------------------------------------------------------

//		 Name,			Label,			Next,			Previous,		Parent,			Child,			Callback,	Variable
MenuItem(miExit2,		"exit",			Menu::NullItem,	Menu::NullItem,	Menu::NullItem,	miSettings,		menuExit,	NULL);
MenuItem(miExit,		"exit",			miExit2,		miExit2,		miExit2,		miSettings,		menuExit,	NULL);

MenuItem(miSettings,	"Settings",		miTest1,		Menu::NullItem,	miExit,			miCalibrateLo,	menuDummy,	NULL);

MenuItem(miCalibrateLo,	"Calibrate Lo",	miCalibrateHi,	Menu::NullItem,	miSettings,		Menu::NullItem,	menuDummy,	NULL);
MenuItem(miCalibrateHi,	"Calibrate Hi",	miChannel0,		miCalibrateLo,	miSettings,		Menu::NullItem,	menuDummy,	NULL);

MenuItem(miChannel0,	"Channel 0",	miTime,			miCalibrateHi,	miSettings,		miChView0,		menuDummy,	NULL);
MenuItem(miChView0,		"Ch0:View",		miChScale0,		Menu::NullItem,	miChannel0,		Menu::NullItem,	menuDummy,	NULL);
MenuItem(miChScale0,	"Ch0:Scale",	Menu::NullItem,	miChView0,		miChannel0,		Menu::NullItem,	menuDummy,	NULL);

MenuItem(miTime,		"Zeit",			Menu::NullItem,	miChannel0,		miSettings,		miStunden,		menuDummy,	NULL);
MenuItem(miStunden,		"Stunden",		miMinuten,		Menu::NullItem,	miTime,			Menu::NullItem,	menuDummy,	&Stunden);
MenuItem(miMinuten,		"Minuten",		miSekunden,		miStunden,		miTime,			Menu::NullItem,	menuDummy,	&Minuten);
MenuItem(miSekunden,	"Sekunden",		miChBack1,		miMinuten,		miTime,			Menu::NullItem,	menuDummy,	&Sekunden);
MenuItem(miChBack1,		"Back",			Menu::NullItem,	miSekunden,		miTime,			Menu::NullItem,	menuBack,	NULL);

MenuItem(miTest1,		"Start",		miTest2,		miSettings,		miExit,			Menu::NullItem,	menuDummy,	&Start);
MenuItem(miTest2,		"Test Edit U8",	miTest3,		miTest1,		miExit,			Menu::NullItem,	menuEdit,	&test1);
MenuItem(miTest3,		"Test Edit S8",	miTest4,		miTest2,		miExit,			Menu::NullItem,	menuEdit,	&test2);
MenuItem(miTest4,		"Test EditU16",	miTest5,		miTest3,		miExit,			Menu::NullItem,	menuEdit,	&test5);
MenuItem(miTest5,		"Test EditS16",	miTestpunkte,	miTest4,		miExit,			Menu::NullItem,	menuEdit,	&test6);
MenuItem(miTestpunkte,	"Test Punkte",	miTestcounter,	miTest5,		miExit,			Menu::NullItem,	menuEdit,	&punkte);
MenuItem(miTestcounter,	"Test Counter",	miTest6,		miTestpunkte,	miExit,			Menu::NullItem,	menuEdit,	&anzeige_counter);
MenuItem(miTest6,		"Backlight",	miTest7,		miTestcounter,	miExit,			Menu::NullItem,	menuEdit,	&test3);
MenuItem(miTest7,		"Team-color",	miTest8,		miTest6,		miExit,			Menu::NullItem,	menuEdit,	&test4);
MenuItem(miTest8,		"Exit",			miTest9,		miTest7,		miExit,			Menu::NullItem,	menuExit2,	NULL);
MenuItem(miTest9,		"Save",			Menu::NullItem,	miTest8,		miExit,			Menu::NullItem,	menuSave,	NULL);

// ----------------------------------------------------------------------------

int main(void) {
	wdt_enable(WDTO_8S);
	wdt_reset();
	cli();
	load_ee();
	init_vars();
	// turn on backlight
	BLA_DDR = 1;
	//BLA_OUT = 1;
	if(test3.value)
		BLA_OUT = 1;
	else
		BLA_OUT = 0;
	
	//stdout = uart_file_stream();
	stdout = lcd_file_stream();
	lcd_buffer = buffer;
	usart_init_intr();
    Timer_io_init();
    entprellen	btn_up		(&PIND,  5);
    entprellen	btn_down	(&PIND,  6);
    entprellen	btn_left	(&PIND,  3);
    entprellen	btn_right	(&PIND,  4);
    entprellen	btn_ok		(&PIND,  2);
    entprellen	btn_start	(&PIND,  7);

    init_twi_slave(SLAVE_ADRESSE);			//TWI als Slave mit Adresse slaveadr starten 
    init_pointer();

	st7565_init();
	st7565_command(CMD_DISPLAY_ON);
	st7565_command(CMD_SET_ALLPTS_NORMAL);
	st7565_set_brightness(0x10);
	clear_screen();

	engine = new Menu::Engine(&Menu::NullItem);
	menuExit(Menu::actionDisplay); // reset to initial state
	
	wdt_reset();
	clear_buffer(buffer);
	
	for(uint8_t i = 0; i < 8; i++){
		cursor_x = 70;
		cursor_y = i;
		printf("line: %i",i);
	}

	write_buffer(buffer);
	
	sei();
	wdt_enable(WDTO_2S);
	while(1){
		wdt_reset();
		if(flag_1ms){
			flag_1ms=0;
			btn_up.update();
			btn_down.update();
			btn_left.update();
			btn_right.update();
			btn_ok.update();
			btn_start.update();

			if(btn_up.rised()){
				usart_puts("UP\n");
				if (systemState == State::Settings) {
					engine->navigate(engine->getPrev());
					updateMenu = true;
				}else if(systemState == State::Edit){
					switch((*last_value).type){
						case v_u_char:
						case v_s_char:
							(*last_value).value++;
							break;
						case v_bool:
						case v_team:
							(*last_value).value = !(*last_value).value;
							break;
						case v_u_int:
							var_u_int[(*last_value).value]++;
							break;
						case v_s_int:
							var_s_int[(*last_value).value]++;
							break;
							
					}
					updateMenu = true;
				}
			}
			if(btn_down.rised()){
				usart_puts("DOWN\n");
				if (systemState == State::Settings) {
					engine->navigate(engine->getNext());
					updateMenu = true;
				}else if(systemState == State::Edit){
					switch((*last_value).type){
						case v_u_char:
						case v_s_char:
							(*last_value).value--;
							break;
						case v_bool:
						case v_team:
							(*last_value).value = !(*last_value).value;
							break;
						case v_u_int:
							var_u_int[(*last_value).value]--;
							break;
						case v_s_int:
							var_s_int[(*last_value).value]--;
							break;
					}
					updateMenu = true;
				}
			}
			if(btn_left.rised()){
				usart_puts("LEFT\n");
				if (systemState == State::Settings) {
					engine->navigate(engine->getParent());
					updateMenu = true;
				}else if(systemState == State::Edit){
					switch((*last_value).type){
						case v_u_char:
						case v_s_char:
							(*last_value).value -= 10;
							break;
						case v_bool:
						case v_team:
							(*last_value).value = 0;
							break;
						case v_u_int:
							var_u_int[(*last_value).value]-=10;
							break;
						case v_s_int:
							var_s_int[(*last_value).value]-=10;
							break;
					}
					updateMenu = true;
				}
			}
			if(btn_right.rised()){
				usart_puts("RIGHT\n");
				if (systemState == State::Settings) {
					engine->invoke();
					updateMenu = true;
				}else if(systemState == State::Edit){
					switch((*last_value).type){
						case v_u_char:
						case v_s_char:
							(*last_value).value += 10;
							break;
						case v_bool:
						case v_team:
							(*last_value).value = 1;
							break;
						case v_u_int:
							var_u_int[(*last_value).value]+=10;
							break;
						case v_s_int:
							var_s_int[(*last_value).value]+=10;
							break;
					}
					updateMenu = true;
				}
			}
			if(btn_ok.rised()){
				usart_puts("OK\n");
				if (systemState == State::Edit) { 
					systemState = State::Settings;
					previousSystemState = systemState;
					updateMenu = true;
					
				}else if (systemState == State::Settings) {
					engine->invoke();
					updateMenu = true;
				}else{// enter settings menu
					clear_buffer(buffer);
					write_buffer(buffer);
					engine->navigate(&miSettings);

					systemState = State::Settings;
					previousSystemState = systemState;
					updateMenu = true;
				}
			}
			if(btn_start.fallen()){
				usart_puts("START\n");
				drawstring(buffer, 0, 0, (uint8_t *)"START   ");
				write_buffer(buffer);
				Start.value = 1;
				updateMenu = true;
			}
			
			counter++;
			if (counter>1000 || speed_update)
			{
				if(speed_update){
					speed_update = 0;
				}else{
					counter = 0;
					counter_s++;
				}
				stunden  = (counter_s % 86400) / 3600;
				minuten  = (counter_s %  3600) /   60;
				sekunden =  counter_s %    60;
				Stunden.value = stunden;
				Minuten.value = minuten;
				Sekunden.value = sekunden;
				//Always update these entries
				if( active_menu_item == &miStunden	||
					active_menu_item == &miMinuten	||
					active_menu_item == &miSekunden	||
					active_menu_item == &miChBack1	)
						updateMenu = true;
				if (systemState == State::Default) {

					if(anzeige_counter.value){
						static char dir = 0;
						static int16_t i = 0;
						if(dir)
							i--;
						else
							i++;
						if(i>1100) dir = 1;
						if(i<200) dir = 0;
						draw_num_signed(buffer, i);
					}
					else
						draw_num_signed(buffer, get_i(punkte));

					write_buffer(buffer);
				}
			}
		}		
		if (updateMenu) {
			updateMenu = false;
			
			clear_buffer(buffer);
			if(test3.value)
				BLA_OUT = 1;
			else
				BLA_OUT = 0;
			
			// render the menu
			if (systemState == State::Settings){
				//ScopedTimer tm("render menu");
				engine->render(renderMenuItem, menuItemsVisible);
				write_buffer(buffer);
			}
			if (systemState == State::Edit){
				//ScopedTimer tm("render menu");
				engine->render(renderMenuItem, menuItemsVisible);
				write_buffer(buffer);
			}
		}
	};
}