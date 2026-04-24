#include "sensor_processor/processor_core.hpp"

namespace sensor_processor {

ProcessorCore::ProcessorCore() : voxel_size_(0.1) {
    // 初始化处理参数
}

cv::Mat ProcessorCore::processImage(const sensor_msgs::msg::Image::ConstSharedPtr& msg) {
    try {
        // 将 ROS 图像消息转换为 OpenCV 格式
        cv_bridge::CvImagePtr cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
        cv::Mat gray_img;
        cv::cvtColor(cv_ptr->image, gray_img, cv::COLOR_BGR2GRAY);
        
        // 简单的边缘检测示例
        cv::Mat edges;
        cv::Canny(gray_img, edges, 50, 150);
        
        return edges;
    } catch (cv_bridge::Exception& e) {
        return cv::Mat();
    }
}

pcl::PointCloud<pcl::PointXYZ>::Ptr ProcessorCore::processPointCloud(const sensor_msgs::msg::PointCloud2::ConstSharedPtr& msg) {
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
    pcl::fromROSMsg(*msg, *cloud);

    // 模拟点云处理：计算质心
    Eigen::Vector4f centroid;
    pcl::compute3DCentroid(*cloud, centroid);
    
    // 这里可以添加更复杂的 PCL 滤波算法，如 VoxelGrid
    return cloud;
}

Eigen::Vector3d ProcessorCore::transformPoint(const Eigen::Vector3d& point, const Eigen::Matrix4d& transform) {
    Eigen::Vector4d homogeneous_point(point.x(), point.y(), point.z(), 1.0);
    Eigen::Vector4d transformed_point = transform * homogeneous_point;
    return transformed_point.head<3>();
}

} // namespace sensor_processor
