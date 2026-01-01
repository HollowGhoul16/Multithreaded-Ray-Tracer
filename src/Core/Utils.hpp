#pragma once

#include <thread>
#include <future>
#include <fstream>
#include <iostream>

std::atomic<int> currentFrameCount{0};
std::atomic<bool> continueFPS{true};

void printFPS(std::promise<bool>&& signalCompleteFPS)
{
    std::thread([&] {
        int totalFrameCount = 0;
        int secondsPassed = 0;

        while(continueFPS) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            if(!continueFPS) break; // Prevent last outlier

            std::cout << "FPS: " << currentFrameCount << "\n";

            secondsPassed += 1;
            totalFrameCount += currentFrameCount;
            currentFrameCount = 0;
        }

        std::cout << "Total Time Elapsed: " << secondsPassed << "\n"
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