//VCC -- 5V
//GND -- GND
//DIO -- 0
//RCK -- 2
//SCK -- 1
 
 
int latchPin = 2;//RCK
int clockPin = 1;//SCK 
int dataPin = 0; //DIO
//这里定义了74HC595的那三个脚
 
 
void setup ()
{
  pinMode(latchPin,OUTPUT);
  pinMode(clockPin,OUTPUT);
  pinMode(dataPin,OUTPUT); 
  //让74HC595的三个脚都是输出状态
}
 
//这个函数是显示一个数用的disNum(显示的数，显示的位，是否有小数点（1为有小数点）)
void disNum(unsigned char Num_Qua, int Num_Pos, int Num_Dec)
{
    int Num_Qua_B = 0;
    int Num_Pos_B = 0;
    switch (Num_Qua)
    {
                case 0:Num_Qua_B = 0xC0;break;//0
                case 1:Num_Qua_B = 0xF9;break;//1
                case 2:Num_Qua_B = 0xA4;break;//2
                case 3:Num_Qua_B = 0xB0;break;//3
                case 4:Num_Qua_B = 0x99;break;//4
                case 5:Num_Qua_B = 0x92;break;//5
                case 6:Num_Qua_B = 0x82;break;//6
                case 7:Num_Qua_B = 0xF8;break;//7
                case 8:Num_Qua_B = 0x80;break;//8
                case 9:Num_Qua_B = 0x90;break;//9
				case 'A':Num_Qua_B = 0x90;break;//A
				case 'B':Num_Qua_B = 0x90;break;//B
				case 'C':Num_Qua_B = 0x90;break;//C
				case 'D':Num_Qua_B = 0x90;break;//D
				case 'E':Num_Qua_B = 0x90;break;//E
				case 'F':Num_Qua_B = 0x90;break;//F
                default: Num_Qua_B = 0xff;break;//空白
    };
        //这个值转换为2进制 为一个数abcdefg的需要亮的针脚。
        switch (Num_Pos)
    {
                case 1:Num_Pos_B = 1;break;//0000 0001
                case 2:Num_Pos_B = 2;break;//0000 0010
                case 3:Num_Pos_B = 4;break;//0000 0100
                case 4:Num_Pos_B = 8;break;//0000 1000
                case 5:Num_Pos_B = 16;break;//0001 0000
                case 6:Num_Pos_B = 32;break;//0010 0000
                case 7:Num_Pos_B = 64;break;//0100 0000
                case 8:Num_Pos_B = 128;break;//1000 0000
                default: Num_Pos_B = 0;break;//0000 0000
    };
        //这个值转换为2进制 为当前点亮的位数。
    if (Num_Dec == 1)
    {
        Num_Qua_B = Num_Qua_B + 128;
    };
        //128是Q7端口2进制值转换为10进制的结果。
    digitalWrite(latchPin,LOW); //将ST_CP口上面加低电平让芯片准备好接收数据
    int L = Num_Qua_B; int R = Num_Pos_B;
    shiftOut(dataPin,clockPin,MSBFIRST,R);
    shiftOut(dataPin,clockPin,MSBFIRST,L);
    digitalWrite(latchPin,HIGH); //将ST_CP这个针脚恢复到高电平
        //上面是74HC595的输出方式 每个74HC595能接受一个8位的2进制数值来电灯 第一个74HC595再次输入一个8位的数值他会把他现在的数值传给第二个串联的74HC595。
        delayMicroseconds(100);
}
//这个是显示数用的函数
void showNum(double number)//注意用double，int不够长了。
{
        long  num = number;
        int ShowDec [8];
        for (int x = 1; x < 9; x++)
        {
                ShowDec [x] = 0;
        }
        //创建显示小数点的数组，0为不显示，1为显示
 
        if (num  < 10000000){
        if (num  < 1000000){num = number*100; ShowDec[6]=1;ShowDec[7]=0;}
                else{
                num = number*10; ShowDec[7]=1;
                }
        }
        //简单的2位小数显示
 
 
 
 
        int ShowNumber [8];
        ShowNumber[1] = (num/10000000)%10;
        ShowNumber[2]=  (num/1000000)%10;
        ShowNumber[3] = (num/100000)%10;
        ShowNumber[4] = (num/10000)%10;
        ShowNumber[5] = (num/1000)%10;
        ShowNumber[6] = (num/100)%10;
        ShowNumber[7] = (num/10)%10;
        ShowNumber[8]= (num/1)%10;
        //取得每一位的数。然后存在一个数组里。
 
 
 
        int dectest = 0;
        for (int x = 1; x<8; x++)
        {
                if ((dectest == 0)&&(ShowNumber[x] == 0))
                {
                        ShowNumber[x] = 11; 
                }
                else
                {
                        dectest = 1;
                }
        }
        //左侧开始检查是否有0 如果有就不显示（在显示单个数的函数里面设置了如果不是1-9，那么就不显示东西）
 
 
  disNum(ShowNumber[1],1,ShowDec[1]);
  disNum(ShowNumber[2],2,ShowDec[2]);
  disNum(ShowNumber[3],3,ShowDec[3]);
  disNum(ShowNumber[4],4,ShowDec[4]);
  disNum(ShowNumber[5],5,ShowDec[5]);
  disNum(ShowNumber[6],6,ShowDec[6]);
  disNum(ShowNumber[7],7,ShowDec[7]);
  disNum(ShowNumber[8],8,ShowDec[8]);
        //依次显示数字，因为LED的闪烁很快，所以人眼无法识别闪烁。
}
 
void loop()
{
     showNum(13446.8);//相对简单函数调用
}