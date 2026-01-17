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

constexpr float SHIFT = 2.5f;
constexpr float THETA = 2.5f * (M_PI / 180); // Converts degrees to radians for the cmath functions

// Color Constants

const Color WHITE              (255, 255, 255);
const Color GREY               (128, 128, 128);
const Color RED                (255, 0, 0);
const Color GREEN              (0, 255, 0);
const Color BLUE               (0, 0, 255);

const Color MIRROR_WHITE       (250, 255, 250); // Slight green tint like real mirror
const Color MIRROR_RED         (255, 127.5, 127.5);
const Color MIRROR_GREEN       (127.5, 255, 127.5);
const Color MIRROR_BLUE        (127.5, 127.5, 255);

const Color SUN_COLOR          (255, 150, 100);
const Color SKY_COLOR          (40, 40, 255);
const Color NORM_HORIZON_COLOR (100, 100, 255);

const Color HORIZON_SUN_COLOR  (255, 150, 100);
const Color HORIZON_SKY_COLOR  (100, 104, 255);
const Color HORIZON_COLOR      (255, 120, 60);

// Material Constants

constexpr float AMB_COEFF = 0.05f; // Ambient Coefficient
constexpr float DIF_COEFF = 0.55f; // Diffuse Coefficient
constexpr float SPE_COEFF = 0.4f;  // Specular Coefficient
constexpr float SPE_EXP   = 100;   // Specular Exponent

constexpr bool  IS_GLAZED = true;
constexpr float MIR_COEFF = 0.95f;  // Specular coefficient for mirrors

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
    !IS_GLAZED
);

const Material MIRROR_MAT(
    MIRROR_WHITE,
    MIR_COEFF
);

const Material RED_MIRROR_MAT(
    MIRROR_RED,
    MIR_COEFF
);

const Material GREEN_MIRROR_MAT(
    MIRROR_GREEN,
    MIR_COEFF
);

const Material BLUE_MIRROR_MAT(
    MIRROR_BLUE,
    MIR_COEFF
);

// Light Constants

constexpr float LIGHT_INTENSITY = 1.0f;

const DirectionalLight DIR_LIGHT         (HORIZON_SUN_COLOR, Vec3(1, -1, -1), LIGHT_INTENSITY);  // Normal sun
const DirectionalLight HIGH_NOON_LIGHT   (SUN_COLOR, Vec3(0, -1, 0), LIGHT_INTENSITY);           // Sun straight up
const DirectionalLight HORIZON_DIR_LIGHT (HORIZON_SUN_COLOR, Vec3(0, -1, -5), LIGHT_INTENSITY);  // Sunset (on horizon)

// Atmosphere Constants

constexpr float SUN_SIZE = 1.0f;

const Atmosphere HIGH_NOON (HIGH_NOON_LIGHT, SUN_SIZE, SKY_COLOR, NORM_HORIZON_COLOR);
const Atmosphere SUN_SET   (HORIZON_DIR_LIGHT, SUN_SIZE, HORIZON_SKY_COLOR, HORIZON_COLOR);