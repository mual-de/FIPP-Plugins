#ifndef __RGB2GRAY_CONVERTER_HPP__
#define __RGB2GRAY_CONVERTER_HPP__
#include <FIPP/PipelineElements/GenericPluginSiSo.hpp>
#include <FIPP/PipelineElements/IGenericPipelineElement.hpp>
#include <FIPP/Point.hpp>
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
                std::shared_ptr<img::IImageContainer> doCalculation(std::shared_ptr<img::IImageContainer> img);
        };

    }
}
extern "C"
{
    /**
     * @brief Create a pipeline element of type MetaDataLogger
     * 
     * @param pipeElemPtr shared ptr to store the pipeline element after creation
     * @param config yaml configuration of this pipeline element
     * @param elementName special name of this pipeline element
     * @param elemId unique id of this element
     * @param log shared pointer to logger to store the needed information.
     */
    void makePipeElement(std::shared_ptr<FIPP::pipe::IGenericPipelineElement>& pipeElemPtr, YAML::Node config, std::string elementName, int elemId, std::shared_ptr<FIPP::logging::ILogger> log)
    {
        pipeElemPtr = std::make_shared<FIPP::plugins::MetaDataLogger>(config, elemId, log);
    }
    /**
     * @brief Get the Element Identifier object
     * 
     * @param id string variable to store the element identifier
     */
    void getElementIdentifier(std::string &id) { id = std::string("MetaDataLogger"); };
    /**
     * @brief Get the Element Type object
     * 
     * @param type variable to store the element type
     */
    void getElementType(FIPP::pipe::ElementTypes &type) { type = FIPP::pipe::ElementTypes::PLUGIN; };
}
#endif