#include "TestVideoSrc.hpp"
#include <FIPP/ImageContainer/ImagePool.hpp>
#include <FIPP/ImageContainer/ImageFormat.hpp>
#include "cuda_runtime.h"
#define PLUGIN_NAME "TestVideoSrc"

#define LOG(lvl, msg) this->m_log->log(lvl, msg, PLUGIN_NAME, __func__, __LINE__, this->m_frameNumber);
using namespace cv;
using namespace FIPP::plugins;
TestVideoSrc::TestVideoSrc(YAML::Node config, int elemId, std::shared_ptr<FIPP::logging::ILogger> log) : GenericSource(config["name"].as<std::string>(), elemId, log)
{
    if (config["fps"])
    {
        this->m_fps = config["fps"].as<float>();
    }
    else
    {
        this->m_fps = 25.00f;
    }
    this->m_imgConfig = FIPP::img::getContainerConfigFromYaml(config["imgConfig"]);
    this->m_cCenter = cv::Point(this->m_imgConfig.dimensions.getXInt() / 2, this->m_imgConfig.dimensions.getYInt() / 2);
    this->m_radius = (this->m_imgConfig.dimensions.getX() <= this->m_imgConfig.dimensions.getY()) ? this->m_imgConfig.dimensions.getXInt() : this->m_imgConfig.dimensions.getYInt();
    this->m_radius /= 2;
    LOG(logging::CONFIG, "Radius is: " + std::to_string(this->m_radius));
    this->m_pool = std::make_unique<FIPP::img::ImagePool>(10, this->m_imgConfig);
    this->createInitialPattern();
}

void TestVideoSrc::createInitialPattern()
{
    this->m_buffer = (unsigned char *)malloc(this->m_imgConfig.getMemSize());    
    this->m_outBuffer = (unsigned char *)malloc(this->m_imgConfig.getMemSize());
    switch(this->m_imgConfig.imgFormat.imgType){
    case img::ImageType::GRAY:
        this->createInitialPatternGray();
        break;
    case img::ImageType::RGB:
        this->createInitialPatternRGB();
        break;
    default:
        this->createInitialPatternRGBA();
        break;
    }
}

void TestVideoSrc::createInitialPatternGray()
{
    Point<unsigned int> dims = this->m_imgConfig.dimensions;
    memset(this->m_buffer, 255, dims.getAbsValue() * this->m_imgConfig.imgFormat.bytesPerPixel);
    cv::Mat testImg(dims.getY(), dims.getX(), CV_8UC1, this->m_buffer);
    // Draw white circle in image center to the nearest border
    cv::circle(testImg, this->m_cCenter, m_radius - 2, cv::Scalar(0, 0, 0), -1);
}

void TestVideoSrc::createInitialPatternRGB()
{
    Point<unsigned int> dims = this->m_imgConfig.dimensions;
    memset(this->m_buffer, 255, dims.getAbsValue() * this->m_imgConfig.imgFormat.bytesPerPixel);
    cv::Mat testImg(dims.getY(), dims.getX(), CV_8UC3, this->m_buffer, 0);
    // fill with color
    cv::rectangle(testImg, cv::Rect(1, 1, dims.getX() - 1, dims.getY() - 1), CV_RGB(255, 128, 128));
    // Draw white circle in image center to the nearest border.
    cv::circle(testImg, this->m_cCenter, m_radius - 2, CV_RGB(0, 0, 128), -1);
}

void TestVideoSrc::createInitialPatternRGBA()
{
    Point<unsigned int> dims = this->m_imgConfig.dimensions;
    memset(this->m_buffer, 255, dims.getAbsValue() * this->m_imgConfig.imgFormat.bytesPerPixel);
    cv::Mat testImg(dims.getY(), dims.getX(), CV_8UC4, this->m_buffer, 0);
    // fill with color
    cv::rectangle(testImg, cv::Rect(1, 1, dims.getX() - 1, dims.getY() - 1), CV_RGB(255, 128, 128));
    // Draw white circle in image center to the nearest border.
    cv::circle(testImg, this->m_cCenter, m_radius - 2, CV_RGB(0, 0, 128), -1);
}

void TestVideoSrc::createTestPatternRGB()
{


    FIPP::Point<unsigned int> dims = this->m_imgConfig.dimensions;
    // Convert to cv Mat
    cv::Mat testImg(dims.getY(), dims.getX(), CV_8UC3, this->m_outBuffer, 0);
    std::string text = "FIPP - Frame: " + std::to_string(this->m_frameNumber);
    cv::putText(testImg, text, cv::Point(10, testImg.rows / 2), cv::FONT_HERSHEY_DUPLEX, 1.0, CV_RGB(0, 255, 0), 2);

}

void TestVideoSrc::createTestPatternGray()
{
    FIPP::Point<unsigned int> dims = this->m_imgConfig.dimensions;
    // Convert to cv Mat
    cv::Mat testImg(dims.getY(), dims.getX(), CV_8UC1, this->m_outBuffer, 0);
    std::string text = "FIPP - Frame: " + std::to_string(this->m_frameNumber);
    cv::putText(testImg, text, cv::Point(10, testImg.rows / 2), cv::FONT_HERSHEY_DUPLEX, 1.0, CV_RGB(0, 255, 0), 2);
}

void TestVideoSrc::doCalculation(std::shared_ptr<FIPP::img::ImageContainer> img)
{
    memcpy(this->m_outBuffer, this->m_buffer, this->m_imgConfig.getMemSize());

    switch (this->m_imgConfig.imgFormat.imgType)
    {
    case FIPP::img::ImageType::GRAY:
        this->createTestPatternGray();
        break;
    default:
        this->createTestPatternRGB();
        break;
    }
    img::Backend exportBackend;
    exportBackend.type = img::BackendType::CPU;
    img->updateMemory(this->m_frameNumber, this->m_buffer, this->m_imgConfig.dimensions, 3, exportBackend, 0);
}
void TestVideoSrc::initializeInterfaces()
{
    return;
}
void TestVideoSrc::closeInterfaces()
{
    return;
}