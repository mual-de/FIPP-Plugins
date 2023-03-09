#include "TestVideoSrc.hpp"
#include <FIPP/ImageContainer/ImagePool.hpp>
#include <FIPP/ImageContainer/ImageFormat.hpp>
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
    
}

void TestVideoSrc::createTestPatternRGBCpu(std::shared_ptr<FIPP::img::ImageContainer> img)
{
    img->getMutex()->lock();
    FIPP::Point<unsigned int> dims = img->getDims();
    // Convert to cv Mat
    cv::Mat testImg(dims.getY(), dims.getX(), CV_8UC3, img->getPtr(),0);
    // fill with color
    cv::rectangle(testImg, cv::Rect(1,1, dims.getX()-1, dims.getY()-1), CV_RGB(255,128,128));
    // Draw white circle in image center to the nearest border.
    cv::circle(testImg, this->m_cCenter, m_radius-2, (0, 0, 128),-1);
    std::string text = "FIPP - Frame: " + std::to_string(this->m_frameNumber);
    cv::putText(testImg, text, cv::Point(10, testImg.rows/2), cv::FONT_HERSHEY_DUPLEX, 1.0, CV_RGB(0,255,0),2 );
    img->getMutex()->unlock();
}
void TestVideoSrc::createTestPatternGrayCpu(std::shared_ptr<FIPP::img::ImageContainer> img)
{
    img->getMutex()->lock();
    FIPP::Point<unsigned int> dims = img->getDims();
    // Convert to cv Mat
    cv::Mat testImg(dims.getY(), dims.getX(), CV_8U, img->getPtr());
    // Draw white circle in image center to the nearest border.
    cv::circle(testImg, this->m_cCenter, m_radius, (0, 0, 255));
    img->getMutex()->unlock();
}

void TestVideoSrc::doCalculation(std::shared_ptr<FIPP::img::ImageContainer> img)
{
    if (img->getBackendType() == FIPP::img::BackendType::CPU)
    {
        switch (this->m_imgConfig.imgFormat.imgType)
        {
        case FIPP::img::ImageType::GRAY:
            this->createTestPatternGrayCpu(img);
            break;
        default:
            this->createTestPatternRGBCpu(img);
            break;
        }
    }
}
void TestVideoSrc::initializeInterfaces()
{
    return;
}
void TestVideoSrc::closeInterfaces()
{
    return;
}