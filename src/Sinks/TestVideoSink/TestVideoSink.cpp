#include "TestVideoSink.hpp"
#include <FIPP/Logging/ILogging.hpp>
using namespace FIPP;
using namespace FIPP::plugins;
#include <FIPP/ImageContainer/ImageFormat.hpp>

TestVideoSink::TestVideoSink(YAML::Node config, int elemId, std::shared_ptr<FIPP::logging::ILogger> log) : GenericSinkSi(config["name"].as<std::string>(), elemId, log)
{
    if (config["time_counter"])
    {
        this->m_enableCounter = config["time_counter"].as<bool>();
    }
    else
    {
        this->m_enableCounter = false;
    }
}

void TestVideoSink::closeInterfaces(){
    LOG(logging::CONFIG, "Close uneeded interfaces!");
}
void TestVideoSink::initializeInterfaces(){
    LOG(logging::CONFIG, "Initialize uneeded interfaces!");
}

void TestVideoSink::doCalculation(std::shared_ptr<img::ImageContainer> img)
{
    LOG(logging::CONFIG, "Received image from pipeline");
    this->m_frameNumber = img->getFrameNumber();
    LOG(logging::DEBUG, "Image Size: " + img->getDims().asStr());
    
}