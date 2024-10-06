#pragma once
#include "Class.h"

#define PI 3.141592f
#define iSteps 16
#define jSteps 8

class MBasicPlatform;
class MScene;

static glm::vec2 rsi(const glm::vec3& r0, const glm::vec3& rd, float sr) {
    // ray-sphere intersection that assumes
    // the sphere is centered at the origin.
    // No intersection when result.x > result.y
    float a = glm::dot(rd, rd);
    float b = 2.0f * dot(rd, r0);
    float c = dot(r0, r0) - (sr * sr);
    float d = (b * b) - 4.0f * a * c;
    if (d < 0.0f) return glm::vec2(1e5, -1e5);
    return glm::vec2(
        (-b - sqrt(d)) / (2.0f * a),
        (-b + sqrt(d)) / (2.0f * a)
    );
}

static glm::vec3 atmosphere(glm::vec3 r, glm::vec3 r0, glm::vec3 pSun, float iSun, float rPlanet, float rAtmos, glm::vec3 kRlh, float kMie, float shRlh, float shMie, float g) {
    // Normalize the sun and view directions.
    pSun = glm::normalize(pSun);
    r = glm::normalize(r);

    // Calculate the step size of the primary ray.
    glm::vec2 p = rsi(r0, r, rAtmos);
    if (p.x > p.y) return glm::vec3(0, 0, 0);
    p.y = std::min(p.y, rsi(r0, r, rPlanet).x);
    float iStepSize = (p.y - p.x) / float(iSteps);

    // Initialize the primary ray time.
    float iTime = 0.0f;

    // Initialize accumulators for Rayleigh and Mie scattering.
    glm::vec3 totalRlh = glm::vec3(0, 0, 0);
    glm::vec3 totalMie = glm::vec3(0, 0, 0);

    // Initialize optical depth accumulators for the primary ray.
    float iOdRlh = 0.0f;
    float iOdMie = 0.0f;

    // Calculate the Rayleigh and Mie phases.
    float mu = dot(r, pSun);
    float mumu = mu * mu;
    float gg = g * g;
    float pRlh = 3.0f / (16.0f * PI) * (1.0f + mumu);
    float pMie = 3.0f / (8.0f * PI) * ((1.0f - gg) * (mumu + 1.0f)) / (pow(1.0f + gg - 2.0f * mu * g, 1.5f) * (2.0f + gg));

    // Sample the primary ray.
    for (int i = 0; i < iSteps; i++) {

        // Calculate the primary ray sample position.
        glm::vec3 iPos = r0 + r * (iTime + iStepSize * 0.5f);

        // Calculate the height of the sample.
        float iHeight = glm::length(iPos) - rPlanet;

        // Calculate the optical depth of the Rayleigh and Mie scattering for this step.
        float odStepRlh = exp(-iHeight / shRlh) * iStepSize;
        float odStepMie = exp(-iHeight / shMie) * iStepSize;

        // Accumulate optical depth.
        iOdRlh += odStepRlh;
        iOdMie += odStepMie;

        // Calculate the step size of the secondary ray.
        float jStepSize = rsi(iPos, pSun, rAtmos).y / float(jSteps);

        // Initialize the secondary ray time.
        float jTime = 0.0f;

        // Initialize optical depth accumulators for the secondary ray.
        float jOdRlh = 0.0f;
        float jOdMie = 0.0f;

        // Sample the secondary ray.
        for (int j = 0; j < jSteps; j++) {

            // Calculate the secondary ray sample position.
            glm::vec3 jPos = iPos + pSun * (jTime + jStepSize * 0.5f);

            // Calculate the height of the sample.
            float jHeight = glm::length(jPos) - rPlanet;

            // Accumulate the optical depth.
            jOdRlh += exp(-jHeight / shRlh) * jStepSize;
            jOdMie += exp(-jHeight / shMie) * jStepSize;

            // Increment the secondary ray time.
            jTime += jStepSize;
        }

        // Calculate attenuation.
        glm::vec3 attn = exp(-(kMie * (iOdMie + jOdMie) + kRlh * (iOdRlh + jOdRlh)));

        // Accumulate scattering.
        totalRlh += odStepRlh * attn;
        totalMie += odStepMie * attn;

        // Increment the primary ray time.
        iTime += iStepSize;

    }

    // Calculate and return the final color.
    return iSun * (pRlh * kRlh * totalRlh + pMie * kMie * totalMie);
}

struct MAtmosphereParameters {
	float iSun;
	float rPlanet;
	float rAtmos;
	glm::vec3 kRlh;
	float kMie;
	float shRlh;
	float shMie;
	float g;
};

class MTimeOfDay :public MUnscriptableObject {
public:
	MTimeOfDay(MBasicPlatform* platform, MScene* parent, bool isDynamic, double speed, float xoffset, const MAtmosphereParameters& ap);
	~MTimeOfDay();
	MAtmosphereParameters mAtmosphereParameters;
	glm::vec3 GetSunDirection() { return mDirection; }
	glm::vec3 GetSunColor() { return mColor; }
	void Update();
	void SetTime(double t);
    double GetTime();
	void SetDynamic(bool flag);
	glm::vec3 mDirection = glm::vec3(0.0f);
	glm::vec3 mColor = glm::vec3(0.0f);
	MScene* mParent = NULL;
	double mSpeed = 1.0;
    double mCurrentTime = 0.0, mInitialTime = 0.0;
    float mXOffset = 0.2f;
	bool mDynamicFlag = false;
};