/*
 * PID.c
 *
 *  Created on: 2025��1��25��
 *      Author: zhuji
 */
#include "PID.h"

int basespeed=1000;
int setspeed1=1000;
int setspeed2=1000;
int speed1;//����
int speed2;
int Increase1=0;
int Increase2=0;
float divertion;
float erspeed=0;

float gyro_filtered = 0; // �˲����������ֵ
float ratio=0;
double steer =0 ;
double steer1 =0 ;
PID_Datatypedef sptr1,sptr2;
PID_imu_Datatypedef imu;
PID_Angeltypedef angel;
//float P_L=0.1;
//float I_L=0;
//float P_R=0.1;
//float I_R=0;

void PID_Init(PID_Datatypedef*sptr)
{
    sptr->P=0;
    sptr->I=0;
    sptr->D=0;
    sptr->LastError=0;
    sptr->PrevError=0;
}
void imu_PID_Init(PID_imu_Datatypedef*imu)
{
    imu->KP_1=0;
    imu->KD_1=0;
    imu->KP_2=0;
    imu->GKD=0;
    imu->lasterror=0;

    imu->integrator = 0;   // �������ʼ��Ϊ0
}
int MotorPID_Output(PID_Datatypedef*sptr,float NowSpeed,int ExpectSpeed)
{
    int Increase;
    int iError;
    iError=ExpectSpeed-NowSpeed;
    Increase=(int)(sptr->P*(iError-sptr->LastError)+sptr->I*iError+sptr->D*(iError-2*sptr->LastError+sptr->PrevError));
    sptr->PrevError=sptr->LastError;
    sptr->LastError=iError;
    return Increase;
}
float imuPID_Output(float erspeed,PID_imu_Datatypedef*imu)
{
    float imu_out;
//    imu_out=erspeed*imu->KP_1+(erspeed-imu->lasterror)*imu->KD_1-imu660ra_gyro_x*imu->GKD;
//    imu->lasterror=erspeed;
    imu_out = (erspeed * imu->KP_1) + (erspeed * fabsf(erspeed) * imu->KP_2) + (erspeed - imu->lasterror) * imu->KD_1 + imu660ra_gyro_x * imu->GKD;
    imu->lasterror = erspeed; // ������һ�����
    if (imu_out > 2300) imu_out =2300;
    else if (imu_out < -2300) imu_out = -2300;
    return imu_out;

}
//int ang_pid(float b,int c)
//{
//    int t;
//    int temp_speed;
//
//    temp_speed=(speed1+speed2)/2;
//
//    t=angel.kP*(temp_speed-b)+angel.kD*(temp_speed-angel.LastError1);
//    angel.LastError1=temp_speed-b;
//
//    return t;
//
//}
void Control_car(void)
{
    steer = center_line_error;
    steer1 = fabsf(center_line_error);
    if(zhidaoflag)
    {  if (0.1>steer1)
          steer1=15*steer1;
       else if (0.5>steer1)
       steer1=3*steer1;
       else if (1>steer1)
       steer1=4*steer1;
       else
           steer1=1.5*steer1;}
    else if(!zhidaoflag)
    { if (0.1>steer1)
           steer1=35*steer1;
        else if (0.5>steer1)
        steer1=5*steer1;
        else if (1>steer1)
        steer1=8*steer1;
        else
            steer1=2.5*steer1;}
    if(8<steer1)
        steer1=8;
       ratio = steer1 / MAX_DEVIATION;  // ���������0.0~1.0��
          setspeed1=MAX_SPEED - (MAX_SPEED - MIN_SPEED) * ratio;
          setspeed2=MAX_SPEED - (MAX_SPEED - MIN_SPEED) * ratio;

     if(setspeed1>=(MAX_SPEED+MIN_SPEED)*0.5)
     {  imu.KP_1 = 18;
         imu.KD_1 = 0;
     //     imu.KP_2 = 0.1;
         imu.GKD = -2.7;}
     else if(setspeed1<(MAX_SPEED+MIN_SPEED)*0.5)
     { imu.KP_1 = 15;
             imu.KD_1 = 0;
         //     imu.KP_2 = 0.1;
             imu.GKD = -2.5;}
             if((steer>=-10&steer<=-3)||(steer<=10&&steer>=3))
                       erspeed=70*steer;//7.3
             else if ((steer > 1 && steer < 3) || (steer > -3 && steer < -1))
                        erspeed=60*steer;
          //              else
             else if(steer<1||steer>-1)
                        erspeed=40*steer;

                        else
                            erspeed=65*steer;
}
void PID_output(void)
{

//    static float last_Increase1 = 0;
//    static float last_Increase2 = 0;

//            ����ʽPID����С��ֱ��
    Increase1=MotorPID_Output(&sptr1,speed1,setspeed1);
    Increase2=MotorPID_Output(&sptr2,speed2,setspeed2);
            //����ֱ��ŤתС�����з���
//    Increase1=Increase1-divertion;
//    Increase2=Increase2+divertion;
//    Increase1=-divertion;
//    Increase2=+divertion;
    divertion=imuPID_Output(erspeed,&imu);
    Increase1=Increase1-divertion;
    Increase2=Increase2+divertion;
    // �����������ֹ��ת�����
//    if (Increase1 < 0) Increase1 = 0.02*Increase2;
//    if (Increase2 < 0) Increase2 = 0.02*Increase1;
//    if (Increase1 > 2500) Increase1 = 3000;
//    if (Increase2 > 2500) Increase2 = 3000;
    Motor_Left(Increase1);
    Motor_Right(Increase2);
//    Motor_Left(1000);
//    Motor_Right(1000);

}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     PID����
  @param     int set_speed ,int speed,����ֵ��ʵ��ֵ
  @return    ���ռ�ձ�
  Sample     pwm_R= PID_R(set_speed_right,right_wheel);//pid���Ƶ��ת��
             pwm_L= PID_L(set_speed_left,left_wheel );//pid���Ƶ��ת��
  @note      ������
-------------------------------------------------------------------------------------------------------------------*/
//int PID_L(int set_speed ,float speed)//pid���Ƶ��ת��
//{
//    volatile static int out;
//    volatile static int out_increment;
//    volatile static int ek,ek1;
//    volatile static int speed_bb;
//
//    ek1 = ek;
//    ek = set_speed - speed;
//
//    if(ek>80) speed_bb=SPEED_MAX;
//    else if(ek<-80) speed_bb=SPEED_MIN;
//    else speed_bb=0;
//
//    out_increment= (int)(P_L*(ek-ek1) + I_L*ek + speed_bb);
//    out+= out_increment;
//
//    if(out>=SPEED_MAX)//�޷�����
//        out=SPEED_MAX;
//    else if(out<=SPEED_MIN)
//        out=SPEED_MIN;
//    return (int) out;
//}
//
///*-------------------------------------------------------------------------------------------------------------------
//  @brief     PID����
//  @param     int set_speed ,int speed,����ֵ��ʵ��ֵ
//  @return    ���ռ�ձ�
//  Sample     pwm_R= PID_R(set_speed_right,right_wheel);//pid���Ƶ��ת��
//             pwm_L= PID_L(set_speed_left,left_wheel );//pid���Ƶ��ת��
//  @note      ������
//-------------------------------------------------------------------------------------------------------------------*/
//int PID_R(int set_speed ,float speed)//pid���Ƶ��ת��
//{
//    volatile static int  out;
//    volatile static int  out_increment;
//    volatile static int  ek,ek1;
//    volatile static int speed_bb;
//
//    ek1 = ek;
//    ek = set_speed - speed;
//
//    if(ek>80) speed_bb=SPEED_MAX;
//    else if(ek<-80) speed_bb=SPEED_MIN;
//    else speed_bb=0;
//
//    out_increment= (int)(P_R*(ek-ek1) + I_R*ek + speed_bb);
//    out+= out_increment;
//
//    if(out>=SPEED_MAX)//�޷�����
//        out=SPEED_MAX;
//    else if(out<=SPEED_MIN)
//        out=SPEED_MIN;
//    return (int) out;
//}



