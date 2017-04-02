//
//  LightData.cpp
//  SimpleRenderer
//
//  Created by Hesam Ohadi on 28/11/15.
//
//

#include "LightData.h"

/////////////////////Attenuation/////////////
Attenuation::Attenuation() : mConstant(1.0), mExp(1.0), mLinear(1.0)
{
    
}

Attenuation::~Attenuation()
{
    
}

Attenuation& Attenuation::constant(float aConstant)
{
    mConstant = aConstant;
    return *this;
}

Attenuation& Attenuation::linear(float aLinear)
{
    mLinear = aLinear;
    return *this;
}

Attenuation& Attenuation::exp(float aExp)
{
    mExp = aExp;
    return *this;
}

float Attenuation::getConstant() const
{
    return mConstant;
}

float Attenuation::getLinear() const
{
    return mLinear;
}

float Attenuation::getExp() const
{
    return mExp;
}

void Attenuation::setConstant(float aConstant)
{
    mConstant = aConstant;
}

void Attenuation::setLinear(float aLinear)
{
    mLinear = aLinear;
}

void Attenuation::setExp(float aExp)
{
    mExp = aExp;
}

//////////////////////LightData//////////////////////


LightData::LightData() : mColor(ci::Colorf(1.0, 1.0, 1.0)), mAmbientIntensity(1.0)
{
    
}

LightData::~LightData()
{
    
}

LightData& LightData::color(const ci::Colorf &aColor)
{
    setColor(aColor);
    return *this;
}

LightData& LightData::ambientIntensity(float aAmbientIntensity)
{
    setAmbientIntensity(aAmbientIntensity);
    return *this;
}




//setters/getters

const ci::Colorf& LightData::getColor() const
{
    return mColor;
}

float LightData::getAmbientIntensity() const
{
    return mAmbientIntensity;
}


void LightData::setColor(const ci::Colorf aColor)
{
    mColor = aColor;
}

void LightData::setAmbientIntensity(float aAmbientIntensity)
{
    mAmbientIntensity = aAmbientIntensity;
}





//////////////////////DirectionalLightData//////////////////////

DirectionalLightData::DirectionalLightData() : LightData(), mDirection(ci::vec3(1.0, -1.0, 0.0)), mDiffuseIntensity(1.0)
{
    
}

DirectionalLightData::~DirectionalLightData()
{
    
}

DirectionalLightData& DirectionalLightData::direction(const ci::vec3 &aDirection)
{
    mDirection = aDirection;
    return *this;
}

DirectionalLightData& DirectionalLightData::diffuseIntensity(float aDiffuseIntensity)
{
    setDiffuseIntensity(aDiffuseIntensity);
    return *this;
}

const ci::vec3& DirectionalLightData::getDirection() const
{
    return mDirection;
}

float DirectionalLightData::getDiffuseIntensity() const
{
    return mDiffuseIntensity;
}

void DirectionalLightData::setDirection(const ci::vec3 &aDirection)
{
    mDirection = aDirection;
}

void DirectionalLightData::setDiffuseIntensity(float aDiffuseIntensity)
{
    mDiffuseIntensity = aDiffuseIntensity;
}


//////////////////////PointLightData//////////////////////

PointLightData::PointLightData() : LightData(), mPosition(ci::vec3(0.0, 0.0, 0.0)), mDiffuseIntensity(1.0), mAttenuation(Attenuation()), mRadius(0.0)
{
    
}

PointLightData::~PointLightData()
{
    
}

PointLightData& PointLightData::position(const ci::vec3 &aPosition)
{
    mPosition = aPosition;
    return *this;
}

PointLightData& PointLightData::diffuseIntensity(float aDiffuseIntensity)
{
    setDiffuseIntensity(aDiffuseIntensity);
    return *this;
}

PointLightData& PointLightData::attenuation(const Attenuation &aAttenuation)
{
    mAttenuation = aAttenuation;
    return *this;
}

PointLightData& PointLightData::radius(float aRadius)
{
    mRadius = aRadius;
    return *this;
}

const ci::vec3& PointLightData::getPositon() const
{
    return mPosition;
}

float PointLightData::getDiffuseIntensity() const
{
    return mDiffuseIntensity;
}

const Attenuation& PointLightData::getAttenuation() const
{
    return mAttenuation;
}

float PointLightData::getRadius() const
{
    return mRadius;
}

void PointLightData::setPosition(const ci::vec3 &aPosition)
{
    mPosition = aPosition;
}

void PointLightData::setDiffuseIntensity(float aDiffuseIntensity)
{
    mDiffuseIntensity = aDiffuseIntensity;
}

void PointLightData::setAttenuation(const Attenuation &aAttenuation)
{
    mAttenuation = aAttenuation;
}

void PointLightData::setRadius(float aRadius)
{
    mRadius = aRadius;
}


//////////////////////SpotLightData//////////////////////

SpotLightData::SpotLightData() : PointLightData(), mDirection(ci::vec3(0.0, -1.0, 0.0)), mCutoff(45.0)
{
    
}

SpotLightData::~SpotLightData()
{
    
}

SpotLightData& SpotLightData::direction(const ci::vec3 &aDirection)
{
    mDirection = aDirection;
    return *this;
}

SpotLightData& SpotLightData::cutoff(float aCutoff)
{
    mCutoff = aCutoff;
    return *this;
}

const ci::vec3& SpotLightData::getDirection() const
{
    return  mDirection;
}

float SpotLightData::getCutoff() const
{
    return mCutoff;
}

void SpotLightData::setDirection(const ci::vec3 &aDirection)
{
    mDirection = aDirection;
}

void SpotLightData::setCutoff(float aCutoff)
{
    mCutoff = aCutoff;
}
