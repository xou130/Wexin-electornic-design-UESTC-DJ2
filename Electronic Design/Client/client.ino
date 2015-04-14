 unsigned char  fseg[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
 unsigned char  segbit[]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
 unsigned char  disbuf[8]={0,0,0,0,0,0,0,0};
 #define uchar unsigned char
 void LED4_Display (void);
 void LED_OUT(uchar X);				// LED单字节串行移位函数
uchar  LED_0F[] = 
{// 0	 1	  2	   3	4	 5	  6	   7	8	 9	  A	   b	C    d	  E    F    -
	0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x8C,0xBF,0xC6,0xA1,0x86,0xFF,0xbf
};


 // 全局变量
uchar LED[8];	//用于LED的8位显示缓存
char DIO=0;
char SCLK=1;
char RCLK=2;

void setup()
{

	pinMode(DIO,OUTPUT);
	pinMode(SCLK,OUTPUT);
	pinMode(RCLK,OUTPUT);
	

	LED[0]=100
	;
	LED[1]=2;
	LED[2]=3;
	LED[3]=4;
	LED[4]=5;
	LED[5]=6;
	LED[6]=7;
	LED[7]=8;

	while(1)
		{LED4_Display ();
	}	

}
void loop()
{
	

	
}
void LED4_Display (void)
{
	uchar  *led_table;          // 查表指针
	uchar i;
	//显示第1位
	led_table = LED_0F + LED[0];
	i = *led_table;

	LED_OUT(i);			
	LED_OUT(0x01);		

	digitalWrite(RCLK,HIGH);
	
	digitalWrite(RCLK,LOW);
	//显示第2位
	led_table = LED_0F + LED[1];
	i = *led_table;

	LED_OUT(i);		
	LED_OUT(0x02);		
	digitalWrite(RCLK,HIGH);
	
	digitalWrite(RCLK,LOW);
	//显示第3位
	led_table = LED_0F + LED[2];
	i = *led_table;

	LED_OUT(i);			
	LED_OUT(0x04);	
	digitalWrite(RCLK,HIGH);
	
	digitalWrite(RCLK,LOW);
	//显示第4位
	led_table = LED_0F + LED[3];
	i = *led_table;

	LED_OUT(i);			
	LED_OUT(0x08);		

	digitalWrite(RCLK,HIGH);
	
	digitalWrite(RCLK,LOW);;
	//显示第5位
	led_table = LED_0F + LED[4];
	i = *led_table;

	LED_OUT(i);			
	LED_OUT(0x10);	
	digitalWrite(RCLK,HIGH);
	
	digitalWrite(RCLK,LOW);
	//显示第6位
	led_table = LED_0F + LED[5];
	i = *led_table;

	LED_OUT(i);			
	LED_OUT(0x20);	

	digitalWrite(RCLK,HIGH);
	
	digitalWrite(RCLK,LOW);
	//显示第7位
	led_table = LED_0F + LED[6];
	i = *led_table;

	LED_OUT(i);			
	LED_OUT(0x40);	

	digitalWrite(RCLK,HIGH);
	
	digitalWrite(RCLK,LOW);
	//显示第8位
	led_table = LED_0F + LED[7];
	i = *led_table;

	LED_OUT(i);		
	LED_OUT(0x80);	
	digitalWrite(RCLK,HIGH);
	
	digitalWrite(RCLK,LOW);
}
void LED_OUT(uchar X)
{
	uchar i;
	for(i=8;i>=1;i--)
	{
		if (X&0x80) digitalWrite(0,HIGH); else digitalWrite(0,LOW);
		X<<=1;
	digitalWrite(1,LOW);

	digitalWrite(1,HIGH);
	}
}
