#include "TestVideoSrc.hpp"
#include <FIPP/ImageContainer/ImagePool.hpp>

#define PLUGIN_NAME "TestVideoSrc"

#define LOG(lvl, msg) this->m_log->log(lvl, msg, PLUGIN_NAME, __func__, __LINE__, this->m_frameNumber);
TestVideoSrc::TestVideoSrc(YAML::Node config, int elemId, std::shared_ptr<FIPP::logging::ILogger> log) : GenericSource(config["name"].as<std::string>(), elemId, log){
    if(config["fps"]){
        this->m_fps = config["fps"].as<float>();
    }
    else{
        this->m_fps = 25.00f;
    }
    FIPP::img::ImageContainerConfig imgConfig = FIPP::img::getContainerConfigFromYaml(config["imgConfig"]);

    this->m_pool = std::make_unique<FIPP::img::ImagePool>(10, config);
}


void TestVideoSrc::doCalculation(std::shared_ptr<FIPP::img::ImageContainer> img){
    LOG()
}
void TestVideoSrc::initializeInterfaces(){

}
void TestVideoSrc::closeInterfaces(){

}