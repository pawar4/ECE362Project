void init_keypad();
void init_TIM2();
void get_key_press();
void spi_cmd(char b);
void spi_data(char b);
void spi_init_lcd(void);
void dma_spi_init_lcd(void);
void circdma_display1(const char *s);
void circdma_display2(const char *s);
void init_RTC(int hr, int min, int h12);
void rtcGetTime(char* timeStr);
void init_TIM14();
void get_char();
void initAlarm(int hr, int min, char h12);
void inputAlarm();
void inputTime();
void inputMath();

void setup_dac();
void setup_timer6();
void setup_timer16();
void init_wavetable(void);

void (*cmd)(char b) = 0;
void (*data)(char b) = 0;
void (*display1)(const char *) = 0;
void (*display2)(const char *) = 0;
int8_t history[16] = {0};
int8_t lookup[16] = {1,4,7,0xe,2,5,8,0,3,6,9,0xf,0xa,0xb,0xc,0xd};
char char_lookup[16] = {'1','4','7','*','2','5','8','0','3','6','9','#','A','B','C','D'};
int timeofday = 9;
char clockTime[16];
int col = 1;
char keys[16] = {"Key Pressed:"};
char key;
int value;
int press = 0;
int cnt = 0;
int isAlarmOn = 0;
int answer = 0;
char eqn[30] = "";

//Start of manual clock
int hours = 0;
int mins = 0;
int seconds = 0;
char * hr12 = "P";
int hoursA = 0;
int minsA = 0;
int secondsA = 0;
char * hr12A = "P";
int inputAlrm = 0;

#define RATE 100000
#define N 1000
short int wavetable[N];
int freq = 0;
int freq2 = 0;
int i = 0;
int j = 0;
int offset = 0;
int offset2 = 0;
int step = 1000 * N / 100000.0 * (1 << 16);
int step2 = 1000 * N / 100000.0 * (1 << 16);
