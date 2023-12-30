#!/usr/bin/env python3
 # license removed for brevity
import rospy
from std_msgs.msg import Bool
 
def talker():
     pub = rospy.Publisher('exit', Bool, queue_size=10)
     rospy.init_node('ekill')
     rate = rospy.Rate(10) # 10hz
     while not rospy.is_shutdown():
          try:
            # Wait for user input
            key = input("(Enter 'exit' to quit): ")
            
            if key.lower() == 'exit':
                spacebar_pressed = True
            else:
                spacebar_pressed = False

            rospy.loginfo(f"Exit: {spacebar_pressed}")
            pub.publish(spacebar_pressed)
            rate.sleep()
          except KeyboardInterrupt:
            break
 
if __name__ == '__main__':
    try:
         talker()
    except rospy.ROSInterruptException:
         pass