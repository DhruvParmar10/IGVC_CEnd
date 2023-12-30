#!/usr/bin/env python3
import rospy
from sensor_msgs.msg import Joy
from std_msgs.msg import Float32
import numpy

# Create a publisher to send control commands
pub = rospy.Publisher('/robot_control_topic', Float32, queue_size=10)  # Replace 'YourControlMessageType' with your actual control message type 

def controller_callback(data):
    L2 = data.axes[2]
    acceleration = L2
    
    Left_stick = data.axes[0]
    Steer = Left_stick

    pub.publish(acceleration)
    pub.publish(Steer)


    # print("Axes:",data.axes)
    # print("Buttons",data.buttons)

def controller_listener():
    rospy.init_node('controller_listener')
    rospy.Subscriber('/joy', Joy, controller_callback)  # Subscribe to the controller topic
    rospy.spin()

if __name__ == '__main__':
    try:
        controller_listener()
    except rospy.ROSInterruptException:
        pass
