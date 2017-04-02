//
//  Materials.cpp
//  DeferredRenderer
//
//  Created by Hesam Ohadi on 7/12/15.
//
//

#include "cinder/Utilities.h"
#include "cinder/Log.h"

#include "Materials.h"


using namespace ci;
using namespace std;
using namespace ci::app;

//////////////////MaterialGBuffer//////////////////
MaterialGBuffer::MaterialGBuffer() : ClassicMaterial()
{
    mType = "material_gbuffer";
}

MaterialGBuffer::~MaterialGBuffer()
{
    
}

void MaterialGBuffer::setup()
{
    auto loadGlslProg = [ & ]( const ci::gl::GlslProg::Format& format ) -> ci::gl::GlslProgRef
    {
        std::string names = format.getVertexPath().string() + " + " +
        format.getFragmentPath().string();
        ci::gl::GlslProgRef glslProg;
        try {
            glslProg = ci::gl::GlslProg::create( format );
        } catch ( const ci::Exception& ex ) {
            CI_LOG_EXCEPTION( names, ex );
        }
        return glslProg;
    };
    
    ci::DataSourceRef fragDeferredGBuffer = ci::app::loadAsset( "deferred/gbuffer.frag" );
    ci::DataSourceRef vertDeferredGBuffer = ci::app::loadAsset( "deferred/gbuffer.vert" );
    auto gbufferFormat = ci::gl::GlslProg::Format().version( 330 )
    .vertex( vertDeferredGBuffer ).fragment( fragDeferredGBuffer ).define("MAX_NUM_DIRECTIONAL_LIGHTS", ci::toString(mMaxNumDirectionalLights)).define("MAX_NUM_POINT_LIGHTS", ci::toString(mMaxNumPointLights)).define("MAX_NUM_SPOT_LIGHTS", ci::toString(mMaxNumSpotLights)).define("MAX_NUM_CLASSIC_MATERIALS", ci::toString(mMaxNumClassicMaterials)).define("MAX_NUM_PBR_MATERIALS", ci::toString(mMaxNumPbrMaterials));
    if (mIsAlbedoEnabled) {
        gbufferFormat.define("HAS_ALBEDO");
        gbufferFormat.define("ALBEDO_INDEX", ci::toString(mAlbedoLayoutIndex));
    }
    if (mIsMaterialIdEnabled) {
        gbufferFormat.define("HAS_MATERIAL_ID");
        gbufferFormat.define("MATERIAL_ID_INDEX", ci::toString(mMaterialIdLayoutIndex));
    }
    if (mIsNormalEnabled){
        gbufferFormat.define("HAS_NORMAL");
        gbufferFormat.define("NORMAL_INDEX", ci::toString(mNormalLayoutIndex));
    }
    if (mIsPositionEnabled){
        gbufferFormat.define("HAS_POSITION");
        gbufferFormat.define("POSITION_INDEX", ci::toString(mPositionLayoutIndex));
    }
    if (mIsSpecularEnabled){
        gbufferFormat.define("HAS_SPECULAR");
        gbufferFormat.define("SPECULAR_INDEX", ci::toString(mSpecularLayoutIndex));
    }
    if (mIsTexCoordEnabled){
        gbufferFormat.define("HAS_TEX_COORD");
        gbufferFormat.define("TEX_COORD_INDEX", ci::toString(mTexCoordLayoutIndex));
    }
    if (mIsEmissionEnabled){
        gbufferFormat.define("HAS_EMISSION");
        gbufferFormat.define("EMISSION_INDEX", ci::toString(mEmissionLayoutIndex));
    }
    if (mIsOcclusionEnabled){
        gbufferFormat.define("HAS_OCCLUSION");
        gbufferFormat.define("OCCLUSION_INDEX", ci::toString(mOcclusionLayoutIndex));
    }
    if (mHasClassicMaterials){
        gbufferFormat.define("HAS_CLASSIC_MATERIALS");
    }
    if (mHasPbrMaterials){
        gbufferFormat.define("HAS_PBR_MATERIALS");
    }
    mShaderFormat = gbufferFormat;
    mShader = loadGlslProg(mShaderFormat);
    
    if (mIsAlbedoEnabled){
        mShader->uniform("uDiffuseMap", mAlbedoLayoutIndex);
    }
    
    if (mIsNormalEnabled){
        mShader->uniform("uNormalMap", mNormalLayoutIndex);
    }
    
    if (mIsSpecularEnabled){
        mShader->uniform("uSpecularMap", mSpecularLayoutIndex);
    }
    
    if (mIsEmissionEnabled){
        mShader->uniform("uEmissionMap", mEmissionLayoutIndex);
    }
    
    if (mIsOcclusionEnabled){
        mShader->uniform("uOcclusionMap", mOcclusionLayoutIndex);
    }
    
    mDisplacementLayoutIndex = 11;//could be anything other than the locations for the other maps, 11 to be on the safe side
    mShader->uniform("uDisplacementMap", mDisplacementLayoutIndex);
    
    //tell the shader about the UBOs locations
    if (mHasClassicMaterials){
        mShader->uniformBlock("ClassicMaterials", mClassicMaterialsUboIndex);
        ci::app::console()<<mClassicMaterialsUboIndex<<std::endl;
    }
    if (mHasPbrMaterials){
        mShader->uniformBlock("PbrMaterials", mPbrMaterialsUboIndex);
    }
}

void MaterialGBuffer::attachAndDraw(const MeshRef& aMesh)
{
    ci::gl::BatchRef batch = aMesh->getBatch();
    if (batch->getGlslProg() != mShader){
        batch->replaceGlslProg(mShader);
    }
    
    if (mDisplacementMap){
        mDisplacementMap->bind(mShader->getUniformLocation("uDisplacementMap"));
        mShader->uniform("uHasDisplacementMap", 1);
    }
    
    if (mIsAlbedoEnabled){
        if (mDiffuseMap){
            mDiffuseMap->bind(mAlbedoLayoutIndex);
            mShader->uniform("uHasDiffuseMap", 1);
        }
        else {
            mShader->uniform("uHasDiffuseMap", 0);
        }
    }
    
    if (mIsNormalEnabled){
        if (mNormalMap){
            mNormalMap->bind(mNormalLayoutIndex);
            mShader->uniform("uHasNormalMap", 1);
        }
        else {
            mShader->uniform("uHasNormalMap", 0);
        }
    }
    
    if (mIsSpecularEnabled){
        if (mSpecularMap){
            mSpecularMap->bind(mSpecularLayoutIndex);
            mShader->uniform("uHasSpecularMap", 1);
        }
        else{
            mShader->uniform("uHasSpecularMap", 0);
        }
    }
    
    if (mIsEmissionEnabled){
        if (mEmissionMap){
            mEmissionMap->bind(mEmissionLayoutIndex);
            mShader->uniform("uHasEmissionMap", 1);
        }
        else{
            mShader->uniform("uHasEmissionMap", 0);
        }
    }
    
    if (mIsOcclusionEnabled){
        if (mOcclusionMap){
            mOcclusionMap->bind(mOcclusionLayoutIndex);
            mShader->uniform("uHasOcclusionMap", 1);
        }
        else {
            mShader->uniform("uHasOcclusionMap", 0);
        }
    }
//    if (mIsMaterialIdEnabled){
//        
//    }
    mShader->uniform("uMaterialId", getId());
    
    batch->draw();
    
    if (mDisplacementMap){
        mDisplacementMap->unbind();
    }
    if (mIsAlbedoEnabled && mDiffuseMap) mDiffuseMap->unbind();
    if (mIsNormalEnabled && mNormalMap) mNormalMap->unbind();
    if (mIsSpecularEnabled && mSpecularLayoutIndex) mSpecularMap->unbind();
    if (mIsEmissionEnabled && mEmissionMap) mEmissionMap->unbind();
    if (mIsOcclusionEnabled && mOcclusionMap) mOcclusionMap->unbind();
}


////////////////////////MaterialGBufferInstancer////////////////
MaterialGBufferInstancer::MaterialGBufferInstancer() : ClassicMaterial()
{
    mType = "material_gbuffer_instancer";
    mNumInstances=0;
    mMaxNumInstances=5555;
}

MaterialGBufferInstancer::~MaterialGBufferInstancer()
{
    
}

void MaterialGBufferInstancer::setup()
{
    auto loadGlslProg = [ & ]( const ci::gl::GlslProg::Format& format ) -> ci::gl::GlslProgRef
    {
        std::string names = format.getVertexPath().string() + " + " +
        format.getFragmentPath().string();
        ci::gl::GlslProgRef glslProg;
        try {
            glslProg = ci::gl::GlslProg::create( format );
        } catch ( const ci::Exception& ex ) {
            CI_LOG_EXCEPTION( names, ex );
        }
        return glslProg;
    };
    
    ci::DataSourceRef fragDeferredGBuffer = ci::app::loadAsset( "deferred/gbuffer.frag" );
    ci::DataSourceRef vertDeferredGBuffer = ci::app::loadAsset( "deferred/gbuffer.vert" );
    auto gbufferFormat = ci::gl::GlslProg::Format().version( 330 )
    .vertex( vertDeferredGBuffer ).fragment( fragDeferredGBuffer ).define("HAS_INSTANCING").define("MAX_NUM_DIRECTIONAL_LIGHTS", ci::toString(mMaxNumDirectionalLights)).define("MAX_NUM_POINT_LIGHTS", ci::toString(mMaxNumPointLights)).define("MAX_NUM_SPOT_LIGHTS", ci::toString(mMaxNumSpotLights)).define("MAX_NUM_CLASSIC_MATERIALS", ci::toString(mMaxNumClassicMaterials)).define("MAX_NUM_PBR_MATERIALS", ci::toString(mMaxNumPbrMaterials)).attrib(ci::geom::Attrib::CUSTOM_0, "uInstanceModelMatrix").attrib(ci::geom::Attrib::CUSTOM_1, "uInstanceNormalMatrix").attrib(ci::geom::Attrib::CUSTOM_2, "uInstanceColor").attrib(ci::geom::Attrib::CUSTOM_3, "uInstanceMaterialId");
    if (mIsAlbedoEnabled) {
        gbufferFormat.define("HAS_ALBEDO");
        gbufferFormat.define("ALBEDO_INDEX", ci::toString(mAlbedoLayoutIndex));
    }
    if (mIsMaterialIdEnabled) {
        gbufferFormat.define("HAS_MATERIAL_ID");
        gbufferFormat.define("MATERIAL_ID_INDEX", ci::toString(mMaterialIdLayoutIndex));
    }
    if (mIsNormalEnabled){
        gbufferFormat.define("HAS_NORMAL");
        gbufferFormat.define("NORMAL_INDEX", ci::toString(mNormalLayoutIndex));
    }
    if (mIsPositionEnabled){
        gbufferFormat.define("HAS_POSITION");
        gbufferFormat.define("POSITION_INDEX", ci::toString(mPositionLayoutIndex));
    }
    if (mIsSpecularEnabled){
        gbufferFormat.define("HAS_SPECULAR");
        gbufferFormat.define("SPECULAR_INDEX", ci::toString(mSpecularLayoutIndex));
    }
    if (mIsTexCoordEnabled){
        gbufferFormat.define("HAS_TEX_COORD");
        gbufferFormat.define("TEX_COORD_INDEX", ci::toString(mTexCoordLayoutIndex));
    }
    if (mIsEmissionEnabled){
        gbufferFormat.define("HAS_EMISSION");
        gbufferFormat.define("EMISSION_INDEX", ci::toString(mEmissionLayoutIndex));
    }
    if (mIsOcclusionEnabled){
        gbufferFormat.define("HAS_OCCLUSION");
        gbufferFormat.define("OCCLUSION_INDEX", ci::toString(mOcclusionLayoutIndex));
    }
    if (mHasClassicMaterials){
        gbufferFormat.define("HAS_CLASSIC_MATERIALS");
    }
    if (mHasPbrMaterials){
        gbufferFormat.define("HAS_PBR_MATERIALS");
    }
    mShaderFormat = gbufferFormat;
    mShader = loadGlslProg(mShaderFormat);
    
    if (mIsAlbedoEnabled){
        mShader->uniform("uDiffuseMap", mAlbedoLayoutIndex);
    }
    
    if (mIsNormalEnabled){
        mShader->uniform("uNormalMap", mNormalLayoutIndex);
    }
    
    if (mIsSpecularEnabled){
        mShader->uniform("uSpecularMap", mSpecularLayoutIndex);
    }
    
    if (mIsEmissionEnabled){
        mShader->uniform("uEmissionMap", mEmissionLayoutIndex);
    }
    
    if (mIsOcclusionEnabled){
        mShader->uniform("uOcclusionMap", mOcclusionLayoutIndex);
    }
    
    mDisplacementLayoutIndex = 11;//could be anything other than the locations for the other maps, 11 to be on the safe side
    mShader->uniform("uDisplacementMap", mDisplacementLayoutIndex);
    
    //tell the shader about the UBOs locations
    if (mHasClassicMaterials){
        mShader->uniformBlock("ClassicMaterials", mClassicMaterialsUboIndex);
    }
    if (mHasPbrMaterials){
        mShader->uniformBlock("PbrMaterials", mPbrMaterialsUboIndex);
    }
}

void MaterialGBufferInstancer::attachAndDraw(const MeshRef& aMesh)
{
    ci::gl::BatchRef batch = aMesh->getBatch();
    if (batch->getGlslProg() != mShader){
        batch->replaceGlslProg(mShader);
    }
    
    if (mDisplacementMap){
        mDisplacementMap->bind(mShader->getUniformLocation("uDisplacementMap"));
        mShader->uniform("uHasDisplacementMap", 1);
    }
    
    if (mIsAlbedoEnabled){
        if (mDiffuseMap){
            mDiffuseMap->bind(mAlbedoLayoutIndex);
            mShader->uniform("uHasDiffuseMap", 1);
        }
        else {
            mShader->uniform("uHasDiffuseMap", 0);
        }
    }
    
    if (mIsNormalEnabled){
        if (mNormalMap){
            mNormalMap->bind(mNormalLayoutIndex);
            mShader->uniform("uHasNormalMap", 1);
        }
        else {
            mShader->uniform("uHasNormalMap", 0);
        }
    }
    
    if (mIsSpecularEnabled){
        if (mSpecularMap){
            mSpecularMap->bind(mSpecularLayoutIndex);
            mShader->uniform("uHasSpecularMap", 1);
        }
        else{
            mShader->uniform("uHasSpecularMap", 0);
        }
    }
    
    if (mIsEmissionEnabled){
        if (mEmissionMap){
            mEmissionMap->bind(mEmissionLayoutIndex);
            mShader->uniform("uHasEmissionMap", 1);
        }
        else{
            mShader->uniform("uHasEmissionMap", 0);
        }
    }
    
    if (mIsOcclusionEnabled){
        if (mOcclusionMap){
            mOcclusionMap->bind(mOcclusionLayoutIndex);
            mShader->uniform("uHasOcclusionMap", 1);
        }
        else {
            mShader->uniform("uHasOcclusionMap", 0);
        }
    }
    //    if (mIsMaterialIdEnabled){
    //
    //    }
    
    batch->drawInstanced(mNumInstances);
    
    if (mDisplacementMap){
        mDisplacementMap->unbind();
    }
    if (mIsAlbedoEnabled && mDiffuseMap) mDiffuseMap->unbind();
    if (mIsNormalEnabled && mNormalMap) mNormalMap->unbind();
    if (mIsSpecularEnabled && mSpecularLayoutIndex) mSpecularMap->unbind();
    if (mIsEmissionEnabled && mEmissionMap) mEmissionMap->unbind();
    if (mIsOcclusionEnabled && mOcclusionMap) mOcclusionMap->unbind();
}

////////////////////////MaterialGBufferQuadInstancer////////////////
MaterialGBufferQuadInstancer::MaterialGBufferQuadInstancer() : ClassicMaterial()
{
    mType = "material_gbuffer_instancer";
    mNumInstances=0;
    mMaxNumInstances=5555;
}

MaterialGBufferQuadInstancer::~MaterialGBufferQuadInstancer()
{
    
}

void MaterialGBufferQuadInstancer::setup()
{
    auto loadGlslProg = [ & ]( const ci::gl::GlslProg::Format& format ) -> ci::gl::GlslProgRef
    {
        std::string names = format.getVertexPath().string() + " + " +
        format.getFragmentPath().string();
        ci::gl::GlslProgRef glslProg;
        try {
            glslProg = ci::gl::GlslProg::create( format );
        } catch ( const ci::Exception& ex ) {
            CI_LOG_EXCEPTION( names, ex );
        }
        return glslProg;
    };
    
    ci::DataSourceRef fragDeferredGBuffer = ci::app::loadAsset( "deferred/gbuffer.frag" );
    ci::DataSourceRef vertDeferredGBuffer = ci::app::loadAsset( "deferred/gbuffer.vert" );
    auto gbufferFormat = ci::gl::GlslProg::Format().version( 330 )
    .vertex( vertDeferredGBuffer ).fragment( fragDeferredGBuffer ).define("HAS_INSTANCING").define("HAS_QUAD_INSTANCING").define("MAX_NUM_DIRECTIONAL_LIGHTS", ci::toString(mMaxNumDirectionalLights)).define("MAX_NUM_POINT_LIGHTS", ci::toString(mMaxNumPointLights)).define("MAX_NUM_SPOT_LIGHTS", ci::toString(mMaxNumSpotLights)).define("MAX_NUM_CLASSIC_MATERIALS", ci::toString(mMaxNumClassicMaterials)).define("MAX_NUM_PBR_MATERIALS", ci::toString(mMaxNumPbrMaterials)).attrib(ci::geom::Attrib::CUSTOM_0, "uInstanceModelMatrix").attrib(ci::geom::Attrib::CUSTOM_1, "uInstanceNormalMatrix").attrib(ci::geom::Attrib::CUSTOM_2, "uInstanceColor").attrib(ci::geom::Attrib::CUSTOM_3, "uInstanceMaterialId").attrib(geom::Attrib::CUSTOM_4, "uInstanceTexCoordOffset").attrib(geom::Attrib::CUSTOM_5, "uInstanceTexCoordSize");
    if (mIsAlbedoEnabled) {
        gbufferFormat.define("HAS_ALBEDO");
        gbufferFormat.define("ALBEDO_INDEX", ci::toString(mAlbedoLayoutIndex));
    }
    if (mIsMaterialIdEnabled) {
        gbufferFormat.define("HAS_MATERIAL_ID");
        gbufferFormat.define("MATERIAL_ID_INDEX", ci::toString(mMaterialIdLayoutIndex));
    }
    if (mIsNormalEnabled){
        gbufferFormat.define("HAS_NORMAL");
        gbufferFormat.define("NORMAL_INDEX", ci::toString(mNormalLayoutIndex));
    }
    if (mIsPositionEnabled){
        gbufferFormat.define("HAS_POSITION");
        gbufferFormat.define("POSITION_INDEX", ci::toString(mPositionLayoutIndex));
    }
    if (mIsSpecularEnabled){
        gbufferFormat.define("HAS_SPECULAR");
        gbufferFormat.define("SPECULAR_INDEX", ci::toString(mSpecularLayoutIndex));
    }
    if (mIsTexCoordEnabled){
        gbufferFormat.define("HAS_TEX_COORD");
        gbufferFormat.define("TEX_COORD_INDEX", ci::toString(mTexCoordLayoutIndex));
    }
    if (mIsEmissionEnabled){
        gbufferFormat.define("HAS_EMISSION");
        gbufferFormat.define("EMISSION_INDEX", ci::toString(mEmissionLayoutIndex));
    }
    if (mIsOcclusionEnabled){
        gbufferFormat.define("HAS_OCCLUSION");
        gbufferFormat.define("OCCLUSION_INDEX", ci::toString(mOcclusionLayoutIndex));
    }
    if (mHasClassicMaterials){
        gbufferFormat.define("HAS_CLASSIC_MATERIALS");
    }
    if (mHasPbrMaterials){
        gbufferFormat.define("HAS_PBR_MATERIALS");
    }
    mShaderFormat = gbufferFormat;
    mShader = loadGlslProg(mShaderFormat);
    
    if (mIsAlbedoEnabled){
        mShader->uniform("uDiffuseMap", mAlbedoLayoutIndex);
    }
    
    if (mIsNormalEnabled){
        mShader->uniform("uNormalMap", mNormalLayoutIndex);
    }
    
    if (mIsSpecularEnabled){
        mShader->uniform("uSpecularMap", mSpecularLayoutIndex);
    }
    
    if (mIsEmissionEnabled){
        mShader->uniform("uEmissionMap", mEmissionLayoutIndex);
    }
    
    if (mIsOcclusionEnabled){
        mShader->uniform("uOcclusionMap", mOcclusionLayoutIndex);
    }
    
    mDisplacementLayoutIndex = 11;//could be anything other than the locations for the other maps, 11 to be on the safe side
    mShader->uniform("uDisplacementMap", mDisplacementLayoutIndex);
    
    //tell the shader about the UBOs locations
    if (mHasClassicMaterials){
        mShader->uniformBlock("ClassicMaterials", mClassicMaterialsUboIndex);
    }
    if (mHasPbrMaterials){
        mShader->uniformBlock("PbrMaterials", mPbrMaterialsUboIndex);
    }
}

void MaterialGBufferQuadInstancer::attachAndDraw(const MeshRef& aMesh)
{
    ci::gl::BatchRef batch = aMesh->getBatch();
    if (batch->getGlslProg() != mShader){
        batch->replaceGlslProg(mShader);
    }
    
    if (mDisplacementMap){
        mDisplacementMap->bind(mShader->getUniformLocation("uDisplacementMap"));
        mShader->uniform("uHasDisplacementMap", 1);
    }
    
    if (mIsAlbedoEnabled){
        if (mDiffuseMap){
            mDiffuseMap->bind(mAlbedoLayoutIndex);
            mShader->uniform("uHasDiffuseMap", 1);
        }
        else {
            mShader->uniform("uHasDiffuseMap", 0);
        }
    }
    
    if (mIsNormalEnabled){
        if (mNormalMap){
            mNormalMap->bind(mNormalLayoutIndex);
            mShader->uniform("uHasNormalMap", 1);
        }
        else {
            mShader->uniform("uHasNormalMap", 0);
        }
    }
    
    if (mIsSpecularEnabled){
        if (mSpecularMap){
            mSpecularMap->bind(mSpecularLayoutIndex);
            mShader->uniform("uHasSpecularMap", 1);
        }
        else{
            mShader->uniform("uHasSpecularMap", 0);
        }
    }
    
    if (mIsEmissionEnabled){
        if (mEmissionMap){
            mEmissionMap->bind(mEmissionLayoutIndex);
            mShader->uniform("uHasEmissionMap", 1);
        }
        else{
            mShader->uniform("uHasEmissionMap", 0);
        }
    }
    
    if (mIsOcclusionEnabled){
        if (mOcclusionMap){
            mOcclusionMap->bind(mOcclusionLayoutIndex);
            mShader->uniform("uHasOcclusionMap", 1);
        }
        else {
            mShader->uniform("uHasOcclusionMap", 0);
        }
    }
    //    if (mIsMaterialIdEnabled){
    //
    //    }
    
    batch->drawInstanced(mNumInstances);
    
    if (mDisplacementMap){
        mDisplacementMap->unbind();
    }
    if (mIsAlbedoEnabled && mDiffuseMap) mDiffuseMap->unbind();
    if (mIsNormalEnabled && mNormalMap) mNormalMap->unbind();
    if (mIsSpecularEnabled && mSpecularLayoutIndex) mSpecularMap->unbind();
    if (mIsEmissionEnabled && mEmissionMap) mEmissionMap->unbind();
    if (mIsOcclusionEnabled && mOcclusionMap) mOcclusionMap->unbind();
}