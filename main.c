#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helper.h"
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
	TIM2->PSC = 4800;
	TIM2->ARR = 1000;
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
	//TIM2->SR &= ~TIM_SR_UIF;

	if ((TIM2->SR & TIM_SR_CC1IF) != 0) {
		TIM2->SR &= ~TIM_SR_CC1IF;
		switch(row){
		case 1: get_char('1');
				break;
		case 2: get_char('2');
				break;
		case 3: get_char('3');
				break;
		case 4: get_char('A');
				break;
		}
	}

	if ((TIM2->SR & TIM_SR_CC2IF) != 0) {
		TIM2->SR &= ~TIM_SR_CC2IF;
		switch(row){
		case 1: get_char('4');
				break;
		case 2: get_char('5');
				break;
		case 3: get_char('6');
				break;
		case 4: get_char('B');
				break;
		}
	}

	if ((TIM2->SR & TIM_SR_CC3IF) != 0) {
		TIM2->SR &= ~TIM_SR_CC3IF;
		switch(row){
		case 1: get_char('7');
				break;
		case 2: get_char('8');
				break;
		case 3: get_char('9');
				break;
		case 4: get_char('C');
				break;
		}
	}

	if ((TIM2->SR & TIM_SR_CC4IF) != 0) {
		TIM2->SR &= ~TIM_SR_CC4IF;
		switch(row){
		case 1: get_char('*');
				break;
		case 2: get_char('0');
				break;
		case 3: get_char('#');
				break;
		case 4: get_char('D');
				break;
		}
	}

	while((GPIOA->IDR & 0xf) != 0);
}

int get_key_press() {
	while(1) {
		for(row = 1; row <= 4; row++){
			GPIOC->BSRR |= 1 << (row - 1);
			nano_wait(1000 * 1000);
			GPIOC->BRR |= 1 << (row - 1);
		}
	}
}

void get_char(char key) {
    keys[12] = key;
    display2(keys);
}


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

void init_RTC(){
    // Enable PWR clock
     RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

     /* Enable the Backup Domain Access */
     PWR_BackupAccessCmd(ENABLE);

     /* LSI Enable */
     RCC_LSICmd(ENABLE);

     /* Wait until the LSI crystal is ready */
     while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET){
     }

     /* Set RTC clock source to LSI */
     RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);


     /* Enable RTC clock */
     RCC_RTCCLKCmd(ENABLE);

     /* Waits until the RTC Time and Date registers are synchronized with RTC APB
      * clock.*/
     if(RTC_WaitForSynchro() == ERROR) {
         return;
     }

     // setup the RTC prescalers and 12-hour mode
     RTC_InitTypeDef init;
     RTC_StructInit(&init);
     init.RTC_HourFormat = RTC_HourFormat_12;
     if(RTC_Init(&init) == ERROR) {
         return;
     }

     RTC_TimeTypeDef startTime = {11, 59, 57, RTC_H12_PM};
     if(RTC_SetTime(RTC_Format_BIN, &startTime) == ERROR) {
         return;
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
	display1(clockTime);
	//nano_wait(100000000);
}

int main(void)
{
    //char time[16];
    init_RTC();
    init_TIM14();
    init_keypad();
    init_TIM2();
    get_key_press();
    //step4(time);

}
