#include "Renderer/Scene.h"
#include "Core/Utils.h"

/*
===============================
== Constants for scene usage ==
===============================
*/

// Setting Constants

constexpr unsigned int SCR_WIDTH = 800;
constexpr unsigned int SCR_HEIGHT = 800;

const unsigned int MAX_THREAD_COUNT        = maxThreadCount();
const unsigned int MAX_RENDER_THREAD_COUNT = maxRenderThreadCount();

// Camera Constants

constexpr float SHIFT = 2.5f;
constexpr float THETA = 2.5f * (M_PI / 180); // Converts degrees to radians for the cmath functions

// Transformation Matrix Constants

constexpr Mat4 SHIFT_X = Mat4(Vec4(1, 0, 0, 0), Vec4(0, 1, 0, 0), Vec4(0, 0, 1, 0), Vec4(2, 0, 0, 1));
constexpr Mat4 SHIFT_Y = Mat4(Vec4(1, 0, 0, 0), Vec4(0, 1, 0, 0), Vec4(0, 0, 1, 0), Vec4(0, 2, 0, 1));
constexpr Mat4 SHIFT_Z = Mat4(Vec4(1, 0, 0, 0), Vec4(0, 1, 0, 0), Vec4(0, 0, 1, 0), Vec4(0, 0, 2, 1));

// Color Constants

const Color WHITE               (255, 255, 255);
const Color OFF_WHITE           (245, 245, 245);
const Color GREY                (128, 128, 128);
const Color RED                 (255, 0, 0);
const Color GREEN               (0, 255, 0);
const Color BLUE                (0, 0, 255);
const Color DEBUG               (255, 0, 255);

const Color MIRROR_WHITE        (250, 255, 250); // Slight green tint like real mirror
const Color MIRROR_RED          (255, 127.5, 127.5);
const Color MIRROR_GREEN        (127.5, 255, 127.5);
const Color MIRROR_BLUE         (127.5, 127.5, 255);

const Color SUN_COLOR           (255, 150, 100);
const Color SKY_COLOR           (40, 40, 255);
const Color NORM_HORIZON_COLOR  (100, 100, 255);

const Color HORIZON_SUN_COLOR   (255, 150, 100);
const Color HORIZON_SKY_COLOR   (100, 104, 255);
const Color HORIZON_COLOR       (255, 120, 60);

const Color MIDNIGHT_MOON_COLOR (180, 165, 130);
const Color MIDNIGHT_SKY_COLOR  (13, 13, 56);
const Color MIDNIGHT_HOR_COLOR  (10, 10, 80);

// Material Constants

constexpr float AMB_COEFF = 0.05f; // Ambient Coefficient
constexpr float DIF_COEFF = 0.50f; // Diffuse Coefficient
constexpr float SPE_COEFF = 0.45f;  // Specular Coefficient
constexpr float SPE_EXP   = 100;   // Specular Exponent

constexpr bool  IS_GLAZED = true;
constexpr float MIR_COEFF = 0.95f;  // Specular coefficient for mirrors

const Material DEBUG_MAT(
    DEBUG,
    DEBUG,
    WHITE,
    AMB_COEFF, 
    DIF_COEFF, 
    SPE_COEFF, 
    SPE_EXP, 
    !IS_GLAZED
);

const Material OFF_WHITE_MAT(
    OFF_WHITE,
    OFF_WHITE,
    WHITE,
    AMB_COEFF,
    DIF_COEFF,
    SPE_COEFF,
    SPE_EXP,
    !IS_GLAZED
);

const Material RED_MAT(
    RED,
    RED,
    WHITE,
    AMB_COEFF,
    DIF_COEFF,
    SPE_COEFF,
    SPE_EXP,
    !IS_GLAZED
);

const Material GREEN_MAT(
    GREEN,
    GREEN,
    WHITE,
    AMB_COEFF,
    DIF_COEFF,
    SPE_COEFF,
    SPE_EXP,
    !IS_GLAZED
);

const Material BLUE_MAT(
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

constexpr float MAX_LIGHT_INTENSITY  = 1.0f;
constexpr float MOON_LIGHT_INTENSITY = 0.1f;

const DirectionalLight DIR_LIGHT         (HORIZON_SUN_COLOR, Vec3(1, -1, -1), MAX_LIGHT_INTENSITY);  // Normal sun
const DirectionalLight HIGH_NOON_LIGHT   (SUN_COLOR, Vec3(0, -1, 0), MAX_LIGHT_INTENSITY);           // Sun straight up
const DirectionalLight HORIZON_DIR_LIGHT (HORIZON_SUN_COLOR, Vec3(0, -1, -5), MAX_LIGHT_INTENSITY);  // Sunset (on horizon)
const DirectionalLight MIDNIGHT_LIGHT    (MIDNIGHT_MOON_COLOR, Vec3(0, -1, -5), MOON_LIGHT_INTENSITY);  // Sunset (on horizon)

// Point Light Materials (just colors)

const Material WHITE_LIGHT_MAT(WHITE);
const Material RED_LIGHT_MAT(RED);
const Material BLUE_LIGHT_MAT(BLUE);
const Material GREEN_LIGHT_MAT(GREEN);

// Atmosphere Constants

constexpr float SUN_SIZE  = 1.0f;
constexpr float MOON_SIZE = 0.3f;
constexpr bool  IS_SUN    = true; // Changes skymodel type between sun and moon

const Atmosphere HIGH_NOON (HIGH_NOON_LIGHT, SUN_SIZE, SKY_COLOR, NORM_HORIZON_COLOR, IS_SUN);
const Atmosphere SUN_SET   (HORIZON_DIR_LIGHT, SUN_SIZE, HORIZON_SKY_COLOR, HORIZON_COLOR, IS_SUN);
const Atmosphere MIDNIGHT  (MIDNIGHT_LIGHT, MOON_SIZE, MIDNIGHT_SKY_COLOR, MIDNIGHT_HOR_COLOR, !IS_SUN);