#include <gtest/gtest.h>
#include <yaml-cpp/yaml.h>
#include <memory>
#include "../../../src/Sources/TestVideoSrc/TestVideoSrc.hpp"
#include "Logging/ILogging.hpp"
#include "Logging/UnitTestLogger.hpp"
#include "Logging/TestLogger.hpp"
#include "Point.hpp"
#include "ImageContainer/ImageContainer.hpp"
#include "ImageContainer/ImageContainerCPU.hpp"
#include "ImageContainer/ImageFormat.hpp"
#include <iostream>
#include <chrono>
#include <thread>

using namespace FIPP;
// Demonstrate some basic assertions.
TEST(Creation, basics)
{
    // Expect equality.
    YAML::Node node;
    node["fps"] = 20.00f;
    node["name"] = "TestVideoSrc";
    YAML::Node imgConfig;
    YAML::Node dimensions;
    dimensions["x"] = 1024;
    dimensions["y"] = 768;
    imgConfig["dimensions"] = dimensions;
    imgConfig["bitDepthPerPixel"] = 8;
    imgConfig["bytesPerPixel"] = 1;
    imgConfig["imgType"] = "GRAY";
    imgConfig["backendType"] = "CPU";
    node["imgConfig"] = imgConfig;
    std::shared_ptr<logging::ILogger> log = std::make_shared<logging::UnitTestLogger>(logging::LogLevel::WARNING);
    std::shared_ptr<pipe::IGenericSource> pe = std::make_shared<plugins::TestVideoSrc>(node, 1, log);
    EXPECT_EQ(pe->getId(), 1);
    EXPECT_EQ(pe->getName(), "TestVideoSrc");
}

TEST(Creation, bounding)
{
    // Expect equality.
    Point<unsigned int> size(10, 10);
    img::ImageFormat format;
    format.bitDepthPerPixel=8;
    format.bytesPerPixel = 1;
    format.imgType = img::ImageType::GRAY;
    YAML::Node node = YAML::Load("[1, 2, 3]");
    std::shared_ptr<img::ImageContainer> pImg = std::make_shared<img::ImageContainerCPU>(size, format,0);
    std::shared_ptr<logging::ILogger> log = std::make_shared<logging::UnitTestLogger>(logging::LogLevel::INFO);
    std::shared_ptr<pipe::IGenericSource> pe = std::make_shared<plugins::TestVideoSrc>(node, 1, log);
    std::cout << "connected predecessor" << std::endl;
    img::ImageContainerConfig conf;
    conf.imgFormat = format;
    img::Backend back;
    back.flags = img::BackendFlags::CPU_ONLY;
    back.type = img::BackendType::CPU;
    conf.backend = back;
    bool res = pe->startElement(0);
    while(pe->getState() != pipe::ElementState::RUNNING){};
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    pe->stopElement();
    
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}