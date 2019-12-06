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
int col = 1;
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
int clk = 0;
EXTI_InitTypeDef EXTI_InitStruct;

#define RATE 100000
#define N 1000
short int wavetable[N];
int freq = 0;
int freq2 = 0;
int i = 0;
int j = 0;
int k = 0;
int count = 0;
int offset = 0;
int offset2 = 0;
int step = 100 * N / 100000.0 * (1 << 16);
int step2 = 100 * N / 100000.0 * (1 << 16);
int rep = 0;

const double mario[31] = {523.25, 0, 0, 392,
						0, 0, 329.63, 0,
						0, 440, 0, 493.88,
						0, 466.16, 440, 0,
						392, 659.26, 783.99,
						880, 0, 698.46, 783.99,
						0, 659.26, 0, 523.25,
						587.33, 493.88, 0, 0};

const int noteRepeat[31] = {12, 12, 12, 12,
							12, 12, 12, 12,
							12, 12, 12, 12,
							12, 12, 12, 12,
							9, 9, 9,
							12, 12, 12, 12,
							12, 12, 12, 12,
							12, 12, 12, 12};
