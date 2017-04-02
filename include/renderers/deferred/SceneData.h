//
//  SceneData.h
//  DeferredRenderer
//
//  Created by Hesam Ohadi on 7/12/15.
//
//

//Keeps a copy/manipulated data of the scene for renderer's sake

#include "../../objects/Scene.h"
#include "../deferred/Materials.h"
#include "../../extra/QuadInstancer.h"

class SceneData
{
public:
    
    SceneData(const SceneRef& aScene);
    ~SceneData();
    
    void                                            onNotifyAdded(const Object3DRef& aAddedObject);
    void                                            onNotifyRemoved(const Object3DRef& aRemovedObject);
    
    void                                            bind();
    void                                            unbind();
    void                                            update();
    void                                            drawGBuffer();
    void                                            drawLBuffer(const ci::ivec2& aSize);
    void                                            updateCameraUbo();
    void                                            updateLightUbos();
    void                                            updateMaterialUbos();
    void                                            setScene(const SceneRef& aScene);
    const SceneRef&                                 getScene() const;
    const std::map<MeshRef, MeshRef>&               getMeshes() const;
    bool                                            getIsBinded() const;
    
public:
    // a refrence to the scene meshes but with the gbuffer shader attatched, this is strictly for optimization reasons
    // if we know what objects we should render in the gbuffer we can have batches ready for them
    std::map<MeshRef, MeshRef>                      mMeshes;
    std::map<InstancerRef, MeshRef>                 mInstancers;
    std::map<QuadInstancerRef, MeshRef>             mQuadInstancers;
    ci::gl::BatchRef                                mBatchLBufferDirectionalLight;
    ci::gl::BatchRef                                mBatchLBufferPointLight;
    ci::gl::BatchRef                                mBatchLBufferSpotLight;
    SceneRef                                        mScene;
    MaterialGBufferRef                              mGBufferMaterial;
    MaterialGBufferInstancerRef                     mGBufferInstancerMaterial;
    MaterialGBufferQuadInstancerRef                 mGBufferQuadInstancerMaterial;
    bool                                            mIsBinded;
    ci::gl::UboRef                                  mUboClassicMaterials, mUboPbrMaterials;
    ci::gl::UboRef                                  mUboDirectionalLights, mUboPointLights, mUboSpotLights;
    ci::gl::UboRef                                  mUboActiveCamera;
    int                                             mDirectionalLightsUboIndex, mPointLightsUboIndex, mSpotLightsUboIndex, mClassicMaterialsUboIndex, mPbrMaterialsUboIndex, mActiveCameraUboIndex;
};

