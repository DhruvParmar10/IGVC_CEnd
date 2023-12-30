#include <ros.h>
#include <std_msgs/Float32.h>

ros::NodeHandle  nh;
std_msgs::Float32 avg_msg;

float offsetFinal = 15000;
float acc = 0;
float steer = 0;

void avgCallback(const std_msgs::Float32& msg)
{
  offsetFinal = msg.data;
}
void accCallback(const std_msgs::Float32& msg)
{
  acc = msg.data;
}
void steerCallback(const std_msgs::Float32& msg)
{
  steer = msg.data;
}

ros::Subscriber<std_msgs::Float32> off("/float_data", &avgCallback);
ros::Subscriber<std_msgs::Float32> Accelaration("/robot_acc_topic", &accCallback);
ros::Subscriber<std_msgs::Float32> Steering("/robot_acc_topic", &accCallback);

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
  nh.subscribe(off);
  nh.subscribe(Accelaration);
  nh.subscribe(Steering); 
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

  Serial.println(offsetFinal); // Display offsetFinal in the serial monitor
  
  int motorSpeed = map(acc, 1, -1, 0, 255);
  int motorSteer = map(steer, -1, 1, 0, 255);
  if(offsetFinal != 15000){
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
  else {
    
    if (motorSpeed > 50) {  // Adjust the threshold value as needed
      // Control motors based on the received value
      digitalWrite(motor1IN1, HIGH);
      digitalWrite(motor1IN2, LOW);
      analogWrite(motor1PWM, motorSpeed);
      digitalWrite(motor2IN1, HIGH);
      digitalWrite(motor2IN2, LOW);
      analogWrite(motor2PWM, motorSpeed);
      if (motorSteer > 126){
        digitalWrite(motor1PWM, 50);
        digitalWrite(motor1IN1, HIGH);
        digitalWrite(motor1IN2, LOW);

        digitalWrite(motor2PWM, motorSteer);
        digitalWrite(motor2IN1, HIGH);
        digitalWrite(motor2IN2, LOW);
      } 
      else {
        digitalWrite(motor1PWM, motorSteer);
        digitalWrite(motor1IN1, HIGH);
        digitalWrite(motor1IN2, LOW);

        digitalWrite(motor2PWM, 50);
        digitalWrite(motor2IN1, HIGH);
        digitalWrite(motor2IN2, LOW);
      }
    }
    else{
      // Stop the motors if the value is below a certain threshold
      analogWrite(motor1PWM, 0);
      analogWrite(motor2PWM, 0);
    }
  }  
}