// Based on templates from learnopengl.com
#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <limits>

#include "scene.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

std::string readShaderFile(const std::string& path) {
    std::ifstream shader;
    std::string content;
    std::string line;

    shader.open(path);
    while(std::getline(shader, line)) content += line + "\n";
    shader.close();

    return content;
}

std::string vertexShaderSourceString = readShaderFile("../src/source.vs");
const char *vertexShaderSource = vertexShaderSourceString.c_str();

std::string fragmentShaderSourceString = readShaderFile("../src/source.fs");
const char *fragmentShaderSource = fragmentShaderSourceString.c_str();


// Constants for scene usage
const Color white(255, 255, 255);
const Color grey(128, 128, 128);
const Color red(255, 0, 0);
const Color green(0, 255, 0);
const Color blue(0, 0, 255);
const float ambCoeff = 0.3f;
const float difCoeff = 0.4f;
const float speCoeff = 0.3f;
const float speExp = 100;

// const Material Sphere1Mat(red, red, white, ambCoeff, difCoeff, speCoeff, speExp, false);
// const Material Sphere2Mat(green, green, white, ambCoeff, difCoeff, speCoeff, speExp, false);
// const Material Sphere3Mat(blue, blue, white, ambCoeff, difCoeff, speCoeff, speExp, false);

// Snowman constants (Current scene)
const Color BodyColor(243, 243, 243);
const Color FacePartColor(62, 62, 70); //35
const Color NoseColor(255, 169, 77);

const Material Body(BodyColor, BodyColor, white, ambCoeff, difCoeff, speCoeff, speExp, false);
const Material FacePart(FacePartColor, FacePartColor, white, ambCoeff, difCoeff, speCoeff, speExp, true);
const Material Nose(NoseColor, NoseColor, white, ambCoeff, difCoeff, speCoeff, speExp, false);
const Material PlaneMat(grey, grey, white, ambCoeff, difCoeff, speCoeff, speExp, true); // Glazed plane

const DirectionalLight light(Vec3(-1, -1, 1), 1.0f);

std::vector<Surface*> surfaces;

#define SCENE 1

#if SCENE == 1
// In front and center
Camera* cameras[2] = {
    new OrthographicCamera(Vec3(0, 50, -100), Vec3(0, 20, 1), Vec3(0, 1, 0)),
    new PerspectiveCamera(Vec3(0, 50, -100), Vec3(0, 20, 1), Vec3(0, 1, 0), 270.0)
};

#elif SCENE == 2
// On top looking down
Camera* cameras[2] = {
    new OrthographicCamera(Vec3(0, 200, 160), Vec3(0, 0, 160), Vec3(0, 0, 1)),
    new PerspectiveCamera(Vec3(0, 200, 160), Vec3(0, 0, 160), Vec3(0, 0, 1), 270.0)
};

#elif SCENE == 3
// On left of scene looking center
Camera* cameras[2] = {
    new OrthographicCamera(Vec3(200, 80, 160), Vec3(0, 10, 160), Vec3(0, 1, 0)),
    new PerspectiveCamera(Vec3(200, 80, 160), Vec3(0, 10, 160), Vec3(0, 1, 0), 270.0)
};

#elif SCENE == 4
// Back of scene looking center
Camera* cameras[2] = {
    new OrthographicCamera(Vec3(0, 80, 350), Vec3(0, 10, 0), Vec3(0, 1, 0)),
    new PerspectiveCamera(Vec3(0, 80, 350), Vec3(0, 10, 0), Vec3(0, 1, 0), 270.0)
};

#elif SCENE == 5
// Camera from the left angled right
Camera* cameras[2] = {
    new OrthographicCamera(Vec3(120, 20, 40), Vec3(0, 21, 100), Vec3(0, 1, 0)),
    new PerspectiveCamera(Vec3(120, 20, 40), Vec3(0, 21, 100), Vec3(0, 1, 0), 270.0)
};

#elif SCENE == 6
// Camera from the back top angled down
Camera* cameras[2] = {
    new OrthographicCamera(Vec3(0, 150, 350), Vec3(0, 0, 100), Vec3(0, 0, -1)),
    new PerspectiveCamera(Vec3(0, 150, 350), Vec3(0, 0, 100), Vec3(0, 0, -1), 270.0)
};
#endif

// Setup scene
Scene* scene = nullptr;

// From https://www.glfw.org/docs/3.3/input_guide.html
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Toggles the camera type
    if (key == GLFW_KEY_P && action == GLFW_PRESS) scene->switchCamera();
}

int main()
{
    // Regular Scene
    // surfaces.push_back(new Sphere(Vec3(0.0f, 40.0f, 60.0f), 40.0f, Sphere1Mat));
    // surfaces.push_back(new Sphere(Vec3(0.0f, 60.0f, 170.0f), 60.0f, Sphere2Mat));
    // surfaces.push_back(new Sphere(Vec3(0.0f, 10.0f, 10.0f), 10.0f, Sphere3Mat));
    // surfaces.push_back(new Plane(Vec3(0, 0, 0), Vec3(0, 1, 0), PlaneMat));

    // Snowman scene
    surfaces.push_back(new Sphere(Vec3(0.0f, 40.0f, 160.0f), 40.0f, Body));
    surfaces.push_back(new Sphere(Vec3(0.0f, 90.0f, 160.0f), 30.0f, Body));
    surfaces.push_back(new Sphere(Vec3(0.0f, 130.0f, 160.0f), 20.0f, Body));

    surfaces.push_back(new Sphere(Vec3(5.0f, 132.5f, 141.5f), 2.5f, FacePart));
    surfaces.push_back(new Sphere(Vec3(-5.0f, 132.5f, 141.5f), 2.5f, FacePart));

    surfaces.push_back(new Sphere(Vec3(0.0f, 127.5f, 141.5f), 3.0f, Nose));

    surfaces.push_back(new Plane(Vec3(0, 0, 0), Vec3(0, 1, 0), PlaneMat));

    scene = new Scene(light, surfaces, cameras);

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
    const int width  = 512; // keep it in powers of 2!
    const int height = 512; // keep it in powers of 2!
    unsigned char image[width*height*3];

    unsigned char *data = image;

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

        for(int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++)
            {
                // Translate pixels to image plane coords (normalize each pixel)
                float u = (j + 0.5) / width;
                float v = (i + 0.5) / height;

                // Translate image plane coords to world coords
                float x = (u - 0.5) * width;
                float y = (0.5 - v) * height;

                Color color = scene->getPixelColor(x, y);
                
                int idx = (i * width + j) * 3;
                image[idx] = (unsigned char) (color.r);
                image[idx+1] = color.g;
                image[idx+2] = color.b;
                
            }
        }

        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
    }


    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);


    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();

    delete scene;
    return 0;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    constexpr float shift = 4.0f;
    constexpr float theta = 5 * (M_PI / 180); // Converts degrees to radians for the cmath functions

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    
    // Shifts

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        for(Camera* camera : cameras) camera->updateOrigin(camera->basis.w * shift);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        for(Camera* camera : cameras) camera->updateOrigin(camera->basis.w * -shift);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        for(Camera* camera : cameras) camera->updateOrigin(camera->basis.u * -shift);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        for(Camera* camera : cameras) camera->updateOrigin(camera->basis.u * shift);
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        for(Camera* camera : cameras) camera->updateOrigin(camera->basis.v * -shift);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        for(Camera* camera : cameras) camera->updateOrigin(camera->basis.v * shift);
    }

    // Rotations (have floating point errors and end up wrong, but can manually rotate back)

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        for(Camera* camera : cameras) camera->yaw(theta);
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        for(Camera* camera : cameras) camera->yaw(-theta);
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        for(Camera* camera : cameras) camera->pitch(theta);
    }

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        for(Camera* camera : cameras) camera->pitch(-theta);
    }

    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        for(Camera* camera : cameras) camera->roll(-theta);
    }

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        for(Camera* camera : cameras) camera->roll(theta);
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
