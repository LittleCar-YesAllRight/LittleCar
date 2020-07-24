#include "reg52.h"
#define turn_time 1
#define ABS_NOT(x) (x<0?~x:x)
// 数字灯
sbit led_S=P0^1;  // 中间杠
sbit led_LF=P0^2;  // 左前杠
sbit led_LB=P0^3;  // 左后杠
sbit led_B=P0^4;  // 后杠
sbit led_RB=P0^5;  // 右后杠
sbit led_RF=P0^6;  // 右前杠
sbit led_F=P0^7;  // 前杠
// 马达
sbit moto_RF=P1^6;  // 右轮向前	
sbit moto_RB=P1^5;  // 右轮向后
sbit moto_LF=P1^4;  // 左轮向前
sbit moto_LB=P1^3;  // 左轮向后
// 红外避障串口,有障碍为0
sbit lavoidance=P3^4;  // 左红外
sbit ravoidance=P3^5;  // 右红外
// 巡线串口, 压线为0
sbit linspection=P3^7;  // 左巡线
sbit rinspection=P3^6;  // 右巡线
void Delay(unsigned short num);
void CarRun(unsigned char dire,unsigned char mode);
void FlashLED();
void AvoidanceAndIndicate();
void CarRunPro(signed char left_rate,signed char right_rate);

int main(void)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
{
				
	AvoidanceAndIndicate();
	while(1);
}

// 巡线和避障
void AvoidanceAndIndicate()
{
	while(1)
	{
		// 拿在空中 或者 碰到黑线 就停
		if(1==linspection && 1==rinspection)
		{
			CarRun('S','T');
			continue;
		}
		// 避障
		if(1==lavoidance && 1==ravoidance && 0==linspection && 0==rinspection)  // 没有障碍,并且不压线
			CarRun('F','T');
		else if(0==lavoidance && 1==ravoidance)  // 左边有障碍
		{
			CarRun('R','T');
			Delay(turn_time);
		}
		else if(1==lavoidance && 0==ravoidance)  // 右边有障碍
		{
			CarRun('L','T');
			Delay(turn_time);
		}
		else if(0==lavoidance && 0==ravoidance)  // 前面有障碍
		{
			CarRun('L','B');
		}
		//巡线
		if(0==linspection && 1==rinspection)
			CarRunPro(126,-126);
		else if(1==linspection && 0==rinspection)
			CarRunPro(-126,126);
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
        temp=200;
        while(temp--);
    }
}
/**********************************
***********************************/


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
						CarRunPro(0,0);
            break;
        case 'F':            //前进
            CarRunPro(127,127);
            break;
        case 'B':            //后退 
            CarRunPro(-128,-128);
	    break;
        case 'L':            //左转        
						if('B'==mode)
							CarRunPro(-128,0);
						if('F'==mode)
							CarRunPro(0,127);
						else
							CarRunPro(-128,127);
	    break;
        case 'R':            //右转
            if('B'==mode)
							CarRunPro(0,-128);
						if('F'==mode)
							CarRunPro(127,0);
						else
							CarRunPro(127,-128);
						break;
    }
}

/**********************************
***********************************/

/***********************************
*函数名称：void CarRunPro(signed char left_rate,signed char right_rate)
*函数功能：行进方向的高级控制
*参数说明：left_rate/right_rate(-128~127)表示左/右轮跑的速度,0/-1表示完全不运动,实际上,只有接近127能驱动

***********************************/
void CarRunPro(signed char left_rate,signed char right_rate)
{
	static signed char l=0,r=0;
	moto_RF=0<right_rate && 0==r;
	moto_RB=-1>right_rate && 0==r;
	moto_LF=0<left_rate && 0==l;
	moto_LB=-1>left_rate && 0==l;
	led_S=!((0==left_rate||-1==left_rate)&&(0==right_rate||-1==right_rate));// 左右皆止
	led_LF=!(0<right_rate&&!(0<left_rate));// 右进左转
	led_LB=!(-1>left_rate&&!(-1>right_rate));// 左退左转
	led_B=!(-1>left_rate&&-1>right_rate);// 后退
	led_RB=!(!(-1>left_rate)&&-1>right_rate);// 右退右转
	led_RF=!(0<left_rate&&!(0<right_rate));// 左进右转
	led_F=!(0<left_rate&&0<right_rate);// 前进
	l++;
	r++;
	l%=128-ABS_NOT(left_rate);
	r%=128-ABS_NOT(right_rate);
}
/**********************************
***********************************/
