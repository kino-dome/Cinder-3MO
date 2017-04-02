//
//  ThreeCamera.cpp
//  iPhotoWall
//
//  Created by Hesam Ohadi on 16/11/15.
//
//

#include "Camera.h"

///////////////////////CameraObject///////////////////////////

CameraObject::CameraObject() : Object3D()
{
    mType = "camera";
    mIsVisible = false;
    mShouldNotifyTopParent = true;
    addTag("camera");
    mName = "camera";
}

//being OCD on doing this ONLY if necessry (mModelWorld was updated)
void CameraObject::onUpdate()
{
    auto& cinderCamera = getCinderCamera();
    cinderCamera.setEyePoint(mModelWorld.getModelTranslation());
    cinderCamera.setWorldUp(mWorldUp);
    cinderCamera.setViewDirection(mViewDirection);
    cinderCamera.setOrientation(mModelWorld.getModelOrientation());
    updateCameraData();
}

void CameraObject::cinderCameraChanged()
{
    auto& cinderCamera = getCinderCamera();
    translate(cinderCamera.getEyePoint()-mModelWorld.getModelTranslation());
    mModel.setModelOrientation(cinderCamera.getOrientation());
    setViewDirection(cinderCamera.getViewDirection());
    setWorldUp(cinderCamera.getWorldUp());
    updateCameraData();
    setShouldUpdate(false);
}

void CameraObject::setCameraUi(ci::CameraUi &aCameraUi)
{
    aCameraUi.getSignalCameraChange().connect(std::bind(&CameraObject::cinderCameraChanged,this));
}

const CameraData& CameraObject::getCameraData() const
{
    return mCameraData;
}

void CameraObject::updateCameraData()
{
    auto& cinderCamera = getCinderCamera();
    const float f					= cinderCamera.getFarClip();
    const float n					= cinderCamera.getNearClip();
    const ci::vec2 projectionParams		= ci::vec2( f / ( f - n ), ( -f * n ) / ( f - n ) );
    mCameraData.viewMatrix(cinderCamera.getViewMatrix()).inversedViewMatrix(cinderCamera.getInverseViewMatrix()).projectionMatrix(cinderCamera.getProjectionMatrix()).inversedProjectionMatrix(glm::inverse( cinderCamera.getProjectionMatrix())).position(cinderCamera.getEyePoint()).viewDirection(cinderCamera.getViewDirection()).projectionParameters(projectionParams).fov(cinderCamera.getFov()).aspectRatio(cinderCamera.getAspectRatio());
}

///////////////////////PerspectiveCamera///////////////////////////

PerspectiveCamera::PerspectiveCamera() : CameraObject()
{
    mType = "perspective_camera";
    mCinderCamera = ci::CameraPersp();
    addTag("perspective_camera");
    mName = "perspective_camera";
}

PerspectiveCamera::~PerspectiveCamera()
{
    
}

void PerspectiveCamera::drawSelf()
{
    
}

//setters/getters

void PerspectiveCamera::setCinderCamera(const ci::CameraPersp &aCinderCamera)
{
    mCinderCamera = aCinderCamera;
}

ci::CameraPersp& PerspectiveCamera::getCinderCamera()
{
    return mCinderCamera;
}


void PerspectiveCamera::setPerspective(float aVerticalFovDegrees, float aAspectRatio, float aNearPlane, float aFarPlane)
{
    getCinderCamera().setPerspective(aVerticalFovDegrees, aAspectRatio, aNearPlane, aFarPlane);
}


///////////////////////OrthographicCamera///////////////////////////

OrthographicCamera::OrthographicCamera() : CameraObject()
{
    mType = "orthographic_camera";
    mCinderCamera = ci::CameraOrtho();
    addTag("orthographic_camera");
    mName = "orthographic_camera";
}

OrthographicCamera::~OrthographicCamera()
{
    
}

void OrthographicCamera::drawSelf()
{
    
}

//setters/getters

void OrthographicCamera::setCinderCamera(const ci::CameraOrtho &aCinderCamera)
{
    mCinderCamera = aCinderCamera;
}

ci::CameraOrtho& OrthographicCamera::getCinderCamera()
{
    return mCinderCamera;
}


void OrthographicCamera::setOrtho(float aLeft, float aRight, float aBottom, float aTop, float aNearPlane, float aFarPlane)
{
    getCinderCamera().setOrtho(aLeft, aRight, aBottom, aTop, aNearPlane, aFarPlane);
}


///////////////////////StereoCamera///////////////////////////

StereoCamera::StereoCamera() : CameraObject()
{
    mType = "stereo_camera";
    mCinderCamera = ci::CameraStereo();
    addTag("stereo_camera");
    mName = "stereo_camera";
}

StereoCamera::~StereoCamera()
{
    
}

void StereoCamera::drawSelf()
{
    
}

//setters/getters

void StereoCamera::setCinderCamera(const ci::CameraStereo &aCinderCamera)
{
    mCinderCamera = aCinderCamera;
}

ci::CameraStereo& StereoCamera::getCinderCamera()
{
    return mCinderCamera;
}