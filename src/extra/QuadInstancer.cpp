//
//  QuadInstancer.cpp
//  a_Init
//
//  Created by Hesam Ohadi on 20/9/16.
//
//

#include "QuadInstancer.h"

#include "cinder/Utilities.h"

#include "Scene.h"

using namespace ci;
using namespace ci::app;
using namespace std;


//////////////////////InstanceData///////////////////////

QuadInstanceData::QuadInstanceData() : mColor(ci::ColorAf(1.0, 1.0, 1.0, 1.0)), mMaterialId(0), mModelMatrix(ci::mat4()), mNormalMatrix(ci::mat3()), mTexCoordOffset(vec2()), mTexCoordSize(vec2())
{
    
}

QuadInstanceData::~QuadInstanceData()
{
    
}

//////////////////////Instance////////////////////////////

QuadInstance::QuadInstance() : mColor(ColorAf(1.0, 1.0, 1.0, 1.0)), mInstanceData(QuadInstanceData()), mMaterial(nullptr), mTexCoordOffset(vec2()), mTexCoordSize(vec2())
{
    setType("quad_instance");
    addTag("quad_instance");
}

QuadInstance::~QuadInstance()
{
    
}

void QuadInstance::updateSelf()
{
    mat4 m= getModelWorld().getModelMatrix();
    mat4 cameraViewMatrix = getTopParent()->cast<Scene>()->getActiveCamera()->getCinderCamera().getViewMatrix();
    mModelWorld.setNormalMatrix(glm::inverseTranspose( mat3( cameraViewMatrix * m ) ));
    
    mInstanceData.setColor(mColor);
    mInstanceData.setMaterialId(mMaterial->getId());
    mInstanceData.setModelMatrix(getModelWorld().getModelMatrix());
    mInstanceData.setNormalMatrix(getModelWorld().getNormalMatrix());
    mInstanceData.setTexCoordOffset(mTexCoordOffset);
    mInstanceData.setTexCoordSize(mTexCoordSize);
}

///////////////////////////Instancer////////////////////

QuadInstancer::QuadInstancer() : mDataVbo(nullptr), mReferenceMesh(nullptr), mMaxCapacity(100)
{
    addTag("quad_instancer");
    setType("quad_instancer");
    setShouldNotifyTopParent(true);
}

QuadInstancer::QuadInstancer(int aMaxCapacity) : Object3D(), mDataVbo(nullptr), mReferenceMesh(nullptr), mMaxCapacity(aMaxCapacity)
{
    addTag("quad_instancer");
    setType("quad_instancer");
    setShouldNotifyTopParent(true);
}

QuadInstancer::~QuadInstancer()
{
    
}

void QuadInstancer::updateSelf()
{
    QuadInstanceData* instanceData = (QuadInstanceData*)mDataVbo->mapReplace();
    for( const auto& child : mChildren ) {
        QuadInstanceRef instance = child->cast<QuadInstance>();
        instanceData->setModelMatrix(instance->getInstanceData().getModelMatrix());
        instanceData->setNormalMatrix(instance->getInstanceData().getNormalMatrix());
        instanceData->setColor(instance->getInstanceData().getColor());
        instanceData->setMaterialId(instance->getInstanceData().getMaterialId());
        instanceData->setTexCoordOffset(instance->getTexCoordOffset());
        instanceData->setTexCoordSize(instance->getTexCoordSize());
        ++instanceData;
    }
    mDataVbo->unmap();
}

void QuadInstancer::setup()
{
    // create the VBO which will contain per-instance (rather than per-vertex) data
    mDataVbo = gl::Vbo::create( GL_ARRAY_BUFFER, mMaxCapacity * sizeof(InstanceData), nullptr , GL_DYNAMIC_DRAW );
    
    // we need a geom::BufferLayout to describe this data as mapping to the CUSTOM_0 semantic, and the 1 (rather than 0) as the last param indicates per-instance (rather than per-vertex)
    geom::BufferLayout instanceDataLayout;
    instanceDataLayout.append( geom::Attrib::CUSTOM_0, geom::DataType::FLOAT, 16, sizeof(QuadInstanceData), 0, 1 );//modelMatrix
    instanceDataLayout.append( geom::Attrib::CUSTOM_1, geom::DataType::FLOAT, 9, sizeof(QuadInstanceData), sizeof(mat4), 1 );//normalMatrix
    instanceDataLayout.append( geom::Attrib::CUSTOM_2, geom::DataType::FLOAT, 4, sizeof(QuadInstanceData), sizeof(mat4)+sizeof(mat3), 1 );//color
    instanceDataLayout.append( geom::Attrib::CUSTOM_3, geom::DataType::INTEGER, 1, sizeof(QuadInstanceData), sizeof(mat4)+sizeof(mat3)+sizeof(ColorAf), 1 );//materialId
    instanceDataLayout.append( geom::Attrib::CUSTOM_4, geom::DataType::FLOAT, 2, sizeof(QuadInstanceData), sizeof(mat4)+sizeof(mat3)+sizeof(ColorAf) + sizeof(int), 1 ); // tex offset
    instanceDataLayout.append( geom::Attrib::CUSTOM_5, geom::DataType::FLOAT, 2, sizeof(QuadInstanceData), sizeof(mat4)+sizeof(mat3)+sizeof(ColorAf) + sizeof(int) + sizeof(vec2), 1 ); // tex size
    
    // now add it to the VboMesh we already made of the Teapot
    mReferenceMesh->getVboMesh()->appendVbo( instanceDataLayout, mDataVbo );
    
    // and finally, build our batch, mapping our CUSTOM_0 attribute to the "vInstancePosition" GLSL vertex attribute
    gl::GlslProg::Format shaderFormat;
    shaderFormat.vertex(app::loadAsset("objects/instancer/instancer.vert")).fragment(app::loadAsset("objects/instancer/instancer.frag")).define("HAS_QUAD_INSTANCING").attrib(geom::Attrib::CUSTOM_0, "uInstanceModelMatrix").attrib(geom::Attrib::CUSTOM_1, "uInstanceNormalMatrix").attrib(geom::Attrib::CUSTOM_2, "uInstanceColor").attrib(geom::Attrib::CUSTOM_3, "uInstanceMaterialId").attrib(geom::Attrib::CUSTOM_4, "uInstanceTexCoordOffset").attrib(geom::Attrib::CUSTOM_5, "uInstanceTexCoordSize").define("HAS_CLASSIC_MATERIALS").define("MAX_NUM_CLASSIC_MATERIALS", toString(25));
    auto shader = gl::GlslProg::create(shaderFormat);
    shader->uniformBlock("ClassicMaterials", 3);
    mInstancerBatch = gl::Batch::create(mReferenceMesh->getVboMesh(), shader);
}

void QuadInstancer::setup(int aMaxCapacity)
{
    setMaxCapacity(aMaxCapacity);
    setup();
}

void QuadInstancer::drawSelf()
{
    if (getReferenceMesh()->getMaterial()->getDiffuseMap()){
        mInstancerBatch->getGlslProg()->uniform("uHasDiffuseMap", 1);
        getReferenceMesh()->getMaterial()->getDiffuseMap()->bind(mInstancerBatch->getGlslProg()->getAttribLocation("uDiffuseMap"));
    }
    else {
        mInstancerBatch->getGlslProg()->uniform("uHasDiffuseMap", 0);
    }
    
    mInstancerBatch->drawInstanced(mChildren.size());
    gl::drawSphere(mModelWorld.getModelTranslation(), 20);

    
    if (getReferenceMesh()->getMaterial()->getDiffuseMap()){
        getReferenceMesh()->getMaterial()->getDiffuseMap()->unbind();
    }
}

void QuadInstancer::addInstance(const QuadInstanceRef &aInstance)
{
    addChild(aInstance);
    aInstance->setMaterial(getReferenceMesh()->getMaterial());
    aInstance->setId(mChildren.size()-1);
}

void QuadInstancer::removeInstance(const QuadInstanceRef &aInstance)
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
