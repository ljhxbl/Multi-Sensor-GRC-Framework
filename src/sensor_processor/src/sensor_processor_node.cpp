#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "sensor_msgs/msg/point_cloud2.hpp"
#include "message_filters/subscriber.h"
#include "message_filters/time_synchronizer.h"
#include "message_filters/sync_policies/approximate_time.h"

#include "sensor_processor/processor_core.hpp"

namespace sensor_processor {

class SensorProcessorNode : public rclcpp::Node {
public:
    SensorProcessorNode() : Node("sensor_processor_node") {
        // 初始化参数
        this->declare_parameter("image_topic", "/camera/image_raw");
        this->declare_parameter("lidar_topic", "/lidar/points");

        std::string image_topic = this->get_parameter("image_topic").as_string();
        std::string lidar_topic = this->get_parameter("lidar_topic").as_string();

        // 初始化处理核心
        processor_ = std::make_unique<ProcessorCore>();

        // 设置订阅者（用于时间同步）
        image_sub_.subscribe(this, image_topic);
        lidar_sub_.subscribe(this, lidar_topic);

        // 使用近似时间同步策略（机器人常用，因为不同传感器频率不同）
        sync_ = std::make_shared<message_filters::Synchronizer<SyncPolicy>>(
            SyncPolicy(10), image_sub_, lidar_sub_);
        
        sync_->registerCallback(std::bind(&SensorProcessorNode::combinedCallback, this, 
            std::placeholders::_1, std::placeholders::_2));

        RCLCPP_INFO(this->get_logger(), "Sensor Processor Node initialized. Syncing: %s and %s", 
                    image_topic.c_str(), lidar_topic.c_str());
    }

private:
    void combinedCallback(const sensor_msgs::msg::Image::ConstSharedPtr& image_msg,
                          const sensor_msgs::msg::PointCloud2::ConstSharedPtr& lidar_msg) {
        RCLCPP_DEBUG(this->get_logger(), "Received synchronized image and pointcloud");

        // 1. 处理图像
        cv::Mat processed_img = processor_->processImage(image_msg);

        // 2. 处理点云
        auto processed_cloud = processor_->processPointCloud(lidar_msg);

        // 3. 示例：使用 Eigen 进行简单的矩阵运算（如外参变换模拟）
        Eigen::Matrix4d mock_extrinsic = Eigen::Matrix4d::Identity();
        Eigen::Vector3d test_point(1.0, 0.0, 0.0);
        Eigen::Vector3d transformed = processor_->transformPoint(test_point, mock_extrinsic);

        // 这里可以添加发布处理后结果的代码
        RCLCPP_INFO_ONCE(this->get_logger(), "Data processing pipeline is running smoothly.");
    }

    // 成员变量
    std::unique_ptr<ProcessorCore> processor_;

    // 消息同步相关
    message_filters::Subscriber<sensor_msgs::msg::Image> image_sub_;
    message_filters::Subscriber<sensor_msgs::msg::PointCloud2> lidar_sub_;
    
    typedef message_filters::sync_policies::ApproximateTime<
        sensor_msgs::msg::Image, sensor_msgs::msg::PointCloud2> SyncPolicy;
    std::shared_ptr<message_filters::Synchronizer<SyncPolicy>> sync_;
};

} // namespace sensor_processor

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<sensor_processor::SensorProcessorNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
