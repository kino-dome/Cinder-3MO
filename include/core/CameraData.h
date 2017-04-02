//
//  CameraData.h
//  DeferredRenderer
//
//  Created by Hesam Ohadi on 22/12/15.
//
//

#pragma once

#include "cinder/Vector.h"

class CameraData
{
public:
    CameraData() : mViewMatrix(ci::mat4()), mInversedViewMatrix(ci::mat4()), mProjectionMatrix(ci::mat4()), mInversedProjectionMatrix(ci::mat4()), mPosition(ci::vec3()), mViewDirection(ci::vec3()), mProjectionParameters(ci::vec2()), mFov(45.0), mAspectRatio(1.0) {}
    ~CameraData() {}
    
    //setters/getters
    CameraData&                 viewMatrix(const ci::mat4& aViewMatrix) {mViewMatrix = aViewMatrix; return *this;}
    CameraData&                 inversedViewMatrix(const ci::mat4& aInversedViewMatrix) {mInversedViewMatrix = aInversedViewMatrix; return *this;}
    CameraData&                 projectionMatrix(const ci::mat4& aProjectionMatrix) {mProjectionMatrix = aProjectionMatrix; return *this;}
    CameraData&                 inversedProjectionMatrix(const ci::mat4& aInversedProjectionMatrix) {mInversedProjectionMatrix = aInversedProjectionMatrix; return *this;}
    CameraData&                 position(const ci::vec3& aPosition) {mPosition = aPosition; return *this;}
    CameraData&                 viewDirection(const ci::vec3& aViewDirection) {mViewDirection = aViewDirection; return *this;}
    CameraData&                 projectionParameters(const ci::vec2& aProjectionParameters) {mProjectionParameters = aProjectionParameters; return *this;}
    CameraData&                 fov(float aFov) {mFov = aFov; return *this;}
    CameraData&                 aspectRatio(float aAspectRatio) {mAspectRatio = aAspectRatio; return *this;}
    
    const ci::mat4&             getViewMatrix() const {return mViewMatrix;}
    const ci::mat4&             getInversedViewMatrix() const {return mInversedViewMatrix;}
    const ci::mat4&             getProjectionMatrix() const {return mProjectionMatrix;}
    const ci::mat4&             getInversedProjectionMatrix() const {return mInversedProjectionMatrix;}
    const ci::vec3&             getPosition() const {return mPosition;}
    const ci::vec3&             getViewDirection() const {return mViewDirection;}
    const ci::vec2&             getProjectionParameters() const {return mProjectionParameters;}
    float                       getFov() const {return mFov;}
    float                       getAspectRatio() const {return mAspectRatio;}
    
    void                        setViewMatrix(const ci::mat4& aViewMatrix) {mViewMatrix = aViewMatrix;}
    void                        setInversedViewMatrix(const ci::mat4& aInversedViewMatrix) {mInversedViewMatrix = aInversedViewMatrix;}
    void                        setProjectionMatrix(const ci::mat4& aProjectionMatrix) {mProjectionMatrix = aProjectionMatrix;}
    void                        setInversedProjectionMatrix(const ci::mat4& aInversedProjectionMatrix) {mInversedProjectionMatrix = aInversedProjectionMatrix;}
    void                        setPosition(const ci::vec3& aPosition) {mPosition = aPosition;}
    void                        setViewDirection(const ci::vec3& aViewDirection) {mViewDirection = aViewDirection;}
    void                        setProjectionParameters(const ci::vec2& aProjectionParameters) {mProjectionParameters = aProjectionParameters;}
    void                        setFov(float aFov) {mFov = aFov;}
    void                        setAspectRatio(float aAspectRatio) {mAspectRatio = aAspectRatio;}
    
public:
    //ALIGNMENT
    ci::mat4                    mViewMatrix;
    //ALIGNMENT
    ci::mat4                    mInversedViewMatrix;
    //ALIGNMENT
    ci::mat4                    mProjectionMatrix;
    //ALIGNMENT
    ci::mat4                    mInversedProjectionMatrix;
    //ALIGNMENT
    ci::vec3                    mPosition;
    uint32_t                    mPad0;
    //ALIGNMENT
    ci::vec3                    mViewDirection;
    uint32_t                    mPad1;
    //ALIGNMENT
    ci::vec2                    mProjectionParameters;
    float                       mFov;
    float                       mAspectRatio;
    //ALIGNMENT
    
};