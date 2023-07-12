#include <gtest/gtest.h>
#include <yaml-cpp/yaml.h>
#include <memory>
#include "../../../src/Plugins/MetaDataLogger/MetaDataLogger.hpp"
#include "Logging/ILogging.hpp"
#include "Logging/UnitTestLogger.hpp"
#include "Logging/TestLogger.hpp"
#include "Point.hpp"
#include "ImageContainer/IImageContainer.hpp"
#include "ImageContainer/IImagePool.hpp"
#include "ImageContainer/ImagePoolFactory.hpp"
#include "ImageContainer/ImageFormat.hpp"
#include <iostream>
#include <chrono>
#include <thread>

using namespace FIPP;
// Demonstrate some basic assertions.
TEST(Creation, basics)
{
    // Expect equality.
    YAML::Node node = YAML::Load("[1, 2, 3]");
    std::shared_ptr<logging::ILogger> log = std::make_shared<logging::UnitTestLogger>(logging::LogLevel::WARNING);
    std::shared_ptr<pipe::IGenericPlugin> pe = std::make_shared<plugins::MetaDataLogger>(node, 1, log);
    EXPECT_EQ(pe->getId(), 1);
    EXPECT_EQ(pe->getName(), "MetaDataLogger");
}

TEST(Creation, bounding)
{
    // Expect equality.
    Point<unsigned int> size(10, 10);
    img::ImageContainerConfig config;
    config.backend.type = img::BackendType::CPU;
    config.dimensions = size;
    config.imgFormat.bitDepthPerPixel=8;
    config.imgFormat.bytesPerPixel = 1;
    config.imgFormat.imgType = img::ImageType::GRAY;
    std::unique_ptr<img::IImagePool> pPool = img::getImagePool(1, config);
    std::shared_ptr<img::IImageContainer> pImg = pPool->getNextFreeImage();
    YAML::Node node = YAML::Load("[1, 2, 3]");
    std::shared_ptr<logging::ILogger> log = std::make_shared<logging::UnitTestLogger>(logging::LogLevel::INFO);
    std::shared_ptr<pipe::IGenericPlugin> pe = std::make_shared<plugins::MetaDataLogger>(node, 1, log);
    pe->connectPredecessor(0);
    std::cout << "connected predecessor" << std::endl;
    bool res = pe->startElement(0);
    while(pe->getState() != pipe::ElementState::RUNNING){};
    pe->addImageToInputPipe(pImg);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    pe->stopElement();
    std::vector<logging::UnitTestEntry> entries = std::static_pointer_cast<logging::UnitTestLogger>(log)->getEntriesForFNameAndFunction("MetaDataLogger", "doCalculation");
    EXPECT_EQ(entries.size(), 2);
    EXPECT_EQ(entries.at(0).msg.compare("Dims: x: 10 - y: 10"), 0);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}