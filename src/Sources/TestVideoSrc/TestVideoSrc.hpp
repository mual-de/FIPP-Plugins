#ifndef __TEST_VIDEO_SRC_HPP__
#define __TEST_VIDEO_SRC_HPP__

#include <FIPP/PipelineElements/GenericSource.hpp>
#include <FIPP/ImageContainer/ImageContainer.hpp>
#include <yaml-cpp/yaml.h>
class TestVideoSrc : public FIPP::pipe::GenericSource
{
public:
    TestVideoSrc(YAML::Node config, int elemId, std::shared_ptr<FIPP::logging::ILogger> log);

private:
    void doCalculation(std::shared_ptr<FIPP::img::ImageContainer> img);
    void initializeInterfaces() = 0;
    void closeInterfaces() = 0;
};
#endif