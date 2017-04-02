//
//  QuadInstancer.h
//  a_Init
//
//  Created by Hesam Ohadi on 20/9/16.
//
//

#ifndef __a_Init__QuadInstancer__
#define __a_Init__QuadInstancer__

#include "Instancer.h"

typedef std::shared_ptr<class QuadInstance> QuadInstanceRef;
typedef std::shared_ptr<class QuadInstancer> QuadInstancerRef;

class QuadInstanceData
{
public:
    QuadInstanceData();
    ~QuadInstanceData();
    
    //setters/getters
    QuadInstanceData&                   modelMatrix(const ci::mat4& aModelMatrix) {mModelMatrix = aModelMatrix; return *this;}
    QuadInstanceData&                   normalMatrix(const ci::mat3& aNormalMatrix) {mNormalMatrix = aNormalMatrix; return *this;}
    QuadInstanceData&                   color(const ci::ColorAf& aColor) {mColor = aColor; return *this;}
    QuadInstanceData&                   materialId(int aMaterialId) {mMaterialId = aMaterialId; return *this;}
    QuadInstanceData&                   texCoordOffset(const ci::vec2& aOffset) {mTexCoordOffset = aOffset; return *this;}
    QuadInstanceData&                   texCoordSize(const ci::vec2& aSize) {mTexCoordSize = aSize; return *this;}
    void                                setModelMatrix(const ci::mat4& aModelMatrix) {mModelMatrix = aModelMatrix;}
    void                                setNormalMatrix(const ci::mat3& aNormalMatrix) {mNormalMatrix = aNormalMatrix;}
    void                                setColor(const ci::ColorAf& aColor) {mColor = aColor;}
    void                                setMaterialId(int aMaterialId) {mMaterialId = aMaterialId;}
    void                                setTexCoordOffset(const ci::vec2& aOffset) {mTexCoordOffset = aOffset;}
    void                                setTexCoordSize(const ci::vec2& aSize) {mTexCoordSize = aSize;}
    const ci::mat4&                     getModelMatrix() const {return mModelMatrix;}
    ci::mat4                            getModelMatrix() {return mModelMatrix;}
    const ci::mat3&                     getNormalMatrix() const {return mNormalMatrix;}
    ci::mat3                            getNormalMatrix() {return mNormalMatrix;}
    const ci::ColorAf&                  getColor() const {return mColor;}
    ci::ColorAf                         getColor() {return mColor;}
    int                                 getMaterialId() const {return mMaterialId;}
    int                                 getMaterialId() {return mMaterialId;}
    const ci::vec2&                     getTexCoordOffset() const {return mTexCoordOffset;}
    ci::vec2                            getTexCoordOffset() {return mTexCoordOffset;}
    const ci::vec2&                     getTexCoordSize() const {return mTexCoordSize;}
    ci::vec2                            getTexCoordSize() {return mTexCoordSize;}
    
public:
    ci::mat4                            mModelMatrix;
    ci::mat3                            mNormalMatrix;
    ci::ColorAf                         mColor;
    int                                 mMaterialId;
    ci::vec2                            mTexCoordOffset;
    ci::vec2                            mTexCoordSize;
};

class QuadInstance : public virtual Object3D
{
public:
    static QuadInstanceRef create(){
        return QuadInstanceRef ( new QuadInstance());
    }
    
    QuadInstance();
    ~QuadInstance();
    OBJECT3D_CLONE(QuadInstance)
    
    void                            updateSelf() override;
    
    
    //setters/getters
    QuadInstance&                   color(const ci::ColorAf& aColor) {mColor = aColor; return *this;}
    QuadInstance&                   material(const MaterialRef& aMaterial) {mMaterial = aMaterial; return *this;}
    QuadInstance&                   instanceData(const QuadInstanceData& aInstanceData) {mInstanceData = aInstanceData; return *this;}
    void                            setColor(const ci::ColorAf& aColor) {mColor = aColor;}
    void                            setMaterial(const MaterialRef& aMaterial) {mMaterial = aMaterial;}
    void                            setInstanceData(const QuadInstanceData& aInstanceData) {mInstanceData = aInstanceData;}
    void                            setTexCoordSize(const ci::vec2& aSize) {mTexCoordSize = aSize;}
    void                            setTexCoordOffset(const ci::vec2& aOffset) {mTexCoordOffset = aOffset;}
    const MaterialRef&              getMaterial() const {return mMaterial;}
    MaterialRef                     getMaterial() {return mMaterial;}
    const ci::ColorAf&              getColor() const {return mColor;}
    ci::ColorAf                     getColor() {return mColor;}
    const ci::vec2&                 getTexCoordOffset() const {return mTexCoordOffset;}
    ci::vec2                        getTexCoordOffset() {return mTexCoordOffset;}
    const ci::vec2&                 getTexCoordSize() const {return mTexCoordSize;}
    ci::vec2                        getTexCoordSize() {return mTexCoordSize;}
    const QuadInstanceData&         getInstanceData() const {return mInstanceData;}
    QuadInstanceData                getInstanceData() {return mInstanceData;}
    
    
public:
    MaterialRef                     mMaterial;//this will be the mirror of the desired material for this instance, only the materialId is used atm
    ci::ColorAf                     mColor; // this will be the mirror of the color in InstanceData
    ci::vec2                        mTexCoordOffset;
    ci::vec2                        mTexCoordSize;
    
    QuadInstanceData                mInstanceData;
};

class QuadInstancer : public virtual Object3D
{
public:
    static QuadInstancerRef create(){
        return QuadInstancerRef ( new QuadInstancer());
    }
    
    static QuadInstancerRef create(int aMaxCapacity){
        return QuadInstancerRef ( new QuadInstancer(aMaxCapacity));
    }
    
    QuadInstancer();
    QuadInstancer(int aMaxCapacity);
    ~QuadInstancer();
    OBJECT3D_CLONE(QuadInstancer)
    
    void                            updateSelf() override;
    void                            drawSelf() override;
    virtual void                    setup();
    virtual void                    setup(int aMaxCapacity);
    void                            addInstance(const QuadInstanceRef& aInstance);
    void                            removeInstance(const QuadInstanceRef& aInstance);
    std::vector<InstanceRef>        getInstances() const;
    
    //setters/getters
    QuadInstancer&                  maxCapacity(int aMaxCapacity) {mMaxCapacity = aMaxCapacity; return *this;}
    QuadInstancer&                  referenceMesh(const MeshRef& aReferenceMesh) {mReferenceMesh = aReferenceMesh; return *this;}
    QuadInstancer&                  dataVbo(const ci::gl::VboRef& aDataVbo) {mDataVbo = aDataVbo; return *this;}
    QuadInstancer&                  instancerBatch(const ci::gl::BatchRef& aInstancerBatch) {mInstancerBatch = aInstancerBatch; return *this;}
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

#endif /* defined(__a_Init__QuadInstancer__) */
