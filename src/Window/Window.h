#pragma once

#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>

#include "SceneConstants.hpp"

struct Window {
    GLFWwindow* window = nullptr;
    GLFWmonitor* monitor = nullptr;
    const GLFWvidmode* vidmode = nullptr;
    bool forceClosed = false;

    int monitorX;
    int monitorY;
    int fullscreenWidth;
    int fullscreenHeight;
    int windowedWidth;
    int windowedHeight;
    int currentWidth;
    int currentHeight;

    enum class DisplayMode {
        Windowed,
        Windowed_Fullscreen,
        FullScreen
    };

    DisplayMode displayMode;

    Window(const DisplayMode& displayMode);

    ~Window();

    void update();

    void setDisplayMode(const DisplayMode& displayMode);

    void forceClose();

    bool isOpen() const;

    int getFullScreenWidth() const;

    int getFullScreenHeight() const;

    int getWindowedWidth() const;

    int getWindowedHeight() const;

    int getCurrentWidth() const;

    int getCurrentHeight() const;

    GLFWwindow* getWindowPointer() const;

    DisplayMode getDisplayMode() const;

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};