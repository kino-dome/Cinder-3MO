//
//  DeferredRenderer.h
//  DeferredRenderer
//
//  Created by Hesam Ohadi on 5/12/15.
//
//

#pragma once

#include "cinder/Log.h"
#include "cinder/app/Window.h"
#include "cinder/gl/Batch.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/Ubo.h"

#include "../../objects/Scene.h"
#include "../RendererBase.h"


typedef std::shared_ptr<class DeferredRenderer> DeferredRendererRef;

class SceneData;

class DeferredRenderer : public RendererBase
{
protected:
    enum : int32_t
    {
        Ao_None,
        Ao_Hbao,
        Ao_Sao
    } typedef Ao;
    
public:
    static DeferredRendererRef create(){
        return DeferredRendererRef ( new DeferredRenderer());
    }
    
    DeferredRenderer();
    ~DeferredRenderer();
    
    void                                            setup() override;
    void                                            setup(const ci::app::WindowRef& aParentWindow) override;
    void                                            update() override;
    void                                            draw() override;
    void                                            draw(const SceneRef& aScene) override;
    void                                            draw(const SceneRef& aScene, const PerspectiveCameraRef& aPerspectiveCamera) override;
    void                                            draw(const SceneRef& aScene, const OrthographicCameraRef& aOrthographicCamera) override;
    void                                            draw(const SceneRef& aScene, const StereoCameraRef& aStereoCamera) override;
    void                                            resize() override;
    
    void                                            onNotifyAdded(const Object3DRef& aAddedObject) override;
    void                                            onNotifyRemoved(const Object3DRef& aRemovedObject) override;
    
    void                                            addScene(const SceneRef& aScene);
    void                                            removeScene(const SceneRef& aScene);
    void                                            bindScene(const SceneRef& aScene);
    void                                            unbindScene(const SceneRef& aScene);
    void                                            setActiveScene(const SceneRef& aActiveScene);
    const SceneRef&                                 getActiveScene() const;
    
    //methods for setting up the renderer
    void                                            setupShaders();
    void                                            setupUniforms();
    void                                            setupBatches();
    void                                            setupGBufferLayout();
    
    // Setters/ Getters
    DeferredRenderer&                               isDebugEnabled(bool aIsDebugEnabled);
    bool                                            getIsDebugEnabled() const;
    void                                            setIsDebugEnabled(bool aIsDebugEnabled);
    DeferredRenderer&                               isFxaaEnabled(bool aIsFxaaEnabled);
    bool                                            getIsFxaaEnabled() const;
    void                                            setIsFxaaEnabled(bool aIsFxaaEnabled);

    
    // gbuffer attachment enablers
    DeferredRenderer&                               isDepthEnabled(bool aIsDepthEnabled);
    DeferredRenderer&                               isMaterialIdEnabled(bool aIsMaterialIdEnabled);
    DeferredRenderer&                               isAlbedoEnabled(bool aIsAlbedoEnabled);
    DeferredRenderer&                               isNormalEnabled(bool aIsNormalEnabled);
    DeferredRenderer&                               isPositionEnabled(bool aIsPositionEnabled);
    DeferredRenderer&                               isTexCoordEnabled(bool aIsTexCoordEnabled);
    DeferredRenderer&                               isSpecularEnabled(bool aIsSpecularEnabled);
    DeferredRenderer&                               isEmissionEnabled(bool aIsEmissionEnabled);
    DeferredRenderer&                               isOcclusionEnabled(bool aIsOcclusionEnabled);
    bool                                            getIsDepthEnabled() const;
    bool                                            getIsMaterialIdEnabled() const;
    bool                                            getIsAlbedoEnabled() const;
    bool                                            getIsNormalEnabled() const;
    bool                                            getIsPositionEnabled() const;
    bool                                            getIsTexCoordEnabled() const;
    bool                                            getIsSpecularEnabled() const;
    bool                                            getIsEmissionEnabled() const;
    bool                                            getIsOcclusionEnabled() const;
    void                                            setIsDepthEnabled(bool aIsDepthEnabled);
    void                                            setIsMaterialIdEnabled(bool aIsMaterialIdEnabled);
    void                                            setIsAlbedoEnabled(bool aIsAlbedoEnabled);
    void                                            setIsNormalEnabled(bool aIsNormalEnabled);
    void                                            setIsPositionEnabled(bool aIsPositionEnabled);
    void                                            setIsTexCoordEnabled(bool aIsTexCoordEnabled);
    void                                            setIsSpecularEnabled(bool aIsSpecularEnabled);
    void                                            setIsEmissionEnabled(bool aIsEmissionEnabled);
    void                                            setIsOcclusionEnabled(bool aIsOcclusionEnabled);
    
    //setters/getters for gbuffer settings
    void                                            setAlbedoLayoutIndex(int aIndex) {mAlbedoLayoutIndex=aIndex;}
    void                                            setNormalLayoutIndex(int aIndex) {mNormalLayoutIndex=aIndex;}
    void                                            setPositionLayoutIndex(int aIndex) {mPositionLayoutIndex=aIndex;}
    void                                            setMaterialIdLayoutIndex(int aIndex) {mMaterialIdLayoutIndex=aIndex;}
    void                                            setTexCoordLayoutIndex(int aIndex) {mTexCoordLayoutIndex=aIndex;}
    void                                            setSpecularLayoutIndex(int aIndex) {mSpecularLayoutIndex=aIndex;}
    void                                            setEmissionLayoutIndex(int aIndex) {mEmissionLayoutIndex=aIndex;}
    void                                            setOcclusionLayoutIndex(int aIndex) {mOcclusionLayoutIndex=aIndex;}
    int                                             getAlbedoLayoutIndex() const {return mAlbedoLayoutIndex;}
    int                                             getNormalLayoutIndex() const {return mNormalLayoutIndex;}
    int                                             getPositionLayoutIndex() const {return mPositionLayoutIndex;}
    int                                             getMaterialIdLayoutIndex() const {return mMaterialIdLayoutIndex;}
    int                                             getTexCoordLayoutIndex() const {return mTexCoordLayoutIndex;}
    int                                             getSpecularLayoutIndex() const {return mSpecularLayoutIndex;}
    int                                             getEmissionLayoutIndex() const {return mEmissionLayoutIndex;}
    int                                             getOcclusionLayoutIndex() const {return mOcclusionLayoutIndex;}
    
public:
    
    ci::signals::Connection                         mResizeConnection;
    
    //a buffer of sceneData infos
    std::list<SceneData>                            mSceneDataList;
    
    //active Scene
    SceneRef                                        mActiveScene;
    
    //Renderer settings
    bool                                            mIsDebugEnabled;
    bool                                            mIsFxaaEnabled;
    bool                                            mIsAoEnabled;
    
    //bools to see how big and complete will the GBuffer be
    bool                                            mIsDepthEnabled; //default to true
    bool                                            mIsMaterialIdEnabled; //default to true
    bool                                            mIsAlbedoEnabled; //default to true - will include diffuse map
    bool                                            mIsNormalEnabled; //default to true - will include normal map
    bool                                            mIsPositionEnabled; //default to false - will include displacement map
    bool                                            mIsTexCoordEnabled; //default to false
    bool                                            mIsSpecularEnabled; //default to false - will include specular map
    bool                                            mIsEmissionEnabled; //default to false - will include emission map
    bool                                            mIsOcclusionEnabled; //default to false - will include occlusion map
    int                                             mDepthLayoutIndex, mAlbedoLayoutIndex, mNormalLayoutIndex, mPositionLayoutIndex, mMaterialIdLayoutIndex, mTexCoordLayoutIndex, mSpecularLayoutIndex, mEmissionLayoutIndex, mOcclusionLayoutIndex;
    
    //Ao
    int32_t                                         mAo;
    int32_t                                         mAoPrev;
    
    //Fbos
    ci::gl::FboRef                                  mFboAo;
    ci::gl::FboRef                                  mFboAccum;
    ci::gl::FboRef                                  mFboCsz;
    ci::gl::FboRef                                  mFboGBuffer;
    ci::gl::FboRef                                  mFboPingPong;
    ci::gl::FboRef                                  mFboRayColor;
    ci::gl::FboRef                                  mFboRayDepth;
    ci::gl::FboRef                                  mFboShadowMap;
    
    //textures
    std::vector<ci::gl::Texture2dRef>               mTextureFboGBufferVector;
    std::vector<ci::gl::Texture2d::Format>          mTextureFormatFboGBufferVector;
    ci::gl::Texture2dRef                            mTextureFboPingPong[ 2 ];
    ci::gl::Texture2dRef                            mTextureFboAo[ 2 ];
    ci::gl::Texture2dRef                            mTextureFboAccum[ 3 ];

    
    //Batches
    ci::gl::BatchRef                                mBatchDebugRect;
    ci::gl::BatchRef                                mBatchEmissionRect;
    ci::gl::BatchRef                                mBatchLBufferShadowRect;
    ci::gl::BatchRef                                mBatchShadowMapIcosahedron;
    ci::gl::BatchRef                                mBatchShadowMapSphere;
    
    ci::gl::BatchRef                                mBatchAoCompositeRect;
    ci::gl::BatchRef                                mBatchHbaoAoRect;
    ci::gl::BatchRef                                mBatchHbaoBlurRect;
    ci::gl::BatchRef                                mBatchSaoAoRect;
    ci::gl::BatchRef                                mBatchSaoBlurRect;
    ci::gl::BatchRef                                mBatchSaoCszRect;
    
    ci::gl::BatchRef                                mBatchBloomBlurRect;
    ci::gl::BatchRef                                mBatchBloomCompositeRect;
    ci::gl::BatchRef                                mBatchBloomHighpassRect;
    
    ci::gl::BatchRef                                mBatchColorRect;
    ci::gl::BatchRef                                mBatchDofRect;
    ci::gl::BatchRef                                mBatchFogRect;
    ci::gl::BatchRef                                mBatchFxaaRect;
    
    ci::gl::BatchRef                                mBatchRayCompositeRect;
    ci::gl::BatchRef                                mBatchRayOccludeRect;
    ci::gl::BatchRef                                mBatchRayScatterRect;
    
    ci::gl::BatchRef                                mBatchStockColorRect;
    ci::gl::BatchRef                                mBatchStockColorSphere;
    ci::gl::BatchRef                                mBatchStockTextureRect;
    
};

