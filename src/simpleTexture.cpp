// Based on templates from learnopengl.com
#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <iostream>
#include <future>

#include "Resources/ResourceManager.h"
#include "Renderer/Scene.h"
#include "Core/MeshSelector.h"
#include "Core/ThreadPool.h"
#include "Core/Screenshot.h"
#include "Core/Utils.h"

#include "SceneConstants.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

std::string vertexShaderSourceString = readShaderFile("../src/Shaders/source.vs");
const char *vertexShaderSource = vertexShaderSourceString.c_str();

std::string fragmentShaderSourceString = readShaderFile("../src/Shaders/source.fs");
const char *fragmentShaderSource = fragmentShaderSourceString.c_str();


// In front and center
Camera* cameras[2] = {
    new OrthographicCamera(Vec3(0, 100, 100), Vec3(0, 100, 0), Vec3(0, 1, 0)),
    new PerspectiveCamera(Vec3(0, 100, 100), Vec3(0, 100, 0), Vec3(0, 1, 0), 270.0)
};

Scene* scene = nullptr;
ThreadPool* threadPoolptr = nullptr;

unsigned char* imageData = nullptr;
unsigned int imageWidth, imageHeight;

bool Mesh::wireframeAABB = false;

MeshSelector meshSelector;
double mouseX, mouseY;

void getMouseWorldCoords(GLFWwindow *window, double& worldX, double& worldY)
{
    glfwGetCursorPos(window, &mouseX, &mouseY);
    worldX = ((mouseX / SCR_WIDTH) * imageWidth) + 0.5 - 0.5 * imageWidth;
    worldY = ((-(mouseY / SCR_HEIGHT) + 1) * imageHeight) + 0.5 - 0.5 * imageHeight;
}

// From https://www.glfw.org/docs/3.3/input_guide.html
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) // TODO: Create input handler class
{
    // Toggles the camera type

    if (key == GLFW_KEY_X && action == GLFW_PRESS) scene->switchCamera();

    // Screenshot

    if (key == GLFW_KEY_RIGHT_SHIFT && action == GLFW_PRESS) Screenshot::screenshot(imageData, imageWidth, imageHeight);

    // Toggle Debug AABB (note: displays as an OBB)

    if (key == GLFW_KEY_B && action == GLFW_PRESS) Mesh::wireframeAABB = !Mesh::wireframeAABB;

    // Duplicates currently selected meshes

    if (key == GLFW_KEY_V && action == GLFW_PRESS) {
        std::vector<Mesh> duplicatedMeshes = meshSelector.duplicate();
        meshSelector.deselectAll();
        std::vector<Mesh*> newSelectedMeshes = scene->addMeshes(std::move(duplicatedMeshes));
        for(Mesh* mesh : newSelectedMeshes) meshSelector.select(mesh);
    }

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

void mouseCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double worldX, worldY;
        getMouseWorldCoords(window, worldX, worldY);

        Mesh* selectedMesh = scene->selectMesh(worldX, worldY);
        if(selectedMesh != nullptr) meshSelector.select(selectedMesh);
        else meshSelector.deselectAll();
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        double worldX, worldY;
        getMouseWorldCoords(window, worldX, worldY);

        Mesh* selectedMesh = scene->selectMesh(worldX, worldY);
        if(selectedMesh != nullptr) meshSelector.deselect(selectedMesh);
    }
}

int main()
{
    // unsigned int textureSideLength;

    // while(true) {
    //     std::cout << "\nEnter side length of the rendered square texture (logical pixels).\n"
    //               << "Sets texture resolution (NxN) before sampling to the framebuffer: ";

    //     std::cin >> textureSideLength;

    //     if(isPowerOfTwo(textureSideLength)) {
    //         std::cout << "\n";
    //         break;
    //     }

    //     std::cout << "Error.\n";
    // }

    ResourceManager resourceManager;

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
    // groundSphereSurface.push_back(new Sphere(Vec3(0, -7000, -160), 7000, BLUE_MAT));
    groundSphereSurface.push_back(new Sphere());
    Mat4 groundSphereModelMatrix(Vec4(7000, 0, 0, 0), Vec4(0, 7000, 0, 0), Vec4(0, 0, 7000, 0), Vec4(0, -7000, -160, 1));

    Mesh groundSphere(std::move(groundSphereSurface), groundSphereModelMatrix);
    groundSphere.material = BLUE_MAT;
    // meshes.push_back(std::move(groundSphere));

    // Import Models
    Mat4 cubeModelMatrix(Vec4(50, 0, 0, 0), Vec4(0, 50, 0, 0), Vec4(0, 0, 50, 0), Vec4(0, 50, -200, 1));
    Mat4 cubeModelMatrix2(Vec4(7000, 0, 0, 0), Vec4(0, 7000, 0, 0), Vec4(0, 0, 7000, 0), Vec4(0, -7000, -160, 1));
    MeshData cubeData = resourceManager.loadMesh("../assets/models/cube.obj");
    Mesh cube = cubeData.makeInstance(cubeModelMatrix, BRICK_MAT);
    Mesh cube2 = cubeData.makeInstance(cubeModelMatrix2, MIRROR_MAT);

    TextureData brickTexture = resourceManager.loadTexture("../assets/textures/Horror_Brick_02-512x512.png");
    brickTexture.generateMipmap(TextureData::MipmapFilter::Triangle);
    cube.applyTexture(brickTexture, Texture::SampleFilter::Linear_Mipmap_Linear);

    meshes.push_back(std::move(cube));
    meshes.push_back(std::move(cube2));    

    Mat4 pawnModelMatrix(Vec4(50, 0, 0, 0), Vec4(0, 50, 0, 0), Vec4(0, 0, 50, 0), Vec4(0, 150, -200, 1));
    MeshData pawnData = resourceManager.loadMesh("../assets/models/pawn.obj");
    Mesh pawn = pawnData.makeInstance(pawnModelMatrix, RED_MAT);
    meshes.push_back(std::move(pawn));

    // Lights
    std::vector<Light*> lights; // intensity needs to be huge for point lights?

    // Surface* lightSphere1 = new Sphere(Vec3(40, 60, -170), 3.0f, RED_LIGHT_MAT);
    // Light* pointlight1  = new PointLight(RED, Vec3(40, 60, -170), 1000, lightSphere1);
    // // lights.push_back(pointlight1);

    // Surface* lightSphere2 = new Sphere(Vec3(0, 50, -280), 3.0f, GREEN_LIGHT_MAT);
    // Light* pointlight2  = new PointLight(GREEN, Vec3(0, 50, -280), 1000, lightSphere2);
    // lights.push_back(pointlight2);

    scene = new Scene(SUN_SET, std::move(meshes), std::move(lights), cameras);

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
    // glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_FALSE);
    glfwSetMouseButtonCallback(window, mouseCallback);
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
    imageWidth  = 640; // keep it in powers of 2! // 640x360
    imageHeight = 480; // keep it in powers of 2!
    imageData = new unsigned char[imageWidth * imageHeight * 3]; // TODO: attempt dynamic texture sizing

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

        rayTrace(scene, imageData, imageWidth, imageHeight, threadPool);

        if (imageData)
        {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }

        currentFrameCount += 1; // For calculating FPS
    }

    continueFPS = false; // For ending FPS thread execution
    signalCompleteFPS.get(); // Wait until FPS thread is done executing

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);


    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();

    delete[] imageData;
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

    // Object Controls

    // Shifts

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        meshSelector.applyTransform(SHIFT_Z);
    }

    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        meshSelector.applyTransform(SHIFT_Z.negateShift());
    }

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        meshSelector.applyTransform(SHIFT_X);
    }

    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        meshSelector.applyTransform(SHIFT_X.negateShift());
    }

    if (glfwGetKey(window, GLFW_KEY_BACKSLASH) == GLFW_PRESS) {
        meshSelector.applyTransform(SHIFT_Y);
    }

    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
        meshSelector.applyTransform(SHIFT_Y.negateShift());
    }

    // Scale

    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        meshSelector.applyTransform(SCALE_X);
    }

    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
        meshSelector.applyTransform(1.0f / SCALE_X);
    }

    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
        meshSelector.applyTransform(SCALE_Y);
    }

    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
        meshSelector.applyTransform(1.0f / SCALE_Y);
    }

    if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) {
        meshSelector.applyTransform(SCALE_Z);
    }

    if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS) {
        meshSelector.applyTransform(1.0f / SCALE_Z);
    }

    // Rotations

    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        meshSelector.applyTransform(ROTATION_PITCH);
    }

    if (glfwGetKey(window, GLFW_KEY_SEMICOLON) == GLFW_PRESS) {
        meshSelector.applyTransform(ROTATION_PITCH_INVERSE);
    }

    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
        meshSelector.applyTransform(ROTATION_YAW);
    }

    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
        meshSelector.applyTransform(ROTATION_YAW_INVERSE);
    }

    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
        meshSelector.applyTransform(ROTATION_ROLL);
    }

    if (glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS) {
        meshSelector.applyTransform(ROTATION_ROLL_INVERSE);
    }

    // Camera Controls
    
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
        for(Camera* camera : cameras) camera->applyTransform(ROTATION_PITCH);
    }

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        for(Camera* camera : cameras) camera->applyTransform(ROTATION_PITCH_INVERSE);
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        for(Camera* camera : cameras) camera->applyTransform(ROTATION_YAW_INVERSE);
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        for(Camera* camera : cameras) camera->applyTransform(ROTATION_YAW);
    }

    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        for(Camera* camera : cameras) camera->applyTransform(ROTATION_ROLL);
    }

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        for(Camera* camera : cameras) camera->applyTransform(ROTATION_ROLL_INVERSE);
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
