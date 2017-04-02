//
//  MaterialData.h
//  SimpleRenderer
//
//  Created by Hesam Ohadi on 4/12/15.
//
//

#pragma once

#include "cinder/Color.h"
#include "cinder/Vector.h"

//base class for Material Data (Classic or Pbr)
//can't have any virtual methods cause it will mess the size and alignment of the object
class MaterialData
{
public:
    MaterialData() {}
    ~MaterialData() {}
};


class ClassicMaterialData : public MaterialData
{
public:
    ClassicMaterialData() : MaterialData(), mDiffuseColor(ci::Colorf(1.0, 1.0, 1.0)), mDiffuseIntensity(1.0), mSpecularColor(ci::Colorf(1.0, 1.0, 1.0)), mSpecularIntensity(0.0), mEmissionColor(ci::Colorf(1.0, 1.0, 1.0)), mEmissionIntensity(0.0), mShininess(1.0), mPad0(0), mPad1(0), mPad2(0) {}
    ~ClassicMaterialData() {}
    bool                            getIsPbr() const{return false;}
    
    //setters/getters
    ClassicMaterialData&            diffuseColor(const ci::Colorf& aDiffuseColor) {mDiffuseColor = aDiffuseColor; return *this;}
    ClassicMaterialData&            diffuseIntensity(float aDiffuseIntensity) {mDiffuseIntensity = aDiffuseIntensity; return *this;}
    ClassicMaterialData&            specularColor(const ci::Colorf& aSpecularColor) {mSpecularColor = aSpecularColor; return *this;}
    ClassicMaterialData&            specularIntensity(float aSpecularIntensity) {mSpecularIntensity = aSpecularIntensity; return *this;}
    ClassicMaterialData&            emissionColor(const ci::Colorf& aEmissionColor) {mEmissionColor = aEmissionColor; return *this;}
    ClassicMaterialData&            emissionIntensity(float aEmissionIntensity) {mEmissionIntensity = aEmissionIntensity; return *this;}
    ClassicMaterialData&            shininess(float aShininess) {mShininess = aShininess; return *this;}
    
    const ci::Colorf&               getDiffuseColor() const {return mDiffuseColor;}
    float                           getDiffuseIntensity() const {return mDiffuseIntensity;}
    const ci::Colorf&               getSpecularColor() const {return mSpecularColor;}
    float                           getSpecularIntensity() const {return mSpecularIntensity;}
    const ci::Colorf&               getEmissionColor() const {return mEmissionColor;}
    float                           getEmissionIntensity() const {return mEmissionIntensity;}
    float                           getShininess() const {return mShininess;}
    
    void                            setDiffuseColor(const ci::Colorf& aDiffuseColor) {mDiffuseColor = aDiffuseColor;}
    void                            setDiffuseIntensity(float aDiffuseIntensity) {mDiffuseIntensity = aDiffuseIntensity;}
    void                            setSpecularColor(const ci::Colorf& aSpecularColor) {mSpecularColor = aSpecularColor;}
    void                            setSpecularIntensity(float aSpecularIntensity) {mSpecularIntensity = aSpecularIntensity;}
    void                            setEmissionColor(const ci::Colorf& aEmissionColor) {mEmissionColor = aEmissionColor;}
    void                            setEmissionIntensity(float aEmissionIntensity) {mEmissionIntensity = aEmissionIntensity;}
    void                            setShininess(float aShininess) {mShininess = aShininess;}
    
    
public:
    //ALIGNMENT
    ci::Colorf                      mDiffuseColor;
    float                           mDiffuseIntensity;
    //ALIGNMENT
    ci::Colorf                      mSpecularColor;
    float                           mSpecularIntensity;
    //ALIGNMENT
    ci::Colorf                      mEmissionColor;
    float                           mEmissionIntensity;
    //ALIGNMENT
    float                           mShininess;
    float                           mAlpha;
    uint32_t                        mPad0, mPad1, mPad2;
};

class PbrMaterialData : public MaterialData
{
public:
    PbrMaterialData() : MaterialData(), mBaseColor(ci::ColorAf(1.0, 1.0, 1.0, 1.0)), mEmissionColor(ci::Colorf(1.0, 1.0, 1.0)), mEmissionIntensity(1.0), mMetallic(0.0), mRoughness(0.0), mSpecular(1.0), mPad0(0) {}
    ~PbrMaterialData() {}
    
    bool                            getIsPbr() const {return true;}
    
    //setters/getters
    PbrMaterialData&                baseColor(const ci::ColorAf& aBaseColor) {mBaseColor = aBaseColor; return *this;}
    PbrMaterialData&                emissionColor(const ci::Colorf& aEmissionColor) {mEmissionColor = aEmissionColor; return *this;}
    PbrMaterialData&                emissionIntensity(float aEmissionIntensity) {mEmissionIntensity = aEmissionIntensity; return *this;}
    PbrMaterialData&                metallic(float aMetallic) {mMetallic = aMetallic; return *this;}
    PbrMaterialData&                roughness(float aRoughness) {mRoughness = aRoughness; return *this;}
    PbrMaterialData&                specular(float aSpecular) {mSpecular = aSpecular; return *this;}
    
    const ci::ColorAf&              getBaseColor() const {return mBaseColor;}
    const ci::Colorf&               getEmissionColor() const {return mEmissionColor;}
    float                           getEmissionIntensity() const {return mEmissionIntensity;}
    float                           getMetallic() const {return mMetallic;}
    float                           getRoughness() const {return mRoughness;}
    float                           getSpecular() const {return mSpecular;}
    
    void                            setBaseColor(const ci::ColorAf& aBaseColor) {mBaseColor = aBaseColor;}
    void                            setEmissionColor(const ci::Colorf& aEmissionColor) {mEmissionColor = aEmissionColor;}
    void                            setEmissionIntensity(float aEmissionIntensity) {mEmissionIntensity = aEmissionIntensity;}
    void                            setMetallic(float aMetallic) {mMetallic = aMetallic;}
    void                            setRoughness(float aRoughness) {mRoughness = aRoughness;}
    void                            setSpecular(float aSpecular) {mSpecular = aSpecular;}
    
public:
    //ALIGNMENT
    ci::ColorAf                     mBaseColor;
    //ALIGNMENT
    ci::Colorf                      mEmissionColor;
    float                           mEmissionIntensity;
    //ALIGNMENT
    float                           mMetallic;
    float                           mRoughness;
    float                           mSpecular;
    uint32_t                        mPad0;
};

