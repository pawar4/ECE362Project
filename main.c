#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helper.h"
#include <math.h>
// These are function pointers.  They can be called like functions
// after you set them to point to other functions.
// e.g.  cmd = bitbang_cmd;
// They will be set by the stepX() subroutines to point to the new
// subroutines you write below.

// Prototypes for subroutines in support.c
void generic_lcd_startup(void);

// This array will be used with dma_display1() and dma_display2() to mix
// commands that set the cursor location at zero and 64 with characters.
//
uint16_t dispmem[34] = {
        0x080 + 0,
        0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220,
        0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220,
        0x080 + 64,
        0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220,
        0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220,
};

//============================================
//Code not written by group
//START
//============================================
void nano_wait(unsigned int n) {
    asm(    "        mov r0,%0\n"
            "repeat: sub r0,#83\n"
            "        bgt repeat\n" : : "r"(n) : "r0", "cc");
}

void generic_lcd_startup(void) {
    nano_wait(100000000); // Give it 100ms to initialize
    cmd(0x38);  // 0011 NF00 N=1, F=0: two lines
    cmd(0x0c);  // 0000 1DCB: display on, no cursor, no blink
    cmd(0x01);  // clear entire display
    nano_wait(6200000); // clear takes 6.2ms to complete
    cmd(0x02);  // put the cursor in the home position
    cmd(0x06);  // 0000 01IS: set display to increment
}

//============================================
//Code not written by group
//END
//============================================

//Keypad setup
void init_keypad() {

    /* Student code goes here */
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

    GPIOC->MODER &= ~(GPIO_MODER_MODER0 | GPIO_MODER_MODER1 | GPIO_MODER_MODER2 | GPIO_MODER_MODER3);
    GPIOC->MODER |= GPIO_MODER_MODER0_0 | GPIO_MODER_MODER1_0 | GPIO_MODER_MODER2_0 | GPIO_MODER_MODER3_0;

    GPIOA->MODER &= ~(GPIO_MODER_MODER5 | GPIO_MODER_MODER1 | GPIO_MODER_MODER2 | GPIO_MODER_MODER3);
    GPIOA->MODER|= GPIO_MODER_MODER5_1 | GPIO_MODER_MODER1_1 | GPIO_MODER_MODER2_1 | GPIO_MODER_MODER3_1;
    GPIOA->AFR[0] |= (2 << (4 * 5)) | (2 << (4 * 1)) | (2 << (4 * 2)) | (2 << (4 * 3));
    GPIOA->PUPDR |= GPIO_PUPDR_PUPDR5_1 | GPIO_PUPDR_PUPDR1_1 | GPIO_PUPDR_PUPDR2_1 | GPIO_PUPDR_PUPDR3_1;
}

void init_TIM2() {
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    //channel 1
    TIM2->CCMR1 |= TIM_CCMR1_CC1S_0;
    TIM2->CCMR1 |= TIM_CCMR1_IC1F_0 | TIM_CCMR1_IC1F_1 | TIM_CCMR1_IC1F_2 | TIM_CCMR1_IC1F_3;
    TIM2->CCER &= ~(TIM_CCER_CC1P | TIM_CCER_CC1NP);
    TIM2->CCMR1 &= ~TIM_CCMR1_IC1PSC;
    TIM2->CCER |= TIM_CCER_CC1E;
    TIM2->DIER |= TIM_DIER_CC1IE;

    //channel 2
    TIM2->CCMR1 |= TIM_CCMR1_CC2S_0;
    TIM2->CCMR1 |= TIM_CCMR1_IC2F_0 | TIM_CCMR1_IC2F_1 | TIM_CCMR1_IC2F_2 | TIM_CCMR1_IC2F_3;
    TIM2->CCER &= ~(TIM_CCER_CC2P | TIM_CCER_CC2NP);
    TIM2->CCMR1 &= ~TIM_CCMR1_IC2PSC;
    TIM2->CCER |= TIM_CCER_CC2E;
    TIM2->DIER |= TIM_DIER_CC2IE;

    //channel 3
    TIM2->CCMR2 |= TIM_CCMR2_CC3S_0;
    TIM2->CCMR2 |= TIM_CCMR2_IC3F_0 | TIM_CCMR2_IC3F_1 | TIM_CCMR2_IC3F_2 | TIM_CCMR2_IC3F_3;
    TIM2->CCER &= ~(TIM_CCER_CC3P | TIM_CCER_CC3NP);
    TIM2->CCMR2 &= ~TIM_CCMR2_IC3PSC;
    TIM2->CCER |= TIM_CCER_CC3E;
    TIM2->DIER |= TIM_DIER_CC3IE;

    //channel 4
    TIM2->CCMR2 |= TIM_CCMR2_CC4S_0;
    TIM2->CCMR2 |= TIM_CCMR2_IC4F_0 | TIM_CCMR2_IC4F_1 | TIM_CCMR2_IC4F_2 | TIM_CCMR2_IC4F_3;
    TIM2->CCER &= ~(TIM_CCER_CC4P | TIM_CCER_CC4NP);
    TIM2->CCMR2 &= ~TIM_CCMR2_IC4PSC;
    TIM2->CCER |= TIM_CCER_CC4E;
    TIM2->DIER |= TIM_DIER_CC4IE;

    TIM2->CR1 |= TIM_CR1_CEN;
    NVIC->ISER[0] |= 1<<TIM2_IRQn;
    NVIC_SetPriority(TIM2_IRQn,1);
}

void TIM2_IRQHandler() {
    int discard __attribute__((unused));
    while((GPIOA->IDR & 0x2E));

    if ((TIM2->SR & TIM_SR_CC1IF) != 0) {
        discard = TIM2->CCR1;
        press = 1;
        switch(col){
        case 1: key = '1';
                value = 1;
                break;
        case 2: key = '2';
                value = 2;
                break;
        case 3: key = '3';
                value = 3;
                break;
        case 4: key = 'A';
                value = 10;
                break;
        }
    }

    if ((TIM2->SR & TIM_SR_CC2IF) != 0) {
        discard = TIM2->CCR2;
        press = 1;
        switch(col){
        case 1: key = '4';
                value = 4;
                break;
        case 2: key = '5';
                value = 5;
                break;
        case 3: key = '6';
                value = 6;
                break;
        case 4: key = 'B';
                value = 11;
                break;
        }
    }

    if ((TIM2->SR & TIM_SR_CC3IF) != 0) {
        discard = TIM2->CCR3;
        press = 1;
        switch(col){
        case 1: key = '7';
                value = 7;
                break;
        case 2: key = '8';
                value = 8;
                break;
        case 3: key = '9';
                value = 9;
                break;
        case 4: key = 'C';
                value = 12;
                break;
        }
    }

    if ((TIM2->SR & TIM_SR_CC4IF) != 0) {
        discard = TIM2->CCR4;
        press = 1;
        switch(col){
        case 1: key = '*';
                value = 14;
                break;
        case 2: key = '0';
                value = 0;
                break;
        case 3: key = '#';
                value = 15;
                break;
        case 4: key = 'D';
                value = 13;
                break;
        }
    }
}


//Get pressed key
void get_key_press() {
    while(1) {
        for(col = 1; col <= 4; col++){
            GPIOC->BSRR |= 1 << (col - 1);
            nano_wait(1000 * 1000);
            GPIOC->BRR |= 1 << (col - 1);

            if (press == 1) {
                press = 0;
                return;
            }
        }
    }
}

void get_char() {
    keys[12] = key;
    display2(keys);
}


//Display SPI setup
void spi_cmd(char b) {
    while((SPI1->SR & SPI_SR_TXE) == 0);
    SPI1->DR = b;

}

void spi_data(char b) {
    while((SPI1->SR & SPI_SR_TXE) == 0);
    SPI1->DR = 0x200 + b;

}

void spi_init_lcd(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    GPIOA->MODER |= GPIO_MODER_MODER15_1;
    GPIOA->MODER &= ~GPIO_MODER_MODER15_0;
    GPIOB->MODER |= GPIO_MODER_MODER3_1 | GPIO_MODER_MODER5_1;
    GPIOB->MODER &= ~(GPIO_MODER_MODER3_0 | GPIO_MODER_MODER5_0);
    GPIOA->AFR[1] &= ~(0xf<<(4*(15-8)));
    GPIOB->AFR[0] &= ~(0xf<<(4*(3)));
    GPIOB->AFR[0] &= ~(0xf<<(4*(5)));

    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    SPI1->I2SCFGR &= ~SPI_I2SCFGR_I2SMOD;
    SPI1->CR1 |= SPI_CR1_MSTR | SPI_CR1_BR;
    SPI1->CR2 = SPI_CR2_SSOE| SPI_CR2_DS_3 | SPI_CR2_DS_0 |SPI_CR2_NSSP;
    SPI1->CR1 |= SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE;
    SPI1->CR1 |= SPI_CR1_SPE;
    //SPI1->CR1 &= ~SPI_CR1_BR;
    //SPI1->CR1 |= SPI_CR1_BR_1 | SPI_CR1_BR_0;
    generic_lcd_startup();
}

void dma_spi_init_lcd(void) {
    spi_init_lcd();
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;
    DMA1_Channel3->CCR &= ~DMA_CCR_EN;
    DMA1_Channel3->CMAR = (uint32_t) dispmem;
    DMA1_Channel3->CPAR = (uint32_t) (&(SPI1->DR));
    DMA1_Channel3->CNDTR = 34;
    DMA1_Channel3->CCR |= DMA_CCR_MSIZE_0;
    DMA1_Channel3->CCR |= DMA_CCR_PSIZE_0;
    DMA1_Channel3->CCR |= DMA_CCR_DIR;
    DMA1_Channel3->CCR &= ~DMA_CCR_PINC;
    DMA1_Channel3->CCR |=DMA_CCR_MINC;
    DMA1_Channel3->CCR &= ~DMA_CCR_PL;
    DMA1_Channel3->CCR &=~DMA_CCR_MEM2MEM;
    SPI1->CR2 |= SPI_CR2_TXDMAEN;
    DMA1_Channel3->CCR |= DMA_CCR_CIRC;
    DMA1_Channel3->CCR |= DMA_CCR_EN;
}

// Display a string on line 1 by copying a string into the
// memory region circularly moved into the display by DMA.
void circdma_display1(const char *s) {
    int x;
    for(x=0; x<16; x+=1){
        if (s[x]){
            dispmem[x+1] = s[x] | 0x200;
        }
        else{
            break;
        }
    }

    while(x<16){
        dispmem[x+1] = 0x220;
        x++;
    }
}

void circdma_display2(const char *s) {
    int x;
    for(x=0; x<16; x+=1){
        if (s[x]){
            dispmem[x+18] = s[x] | 0x200;
        }
        else{
            break;
        }
    }

    while(x<16){
        dispmem[x+18] = 0x220;
        x++;
    }
}


//RTC Time setup
void init_RTC(int hr, int min, int h12) {
     RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
     PWR_BackupAccessCmd(ENABLE);
     RCC_LSICmd(ENABLE);
     while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET){
     }

     RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
     RCC_RTCCLKCmd(ENABLE);
     if(RTC_WaitForSynchro() == ERROR) {
         return;
     }

     RTC_InitTypeDef init;
     RTC_StructInit(&init);
     init.RTC_HourFormat = RTC_HourFormat_12;
     if(RTC_Init(&init) == ERROR) {
         return;
     }

    if (h12 == 'A') {
         RTC_TimeTypeDef startTime = {hr, min, 50, RTC_H12_AM};
         if(RTC_SetTime(RTC_Format_BIN, &startTime) == ERROR) {
             return;
         }
    }
    else if (h12 == 'P') {
        RTC_TimeTypeDef startTime = {hr, min, 50, RTC_H12_PM};
        if(RTC_SetTime(RTC_Format_BIN, &startTime) == ERROR) {
            return;
        }
    }
    else {
        RTC_TimeTypeDef startTime = {11, 59, 57, RTC_H12_PM};
        if(RTC_SetTime(RTC_Format_BIN, &startTime) == ERROR) {
            return;
        }
    }
}

void rtcGetTime(char* timeStr) {
    RTC_TimeTypeDef time;
    RTC_GetTime(RTC_Format_BIN, &time);
    sprintf(timeStr, "%02d:%02d:%02d %s",
            time.RTC_Hours, time.RTC_Minutes,
            time.RTC_Seconds,
            (time.RTC_H12 == RTC_H12_AM ? "AM" : "PM"));

    if (timeStr[9] == 'A')
    {
        timeofday = 1;
    }
    else
    {
        timeofday = 0;
    }
}

void init_TIM14() {
    RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;
    TIM14->PSC = 48 - 1;
    TIM14->ARR = 1000 - 1;
    TIM14->DIER |= TIM_DIER_UIE;

    cmd = spi_cmd;
    data = spi_data;
    display1 = circdma_display1;
    display2 = circdma_display2;
    dma_spi_init_lcd();
    display2(keys);

    TIM14->CR1 |= TIM_CR1_CEN;
    NVIC->ISER[0] = 1<<TIM14_IRQn;
    NVIC_SetPriority(TIM14_IRQn,0);
}

void TIM14_IRQHandler() {
    TIM14->SR &= ~TIM_SR_UIF;
    rtcGetTime(clockTime);
    if (isAlarmOn == 0)
        display1(clockTime);
}


//RTC Alarm setup
void initAlarm(int hr, int min, char h12) {
    PWR_BackupAccessCmd(ENABLE);

    RTC_AlarmCmd(RTC_Alarm_A, DISABLE);
    //RTC_AlarmTypeDef init = {12, 02, 00, RTC_H12_AM};
    if (h12 == 'A') {
        RTC_AlarmTypeDef init = {hr, min, 00, RTC_H12_AM};

        init.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;
        RTC->WPR = 0xCA;
        RTC->WPR = 0x53;
        RTC->CR |= RTC_CR_ALRAIE;
        RTC->WPR = 0xFF;

        RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &init);
        RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
        EXTI->IMR |= EXTI_IMR_MR17;
        EXTI->RTSR |= EXTI_RTSR_TR17;
        NVIC->ISER[0] |= 1 << RTC_IRQn;
    }
    else if (h12 == 'P') {
        RTC_AlarmTypeDef init = {hr, min, 00, RTC_H12_PM};

        init.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;
        RTC->WPR = 0xCA;
        RTC->WPR = 0x53;
        RTC->CR |= RTC_CR_ALRAIE;
        RTC->WPR = 0xFF;

        RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &init);
        RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
        EXTI->IMR |= EXTI_IMR_MR17;
        EXTI->RTSR |= EXTI_RTSR_TR17;
        NVIC->ISER[0] |= 1 << RTC_IRQn;
    }
}

void RTC_IRQHandler(){
    isAlarmOn = 1;
    display1("ALARM!!");
    RTC->ISR &= ~RTC_ISR_ALRAF;
    EXTI->PR |= EXTI_PR_PR17;
    setup_dac();
    TIM15->CR1 |= TIM_CR1_CEN;
    NVIC->ISER[0] |= 1<<TIM15_IRQn;
    inputMath();
}


//Alarm DAC sound setup
void setup_dac() {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    GPIOA->MODER |= GPIO_MODER_MODER4;
    RCC->APB1ENR |= RCC_APB1ENR_DACEN;
    DAC->CR &= ~DAC_CR_EN1;
    DAC->CR |= DAC_CR_TEN1;
    DAC->CR |= DAC_CR_TSEL1;
    DAC->CR |= DAC_CR_EN1;
}

void setup_timer6() {
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
    TIM6->ARR = 10-1;
    TIM6->PSC = 48-1;
    TIM6->DIER |= TIM_DIER_UIE;
    TIM6->CR1 |= TIM_CR1_CEN;

    NVIC->ISER[0] |= 1<<TIM6_DAC_IRQn;
    NVIC_SetPriority(TIM6_DAC_IRQn, 2);
}

void TIM6_DAC_IRQHandler() {
    DAC->SWTRIGR |= DAC_SWTRIGR_SWTRIG1;
    TIM6->SR &= ~TIM_SR_UIF;

    offset += step;
    if ((offset>>16) >= N) {
        offset -= N<<16;
    }
    offset2 += step2;
    if ((offset2>>16) >= N) {
        offset2 -= N<<16;
    }
    int sample = 0;
    sample += wavetable[offset>>16];
    sample += wavetable[offset2>>16];
    sample = sample / 16 + 2048;
    if (sample > 4095) {
        sample = 4095;
    }
    else if (sample < 0) {
        sample = 0;
    }
    DAC->DHR12R1 = sample;
}

void setup_timer16() {
    // Student code goes here...
    RCC->APB2ENR |= RCC_APB2ENR_TIM16EN;
    TIM16->ARR = 10000 - 1;
    TIM16->PSC = 1200 - 1;
    TIM16->DIER |= TIM_DIER_UIE;
    NVIC_SetPriority(TIM16_IRQn, 2);
}

void TIM16_IRQHandler() {
    TIM16->SR &= ~TIM_SR_UIF;
    double array[26] = {783.99, 0, 880, 698.46, 0, 783.99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 932.33, 0, 880, 698.46, 0, 783.99, 0, 0, 0, 0};

    freq = array[i];
    step = freq * N / 100000.0 * (1 << 16);
    if (i >= 26)
        i = 0;
    else
        i++;

    freq2 = array[j];
    step2 = freq2 * N / 100000.0 * (1 << 16);
    if (j >= 26)
        j = 0;
    else
        j++;
    /*if (freq == 500) {
        freq = 1000;
        step = freq * N / 100000.0 * (1 << 16);
    }
    else {
        freq -= 50;
        step = (freq) * N / 100000.0 * (1 << 16);
    }*/
}

void setup_timer15() {
    // Student code goes here...
    RCC->APB2ENR |= RCC_APB2ENR_TIM15EN;
    TIM15->ARR = 10000 - 1;
    TIM15->PSC = 1200 - 1;
    TIM15->DIER |= TIM_DIER_UIE;

}

void TIM15_IRQHandler() {
    TIM15->SR &= ~TIM_SR_UIF;
    double array[26] = {1567.98, 0, 1760, 1396.91, 0, 1567.98, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1864.66, 0, 1760, 1396.91, 0, 1567.98, 0, 0, 0, 0};
    freq2 = array[j];
    step2 = freq2 * N / 100000.0 * (1 << 16);
    if (j >= 26)
        j = 0;
    else
        j++;
    /*if (freq2 == 500) {
        freq2 = 0;
        step2 = freq2 * N / 100000.0 * (1 << 16);
    }
    else {
        freq2 += 50;
        step2 = (freq2) * N / 100000.0 * (1 << 16);
    }*/
}

void init_wavetable(void)
{
    // Student code goes here...
    int x;
    for(x=0; x<N; x++){
        wavetable[x] = 32767 * sin(2 * M_PI * x / N);
    }
}


//Math Question Setup
void init_TIM3() {
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    TIM3->ARR = 1000;
    TIM3->PSC = 48;
    TIM3->CR1 |= TIM_CR1_CEN;
}

void math_eqn(){
    int dig1 = rand() % 10;
    int dig2 = rand() % 10;
    int operation = dig1 % 4;
    char cdig1[10] = "";
    char cdig2[10] = "";

    itoa(dig1, cdig1, 10);
    strcat(eqn, cdig1);

    switch(operation){
        case 0: strcat(eqn, "+");
                answer = dig1 + dig2;
                break;
        case 1: strcat(eqn, "-");
                answer = dig1 - dig2;
                break;
        case 2: strcat(eqn, "*");
                answer = dig1 * dig2;
                break;
        case 3: strcat(eqn, "/");
                answer = dig1 / dig2;
                break;
    }

    itoa(dig2, cdig2, 10);
    strcat(eqn, cdig2);
    strcat(eqn, "=");
    display2(eqn);
}

void inputTime(){
    int tm[4];
    char time[20] = "00:00 AM";
    display1(time);
    int pos = 0;
    int x = 0;
    char h12 = 'A';
    display2("Input Time");

    while(1){

        get_key_press();

        if((value >= 0 && value <= 9) && pos < 5){
            tm[x] = value;
            x++;
            if(pos < 8){
                if(pos == 2) pos++;
                time[pos] = value + '0';
            }
            pos++;
            display1(time);
        }

        if(key == '*'){
           h12 = 'P';
           time[6] = h12;
           display1(time);
           cnt = TIM3->CNT;
           break;
        }else if (key == '#'){
            cnt = TIM3->CNT;
            break;
        }
    }

    int hr = tm[0] * 10 + tm[1];
    int min = tm[2] * 10 + tm[3];
    init_RTC(hr, min, h12);
    isAlarmOn = 0;
}

void inputAlarm(){
    int tm[4];
    char time[20] = "00:00 AM";
    display1(time);
    int pos = 0;
    int x = 0;
    char h12 = 'A';
    display2("Input Alarm");
    while(1){
        get_key_press();

        if((value >= 0 && value <= 9) && pos < 5){
            tm[x] = value;
            x++;
            if(pos < 8){
                if(pos == 2) pos++;
                time[pos] = value + '0';
            }
            pos++;
            display1(time);
        }

        if(key == '*'){
           h12 = 'P';
           time[6] = h12;
           display1(time);
           break;
        }else if (key == '#'){
            break;
        }
    }

    int hr = tm[0] * 10 + tm[1];
    int min = tm[2] * 10 + tm[3];
    initAlarm(hr, min, h12);
    isAlarmOn = 0;
}

void inputMath() {
    srand(cnt);
    int userAns = 0;
    char ans[3];
    int count = 0;

    while(1){
        if(count == 0) math_eqn();
        get_key_press();

        if(key != '#'){
            userAns = userAns * 10 + value;
            ans[count] = value + '0';
            strcat(eqn, ans);
            display2(eqn);
            count++;
        }

        if(key == '#' && userAns == answer){
            display2("Right Answer!!");
            TIM6->CR1 &= ~TIM_CR1_CEN;
            break;
        }else if(key == '#' && userAns != answer){
            display2("Wrong Answer!!");
            nano_wait(1000* 1000*250);
            userAns = 0;
            memset(eqn, 0, strlen(eqn));
            memset(ans, 0, strlen(ans));
            count = 0;
        }
    }

    isAlarmOn = 0;
}

int main(void){
    init_TIM14();
    init_keypad();
    init_TIM2();
    init_wavetable();
    setup_timer6(); //DAC
    setup_timer16(); //Frequency
    setup_timer15(); //Nothing
    init_TIM3();
    inputTime();

    while(1){
        display2("A-Alarm | B-Time");
        get_key_press();
        if(key == 'A'){
            isAlarmOn = 1;
            inputAlarm();
        }else if(key == 'B'){
            inputTime();
        }
    }

}
