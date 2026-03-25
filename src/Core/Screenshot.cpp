#include "Screenshot.h"

using namespace Screenshot;

ScreenshotFileFormat Screenshot::currentSSFileFormat = ScreenshotFileFormat::BMP;

const std::unordered_map<ScreenshotFileFormat, ScreenshotWriter> Screenshot::screenshotWriteFunctionMap = {
    {ScreenshotFileFormat::BMP, writeBMP},
    {ScreenshotFileFormat::PPM, writePPM}
};

void Screenshot::screenshot(const unsigned char imageData[], const int imageWidth, const int imageHeight)
{
    std::string timestamp = localTimestamp();
    std::string fileName = "Screenshot " + timestamp;
    std::string filePath = "../screenshots/" + fileName;

    screenshotWriteFunctionMap.at(currentSSFileFormat)(filePath, imageData, imageWidth, imageHeight);
}

void Screenshot::writeBMP(const std::string& filePath, const unsigned char imageData[], int imageWidth, int imageHeight)
{
    std::ofstream bpmFile(filePath + ".bmp", std::ios::binary);

    const int bytesPerPixel = 3;
    const int rowStride = imageWidth * bytesPerPixel;
    const int paddingSize = (4 - (rowStride % 4)) % 4;
    const int paddedRowSize = rowStride + paddingSize;
    const int pixelDataSize = paddedRowSize * imageHeight;
    const int fileHeaderSize = 14;
    const int infoHeaderSize = 40;
    const int fileSize = fileHeaderSize + infoHeaderSize + pixelDataSize;

    // .bmp Header
    unsigned char fileHeader[fileHeaderSize] = {
        'B', 'M',                         // Signature
        0, 0, 0, 0,                       // File size
        0, 0, 0, 0,                       // Reserved
        54, 0, 0, 0                       // Pixel data offset
    };

    // DIB Header
    unsigned char infoHeader[infoHeaderSize] = {
        40, 0, 0, 0,                      // Header size
        0, 0, 0, 0,                       // Width
        0, 0, 0, 0,                       // Height
        1, 0,                             // Planes
        24, 0,                            // Bits per pixel
        0, 0, 0, 0,                       // Compression (none)
        0, 0, 0, 0,                       // Image size
        0, 0, 0, 0,                       // X pixels per meter
        0, 0, 0, 0,                       // Y pixels per meter
        0, 0, 0, 0,                       // Total colors
        0, 0, 0, 0                        // Important colors
    };

    // Fill file size
    fileHeader[2] = (unsigned char)(fileSize);
    fileHeader[3] = (unsigned char)(fileSize >> 8);
    fileHeader[4] = (unsigned char)(fileSize >> 16);
    fileHeader[5] = (unsigned char)(fileSize >> 24);

    // Fill image width
    infoHeader[4] = (unsigned char)(imageWidth);
    infoHeader[5] = (unsigned char)(imageWidth >> 8);
    infoHeader[6] = (unsigned char)(imageWidth >> 16);
    infoHeader[7] = (unsigned char)(imageWidth >> 24);

    // Fill image height
    infoHeader[8] = (unsigned char)(imageHeight);
    infoHeader[9] = (unsigned char)(imageHeight >> 8);
    infoHeader[10] = (unsigned char)(imageHeight >> 16);
    infoHeader[11] = (unsigned char)(imageHeight >> 24);

    // Fill image size
    infoHeader[20] = (unsigned char)(pixelDataSize);
    infoHeader[21] = (unsigned char)(pixelDataSize >> 8);
    infoHeader[22] = (unsigned char)(pixelDataSize >> 16);
    infoHeader[23] = (unsigned char)(pixelDataSize >> 24);

    bpmFile.write(reinterpret_cast<char*>(fileHeader), fileHeaderSize);
    bpmFile.write(reinterpret_cast<char*>(infoHeader), infoHeaderSize);

    unsigned char padding[3] = {0, 0, 0};

    // .bmp stores pixels bottom to top
    for (int j = 0; j < imageHeight; ++j)
    {
        const unsigned char* row = imageData + (j * imageWidth * bytesPerPixel);

        for (int i = 0; i < imageWidth; ++i)
        {
            const unsigned char* pixel = row + i * 3;

            // .bmp stores colors as BGR
            unsigned char bgr[3];
            bgr[0] = pixel[2];
            bgr[1] = pixel[1];
            bgr[2] = pixel[0];

            bpmFile.write(reinterpret_cast<char*>(bgr), 3);
        }

        bpmFile.write(reinterpret_cast<char*>(padding), paddingSize);
    }

    bpmFile.close();
}

void Screenshot::writePPM(const std::string& filePath, const unsigned char imageData[], int imageWidth, int imageHeight)
{
    std::ofstream ppmFile(filePath + ".ppm");

    // .ppm header
    ppmFile << "P6\n" << imageWidth << " " << imageHeight << "\n255\n";

    for(int j = imageHeight - 1; j >= 0; --j) {
		for (int i = 0; i < imageWidth; ++i)
		{
			int idx = (j * imageWidth + i) * 3;
			unsigned char r = imageData[idx];
			unsigned char g = imageData[idx+1];
			unsigned char b = imageData[idx+2];

            ppmFile << r << g << b;
		}
	}

    ppmFile.close();
}