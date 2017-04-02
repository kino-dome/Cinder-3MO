//
//  Instancer.h
//  DeferredRenderer
//
//  Created by Hesam Ohadi on 12/12/15.
//
//
#pragma once
#include "cinder/gl/Vbo.h"

#include "../core/Object3D.h"
#include "../objects/Mesh.h"

typedef std::shared_ptr<class Instance> InstanceRef;
typedef std::shared_ptr<class Instancer> InstancerRef;

class InstanceData
{
public:
    InstanceData();
    ~InstanceData();
    
    //setters/getters
    InstanceData&                   modelMatrix(const ci::mat4& aModelMatrix) {mModelMatrix = aModelMatrix; return *this;}
    InstanceData&                   normalMatrix(const ci::mat3& aNormalMatrix) {mNormalMatrix = aNormalMatrix; return *this;}
    InstanceData&                   color(const ci::ColorAf& aColor) {mColor = aColor; return *this;}
    InstanceData&                   materialId(int aMaterialId) {mMaterialId = aMaterialId; return *this;}
    void                            setModelMatrix(const ci::mat4& aModelMatrix) {mModelMatrix = aModelMatrix;}
    void                            setNormalMatrix(const ci::mat3& aNormalMatrix) {mNormalMatrix = aNormalMatrix;}
    void                            setColor(const ci::ColorAf& aColor) {mColor = aColor;}
    void                            setMaterialId(int aMaterialId) {mMaterialId = aMaterialId;}
    const ci::mat4&                 getModelMatrix() const {return mModelMatrix;}
    ci::mat4                        getModelMatrix() {return mModelMatrix;}
    const ci::mat3&                 getNormalMatrix() const {return mNormalMatrix;}
    ci::mat3                        getNormalMatrix() {return mNormalMatrix;}
    const ci::ColorAf&              getColor() const {return mColor;}
    ci::ColorAf                     getColor() {return mColor;}
    int                             getMaterialId() const {return mMaterialId;}
    int                             getMaterialId() {return mMaterialId;}
    
public:
    ci::mat4                        mModelMatrix;
    ci::mat3                        mNormalMatrix;
    ci::ColorAf                     mColor;
    int                             mMaterialId;
};

class Instance : public virtual Object3D
{
public:
    static InstanceRef create(){
        return InstanceRef ( new Instance());
    }
    
    Instance();
    ~Instance();
    OBJECT3D_CLONE(Instance)
    
    void                            updateSelf() override;
    
    
    //setters/getters
    Instance&                       color(const ci::ColorAf& aColor) {mColor = aColor; return *this;}
    Instance&                       material(const MaterialRef& aMaterial) {mMaterial = aMaterial; return *this;}
    Instance&                       instanceData(const InstanceData& aInstanceData) {mInstanceData = aInstanceData; return *this;}
    void                            setColor(const ci::ColorAf& aColor) {mColor = aColor;}
    void                            setMaterial(const MaterialRef& aMaterial) {mMaterial = aMaterial;}
    void                            setInstanceData(const InstanceData& aInstanceData) {mInstanceData = aInstanceData;}
    const MaterialRef&              getMaterial() const {return mMaterial;}
    MaterialRef                     getMaterial() {return mMaterial;}
    const ci::ColorAf&              getColor() const {return mColor;}
    ci::ColorAf                     getColor() {return mColor;}
    const InstanceData&             getInstanceData() const {return mInstanceData;}
    InstanceData                    getInstanceData() {return mInstanceData;}
    
    
public:
    MaterialRef                     mMaterial;//this will be the mirror of the desired material for this instance, only the materialId is used atm
    ci::ColorAf                     mColor; // this will be the mirror of the color in InstanceData
    
    InstanceData                    mInstanceData;
};

class Instancer : public virtual Object3D
{
public:
    
    static InstancerRef create(int aMaxCapacity){
        return InstancerRef ( new Instancer(aMaxCapacity));
    }
    
    Instancer();
    Instancer(int aMaxCapacity);
    ~Instancer();
    OBJECT3D_CLONE(Instancer)
    
    void                            updateSelf() override;
    void                            drawSelf() override;
    virtual void                    setup();
    virtual void                    setup(int aMaxCapacity);
    void                            addInstance(const InstanceRef& aInstance);
    void                            removeInstance(const InstanceRef& aInstance);
    std::vector<InstanceRef>        getInstances() const;
    
    //setters/getters
    Instancer&                      maxCapacity(int aMaxCapacity) {mMaxCapacity = aMaxCapacity; return *this;}
    Instancer&                      referenceMesh(const MeshRef& aReferenceMesh) {mReferenceMesh = aReferenceMesh; return *this;}
    Instancer&                      dataVbo(const ci::gl::VboRef& aDataVbo) {mDataVbo = aDataVbo; return *this;}
    Instancer&                      instancerBatch(const ci::gl::BatchRef& aInstancerBatch) {mInstancerBatch = aInstancerBatch; return *this;}
    void                            setMaxCapacity(int aMaxCapacity) {mMaxCapacity = aMaxCapacity;}
    void                            setDataVbo(const ci::gl::VboRef& aDataVbo) {mDataVbo = aDataVbo;}
    void                            setReferenceMesh(const MeshRef& aReferenceMesh) {mReferenceMesh = aReferenceMesh;}
    void                            setInstancerBatch(const ci::gl::BatchRef& aInstancerBatch) {mInstancerBatch = aInstancerBatch;}
    int                             getMaxCapacity() const {return mMaxCapacity;}
    int                             getMaxCapacity() {return mMaxCapacity;}
    const ci::gl::VboRef&           getDataVbo() const {return mDataVbo;}
    ci::gl::VboRef                  getDataVbo() {return mDataVbo;}
    const MeshRef&                  getReferenceMesh() const {return mReferenceMesh;}
    MeshRef                         getReferenceMesh() {return mReferenceMesh;}
    const ci::gl::BatchRef&         getInstancerBatch() const {return mInstancerBatch;}
    ci::gl::BatchRef                getInstancerBatch() {return mInstancerBatch;}
    
    
public:
    int                             mMaxCapacity;
    ci::gl::VboRef                  mDataVbo;
    MeshRef                         mReferenceMesh;
    ci::gl::BatchRef                mInstancerBatch;
};