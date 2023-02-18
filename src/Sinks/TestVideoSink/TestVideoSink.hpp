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
        void doCalculation(std::shared_ptr<img::ImageContainer> img);
        bool m_enableCounter;
        unsigned long long int m_frameNumber;
        std::time_t m_lastTS;
    };

    class TestVideoSinkFactory : public pipe::IGenericPipelineElementFactory
    {
    public:
        std::shared_ptr<pipe::IGenericPipelineElement> makePipeElement(YAML::Node config, std::string elementName, int elemId, std::shared_ptr<FIPP::logging::ILogger> log)
        {
            return std::make_shared<TestVideoSink>(config, elemId, log);
        }
        std::string getElementIdentifier() { return std::string("TestVideoSink"); };
        pipe::ElementTypes getElementType() { return pipe::ElementTypes::SINK; };
    };

};
#endif