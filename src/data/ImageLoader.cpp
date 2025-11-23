#include "ImageLoader.h"
#include "utils/Logger.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

ImageLoader::ImageLoader()
{
    // Nothing to initialize
}

bool ImageLoader::loadImage(
    const std::string& path,
    int& width,
    int& height,
    std::vector<unsigned char>& outData)
{
    stbi_set_flip_vertically_on_load(false);

    int channels = 0;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 3);

    if (!data)
    {
        LOG_ERROR("Failed to load image: " + path);
        return false;
    }

    size_t imageSize = width * height * 3;
    outData.resize(imageSize);
    std::memcpy(outData.data(), data, imageSize);

    stbi_image_free(data);

    LOG_INFO("Loaded image: " + path +
             " (" + std::to_string(width) + "x" +
             std::to_string(height) + ")");

    return true;
}
