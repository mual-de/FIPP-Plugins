#include "Plugins/MetaDataLogger.hpp"
#include <string.h>
using namespace FIPP;
using namespace FIPP::plugins;
using namespace FIPP::logging;
#define PLUGIN_NAME "MetaDataLogger"

#define LOG(lvl, msg) this->m_log->log(lvl, msg, PLUGIN_NAME, __func__, __LINE__, this->m_frameNumber);

MetaDataLogger::MetaDataLogger(YAML::Node config, int elemId, std::shared_ptr<FIPP::logging::ILogger> log) : GenericPluginSiSo(PLUGIN_NAME, elemId, log){
    LOG(LogLevel::INFO, "Plugin created");
    this->m_filterActivated = INPUT_FILTER_ALLOW_ALL_INPUTS;
}

MetaDataLogger::~MetaDataLogger(){
    LOG(LogLevel::INFO, "Plugin shutdown");
}

std::shared_ptr<img::ImageContainer> MetaDataLogger::doCalculation(std::shared_ptr<img::ImageContainer> img){
    LOG(LogLevel::INFO, "Dims: " + img->getDims().asStr());
    LOG(LogLevel::INFO, "BackendType: " + std::to_string(img->getBackendType()));
    return img;
}