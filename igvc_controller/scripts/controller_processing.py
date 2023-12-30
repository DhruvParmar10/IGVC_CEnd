#!/usr/bin/env python3
import rospy
from std_msgs.msg import String
from evdev import InputDevice, list_devices, ecodes

class ControllerInputs():
    def __init__(self):
        self.action = ""
        
    def handle_axis(self, event):
        # Handle joystick axes
        if event.code == ecodes.ABS_X:
            if event.value > 150:
                self.action = "Right"
            elif event.value < 100:
                self.action = "Left"
            else:
                self.action = ""
        elif event.code == ecodes.ABS_Y:
            if event.value > 200:
                self.action = "Down"
            elif event.value < 100:
                self.action = "Up"
            else:
                self.action = ""
        elif event.code == ecodes.ABS_Z:
            if event.value > 150:
                self.action = "Go"
            else:
                self.action = ""

    def handle_button(self, event):
        # Handle button presses
        if event.code == ecodes.BTN_SOUTH:
            self.action = "X"
        elif event.code == ecodes.BTN_WEST:
            self.action = "STOP"

def main():
    devices = [InputDevice(path) for path in list_devices()]
    ds4 = None

    for dev in devices:
        if dev.name == 'Sony Computer Entertainment Wireless Controller':
            ds4 = dev
            break
    if ds4 is None:
        print("DualShock 4 controller not found.")
        return

    print("DualShock 4 controller found.")
    controller = ControllerInputs()
    
    rospy.init_node('input_daf')
    pub = rospy.Publisher('input_daf', String, queue_size=10)
    
    rate = rospy.Rate(75)
    print("input_daf START")
    
    

    while not rospy.is_shutdown():
        for event in ds4.read_loop():
            if event.type == ecodes.EV_ABS:
                controller.handle_axis(event)
            elif event.type == ecodes.EV_KEY:
                controller.handle_button(event)

            rospy.loginfo(controller.action)
            pub.publish(controller.action)
            rate.sleep()

if __name__ == "__main__":
    try:
        main()
    except rospy.ROSInterruptException:
        pass
