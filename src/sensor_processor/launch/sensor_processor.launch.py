import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    # 获取配置文件的路径
    config = os.path.join(
        get_package_share_directory('sensor_processor'),
        'config',
        'params.yaml'
    )

    return LaunchDescription([
        Node(
            package='sensor_processor',
            executable='sensor_processor_node',
            name='sensor_processor_node',
            output='screen',
            parameters=[config]
        )
    ])
