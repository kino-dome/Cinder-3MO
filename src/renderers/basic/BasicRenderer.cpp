//
//  BasicRenderer.cpp
//  iPhotoWall
//
//  Created by Hesam Ohadi on 16/11/15.
//
//

#include "BasicRenderer.h"


BasicRenderer::BasicRenderer() : RendererBase()
{
    mType = "basic_renderer";
}

BasicRenderer::~BasicRenderer()
{
    mResizeConnection.disconnect();
}

void BasicRenderer::setup()
{
    RendererBase::setup();
    
    ci::gl::enableVerticalSync();
    ci::gl::color( ci::ColorAf::white() );
    ci::gl::disableAlphaBlending();
    
    // Call resize to create FBOs
    resize();
    
    mResizeConnection = mParentWindow->getSignalResize().connect(std::bind(&BasicRenderer::resize, this));
}

void BasicRenderer::setup(const ci::app::WindowRef& aParentWindow)
{
    RendererBase::setup(aParentWindow);
    setup();
}

void BasicRenderer::resize()
{
    
}

void BasicRenderer::update()
{
    
}

void BasicRenderer::draw(const SceneRef& aScene)
{
    if (!aScene->getActiveCamera()){
        auto perspectiveCameras = aScene->getPerspectiveCameras();
        auto iter = perspectiveCameras.begin();
        aScene->setActiveCamera(*iter);
    }
    auto activeCamera = aScene->getActiveCamera();
    if ( activeCamera->hasTag("perspective_camera")){
        draw(aScene, activeCamera->cast<PerspectiveCamera>());
    }
    else if (activeCamera->hasTag("orthographic_camera")){
        draw(aScene, activeCamera->cast<OrthographicCamera>());
    }
    else if (activeCamera->hasTag("stereo_camera")){
        draw(aScene, activeCamera->cast<StereoCamera>());
    }
    
    
}

void BasicRenderer::draw(const SceneRef &aScene, const PerspectiveCameraRef &aPerspectiveCamera)
{
    {
        //const ci::gl::ScopedFramebuffer scopedFrameBuffer( mFboCanvas );
        const ci::gl::ScopedViewport scopedViewport( ci::ivec2( 0 ), mParentWindow->getBounds().getSize() );
        const ci::gl::ScopedMatrices scopedMatrices;
        ci::gl::enableDepthRead();
        ci::gl::enableDepthWrite();
        ci::gl::setMatrices(aPerspectiveCamera->getCinderCamera());
        //ci::gl::clear(mClearColor);
        //ci::gl::drawSphere(ci::vec3(0.0, 0.0, 0.0), 10.0);
        aScene->draw();
        
        
    }
    
}

void BasicRenderer::draw(const SceneRef &aScene, const OrthographicCameraRef &aOrthographicCamera)
{
    {
        //const ci::gl::ScopedFramebuffer scopedFrameBuffer( mFboCanvas );
        const ci::gl::ScopedViewport scopedViewport( ci::ivec2( 0 ), mParentWindow->getBounds().getSize() );
        const ci::gl::ScopedMatrices scopedMatrices;
        ci::gl::enableDepthRead();
        ci::gl::enableDepthWrite();
        ci::gl::setMatrices(aOrthographicCamera->getCinderCamera());
        ci::gl::clear(mClearColor);
        aScene->draw();
        
        
    }
}

void BasicRenderer::draw(const SceneRef &aScene, const StereoCameraRef &aStereoCamera)
{
    {
        //const ci::gl::ScopedFramebuffer scopedFrameBuffer( mFboCanvas );
        const ci::gl::ScopedViewport scopedViewport( ci::ivec2( 0 ), mParentWindow->getBounds().getSize() );
        const ci::gl::ScopedMatrices scopedMatrices;
        ci::gl::enableDepthRead();
        ci::gl::enableDepthWrite();
        ci::gl::setMatrices(aStereoCamera->getCinderCamera());
        ci::gl::clear(mClearColor);
        aScene->draw();
        
        
    }
}

