#include <gtest/gtest.h>
#include <yaml-cpp/yaml.h>
#include <memory>
#include "../../../src/Sinks/TestVideoSink/TestVideoSink.hpp"
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

TEST(Creation, connecting)
{
    YAML::Node sourceConfig;
    sourceConfig["fps"] = 100.00f;
    sourceConfig["name"] = "TestVideoSrc";
    YAML::Node imgConfig;
    YAML::Node dimensions;
    dimensions["x"] = 1024;
    dimensions["y"] = 768;
    imgConfig["dimensions"] = dimensions;
    imgConfig["bitDepthPerPixel"] = 8;
    imgConfig["bytesPerPixel"] = 3;
    imgConfig["imgType"] = "RGB";
    imgConfig["backendType"] = "CPU";
    sourceConfig["imgConfig"] = imgConfig;
    std::shared_ptr<logging::ILogger> srcLog = std::make_shared<logging::UnitTestLogger>(logging::LogLevel::INFO);
    std::shared_ptr<pipe::IGenericSource> src = std::make_shared<plugins::TestVideoSrc>(sourceConfig, 1, srcLog);
    YAML::Node sinkConfig;
    sinkConfig["name"] = "TestVideoSink";
    sinkConfig["time_counter"] = false;
    std::shared_ptr<logging::ILogger> sinkLog = std::make_shared<logging::UnitTestLogger>(logging::LogLevel::INFO);
    std::shared_ptr<pipe::IGenericSink> sink = std::make_shared<plugins::TestVideoSink>(sinkConfig, 2, sinkLog);

    EXPECT_EQ(src->getId(), 1);
    EXPECT_EQ(sink->getId(), 2);
    // Connect Successor
    src->connectSuccessor(sink);
    // Connect Predecessor via ID
    sink->connectPredecessor(1);

    bool interogateRes = src->interogateConnection();
    EXPECT_EQ(interogateRes, true);

    pipe::StartState startRes = src->startElement(-1);
    EXPECT_EQ(startRes, pipe::StartState::STARTED);

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    pipe::StopState stopRes = src->stopElement();
    EXPECT_EQ(stopRes, pipe::StopState::STOPPED);


    std::cout << "SINK OUTPUT" << std::endl << std::endl;
    std::vector<logging::UnitTestEntry> entries = std::static_pointer_cast<logging::UnitTestLogger>(sinkLog)->getEntries();
    for(auto e: entries){
        std::cout << e.fName << " " << e.fnName << " " << e.frameNumber << " " << e.msg << " " << std::endl;
    }
    std::cout << "SOURCE OUTPUT" << std::endl << std::endl;
    entries = std::static_pointer_cast<logging::UnitTestLogger>(srcLog)->getEntries();
    for(auto e: entries){
        std::cout << e.fName << " " << e.fnName << " " << e.frameNumber << " " << e.msg << " " << std::endl;
    }

}
int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}