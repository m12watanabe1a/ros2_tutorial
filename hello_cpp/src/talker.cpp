// Copyright 2021 WATANABE Aoi
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <chrono>
#include <cstdio>
#include <memory>
#include <string>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

using namespace std::chrono_literals;

class Talker : public rclcpp::Node
{
public:
  explicit Talker(const std::string &topic_name);

private:
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_;
  rclcpp::TimerBase::SharedPtr timer_;

  void publish_msg();
};

Talker::Talker(const std::string &topic_name)
    : Node("talker")
{

  // chatterトピックの送信設定
  rclcpp::QoS qos(rclcpp::KeepLast(10));
  pub_ = create_publisher<std_msgs::msg::String>(topic_name, qos);
  timer_ = create_wall_timer(100ms, std::bind(&Talker::publish_msg, this));
}

void Talker::publish_msg()
{
  auto msg = std::make_unique<std_msgs::msg::String>();
  msg->data = "Hello world!";

  RCLCPP_INFO(this->get_logger(), "%s", msg->data.c_str());
  this->pub_->publish(std::move(msg));
}

int main(int argc, char *argv[])
{
  setvbuf(stdout, NULL, _IONBF, BUFSIZ);
  rclcpp::init(argc, argv);

  auto node = std::make_shared<Talker>("chatter");
  rclcpp::spin(node);
  rclcpp::shutdown();

  return EXIT_SUCCESS;
}