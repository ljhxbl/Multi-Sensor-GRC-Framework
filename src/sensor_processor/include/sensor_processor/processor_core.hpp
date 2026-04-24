#ifndef SENSOR_PROCESSOR__PROCESSOR_CORE_HPP_
#define SENSOR_PROCESSOR__PROCESSOR_CORE_HPP_

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl_conversions/pcl_conversions.h>
#include <Eigen/Dense>

namespace sensor_processor {

class ProcessorCore {
public:
    ProcessorCore();
    ~ProcessorCore() = default;

    /**
     * @brief 处理图像数据，进行灰度化和简单的边缘检测
     * @param msg ROS2 图像消息
     * @return 处理后的 OpenCV 矩阵
     */
    cv::Mat processImage(const sensor_msgs::msg::Image::ConstSharedPtr& msg);

    /**
     * @brief 处理点云数据，进行下采样和地面过滤（模拟）
     * @param msg ROS2 点云消息
     * @return 处理后的 PCL 点云对象
     */
    pcl::PointCloud<pcl::PointXYZ>::Ptr processPointCloud(const sensor_msgs::msg::PointCloud2::ConstSharedPtr& msg);

    /**
     * @brief 使用 Eigen 进行坐标变换计算
     * @param point 原始点
     * @param transform 变换矩阵
     * @return 变换后的点
     */
    Eigen::Vector3d transformPoint(const Eigen::Vector3d& point, const Eigen::Matrix4d& transform);

private:
    // 可以在这里添加参数，如滤波器阈值等
    double voxel_size_;
};

} // namespace sensor_processor

#endif // SENSOR_PROCESSOR__PROCESSOR_CORE_HPP_
