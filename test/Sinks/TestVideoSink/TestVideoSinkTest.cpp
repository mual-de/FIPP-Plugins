#include <gtest/gtest.h>
#include <yaml-cpp/yaml.h>
#include <memory>
#include "../../../src/Sinks/TestVideoSink/TestVideoSink.hpp"
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
    node["name"] = "TestVideoSink";
    node["time_counter"] = false;
    std::shared_ptr<logging::ILogger> log = std::make_shared<logging::UnitTestLogger>(logging::LogLevel::WARNING);
    std::shared_ptr<pipe::IGenericSink> pe = std::make_shared<plugins::TestVideoSink>(node, 1, log);
    EXPECT_EQ(pe->getId(), 1);
    EXPECT_EQ(pe->getName(), "TestVideoSink");
    
}

TEST(Creation, bounding)
{
    // Expect equality.
    Point<unsigned int> size(10, 10);
    img::ImageFormat format;
    format.bitDepthPerPixel=8;
    format.bytesPerPixel = 1;
    format.imgType = img::ImageType::GRAY;
    YAML::Node node;
    node["name"] = "TestVideoSink";
    node["time_counter"] = false;
    std::shared_ptr<img::ImageContainer> pImg = std::make_shared<img::ImageContainerCPU>(size, format,0);
    pImg->setFrameNumber(0);
    std::shared_ptr<logging::ILogger> log = std::make_shared<logging::UnitTestLogger>(logging::LogLevel::INFO);
    std::shared_ptr<pipe::IGenericSink> pe = std::make_shared<plugins::TestVideoSink>(node, 1, log);
    std::cout << "connected predecessor" << std::endl;
    img::ImageContainerConfig conf;
    conf.imgFormat = format;
    img::Backend back;
    back.flags = img::BackendFlags::CPU_ONLY;
    back.type = img::BackendType::CPU;
    conf.backend = back;
    pe->connectPredecessor(0);
    bool iCon = pe->interogateConnection(conf, 0);
    EXPECT_EQ(iCon, true);

    FIPP::pipe::StartState res = pe->startElement(0);
    EXPECT_EQ(res, FIPP::pipe::StartState::STARTED);
    while(pe->getState() != pipe::ElementState::RUNNING){};
    pe->addImageToInputPipe(pImg);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    FIPP::pipe::StopState resStop = pe->stopElement();
    EXPECT_EQ(resStop, FIPP::pipe::StopState::STOPPED);
    std::vector<logging::UnitTestEntry> entries = std::static_pointer_cast<logging::UnitTestLogger>(log)->getEntries();
    for(auto e: entries){
        std::cout << e.fName << " " << e.fnName << " " << e.frameNumber << " " << e.msg << " " << std::endl;
    }
}
int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}