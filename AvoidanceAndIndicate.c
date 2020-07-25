#include "reg52.h"  // keil下请解除这个的注释
#define ABS_NOT(x) (x<0?~x:x)
#define digit P0
// 数字灯
//sbit led_S=P0^1;  // 中间杠
//sbit led_LF=P0^2;  // 左前杠
//sbit led_LB=P0^3;  // 左后杠
//sbit led_B=P0^4;  // 后杠
//sbit led_RB=P0^5;  // 右后杠
//sbit led_RF=P0^6;  // 右前杠
//sbit led_F=P0^7;  // 前杠
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
void Delay(unsigned short);
void Avoidance();
void CarRunPro(char left_rate,char right_rate);
void FlashLightStatus();
void Patrol();

int main(void)
{
    
    while(1)
		{
			Patrol();
			FlashLightStatus();
		}
}

// 避障
void Avoidance()
{
    /*
        // 避障
        if(1==linfrared && 1==rinfrared && 0==llp && 0==rlp)  // 没有障碍,并且不压线
            CarRunPro('F','T');
        else if(0==linfrared && 1==rinfrared)  // 左边有障碍
        {
            CarRun('R','T');
            //Delay(turn_time);
        }
        else if(1==linfrared && 0==rinfrared)  // 右边有障碍
        {
            CarRun('L','T');
            //Delay(turn_time);
        }
        else if(0==linfrared && 0==rinfrared)  // 前面有障碍
        {
            CarRun('L','B');
        }
        //巡线
        if(0==llp && 1==rlp)
            CarRunPro(126,-126);
        else if(1==llp && 0==rlp)
            CarRunPro(-126,126);
    }
*/
}

//右光感巡线
void Patrol()
{
    if(1==llp)  // 如果左光感遇到黑线
    {
        CarRunPro(0,0);  // 停止运行
        return;
    }
    switch(1==rlp){
        case 0:  // 右光感是白色
            CarRunPro(127,-127);  // 往右转圈
            break;
        case 1:  // 右光感是黑色
            CarRunPro(-127,127);  // 往左转圈
            break;
    }
}


/***********************************
*函数名称:void Delay(unsigned short num)
*函数功能:延时
*参数说明:num 延时时间 值最大255

***********************************/
void Delay(unsigned short num)
{
    unsigned int temp=0;
    while(num--)
    {
        temp=2000;
        while(temp--);
    }
}
/**********************************
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
    if(r <= ABS_NOT(left_rate)){
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
        case 0:
            digit=0xFD;  // 中
        case 1:
            digit=0xBF;  // 右前
        case 2:
            digit=0xDF;  // 右后
        case 3:
            digit=0xFB;  // 左前
        case 4:
            digit=0x7F;  // 前
				case 6:
            digit=0xF7;  // 左后
        case 7:
            digit=0xB7;  // 右前左后
        case 8:
            digit=0xD7;  // 后
    }
}
/**********************************
***********************************/
