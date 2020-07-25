#include "reg52.h"  // keil下请解除这个的注释
#define ABS_NOT(x) (x<0?~x:x)
#define digit P0  // 数字灯
// 马达
sbit moto_RF=P1^6;  // 右轮向前    
sbit moto_RB=P1^5;  // 右轮向后
sbit moto_LF=P1^4;  // 左轮向前
sbit moto_LB=P1^3;  // 左轮向后
// 红外避障串口,有障碍为0
sbit linfrared=P3^4;  // 左红外
sbit rinfrared=P3^5;  // 右红外
// 巡线串口, 压线为0
sbit llp=P3^7;  // 左巡线
sbit rlp=P3^6;  // 右巡线
//void Delay(unsigned short);
void CarRunPro(char left_rate,char right_rate);
void Avoidance();
void LPatrol();
void FlashLightStatus();

int main(void)
{
    
    while(1)
    {
        LPatrol();
        FlashLightStatus();
    }
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
        CarRunPro(127,-127);
    else if(0==linfrared && 0==rinfrared)  // 前面有障碍
        CarRunPro(-127,0);

}
/**********************************
***********************************/


/***********************************
*函数名称：void Avoidance()
*函数功能：右光感巡线
*参数说明：无参数

***********************************/
void LPatrol()
{
	//if(1==llp && 1==rlp)  // 如果左光感遇到黑线就停，以防万一，也检测一下右光感是否压在黑线上
    if(1==llp)  
    {
        CarRunPro(0,0);  // 停止运行
        return;
    }
    switch(1==rlp){
        case 0:  // 右光感是白色
            CarRunPro(100,0);  // 往右转圈
            break;
        case 1:  // 右光感是黑色
            CarRunPro(0,100);  // 往左转圈
            break;
    }
		/*
    switch(1==llp){
        case 1:  // 万一到这里左光感还能是1, 就说明脱轨了, 这时候给他个微调
            CarRunPro(0,100);  // 往左转圈
            break;
    }
		*/
}
/**********************************
***********************************/


/***********************************
*函数名称:void Delay(unsigned short num)
*函数功能:延时
*参数说明:num 延时时间 值最大255

***********************************
void Delay(unsigned short num)
{
    unsigned int temp=0;
    while(num--)
    {
        temp=2000;
        while(temp--);
    }
}
**********************************
***********************************/


/***********************************
*函数名称：void CarRunPro(char left_rate,char right_rate)
*函数功能：行进方向的高级控制
*参数说明：left_rate/right_rate(-128~127)表示左/右轮跑的速度,数值太小可能会无法驱动,0/-1表示完全不运动

***********************************/
void CarRunPro(char left_rate,char right_rate)
{
    static char l=0,r=0;
    if(l <= ABS_NOT(left_rate)){
        if(left_rate > 0){
            moto_LF = 1;
            moto_LB = 0;
        }else if(left_rate < -1){
            moto_LB = 1;
            moto_LF = 0;
        }else{
            moto_LB = 0;
            moto_LF = 0;
        }
    }else{
        moto_LB = 0;
        moto_LF = 0;
    }
    if(r <= ABS_NOT(right_rate)){
        if(right_rate>0){
            moto_RF = 1;
            moto_RB = 0;
        }else if(right_rate<-1){
            moto_RB = 1;
            moto_RF = 0;
        }else{
            moto_RB = 0;
            moto_RF = 0;
        }
    }else{
        moto_RB = 0;
        moto_RB = 0;
    }
    l++;
    r++;
    l%=128;
    r%=128;
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
