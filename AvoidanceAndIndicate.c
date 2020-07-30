#include "reg52.h"  // keil下请解除这个的注释
#define ABS_NOT(x) (x<0?~x:x)
#define digit P0  // 数字灯
// 马达
sbit moto_RF=P1^3;  // 右轮向前    
sbit moto_RB=P1^2;  // 右轮向后
sbit moto_LF=P1^1;  // 左轮向前
sbit moto_LB=P1^0;  // 左轮向后
sbit ena=P1^4;  // 使能A，用于左轮PWM调速
sbit enb=P1^5;  // 使能B，用于右轮PWM调速
// 红外避障串口,有障碍为0
sbit linfrared=P3^4;  // 左红外
sbit rinfrared=P3^5;  // 右红外
// 巡线串口, 压线为0
sbit llp=P3^7;  // 左巡线
sbit rlp=P3^6;  // 右巡线
sbit led1=P0^1;

//void Delay(unsigned short);
void init_sys(void);
void CarRunPro(char left_rate,char right_rate);
void Avoidance(void);
void FlashLightStatus(void);
char ldutyfactor=127;
char rdutyfactor=127;

int main(void)
{
    init_sys();
    while(1)
    {
			CarRunPro(10,10);
        //Avoidance();
        //Patrol();
        
        //FlashLightStatus();
    }
}

void init_sys()
{
    TMOD=0x01;
    TH0=0xFF;
    TL0=0xF6;
    TR0=1;
    ET0=1;
    EA=1;
}

/***********************************
*函数名称：void Avoidance()
*函数功能：避障
*参数说明：无参数

***********************************/
void Avoidance()
{
    if(1==linfrared && 1==rinfrared)  // 没有障碍,并且不压线
        CarRunPro(127,127);
    else if(0==linfrared && 1==rinfrared)  // 左边有障碍 
        CarRunPro(127,-127);
    else if(1==linfrared && 0==rinfrared)  // 右边有障碍 
        CarRunPro(-127,127);
    else if(0==linfrared && 0==rinfrared)  // 前面有障碍
        CarRunPro(-127,0);
}
/**********************************
***********************************/

/***********************************
*函数名称：void CarRunPro()
*函数功能：PWM控制的小车行驶函数
*参数说明：left_rate和right_rate为占空比,范围-100~100,0未停止

***********************************/
void CarRunPro(char left_rate,char right_rate)
{
    rdutyfactor=right_rate;
    ldutyfactor=left_rate;
    // 左轮
    if (left_rate>0)
    {
        moto_LF=1;
        moto_LB=0;
    }else if (left_rate<-1)
    {
        moto_LF=0;
        moto_LB=1;
    }else
    {
        moto_LF=0;
        moto_LB=0;
    }
    // 右轮
    if (right_rate>0)
    {
        moto_RF=1;
        moto_RB=0;
    }else if (right_rate<-1)
    {
        moto_RF=0;
        moto_RB=1;
    }else
    {
        moto_RF=0;
        moto_RB=0;
    }
}
/**********************************
***********************************/



/***********************************
*函数名称：void FlashLightStatus()
*函数功能：刷新小车数字灯, 指示小车运行状态
*参数说明：无参数

***********************************/
void FlashLightStatus()
{
    int status;
    status=1*(1==moto_RF)+2*(1==moto_RB)+3*(1==moto_LF)+6*(1==moto_LB);
    switch(status){
        case 0:  // 停止时
            digit=0xFD;  // 中
            break;
        case 1:  // 只有右轮前进时
            digit=0xFB;  // 左前
            break;
        case 2:  // 只有右轮后退时
            digit=0xF7;  // 左后
            break;
        case 3:  //  只有左轮前进时
            digit=0xBF;  // 右前
            break;
        case 4:  // 左右轮同时前进时
            digit=0x7F;  // 前
            break;
        case 5:  // 右轮前进左轮后退时
            digit=0xDB;  // 前
            break;
        case 6:  // 左轮后退时
            digit=0xDF;  // 右后
            break;
        case 7:  // 右轮前进左轮后退时
            digit=0xB7;  // 右前左后
            break;
        case 8:  // 左右轮同时后退时
            digit=0xD7;  // 后
            break;
    }
}
/**********************************
***********************************/

void timer0(void) interrupt 1 using 2
{
	static char l=0,r=0;
	
    if(l<ldutyfactor)
			ena=1;
			else
				ena=0;
	if(r<rdutyfactor)
		enb=1;
	else
		enb=0;
	    TH0=0xFF;
    TL0=0xF6;
	l=(l+1)%128;
	r=(r+1)%128;
}