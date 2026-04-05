import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Twist, TwistStamped

class CmdVelConverter(Node):

    def __init__(self):
        super().__init__('cmd_vel_converter')

        self.sub = self.create_subscription(
            Twist,
            '/cmd_vel',
            self.callback,
            10)

        self.pub = self.create_publisher(
            TwistStamped,
            '/diff_drive_controller/cmd_vel',
            10)

    def callback(self, msg):
        stamped = TwistStamped()
        stamped.header.stamp = self.get_clock().now().to_msg()
        stamped.header.frame_id = "base_link"
        stamped.twist = msg
        self.pub.publish(stamped)


def main():
    rclpy.init()
    node = CmdVelConverter()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()