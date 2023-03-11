/**
 * @file TestVideoSrc.hpp
 * @author Alexander Mueller (dev@alexandermaxmueller.de)
 * @brief A small TestVideoSrc to create test images (GPU and CPU)
 * @version 0.1
 * @date 2023-03-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */
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
            void createInitialPattern();
            void createInitialPatternGray();
            void createInitialPatternRGB();
            void createInitialPatternRGBA();
            void createTestPatternRGB();
            void createTestPatternGray();
            FIPP::img::ImageContainerConfig m_imgConfig;
            int m_radius;
            /**
             * @brief Image center point
             * 
             */
            cv::Point m_cCenter;
            /**
             * @brief check if cuda backend should be used.
             * 
             */
            bool m_enableCuda;
            /**
             * @brief Depending on configuration cpu/gpu accessable buffer for demo img
             * 
             */
            unsigned char* m_buffer;
            unsigned char* m_outBuffer;
        };

        
    };
};
#if !TEST_MODE
extern "C"
{
    /**
     * @brief Create a pipeline element of type TestVideoSrc
     * 
     * @param pipeElemPtr shared ptr to store the pipeline element after creation
     * @param config yaml configuration of this pipeline element
     * @param elementName special name of this pipeline element
     * @param elemId unique id of this element
     * @param log shared pointer to logger to store the needed information.
     */
    void makePipeElement(std::shared_ptr<FIPP::pipe::IGenericPipelineElement>& pipeElemPtr, YAML::Node config, std::string elementName, int elemId, std::shared_ptr<FIPP::logging::ILogger> log)
    {
        pipeElemPtr = std::make_shared<FIPP::plugins::TestVideoSrc>(config, elemId, log);
    }
    /**
     * @brief Get the Element Identifier object
     * 
     * @param id string variable to store the element identifier
     */
    void getElementIdentifier(std::string &id) { id = std::string("TestVideoSrc"); };
    /**
     * @brief Get the Element Type object
     * 
     * @param type variable to store the element type
     */
    void getElementType(FIPP::pipe::ElementTypes &type) { type = FIPP::pipe::ElementTypes::SOURCE; };
}
#endif
#endif