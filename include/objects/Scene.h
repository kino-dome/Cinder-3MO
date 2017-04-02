//
//  Scene.h
//  iPhotoWall
//
//  Created by Hesam Ohadi on 16/11/15.
//
//
#pragma once

#include "../core/Object3D.h"
#include "../core/MaterialPallet.h"
#include "../objects/Light.h"
#include "../objects/Camera.h"
#include "../objects/Instancer.h"
#include "../extra/QuadInstancer.h"


typedef std::shared_ptr<class Scene> SceneRef;
typedef std::shared_ptr<class RendererBase> RendererBaseRef;

class Scene : public virtual Object3D
{
    friend class Object3D;
    
public:
    static SceneRef create(){
        return SceneRef ( new Scene());
    }
    
    Scene();
    ~Scene();
    
    void                                                    drawSelf() override;
    void                                                    updateSelf() override;
    
    void                                                    addToMaterialPallet(const MaterialRef& aMaterial);
    void                                                    removeFromMaterialPallet(const MaterialRef& aMaterial);
    
    void                                                    notifyParentRendererAdded(const Object3DRef& aAddedObject);
    void                                                    notifyParentRendererRemoved(const Object3DRef& aRemovedObject);
    
    //methods for parent renderer (if necessary like the deferred renderer)
    
    void                                                    setParentRenderer(const RendererBaseRef& aParentRenderer);
    const RendererBaseRef&                                  getParentRenderer() const;
    
    //active camera
    void                                                    setActiveCamera(const CameraObjectRef& aActiveCamera);
    const CameraObjectRef&                                  getActiveCamera() const;
    
    const std::list<DirectionalLightRef>&                   getDirectionalLights() const {return mDirectionalLights;}
    const std::list<PointLightRef>&                         getPointLights() const {return mPointLights;}
    const std::list<SpotLightRef>&                          getSpotLights() const {return mSpotLights;}
    const std::list<PerspectiveCameraRef>&                  getPerspectiveCameras() const {return mPerspectiveCameras;}
    const std::list<OrthographicCameraRef>&                 getOrthographicCameras() const {return mOrthographicCameras;}
    const std::list<StereoCameraRef>&                       getStereoCameras() const {return mStereoCameras;}
    const ClassicMaterialPallet&                            getClassicMaterialPallet() const {return mClassicMaterialPallet;}
    const PbrMaterialPallet&                                getPbrMaterialPallet() const {return mPbrMaterialPallet;}
    const std::list<MeshRef>&                               getMeshes() const {return mMeshes;}
    const std::list<InstancerRef>&                          getInstancers() const {return mInstancers;}
    const std::list<QuadInstancerRef>&                      getQuadInstancers() const {return mQuadInstancers;}
    
    //setters/getters
    void                                                    setMaxNumDirectionalLights(int aNum){mMaxNumDirectionalLights=aNum;}
    void                                                    setMaxNumPointLights(int aNum){mMaxNumPointLights=aNum;}
    void                                                    setMaxNumSpotLights(int aNum){mMaxNumSpotLights=aNum;}
    void                                                    setMaxNumClassicMaterials(int aNum){mMaxNumClassicMaterials=aNum;}
    void                                                    setMaxNumPbrMaterials(int aNum){mMaxNumPbrMaterials=aNum;}
    void                                                    setShouldCheckChildrenNames(bool aShould = true) {mShouldCheckChildrenNames = aShould;}
    int                                                     getMaxNumDirectionalLights() const{return mMaxNumDirectionalLights;}
    int                                                     getMaxNumPointLights() const{return mMaxNumPointLights;}
    int                                                     getMaxNumSpotLights() const{return mMaxNumSpotLights;}
    int                                                     getMaxNumClassicMaterials() const{return mMaxNumClassicMaterials;}
    int                                                     getMaxNumPbrMaterials() const {return mMaxNumPbrMaterials;}
    int                                                     getNumDirectionalLights() const {return mNumDirectionalLights;}
    int                                                     getNumPointLights() const {return mNumPointLights;}
    int                                                     getNumSpotLights() const {return mNumSpotLights;}
    int                                                     getNumClassicMaterials() const{return mNumClassicMaterials;}
    int                                                     getNumPbrMaterials() const {return mNumPbrMaterials;}
    bool                                                    getShouldCheckChildrenNames() const {return mShouldCheckChildrenNames;}
    
protected:
    void                                                    onNotifyAdded(const Object3DRef& aAddedObject);
    void                                                    onNotifyRemoved(const Object3DRef& aRemovedObject);
    void                                                    sortMeshesByDepth();
    std::string                                             verifyName(const std::string& aName); // returns the verified name
    void                                                    addNameToMap(const std::string& aName);
    void                                                    removeNameFromMap(const std::string& aName);
    int                                                     deduceNameIndex(const std::string& aName); // this is for mNameMap where each name (ex. "mesh") automatically gets an id when not unique (ex. "mesh_2")->Houdini Style;
    std::string                                             deduceNameBase(const std::string& aName); // returns the name without index
    bool                                                    isNumber(const std::string& aString);
    
public:
    //a refrence to the renderer responsible
    RendererBaseRef                                         mParentRenderer;
    
    //numbers and max numbers of the scene contents
    int                                                     mMaxNumDirectionalLights, mMaxNumPointLights, mMaxNumSpotLights, mMaxNumPbrMaterials, mMaxNumClassicMaterials;
    int                                                     mNumPbrMaterials, mNumClassicMaterials, mNumPointLights, mNumSpotLights, mNumDirectionalLights, mNumMeshes, mNumInstancers, mNumQuadInstancers;
    
    //a refrence to all the lights in the scene
    std::list<DirectionalLightRef>                          mDirectionalLights;
    std::list<PointLightRef>                                mPointLights;
    std::list<SpotLightRef>                                 mSpotLights;
    std::list<PerspectiveCameraRef>                         mPerspectiveCameras;
    std::list<OrthographicCameraRef>                        mOrthographicCameras;
    std::list<StereoCameraRef>                              mStereoCameras;
    
    //active camera
    CameraObjectRef                                         mActiveCamera;
    
    //a refrence to the two material pallets in the scene
    ClassicMaterialPallet                                   mClassicMaterialPallet;
    PbrMaterialPallet                                       mPbrMaterialPallet;
    
    
    //a refrence to all the meshes in the scene
    std::list<MeshRef>                                      mMeshes;
    
    //a reference to all the instancers
    std::list<InstancerRef>                                 mInstancers;
    
    //a reference to all the quadInstancers
    std::list<QuadInstancerRef>                             mQuadInstancers;
    
    //makes children names unique by using index
    bool                                                    mShouldCheckChildrenNames;
    std::map<std::string, std::list<int>>                   mNameMap;
};