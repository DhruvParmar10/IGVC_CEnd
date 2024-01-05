import rospy
from sensor_msgs.msg import Joy
from std_msgs.msg import Float32

pub_rht = rospy.Publisher('/robot_right_topic', Float32, queue_size=10) 

def controller_callback(data):
    R2 = data.axes[5]
    right = R2

    pub_rht.publish(right)
    rospy.loginfo(right)

def controller_listener():
    rospy.init_node('controller_right')
    rospy.Subscriber('/joy', Joy, controller_callback)
    rospy.spin()

if __name__ == '__main__':
    try:
        controller_listener()
    except rospy.ROSInterruptException:
        pass