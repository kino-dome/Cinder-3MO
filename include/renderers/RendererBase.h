//
//  Renderer.h
//  DeferredRenderer
//
//  Created by Hesam Ohadi on 5/12/15.
//
//

#pragma once

#include "../objects/Scene.h"
#include "../objects/Camera.h"
#include "cinder/app/Window.h"

class RendererBase : public std::enable_shared_from_this<RendererBase>
{
public:
    RendererBase();
    virtual ~RendererBase()=0;
    
    virtual void                                            setup();
    virtual void                                            setup(const ci::app::WindowRef& aParentWindow);
    virtual void                                            update(){}
    virtual void                                            draw(){}
    virtual void                                            draw(const SceneRef& aScene){}
    virtual void                                            draw(const SceneRef& aScene, const PerspectiveCameraRef& aPerspectiveCamera){}
    virtual void                                            draw(const SceneRef& aScene, const OrthographicCameraRef& aOrthographicCamera){}
    virtual void                                            draw(const SceneRef& aScene, const StereoCameraRef& aStereoCamera){}
    virtual void                                            resize(){}
    
    //events to notify parent renderer (if necessary like the deferred renderer)
    virtual void                                            onNotifyAdded(const Object3DRef& aAddedObject){}
    virtual void                                            onNotifyRemoved(const Object3DRef& aRemovedObject){}
    
    // Setters/ Getters
    void                                                    setParentWindow( const ci::app::WindowRef& aParentWindow);
    void                                                    setType(const std::string& aType);
    void                                                    setClearColor(const ci::ColorAf aClearColor);
    const ci::app::WindowRef&                               getParentWindow() const;
    const std::string&                                      getType() const;
    const ci::ColorAf&                                      getClearColor() const;
    
    
public:
    ci::app::WindowRef                                      mParentWindow;
    std::string                                             mType;
    ci::ColorAf                                             mClearColor;
};