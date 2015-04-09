#include "stm32f10x.h"
#define uchar unsigned char
#define uint unsigned int

void delayms(int m)
{m*=10;while(m--);}

 //LCD size
#define LCD_W 240
#define LCD_H 320


void LED_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;			 //??LED?? D2
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOG, &GPIO_InitStructure);
}


void LCD_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_4);
	GPIO_ResetBits(GPIOB,GPIO_Pin_4);
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_5);
	GPIO_SetBits(GPIOB,GPIO_Pin_5);
	

}//CS=B4,RST=B5,DC=B6,SCL=B7,SDA=B8

void LCD_Writ_Bus(char data)
{
	int cnt;
	for(cnt=0;cnt<8;cnt++)
	{
		if((data<<cnt)&0x80)GPIO_SetBits(GPIOB,GPIO_Pin_8);
			else GPIO_ResetBits(GPIOB,GPIO_Pin_8);
		GPIO_SetBits(GPIOB,GPIO_Pin_7);
	
		GPIO_ResetBits(GPIOB,GPIO_Pin_7);	
	}
}

void LCD_WR_DATA8(char da)
{
	GPIO_SetBits(GPIOB,GPIO_Pin_6);
	LCD_Writ_Bus(da);

}

void LCD_WR_DATA(int da)
{
  GPIO_SetBits(GPIOB,GPIO_Pin_6);
	LCD_Writ_Bus(da>>8);
	LCD_Writ_Bus(da);
}

void LCD_WR_REG(char da)	 
{
  GPIO_ResetBits(GPIOB,GPIO_Pin_6);
	LCD_Writ_Bus(da);
}
 void LCD_WR_REG_DATA(int reg,int da)
{
    LCD_WR_REG(reg);
	LCD_WR_DATA(da);
}

void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{  
   LCD_WR_REG(0x2a);
   LCD_WR_DATA8(x1>>8);
   LCD_WR_DATA8(x1);
   LCD_WR_DATA8(x2>>8);
   LCD_WR_DATA8(x2); 
   LCD_WR_REG(0x2b);
   LCD_WR_DATA8(y1>>8);
   LCD_WR_DATA8(y1);
   LCD_WR_DATA8(y2>>8);
   LCD_WR_DATA8(y2);
   LCD_WR_REG(0x2C);
}

void LCD_Clear(u16 Color)
{
	u8 VH,VL;
	u16 i,j;
	VH=Color>>8;
	VL=Color;	
	Address_set(0,0,LCD_W-1,LCD_H-1);
    for(i=0;i<LCD_W;i++)
	 {
	  for (j=0;j<LCD_H;j++)
	   	{
       LCD_WR_DATA8(VH);
			 LCD_WR_DATA8(VL);	
	    }
	  }
}
void LCD_fInit(void)
{


   	GPIO_SetBits(GPIOB,GPIO_Pin_4);

	   LCD_WR_REG_DATA(0,0);

    GPIO_SetBits(GPIOB,GPIO_Pin_5);
    delayms(5);	
	GPIO_ResetBits(GPIOB,GPIO_Pin_5);
	delayms(5);
	GPIO_SetBits(GPIOB,GPIO_Pin_5);
	GPIO_SetBits(GPIOB,GPIO_Pin_4);
	delayms(5);
	GPIO_ResetBits(GPIOB,GPIO_Pin_4);  

		LCD_WR_REG(0xCB);  
        LCD_WR_DATA8(0x39); 
        LCD_WR_DATA8(0x2C); 
        LCD_WR_DATA8(0x00); 
        LCD_WR_DATA8(0x34); 
        LCD_WR_DATA8(0x02); 

        LCD_WR_REG(0xCF);  
        LCD_WR_DATA8(0x00); 
        LCD_WR_DATA8(0XC1); 
        LCD_WR_DATA8(0X30); 
 
        LCD_WR_REG(0xE8);  
        LCD_WR_DATA8(0x85); 
        LCD_WR_DATA8(0x00); 
        LCD_WR_DATA8(0x78); 
 
        LCD_WR_REG(0xEA);  
        LCD_WR_DATA8(0x00); 
        LCD_WR_DATA8(0x00); 
 
        LCD_WR_REG(0xED);  
        LCD_WR_DATA8(0x64); 
        LCD_WR_DATA8(0x03); 
        LCD_WR_DATA8(0X12); 
        LCD_WR_DATA8(0X81); 

        LCD_WR_REG(0xF7);  
        LCD_WR_DATA8(0x20); 
  
        LCD_WR_REG(0xC0);    //Power control 
        LCD_WR_DATA8(0x23);   //VRH[5:0] 
 
        LCD_WR_REG(0xC1);    //Power control 
        LCD_WR_DATA8(0x10);   //SAP[2:0];BT[3:0] 
 
        LCD_WR_REG(0xC5);    //VCM control 
        LCD_WR_DATA8(0x3e); //?????
        LCD_WR_DATA8(0x28); 
 
        LCD_WR_REG(0xC7);    //VCM control2 
        LCD_WR_DATA8(0x86);  //--
 
        LCD_WR_REG(0x36);    // Memory Access Control 
        LCD_WR_DATA8(0x48); //C8	   //48 68??//28 E8 ??

        LCD_WR_REG(0x3A);    
        LCD_WR_DATA8(0x55); 

        LCD_WR_REG(0xB1);    
        LCD_WR_DATA8(0x00);  
        LCD_WR_DATA8(0x18); 
 
        LCD_WR_REG(0xB6);    // Display Function Control 
        LCD_WR_DATA8(0x08); 
        LCD_WR_DATA8(0x82);
        LCD_WR_DATA8(0x27);  
 
        LCD_WR_REG(0xF2);    // 3Gamma Function Disable 
        LCD_WR_DATA8(0x00); 
 
        LCD_WR_REG(0x26);    //Gamma curve selected 
        LCD_WR_DATA8(0x01); 
 
        LCD_WR_REG(0xE0);    //Set Gamma 
        LCD_WR_DATA8(0x0F); 
        LCD_WR_DATA8(0x31); 
        LCD_WR_DATA8(0x2B); 
        LCD_WR_DATA8(0x0C); 
        LCD_WR_DATA8(0x0E); 
        LCD_WR_DATA8(0x08); 
        LCD_WR_DATA8(0x4E); 
        LCD_WR_DATA8(0xF1); 
        LCD_WR_DATA8(0x37); 
        LCD_WR_DATA8(0x07); 
        LCD_WR_DATA8(0x10); 
        LCD_WR_DATA8(0x03); 
        LCD_WR_DATA8(0x0E); 
        LCD_WR_DATA8(0x09); 
        LCD_WR_DATA8(0x00); 

        LCD_WR_REG(0XE1);    //Set Gamma 
        LCD_WR_DATA8(0x00); 
        LCD_WR_DATA8(0x0E); 
        LCD_WR_DATA8(0x14); 
        LCD_WR_DATA8(0x03); 
        LCD_WR_DATA8(0x11); 
        LCD_WR_DATA8(0x07); 
        LCD_WR_DATA8(0x31); 
        LCD_WR_DATA8(0xC1); 
        LCD_WR_DATA8(0x48); 
        LCD_WR_DATA8(0x08); 
        LCD_WR_DATA8(0x0F); 
        LCD_WR_DATA8(0x0C); 
        LCD_WR_DATA8(0x31); 
        LCD_WR_DATA8(0x36); 
        LCD_WR_DATA8(0x0F); 
 
        LCD_WR_REG(0x11);    //Exit Sleep 
        delayms(120); 
				
        LCD_WR_REG(0x29);    //Display on 
        LCD_WR_REG(0x2c); 

}



int main(void)
{
	LCD_Init();
	LED_Init();
	LCD_fInit();
	LCD_Clear(0x0000);
	GPIO_SetBits(GPIOG,GPIO_Pin_14);
	
	
	
	LCD_Clear(0xAAAA);
}
