#ifndef __RGB2GRAY_CONVERTER_HPP__
#define __RGB2GRAY_CONVERTER_HPP__
#include "../PipelineElements/GenericPluginSiSo.hpp"
#include "../PipelineElements/IGenericPipelineElement.hpp"
#include <yaml-cpp/yaml.h>

namespace FIPP
{
    namespace plugins
    {
        class MetaDataLogger : public pipe::GenericPluginSiSo
        {
            public:
                MetaDataLogger(YAML::Node config, int elemId, std::shared_ptr<FIPP::logging::ILogger> log);
                ~MetaDataLogger();
            protected:
                std::shared_ptr<img::ImageContainer> doCalculation(std::shared_ptr<img::ImageContainer> img);
        };

        class MetaDatLoggerFactory : public pipe::IGenericPipelineElementFactory{
            public:
                std::shared_ptr<pipe::IGenericPipelineElement> makePipeElement(YAML::Node config, std::string elementName, int elemId, std::shared_ptr<FIPP::logging::ILogger> log){
                    return std::make_shared<MetaDataLogger>(config, elemId, log);
                }
                std::string getElementIdentifier(){return std::string("MetaDataLogger");};
                pipe::ElementTypes getElementType(){return pipe::ElementTypes::PLUGIN;};
        };

    }
}
#endif