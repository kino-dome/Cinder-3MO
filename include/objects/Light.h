#pragma once

#include "cinder/Camera.h"
#include "cinder/Color.h"
#include "cinder/Vector.h"

#include "../core/Object3D.h"
#include "../core/LightData.h"

typedef std::shared_ptr<class LightObject> LightObjectRef;
typedef std::shared_ptr<class DirectionalLight> DirectionalLightRef;
typedef std::shared_ptr<class PointLight> PointLightRef;
typedef std::shared_ptr<class SpotLight> SpotLightRef;

//////////////////LightObject////////////////////////////

class LightObject : public virtual Object3D
{
public:
    
	LightObject();
    virtual ~LightObject()=0;
    virtual const LightData&        getLightData() const =0;
    virtual LightData&              getLightData() =0;
    
    //forwarding methods
    LightObject&                    color(const ci::Colorf& aColor);
    LightObject&                    ambientIntensity(float aAmbientIntensity);
    const ci::Colorf&               getColor() const;
    float                           getAmbientIntensity() const;
    void                            setColor(const ci::Colorf& aColor);
    void                            setAmbientIntensity(float aAmbientIntensity);
    
    
public:
    

};


//////////////////DirectionalLight/////////////////////
class DirectionalLight : public virtual LightObject
{
public:
    static DirectionalLightRef create(){
        return DirectionalLightRef ( new DirectionalLight());
    }
    
    DirectionalLight();
    ~DirectionalLight();
    OBJECT3D_CLONE(DirectionalLight)
    
    void                            updateSelf() override;
    void                            drawSelf() override;
    const DirectionalLightData&     getLightData() const override;
    DirectionalLightData&           getLightData() override;
    
    //forwarding methods
    DirectionalLight&               direction(const ci::vec3& aDirection);
    DirectionalLight&               diffuseIntensity(float aDiffuseIntensity);
    const ci::vec3&                 getDirection() const;
    float                           getDiffuseIntensity() const;
    void                            setDirection(const ci::vec3& aDirection);
    void                            setDiffuseIntensity(float aDiffuseIntensity);
    
public:
    DirectionalLightData            mLightData;
};

//////////////////PointLight/////////////////////
class PointLight : public virtual LightObject
{
public:
    static PointLightRef create(){
        return PointLightRef ( new PointLight());
    }
    
    PointLight();
    ~PointLight();
    OBJECT3D_CLONE(PointLight)
    
    void                            updateSelf() override;
    void                            drawSelf() override;
    const PointLightData&           getLightData() const override;
    PointLightData&                 getLightData() override;
    
    float                           calcBSphere() const;
    
    //forwarding methods
    PointLight&                     position(const ci::vec3& aPosition);
    PointLight&                     diffuseIntensity(float aDiffuseIntensity);
    PointLight&                     attenuation(const Attenuation& aAttenuation);
    PointLight&                     radius(float aRadius);
    
    const ci::vec3&                 getPosition() const;
    float                           getDiffuseIntensity() const;
    const Attenuation&              getAttenuation() const;
    float                           getRadius() const;
    void                            setPosition(const ci::vec3& aPosition);
    void                            setDiffuseIntensity(float aDiffuseIntensity);
    void                            setAttenuation(const Attenuation& aAttenuation);
    void                            setRadius(float aRadius);
    
public:
    PointLightData                  mLightData;
};

//////////////////SpotLight/////////////////////
class SpotLight : public virtual LightObject
{
public:
    static SpotLightRef create(){
        return SpotLightRef ( new SpotLight());
    }
    
    SpotLight();
    ~SpotLight();
    OBJECT3D_CLONE(SpotLight)
    
    void                            updateSelf() override;
    void                            drawSelf() override;
    const SpotLightData&            getLightData() const override;
    SpotLightData&                  getLightData() override;
    
    float                           calcBSphere() const;
    
    //forwarding methods
    SpotLight&                      position(const ci::vec3& aPosition);
    SpotLight&                      diffuseIntensity(float aDiffuseIntensity);
    SpotLight&                      attenuation(const Attenuation& aAttenuation);
    SpotLight&                      radius(float aRadius);
    SpotLight&                      direction(const ci::vec3& aDirection);
    SpotLight&                      cutoff(float aCutoff);
    
    const ci::vec3&                 getPosition() const;
    float                           getDiffuseIntensity() const;
    const Attenuation&              getAttenuation() const;
    float                           getRadius() const;
    const ci::vec3&                 getDirection() const;
    float                           getCutoff() const;
    void                            setPosition(const ci::vec3& aPosition);
    void                            setDiffuseIntensity(float aDiffuseIntensity);
    void                            setAttenuation(const Attenuation& aAttenuation);
    void                            setRadius(float aRadius);
    void                            setDirection(const ci::vec3& aDirection);
    void                            setCutoff(float aCutoff);
    
public:
    SpotLightData                   mLightData;
};