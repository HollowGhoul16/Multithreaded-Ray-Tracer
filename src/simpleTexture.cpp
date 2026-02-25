// Based on templates from learnopengl.com
#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <iostream>
#include <future>

#include "Geometry/MeshLoader.h"
#include "Renderer/Scene.h"
#include "Core/ThreadPool.h"
#include "Core/Utils.h"

#include "SceneConstants.cpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

std::string vertexShaderSourceString = readShaderFile("../src/Shaders/source.vs");
const char *vertexShaderSource = vertexShaderSourceString.c_str();

std::string fragmentShaderSourceString = readShaderFile("../src/Shaders/source.fs");
const char *fragmentShaderSource = fragmentShaderSourceString.c_str();


// In front and center
Camera* cameras[2] = {
    new OrthographicCamera(Vec3(0, 50, 100), Vec3(0, 20, -1), Vec3(0, 1, 0)),
    new PerspectiveCamera(Vec3(0, 50, 100), Vec3(0, 20, -1), Vec3(0, 1, 0), 270.0)
};

Scene* scene = nullptr;
ThreadPool* threadPoolptr = nullptr;
bool Mesh::wireframeAABB = false;

// From https://www.glfw.org/docs/3.3/input_guide.html
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) // TODO: Create input handler class
{
    // Toggles the camera type

    if (key == GLFW_KEY_P && action == GLFW_PRESS) scene->switchCamera();

    // Toggle Debug AABB

    if (key == GLFW_KEY_B && action == GLFW_PRESS) Mesh::wireframeAABB = !Mesh::wireframeAABB;

    // Change thread amount by power of 2

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        size_t currentThreadCount = threadPoolptr->getThreadCount();
        if(currentThreadCount == 1) return;
        threadPoolptr->setThreadCount(currentThreadCount / 2);
    }

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        size_t currentThreadCount = threadPoolptr->getThreadCount();
        if(currentThreadCount == MAX_RENDER_THREAD_COUNT) return;
        threadPoolptr->setThreadCount(currentThreadCount * 2);
    }
}

int main()
{
    unsigned int textureSideLength;

    while(true) {
        std::cout << "\nEnter side length of the rendered square texture (logical pixels).\n"
                  << "Sets texture resolution (NxN) before sampling to the framebuffer: ";

        std::cin >> textureSideLength;

        if(isPowerOfTwo(textureSideLength)) {
            std::cout << "\n";
            break;
        }

        std::cout << "Error.\n";
    }

    std::vector<Mesh> meshes;
    // std::vector<Surface*> snowmanSurfaces;
    std::vector<Surface*> mirrorCubeSurfaces;
    std::vector<Surface*> groundSphereSurface;

    // Snowman scene
    // snowmanSurfaces.push_back(new Sphere(Vec3(0.0f, 40.0f, -160.0f), 40.0f, SNOWMAN_BODY_MAT));
    // snowmanSurfaces.push_back(new Sphere(Vec3(0.0f, 90.0f, -160.0f), 30.0f, SNOWMAN_BODY_MAT));
    // snowmanSurfaces.push_back(new Sphere(Vec3(0.0f, 130.0f, -160.0f), 20.0f, SNOWMAN_BODY_MAT));

    // snowmanSurfaces.push_back(new Sphere(Vec3(5.0f, 132.5f, -141.5f), 2.5f, SNOWMAN_EYE_MAT));
    // snowmanSurfaces.push_back(new Sphere(Vec3(-5.0f, 132.5f, -141.5f), 2.5f, SNOWMAN_EYE_MAT));

    // snowmanSurfaces.push_back(new Sphere(Vec3(0.0f, 127.5f, -141.5f), 3.0f, SNOWMAN_NOSE_MAT));

    // Mesh snowman(std::move(snowmanSurfaces));
    // meshes.push_back(std::move(snowman));

    // Mirror box
    // Left/Right Walls
    // mirrorCubeSurfaces.push_back(new Rectangle(Vec3(-130.0f, 0.0f, -60.0f),
    //                               Vec3(1, 0, 0),
    //                               Vec3(0.0f, 250.0f, 0.0f),
    //                               Vec3(0.0f, 0.0f, -200.0f),
    //                               RED_MIRROR_MAT));
    // mirrorCubeSurfaces.push_back(new Rectangle(Vec3(130.0f, 0.0f, -60.0f),
    //                               Vec3(-1, 0, 0),
    //                               Vec3(0.0f, 250.0f, 0.0f),
    //                               Vec3(0.0f, 0.0f, -200.0f),
    //                               GREEN_MIRROR_MAT));
    // // Front/Back Wall
    // mirrorCubeSurfaces.push_back(new Rectangle(Vec3(-130.0f, 0.0f, -60.0f),
    //                               Vec3(0, 0, 1),
    //                               Vec3(0.0f, 250.0f, 0.0f),
    //                               Vec3(260.0f, 0.0f, 0.0f),
    //                               MIRROR_MAT));
    // mirrorCubeSurfaces.push_back(new Rectangle(Vec3(-130.0f, 0.0f, -260.0f),
    //                               Vec3(0, 0, 1),
    //                               Vec3(0.0f, 250.0f, 0.0f),
    //                               Vec3(260.0f, 0.0f, 0.0f),
    //                               BLUE_MIRROR_MAT));
    // // Top/Bottom Floor
    // mirrorCubeSurfaces.push_back(new Rectangle(Vec3(-130.0f, 250.0f, -60.0f),
    //                               Vec3(0, -1, 0),
    //                               Vec3(0.0f, 0.0f, -200.0f),
    //                               Vec3(260.0f, 0.0f, 0.0f),
    //                               MIRROR_MAT));
    // mirrorCubeSurfaces.push_back(new Rectangle(Vec3(-130.0f, 0.0f, -60.0f),
    //                               Vec3(0, 1, 0),
    //                               Vec3(0.0f, 0.0f, -200.0f),
    //                               Vec3(260.0f, 0.0f, 0.0f),
    //                               MIRROR_MAT));

    // Mesh mirrorCube(std::move(mirrorCubeSurfaces));
    // meshes.push_back(std::move(mirrorCube));

    // Big Ground Sphere
    groundSphereSurface.push_back(new Sphere(Vec3(0, -7000, -160), 7000, BLUE_MAT));

    Mesh groundSphere(std::move(groundSphereSurface));
    meshes.push_back(std::move(groundSphere));

    // Import Models
    Mat4 cubeModelMatrix(Vec4(130, 0, 0, 0), Vec4(0, 125, 0, 0), Vec4(0, 0, 100, 0), Vec4(0, 500, -160, 0));
    Mesh cube = loadObj("../models/cube.obj", cubeModelMatrix, MIRROR_MAT);
    meshes.push_back(std::move(cube));

    Mat4 pawnModelMatrix(Vec4(0.1, 0, 0, 0), Vec4(0, 0.1, 0, 0), Vec4(0, 0, 0.1, 0), Vec4(0, 52, -200, 0));
    Mesh pawn = loadObj("../models/pawn.obj", pawnModelMatrix, RED_MAT);
    meshes.push_back(std::move(pawn));

    // Lights
    std::vector<Light*> lights; // intensity needs to be huge for point lights?

    Surface* lightSphere1 = new Sphere(Vec3(40, 60, -170), 3.0f, WHITE_LIGHT_MAT);
    Light* pointlight1  = new PointLight(Color(255, 255, 255), Vec3(40, 60, -170), 1000, lightSphere1);
    lights.push_back(pointlight1);

    Surface* lightSphere2 = new Sphere(Vec3(0, 50, -250), 3.0f, WHITE_LIGHT_MAT);
    Light* pointlight2  = new PointLight(Color(255, 255, 255), Vec3(0, 50, -250), 1000, lightSphere2);
    lights.push_back(pointlight2);

    scene = new Scene(MIDNIGHT, std::move(meshes), std::move(lights), cameras);

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Multithreaded Ray Tracer", NULL, NULL);
    glfwSetKeyCallback(window, keyCallback);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    // // GLEW: load all OpenGL function pointers
    glewInit();


    // build and compile the shaders
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // texture coords
         1.0f,  1.0f, 0.0f,   1.0f, 1.0f, // top right
         1.0f, -1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, // bottom left
        -1.0f,  1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);


    glBindVertexArray(VAO);


    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // load and create a texture 
    // -------------------------
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Create the image (RGB Array) to be displayed
    const unsigned int width  = textureSideLength; // keep it in powers of 2!
    const unsigned int height = textureSideLength; // keep it in powers of 2!
    unsigned char *image = new unsigned char[width * height * 3]; // TODO: attempt dynamic texture sizing

    ThreadPool threadPool(MAX_RENDER_THREAD_COUNT);
    threadPoolptr = &threadPool;

    std::promise<bool> promiseFPS;
    std::future<bool> signalCompleteFPS = promiseFPS.get_future();
    printFPS(std::move(promiseFPS));

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);


        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        // bind Texture
        glBindTexture(GL_TEXTURE_2D, texture);


        // render container
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();

        rayTrace(scene, image, width, height, threadPool);

        if (image)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }

        currentFrameCount += 1; // For calculating FPS
    }

    continueFPS = false;; // For ending FPS thread execution
    signalCompleteFPS.get(); // Wait until FPS thread is done executing

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);


    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();

    delete[] image;
    delete scene;

    return 0;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    // Quit program

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    
    // Shifts

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        for(Camera* camera : cameras) camera->updateOrigin(camera->basis.w * SHIFT);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        for(Camera* camera : cameras) camera->updateOrigin(camera->basis.w * -SHIFT);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        for(Camera* camera : cameras) camera->updateOrigin(camera->basis.u * -SHIFT);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        for(Camera* camera : cameras) camera->updateOrigin(camera->basis.u * SHIFT);
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        for(Camera* camera : cameras) camera->updateOrigin(camera->basis.v * SHIFT);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        for(Camera* camera : cameras) camera->updateOrigin(camera->basis.v * -SHIFT);
    }

    // Rotations (Suffers from gimbal lock, would need quaternions to solve)

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        for(Camera* camera : cameras) camera->pitch(THETA);
    }

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        for(Camera* camera : cameras) camera->pitch(-THETA);
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        for(Camera* camera : cameras) camera->yaw(THETA);
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        for(Camera* camera : cameras) camera->yaw(-THETA);
    }

    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        for(Camera* camera : cameras) camera->roll(-THETA);
    }

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        for(Camera* camera : cameras) camera->roll(THETA);
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
