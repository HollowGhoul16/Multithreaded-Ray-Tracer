#include "Renderer/Scene.h"

/*
===============================
== Constants for scene usage ==
===============================
*/

// Setting Constants

constexpr unsigned int SCR_WIDTH = 800;
constexpr unsigned int SCR_HEIGHT = 800;


// Color Constants

constexpr Color WHITE     (255, 255, 255);
constexpr Color GREY      (128, 128, 128);
constexpr Color RED       (255, 0, 0);
constexpr Color GREEN     (0, 255, 0);
constexpr Color BLUE      (0, 0, 255);


// Material Constants

constexpr float AMB_COEFF = 0.3f; // Ambient Coefficient
constexpr float DIF_COEFF = 0.4f; // Diffuse Coefficient
constexpr float SPE_COEFF = 0.3f; // Specular Coefficient
constexpr float SPE_EXP   = 100;  // Specular Exponent

constexpr bool  IS_GLAZED = true;

constexpr Material RED_SPHERE_MAT(
    RED,
    RED, 
    WHITE, 
    AMB_COEFF, 
    DIF_COEFF, 
    SPE_COEFF, 
    SPE_EXP, 
    !IS_GLAZED
);

constexpr Material GREEN_SPHERE_MAT(
    GREEN, 
    GREEN, 
    WHITE, 
    AMB_COEFF, 
    DIF_COEFF, 
    SPE_COEFF, 
    SPE_EXP, 
    !IS_GLAZED
);

constexpr Material BLUE_SPHERE_MAT(
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

constexpr Color SNOWMAN_BODY_COLOR   (243, 243, 243);
constexpr Color SNOWMAN_BUTTON_COLOR (62, 62, 70);
constexpr Color SNOWMAN_NOSE_COLOR   (255, 169, 77);

constexpr Material SNOWMAN_BODY_MAT(
    SNOWMAN_BODY_COLOR, 
    SNOWMAN_BODY_COLOR, 
    WHITE, 
    AMB_COEFF, 
    DIF_COEFF, 
    SPE_COEFF, 
    SPE_EXP, 
    !IS_GLAZED
);

constexpr Material SNOWMAN_EYE_MAT(
    SNOWMAN_BUTTON_COLOR, 
    SNOWMAN_BUTTON_COLOR, 
    WHITE, 
    AMB_COEFF, 
    DIF_COEFF, 
    SPE_COEFF, 
    SPE_EXP, 
    IS_GLAZED
);

constexpr Material SNOWMAN_NOSE_MAT(
    SNOWMAN_NOSE_COLOR, 
    SNOWMAN_NOSE_COLOR, 
    WHITE, 
    AMB_COEFF, 
    DIF_COEFF, 
    SPE_COEFF, 
    SPE_EXP, 
    !IS_GLAZED
);

constexpr Material PLANE_MAT(
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

const DirectionalLight DIR_LIGHT(Vec3(-1.0f, -1.0f, 1.0f), 1.0f);