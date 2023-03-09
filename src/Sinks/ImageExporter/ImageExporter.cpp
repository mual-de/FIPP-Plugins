#include "ImageExporter.hpp"
#include <FIPP/Logging/ILogging.hpp>
using namespace FIPP;
using namespace FIPP::plugins;
#include <FIPP/ImageContainer/ImageFormat.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <FIPP/ImageContainer/ImageContainerCUDA.hpp>
#include <FIPP/ImageContainer/ImageFormat.hpp>

ImageExporter::ImageExporter(YAML::Node config, int elemId, std::shared_ptr<FIPP::logging::ILogger> log) : GenericSinkSi(config["name"].as<std::string>(), elemId, log)
{
    if (config["storagePath"].IsDefined())
    {
        this->m_storagePath = config["storagePath"].as<std::string>();
    }
    else
    {
        this->m_storagePath = "/tmp";
    }
    if (config["filePrefix"].IsDefined())
    {
        this->m_filePrefix = config["filePrefix"].as<std::string>();
    }
    else
    {
        this->m_filePrefix = "img-";
    }
    if (config["fileFormat"].IsDefined())
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

void ImageExporter::exportGpuImages(std::shared_ptr<img::ImageContainer> img)
{

    if (img->getBackendFlags() == img::BackendFlags::UNIFIED_MEMORY || img->getBackendFlags() == img::BackendFlags::ZERO_COPY)
    {
        this->exportCpuImages(img);
    }
    else
    {
        std::shared_ptr<img::ImageContainerCUDA> imgCuda = std::static_pointer_cast<img::ImageContainerCUDA>(img);
        img::ImageFormat fmt = img->getImgFormat();
        FIPP::Point<unsigned int> dims = img->getDims();
        if (fmt.imgType == img::ImageType::GRAY)
        {
            // Grayscale img
            if (fmt.bitDepthPerPixel == 8)
            {
                cv::cuda::GpuMat in(dims.getY(), dims.getX(), CV_8UC1, imgCuda->getDevPtr());
                cv::cuda::GpuMat out;
                cv::cuda::cvtColor(in, out, cv::COLOR_GRAY2BGR);
                cv::Mat exp;
                out.download(exp);
                LOG(logging::INFO, "Store image at: " + this->m_storagePath + "/" + this->m_filePrefix + std::to_string(this->m_frameNumber) + "." + this->m_fileFormat);
                cv::imwrite(this->m_storagePath + "/" + this->m_filePrefix + std::to_string(this->m_frameNumber) + "." + this->m_fileFormat, exp);
            }
            else
            {
                // ToDo: 10/12/14/16 Bit Grayscale
            }
        }
        else if (fmt.imgType == img::ImageType::RGB)
        {
            // RGB image
            if (fmt.bitDepthPerPixel == 8)
            {
                cv::cuda::GpuMat in(dims.getY(), dims.getX(), CV_8UC3, imgCuda->getDevPtr());
                cv::cuda::GpuMat out;
                cv::cuda::cvtColor(in, out, cv::COLOR_RGB2BGR);
                cv::Mat exp;
                out.download(exp);
                LOG(logging::INFO, "Store image at: " + this->m_storagePath + "/" + this->m_filePrefix + std::to_string(this->m_frameNumber) + "." + this->m_fileFormat);
                cv::imwrite(this->m_storagePath + "/" + this->m_filePrefix + std::to_string(this->m_frameNumber) + "." + this->m_fileFormat, exp);
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
                cv::cuda::GpuMat in(dims.getY(), dims.getX(), CV_8UC4, imgCuda->getDevPtr());
                cv::cuda::GpuMat out;
                cv::cuda::cvtColor(in, out, cv::COLOR_RGBA2BGR);
                cv::Mat exp;
                cv::imwrite(this->m_storagePath + "/" + this->m_filePrefix + std::to_string(this->m_frameNumber) + "." + this->m_fileFormat, exp);
            }
            else
            {
                // ToDo: 10/12/14/16 Bit RGBA
            }
        }
    }
}

void ImageExporter::exportCpuImages(std::shared_ptr<img::ImageContainer> img)
{
    img::ImageFormat fmt = img->getImgFormat();
    FIPP::Point<unsigned int> dims = img->getDims();
    if (fmt.imgType == img::ImageType::GRAY)
    {
        // Grayscale img
        if (fmt.bitDepthPerPixel == 8)
        {
            cv::Mat out(dims.getY(), dims.getX(), CV_8UC1, img->getPtr(), 0);
            LOG(logging::INFO, "Store image at: " + this->m_storagePath + "/" + this->m_filePrefix + std::to_string(this->m_frameNumber) + "." + this->m_fileFormat);
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
        if (fmt.bitDepthPerPixel == 8)
        {
            cv::Mat out(dims.getY(), dims.getX(), CV_8UC3, img->getPtr(), 0);
            LOG(logging::INFO, "Store image at: " + this->m_storagePath + "/" + this->m_filePrefix + std::to_string(this->m_frameNumber) + "." + this->m_fileFormat);
            cv::Mat outBGR;
            cv::cvtColor(out, outBGR, cv::COLOR_RGB2BGR);
            cv::imwrite(this->m_storagePath + "/" + this->m_filePrefix + std::to_string(this->m_frameNumber) + "." + this->m_fileFormat, outBGR);
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
    else if (type == img::BackendType::CUDA)
    {
        this->exportGpuImages(img);
    }
}