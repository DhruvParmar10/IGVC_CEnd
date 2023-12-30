#include <ros.h>
#include <std_msgs/Empty.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Bool.h>


ros::NodeHandle  nh;

std_msgs::Float32 avg_msg;
float offsetFinal = 0;
bool kill = false;

void avgCallback(const std_msgs::Float32& msg)
{
  offsetFinal = msg.data;
}

void killCallback(const std_msgs::Bool& msg)
{
  kill = msg.data;
}

ros::Subscriber<std_msgs::Float32> sub("/float_data", &avgCallback);
ros::Subscriber<std_msgs::Float32> sub("/exit", &killCallback);

const int motor1IN1 = 11;
const int motor1IN2 = 9;
const int motor1PWM = 13;
const int motor2IN1 = 10;
const int motor2IN2 = 8;
const int motor2PWM = 12;

int pwm = 140;
int pwm1 = 84;
int pwm2 = 84;
float offset = 0;


void setup()
{
  Serial.begin(57600);

  pinMode(motor1IN1, OUTPUT);
  pinMode(motor1IN2, OUTPUT);
  pinMode(motor1PWM, OUTPUT);
  pinMode(motor2IN1, OUTPUT);
  pinMode(motor2IN2, OUTPUT);
  pinMode(motor2PWM, OUTPUT);

  nh.initNode();
  nh.subscribe(sub); 
}

void forward()
{
  digitalWrite(motor1IN1, HIGH);
  digitalWrite(motor1IN2, LOW);
  analogWrite(motor1PWM, pwm);
  digitalWrite(motor2IN1, HIGH);
  digitalWrite(motor2IN2, LOW);
  analogWrite(motor2PWM, pwm);
}

void left(){
  digitalWrite(motor1PWM, 0);
  digitalWrite(motor1IN1, HIGH);
  digitalWrite(motor1IN2, LOW);

  digitalWrite(motor2PWM, 120);
  digitalWrite(motor2IN1, HIGH);
  digitalWrite(motor2IN2, LOW);
}

void right() {
  digitalWrite(motor1PWM, 120);
  digitalWrite(motor1IN1, HIGH);
  digitalWrite(motor1IN2, LOW);

  digitalWrite(motor2PWM, 0);
  digitalWrite(motor2IN1, HIGH);
  digitalWrite(motor2IN2, LOW);
}

void emergencyKill() {
  digitalWrite(motor1IN1, LOW);
  digitalWrite(motor1IN2, LOW);
  analogWrite(motor1PWM, 0);
  
  digitalWrite(motor2IN1, LOW);
  digitalWrite(motor2IN2, LOW);
  analogWrite(motor2PWM, 0);
}

void loop()
{  
  nh.spinOnce();
  delay(1);

  Serial.println(offsetFinal); // Display offsetFinal in the serial monitor
  if(kill == true){
    emergencyKill();
  }else{
    if (offsetFinal > 0)
    {
    right();
    }
    else if (offsetFinal < 0)
    {
    left();
    }
    else{
    forward();
    }
  }
  
}