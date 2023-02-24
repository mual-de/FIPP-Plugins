#include "ImageExporter.hpp"
#include <FIPP/Logging/ILogging.hpp>
using namespace FIPP;
using namespace FIPP::plugins;
#include <FIPP/ImageContainer/ImageFormat.hpp>
#include <opencv2/opencv.hpp>

ImageExporter::ImageExporter(YAML::Node config, int elemId, std::shared_ptr<FIPP::logging::ILogger> log) : GenericSinkSi(config["name"].as<std::string>(), elemId, log)
{
    if (config["storagePath"])
    {
        this->m_storagePath = config["storage_path"].as<std::string>();
    }
    else
    {
        this->m_storagePath = "~/tmp";
    }
    if (config["filePrefix"])
    {
        this->m_filePrefix = config["file_prefix"].as<std::string>();
    }
    else
    {
        this->m_filePrefix = "img-";
    }
    if (config["fileFormat"])
    {
        this->m_fileFormat = config["fileFormat"].as<std::string>();
    }
    else
    {
        // ToDo: Need check for available formats!
        this->m_fileFormat = "png";
    }
}

void ImageExporter::closeInterfaces()
{
    LOG(logging::CONFIG, "Close uneeded interfaces!");
}
void ImageExporter::initializeInterfaces()
{
    LOG(logging::CONFIG, "Initialize uneeded interfaces!");
}

void ImageExporter::exportCpuImages(std::shared_ptr<img::ImageContainer> img)
{
    img::ImageFormat fmt = img->getImgFormat();
    FIPP::Point<unsigned int> dims = img->getDims();
    if (fmt.imgType == img::ImageType::GRAY)
    {
        // Grayscale img
        if (fmt.bytesPerPixel == 1)
        {
            cv::Mat out(dims.getY(), dims.getX(), CV_8UC1, img->getPtr(), 0);
            cv::imwrite(this->m_storagePath + "/" + this->m_filePrefix + std::to_string(this->m_frameNumber) + "." + this->m_fileFormat, out);
        }
        else
        {
            // ToDo: 10/12/14/16 Bit Grayscale
        }
    }
    else if (fmt.imgType == img::ImageType::RGB)
    {
        // RGB image
        if (fmt.bytesPerPixel == 1)
        {
            cv::Mat out(dims.getY(), dims.getX(), CV_8UC3, img->getPtr(), 0);
            cv::imwrite(this->m_storagePath + "/" + this->m_filePrefix + std::to_string(this->m_frameNumber) + "." + this->m_fileFormat, out);
        }
        else
        {
            // ToDo: 10/12/14/16 Bit RGB
        }
    }
    else if (fmt.imgType == img::ImageType::RGBA)
    {
        // RGBA img
        if (fmt.bytesPerPixel == 1)
        {
            cv::Mat out(dims.getY(), dims.getX(), CV_8UC1, img->getPtr(), 0);
            cv::imwrite(this->m_storagePath + "/" + this->m_filePrefix + std::to_string(this->m_frameNumber) + "." + this->m_fileFormat, out);
        }
        else
        {
            // ToDo: 10/12/14/16 Bit RGBA
        }
    }
}

void ImageExporter::doCalculation(std::shared_ptr<img::ImageContainer> img)
{
    LOG(logging::CONFIG, "Received image from pipeline");
    this->m_frameNumber = img->getFrameNumber();
    img::BackendType type = img->getBackendType();
    if (type == img::BackendType::CPU)
    {
        this->exportCpuImages(img);
    }
}