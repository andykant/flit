
//debug
#define DEBUG

//BIG GLOBALS
bool rtc_missing=false;
bool rotate=true;
bool autoOnOff=true;
uint8_t globalBrightness=254;
uint8_t globalSpeed=128;
uint8_t breatheHue=0;
uint8_t rainbowWidth=255;
uint8_t rippleCount=128;
uint8_t springSize=200;
uint8_t summerShimmer=128;
uint8_t fallSaturation=255;
uint8_t winterSnowfall=128;
uint8_t flitmode=0;


String controllerInput;
long rotate_time = 0;

//general LED globals
long modeTimer;
bool modeinit;

//ripple
int thisdelay = 10;
uint8_t bgcol = 0;
uint8_t colour;
int center = 0;
int step = -1;

//summer
CRGBPalette16 currentPalette(CloudColors_p);
uint16_t x,y,z;
uint16_t noise_speed = 20;
uint16_t noise_scale = 30;
uint8_t noise[2][31];
uint8_t       colorLoop = 1;



bool  btn_pwr_press,
      btn_nxt_press,
      btn_lck_press,
      cooldown_state;
      
long last_btn_mils, cooldown_mils;


