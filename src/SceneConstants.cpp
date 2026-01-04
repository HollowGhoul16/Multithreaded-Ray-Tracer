#include "Renderer/Scene.h"

/*
===============================
== Constants for scene usage ==
===============================
*/

// Setting Constants

constexpr unsigned int SCR_WIDTH = 800;
constexpr unsigned int SCR_HEIGHT = 800;


// Camera Constants

constexpr float SHIFT = 4.0f;
constexpr float THETA = 3 * (M_PI / 180); // Converts degrees to radians for the cmath functions

// Color Constants

const Color WHITE     (255, 255, 255);
const Color GREY      (128, 128, 128);
const Color RED       (255, 0, 0);
const Color GREEN     (0, 255, 0);
const Color BLUE      (0, 0, 255);


// Material Constants

constexpr float AMB_COEFF = 0.05f; // Ambient Coefficient
constexpr float DIF_COEFF = 0.55f; // Diffuse Coefficient
constexpr float SPE_COEFF = 0.4f;  // Specular Coefficient
constexpr float SPE_EXP   = 100;   // Specular Exponent

constexpr bool  IS_GLAZED = true;

const Material RED_SPHERE_MAT(
    RED,
    RED, 
    WHITE, 
    AMB_COEFF, 
    DIF_COEFF, 
    SPE_COEFF, 
    SPE_EXP, 
    !IS_GLAZED
);

const Material GREEN_SPHERE_MAT(
    GREEN, 
    GREEN, 
    WHITE, 
    AMB_COEFF, 
    DIF_COEFF, 
    SPE_COEFF, 
    SPE_EXP, 
    !IS_GLAZED
);

const Material BLUE_SPHERE_MAT(
    BLUE, 
    BLUE, 
    WHITE, 
    AMB_COEFF, 
    DIF_COEFF, 
    SPE_COEFF, 
    SPE_EXP, 
    !IS_GLAZED
);


// Snowman Constants (Current scene)

const Color SNOWMAN_BODY_COLOR   (243, 243, 243);
const Color SNOWMAN_BUTTON_COLOR (62, 62, 70);
const Color SNOWMAN_NOSE_COLOR   (255, 169, 77);

const Material SNOWMAN_BODY_MAT(
    SNOWMAN_BODY_COLOR, 
    SNOWMAN_BODY_COLOR, 
    WHITE, 
    AMB_COEFF, 
    DIF_COEFF, 
    SPE_COEFF, 
    SPE_EXP, 
    !IS_GLAZED
);

const Material SNOWMAN_EYE_MAT(
    SNOWMAN_BUTTON_COLOR, 
    SNOWMAN_BUTTON_COLOR, 
    WHITE, 
    AMB_COEFF, 
    DIF_COEFF, 
    SPE_COEFF, 
    SPE_EXP, 
    IS_GLAZED
);

const Material SNOWMAN_NOSE_MAT(
    SNOWMAN_NOSE_COLOR, 
    SNOWMAN_NOSE_COLOR, 
    WHITE, 
    AMB_COEFF, 
    DIF_COEFF, 
    SPE_COEFF, 
    SPE_EXP, 
    !IS_GLAZED
);

const Material PLANE_MAT(
    GREY, 
    GREY, 
    WHITE, 
    AMB_COEFF, 
    DIF_COEFF, 
    SPE_COEFF, 
    SPE_EXP, 
    IS_GLAZED
);

// Light Constants

const DirectionalLight DIR_LIGHT(Vec3(1.0f, -1.0f, -1.0f), 1.0f);