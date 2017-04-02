//
//  Instancer.cpp
//  DeferredRenderer
//
//  Created by Hesam Ohadi on 12/12/15.
//
//
#include "cinder/Utilities.h"

#include "Instancer.h"
#include "Scene.h"


//////////////////////InstanceData///////////////////////

InstanceData::InstanceData() : mColor(ci::ColorAf(1.0, 1.0, 1.0, 1.0)), mMaterialId(0), mModelMatrix(ci::mat4()), mNormalMatrix(ci::mat3())
{
    
}

InstanceData::~InstanceData()
{
    
}

//////////////////////Instance////////////////////////////

Instance::Instance() : mColor(ci::ColorAf(1.0, 1.0, 1.0, 1.0)), mInstanceData(InstanceData()), mMaterial(nullptr)
{
    setType("instance");
    addTag("instance");
    mName = "instance";
}

Instance::~Instance()
{
    
}

void Instance::updateSelf()
{
    ci::mat4 m= getModelWorld().getModelMatrix();
    ci::mat4 cameraViewMatrix = getTopParent()->cast<Scene>()->getActiveCamera()->getCinderCamera().getViewMatrix();
    mModelWorld.setNormalMatrix(glm::inverseTranspose( ci::mat3( cameraViewMatrix * m ) ));
    
    mInstanceData.setColor(mColor);
    mInstanceData.setMaterialId(mMaterial->getId());
    mInstanceData.setModelMatrix(getModelWorld().getModelMatrix());
    mInstanceData.setNormalMatrix(getModelWorld().getNormalMatrix());
}

///////////////////////////Instancer////////////////////

Instancer::Instancer() : mDataVbo(nullptr), mReferenceMesh(nullptr), mMaxCapacity(100)
{
    addTag("instancer");
    setType("instancer");
    setShouldNotifyTopParent(true);
    mName = "instancer";
}

Instancer::Instancer(int aMaxCapacity) : Object3D(), mDataVbo(nullptr), mReferenceMesh(nullptr), mMaxCapacity(aMaxCapacity)
{
    addTag("instancer");
    setType("instancer");
    setShouldNotifyTopParent(true);
    mName = "instancer";
}

Instancer::~Instancer()
{
    
}

void Instancer::updateSelf()
{
    InstanceData* instanceData = (InstanceData*)mDataVbo->mapReplace();
    for( const auto& child : mChildren ) {
        InstanceRef instance = child->cast<Instance>();
        instanceData->setModelMatrix(instance->getInstanceData().getModelMatrix());
        instanceData->setNormalMatrix(instance->getInstanceData().getNormalMatrix());
        instanceData->setColor(instance->getInstanceData().getColor());
        instanceData->setMaterialId(instance->getInstanceData().getMaterialId());
        ++instanceData;
    }
    mDataVbo->unmap();
}

void Instancer::setup()
{
    // create the VBO which will contain per-instance (rather than per-vertex) data
    mDataVbo = ci::gl::Vbo::create( GL_ARRAY_BUFFER, mMaxCapacity * sizeof(InstanceData), nullptr , GL_DYNAMIC_DRAW );
    
    // we need a geom::BufferLayout to describe this data as mapping to the CUSTOM_0 semantic, and the 1 (rather than 0) as the last param indicates per-instance (rather than per-vertex)
    ci::geom::BufferLayout instanceDataLayout;
    instanceDataLayout.append( ci::geom::Attrib::CUSTOM_0, ci::geom::DataType::FLOAT, 16, sizeof(InstanceData), 0, 1 );//modelMatrix
    instanceDataLayout.append( ci::geom::Attrib::CUSTOM_1, ci::geom::DataType::FLOAT, 9, sizeof(InstanceData), sizeof(ci::mat4), 1 );//normalMatrix
    instanceDataLayout.append( ci::geom::Attrib::CUSTOM_2, ci::geom::DataType::FLOAT, 4, sizeof(InstanceData), sizeof(ci::mat4)+sizeof(ci::mat3), 1 );//color
    instanceDataLayout.append( ci::geom::Attrib::CUSTOM_3, ci::geom::DataType::INTEGER, 1, sizeof(InstanceData), sizeof(ci::mat4)+sizeof(ci::mat3)+sizeof(ci::ColorAf), 1 );//materialId
    
    // now add it to the VboMesh we already made of the Teapot
    mReferenceMesh->getVboMesh()->appendVbo( instanceDataLayout, mDataVbo );
    
    // and finally, build our batch, mapping our CUSTOM_0 attribute to the "vInstancePosition" GLSL vertex attribute
    ci::gl::GlslProg::Format shaderFormat;
    shaderFormat.vertex(ci::app::loadAsset("objects/instancer/instancer.vert")).fragment(ci::app::loadAsset("objects/instancer/instancer.frag")).attrib(ci::geom::Attrib::CUSTOM_0, "uInstanceModelMatrix").attrib(ci::geom::Attrib::CUSTOM_1, "uInstanceNormalMatrix").attrib(ci::geom::Attrib::CUSTOM_2, "uInstanceColor").attrib(ci::geom::Attrib::CUSTOM_3, "uInstanceMaterialId").define("HAS_CLASSIC_MATERIALS").define("MAX_NUM_CLASSIC_MATERIALS", ci::toString(25));
    auto shader = ci::gl::GlslProg::create(shaderFormat);
    shader->uniformBlock("ClassicMaterials", 3);
    mInstancerBatch = ci::gl::Batch::create(mReferenceMesh->getVboMesh(), shader);
}

void Instancer::drawSelf()
{
    if (getReferenceMesh()->getMaterial()->getDiffuseMap()){
        mInstancerBatch->getGlslProg()->uniform("uHasDiffuseMap", 1);
        getReferenceMesh()->getMaterial()->getDiffuseMap()->bind(mInstancerBatch->getGlslProg()->getAttribLocation("uDiffuseMap"));
    }
    else {
        mInstancerBatch->getGlslProg()->uniform("uHasDiffuseMap", 0);
    }
    
    mInstancerBatch->drawInstanced(mChildren.size());

    
    if (getReferenceMesh()->getMaterial()->getDiffuseMap()){
        getReferenceMesh()->getMaterial()->getDiffuseMap()->unbind();
    }
}

void Instancer::addInstance(const InstanceRef &aInstance)
{
    addChild(aInstance);
    aInstance->setMaterial(getReferenceMesh()->getMaterial());
    aInstance->setId(mChildren.size()-1);
}

void Instancer::removeInstance(const InstanceRef &aInstance)
{
    removeChild(aInstance);
    int id=0;
    for (auto& child : mChildren){
        child->setId(id);
        id++;
    }
}

std::vector<InstanceRef> Instancer::getInstances() const
{
    return getCastedChildrenByTag<Instance>("instance", true);
}
