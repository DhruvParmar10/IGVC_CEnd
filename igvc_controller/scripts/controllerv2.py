#!/usr/bin/env python3
import rospy
from sensor_msgs.msg import Joy
from std_msgs.msg import Float32

# Create publishers to send control commands
pub_acceleration = rospy.Publisher('/robot_acc_topic', Float32, queue_size=10)
pub_steer = rospy.Publisher('/robot_steer_topic', Float32, queue_size=10)

def controller_callback(data):
    L2 = data.axes[2]
    acceleration = L2
    
    Left_stick = data.axes[0]
    Steer = Left_stick
    
    pub_acceleration.publish(acceleration)
    pub_steer.publish(Steer)
    rospy.loginfo(acceleration)
    rospy.loginfo(Steer)

def controller_listener():
    rospy.init_node('controller_listener')
    rospy.Subscriber('/joy', Joy, controller_callback)  # Subscribe to the controller topic
    rospy.spin()

if __name__ == '__main__':
    try:
        controller_listener()
    except rospy.ROSInterruptException:
        pass
