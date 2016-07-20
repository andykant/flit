#ifndef __BUTTONS_H
#define __BUTTONS_H
/*
	Button board support
	Microcontroller code for Flit
	Nate Marx
	2016
*/

//#define LED_LCK_PIN 2
//#define BTN_PWR_PIN 4
//#define BTN_NXT_PIN 5
//#define BTN_LCK_PIN 6

//
#define LED_LCK_PIN 24
#define BTN_PWR_PIN 26
#define BTN_NXT_PIN 28
#define BTN_LCK_PIN 30

#define DEB_MILS 50
#define COOLDOWN_MILS 1000

extern bool autoOnOff, rotate,
            btn_pwr_press,
            btn_nxt_press,
            btn_lck_press,
            cooldown_state;

extern long last_btn_mils, cooldown_mils;

void trig_pwr_btn();
void trig_nxt_btn();
void trig_lck_btn();
void buttonSetup();
void buttonCheck();
void buttonSelfTest();
#endif //__BUTTONS_H
