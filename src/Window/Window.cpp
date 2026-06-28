#include "Window.h"

Window::Window(const DisplayMode& displayMode = DisplayMode::Windowed) : displayMode(displayMode)
{
    if(glfwInit() == GLFW_FALSE) throw std::runtime_error("GLFW failed to initialize");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    monitor = glfwGetPrimaryMonitor();
    vidmode = glfwGetVideoMode(monitor);

    glfwWindowHint(GLFW_RED_BITS, vidmode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, vidmode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, vidmode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, vidmode->refreshRate);

    glfwGetMonitorPos(monitor, &monitorX, &monitorY);

    fullscreenWidth = vidmode->width;
    fullscreenHeight = vidmode->height;
    windowedWidth = FRAMEBUFFER_WIDTH * 2;
    windowedHeight = FRAMEBUFFER_HEIGHT * 2;

    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    switch(displayMode) {
        case DisplayMode::Windowed:
            currentWidth = windowedWidth;
            currentHeight = windowedHeight;
            window = glfwCreateWindow(windowedWidth, windowedHeight, "Multithreaded Ray Tracer", NULL, NULL);
            glfwSetWindowPos(window, monitorX + (fullscreenWidth - windowedWidth) / 2.0f, monitorY + (fullscreenHeight - windowedHeight) / 2.0f); // Centered
            break;
        case DisplayMode::Windowed_Fullscreen:
            glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
            currentWidth = fullscreenWidth;
            currentHeight = fullscreenHeight;
            window = glfwCreateWindow(fullscreenWidth, fullscreenHeight, "Multithreaded Ray Tracer", NULL, NULL);
            break;
        case DisplayMode::FullScreen:
            currentWidth = fullscreenWidth;
            currentHeight = fullscreenHeight;
            window = glfwCreateWindow(fullscreenWidth, fullscreenHeight, "Multithreaded Ray Tracer", monitor, NULL);
            break;
        // default:
            // ?
    }

    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(window);
    framebuffer_size_callback(window, currentWidth, currentHeight);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwShowWindow(window);
}

Window::~Window()
{
    glfwDestroyWindow(window);
    window = nullptr;
    monitor = nullptr;
    vidmode = nullptr;
}

void Window::update()
{
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Window::setDisplayMode(const DisplayMode& displayMode)
{
    this->displayMode = displayMode;
}

void Window::forceClose()
{
    forceClosed = true;
}

bool Window::isOpen() const
{
    return !(glfwWindowShouldClose(window) || forceClosed);
}

int Window::getFullScreenWidth() const
{
    return fullscreenWidth;
}

int Window::getFullScreenHeight() const
{
    return fullscreenHeight;
}

int Window::getWindowedWidth() const
{
    return windowedWidth;
}

int Window::getWindowedHeight() const
{
    return windowedHeight;
}

int Window::getCurrentWidth() const
{
    return currentWidth;
}

int Window::getCurrentHeight() const
{
    return currentHeight;
}

GLFWwindow* Window::getWindowPointer() const
{
    return window;
}

Window::DisplayMode Window::getDisplayMode() const
{
    return displayMode;
}

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    float window_aspect = (float)width / (float)height;

    int view_width, view_height;
    int view_x, view_y;

    if(window_aspect > FRAMEBUFFER_ASPECT) {
        // Pillarbox
        view_height = height;
        view_width = (int)(height * FRAMEBUFFER_ASPECT);
        view_x = (width - view_width) / 2;
        view_y = 0;
    } else {
        // Letterbox
        view_width = width;
        view_height = (int)(width / FRAMEBUFFER_ASPECT);
        view_x = 0;
        view_y = (height - view_height) / 2;
    }

    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(view_x, view_y, view_width, view_height);
}