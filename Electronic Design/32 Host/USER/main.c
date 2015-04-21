#include "stm32f10x.h"
#include "sys.h"
#include "LCD.h"
//#include "I2C.h"
#include "stdlib.h"

#define uchar unsigned char
#define uint unsigned int

/*sys:delayms() not precise*/
int time=10;
void LED_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;			 //??LED?? D2
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  	GPIO_Init(GPIOG, &GPIO_InitStructure);
}

void SPI_24L01_Init()
{
;
}

void Timer_Init()
{TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
TIM_DeInit(TIM3);

TIM_TimeBaseStructure.TIM_Period=2000;
TIM_TimeBaseStructure.TIM_Prescaler=35999;
TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
TIM_ClearFlag(TIM3,TIM_FLAG_Update);
TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
TIM_Cmd(TIM3,ENABLE);
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_InitStructure);
}

int main(void)
{
	Timer_Init();
LCD_SPI_Init();
LCD_fInit();
LCD_Clear(0xFFFF);
LCD_ShowString(0,0,"Test of Timer");
while(1);
}



void TIM3_IRQHandler(void)//enter every 1 second
{
TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
LCD_ShowNum(80,144,time,5);
time--;
	if(time<0){
		BACK_COLOR=RED;
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,DISABLE);
		LCD_Clear(RED);
		LCD_ShowString(90,144,"Boom!");
}

}

