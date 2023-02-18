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
    this->m_pool = std::make_unique<FIPP::img::ImagePool>(10, this->m_imgConfig);
}

void TestVideoSrc::createTestPatternRGBCpu(std::shared_ptr<FIPP::img::ImageContainer> img)
{
    img->getMutex()->lock();
    FIPP::Point<unsigned int> dims = img->getDims();
    // Convert to cv Mat
    cv::Mat testImg(dims.getY(), dims.getX(), CV_8UC3, img->getPtr(), 0);
    // Draw white circle in image center to the nearest border.
    cv::circle(testImg, this->m_cCenter, m_radius, (255, 0, 255));
    img->getMutex()->unlock();
}
void TestVideoSrc::createTestPatternGrayCpu(std::shared_ptr<FIPP::img::ImageContainer> img)
{
    img->getMutex()->lock();
    FIPP::Point<unsigned int> dims = img->getDims();
    // Convert to cv Mat
    cv::Mat testImg(dims.getY(), dims.getX(), CV_8U, img->getPtr(), 0);
    // Draw white circle in image center to the nearest border.
    cv::circle(testImg, this->m_cCenter, m_radius, (255, 255, 255));
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