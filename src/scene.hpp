#pragma once

#include <cmath>
#include <limits>
#include <string>
#include <vector>

struct Vec3 {
    float x = 0;
    float y = 0;
    float z = 0;

    constexpr Vec3() : x(0), y(0), z(0) {};

    constexpr Vec3(const float& x, const float& y, const float& z) : x(x), y(y), z(z) {};

    const float& operator[](const int i) const { 
        return i == 0 ? x : (i == 1 ? y : z); 
    }

    Vec3 operator*(const float scalar) const { 
        return Vec3(x * scalar, y * scalar, z * scalar);       
    }

    Vec3 operator+(const Vec3& otherVec) const { 
        return Vec3(x + otherVec.x, y + otherVec.y, z + otherVec.z); 
    }

    Vec3 operator-(const Vec3& otherVec) const { 
        return Vec3(x - otherVec.x, y - otherVec.y, z - otherVec.z); 
    }

    Vec3 operator-() const { 
        return Vec3(-x, -y, -z);          
    }

    float magnitude() const { 
        return std::sqrt(x * x + y * y + z * z); 
    }

    Vec3 normalize() const { 
        if(this->magnitude() == 0) return Vec3(0, 0, 0);
        return (*this) * (1.0 / this->magnitude()); 
    }

    float dot(const Vec3& otherVec) const {
        return this->x * otherVec.x + this->y * otherVec.y + this->z * otherVec.z;
    }

    Vec3 cross(const Vec3& otherVec) const {
        return Vec3((this->y * otherVec.z - this->z * otherVec.y), (this->z * otherVec.x- this->x * otherVec.z), (this->x * otherVec.y - this->y * otherVec.x));
    }
};

struct Matrix3 {
    Vec3 u;
    Vec3 v;
    Vec3 w;

    constexpr Matrix3() {};

    constexpr Matrix3(const Vec3& u, const Vec3& v, const Vec3& w) : u(u), v(v), w(w) {};

    Vec3 matvec(const Vec3& vec) const {
        Vec3 c1 = u * vec.x;
        Vec3 c2 = v * vec.y;
        Vec3 c3 = w * vec.z;

        return Vec3(c1.x + c2.x + c3.x, c1.y + c2.y + c3.y, c1.z + c2.z + c3.z);
    }

    Matrix3 matmat(const Matrix3& otherM) const {
        return Matrix3(matvec(otherM.u), matvec(otherM.v), matvec(otherM.w));
    }

    // Prevent floating point errors with the rotations for camera
    void orthoNormalize() {
        u = u.normalize();
        v = v - (u * v.dot(u));
        v = v.normalize();
        w = u.cross(v);
        w = w.normalize();
    }
};

struct Ray {
    Vec3 origin;
    Vec3 direction;

    constexpr Ray(const Vec3& o, const Vec3& d) : origin(o), direction(d) {};

    Vec3 parametrize(const float& t) const {
        return origin + direction * t;
    }
};

struct Color {
    float r;
    float g;
    float b;

    constexpr Color(const float& r = 0, const float& g = 0, const float& b = 0) : r(r), g(g), b(b) {};

    Color operator*(const float& coeff) const {
        return Color(r * coeff, g * coeff, b * coeff);
    }

    Color operator/(const float& coeff) const {
        return Color(r / coeff, g / coeff, b / coeff);
    }

    Color operator*(const Color& otherColor) const {
        return Color(r * otherColor.r, g * otherColor.g, b * otherColor.b);
    }

    Color operator+(const Color& otherColor) const {
        return Color(r + otherColor.r, g + otherColor.g, b + otherColor.b);
    }

    void clamp() {
        r = std::min(r, 255.0f);
        g = std::min(g, 255.0f);
        b = std::min(b, 255.0f);
    }
};

// Sum of the coefficients should be 0 <= sum <= 1
struct Material {
    Color ambientColor;
    Color diffuseColor;
    Color specularColor;
    float ambientCoeff;
    float diffuseCoeff;
    float specularCoeff;
    float specularExp;
    bool glazed;

    constexpr Material(const Color& ac, const Color& dc, const Color& sc, const float& ace, const float& dce, const float& sce, const float& se, const bool& g) : 
    ambientColor(ac), diffuseColor(dc), specularColor(sc), ambientCoeff(ace), diffuseCoeff(dce), specularCoeff(sce), specularExp(se), glazed(g) {};

    Color getColor(const Ray& ray, const Vec3& normal, const Vec3& lightDir, const float& lightIntensity, const bool& shadow) const {
        Vec3 reversedLight = -lightDir; 

        // Ambient Light
        Color ambient = ambientColor * ambientCoeff;

        if(shadow) return ambient; // No specular and diffuse lighting if there should be shadow

        // Diffuse Light
        Color diffuse = diffuseColor * diffuseCoeff * std::max(0.0f, normal.dot(reversedLight)) * lightIntensity;
        
        // Specular Light
        Vec3 viewDir = -ray.direction.normalize();
        Vec3 bisector = (reversedLight + viewDir).normalize();
        Color specular = specularColor * specularCoeff * std::pow((std::max(0.0f, normal.dot(bisector))), specularExp) * lightIntensity;

        return ambient + diffuse + specular;
    };
};

struct Surface {
    Material material;

    constexpr Surface(const Material& m) : material(m) {};
    virtual ~Surface() = default;

    virtual std::pair<bool, float> intersection(const Ray& ray) const = 0;
    virtual Vec3 normal(const Vec3& point) const = 0;
};

struct Sphere : Surface {
    Vec3 center;
    float radius;

    constexpr Sphere(const Vec3& cent, const float& r, const Material& m) : Surface(m), center(cent), radius(r) {};

    std::pair<bool, float> intersection(const Ray& ray) const override {
        Vec3 CtoO = ray.origin - this->center;
        float a = ray.direction.dot(ray.direction);
        float b = 2.0 * CtoO.dot(ray.direction);
        float c = CtoO.dot(CtoO) - this->radius * this->radius;

        float discriminant = b * b - (4 * a * c);

        if (discriminant < 0) return {false, 0.0};

        float t1 = (-b - std::sqrt(discriminant)) / (2.0 * a);
        float t2 = (-b + std::sqrt(discriminant)) / (2.0 * a);

        // Makes sure to grab the closest point to the camera
        if(t1 > 0.001) return {true, t1};
        else if (t2 > 0.001) return {true, t2};
        else return {false, 0.0};
    }

    Vec3 normal(const Vec3& point) const override {
        return (point - center).normalize();
    }
};

struct Plane : Surface {
    Vec3 point;
    Vec3 normalVec;

    constexpr Plane(const Vec3& p, const Vec3& n, const Material& m) : Surface(m), point(p), normalVec(n){};

    std::pair<bool, float> intersection(const Ray& ray) const override {
        float t = ((point - ray.origin).dot(normalVec)) / (ray.direction.dot(normalVec));

        // Makes sure to grab the point IF in front of camera
        if(t > 0) return {true, t};
        else return {false, 0.0};
    }

    Vec3 normal(const Vec3& point) const override {
        return normalVec;
    }
};

struct Camera {
    Vec3 origin;
    Matrix3 basis;

    Camera(const Vec3& origin, const Vec3& lookAt, const Vec3& orientation) : origin(origin) {
        basis.w = ((lookAt - origin).normalize());
        basis.u = (basis.w.cross(orientation).normalize());
        basis.v = (-(basis.u.cross(basis.w)));
    }

    virtual Ray getRay(const float x, const float y) const = 0;
    virtual ~Camera() = default;

    // Camera movement
    void updateOrigin(const Vec3& shift) {
        origin = origin + shift;
    }

    // Camera rotations
    void yaw(const float& theta) {
        Matrix3 rotation(Vec3(std::cos(theta), 0, -std::sin(theta)), Vec3(0, 1, 0), Vec3(std::sin(theta), 0, std::cos(theta)));
        Matrix3 newBasis = basis.matmat(rotation);
        newBasis.orthoNormalize();
        basis = newBasis;
    }

    void pitch(const float& theta) {
        Matrix3 rotation(Vec3(1, 0, 0), Vec3(0, std::cos(theta), std::sin(theta)), Vec3(0, -std::sin(theta), std::cos(theta)));
        Matrix3 newBasis = basis.matmat(rotation);
        newBasis.orthoNormalize();
        basis = newBasis;
    }

    void roll(const float& theta) {
        Matrix3 rotation(Vec3(std::cos(theta), std::sin(theta), 0), Vec3(-std::sin(theta), std::cos(theta), 0), Vec3(0, 0, 1));
        Matrix3 newBasis = basis.matmat(rotation);
        newBasis.orthoNormalize();
        basis = newBasis;
    }
};

struct OrthographicCamera : Camera {
    OrthographicCamera(const Vec3& origin, const Vec3& lookAt, const Vec3& orientation) : 
    Camera(origin, lookAt, orientation) {};

    Ray getRay(const float x, const float y) const override {
        return Ray(origin + basis.u * x + basis.v * y, basis.w);
    }
};

struct PerspectiveCamera : Camera {
    float distToImage; // Determines FOV

    PerspectiveCamera(const Vec3& origin, const Vec3& lookAt, const Vec3& orientation, const float d) : 
    Camera(origin, lookAt, orientation), distToImage(d) {};

    Ray getRay(const float x, const float y) const override {
        return Ray(origin, (basis.w * distToImage + basis.u * x + basis.v * y).normalize());
    }
};

struct DirectionalLight {
    Vec3 direction;
    float intensity;

    DirectionalLight(const Vec3& d, const float& i) : direction(d.normalize()), intensity(i) {};
};

struct Scene {
    DirectionalLight light;
    std::vector<Surface*> surfaces;
    Camera* cameras[2]; // 0 index will be orthographic, 1 will be perspective
    Camera* currentCamera;
    bool toggleCamera = 1; // Starts on perspective as default

    Scene(const DirectionalLight& dl, std::vector<Surface*>& s, Camera* c[]) : light(dl), surfaces(s) { 
        currentCamera = cameras[toggleCamera] = c[toggleCamera];
        cameras[!toggleCamera] = c[!toggleCamera];
    }

    ~Scene() {
        for (Surface* surface : surfaces) delete surface;
        for (Camera* camera : cameras) delete camera;
    }

    Color getPixelColor(const float& x, const float& y, int recurse = 1) const {
        Color color; // Gets initialized to black (0, 0, 0)
        Ray cameraRay = currentCamera->getRay(x, y);

        float tClosest = std::numeric_limits<float>::max(); 
        const Surface* closestSurface = nullptr;

        for(const Surface* surface : surfaces) {
            std::pair<bool, float> intersection = surface->intersection(cameraRay);

            if(intersection.first && intersection.second < tClosest) {
                tClosest = intersection.second;
                closestSurface = surface;
            }
        }

        if(!closestSurface) return color; // If we did not hit return a black pixel

        Vec3 pointHit = cameraRay.parametrize(tClosest);
        Vec3 surfaceNormal = closestSurface->normal(pointHit);
        Material surfaceMat = closestSurface->material;
        bool shadow = castShadow(pointHit, surfaceNormal);

        color = surfaceMat.getColor(cameraRay, surfaceNormal, light.direction, light.intensity, shadow);

        if(!surfaceMat.glazed || recurse <= 0) return color;

        Vec3 reflectionDirection = cameraRay.direction - (surfaceNormal * surfaceNormal.dot(cameraRay.direction) * 2);
        Ray reflectedRay(pointHit + surfaceNormal * 0.001f, reflectionDirection); // Adds a little buffer space so we do not hit the same point again

        color = color + (reflectionColor(reflectedRay, --recurse) * surfaceMat.specularCoeff * surfaceMat.specularColor / 255.0f);
        color.clamp();

        return color;
    }

    Color reflectionColor(const Ray& ray, int& recurse) const {
        Color color;

        float tClosest = std::numeric_limits<float>::max(); 
        const Surface* closestSurface = nullptr;

        for(const Surface* surface : surfaces) {
            std::pair<bool, float> intersection = surface->intersection(ray);

            if(intersection.first && intersection.second < tClosest) {
                tClosest = intersection.second;
                closestSurface = surface;
            }
        }

        if(!closestSurface) return color; // If we did not hit return a black pixel

        Vec3 pointHit = ray.parametrize(tClosest);
        Vec3 surfaceNormal = closestSurface->normal(pointHit);
        const Material surfaceMat = closestSurface->material;
        bool shadow = castShadow(pointHit, surfaceNormal);

        color = surfaceMat.getColor(ray, surfaceNormal, light.direction, light.intensity, shadow);

        if(!surfaceMat.glazed || recurse <= 0) return color;

        Vec3 rayDirNorm = ray.direction.normalize();
        Vec3 reflectionDirection = rayDirNorm - (surfaceNormal * surfaceNormal.dot(rayDirNorm) * 2);
        Ray reflectedRay(pointHit + surfaceNormal * 0.001f, reflectionDirection);

        color = color + (reflectionColor(reflectedRay, --recurse) * surfaceMat.specularCoeff * surfaceMat.specularColor / 255.0f);

        return color;
    }

    bool castShadow(const Vec3& pointHit, const Vec3& surfaceNormal) const {
        Ray shadowRay(pointHit + surfaceNormal * 0.001f, -light.direction);

        for(const Surface* surface : surfaces) {
            std::pair<bool, float> intersection = surface->intersection(shadowRay);

            if(intersection.first) return true;
        }

        return false;
    }

    void switchCamera() {
        toggleCamera = !toggleCamera;
        currentCamera = cameras[toggleCamera];
    }
};