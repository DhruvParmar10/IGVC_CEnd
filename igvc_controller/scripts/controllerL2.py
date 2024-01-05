import rospy
from sensor_msgs.msg import Joy
from std_msgs.msg import Float32

pub_lft = rospy.Publisher('/robot_left_topic', Float32, queue_size=10)

def controller_callback(data):
    L2 = data.axes[2]
    left = L2

    pub_lft.publish(left)
    rospy.loginfo(left)

def controller_listener():
    rospy.init_node('controller_left')
    rospy.Subscriber('/joy', Joy, controller_callback)
    rospy.spin()

if __name__ == '__main__':
    try:
        controller_listener()
    except rospy.ROSInterruptException:
        pass