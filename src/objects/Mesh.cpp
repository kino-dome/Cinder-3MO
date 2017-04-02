//
//  Mesh.cpp
//  iPhotoWall
//
//  Created by Hesam Ohadi on 15/11/15.
//
//

#include "Mesh.h"
#include "Material.h"

Mesh::Mesh() : Object3D()
{
    mShouldNotifyTopParent = true;
    mType = "mesh";
    addTag("mesh");
    mName = "mesh";
}

Mesh::Mesh(const ci::gl::VboMeshRef& aVboMesh, const MaterialRef& aMaterial) : Object3D(), mVboMesh(aVboMesh), mMaterial(aMaterial)
{
    mShouldNotifyTopParent = true;
    mType = "mesh";
    addTag("mesh");
    mName = "mesh";
    mBatch = ci::gl::Batch::create(mVboMesh, mMaterial->getShader());\
}

Mesh::~Mesh()
{
    
}

void Mesh::drawSelf()
{
    
    const ci::gl::ScopedModelMatrix scopedModelMatrix;
    ci::gl::multModelMatrix( mModelWorld.getModelMatrix() );
    MeshRef meshRef = std::dynamic_pointer_cast<Mesh>(shared_from_this());
    mMaterial->attachAndDraw(meshRef);
    
}

void Mesh::updateSelf()
{
    
}

//setters/getters
Mesh& Mesh::vboMesh(const ci::gl::VboMeshRef &aVboMesh)
{
    mVboMesh = aVboMesh;
    mBatch->replaceVboMesh(aVboMesh);
    return *this;
}

Mesh& Mesh::material(const MaterialRef &aMaterial)
{
    mMaterial = aMaterial;
    mBatch->replaceGlslProg(mMaterial->getShader());
    return *this;
}

const ci::gl::VboMeshRef& Mesh::getVboMesh() const
{
    return mVboMesh;
}

const MaterialRef& Mesh::getMaterial() const
{
    return mMaterial;
}

const ci::gl::BatchRef& Mesh::getBatch() const
{
    return mBatch;
}

void Mesh::setVboMesh(const ci::gl::VboMeshRef &aVboMesh)
{
    mVboMesh = aVboMesh;
    mBatch->replaceVboMesh(aVboMesh);
}

void Mesh::setMaterial(const MaterialRef &aMaterial)
{
    mMaterial = aMaterial;
    mBatch->replaceGlslProg(mMaterial->getShader());
}
