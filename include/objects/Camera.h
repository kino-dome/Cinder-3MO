//
//  ThreeCamera.h
//  iPhotoWall
//
//  Created by Hesam Ohadi on 16/11/15.
//
//
#pragma once

#include "cinder/Camera.h"
#include "cinder/CameraUi.h"

#include "../core/Object3D.h"
#include "../core/CameraData.h"

typedef std::shared_ptr<class CameraObject>             CameraObjectRef;
typedef std::shared_ptr<class PerspectiveCamera>        PerspectiveCameraRef;
typedef std::shared_ptr<class OrthographicCamera>       OrthographicCameraRef;
typedef std::shared_ptr<class StereoCamera>             StereoCameraRef;

class CameraObject : public virtual Object3D
{
    
public:
    CameraObject();
    virtual ~CameraObject(){};
    virtual ci::Camera& getCinderCamera() =0;
    
    void                    onUpdate() override; // whenever the CameraObject is updated i.e the ModelWorld was updated, the cinderCamera should update based on that
    const CameraData&       getCameraData() const;
    void                    updateCameraData();
    void                    cinderCameraChanged(); //for the sake of the time when the embeded CinderCamera has changed, like CameraUi for instance
    void                    setCameraUi(ci::CameraUi& aCameraUi);
    
public:
    CameraData              mCameraData;
};

class PerspectiveCamera : public virtual CameraObject
{
public:
    static PerspectiveCameraRef create(){
        return PerspectiveCameraRef (new PerspectiveCamera());
    }
    
    PerspectiveCamera();
    ~PerspectiveCamera() override;
    OBJECT3D_CLONE(PerspectiveCamera)
    
    void                        drawSelf() override;
    
    //setters/getters
    void                        setCinderCamera(const ci::CameraPersp& aCinderCamera);
    ci::CameraPersp&            getCinderCamera() override;
    
    //ci::CameraPersp method
    void                        setPerspective(float aVerticalFovDegrees, float aAspectRatio, float aNearPlane, float aFarPlane );
    
public:
    ci::CameraPersp             mCinderCamera;
};

class OrthographicCamera : public virtual CameraObject
{
public:
    static OrthographicCameraRef create(){
        return OrthographicCameraRef (new OrthographicCamera());
    }
    
    OrthographicCamera();
    ~OrthographicCamera() override;
    OBJECT3D_CLONE(OrthographicCamera);
    
    void                        drawSelf() override;
    
    //setters/getters
    void                        setCinderCamera(const ci::CameraOrtho& aCinderCamera);
    ci::CameraOrtho&            getCinderCamera()override;
    
    //ci::CameraOrtho method
    void                        setOrtho(float aLeft, float aRight, float aBottom, float aTop, float aNearPlane, float aFarPlane );
    
public:
    ci::CameraOrtho             mCinderCamera;
};

class StereoCamera : public virtual CameraObject
{
public:
    static StereoCameraRef create(){
        return StereoCameraRef (new StereoCamera());
    }
    
    StereoCamera();
    ~StereoCamera() override;
    OBJECT3D_CLONE(StereoCamera);
    
    void                        drawSelf() override;
    
    //setters/getters
    void                        setCinderCamera(const ci::CameraStereo& aCinderCamera);
    ci::CameraStereo&           getCinderCamera() override;
    
public:
    ci::CameraStereo            mCinderCamera;
};

