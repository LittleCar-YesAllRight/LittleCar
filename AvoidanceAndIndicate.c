#include "reg52.h"
#define turn_time 1
sbit led_S=P0^1;  // 中间杠
sbit led_LF=P0^2;  // 左前杠
sbit led_LB=P0^3;  // 左后杠
sbit led_B=P0^4;  // 后杠
sbit led_RB=P0^5;  // 右后杠
sbit led_RF=P0^6;  // 右前杠
sbit led_F=P0^7;  // 前杠
sbit moto_RF=P1^6;	//这个怎么没备注叫啥名字？？？
sbit moto_RB=P1^5;
sbit moto_LF=P1^4;
sbit moto_LB=P1^3;
// 红外避障串口,有障碍为0
sbit lavoidance=P3^4;  // 左红外
sbit ravoidance=P3^5;  // 右红外
// 巡线串口, 压线为0
sbit linspection=P3^7;  // 左巡线
sbit rinspection=P3^6;  // 右巡线
void Delay(unsigned char num);
void CarRun(unsigned char dire,unsigned char mode);
void FlashLED();

int main(void)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
{
	while(1)
	{
		// 避障
		if(lavoidance==1 && ravoidance==1 && linspection==0 && rinspection==0)  // 没有障碍,并且不压线
			CarRun('F','T');
		else if(lavoidance==0 && ravoidance==1)  // 左边有障碍
		{
			CarRun('R','T');
			Delay(turn_time);
		}
		else if(lavoidance==1 && ravoidance==0)  // 右边有障碍
		{
			CarRun('L','T');
			Delay(turn_time);
		}
		else if(lavoidance==0 && ravoidance==0)  // 前面有障碍
		{
			CarRun('L','B');
		}
		//巡线
		if(linspection==0 && rinspection==1)
			CarRun('R','F');
		else if(linspection==1 && rinspection==0)
			CarRun('L','F');
		else if(linspection==1 && rinspection==1)
			CarRun('S','T');
	}
}

/***********************************
*函数名称:void Delay(unsigned char num)
*函数功能:延时
*参数说明:num 延时时间 值最大255

***********************************/
void Delay(unsigned char num)
{
    unsigned int temp=0;
    while(num--)
    {
        temp=2000;
        while(temp--);
    }
}



/***********************************
*函数名称：void CarRun(unsigned char dire)
*函数功能：行进方向控制
*参数说明：dire 方向 'S'：停止   'F'：前   'B'：后   'L'：左   'R'：右
           mode 转向方式 'F'：一前一停   'B'：一后一停   'T'：坦克式，一前一后

***********************************/
void CarRun(unsigned char dire,unsigned char mode)
{
    switch (dire)
    {
        case 'S':             //停止
						FlashLED();
            moto_RF=0;
            moto_RB=0;
            moto_LF=0;
            moto_LB=0;
						led_S=0;
            break;
        case 'F':            //前进
						FlashLED();
            moto_RF=1;
            moto_RB=0;
            moto_LF=1;
            moto_LB=0;
						led_F=0;
            break;
        case 'B':            //后退 
						FlashLED();       
            moto_RF=0;
            moto_RB=1;
            moto_LF=0;
            moto_LB=1;
						led_B=0;
	    break;
        case 'L':            //左转        
						FlashLED();
						if(mode=='B')
							moto_RF=0;
						else
						{
							moto_RF=1;
							led_LF=0;
            }
						moto_RB=0;
            moto_LF=0;
						if(mode=='F')
							moto_LB=0;
						else
						{
							moto_LB=1;
							led_LB=0;
						}
	    break;
        case 'R':            //右转
						FlashLED();
            moto_RF=0;
						if(mode='F')
							moto_RB=0
						else
						{
							moto_RB=1;
							led_RB=0;
						}
						if(mode='F')
							moto_LF=0;
						else
						{
							moto_LF=1;
							led_RF=0;
						}
            moto_LB=0;
	    break;
    }
}

/***********************************
*函数名称：void Led(unsigned char dire)
*函数功能：行进方向控制
*参数说明：dire 方向 'S'：停止   'F'：前   'B'：后   'L'：左   'R'：右

***********************************/
void FlashLED()
{
						led_S=1;//中间杠
						led_LF=1;//左前杠
						led_LB=1;//左后杠
						led_B=1;//后杠
						led_RB=1;//右后杠
						led_RF=1;//右前杠
						led_F=1;//前杠
}
/**********************************
***********************************/
