//
//  Scene.cpp
//  iPhotoWall
//
//  Created by Hesam Ohadi on 16/11/15.
//
//

#include "Scene.h"
#include "renderers/RendererBase.h"

using namespace ci;
using namespace std;

Scene::Scene() : Object3D()
{
    mType = "scene";
    addTag("scene");
    mMaxNumDirectionalLights = 11;
    mMaxNumPointLights = 11;
    mMaxNumSpotLights = 11;
    mMaxNumClassicMaterials = 25;
    mMaxNumPbrMaterials = 25;
    
    mNumClassicMaterials = 0;
    mNumPbrMaterials = 0;
    mNumDirectionalLights = 0;
    mNumPointLights = 0;
    mNumSpotLights = 0;
    mNumMeshes = 0;
    mNumInstancers = 0;
    mNumQuadInstancers = 0;
    mShouldCheckChildrenNames = true;
}

Scene::~Scene()
{
    
}

void Scene::drawSelf()
{
    
}

void Scene::updateSelf()
{
    
}

void Scene::addToMaterialPallet(const MaterialRef &aMaterial)
{
    if (aMaterial->getIsPbr()){
        mPbrMaterialPallet.addMaterial(static_pointer_cast<PbrMaterial>(aMaterial));
        mNumPbrMaterials++;
    }
    else {
        mClassicMaterialPallet.addMaterial(static_pointer_cast<ClassicMaterial>(aMaterial));
        mNumClassicMaterials++;
    }
}

void Scene::removeFromMaterialPallet(const MaterialRef &aMaterial)
{
    if (aMaterial->getIsPbr()){
        mPbrMaterialPallet.removeMaterial(static_pointer_cast<PbrMaterial>(aMaterial));
        mNumPbrMaterials--;
    }
    else {
        mClassicMaterialPallet.removeMaterial(static_pointer_cast<ClassicMaterial>(aMaterial));
        mNumClassicMaterials--;
    }
}

void Scene::setParentRenderer(const RendererBaseRef &aParentRenderer)
{
    mParentRenderer = aParentRenderer;
}

const RendererBaseRef& Scene::getParentRenderer() const
{
    return mParentRenderer;
}

void Scene::notifyParentRendererAdded(const Object3DRef &aAddedObject)
{
    if (mParentRenderer){
        mParentRenderer->onNotifyAdded(aAddedObject);
    }
}

void Scene::notifyParentRendererRemoved(const Object3DRef &aRemovedObject)
{
    if (mParentRenderer){
        mParentRenderer->onNotifyRemoved(aRemovedObject);
    }
}

void Scene::setActiveCamera(const CameraObjectRef &aActiveCamera)
{
    mActiveCamera = aActiveCamera;
}

const CameraObjectRef& Scene::getActiveCamera() const
{
    return mActiveCamera;
}

void Scene::sortMeshesByDepth()
{
    mMeshes.sort([](const MeshRef& a, const MeshRef& b) { return a->getModelWorld().getModelTranslation().z < b->getModelWorld().getModelTranslation().z; });
}

void Scene::onNotifyAdded(const Object3DRef &aAddedObject)
{
    for ( string addedObjectTag : aAddedObject->getTags()){
        
        if (addedObjectTag == "directional_light"){
            DirectionalLightRef dirLight = aAddedObject->cast<DirectionalLight>();
            mDirectionalLights.push_back(dirLight);
            mNumDirectionalLights++;
        }
        if (addedObjectTag == "point_light"){
            PointLightRef pointLight = aAddedObject->cast<PointLight>();
            mPointLights.push_back(pointLight);
            mNumPointLights++;
        }
        if (addedObjectTag == "spot_light"){
            SpotLightRef spotLight = aAddedObject->cast<SpotLight>();
            mSpotLights.push_back(spotLight);
            mNumSpotLights++;
        }
        if (addedObjectTag == "perspective_camera"){
            PerspectiveCameraRef perspCamera = aAddedObject->cast<PerspectiveCamera>();
            mPerspectiveCameras.push_back(perspCamera);
            if (!mActiveCamera) setActiveCamera(perspCamera);
        }
        if (addedObjectTag == "orthographic_camera"){
            OrthographicCameraRef orthoCamera = aAddedObject->cast<OrthographicCamera>();
            mOrthographicCameras.push_back(orthoCamera);
            if (!mActiveCamera) setActiveCamera(orthoCamera);
        }
        if (addedObjectTag == "stereo_camera"){
            StereoCameraRef stereoCamera = aAddedObject->cast<StereoCamera>();
            mStereoCameras.push_back(stereoCamera);
            if (!mActiveCamera) setActiveCamera(stereoCamera);
        }
        if (addedObjectTag == "mesh"){
            MeshRef mesh = aAddedObject->cast<Mesh>();
            mMeshes.push_back(mesh);
            sortMeshesByDepth();
            mNumMeshes++;
            notifyParentRendererAdded(mesh);
        }
        if (addedObjectTag == "instancer"){
            InstancerRef instancer = aAddedObject->cast<Instancer>();
            mInstancers.push_back(instancer);
            mNumInstancers++;
            notifyParentRendererAdded(instancer);
            
        }
        if (addedObjectTag == "quad_instancer"){
            QuadInstancerRef instancer = aAddedObject->cast<QuadInstancer>();
            mQuadInstancers.push_back(instancer);
            mNumQuadInstancers++;
            notifyParentRendererAdded(instancer);
            
        }
    }
}

void Scene::onNotifyRemoved(const Object3DRef &aRemovedObject)
{
    for (string removedObjectTag : aRemovedObject->getTags()){
        if (removedObjectTag == "directional_light"){
            for (auto iter= mDirectionalLights.begin(); iter!= mDirectionalLights.end(); ++iter){
                auto object = *iter;
                if (object->getGuid()==aRemovedObject->getGuid()){
                    object = nullptr;
                    iter = mDirectionalLights.erase(iter);
                    mNumDirectionalLights--;
                }
            }
        }
        if (removedObjectTag == "point_light"){
            for (auto iter= mPointLights.begin(); iter!= mPointLights.end(); ++iter){
                auto object = *iter;
                if (object->getGuid()==aRemovedObject->getGuid()){
                    object = nullptr;
                    iter = mPointLights.erase(iter);
                    mNumPointLights--;
                }
            }
        }
        if (removedObjectTag == "spot_light"){
            for (auto iter= mSpotLights.begin(); iter!= mSpotLights.end(); ++iter){
                auto object = *iter;
                if (object->getGuid()==aRemovedObject->getGuid()){
                    object = nullptr;
                    iter = mSpotLights.erase(iter);
                    mNumSpotLights--;
                }
            }
        }
        if (removedObjectTag == "perspective_camera"){
            for (auto iter= mPerspectiveCameras.begin(); iter!= mPerspectiveCameras.end(); ++iter){
                auto object = *iter;
                if (object->getGuid()==aRemovedObject->getGuid()){
                    object = nullptr;
                    iter = mPerspectiveCameras.erase(iter);
                }
            }
        }
        if (removedObjectTag == "orthographic_camera"){
            for (auto iter= mOrthographicCameras.begin(); iter!= mOrthographicCameras.end(); ++iter){
                auto object = *iter;
                if (object->getGuid()==aRemovedObject->getGuid()){
                    object = nullptr;
                    iter = mOrthographicCameras.erase(iter);
                }
            }
        }
        if (removedObjectTag == "stereo_camera"){
            for (auto iter= mStereoCameras.begin(); iter!= mStereoCameras.end(); ++iter){
                auto object = *iter;
                if (object->getGuid()==aRemovedObject->getGuid()){
                    object = nullptr;
                    iter = mStereoCameras.erase(iter);
                }
            }
        }
        if (removedObjectTag == "mesh"){
            for (auto iter= mMeshes.begin(); iter!= mMeshes.end(); ++iter){
                auto object = *iter;
                if (object->getGuid() == aRemovedObject->getGuid()){
                    object = nullptr;
                    iter = mMeshes.erase(iter);
                    mNumMeshes--;
                    notifyParentRendererRemoved(object);
                }
            }
        }
        if (removedObjectTag == "instancer"){
            for (auto iter= mInstancers.begin(); iter!= mInstancers.end(); ++iter){
                auto object = *iter;
                if (object->getGuid() == aRemovedObject->getGuid()){
                    object = nullptr;
                    iter = mInstancers.erase(iter);
                    mNumInstancers--;
                    notifyParentRendererRemoved(object);
                }
            }
        }
        if (removedObjectTag == "quad_instancer"){
            for (auto iter= mQuadInstancers.begin(); iter!= mQuadInstancers.end(); ++iter){
                auto object = *iter;
                if (object->getGuid() == aRemovedObject->getGuid()){
                    object = nullptr;
                    iter = mQuadInstancers.erase(iter);
                    mNumQuadInstancers--;
                    notifyParentRendererRemoved(object);
                }
            }
        }
    }
}

string Scene::verifyName(const string& aName)
{
    int index = deduceNameIndex(aName);
    app::console()<<"deduced Index: "<<index<<endl;
    if (index > -1) {
        string nameKey = deduceNameBase(aName);
        if (mNameMap.find(nameKey) != mNameMap.end()){
            int verifiedIndex;
            if (std::find(mNameMap[nameKey].begin(), mNameMap[nameKey].end(), index) != mNameMap[nameKey].end()){
                verifiedIndex = index;
            }
            else {
            for (auto iter = mNameMap[nameKey].begin(); iter != mNameMap[nameKey].end(); ++iter){
                if (std::next(iter, 1) == mNameMap[nameKey].end()) verifiedIndex = (*iter) + 1;
                else if ( (*iter)+1 < (*std::next(iter, 1)) ) verifiedIndex = (*iter) + 1;
            }
            }
            return nameKey +"_"+ std::to_string(verifiedIndex);
        }
        else {
            return aName;
        }
    }
    else {
        if (mNameMap.find(aName) != mNameMap.end()){
            int verifiedIndex;
            for (auto iter = mNameMap[aName].begin(); iter != mNameMap[aName].end(); ++iter){
                if (std::next(iter, 1) == mNameMap[aName].end()) verifiedIndex = (*iter) + 1;
                else if ( (*iter)+1 < (*std::next(iter, 1)) ) verifiedIndex = (*iter) + 1;
            }
            return aName+"_"+ std::to_string(verifiedIndex);
        }
        else {
            return aName;
        }
    }
}

void Scene::addNameToMap(const string &aName)
{
    int index = deduceNameIndex(aName);
    if (index > -1) {
        string nameKey = deduceNameBase(aName);
        mNameMap[nameKey].push_back(index);
        mNameMap[nameKey].sort();
    }
    else {
        mNameMap[aName].push_back(-1); //first entry in the list
    }
}

void Scene::removeNameFromMap(const string &aName)
{
    int index = deduceNameIndex(aName);
    if (index > -1) {
        string nameKey = deduceNameBase(aName);
        mNameMap[nameKey].erase(std::find(mNameMap[nameKey].begin(), mNameMap[nameKey].end(), index));
    }
    else {
        mNameMap.erase(aName);
    }
}

int Scene::deduceNameIndex(const string &aName)
{
    auto charIndex = aName.find_last_of('_');
    if (charIndex == string::npos) return -1;
    else {
        string indexString = aName.substr(charIndex, aName.length()-charIndex + 1);
        if (isNumber(indexString)){
            return stoi(indexString);
        }
        else{
            return -1;
        }
    }
}

string Scene::deduceNameBase(const string &aName)
{
    auto charIndex = aName.find_last_of('_');
    if (charIndex == string::npos) return aName;
    else {
        string indexString = aName.substr(charIndex, aName.length()-charIndex + 1);
        if (isNumber(indexString)){
            return aName.substr(0, charIndex);
        }
        else{
            return aName;
        }
    }
}

bool Scene::isNumber(const string& aString)
{
    return !aString.empty() && std::find_if(aString.begin(),
                                      aString.end(), [](char c) { return !std::isdigit(c); }) == aString.end();
}