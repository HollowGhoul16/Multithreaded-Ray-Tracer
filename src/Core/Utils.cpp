#include "Utils.h"

std::atomic<int> currentFrameCount{0};
std::atomic<bool> continueFPS{true};

void printFPS(std::promise<bool>&& signalCompleteFPS)
{
    if(isPowerOfTwo(maxThreadCount())) {
        std::cout << "\nWARNING: Will not print fps for safety of excessive threads.\n";
        signalCompleteFPS.set_value(true);
        return;
    }

    std::thread([&] {
        int totalFrameCount = 0;
        float secondsPassed = 0;

        while(continueFPS) {
            std::this_thread::sleep_for(std::chrono::milliseconds(250));
            if(!continueFPS) break; // Prevent last outlier

            std::cout << "\rFPS: " << currentFrameCount * 4 << " " << std::flush;

            secondsPassed += .25;
            totalFrameCount += currentFrameCount;
            currentFrameCount = 0;
        }

        std::cout << "\rTotal Time Elapsed: " << secondsPassed << "\n"
                  << "Average FPS: " << (totalFrameCount / secondsPassed) << "\n";

        signalCompleteFPS.set_value_at_thread_exit(true);
    }).detach();
}

std::string readShaderFile(const std::string& path)
{
    std::ifstream shader;
    std::string content;
    std::string line;

    shader.open(path);
    while(std::getline(shader, line)) content += line + "\n";
    shader.close();

    return content;
}

bool isPowerOfTwo(const int& n) { return (n > 0) && ((n & (n - 1)) == 0); }

size_t maxThreadCount() { return std::thread::hardware_concurrency() != 0 ? std::thread::hardware_concurrency() : 1; }

size_t maxRenderThreadCount() 
{
    size_t renderThreadCount = maxThreadCount();

    while(renderThreadCount >= 1) {
        if(isPowerOfTwo(renderThreadCount)) return renderThreadCount;
        --renderThreadCount;
    }

    return renderThreadCount; // 1
}