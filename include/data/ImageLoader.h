#pragma once

#include <string>
#include <vector>

// ------------------------------------------------------------
// ImageLoader
// ------------------------------------------------------------
// Responsible for loading 2D image files (PNG/JPEG) from disk.
// This class is simple and focused on a single responsibility,
// following SRP from SOLID.
//
// • Uses stb_image internally in the .cpp file.
// • Returns raw RGB pixel data to the renderer.
// ------------------------------------------------------------

class ImageLoader
{
public:
    ImageLoader() = default;
    ~ImageLoader() = default;

    // Loads an image file into memory:
    //  - filepath: full path to image
    //  - width, height: output dimensions
    //  - channels: output channels count (3 for RGB, 4 for RGBA)
    //  - data: raw pixel buffer (RGB8 or RGBA8)
    //
    // Returns: true on success, false on failure
    bool loadImage(
        const std::string& filepath,
        int& width,
        int& height,
        int& channels,
        std::vector<unsigned char>& data
    );

    // Frees the loaded image buffer (if needed)
    void freeImage(unsigned char* data);
};
