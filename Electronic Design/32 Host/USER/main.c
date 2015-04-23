#include "stm32f10x.h"
#include "sys.h"
#include "LCD.h"
//#include "I2C.h"
#include "stdlib.h"
#include "24l01.h"
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
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;			 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  	GPIO_Init(GPIOE, &GPIO_InitStructure);
}	

void Timer_Init()
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
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












/*
u8 current[4];
struct cmds{
u8 time;
u8 operation;
};
struct cmds cmd[4][10];

void dataset(u8 raw)
{
u8 i,op,pos;
op=(raw&3);
i=(raw&0x3c)>>2;
pos=(raw&0xc0)>>6;
switch(pos)
	{
case 0:cmd[0][current[0]].time=i;cmd[0][current[0]++].operation=op; break;
case 1:cmd[1][current[1]].time=i;cmd[1][current[1]++].operation=op; break;
case 2:cmd[2][current[2]].time=i;cmd[2][current[2]++].operation=op; break;
case 3:cmd[3][current[3]].time=i;cmd[3][current[3]++].operation=op; break;
}	
}

void LED_Set(u8,u8);
void datacheck(void)
{
	u8 i;
	for(i=0;i<4;i++)
	if(cmd[i][current[i]].time==0){LED_Set(i,cmd[i][current[i]].operation);current[i]--;}

}
*/






int main(void)
{
	int K1;u8 tmp_buf[33];
	Timer_Init();
	LED_Init();
	LCD_SPI_Init();
	LCD_fInit();
	LCD_Clear(0xFFFF);
	LCD_ShowString(0,0,"Test of Timer & 24L01");
	
	NRF24L01_Init();
	
	LCD_ShowString(0,60,"24L01 Check! Check Data Following:(1=Fail)");
	
	LCD_ShowNum(200,76,NRF24L01_Check(),1);
	
	RX_Mode();
	
	GPIO_SetBits(GPIOG,GPIO_Pin_14);
	while(NRF24L01_Check());
	GPIO_ResetBits(GPIOG,GPIO_Pin_14);
	
while(1){
	K1=GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0);
	if(K1)GPIO_ResetBits(GPIOG,GPIO_Pin_14);
	else GPIO_SetBits(GPIOG,GPIO_Pin_14);

	if(NRF24L01_RxPacket(tmp_buf)==0){
		GPIO_SetBits(GPIOG,GPIO_Pin_14);
		LCD_ShowString(0,200,"Data Recived  \\(^o^)/ :");
	LCD_ShowString(0,216,tmp_buf);
	}else delayms(10);

}

}




void TIM3_IRQHandler(void)//enter every 1 second
{
TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
LCD_ShowNum(80,144,time,5);
time--;
	if(time<0){
		time=10;
	}
	/*
	
	
	*/

}

