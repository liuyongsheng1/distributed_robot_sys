#include "uartodompthread.h"
#include "uartodom.h"



namespace zw {



NavPara UartOdomPthread::m_navPara={{0,0,0},{0,0,0},false,false};
geometry_msgs::Twist UartOdomPthread::vel;

void CalNavCmdVel(const NavPara *nav ,geometry_msgs::Twist& ctr);
inline void limit(float& v ,float left,float right);


UartOdomPthread::UartOdomPthread()
{
  pthread_create(&id,NULL,MyPthread,(void *)this);
}

UartOdomPthread::~UartOdomPthread()
{
  pthread_detach(id);
}

void *UartOdomPthread::MyPthread(void *temp)
{
  UartOdomPthread *t=(UartOdomPthread *)temp;
  t->DoPthread();
}

void *UartOdomPthread::DoPthread(void)
{
  ros::NodeHandle n;
  ros::Subscriber key_sub =n.subscribe("cmd_vel",2,cmd_keyCallback);
  ros::Subscriber pose_sub = n.subscribe<geometry_msgs::PoseStamped>("scan_p", 2,PoseReceived);
  ros::Publisher odom_pub = n.advertise<nav_msgs::Odometry>("odom", 100);
  ros::Publisher imu_pub =n.advertise<sensor_msgs::Imu>("imu",100);
  ros::Publisher vel_pub =n.advertise<geometry_msgs::Twist>("cmd_vel",2);

  ros::Time starts = ros::Time::now();
  ros::Time ends = ros::Time::now();

  sensor_msgs::Imu  imu;
 {
      float linear_acceleration_stddev=0;
      float linear_velocity_stddev=0;
      float orientation_stddev =0;

      imu.linear_acceleration_covariance[0]= linear_acceleration_stddev ;
      imu.linear_acceleration_covariance[4]= linear_acceleration_stddev ;
      imu.linear_acceleration_covariance[8]= linear_acceleration_stddev ;
      imu.angular_velocity_covariance[0]= linear_velocity_stddev ;
      imu.angular_velocity_covariance[4]= linear_velocity_stddev ;
      imu.angular_velocity_covariance[8]= linear_velocity_stddev ;
      imu.orientation_covariance[0]=orientation_stddev;
      imu.orientation_covariance[4]=orientation_stddev;
      imu.orientation_covariance[8]=orientation_stddev;
  }

  double dt ;
  double x=0.0;
  double y=0.0;
  double th=0.0;
  double lth =0.0;

  double vx;
  double vy;
  double vth;
  double delta_x;
  double delta_y ;
  double delta_th ;
  tf:: TransformBroadcaster odom_broadcaster;

  //ros::Timer timer=n.createTimer(ros::Duration(0.02),timerCallback,false);

  ros::Rate loop_rate(50);
  Paras m_para;
  while(ros::ok())
  {
    ros::spinOnce();
    ends = ros::Time::now();
    dt =(ends-starts).toSec();

    int32_t car_msg[6];

    ParaGetSet car_para={R_REGISTER,2,MSG_CONTROL,car_msg};
    m_para.GetAddressValue(car_para);
    zw::Float2Int32 mf;
    mf.i= car_msg[0];
    vx =(double)mf.f;
    vy =0;
  //  mf.i=car_msg[1];
  //  vth = (double)mf.f;
    car_para={R_REGISTER,6,MSG_IMU,car_msg};
    m_para.GetAddressValue(car_para);
    vth = (double)car_msg[5] * Gyro_Gr;    //gyr_z

    delta_th = vth*dt;
    th += delta_th;
    delta_x = (vx * cos(th) -vy *sin(th)) * dt;
    delta_y = (vx * sin(th) -vy *cos(th)) * dt;

    x +=delta_x ;
    y +=delta_y ;

    geometry_msgs :: Quaternion odom_quat =tf::createQuaternionMsgFromYaw(th);

    geometry_msgs :: TransformStamped odom_trans;
    {
        odom_trans.header.stamp = ends;
        odom_trans.header.frame_id = "odom";
        odom_trans.child_frame_id ="base_link";

        odom_trans.transform.translation.x =x;
        odom_trans.transform.translation.y =y;
        odom_trans.transform.translation.z =0.0;
        odom_trans.transform.rotation =odom_quat;
        odom_broadcaster.sendTransform(odom_trans);

        nav_msgs ::Odometry odom;
        odom.header.stamp =ends;
        odom.header.frame_id = "odom" ;

        odom.pose.pose.position.x =x;
        odom.pose.pose.position.y =y;
        odom.pose.pose.position.z =0.0;
        odom.pose.pose.orientation = odom_quat ;
        odom.child_frame_id = "base_link" ;
        odom.twist.twist.linear.x =vx;
        odom.twist.twist.linear.y =vy;
        odom.twist.twist.angular.z = vth ;

        //publish odom
        odom_pub.publish(odom);
    }

    {
        float xf=car_msg[0] *Acc_Mss;    //acc_x   m/s^2
        float yf=car_msg[1] *Acc_Mss;
        float zf=car_msg[2] *Acc_Mss;
        imu.linear_acceleration.x =xf;
        imu.linear_acceleration.y =yf;
        imu.linear_acceleration.z =zf;
        xf= car_msg[3] * Gyro_Gr;          //gyr_x     rad/s
        yf= car_msg[4] * Gyro_Gr;
        zf= car_msg[5] * Gyro_Gr;
        imu.angular_velocity.x =xf;
        imu.angular_velocity.y =yf;
        imu.angular_velocity.z =zf;
        imu.header.stamp= ends;
        imu.header.frame_id = "imu_link";
        //publish imu
        imu_pub.publish(imu);
    }

    if(m_navPara.startNav)
       vel_pub.publish(vel);

    starts = ends;
    loop_rate.sleep();
  }
}

void UartOdomPthread::cmd_keyCallback(const geometry_msgs::Twist::ConstPtr & cmd)
{
    Float2Int32 fi;
    int32_t dat[2];
    ParaGetSet packInfo={R_REGISTER,1, BTN_SWITCH,dat};
    Paras m_para;
    m_para.GetAddressValue(packInfo);
    if((dat[0]&KEY_VEL_CTR)==KEY_VEL_CTR)
        return;
    else{
        fi.f=cmd->linear.x;
        dat[0]=fi.i;
        fi.f=cmd->angular.z;
        dat[1]=fi.i;
        packInfo={W_REGISTER,2, CONTROL,dat};
        Paras m_para;
        m_para.SetAddressValue(packInfo);
    }
}

void UartOdomPthread::PoseReceived(const geometry_msgs::PoseStampedConstPtr pose)
{
  int32_t ctr_msg[3];
  zw::ParaGetSet car_para={W_REGISTER,3,(ParaAddress)(MSG_CONTROL+2),ctr_msg};
  zw::Float2Int32 mf;
  m_navPara.current.x =mf.f =(float)pose->pose.position.x;
  ctr_msg[0]=mf.i;
  m_navPara.current.y =mf.f =(float)pose->pose.position.y;
  ctr_msg[1]=mf.i;
  m_navPara.current.h =mf.f =(float)(tf::getYaw(pose->pose.orientation));
  ctr_msg[2]=mf.i;
  zw::Paras m_para;
  m_para.SetAddressValue(car_para);

  car_para.fuc = R_REGISTER;
  car_para.addr = (ParaAddress)(CONTROL+2);
  m_para.GetAddressValue(car_para);
  mf.i=ctr_msg[0];
  m_navPara.desired.x =mf.f;
  mf.i=ctr_msg[1];
  m_navPara.desired.y =mf.f;
  mf.i=ctr_msg[2];
  m_navPara.desired.h =mf.f;

  car_para.len =1;
  car_para.addr = BTN_SWITCH;
  m_para.GetAddressValue(car_para);
  m_navPara.startNav = (bool)(ctr_msg[0] & KEY_START_NAV);
  m_navPara.emergeStop = (bool)(ctr_msg[0] & KEY_EME_STOP);
  if(m_navPara.startNav)
      CalNavCmdVel(&m_navPara,vel);
}

inline void limit(float& v ,float left,float right)
{
    if(v>right)
        v=right;
    else if(v<left)
        v=left;
}

void CalNavCmdVel(const NavPara *nav ,geometry_msgs::Twist& ctr)
{
    if(nav->emergeStop)
    {
        ctr.linear.x=0;
        ctr.angular.z=0;
        return ;
    }

   int32_t pid[6];
   zw::ParaGetSet para={R_REGISTER,6,(ParaAddress)(ADD_PID+6),pid};
   zw::Paras m_para;
   m_para.GetAddressValue(para);
   ROS_INFO("%d,%d,%d,%d,%d,%d",pid[0],pid[1],pid[2],pid[3],pid[4],pid[5]);

   float dpx,dpy,dph;
   dpx = nav->desired.x - nav->current.x;
   dpy = nav->desired.y - nav->current.y;
   dph = nav->desired.h - nav->current.h;

   float dfh = atan2(dpy,dpx) - nav->current.h;
   float ds = sqrt(dpx *dpx +dpy*dpy);

   if(dfh>M_PI)
       dfh -=2*M_PI;
   else if(dfh<-M_PI)
       dfh +=2*M_PI;

   if(dph>M_PI)
       dph -=2*M_PI;
   else if(dph<-M_PI)
       dph +=2*M_PI;

   if(fabs(ds) < 0.05 && fabs(dph)<0.1)
   {
       ctr.linear.x=0;
       ctr.angular.z=0;
       ROS_INFO("dis_err=%6.3f ang_err=%6.3f",ds,dph);
       return ;
   }

   if(fabs(dfh)>M_PI/2)
       ds = -ds ;
   static float lds=0;
   static float ids=0;
   ids +=ds;
   float dds=ds-lds;
   lds=ds;
   limit(ids,-2,2);
   limit(dds,-0.5,-0.5);
   float vx=(ds*pid[0]+ ids*pid[1]+ dds*pid[2])/100.0;
   limit(vx,-0.2,0.2);

   static float ldfh=0;
   static float idfh=0;
   idfh +=dfh;
   float ddfh =dfh-ldfh;
   ldfh=dfh;
   limit(idfh,-2,2);
   limit(ddfh,-1,1);
   float afz=(dfh*pid[3]+ idfh*pid[4]+ ddfh*pid[5])/100.0;
   limit(afz,-1, 1);

   static float ldph=0;
   static float idph=0;
   idph +=dph;
   float ddph =dph-ldph;
   ldph=dph;
   limit(idph,-2,2);
   limit(ddph,-1,1);
   float apz=(dph*pid[3]+ idph*pid[4]+ ddph*pid[5])/100.0;
   limit(apz,-1, 1);

   static float lvx=0,laz=0;

   float az=0;
   if(fabs(ds)>0.05)
   {
       idph=0;
       ldph=0;
       az = afz;
   }else if(fabs(dph)>0.1){
       idfh=0;
       ldfh=0;
       ids=0;
       lds=0;
       if(fabs(vx-lvx)>0.1)
           vx =lvx+(vx-lvx)*0.5;
       else
           vx=0;
       az = apz;
   }else{
       az = 0.8*afz +0.2*apz;
   }
   limit(vx,-0.2,0.2);
   limit(az,-1,1);

   ROS_INFO("vx=%6.3f, az=%6.3f",vx,az);

   lvx =vx;
   laz =az;

   ctr.linear.x =vx;
   ctr.angular.z =az;
}




}

