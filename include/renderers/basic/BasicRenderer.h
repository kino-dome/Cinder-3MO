//
//  Renderer.h
//  iPhotoWall
//
//  Created by Hesam Ohadi on 16/11/15.
//
//
#pragma once

#include "cinder/app/Window.h"

#include "../../objects/Scene.h"
#include "../RendererBase.h"

typedef std::shared_ptr<class BasicRenderer> BasicRendererRef;

class BasicRenderer : public RendererBase
{
public:
    static BasicRendererRef create(){
        return BasicRendererRef ( new BasicRenderer());
    }
    
    BasicRenderer();
    virtual ~BasicRenderer();
    
    void                                            setup() override;
    void                                            setup(const ci::app::WindowRef& aParentWindow) override;
    void                                            update() override;
    void                                            draw(const SceneRef& aScene) override;
    void                                            draw(const SceneRef& aScene, const PerspectiveCameraRef& aPerspectiveCamera) override;
    void                                            draw(const SceneRef& aScene, const OrthographicCameraRef& aOrthographicCamera) override;
    void                                            draw(const SceneRef& aScene, const StereoCameraRef& aStereoCamera) override;
    void                                            resize() override;
    
    // Setters/ Getters

    
public:
    
    ci::signals::Connection                         mResizeConnection;
};