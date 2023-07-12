#ifndef __TEST_VIDEO_SINK_HPP__
#define __TEST_VIDEO_SINK_HPP__

#include <FIPP/PipelineElements/GenericSinkSi.hpp>
#include <yaml-cpp/yaml.h>
#include <ctime>
namespace FIPP::plugins
{
    class TestVideoSink : public FIPP::pipe::GenericSinkSi
    {
    public:
        TestVideoSink(YAML::Node config, int elemId, std::shared_ptr<FIPP::logging::ILogger> log);

    private:
        void closeInterfaces();
        void initializeInterfaces();
        void doCalculation(std::shared_ptr<img::IImageContainer> img);
        bool m_enableCounter;
        unsigned long long int m_frameNumber;
        std::time_t m_lastTS;
    };

};

extern "C"
{
    /**
     * @brief Create a pipeline element of type TestVideoSink
     * 
     * @param pipeElemPtr shared ptr to store the pipeline element after creation
     * @param config yaml configuration of this pipeline element
     * @param elementName special name of this pipeline element
     * @param elemId unique id of this element
     * @param log shared pointer to logger to store the needed information.
     */
    void makePipeElement(std::shared_ptr<FIPP::pipe::IGenericPipelineElement>& pipeElemPtr, YAML::Node config, std::string elementName, int elemId, std::shared_ptr<FIPP::logging::ILogger> log)
    {
        pipeElemPtr = std::make_shared<FIPP::plugins::TestVideoSink>(config, elemId, log);
    }
    /**
     * @brief Get the Element Identifier object
     * 
     * @param id string variable to store the element identifier
     */
    void getElementIdentifier(std::string &id) { id = std::string("TestVideoSink"); };
    /**
     * @brief Get the Element Type object
     * 
     * @param type variable to store the element type
     */
    void getElementType(FIPP::pipe::ElementTypes &type) { type = FIPP::pipe::ElementTypes::SINK; };
}
#endif