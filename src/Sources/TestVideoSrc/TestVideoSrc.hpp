#ifndef __TEST_VIDEO_SRC_HPP__
#define __TEST_VIDEO_SRC_HPP__

#include <FIPP/PipelineElements/GenericSource.hpp>
#include <FIPP/ImageContainer/ImageContainer.hpp>
#include <yaml-cpp/yaml.h>
#include <opencv2/opencv.hpp>
namespace FIPP
{
    namespace plugins
    {
        class TestVideoSrc : public FIPP::pipe::GenericSource
        {
        public:
            TestVideoSrc(YAML::Node config, int elemId, std::shared_ptr<FIPP::logging::ILogger> log);

        private:
            void doCalculation(std::shared_ptr<FIPP::img::ImageContainer> img);
            void initializeInterfaces();
            void closeInterfaces();

            void createTestPatternRGBCpu(std::shared_ptr<FIPP::img::ImageContainer> img);
            void createTestPatternGrayCpu(std::shared_ptr<FIPP::img::ImageContainer> img);
            FIPP::img::ImageContainerConfig m_imgConfig;
            int m_radius;
            cv::Point m_cCenter;
        };

        class TestVideoSrcFactory : public pipe::IGenericPipelineElementFactory
        {
        public:
            std::shared_ptr<pipe::IGenericPipelineElement> makePipeElement(YAML::Node config, std::string elementName, int elemId, std::shared_ptr<FIPP::logging::ILogger> log)
            {
                return std::make_shared<TestVideoSrc>(config, elemId, log);
            }
            std::string getElementIdentifier() { return std::string("TestVideoSrc"); };
            pipe::ElementTypes getElementType() { return pipe::ElementTypes::SOURCE; };
        };
    };
};
#endif