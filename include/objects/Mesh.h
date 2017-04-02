//
//  Mesh.h
//  iPhotoWall
//
//  Created by Hesam Ohadi on 15/11/15.
//
//
#pragma once

#include "cinder/gl/Batch.h"

#include "../core/Object3D.h"

typedef std::shared_ptr<class Material> MaterialRef;
typedef std::shared_ptr<class Mesh> MeshRef;

class Mesh : public virtual Object3D
{
public:
    friend class Material;
    friend class MaterialWireframe;
    
    static MeshRef create(const ci::gl::VboMeshRef& aVboMesh, const MaterialRef& aMaterial){
        return MeshRef ( new Mesh(aVboMesh, aMaterial));
    }
    
    Mesh();
    Mesh(const ci::gl::VboMeshRef& aVboMesh, const MaterialRef& aMaterial);
    ~Mesh();
    
    void                                drawSelf() override;
    void                                updateSelf() override;
    //Object3DRef                         cloneSelf() override;
    OBJECT3D_CLONE(Mesh)
    
    //setters/getters
    Mesh&                               vboMesh(const ci::gl::VboMeshRef& aVboMesh);
    Mesh&                               material(const MaterialRef& aMaterial);
    const ci::gl::VboMeshRef&           getVboMesh() const;
    const MaterialRef&                  getMaterial() const;
    const ci::gl::BatchRef&             getBatch() const;
    void                                setVboMesh(const ci::gl::VboMeshRef& aVboMesh);
    void                                setMaterial(const MaterialRef& aMaterial);
    
public:
    
    ci::gl::VboMeshRef                  mVboMesh;
    MaterialRef                         mMaterial;
    ci::gl::BatchRef                    mBatch;
    
    
};
