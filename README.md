# ROS2 C++ 多传感器数据处理框架

## 项目简介

本项目旨在为机器人实机部署需求，自主搭建一个基于 ROS2 (Humble) 和 C++ 的高性能底层通信与数据处理框架。该框架实现了对相机图像 (Image) 和激光雷达点云 (PointCloud2) 数据的订阅、时间同步与回调处理，并结合 OpenCV 与 Eigen 库进行基础的数据预处理与矩阵运算。通过此项目，成功打通了从纯数学算法到机器人标准操作系统（ROS2）的 C++ 部署链路，强化了在 Linux 环境下的工程落地能力。

**关键词**: ROS2 Humble, C++, 多传感器融合, 图像处理, 点云处理, OpenCV, PCL, Eigen, 时间同步, 机器人软件开发

## 技术亮点

*   **ROS2 Humble**: 基于最新稳定版 ROS2 Humble，利用其高性能通信机制和现代 C++ 特性。
*   **C++17/20**: 采用现代 C++ 编程范式，确保代码的效率、可读性和可维护性。
*   **多传感器数据同步**: 使用 `message_filters` 库实现相机图像和激光雷达点云的近似时间同步，为传感器融合提供基础。
*   **图像处理**: 结合 `OpenCV` 库，在 C++ 回调函数中实现图像的灰度化、边缘检测等基础处理。
*   **点云处理**: 集成 `PCL (Point Cloud Library)`，对点云数据进行基本操作，例如质心计算，并为后续更复杂的滤波和特征提取预留接口。
*   **矩阵运算**: 利用 `Eigen` 库进行高效的线性代数运算，例如模拟传感器坐标变换，展示在机器人运动学/动力学中的应用潜力。
*   **模块化设计**: 采用 ROS2 节点和独立处理核心 (`ProcessorCore`) 的模块化设计，提高代码复用性和系统可扩展性。
*   **CMake 构建系统**: 完善的 CMakeLists.txt 配置，实现独立工程编译和 ROS2 包管理。

## 项目结构

```
multi_sensor_framework/
├── src/
│   └── sensor_processor/
│       ├── CMakeLists.txt
│       ├── package.xml
│       ├── include/
│       │   └── sensor_processor/
│       │       └── processor_core.hpp
│       ├── src/
│       │   ├── processor_core.cpp
│       │   └── sensor_processor_node.cpp
│       ├── launch/
│       │   └── sensor_processor.launch.py
│       └── config/
│           └── params.yaml
└── README.md
└── .gitignore
```

*   `sensor_processor/`: ROS2 功能包根目录。
*   `sensor_processor/include/sensor_processor/processor_core.hpp`: 定义核心数据处理逻辑的头文件，包含图像、点云处理及 Eigen 矩阵运算的接口。
*   `sensor_processor/src/processor_core.cpp`: `ProcessorCore` 类的实现，封装了具体的 OpenCV、PCL 和 Eigen 操作。
*   `sensor_processor/src/sensor_processor_node.cpp`: ROS2 节点的主文件，负责订阅传感器话题、实现时间同步回调，并调用 `ProcessorCore` 进行数据处理。
*   `sensor_processor/launch/sensor_processor.launch.py`: ROS2 启动文件，用于一键启动 `sensor_processor_node` 并加载参数。
*   `sensor_processor/config/params.yaml`: 节点参数配置文件，方便调整传感器话题名称等。
*   `CMakeLists.txt`: CMake 构建脚本，管理编译选项、依赖和可执行文件。
*   `package.xml`: ROS2 包的元数据文件，声明了包的依赖和导出信息。

## 环境配置与依赖

*   **操作系统**: Ubuntu 22.04 LTS
*   **ROS2 版本**: Humble Hawksbill
*   **C++ 标准**: C++17 或更高
*   **主要库**: 
    *   `rclcpp`: ROS2 C++ 客户端库
    *   `sensor_msgs`: ROS2 标准传感器消息类型
    *   `cv_bridge`: ROS2 图像消息与 OpenCV 图像格式转换库
    *   `image_transport`: ROS2 图像传输优化库
    *   `OpenCV`: 计算机视觉库
    *   `PCL (Point Cloud Library)`: 点云处理库
    *   `pcl_conversions`: ROS2 点云消息与 PCL 点云格式转换库
    *   `Eigen`: 高性能 C++ 模板库，用于线性代数、矩阵和向量运算
    *   `message_filters`: ROS2 消息过滤器，用于实现时间同步

确保您的 Ubuntu 22.04 系统已安装 ROS2 Humble，并且已通过 `sudo apt install ros-humble-desktop` 或类似方式安装了必要的开发工具和库。特别是 OpenCV 和 PCL，通常会随 ROS2 的桌面版一同安装，但如果遇到编译问题，请确保它们及其开发文件已正确安装。

```bash
sudo apt update
sudo apt install ros-humble-cv-bridge ros-humble-image-transport ros-humble-pcl-ros ros-humble-pcl-conversions ros-humble-message-filters libeigen3-dev
```

## 构建指南

1.  **克隆仓库** (如果已上传到 GitHub):
    ```bash
    # git clone [your_repo_url]
    # cd multi_sensor_framework
    ```
    或者，直接进入您创建的 `multi_sensor_framework` 目录。

2.  **进入工作空间并编译**: 
    ```bash
    cd ~/multi_sensor_framework
    colcon build --packages-select sensor_processor
    ```

3.  **设置环境变量**: 
    ```bash
    source install/setup.bash
    ```
    建议将此命令添加到您的 `~/.bashrc` 或 `~/.zshrc` 文件中，以便每次打开终端时自动加载。

## 运行示例

在成功编译并设置环境变量后，您可以通过 ROS2 launch 文件启动节点：

```bash
ros2 launch sensor_processor sensor_processor.launch.py
```

此命令将启动 `sensor_processor_node`。由于目前没有实际的传感器数据发布，节点会等待话题 `/camera/image_raw` 和 `/lidar/points`。您可以使用 ROS2 的 `ros2 topic pub` 命令模拟发布数据，或者在仿真环境（如 Gazebo）中运行。

**示例：模拟发布图像数据**

```bash
# 在新终端中，确保已 source install/setup.bash
ros2 topic pub /camera/image_raw sensor_msgs/msg/Image "{header: {stamp: {sec: 0, nanosec: 0}, frame_id: 'camera_link'}, height: 480, width: 640, encoding: 'bgr8', is_bigendian: 0, step: 1920, data: [0]*307200}" -r 1
```

**示例：模拟发布点云数据**

```bash
# 在新终端中，确保已 source install/setup.bash
ros2 topic pub /lidar/points sensor_msgs/msg/PointCloud2 "{header: {stamp: {sec: 0, nanosec: 0}, frame_id: 'lidar_link'}, height: 1, width: 100, fields: [{name: 'x', offset: 0, datatype: 7, count: 1}, {name: 'y', offset: 4, datatype: 7, count: 1}, {name: 'z', offset: 8, datatype: 7, count: 1}], is_bigendian: 0, point_step: 12, row_step: 1200, data: [0]*1200, is_dense: false}" -r 1
```

当节点接收到同步数据时，您将在终端看到类似 `Data processing pipeline is running smoothly.` 的输出信息。

## 核心代码解析

### `processor_core.hpp` / `processor_core.cpp`

这个类封装了所有与 ROS2 无关的纯数据处理逻辑，体现了良好的模块化设计。它提供了以下核心功能：

*   `processImage(const sensor_msgs::msg::Image::ConstSharedPtr& msg)`: 将 ROS2 图像消息转换为 OpenCV `cv::Mat`，并执行灰度化和 Canny 边缘检测。这展示了 `cv_bridge` 的使用以及基础图像处理能力。
*   `processPointCloud(const sensor_msgs::msg::PointCloud2::ConstSharedPtr& msg)`: 将 ROS2 点云消息转换为 PCL `PointCloud` 对象，并计算点云质心。这展示了 `pcl_conversions` 的使用以及 PCL 库的基本应用。
*   `transformPoint(const Eigen::Vector3d& point, const Eigen::Matrix4d& transform)`: 使用 `Eigen::Matrix4d` 对三维点进行坐标变换。这是机器人领域中常见的操作，例如将传感器数据从传感器坐标系转换到机器人基坐标系。

### `sensor_processor_node.cpp`

这是 ROS2 节点的主实现，负责协调传感器数据的接收和处理：

*   **参数声明与获取**: 通过 `declare_parameter` 和 `get_parameter` 机制，实现了节点参数的动态配置，例如传感器话题名称，增强了节点的灵活性。
*   **`message_filters::Synchronizer`**: 核心亮点之一。它使用 `ApproximateTime` 策略同步来自不同传感器（图像和点云）的消息。这对于需要同时处理多个传感器输入的机器人应用至关重要，解决了传感器数据到达时间不一致的问题。
*   **回调函数 `combinedCallback`**: 当同步器成功匹配到一组图像和点云消息时，此回调函数被触发。它将消息转发给 `ProcessorCore` 进行实际的数据处理，并打印处理状态。
*   **现代 C++ 特性**: 广泛使用了 `std::unique_ptr` 进行资源管理，`std::bind` 绑定回调函数，以及 ROS2 的 `rclcpp::Node` 接口，体现了对现代 C++ 和 ROS2 开发规范的掌握。

## 潜在改进与未来工作

1.  **发布处理结果**: 当前节点仅处理数据，但未发布处理后的图像或点云。可以添加 `image_transport::Publisher` 和 `rclcpp::Publisher<sensor_msgs::msg::PointCloud2>` 来发布处理结果，方便可视化和后续模块使用。
2.  **更复杂的传感器融合**: 在 `combinedCallback` 中实现更复杂的融合算法，例如基于图像特征和点云数据的 3D 物体检测或位姿估计。
3.  **参数服务器集成**: 将 `ProcessorCore` 中的处理参数（如 `voxel_size_`）通过 ROS2 参数服务器进行配置，提高灵活性。
4.  **单元测试与集成测试**: 为 `ProcessorCore` 编写单元测试，为整个 ROS2 节点编写集成测试，确保代码质量和功能正确性。
5.  **性能优化**: 对图像和点云处理流程进行性能分析和优化，例如利用多线程、GPU 加速 (CUDA/OpenCL) 或 SIMD 指令集。
6.  **错误处理与鲁棒性**: 增加更完善的错误处理机制，例如处理 `cv_bridge` 转换失败、PCL 异常等情况。

## 作者

**Manus AI** (代为生成)

**联系方式**: your_email@example.com (请替换为您的邮箱)

## 许可证

本项目采用 Apache License 2.0 许可证。详情请参阅项目根目录下的 `LICENSE` 文件（如果存在）。
