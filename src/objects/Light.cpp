#include "Light.h"

using namespace ci;
using namespace std;


//////////////////LightObject////////////////////////////

LightObject::LightObject() : Object3D()
{
    mShouldNotifyTopParent = true;
    mType = "light";
    addTag("light");
    mDoesCastShadow = false;
    mName = "light";
}

LightObject::~LightObject()
{
    
}

LightObject& LightObject::color(const ci::Colorf &aColor)
{
    getLightData().setColor(aColor);
    return *this;
}

LightObject& LightObject::ambientIntensity(float aAmbientIntensity)
{
    getLightData().setAmbientIntensity(aAmbientIntensity);
    return *this;
}

const ci::Colorf& LightObject::getColor() const
{
    return getLightData().getColor();
}

float LightObject::getAmbientIntensity() const
{
    return getLightData().getAmbientIntensity();
}

void LightObject::setColor(const ci::Colorf &aColor)
{
    getLightData().setColor(aColor);
}

void LightObject::setAmbientIntensity(float aAmbientIntensity)
{
    getLightData().setAmbientIntensity(aAmbientIntensity);
}



//////////////////DirectionalLight/////////////////////
DirectionalLight::DirectionalLight() : LightObject()
{
    mLightData = DirectionalLightData();
    mType = "directional_light";
    addTag("directional_light");
    mName = "directional_light";
}

DirectionalLight::~DirectionalLight()
{
    
}

const DirectionalLightData& DirectionalLight::getLightData() const
{
    return mLightData;
}

DirectionalLightData& DirectionalLight::getLightData()
{
    return mLightData;
}

void DirectionalLight::updateSelf()
{
    if (mShouldUpdate){
        mLightData.setDirection(getViewDirection());
    }
}

void DirectionalLight::drawSelf()
{
    
}

DirectionalLight& DirectionalLight::direction(const ci::vec3 &aDirection)
{
    mLightData.setDirection(aDirection);
    setViewDirection(aDirection);
    return *this;
}

DirectionalLight& DirectionalLight::diffuseIntensity(float aDiffuseIntensity)
{
    getLightData().setDiffuseIntensity(aDiffuseIntensity);
    return *this;
}

const ci::vec3& DirectionalLight::getDirection() const
{
    return mLightData.getDirection();
}

float DirectionalLight::getDiffuseIntensity() const
{
    return getLightData().getDiffuseIntensity();
}

void DirectionalLight::setDirection(const ci::vec3 &aDirection)
{
    mLightData.setDirection(aDirection);
    setViewDirection(aDirection);
}

void DirectionalLight::setDiffuseIntensity(float aDiffuseIntensity)
{
    getLightData().setDiffuseIntensity(aDiffuseIntensity);
}

//////////////////PointLight/////////////////////
PointLight::PointLight() : LightObject()
{
    mLightData = PointLightData();
    mType = "point_light";
    addTag("point_light");
    mName = "point_light";
}

PointLight::~PointLight()
{
    
}

const PointLightData& PointLight::getLightData() const
{
    return mLightData;
}

PointLightData& PointLight::getLightData()
{
    return mLightData;
}


float PointLight::calcBSphere() const
{
    float maxChannel = ci::math<float>::max(getLightData().getColor().b ,ci::math<float>::max(getLightData().getColor().r, getLightData().getColor().g));
    float linear = getLightData().getAttenuation().getLinear();
    float exp = getLightData().getAttenuation().getExp();
    float diffuseIntensity = getLightData().getDiffuseIntensity();
    float ret = (-linear  + sqrtf( linear * linear  - 4 * exp * (exp - 256 * maxChannel * diffuseIntensity))) / 2 * exp;
    return ret;
}


void PointLight::updateSelf()
{
    if (mShouldUpdate){
        mLightData.setPosition(mModelWorld.getModelTranslation());
    }
}

void PointLight::drawSelf()
{
    
}

PointLight& PointLight::position(const ci::vec3 &aPosition)
{
    setPosition(aPosition);
    return *this;
}

PointLight& PointLight::diffuseIntensity(float aDiffuseIntensity)
{
    getLightData().setDiffuseIntensity(aDiffuseIntensity);
    setRadius(calcBSphere());
    return *this;
}

PointLight& PointLight::attenuation(const Attenuation &aAttenuation)
{
    mLightData.setAttenuation(aAttenuation);
    setRadius(calcBSphere());
    return *this;
}

PointLight& PointLight::radius(float aRadius)
{
    mLightData.setRadius(aRadius);
    return *this;
}

const ci::vec3& PointLight::getPosition() const
{
    return getLightData().getPositon();
}

float PointLight::getDiffuseIntensity() const
{
    return getLightData().getDiffuseIntensity();
}

const Attenuation& PointLight::getAttenuation() const
{
    return mLightData.getAttenuation();
}

float PointLight::getRadius() const
{
    return mLightData.getRadius();
}

void PointLight::setPosition(const ci::vec3& aPosition)
{
    translate(aPosition-mModelWorld.getModelTranslation());
    setShouldUpdate(true);
}

void PointLight::setDiffuseIntensity(float aDiffuseIntensity)
{
    getLightData().setDiffuseIntensity(aDiffuseIntensity);
    setRadius(calcBSphere());
}

void PointLight::setAttenuation(const Attenuation &aAttenuation)
{
    mLightData.setAttenuation(aAttenuation);
    setRadius(calcBSphere());
}

void PointLight::setRadius(float aRadius)
{
    mLightData.setRadius(aRadius);
}

//////////////////SpotLight/////////////////////
SpotLight::SpotLight() : LightObject()
{
    mLightData = SpotLightData();
    mType = "spot_light";
    addTag("spot_light");
    mName = "spot_light";
}

SpotLight::~SpotLight()
{
    
}

const SpotLightData& SpotLight::getLightData() const
{
    return mLightData;
}

SpotLightData& SpotLight::getLightData()
{
    return mLightData;
}

void SpotLight::updateSelf()
{
    if (mShouldUpdate){
        mLightData.setPosition(mModelWorld.getModelTranslation());
        mLightData.setDirection(mViewDirection);
    }
}

void SpotLight::drawSelf()
{
    
}

float SpotLight::calcBSphere() const
{
    float maxChannel = ci::math<float>::max(getLightData().getColor().b ,ci::math<float>::max(getLightData().getColor().r, getLightData().getColor().g));
    float linear = getLightData().getAttenuation().getLinear();
    float exp = getLightData().getAttenuation().getExp();
    float diffuseIntensity = getLightData().getDiffuseIntensity();
    float ret = (-linear  + sqrtf( linear * linear  - 4 * exp * (exp - 256 * maxChannel * diffuseIntensity))) / 2 * exp;
    return ret;
}

SpotLight& SpotLight::position(const ci::vec3 &aPosition)
{
    setPosition(aPosition);
    return *this;
}

SpotLight& SpotLight::diffuseIntensity(float aDiffuseIntensity)
{
    getLightData().setDiffuseIntensity(aDiffuseIntensity);
    setRadius(calcBSphere());
    return *this;
}

SpotLight& SpotLight::attenuation(const Attenuation &aAttenuation)
{
    mLightData.setAttenuation(aAttenuation);
    setRadius(calcBSphere());
    return *this;
}

SpotLight& SpotLight::radius(float aRadius)
{
    mLightData.setRadius(aRadius);
    return *this;
}

SpotLight& SpotLight::direction(const ci::vec3 &aDirection)
{
    mLightData.setDirection(aDirection);
    return *this;
}

SpotLight& SpotLight::cutoff(float aCutoff)
{
    mLightData.setCutoff(aCutoff);
    return *this;
}

const ci::vec3& SpotLight::getPosition() const
{
    return getLightData().getPositon();
}

float SpotLight::getDiffuseIntensity() const
{
    return getLightData().getDiffuseIntensity();
}

const Attenuation& SpotLight::getAttenuation() const
{
    return mLightData.getAttenuation();
}

float SpotLight::getRadius() const
{
    return mLightData.getRadius();
}

const ci::vec3& SpotLight::getDirection() const
{
    return mLightData.getDirection();
}

float SpotLight::getCutoff() const
{
    return mLightData.getCutoff();
}

void SpotLight::setPosition(const ci::vec3& aPosition)
{
    translate(aPosition-mModelWorld.getModelTranslation());
    setShouldUpdate(true);
}

void SpotLight::setDiffuseIntensity(float aDiffuseIntensity)
{
    getLightData().setDiffuseIntensity(aDiffuseIntensity);
    setRadius(calcBSphere());
}

void SpotLight::setAttenuation(const Attenuation &aAttenuation)
{
    mLightData.setAttenuation(aAttenuation);
    setRadius(calcBSphere());
}

void SpotLight::setRadius(float aRadius)
{
    mLightData.setRadius(aRadius);
}

void SpotLight::setDirection(const ci::vec3 &aDirection)
{
    mLightData.setDirection(aDirection);
}

void SpotLight::setCutoff(float aCutoff)
{
    mLightData.setCutoff(aCutoff);
}