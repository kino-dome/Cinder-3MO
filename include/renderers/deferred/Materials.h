//
//  Materials.h
//  DeferredRenderer
//
//  Created by Hesam Ohadi on 7/12/15.
//
//

#include "../../core/Material.h"


typedef std::shared_ptr<class MaterialGBuffer> MaterialGBufferRef;
class MaterialGBuffer : public ClassicMaterial
{
public:
    
    static MaterialGBufferRef create(){
        return MaterialGBufferRef (new MaterialGBuffer());
    }
    
    MaterialGBuffer();
    
    ~MaterialGBuffer();
    
    void                        setup();
    void                        attachAndDraw(const MeshRef& aMesh) override;
    //MaterialRef                 cloneSelf() override;
    MATERIAL_CLONE(MaterialGBuffer)
    
public:
    bool                                            mIsMaterialIdEnabled; //default to true
    bool                                            mIsAlbedoEnabled; //default to true - will include diffuse map
    bool                                            mIsNormalEnabled; //default to true - will include normal map
    bool                                            mIsPositionEnabled; //default to false - will include displacement map
    bool                                            mIsTexCoordEnabled; //default to false
    bool                                            mIsSpecularEnabled; //default to false - will include specular map
    bool                                            mIsEmissionEnabled; //default to false - will include emission map
    bool                                            mIsOcclusionEnabled; //default to false - will include occlusion map
    int                                             mMaxNumDirectionalLights, mMaxNumPointLights, mMaxNumSpotLights, mMaxNumPbrMaterials, mMaxNumClassicMaterials;
    int                                             mAlbedoLayoutIndex, mNormalLayoutIndex, mPositionLayoutIndex, mMaterialIdLayoutIndex, mTexCoordLayoutIndex, mSpecularLayoutIndex, mEmissionLayoutIndex, mOcclusionLayoutIndex, mDisplacementLayoutIndex;
    int                                             mClassicMaterialsUboIndex, mPbrMaterialsUboIndex;
    bool                                            mHasClassicMaterials, mHasPbrMaterials;
};


typedef std::shared_ptr<class MaterialGBufferInstancer> MaterialGBufferInstancerRef;
class MaterialGBufferInstancer : public ClassicMaterial
{
public:
    
    static MaterialGBufferInstancerRef create(){
        return MaterialGBufferInstancerRef (new MaterialGBufferInstancer());
    }
    
    MaterialGBufferInstancer();
    
    ~MaterialGBufferInstancer();
    
    void                        setup();
    void                        attachAndDraw(const MeshRef& aMesh) override;
    //MaterialRef                 cloneSelf() override;
    MATERIAL_CLONE(MaterialGBufferInstancer)
    
public:
    int                                             mNumInstances, mMaxNumInstances;
    bool                                            mIsMaterialIdEnabled; //default to true
    bool                                            mIsAlbedoEnabled; //default to true - will include diffuse map
    bool                                            mIsNormalEnabled; //default to true - will include normal map
    bool                                            mIsPositionEnabled; //default to false - will include displacement map
    bool                                            mIsTexCoordEnabled; //default to false
    bool                                            mIsSpecularEnabled; //default to false - will include specular map
    bool                                            mIsEmissionEnabled; //default to false - will include emission map
    bool                                            mIsOcclusionEnabled; //default to false - will include occlusion map
    int                                             mMaxNumDirectionalLights, mMaxNumPointLights, mMaxNumSpotLights, mMaxNumPbrMaterials, mMaxNumClassicMaterials;
    int                                             mAlbedoLayoutIndex, mNormalLayoutIndex, mPositionLayoutIndex, mMaterialIdLayoutIndex, mTexCoordLayoutIndex, mSpecularLayoutIndex, mEmissionLayoutIndex, mOcclusionLayoutIndex, mDisplacementLayoutIndex;
    int                                             mClassicMaterialsUboIndex, mPbrMaterialsUboIndex;
    bool                                            mHasClassicMaterials, mHasPbrMaterials;
};

typedef std::shared_ptr<class MaterialGBufferQuadInstancer> MaterialGBufferQuadInstancerRef;
class MaterialGBufferQuadInstancer : public ClassicMaterial
{
public:
    
    static MaterialGBufferQuadInstancerRef create(){
        return MaterialGBufferQuadInstancerRef (new MaterialGBufferQuadInstancer());
    }
    
    MaterialGBufferQuadInstancer();
    
    ~MaterialGBufferQuadInstancer();
    
    void                        setup();
    void                        attachAndDraw(const MeshRef& aMesh) override;
    //MaterialRef                 cloneSelf() override;
    MATERIAL_CLONE(MaterialGBufferQuadInstancer)
    
public:
    int                                             mNumInstances, mMaxNumInstances;
    bool                                            mIsMaterialIdEnabled; //default to true
    bool                                            mIsAlbedoEnabled; //default to true - will include diffuse map
    bool                                            mIsNormalEnabled; //default to true - will include normal map
    bool                                            mIsPositionEnabled; //default to false - will include displacement map
    bool                                            mIsTexCoordEnabled; //default to false
    bool                                            mIsSpecularEnabled; //default to false - will include specular map
    bool                                            mIsEmissionEnabled; //default to false - will include emission map
    bool                                            mIsOcclusionEnabled; //default to false - will include occlusion map
    int                                             mMaxNumDirectionalLights, mMaxNumPointLights, mMaxNumSpotLights, mMaxNumPbrMaterials, mMaxNumClassicMaterials;
    int                                             mAlbedoLayoutIndex, mNormalLayoutIndex, mPositionLayoutIndex, mMaterialIdLayoutIndex, mTexCoordLayoutIndex, mSpecularLayoutIndex, mEmissionLayoutIndex, mOcclusionLayoutIndex, mDisplacementLayoutIndex;
    int                                             mClassicMaterialsUboIndex, mPbrMaterialsUboIndex;
    bool                                            mHasClassicMaterials, mHasPbrMaterials;
};