#include <Servo.h> 
//#define send

char getstr;

//定义了板上的4个控制端为电机控制，12一组，34一组 
int in1 = 9;
int in2 = 10;
int in3 = 11;  
int in4 = 12;  

//定义了头部舵机和超声波传感器
Servo headServo;
Servo RightArmServo; 
Servo LeftArmServo; 
 
int Echo = A3;  // Echo回声脚 A3换5
int Trig = A2;  // Trig 触发脚 A2换6

int rightDistance = 0,leftDistance = 0,middleDistance = 0 ;

int Distance_test()   // 测距
{
  digitalWrite(Trig, LOW);   // 给触发脚低电平2μs
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);  // 给触发脚高电平10μs，这里至少是10μs
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);    // 持续给触发脚低电
  float Fdistance = pulseIn(Echo, HIGH);  // 读取高电平时间(单位：微秒)
  Fdistance= Fdistance/58;       //为什么除以58等于厘米，  Y米=（X秒*344）/2
  // X秒=（ 2*Y米）/344 ==》X秒=0.0058*Y米 ==》厘米=微秒/58
  return (int)Fdistance;
} 

void forward()
{
 digitalWrite(in1,HIGH);
 digitalWrite(in2,LOW);
 digitalWrite(in3,HIGH);
 digitalWrite(in4,LOW);
}
 
void back()
{
 digitalWrite(in1,LOW);
 digitalWrite(in2,HIGH);
 digitalWrite(in3,LOW);
 digitalWrite(in4,HIGH);
 delay(500);
 mStop();
}
 
void turnleft()
{
 digitalWrite(in1,HIGH);
 digitalWrite(in2,LOW);
 digitalWrite(in3,LOW);
 digitalWrite(in4,HIGH);
 delay(100);
 mStop();
}
 
void turnright()
{
 digitalWrite(in1,LOW);
 digitalWrite(in2,HIGH);
 digitalWrite(in3,HIGH);
 digitalWrite(in4,LOW);
 delay(100);
 mStop();
} 

void mStop()
{
 digitalWrite(in1,LOW);
 digitalWrite(in2,LOW);
 digitalWrite(in3,LOW);
 digitalWrite(in4,LOW);
} 

void LeftArmWave()
{
  LeftArmServo.write(0); 
  delay(1000);
  LeftArmServo.write(180);
  delay(1000);
  LeftArmServo.write(90);  
  delay(1000);
}

void RightArmWave()
{
  RightArmServo.write(180); 
  delay(1000);
  RightArmServo.write(0);
  delay(1000);
  RightArmServo.write(90);  
  delay(1000);
}

void headWave()
{
  headServo.write(180); 
  delay(1000);
  headServo.write(0);
  delay(1000);
  headServo.write(90);  
  delay(1000);
}

void setup()     
{      
  headServo.attach(2);        //引脚2 与头部舵机相连
  RightArmServo.attach(3);    //引脚3 Right Arm 
  LeftArmServo.attach(4);     //引脚4 Left Arm

  Serial.begin(9600);     //初始化串口
  Serial.flush(); //清除串口缓存

  //初始舵机角度
  headServo.write(90);
  RightArmServo.write(0);
  LeftArmServo.write(180);  
    
  pinMode(Echo, INPUT);    // 定义超声波输入脚
  pinMode(Trig, OUTPUT);   // 定义超声波输出脚  
  pinMode(in1,OUTPUT);  
  pinMode(in2,OUTPUT);
  pinMode(in3,OUTPUT);   
  pinMode(in4,OUTPUT);   
    
  //下面程序开始时让控制端都为低电平，电机保持不动。
  mStop();
}

void loop()
{    
  getstr=Serial.read();
  
  if(getstr=='f')  
  { 
    Serial.println("Go forward!"); 
    mStop();
    forward();
  } 
  else if(getstr=='b')
  {
    Serial.println("Go back!"); 
    mStop();
    back();
  } 
  else if(getstr=='l')
  { 
    Serial.println("Go left!"); 
    mStop();
    turnleft();
  } 
  else if(getstr=='r')
  { 
    Serial.println("Go right!"); 
    mStop();
    turnright();
  }  
  else if(getstr=='s')
  { 
    Serial.println("Stop!"); 
    mStop();  
  }   
  else if(getstr=='1')
  {
    Serial.println("Left Arm Wave!"); 
    LeftArmWave();  
  }   
  else if(getstr=='2')
  {
    Serial.println("Right Arm Wave!"); 
    RightArmWave();  
  }
  else if(getstr=='n')
  {
    Serial.println("Head Move!"); 
    headWave();  
  }
  else if(getstr=='a') 
  {
    headServo.write(90);
    delay(500);
    RightArmWave();  
  
    middleDistance = Distance_test();
    #ifdef send
    Serial.print("middleDistance=");
    Serial.println(middleDistance);
    #endif
  
  if(middleDistance<=20)
    {     
      mStop();
      delay(500);                         
      headServo.write(5);          
      delay(1000);      
      rightDistance = Distance_test();

      #ifdef send
      Serial.print("rightDistance=");
      Serial.println(rightDistance);
      #endif
       
      delay(500);
      headServo.write(90);              
      delay(1000);                                                  
      headServo.write(175);              
      delay(1000); 
      leftDistance = Distance_test();

      #ifdef send
      Serial.print("leftDistance=");
      Serial.println(leftDistance);
      #endif

      delay(500);
      headServo.write(90);              
      delay(1000);
      if(rightDistance>leftDistance)  
      {
        turnright();
        delay(100);
       }
       else if(rightDistance<leftDistance)
       {
        turnleft();
        delay(100);
       }
       else
       {
        forward();
       }
    }  
    else
    {
      forward(); 
    }
  }
 
}

