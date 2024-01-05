#include <ros.h>
#include <std_msgs/Float32.h>

ros::NodeHandle  nh;
std_msgs::Float32 float_msg;

float offsetFinal = 15000.0;
float acc = 1.0;
float steer = 0.0;

void avgCallback(const std_msgs::Float32& msg)
{
  offsetFinal = msg.data;
}

void rightCallback(const std_msgs::Float32& msg)
{
  right = msg.data;
}

void leftCallback(const std_msgs::Float32& msg)
{
  left = msg.data;
}

ros::Subscriber<std_msgs::Float32> off("/float_data", &avgCallback);
ros::Subscriber<std_msgs::Float32> lft("/robot_right_topic", &rightCallback);
ros::Subscriber<std_msgs::Float32> rht("/robot_left_topic", &leftCallback);

const int motor1IN1 = 11;
const int motor1IN2 = 9;  
const int motor1PWM = 13;
const int motor2IN1 = 10;
const int motor2IN2 = 8;
const int motor2PWM = 12;

int pwm = 255;
int pwm1 = 84;
int pwm2 = 84;

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
  nh.subscribe(off);
  nh.subscribe(lft);
  nh.subscribe(rht); 
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


void loop()
{  
  nh.spinOnce();
  delay(1);

  Serial.println(offsetFinal);
  int motorLeft = map(acc, 1, -1, 0, 205);
  int motorRight = map(steer, 1, -1, 0, 205);
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
  
  if (motorLeft > 50 || motorRight > 50) {
      digitalWrite(motor1IN1, HIGH);
      digitalWrite(motor1IN2, LOW);
      analogWrite(motor1PWM, motorRight); // right wheel
      digitalWrite(motor2IN1, HIGH);
      digitalWrite(motor2IN2, LOW);
      analogWrite(motor2PWM, motorLeft); // left wheel
  }
  else{
      // Stop the motors if the value is below a certain threshold
      digitalWrite(motor1IN1, HIGH);
      digitalWrite(motor1IN2, LOW);
      analogWrite(motor1PWM, 0);
      digitalWrite(motor2IN1, HIGH);
      digitalWrite(motor2IN2, LOW);
      analogWrite(motor2PWM, 0);
  }
}