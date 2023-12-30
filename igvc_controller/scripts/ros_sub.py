#!/usr/bin/env python3
import rospy
from evdev import InputDevice, list_devices
from subprocess import Popen, PIPE, STDOUT

def is_controller_connected():
    devices = [InputDevice(path) for path in list_devices()]
    for dev in devices:
        if dev.name == 'Sony Computer Entertainment Wireless Controller':
            return True
    return False

def check_controller_status(event):
    controller_connected = is_controller_connected()
    rospy.loginfo(f"Changed igvc_controller default value to {controller_connected}")

if __name__ == "__main__":
    rospy.init_node('controller_monitor')
    controller_connected = is_controller_connected()

    if controller_connected == True:
        # Run node A
        rospy.loginfo("Launching Node A")
        Popen(['roslaunch', 'igvc_controller', 'nodeA.launch'])
    else:
        # Run node B
        rospy.loginfo("Launching Node B")
        Popen(['roslaunch', 'igvc_controller', 'nodeB.launch'])
    rospy.spin()