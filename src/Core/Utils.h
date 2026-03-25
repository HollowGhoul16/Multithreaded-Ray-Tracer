#pragma once

#include <thread>
#include <future>
#include <iomanip>
#include <fstream>
#include <iostream>

extern std::atomic<int> currentFrameCount;
extern std::atomic<bool> continueFPS;

void printFPS(std::promise<bool>&& signalCompleteFPS); // Updates every quarter second

std::string localTimestamp();

std::string readShaderFile(const std::string& path);

bool isPowerOfTwo(const int& n);

size_t maxThreadCount();

size_t maxRenderThreadCount();