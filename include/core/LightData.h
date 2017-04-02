//
//  LightData.h
//  SimpleRenderer
//
//  Created by Hesam Ohadi on 28/11/15.
//
//

#ifndef __SimpleRenderer__LightData__
#define __SimpleRenderer__LightData__

#include "cinder/Color.h"
#include "cinder/Vector.h"

/////////////////////Attenuation/////////////

class Attenuation
{
public:
    Attenuation();
    ~Attenuation();
    
    //setters/getters
    Attenuation&                    constant(float aConstant);
    Attenuation&                    linear(float aLinear);
    Attenuation&                    exp(float aExp);
    float                           getConstant() const;
    float                           getLinear() const;
    float                           getExp() const;
    void                            setConstant(float aConstant);
    void                            setLinear(float aLinear);
    void                            setExp(float aExp);
    
public:
    float                           mConstant;
    float                           mLinear;
    float                           mExp;
};

//////////////////////LightData//////////////////////

class LightData
{
public:
    
    LightData();
    ~LightData();
    
    //setters / getters
    LightData&                      color(const ci::Colorf& aColor);
    LightData&                      ambientIntensity(float aAmbientIntensity);
    const ci::Colorf&               getColor() const;
    float                           getAmbientIntensity() const;
    void                            setColor(const ci::Colorf aColor);
    void                            setAmbientIntensity(float aAmbientIntensity);
    
    
public:
    ci::Colorf                      mColor;
    float                           mAmbientIntensity;
    
};

///////////////////////DirectionalLightData///////////////////

class DirectionalLightData : public LightData
{
public:
    
    DirectionalLightData();
    ~DirectionalLightData();
    
    //setters/getters
    DirectionalLightData&           direction(const ci::vec3& aDirection);
    DirectionalLightData&                      diffuseIntensity(float aDiffuseIntensity);
    const ci::vec3&                 getDirection() const;
    void                            setDirection(const ci::vec3& aDirection);
    void                            setDiffuseIntensity(float aDiffuseIntensity);
    float                           getDiffuseIntensity() const;
    
public:
    ci::vec3                        mDirection;
    float                           mDiffuseIntensity;
    
};

///////////////////////PointLightData///////////////////

class PointLightData : public LightData
{
public:
    PointLightData();
    ~PointLightData();
    
    //setters / getters
    PointLightData&                 position(const ci::vec3& aPosition);
    PointLightData&                 diffuseIntensity(float aDiffuseIntensity);
    PointLightData&                 attenuation(const Attenuation& aAttenuation);
    PointLightData&                 radius(float aRadius);
    const ci::vec3&                 getPositon() const;
    const Attenuation&              getAttenuation() const;
    void                            setPosition(const ci::vec3& aPosition);
    void                            setAttenuation(const Attenuation& aAttenuation);
    void                            setRadius(float aRadius);
    void                            setDiffuseIntensity(float aDiffuseIntensity);
    float                           getDiffuseIntensity() const;
    float                           getRadius() const;
    
public:
    ci::vec3                        mPosition;
    float                           mDiffuseIntensity;
    Attenuation                     mAttenuation;
    float                           mRadius;
};

//////////////////////SpotLightData//////////////////////

class SpotLightData : public PointLightData
{
public:
    SpotLightData();
    ~SpotLightData();
    
    //setters / getters
    SpotLightData&                  direction(const ci::vec3& aDirection);
    SpotLightData&                  cutoff(float aCutoff);
    const ci::vec3&                 getDirection() const;
    float                           getCutoff() const;
    void                            setDirection(const ci::vec3& aDirection);
    void                            setCutoff(float aCutoff);
    
public:
    ci::vec3                        mDirection;
    float                           mCutoff;
};



#endif /* defined(__SimpleRenderer__LightData__) */