#pragma once

#include <thread>
#include <iostream>

std::atomic<int> frameCount{0};

void printFPS() {
    std::thread([&] {
        while(frameCount != -1) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "FPS: " << frameCount << "\n";
            frameCount = 0;
        }
    }).detach();
}

std::string readShaderFile(const std::string& path) {
    std::ifstream shader;
    std::string content;
    std::string line;

    shader.open(path);
    while(std::getline(shader, line)) content += line + "\n";
    shader.close();

    return content;
}

bool isPowerOfTwo(int n) { return (n > 0) && ((n & (n - 1)) == 0); }