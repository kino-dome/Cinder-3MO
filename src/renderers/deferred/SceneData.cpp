//
//  SceneData.cpp
//  DeferredRenderer
//
//  Created by Hesam Ohadi on 7/12/15.
//
//
#include "cinder/Utilities.h"

#include "SceneData.h"
#include "DeferredRenderer.h"

using namespace ci;
using namespace std;
using namespace ci::app;

//////////////////SceneData///////////////////////
SceneData::SceneData(const SceneRef& aScene)
{
    mIsBinded = false;
    mDirectionalLightsUboIndex=0;
    mPointLightsUboIndex=1;
    mSpotLightsUboIndex=2;
    mClassicMaterialsUboIndex=3;
    mPbrMaterialsUboIndex=4;
    mActiveCameraUboIndex=5;
    setScene(aScene);
}

SceneData::~SceneData()
{
    
}

void SceneData::setScene(const SceneRef &aScene)
{
    auto loadGlslProg = [ & ]( const gl::GlslProg::Format& format ) -> ci::gl::GlslProgRef
    {
        string names = format.getVertexPath().string() + " + " +
        format.getFragmentPath().string();
        gl::GlslProgRef glslProg;
        try {
            glslProg = gl::GlslProg::create( format );
        } catch ( const Exception& ex ) {
            CI_LOG_EXCEPTION( names, ex );
        }
        return glslProg;
    };
    
    mScene = aScene;
    //make the appropraie gbuffermaterial
    
    mGBufferMaterial = MaterialGBuffer::create();
    auto matGBuffer = mGBufferMaterial;
    auto parentRenderer = std::dynamic_pointer_cast<DeferredRenderer>(mScene->getParentRenderer());
    matGBuffer->mIsAlbedoEnabled = parentRenderer->getIsAlbedoEnabled();
    matGBuffer->mIsMaterialIdEnabled = parentRenderer->getIsMaterialIdEnabled();
    matGBuffer->mIsTexCoordEnabled = parentRenderer->getIsTexCoordEnabled();
    matGBuffer->mIsNormalEnabled = parentRenderer->getIsNormalEnabled();
    matGBuffer->mIsPositionEnabled = parentRenderer->getIsPositionEnabled();
    matGBuffer->mIsEmissionEnabled = parentRenderer->getIsEmissionEnabled();
    matGBuffer->mIsOcclusionEnabled = parentRenderer->getIsOcclusionEnabled();
    matGBuffer->mIsSpecularEnabled = parentRenderer->getIsSpecularEnabled();
    matGBuffer->mMaxNumDirectionalLights = mScene->getMaxNumDirectionalLights();
    matGBuffer->mMaxNumPointLights = mScene->getMaxNumPointLights();
    matGBuffer->mMaxNumSpotLights = mScene->getMaxNumSpotLights();
    matGBuffer->mMaxNumClassicMaterials = mScene->getMaxNumClassicMaterials();
    matGBuffer->mMaxNumPbrMaterials = mScene->getMaxNumPbrMaterials();
    matGBuffer->mAlbedoLayoutIndex = parentRenderer->getAlbedoLayoutIndex();
    matGBuffer->mMaterialIdLayoutIndex = parentRenderer->getMaterialIdLayoutIndex();
    matGBuffer->mTexCoordLayoutIndex = parentRenderer->getTexCoordLayoutIndex();
    matGBuffer->mNormalLayoutIndex = parentRenderer->getNormalLayoutIndex();
    matGBuffer->mPositionLayoutIndex = parentRenderer->getPositionLayoutIndex();
    matGBuffer->mEmissionLayoutIndex = parentRenderer->getEmissionLayoutIndex();
    matGBuffer->mOcclusionLayoutIndex = parentRenderer->getOcclusionLayoutIndex();
    matGBuffer->mSpecularLayoutIndex = parentRenderer->getSpecularLayoutIndex();
    matGBuffer->mClassicMaterialsUboIndex = mClassicMaterialsUboIndex;
    matGBuffer->mPbrMaterialsUboIndex = mPbrMaterialsUboIndex;
    matGBuffer->mHasClassicMaterials = true;
    matGBuffer->mHasPbrMaterials = false;
    matGBuffer->setup();
    
    //instancerMaterial
    auto matInstancer = MaterialGBufferInstancer::create();
    matInstancer->mIsAlbedoEnabled = parentRenderer->getIsAlbedoEnabled();
    matInstancer->mIsMaterialIdEnabled = parentRenderer->getIsMaterialIdEnabled();
    matInstancer->mIsTexCoordEnabled = parentRenderer->getIsTexCoordEnabled();
    matInstancer->mIsNormalEnabled = parentRenderer->getIsNormalEnabled();
    matInstancer->mIsPositionEnabled = parentRenderer->getIsPositionEnabled();
    matInstancer->mIsEmissionEnabled = parentRenderer->getIsEmissionEnabled();
    matInstancer->mIsOcclusionEnabled = parentRenderer->getIsOcclusionEnabled();
    matInstancer->mIsSpecularEnabled = parentRenderer->getIsSpecularEnabled();
    matInstancer->mMaxNumDirectionalLights = mScene->getMaxNumDirectionalLights();
    matInstancer->mMaxNumPointLights = mScene->getMaxNumPointLights();
    matInstancer->mMaxNumSpotLights = mScene->getMaxNumSpotLights();
    matInstancer->mMaxNumClassicMaterials = mScene->getMaxNumClassicMaterials();
    matInstancer->mMaxNumPbrMaterials = mScene->getMaxNumPbrMaterials();
    matInstancer->mAlbedoLayoutIndex = parentRenderer->getAlbedoLayoutIndex();
    matInstancer->mMaterialIdLayoutIndex = parentRenderer->getMaterialIdLayoutIndex();
    matInstancer->mTexCoordLayoutIndex = parentRenderer->getTexCoordLayoutIndex();
    matInstancer->mNormalLayoutIndex = parentRenderer->getNormalLayoutIndex();
    matInstancer->mPositionLayoutIndex = parentRenderer->getPositionLayoutIndex();
    matInstancer->mEmissionLayoutIndex = parentRenderer->getEmissionLayoutIndex();
    matInstancer->mOcclusionLayoutIndex = parentRenderer->getOcclusionLayoutIndex();
    matInstancer->mSpecularLayoutIndex = parentRenderer->getSpecularLayoutIndex();
    matInstancer->mClassicMaterialsUboIndex = mClassicMaterialsUboIndex;
    matInstancer->mPbrMaterialsUboIndex = mPbrMaterialsUboIndex;
    matInstancer->mHasClassicMaterials = true;
    matInstancer->mHasPbrMaterials = false;
    matInstancer->setup();
    mGBufferInstancerMaterial = matInstancer;
    
    //quadInstancerMaterial
    //instancerMaterial
    auto matQuadInstancer = MaterialGBufferQuadInstancer::create();
    matQuadInstancer->mIsAlbedoEnabled = parentRenderer->getIsAlbedoEnabled();
    matQuadInstancer->mIsMaterialIdEnabled = parentRenderer->getIsMaterialIdEnabled();
    matQuadInstancer->mIsTexCoordEnabled = parentRenderer->getIsTexCoordEnabled();
    matQuadInstancer->mIsNormalEnabled = parentRenderer->getIsNormalEnabled();
    matQuadInstancer->mIsPositionEnabled = parentRenderer->getIsPositionEnabled();
    matQuadInstancer->mIsEmissionEnabled = parentRenderer->getIsEmissionEnabled();
    matQuadInstancer->mIsOcclusionEnabled = parentRenderer->getIsOcclusionEnabled();
    matQuadInstancer->mIsSpecularEnabled = parentRenderer->getIsSpecularEnabled();
    matQuadInstancer->mMaxNumDirectionalLights = mScene->getMaxNumDirectionalLights();
    matQuadInstancer->mMaxNumPointLights = mScene->getMaxNumPointLights();
    matQuadInstancer->mMaxNumSpotLights = mScene->getMaxNumSpotLights();
    matQuadInstancer->mMaxNumClassicMaterials = mScene->getMaxNumClassicMaterials();
    matQuadInstancer->mMaxNumPbrMaterials = mScene->getMaxNumPbrMaterials();
    matQuadInstancer->mAlbedoLayoutIndex = parentRenderer->getAlbedoLayoutIndex();
    matQuadInstancer->mMaterialIdLayoutIndex = parentRenderer->getMaterialIdLayoutIndex();
    matQuadInstancer->mTexCoordLayoutIndex = parentRenderer->getTexCoordLayoutIndex();
    matQuadInstancer->mNormalLayoutIndex = parentRenderer->getNormalLayoutIndex();
    matQuadInstancer->mPositionLayoutIndex = parentRenderer->getPositionLayoutIndex();
    matQuadInstancer->mEmissionLayoutIndex = parentRenderer->getEmissionLayoutIndex();
    matQuadInstancer->mOcclusionLayoutIndex = parentRenderer->getOcclusionLayoutIndex();
    matQuadInstancer->mSpecularLayoutIndex = parentRenderer->getSpecularLayoutIndex();
    matQuadInstancer->mClassicMaterialsUboIndex = mClassicMaterialsUboIndex;
    matQuadInstancer->mPbrMaterialsUboIndex = mPbrMaterialsUboIndex;
    matQuadInstancer->mHasClassicMaterials = true;
    matQuadInstancer->mHasPbrMaterials = false;
    matQuadInstancer->setup();
    mGBufferQuadInstancerMaterial = matQuadInstancer;
    
    
    
    //lbufferbatches
    DataSourceRef vertLBufferDirectionalLight = loadAsset("../assets/deferred/lbuffer_directional_light.vert");
    DataSourceRef vertLBufferPointLight = loadAsset("../assets/deferred/lbuffer_point_light.vert");
    DataSourceRef vertLBufferSpotLight = loadAsset("../assets/deferred/lbuffer_spot_light.vert");
    DataSourceRef fragLBufferDirectionalLight = loadAsset("../assets/deferred/lbuffer_directional_light.frag");
    DataSourceRef fragLBufferPointLight = loadAsset("../assets/deferred/lbuffer_point_light.frag");
    DataSourceRef fragLBufferSpotLight = loadAsset("../assets/deferred/lbuffer_spot_light.frag");
    
    int32_t version					= 330;
    
    //directionallight L-Buffer shader setup
    gl::GlslProgRef lbufferDirectionalLight = loadGlslProg(gl::GlslProg::Format().version( version ).vertex(vertLBufferDirectionalLight).fragment(fragLBufferDirectionalLight).define("MAX_NUM_DIRECTIONAL_LIGHTS", ci::toString(mScene->getMaxNumDirectionalLights())).define("HAS_CLASSIC_MATERIALS").define("MAX_NUM_CLASSIC_MATERIALS", ci::toString(mScene->getMaxNumClassicMaterials())));
    lbufferDirectionalLight->uniformBlock("DirectionalLights", mDirectionalLightsUboIndex);
    lbufferDirectionalLight->uniformBlock("ClassicMaterials", mClassicMaterialsUboIndex);
    lbufferDirectionalLight->uniformBlock("ActiveCamera", mActiveCameraUboIndex);
    lbufferDirectionalLight->uniform("uSamplerAlbedo", parentRenderer->getAlbedoLayoutIndex());
    lbufferDirectionalLight->uniform("uSamplerNormal", parentRenderer->getNormalLayoutIndex());
    lbufferDirectionalLight->uniform("uSamplerMaterial", parentRenderer->getMaterialIdLayoutIndex());
    lbufferDirectionalLight->uniform("uSamplerDepth", parentRenderer->mDepthLayoutIndex);
    lbufferDirectionalLight->uniform("uWindowSize", ci::vec2(parentRenderer->getParentWindow()->getSize()));
    lbufferDirectionalLight->uniform("uOffset", ci::vec2(0.0, 0.0));
    
    //pointlight L-Buffer shader setup
    gl::GlslProgRef lbufferPointLight = loadGlslProg(gl::GlslProg::Format().version( version ).vertex(vertLBufferPointLight).fragment(fragLBufferPointLight).define("MAX_NUM_POINT_LIGHTS", ci::toString(mScene->getMaxNumPointLights())).define("HAS_CLASSIC_MATERIALS").define("MAX_NUM_CLASSIC_MATERIALS", ci::toString(mScene->getMaxNumClassicMaterials())));
    lbufferPointLight->uniformBlock("PointLights", mPointLightsUboIndex);
    lbufferPointLight->uniformBlock("ClassicMaterials", mClassicMaterialsUboIndex);
    lbufferPointLight->uniformBlock("ActiveCamera", mActiveCameraUboIndex);
    lbufferPointLight->uniform("uSamplerAlbedo", parentRenderer->getAlbedoLayoutIndex());
    lbufferPointLight->uniform("uSamplerNormal", parentRenderer->getNormalLayoutIndex());
    lbufferPointLight->uniform("uSamplerMaterial", parentRenderer->getMaterialIdLayoutIndex());
    lbufferPointLight->uniform("uSamplerDepth", parentRenderer->mDepthLayoutIndex);
    lbufferPointLight->uniform("uWindowSize", ci::vec2(parentRenderer->getParentWindow()->getSize()));
    lbufferPointLight->uniform("uOffset", ci::vec2(0.0, 0.0));
    
    //spotlight L-Buffer shader setup
    gl::GlslProgRef lbufferSpotLight = loadGlslProg(gl::GlslProg::Format().version( version ).vertex(vertLBufferPointLight).fragment(fragLBufferPointLight).define("MAX_NUM_POINT_LIGHTS", ci::toString(mScene->getMaxNumPointLights())).define("HAS_CLASSIC_MATERIALS").define("MAX_NUM_CLASSIC_MATERIALS", ci::toString(mScene->getMaxNumClassicMaterials())));
    lbufferSpotLight->uniformBlock("SpotLights", mSpotLightsUboIndex);
    lbufferSpotLight->uniformBlock("ClassicMaterials", mClassicMaterialsUboIndex);
    lbufferSpotLight->uniformBlock("ActiveCamera", mActiveCameraUboIndex);
    lbufferSpotLight->uniform("uSamplerAlbedo", parentRenderer->getAlbedoLayoutIndex());
    lbufferSpotLight->uniform("uSamplerNormal", parentRenderer->getNormalLayoutIndex());
    lbufferSpotLight->uniform("uSamplerMaterial", parentRenderer->getMaterialIdLayoutIndex());
    lbufferSpotLight->uniform("uSamplerDepth", parentRenderer->mDepthLayoutIndex);
    lbufferSpotLight->uniform("uWindowSize", ci::vec2(parentRenderer->getParentWindow()->getSize()));
    lbufferSpotLight->uniform("uOffset", ci::vec2(0.0, 0.0));
    
    gl::VboMeshRef cube			= gl::VboMesh::create( geom::Cube().size( vec3( 2.0f ) ) );
    gl::VboMeshRef rect			= gl::VboMesh::create( geom::Rect());
    gl::VboMeshRef sphere		= gl::VboMesh::create( geom::Sphere().subdivisions( 64 ) );
    gl::VboMeshRef sphereLow	= gl::VboMesh::create( geom::Sphere().subdivisions( 12 ) );
    gl::VboMeshRef cone         = gl::VboMesh::create( geom::Cone().subdivisionsHeight(60).subdivisionsAxis(60).height(1.0).radius(0.0, 1.0));
    gl::VboMeshRef coneLow      = gl::VboMesh::create( geom::Cone().subdivisionsHeight(16).subdivisionsAxis(16));
    
    mBatchLBufferDirectionalLight = gl::Batch::create( rect, lbufferDirectionalLight);
    mBatchLBufferPointLight = gl::Batch::create( sphere, lbufferPointLight);
    mBatchLBufferSpotLight = gl::Batch::create( cone, lbufferSpotLight);
    
    //setup ubos
    mUboDirectionalLights = ci::gl::Ubo::create(sizeof(DirectionalLightData) * mScene->getMaxNumDirectionalLights());
    mUboPointLights = ci::gl::Ubo::create(sizeof(PointLightData) * mScene->getMaxNumPointLights());
    mUboSpotLights = ci::gl::Ubo::create(sizeof(SpotLightData) * mScene->getMaxNumSpotLights());
    mUboClassicMaterials = ci::gl::Ubo::create(sizeof(ClassicMaterialData) * mScene->getMaxNumClassicMaterials());
    mUboPbrMaterials = ci::gl::Ubo::create(sizeof(PbrMaterialData) * mScene->getMaxNumPbrMaterials());
    mUboActiveCamera = ci::gl::Ubo::create(sizeof(CameraData));
    
    
    //fillout mMeshes
    mMeshes.clear();
    auto& meshes = mScene->getMeshes();
    for (auto& mesh : meshes){
        MaterialRef localMaterial = matGBuffer->clone();
        localMaterial->setDiffuseMap(mesh->getMaterial()->getDiffuseMap());
        localMaterial->setNormalMap(mesh->getMaterial()->getNormalMap());
        localMaterial->setDisplacementMap(mesh->getMaterial()->getDisplacementMap());
        localMaterial->setSpecularMap(mesh->getMaterial()->getSpecularMap());
        localMaterial->setEmissionMap(mesh->getMaterial()->getEmissionMap());
        localMaterial->setOcclusionMap(mesh->getMaterial()->getOcclusuionMap());
        localMaterial->setId(mesh->getMaterial()->getId());
        
        MeshRef localMesh = mesh->clone(false);//a clone so everything would tag along
        localMesh->setGuid(mesh->getGuid());
        localMesh->setModelWorld(mesh->getModelWorld());
        localMesh->setMaterial(localMaterial);
        
        mMeshes[mesh] = localMesh;
    }
    
    mInstancers.clear();
    auto& instancers = mScene->getInstancers();
    for (auto& instancer : instancers){
        MaterialGBufferInstancerRef localMaterial = matInstancer->clone();
        const auto& mesh = instancer->getReferenceMesh();
        localMaterial->setDiffuseMap(mesh->getMaterial()->getDiffuseMap());
        localMaterial->setNormalMap(mesh->getMaterial()->getNormalMap());
        localMaterial->setDisplacementMap(mesh->getMaterial()->getDisplacementMap());
        localMaterial->setSpecularMap(mesh->getMaterial()->getSpecularMap());
        localMaterial->setEmissionMap(mesh->getMaterial()->getEmissionMap());
        localMaterial->setOcclusionMap(mesh->getMaterial()->getOcclusuionMap());
        localMaterial->setId(mesh->getMaterial()->getId());
        localMaterial->mNumInstances = instancer->getChildren().size();
        localMaterial->mMaxNumInstances = instancer->getMaxCapacity();
        
        MeshRef localMesh = mesh->clone(false);
        localMesh->setGuid(mesh->getGuid());
        localMesh->setModelWorld(mesh->getModelWorld());
        localMesh->setMaterial(localMaterial);
        
        mInstancers[instancer] = localMesh;
    }
    
    mQuadInstancers.clear();
    auto& quadInstancers = mScene->getQuadInstancers();
    for (auto& instancer : quadInstancers){
        MaterialGBufferQuadInstancerRef localMaterial = matQuadInstancer->clone();
        const auto& mesh = instancer->getReferenceMesh();
        localMaterial->setDiffuseMap(mesh->getMaterial()->getDiffuseMap());
        localMaterial->setNormalMap(mesh->getMaterial()->getNormalMap());
        localMaterial->setDisplacementMap(mesh->getMaterial()->getDisplacementMap());
        localMaterial->setSpecularMap(mesh->getMaterial()->getSpecularMap());
        localMaterial->setEmissionMap(mesh->getMaterial()->getEmissionMap());
        localMaterial->setOcclusionMap(mesh->getMaterial()->getOcclusuionMap());
        localMaterial->setId(mesh->getMaterial()->getId());
        localMaterial->mNumInstances = instancer->getChildren().size();
        localMaterial->mMaxNumInstances = instancer->getMaxCapacity();
        
        MeshRef localMesh = mesh->clone(false);
        localMesh->setGuid(mesh->getGuid());
        localMesh->setModelWorld(mesh->getModelWorld());
        localMesh->setMaterial(localMaterial);
        
        mQuadInstancers[instancer] = localMesh;
    }
}

void SceneData::onNotifyAdded(const Object3DRef &aAddedObject)
{
    
    if (aAddedObject->hasTag("mesh")){
        auto mesh = aAddedObject->cast<Mesh>();
        MaterialRef localMaterial = mGBufferMaterial->clone();
        localMaterial->setDiffuseMap(mesh->getMaterial()->getDiffuseMap());
        localMaterial->setNormalMap(mesh->getMaterial()->getNormalMap());
        localMaterial->setDisplacementMap(mesh->getMaterial()->getDisplacementMap());
        localMaterial->setSpecularMap(mesh->getMaterial()->getSpecularMap());
        localMaterial->setEmissionMap(mesh->getMaterial()->getEmissionMap());
        localMaterial->setOcclusionMap(mesh->getMaterial()->getOcclusuionMap());
        localMaterial->setId(mesh->getMaterial()->getId());
        
        MeshRef localMesh = mesh->clone(false);//a clone so everything would tag along
        localMesh->setGuid(mesh->getGuid());
        localMesh->setModelWorld(mesh->getModelWorld());
        localMesh->setMaterial(localMaterial);
        
        mMeshes[mesh] = localMesh;
    }
    if (aAddedObject->hasTag("instancer")){
        auto instancer = aAddedObject->cast<Instancer>();
        MaterialGBufferInstancerRef localMaterial = mGBufferInstancerMaterial->clone();
        const auto& mesh = instancer->getReferenceMesh();
        localMaterial->setDiffuseMap(mesh->getMaterial()->getDiffuseMap());
        localMaterial->setNormalMap(mesh->getMaterial()->getNormalMap());
        localMaterial->setDisplacementMap(mesh->getMaterial()->getDisplacementMap());
        localMaterial->setSpecularMap(mesh->getMaterial()->getSpecularMap());
        localMaterial->setEmissionMap(mesh->getMaterial()->getEmissionMap());
        localMaterial->setOcclusionMap(mesh->getMaterial()->getOcclusuionMap());
        localMaterial->setId(mesh->getMaterial()->getId());
        localMaterial->mNumInstances = instancer->getChildren().size();
        localMaterial->mMaxNumInstances = instancer->getMaxCapacity();
        
        MeshRef localMesh = mesh->clone(false);
        localMesh->setGuid(mesh->getGuid());
        localMesh->setModelWorld(mesh->getModelWorld());
        localMesh->setMaterial(localMaterial);
        
        mInstancers[instancer] = localMesh;
    }
    
    if (aAddedObject->hasTag("quad_instancer")){
        auto instancer = aAddedObject->cast<QuadInstancer>();
        MaterialGBufferQuadInstancerRef localMaterial = mGBufferQuadInstancerMaterial->clone();
        const auto& mesh = instancer->getReferenceMesh();
        localMaterial->setDiffuseMap(mesh->getMaterial()->getDiffuseMap());
        localMaterial->setNormalMap(mesh->getMaterial()->getNormalMap());
        localMaterial->setDisplacementMap(mesh->getMaterial()->getDisplacementMap());
        localMaterial->setSpecularMap(mesh->getMaterial()->getSpecularMap());
        localMaterial->setEmissionMap(mesh->getMaterial()->getEmissionMap());
        localMaterial->setOcclusionMap(mesh->getMaterial()->getOcclusuionMap());
        localMaterial->setId(mesh->getMaterial()->getId());
        localMaterial->mNumInstances = instancer->getChildren().size();
        localMaterial->mMaxNumInstances = instancer->getMaxCapacity();
        
        MeshRef localMesh = mesh->clone(false);
        localMesh->setGuid(mesh->getGuid());
        localMesh->setModelWorld(mesh->getModelWorld());
        localMesh->setMaterial(localMaterial);
        mQuadInstancers[instancer] = localMesh;
    }
}

void SceneData::onNotifyRemoved(const Object3DRef &aRemovedObject)
{
    if (aRemovedObject->hasTag("mesh")){
        auto mesh = aRemovedObject->cast<Mesh>();
        mMeshes.erase(mesh);
    }
    if (aRemovedObject->hasTag("instancer")){
        auto instancer = aRemovedObject->cast<Instancer>();
        mInstancers.erase(instancer);
    }
    if (aRemovedObject->hasTag("quad_instancer")){
        auto instancer = aRemovedObject->cast<QuadInstancer>();
        mQuadInstancers.erase(instancer);
    }
}

void SceneData::bind()
{
    if (mIsBinded) return;
    else{
        mIsBinded = true;
        mUboDirectionalLights->bindBufferBase(mDirectionalLightsUboIndex);
        mUboPointLights->bindBufferBase(mPointLightsUboIndex);
        mUboSpotLights->bindBufferBase(mSpotLightsUboIndex);
        mUboClassicMaterials->bindBufferBase(mClassicMaterialsUboIndex);
        mUboPbrMaterials->bindBufferBase(mPbrMaterialsUboIndex);
        mUboActiveCamera->bindBufferBase(mActiveCameraUboIndex);
    }
}

void SceneData::unbind()
{
    if (!mIsBinded) return;
    else {
        mIsBinded = false;
        //unbind UBOs hopefully
        cinder::gl::context()->bindBufferBase(mUboDirectionalLights->getTarget(), mDirectionalLightsUboIndex, 0);
        cinder::gl::context()->bindBufferBase(mUboPointLights->getTarget(), mPointLightsUboIndex, 0);
        cinder::gl::context()->bindBufferBase(mUboSpotLights->getTarget(), mSpotLightsUboIndex, 0);
        cinder::gl::context()->bindBufferBase(mUboClassicMaterials->getTarget(), mClassicMaterialsUboIndex, 0);
        cinder::gl::context()->bindBufferBase(mUboPbrMaterials->getTarget(), mPbrMaterialsUboIndex, 0);
        cinder::gl::context()->bindBufferBase(mUboActiveCamera->getTarget(), mActiveCameraUboIndex, 0);
    }
}

void SceneData::update()
{
    updateCameraUbo();
    updateLightUbos();
    updateMaterialUbos();
    const auto& meshes = mScene->getMeshes();
    for (const auto& mesh : meshes){
        const auto& localMaterial = mMeshes[mesh]->getMaterial();
        localMaterial->setDiffuseMap(mesh->getMaterial()->getDiffuseMap());
        localMaterial->setNormalMap(mesh->getMaterial()->getNormalMap());
        localMaterial->setDisplacementMap(mesh->getMaterial()->getDisplacementMap());
        localMaterial->setSpecularMap(mesh->getMaterial()->getSpecularMap());
        localMaterial->setEmissionMap(mesh->getMaterial()->getEmissionMap());
        localMaterial->setOcclusionMap(mesh->getMaterial()->getOcclusuionMap());
        localMaterial->setId(mesh->getMaterial()->getId());
        
        MeshRef& localMesh = mMeshes[mesh];
        localMesh->setModelWorld(mesh->getModelWorld());
        
        mMeshes[mesh] = localMesh;
    }
    auto& instancers = mScene->getInstancers();
    for (auto& instancer : instancers){
        auto localMaterial = mInstancers[instancer]->getMaterial()->cast<MaterialGBufferInstancer>();
        const auto& mesh = instancer->getReferenceMesh();
        localMaterial->setDiffuseMap(mesh->getMaterial()->getDiffuseMap());
        localMaterial->setNormalMap(mesh->getMaterial()->getNormalMap());
        localMaterial->setDisplacementMap(mesh->getMaterial()->getDisplacementMap());
        localMaterial->setSpecularMap(mesh->getMaterial()->getSpecularMap());
        localMaterial->setEmissionMap(mesh->getMaterial()->getEmissionMap());
        localMaterial->setOcclusionMap(mesh->getMaterial()->getOcclusuionMap());
        localMaterial->setId(mesh->getMaterial()->getId());
        localMaterial->mNumInstances = instancer->getChildren().size();
        localMaterial->mMaxNumInstances = instancer->getMaxCapacity();
    }
    
    auto& quadInstancers = mScene->getQuadInstancers();
    for (auto& instancer : quadInstancers){
        auto localMaterial = mQuadInstancers[instancer]->getMaterial()->cast<MaterialGBufferQuadInstancer>();
        const auto& mesh = instancer->getReferenceMesh();
        localMaterial->setDiffuseMap(mesh->getMaterial()->getDiffuseMap());
        localMaterial->setNormalMap(mesh->getMaterial()->getNormalMap());
        localMaterial->setDisplacementMap(mesh->getMaterial()->getDisplacementMap());
        localMaterial->setSpecularMap(mesh->getMaterial()->getSpecularMap());
        localMaterial->setEmissionMap(mesh->getMaterial()->getEmissionMap());
        localMaterial->setOcclusionMap(mesh->getMaterial()->getOcclusuionMap());
        localMaterial->setId(mesh->getMaterial()->getId());
        localMaterial->mNumInstances = instancer->getChildren().size();
        localMaterial->mMaxNumInstances = instancer->getMaxCapacity();
    }
}

void SceneData::drawGBuffer()
{
    auto& meshes = mScene->getMeshes();
    for (auto& mesh : meshes){
        if (mesh->getIsVisible()){
            mMeshes[mesh]->draw();
        }
    }
    auto& instancers = mScene->getInstancers();
    for (auto& instancer : instancers){
        if (instancer->getIsVisible()){
            mInstancers[instancer]->draw();
        }
    }
//    auto& quadInstancers = mScene->getQuadInstancers();
//    for (auto& instancer : quadInstancers){
//        if (instancer->getIsVisible()){
//            mQuadInstancers[instancer]->draw();
//        }
//    }
}

void SceneData::drawLBuffer(const ci::ivec2& aSize)
{
    //draw directional lights
    {
        const gl::ScopedViewport scopedViewport( ivec2( 0 ), aSize );
        const gl::ScopedMatrices scopedMatrices;
        gl::setMatricesWindow( aSize );
        gl::translate( aSize / 2 );
        gl::scale(aSize);
        mBatchLBufferDirectionalLight->drawInstanced( mScene->getDirectionalLights().size() );
    }
    
    //draw point lights
    {
        const gl::ScopedFaceCulling scopedFaceCulling( true, GL_FRONT );
        mBatchLBufferPointLight->drawInstanced(mScene->getPointLights().size());
    }
    
    //draw spot lights
    {
        const gl::ScopedFaceCulling scopedFaceCulling( true, GL_FRONT );
        mBatchLBufferSpotLight->drawInstanced(mScene->getSpotLights().size());
    }
    
}

void SceneData::updateCameraUbo()
{
    //update ActiveCamera
    CameraData* cameraData = (CameraData*)mUboActiveCamera->mapWriteOnly();
    const CameraObjectRef& activeCamera = mScene->getActiveCamera();
    cameraData->setViewMatrix(activeCamera->getCameraData().getViewMatrix());
    //ci::app::console()<<mScene->getActiveCamera()->getCameraData().getViewMatrix()<<std::endl;
    cameraData->setInversedViewMatrix(activeCamera->getCameraData().getInversedViewMatrix());
    cameraData->setProjectionMatrix(activeCamera->getCameraData().getProjectionMatrix());
    cameraData->setInversedProjectionMatrix(activeCamera->getCameraData().getInversedProjectionMatrix());
    cameraData->setPosition(activeCamera->getCameraData().getPosition());
    cameraData->setViewDirection(activeCamera->getCameraData().getViewDirection());
    cameraData->setProjectionParameters(activeCamera->getCameraData().getProjectionParameters());
    cameraData->setFov(activeCamera->getCameraData().getFov());
    cameraData->setAspectRatio(activeCamera->getCameraData().getAspectRatio());
    mUboActiveCamera->unmap();
}

void SceneData::updateLightUbos()
{
    //update directional lights
    DirectionalLightData* dirLightsData = (DirectionalLightData*)mUboDirectionalLights->mapWriteOnly();
    for ( const DirectionalLightRef& dirLight : mScene->getDirectionalLights() ) {
        dirLightsData->setAmbientIntensity(dirLight->getAmbientIntensity());
        dirLightsData->setDiffuseIntensity(dirLight->getDiffuseIntensity());
        dirLightsData->setColor(dirLight->getColor());
        dirLightsData->setDirection(dirLight->getDirection());
        ++dirLightsData;
    }
    mUboDirectionalLights->unmap();
    
    //update point lights
    PointLightData* pointLightsData = (PointLightData*)mUboPointLights->mapWriteOnly();
    for ( const PointLightRef& pointLight : mScene->getPointLights() ) {
        pointLightsData->setAmbientIntensity(pointLight->getAmbientIntensity());
        pointLightsData->setDiffuseIntensity(pointLight->getDiffuseIntensity());
        pointLightsData->setColor(pointLight->getColor());
        pointLightsData->setPosition(pointLight->getPosition());
        pointLightsData->setAttenuation(pointLight->getAttenuation());
        pointLightsData->setRadius(pointLight->getRadius());
        ++pointLightsData;

    }
    mUboPointLights->unmap();
    
    //update spot lights
    SpotLightData* spotLightsData = (SpotLightData*)mUboSpotLights->mapWriteOnly();
    for ( const SpotLightRef& spotLight : mScene->getSpotLights() ) {
        spotLightsData->setAmbientIntensity(spotLight->getAmbientIntensity());
        spotLightsData->setDiffuseIntensity(spotLight->getDiffuseIntensity());
        spotLightsData->setColor(spotLight->getColor());
        spotLightsData->setPosition(spotLight->getPosition());
        spotLightsData->setAttenuation(spotLight->getAttenuation());
        spotLightsData->setRadius(spotLight->getRadius());
        spotLightsData->setDirection(spotLight->getDirection());
        spotLightsData->setCutoff(spotLight->getCutoff());
        ++spotLightsData;
    }
    mUboSpotLights->unmap();
}

void SceneData::updateMaterialUbos()
{
    //update PbrMaterials
    PbrMaterialData* pbrMaterialData = (PbrMaterialData*) mUboPbrMaterials->mapWriteOnly();
    std::list<PbrMaterialRef> pbrMaterials = mScene->getPbrMaterialPallet().getMaterials();
    for (const PbrMaterialRef& pbrMaterial : pbrMaterials){
        pbrMaterialData = &(pbrMaterial->getMaterialData());
    }
    mUboPbrMaterials->unmap();
    
    //update ClassicMaterials
    ClassicMaterialData* classicMaterialData = (ClassicMaterialData*) mUboClassicMaterials->mapWriteOnly();
    std::list<ClassicMaterialRef> classicMaterials = mScene->getClassicMaterialPallet().getMaterials();
    for (const ClassicMaterialRef& classicMaterial : classicMaterials){
        classicMaterialData->setDiffuseColor(classicMaterial->getMaterialData().getDiffuseColor());
        classicMaterialData->setDiffuseIntensity(classicMaterial->getMaterialData().getDiffuseIntensity());
        classicMaterialData->setEmissionColor(classicMaterial->getMaterialData().getEmissionColor());
        classicMaterialData->setEmissionIntensity(classicMaterial->getMaterialData().getEmissionIntensity());
        classicMaterialData->setSpecularColor(classicMaterial->getMaterialData().getSpecularColor());
        classicMaterialData->setSpecularIntensity(classicMaterial->getMaterialData().getSpecularIntensity());
        classicMaterialData->setShininess(classicMaterial->getMaterialData().getShininess());
        ++classicMaterialData;
    }
    mUboClassicMaterials->unmap();
}

const SceneRef& SceneData::getScene() const
{
    return mScene;
}

const std::map<MeshRef, MeshRef>& SceneData::getMeshes() const
{
    return mMeshes;
}

bool SceneData::getIsBinded() const
{
    return mIsBinded;
}


