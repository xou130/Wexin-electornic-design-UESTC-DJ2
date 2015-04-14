//VCC -- 5V
//GND -- GND
//DIO -- 0
//RCK -- 2
//SCK -- 1
 
 
int latchPin = 2;//RCK
int clockPin = 1;//SCK 
int dataPin = 0; //DIO
//���ﶨ����74HC595����������
 
 
void setup ()
{
  pinMode(latchPin,OUTPUT);
  pinMode(clockPin,OUTPUT);
  pinMode(dataPin,OUTPUT); 
  //��74HC595�������Ŷ������״̬
}
 
//�����������ʾһ�����õ�disNum(��ʾ��������ʾ��λ���Ƿ���С���㣨1Ϊ��С���㣩)
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
                default: Num_Qua_B = 0xff;break;//�հ�
    };
        //���ֵת��Ϊ2���� Ϊһ����abcdefg����Ҫ������š�
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
        //���ֵת��Ϊ2���� Ϊ��ǰ������λ����
    if (Num_Dec == 1)
    {
        Num_Qua_B = Num_Qua_B + 128;
    };
        //128��Q7�˿�2����ֵת��Ϊ10���ƵĽ����
    digitalWrite(latchPin,LOW); //��ST_CP������ӵ͵�ƽ��оƬ׼���ý�������
    int L = Num_Qua_B; int R = Num_Pos_B;
    shiftOut(dataPin,clockPin,MSBFIRST,R);
    shiftOut(dataPin,clockPin,MSBFIRST,L);
    digitalWrite(latchPin,HIGH); //��ST_CP�����Żָ����ߵ�ƽ
        //������74HC595�������ʽ ÿ��74HC595�ܽ���һ��8λ��2������ֵ����� ��һ��74HC595�ٴ�����һ��8λ����ֵ����������ڵ���ֵ�����ڶ���������74HC595��
        delayMicroseconds(100);
}
//�������ʾ���õĺ���
void showNum(double number)//ע����double��int�������ˡ�
{
        long  num = number;
        int ShowDec [8];
        for (int x = 1; x < 9; x++)
        {
                ShowDec [x] = 0;
        }
        //������ʾС��������飬0Ϊ����ʾ��1Ϊ��ʾ
 
        if (num  < 10000000){
        if (num  < 1000000){num = number*100; ShowDec[6]=1;ShowDec[7]=0;}
                else{
                num = number*10; ShowDec[7]=1;
                }
        }
        //�򵥵�2λС����ʾ
 
 
 
 
        int ShowNumber [8];
        ShowNumber[1] = (num/10000000)%10;
        ShowNumber[2]=  (num/1000000)%10;
        ShowNumber[3] = (num/100000)%10;
        ShowNumber[4] = (num/10000)%10;
        ShowNumber[5] = (num/1000)%10;
        ShowNumber[6] = (num/100)%10;
        ShowNumber[7] = (num/10)%10;
        ShowNumber[8]= (num/1)%10;
        //ȡ��ÿһλ������Ȼ�����һ�������
 
 
 
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
        //��࿪ʼ����Ƿ���0 ����оͲ���ʾ������ʾ�������ĺ��������������������1-9����ô�Ͳ���ʾ������
 
 
  disNum(ShowNumber[1],1,ShowDec[1]);
  disNum(ShowNumber[2],2,ShowDec[2]);
  disNum(ShowNumber[3],3,ShowDec[3]);
  disNum(ShowNumber[4],4,ShowDec[4]);
  disNum(ShowNumber[5],5,ShowDec[5]);
  disNum(ShowNumber[6],6,ShowDec[6]);
  disNum(ShowNumber[7],7,ShowDec[7]);
  disNum(ShowNumber[8],8,ShowDec[8]);
        //������ʾ���֣���ΪLED����˸�ܿ죬���������޷�ʶ����˸��
}
 
void loop()
{
     showNum(13446.8);//��Լ򵥺�������
}