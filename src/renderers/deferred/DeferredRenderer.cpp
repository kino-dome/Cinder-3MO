//
//  DeferredRenderer.cpp
//  DeferredRenderer
//
//  Created by Hesam Ohadi on 5/12/15.
//
//

#include "DeferredRenderer.h"

#include "cinder/Log.h"
#include "cinder/Utilities.h"
#include "SceneData.h"

using namespace ci;
using namespace std;
using namespace ci::app;


/////////////////DeferredRenderer/////////////////

DeferredRenderer::DeferredRenderer() : RendererBase()
{
    mType = "deferred_renderer";
    mIsDepthEnabled = true;
    mIsMaterialIdEnabled = true;
    mIsAlbedoEnabled = true;
    mIsNormalEnabled = true;
    mIsPositionEnabled = false;
    mIsTexCoordEnabled = false;
    mIsSpecularEnabled = false;
    mIsEmissionEnabled = false;
    mIsOcclusionEnabled = false;
    
    mIsDebugEnabled = false;
    mIsFxaaEnabled = false;
    mIsAoEnabled = false;
    
    mAo = Ao_None;
}

DeferredRenderer::~DeferredRenderer()
{
    mResizeConnection.disconnect();
}

void DeferredRenderer::setup()
{
    RendererBase::setup();
    resize();
    
    mResizeConnection = mParentWindow->getSignalResize().connect(std::bind(&DeferredRenderer::resize, this));
}

void DeferredRenderer::setup(const ci::app::WindowRef& aParentWindow)
{
    RendererBase::setup(aParentWindow);
    setup();
}


void DeferredRenderer::update()
{
    for (auto& sceneData : mSceneDataList){
        if (sceneData.getIsBinded()){
            sceneData.update();
        }
    }
}

void DeferredRenderer::draw()
{
    auto activeCamera = mActiveScene->getActiveCamera();
    if ( activeCamera->hasTag("perspective_camera")){
        draw(mActiveScene, activeCamera->cast<PerspectiveCamera>());
    }
    else if (activeCamera->hasTag("orthographic_camera")){
        draw(mActiveScene, activeCamera->cast<OrthographicCamera>());
    }
    else if (activeCamera->hasTag("stereo_camera")){
        draw(mActiveScene, activeCamera->cast<StereoCamera>());
    }
}


void DeferredRenderer::draw(const SceneRef& aScene)
{
    setActiveScene(aScene);
    draw();
    
}

void DeferredRenderer::draw(const SceneRef &aScene, const PerspectiveCameraRef &aPerspectiveCamera)
{
    aScene->setActiveCamera(aPerspectiveCamera);
    setActiveScene(aScene);
    
    const float f					= aPerspectiveCamera->getCinderCamera().getFarClip();
    const float n					= aPerspectiveCamera->getCinderCamera().getNearClip();
    const mat4 projMatrixInverse	= glm::inverse( aPerspectiveCamera->getCinderCamera().getProjectionMatrix() );

    
    //////////////////////////////////////////////////////////////////////////////////////////////
    /* G-BUFFER
     *
     * The geometry buffer, or G-buffer, captures our 3D scene's data in 2D screen space.
     * A G-buffer can store pretty much anything you want. Position, normal, color, velocity,
     * material, luminance data. You name it. However, it's best to keep this information to
     * a minimum to improve performance. Our G-buffer stores depth, normals encoded to 8-bit
     * values in two channels, and material IDs. We also render everything with instancing to
     * keep draw calls to a minimum.
     *
     * "unpack.glsl" contains methods for decoding normals and calculating 3D positions from
     * depth and camera data. The material ID represents the index of a material in our
     * UBO. This allows models to access information for diffuse, specular, shininess, etc
     * values without having to store them in a texture.
     */
    
    {
        const gl::ScopedFramebuffer scopedFrameBuffer( mFboGBuffer );
        GLenum buffers[mTextureFboGBufferVector.size()];
        for (size_t i=0; i<mTextureFboGBufferVector.size(); ++i){
            buffers[i] = GL_COLOR_ATTACHMENT0 + (GLenum)i;
        }
        gl::drawBuffers( mTextureFboGBufferVector.size(), buffers );
        const gl::ScopedViewport scopedViewport( ivec2( 0 ), mFboGBuffer->getSize() );
        gl::clear(ColorAf(0.0, 0.0, 0.0, 0.0));
        const gl::ScopedMatrices scopedMatrices;
        gl::setMatrices(aPerspectiveCamera->getCinderCamera());
        gl::enableDepthRead();
        gl::enableDepthWrite();
        
        ////// BEGIN DRAW STUFF ////////////////////////////////////////////////
        const gl::ScopedBlendAlpha blend;
        for (auto iter = mSceneDataList.begin(); iter != mSceneDataList.end(); ++iter){
            if (iter->getScene() == mActiveScene){
                iter->drawGBuffer();
            }
        }
        //gl::drawSphere(vec3(160.488,   44.952,   28.811), 50);
        ////// END DRAW STUFF //////////////////////////////////////////////////
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    /* L-BUFFER
     *
     * In this pass, we draw light volumes as cubes. We are only using point lights in this
     * scene, which are represented spherically. We use cubes because they have simpler geometry
     * than spheres. As the lights are drawn, the screen position of each fragment is used to
     * read the G-buffer to calculate the shaded color.
     *
     * After the light is rendered, we draw a large cube covering the scene to calculate shadows
     * and subtract color. Using one large cube at the end gives us depth information and keeps
     * the overhead of implementing shadows low.
     */
    
    size_t ping = 0;
    size_t pong = 1;
    
    {
        const gl::ScopedFramebuffer scopedFrameBuffer( mFboPingPong );
        const gl::ScopedViewport scopedViewport( ivec2( 0 ), mFboPingPong->getSize() );
        {
            const static GLenum buffers[] = {
                GL_COLOR_ATTACHMENT0,
                GL_COLOR_ATTACHMENT1
            };
            gl::drawBuffers( 2, buffers );
            gl::clear(ColorAf(0.0, 0.0, 0.0, 0.0));
        }
        
        gl::drawBuffer( GL_COLOR_ATTACHMENT0 + (GLenum)ping );
        gl::enableDepthRead();
        {
            if (mIsAlbedoEnabled){
                mTextureFboGBufferVector[mAlbedoLayoutIndex]->bind(mAlbedoLayoutIndex);
            }
            if (mIsMaterialIdEnabled){
                mTextureFboGBufferVector[mMaterialIdLayoutIndex]->bind(mMaterialIdLayoutIndex);
            }
            if (mIsNormalEnabled){
                mTextureFboGBufferVector[mNormalLayoutIndex]->bind(mNormalLayoutIndex);
            }
            if (mIsSpecularEnabled){
                mTextureFboGBufferVector[mSpecularLayoutIndex]->bind(mSpecularLayoutIndex);
            }
            if (mIsEmissionEnabled){
                mTextureFboGBufferVector[mEmissionLayoutIndex]->bind(mEmissionLayoutIndex);
            }
            if (mIsOcclusionEnabled){
                mTextureFboGBufferVector[mOcclusionLayoutIndex]->bind(mOcclusionLayoutIndex);
            }
            if (mIsDepthEnabled){
                mFboGBuffer->getDepthTexture()->bind(mDepthLayoutIndex);
            }
            
            gl::enableDepthWrite();
            const gl::ScopedMatrices scopedMatrices;
            gl::setMatrices(mActiveScene->getActiveCamera()->getCinderCamera());
            const gl::ScopedBlendAdditive scopedBlendAdditive;
                        
            for (auto iter = mSceneDataList.begin(); iter != mSceneDataList.end(); ++iter){
                if (iter->getScene() == mActiveScene){
                    iter->drawLBuffer(mFboPingPong->getSize());
                }
            }
            
            //        // Draw shadows onto L-buffer
            //        if ( mEnabledShadow ) {
            //            gl::disableDepthWrite();
            //            const gl::ScopedTextureBind scopedTextureBind0( mFboShadowMap->getDepthTexture(),	0 );
            //            const gl::ScopedTextureBind scopedTextureBind1( mFboGBuffer->getDepthTexture(),		1 );
            //
            //            mBatchLBufferShadowRect->getGlslProg()->uniform( "uProjMatrixInverse",	projMatrixInverse );
            //            mBatchLBufferShadowRect->getGlslProg()->uniform( "uProjectionParams",	projectionParams );
            //            mBatchLBufferShadowRect->getGlslProg()->uniform( "uProjView",			mShadowCamera.getProjectionMatrix() * mShadowCamera.getViewMatrix() );
            //            mBatchLBufferShadowRect->getGlslProg()->uniform( "uViewMatrixInverse",	mCamera.getInverseViewMatrix() );
            //
            //            const gl::ScopedBlendAlpha scopedBlendAlpha;
            //            const gl::ScopedModelMatrix scopedModelMatrix;
            //            gl::translate( getWindowSize() / 2 );
            //            gl::scale( getWindowSize() );
            //            mBatchLBufferShadowRect->draw();
            //        }
            
            if (mIsAlbedoEnabled){
                mTextureFboGBufferVector[mAlbedoLayoutIndex]->unbind();
            }
            if (mIsMaterialIdEnabled){
                mTextureFboGBufferVector[mMaterialIdLayoutIndex]->unbind();
            }
            if (mIsNormalEnabled){
                mTextureFboGBufferVector[mNormalLayoutIndex]->unbind();
            }
            if (mIsSpecularEnabled){
                mTextureFboGBufferVector[mSpecularLayoutIndex]->unbind();
            }
            if (mIsEmissionEnabled){
                mTextureFboGBufferVector[mEmissionLayoutIndex]->unbind();
            }
            if (mIsOcclusionEnabled){
                mTextureFboGBufferVector[mOcclusionLayoutIndex]->unbind();
            }
            if (mIsDepthEnabled){
                mFboGBuffer->getDepthTexture()->unbind();
            }
        }
        ping = pong;
        pong = ( ping + 1 ) % 2;
    }
    
    //DEBUG VIEW
    if ( mIsDebugEnabled ) {
        const gl::ScopedFramebuffer scopedFramebuffer( mFboPingPong );
        gl::drawBuffer( GL_COLOR_ATTACHMENT0 + (GLenum)ping );
        const gl::ScopedViewport scopedViewport( ivec2( 0 ), mFboPingPong->getSize() );
        const gl::ScopedMatrices scopedMatrices;
        gl::setMatricesWindow( mFboPingPong->getSize() );
        gl::disableDepthRead();
        gl::disableDepthWrite();
        
        const size_t columns = 4;
        
        vec2 sz;
        sz.x = (float)mFboPingPong->getWidth() / (float)columns;
        sz.y = sz.x / mFboPingPong->getAspectRatio();
        
        if (mIsAlbedoEnabled){
            mTextureFboGBufferVector[mAlbedoLayoutIndex]->bind(mAlbedoLayoutIndex);
        }
        if (mIsMaterialIdEnabled){
            mTextureFboGBufferVector[mMaterialIdLayoutIndex]->bind(mMaterialIdLayoutIndex);
        }
        if (mIsNormalEnabled){
            mTextureFboGBufferVector[mNormalLayoutIndex]->bind(mNormalLayoutIndex);
        }
        if (mIsDepthEnabled){
            mFboGBuffer->getDepthTexture()->bind(mDepthLayoutIndex);
        }
        //        const gl::ScopedTextureBind scopedTextureBind4( mTextureFboAo[ 0 ],	4 );
        //        const gl::ScopedTextureBind scopedTextureBind5( mTextureFboAccum[ mEnabledBloom ? 2 : 0 ],	5 );
        //        if ( mTextureFboRayColor[ 1 ] ) {
        //            mTextureFboRayColor[ 1 ]->bind( 6 );
        //        }
        
        mBatchDebugRect->getGlslProg()->uniform( "uFar", f );
        //        size_t count = mEnabledRay ? 12 : 11;
        size_t count = 11;
        for ( int32_t i = 0; i <= count; ++i ) {
            const gl::ScopedModelMatrix scopedModelMatrix;
            const vec2 pos( ( i % columns ) * sz.x, glm::floor( (float)i / (float)columns ) * sz.y );
            gl::translate( pos + sz * 0.5f );
            gl::scale( sz );
            mBatchDebugRect->getGlslProg()->uniform( "uMode", i );
            mBatchDebugRect->draw();
        }
        
        if (mIsAlbedoEnabled){
            mTextureFboGBufferVector[mAlbedoLayoutIndex]->unbind();
        }
        if (mIsMaterialIdEnabled){
            mTextureFboGBufferVector[mMaterialIdLayoutIndex]->unbind();
        }
        if (mIsNormalEnabled){
            mTextureFboGBufferVector[mNormalLayoutIndex]->unbind();
        }
        if (mIsDepthEnabled){
            mFboGBuffer->getDepthTexture()->unbind();
            
        }
        
        //        if ( mTextureFboRayColor[ 1 ] ) {
        //            mTextureFboRayColor[ 1 ]->unbind();
        //        }
    }
    else {
        {
            //////////////////////////////////////////////////////////////////////////////////////////////
            /* COMPOSITE
             *
             * This first pass begins post-processing. That is, we actually start working on our final
             * image in screen space here. If we have AO enabled, it is applied to the L-buffer result.
             * Otherwise, we'll just make a copy of the L-buffer and move on.
             */
            
            const gl::ScopedFramebuffer scopedFrameBuffer( mFboPingPong );
            const gl::ScopedViewport scopedViewport( ivec2( 0 ), mFboPingPong->getSize() );
            const gl::ScopedMatrices scopedMatrices;
            gl::setMatricesWindow( mFboPingPong->getSize() );
            gl::translate( mFboPingPong->getSize() / 2 );
            gl::scale( mFboPingPong->getSize() );
            gl::disableDepthRead();
            gl::disableDepthWrite();
            
            {
                gl::drawBuffer( GL_COLOR_ATTACHMENT0 + (GLenum)ping );
                if ( mAo != Ao_None ) {
                    
                    // Blend L-buffer and AO
                    const gl::ScopedTextureBind scopedTextureBind1( mTextureFboPingPong[ pong ],	0 );
                    const gl::ScopedTextureBind scopedTextureBind0( mTextureFboAo[ 0 ],				1 );
                    mBatchAoCompositeRect->draw();
                } else {
                    
                    // Draw L-buffer without AO
                    const gl::ScopedTextureBind scopedTextureBind( mTextureFboPingPong[ pong ], 0 );
                    mBatchStockTextureRect->draw();
                }
                
                ping = pong;
                pong = ( ping + 1 ) % 2;
            }
            
            //            //////////////////////////////////////////////////////////////////////////////////////////////
            //            /* FOG
            //             *
            //             * To simulate fog, all we really have to do represent the depth buffer as color and mix
            //             * it into our image.
            //             */
            //
            //            if ( mEnabledFog ) {
            //                gl::drawBuffer( GL_COLOR_ATTACHMENT0 + (GLenum)ping );
            //                const gl::ScopedTextureBind scopedTextureBind0( mFboGBuffer->getDepthTexture(),	0 );
            //                const gl::ScopedTextureBind scopedTextureBind1( mTextureFboPingPong[ pong ],	1 );
            //                mBatchFogRect->draw();
            //
            //                ping = pong;
            //                pong = ( ping + 1 ) % 2;
            //            }
            //
            //            //////////////////////////////////////////////////////////////////////////////////////////////
            //            /* DEPTH OF FIELD
            //             *
            //             * Depth of field simulates a lens effect by performing a shaped blur (bokeh) on our
            //             * image; based on its depth and distance from the camera. It helps to "unflatten" a 3D
            //             * image's appearance on a 2D screen. This is a fairly expensive operation. It really only
            //             * benefits a scene which has objects close to the camera that should be unfocused.
            //             * If this doesn't describe your scene, do not enable this pass.
            //             */
            //
            //            if ( mEnabledDoF ) {
            //                gl::drawBuffer( GL_COLOR_ATTACHMENT0 + (GLenum)ping );
            //
            //                const float d = glm::length( mCamera.getEyePoint() );
            //                const gl::ScopedTextureBind scopedTextureBind0( mFboGBuffer->getDepthTexture(), 0 );
            //                const gl::ScopedTextureBind scopedTextureBind1( mTextureFboPingPong[ pong ],	1 );
            //                mBatchDofRect->getGlslProg()->uniform( "uFocalDepth",	d );
            //                mBatchDofRect->getGlslProg()->uniform( "uNear",			n );
            //                mBatchDofRect->draw();
            //
            //                ping = pong;
            //                pong = ( ping + 1 ) % 2;
            //            }
            //
            //            //////////////////////////////////////////////////////////////////////////////////////////////
            //            /* COLOR
            //             *
            //             * This pass applies chromatic aberration, brightness, saturation, contrast, and intensity
            //             * filtering. You may modify these settings in post/color.frag.
            //             */
            //
            //            if ( mEnabledColor ) {
            //                gl::drawBuffer( GL_COLOR_ATTACHMENT0 + (GLenum)ping );
            //                const gl::ScopedTextureBind scopedTextureBind( mTextureFboPingPong[ pong ], 0 );
            //                mBatchColorRect->draw();
            //
            //                ping = pong;
            //                pong = ( ping + 1 ) % 2;
            //            }
        }
        
        //////////////////////////////////////////////////////////////////////////////////////////////
        /* FINAL RENDER
         *
         * This pass prepares our image to be rendered to the screen. Light accumulation is painted
         * onto the image. If we are in full screen AO mode, we'll prepare that view instead.
         */
        
        const gl::ScopedFramebuffer scopedFramebuffer( mFboPingPong );
        gl::drawBuffer( GL_COLOR_ATTACHMENT0 + (GLenum)ping );
        const gl::ScopedViewport scopedViewport( ivec2( 0 ), mFboPingPong->getSize() );
        const gl::ScopedMatrices scopedMatrices;
        gl::setMatricesWindow( mFboPingPong->getSize() );
        gl::translate( mFboPingPong->getSize() / 2 );
        gl::scale( mFboPingPong->getSize() );
        gl::disableDepthRead();
        gl::disableDepthWrite();
        
        // Fill screen with AO in AO view mode
        if ( mIsAoEnabled ) {
            const gl::ScopedTextureBind scopedTextureBind( mTextureFboAo[ 0 ], 4 );
            mBatchDebugRect->getGlslProg()->uniform( "uMode", 11 );
            mBatchDebugRect->draw();
        } else {
            //            // Composite light rays into image
            //            if ( mEnabledRay ) {
            //                const gl::ScopedTextureBind scopedTextureBind0( mTextureFboPingPong[ pong ],	0 );
            //                const gl::ScopedTextureBind scopedTextureBind1( mTextureFboRayColor[ 1 ],		1 );
            //                mBatchRayCompositeRect->draw();
            //
            //                ping = pong;
            //                pong = ( ping + 1 ) % 2;
            //            }
            
            //            // Composite light accumulation / bloom into our final image
            //            gl::drawBuffer( GL_COLOR_ATTACHMENT0 + (GLenum)ping );
            //            {
            //                const gl::ScopedTextureBind scopedTextureBind0( mTextureFboPingPong[ pong ],				0 );
            //                const gl::ScopedTextureBind scopedTextureBind1( mTextureFboAccum[ mEnabledBloom ? 2 : 0 ],	1 );
            //                mBatchBloomCompositeRect->draw();
            //            }
            
            // Draw light volumes for debugging
            //            if ( mDrawLightVolume ) {
            //                const gl::ScopedBlendAlpha scopedBlendAlpha;
            //                const gl::ScopedPolygonMode scopedPolygonMode( GL_LINE );
            //                const gl::ScopedMatrices scopedMatrices;
            //                gl::setMatrices( mCamera );
            //
            //                for ( const Light& light : mLights ) {
            //                    const gl::ScopedModelMatrix scopedModelMatrix;
            //                    const gl::ScopedColor scopedColor( light.getColorDiffuse() * ColorAf( Colorf::white(), 0.08f ) );
            //                    gl::translate( light.getPosition() );
            //                    gl::scale( vec3( light.getVolume() ) );
            //                    mBatchStockColorSphere->draw();
            //                }
            //            }
        }
    }
    
    ping = pong;
    pong = ( ping + 1 ) % 2;
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    // BLIT
    
    // Render our final image to the screen
    const gl::ScopedViewport scopedViewport( ivec2( 0 ), toPixels( getWindowSize() ) );
    const gl::ScopedMatrices scopedMatrices;
    gl::setMatricesWindow( toPixels( getWindowSize() ) );
    gl::translate( toPixels( getWindowSize() / 2 )  );
    gl::scale( toPixels( getWindowSize() ) );
    gl::disableDepthRead();
    gl::disableDepthWrite();
    const gl::ScopedTextureBind scopedTextureBind( mTextureFboPingPong[ pong ], 0 );
    if ( mIsFxaaEnabled ) {
        
        // To keep bandwidth in check, we aren't using any hardware
        // anti-aliasing (MSAA). Instead, we use FXAA as a post-process
        // to clean up our image.
        mBatchFxaaRect->draw();
    } else {
        
        // Draw to screen without FXAA
        mBatchStockTextureRect->draw();
    }
}

void DeferredRenderer::draw(const SceneRef &aScene, const OrthographicCameraRef &aOrthographicCamera)
{
    aScene->setActiveCamera(aOrthographicCamera);
    setActiveScene(aScene);
}

void DeferredRenderer::draw(const SceneRef &aScene, const StereoCameraRef &aStereoCamera)
{
    aScene->setActiveCamera(aStereoCamera);
    setActiveScene(aScene);
}

void DeferredRenderer::onNotifyAdded(const Object3DRef &aAddedObject)
{
    SceneRef parentScene = aAddedObject->getTopParent()->cast<Scene>();
    for (auto& sceneData : mSceneDataList){
        if (parentScene == sceneData.getScene()){
            sceneData.onNotifyAdded(aAddedObject);
        }
    }
}

void DeferredRenderer::onNotifyRemoved(const Object3DRef &aRemovedObject)
{
    SceneRef parentScene = aRemovedObject->getTopParent()->cast<Scene>();
    for (auto& sceneData : mSceneDataList){
        if (parentScene == sceneData.getScene()){
            sceneData.onNotifyRemoved(aRemovedObject);
        }
    }
}

void DeferredRenderer::addScene(const SceneRef &aScene)
{
    for (auto& sceneData : mSceneDataList){
        if (aScene == sceneData.getScene()) return;
    }
    aScene->setParentRenderer(shared_from_this());
    SceneData sceneData = SceneData(aScene);
    sceneData.update();
    mSceneDataList.push_back(sceneData);
}

void DeferredRenderer::removeScene(const SceneRef &aScene)
{
    for (auto iter = mSceneDataList.begin(); iter != mSceneDataList.end(); ++iter){
        
        if (aScene == iter->getScene()){
            mSceneDataList.erase(iter);
        }
    }
}

void DeferredRenderer::bindScene(const SceneRef &aScene)
{
    for (auto& sceneData : mSceneDataList){
        if (aScene == sceneData.getScene()){
            sceneData.bind();
            sceneData.update();
        }
    }
}

void DeferredRenderer::unbindScene(const SceneRef &aScene)
{
    for (auto& sceneData : mSceneDataList){
        if (aScene == sceneData.getScene()){
            sceneData.unbind();
        }
    }
}

void DeferredRenderer::setActiveScene(const SceneRef &aActiveScene)
{
    if (mActiveScene == aActiveScene) return;
    mActiveScene = aActiveScene;
    bool isNew = true;
    for (auto& sceneData : mSceneDataList){
        if (sceneData.getScene()==mActiveScene){
            isNew = false;
            if (!sceneData.getIsBinded()){
                sceneData.bind();
            }
        }
    }
    if (isNew){
        addScene(mActiveScene);
        mSceneDataList.back().bind();
    }
}

//for setting up the renderer

void DeferredRenderer::setupGBufferLayout()
{
    GLuint colorInternalFormat = GL_RGB10_A2;
    gl::Texture2d::Format colorTextureFormatLinear = gl::Texture2d::Format()
    .internalFormat( colorInternalFormat )
    .magFilter( GL_LINEAR )
    .minFilter( GL_LINEAR )
    .wrap( GL_CLAMP_TO_EDGE )
    .dataType( GL_FLOAT );
    
    gl::Texture2d::Format colorTextureFormatNearest = gl::Texture2d::Format()
    .internalFormat( colorInternalFormat )
    .magFilter( GL_NEAREST )
    .minFilter( GL_NEAREST )
    .wrap( GL_CLAMP_TO_EDGE )
    .dataType( GL_FLOAT );
    
    // Texture format for depth buffers
    gl::Texture2d::Format depthTextureFormat = gl::Texture2d::Format()
    .internalFormat( GL_DEPTH_COMPONENT32F )
    .magFilter( GL_LINEAR )
    .minFilter( GL_LINEAR )
    .wrap( GL_CLAMP_TO_EDGE )
    .dataType( GL_FLOAT );
    
    int counter=0;
    mTextureFormatFboGBufferVector.clear();
    if (getIsAlbedoEnabled()){
        mAlbedoLayoutIndex = counter;
        counter++;
        mTextureFormatFboGBufferVector.push_back(colorTextureFormatNearest);
    }
    if (getIsNormalEnabled()){
        mNormalLayoutIndex = counter;
        counter++;
        mTextureFormatFboGBufferVector.push_back(gl::Texture2d::Format()
                                                 .internalFormat( GL_RGB16F )
                                                 .magFilter( GL_NEAREST )
                                                 .minFilter( GL_NEAREST )
                                                 .wrap( GL_CLAMP_TO_EDGE )
                                                 .dataType( GL_BYTE ) );
    }
    if (getIsMaterialIdEnabled()){
        mMaterialIdLayoutIndex = counter;
        counter++;
        mTextureFormatFboGBufferVector.push_back(gl::Texture2d::Format()
                                                 .internalFormat( GL_R8I )
                                                 .magFilter( GL_NEAREST )
                                                 .minFilter( GL_NEAREST )
                                                 .wrap( GL_CLAMP_TO_EDGE )
                                                 .dataType( GL_BYTE ) );
    }
    
    if (getIsPositionEnabled()){
        mPositionLayoutIndex = counter;
        counter++;
        mTextureFormatFboGBufferVector.push_back(gl::Texture2d::Format()
                                                 .internalFormat( GL_RGB16F )
                                                 .magFilter( GL_NEAREST )
                                                 .minFilter( GL_NEAREST )
                                                 .wrap( GL_CLAMP_TO_EDGE )
                                                 .dataType( GL_BYTE ) );
    }
    if (getIsTexCoordEnabled()){
        mTexCoordLayoutIndex = counter;
        counter++;
        mTextureFormatFboGBufferVector.push_back(gl::Texture2d::Format()
                                                 .internalFormat( GL_RG16F )
                                                 .magFilter( GL_NEAREST )
                                                 .minFilter( GL_NEAREST )
                                                 .wrap( GL_CLAMP_TO_EDGE )
                                                 .dataType( GL_BYTE ) );
    }
    if (getIsSpecularEnabled()){
        mSpecularLayoutIndex = counter;
        counter++;
        mTextureFormatFboGBufferVector.push_back(gl::Texture2d::Format()
                                                 .internalFormat( GL_RGB16F )
                                                 .magFilter( GL_NEAREST )
                                                 .minFilter( GL_NEAREST )
                                                 .wrap( GL_CLAMP_TO_EDGE )
                                                 .dataType( GL_BYTE ) );
    }
    if (getIsEmissionEnabled()){
        mEmissionLayoutIndex = counter;
        counter++;
        mTextureFormatFboGBufferVector.push_back(gl::Texture2d::Format()
                                                 .internalFormat( GL_RGB16F )
                                                 .magFilter( GL_NEAREST )
                                                 .minFilter( GL_NEAREST )
                                                 .wrap( GL_CLAMP_TO_EDGE )
                                                 .dataType( GL_BYTE ) );
    }
    if (getIsOcclusionEnabled()){
        mOcclusionLayoutIndex = counter;
        counter++;
        mTextureFormatFboGBufferVector.push_back(gl::Texture2d::Format()
                                                 .internalFormat( GL_RGB16F )
                                                 .magFilter( GL_NEAREST )
                                                 .minFilter( GL_NEAREST )
                                                 .wrap( GL_CLAMP_TO_EDGE )
                                                 .dataType( GL_BYTE ) );
    }
    if (mIsDepthEnabled){
        mDepthLayoutIndex = counter;
    }
    
}

void DeferredRenderer::setupShaders()
{
    // Shortcut for shader loading and error handling
}

void DeferredRenderer::setupUniforms()
{
    mBatchFxaaRect->getGlslProg()->uniform("uSampler", 0 );
    
    //dimension aware uniforms
    const vec2 szPingPong	= mFboPingPong	? mFboPingPong->getSize()	: toPixels( getWindowSize() );
    mBatchFxaaRect->getGlslProg()->uniform("uPixel", 1.0f / vec2( szPingPong ) );
    
    mBatchDebugRect->getGlslProg()->uniform(			"uSamplerAlbedo",		mAlbedoLayoutIndex );
    mBatchDebugRect->getGlslProg()->uniform(			"uSamplerMaterial",		mMaterialIdLayoutIndex );
    mBatchDebugRect->getGlslProg()->uniform(			"uSamplerNormal",		mNormalLayoutIndex );
    mBatchDebugRect->getGlslProg()->uniform(			"uSamplerDepth",		mDepthLayoutIndex );
    mBatchDebugRect->getGlslProg()->uniform(			"uSamplerAo",			4 );
    mBatchDebugRect->getGlslProg()->uniform(			"uSamplerAccum",		5 );
    mBatchDebugRect->getGlslProg()->uniform(			"uSamplerRay",			6 );
}

void DeferredRenderer::setupBatches()
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
            mParentWindow->getApp()->quit();
        }
        return glslProg;
    };
    
    // Load shader files
    DataSourceRef fragPostFxaa = loadAsset( "post/fxaa.frag" );
    DataSourceRef vertPassThrough = loadAsset( "pass_through.vert" );
    DataSourceRef fragDeferredDebug	= loadAsset( "deferred/debug.frag" );
    
    
    // Create GLSL programs
    int32_t version					= 330;
    
    gl::GlslProgRef postFxaa		= loadGlslProg( gl::GlslProg::Format().version( version )
                                                   .vertex( vertPassThrough ).fragment( fragPostFxaa )
                                                   .define( "TEX_COORD" ) );
    gl::GlslProgRef stockColor		= gl::context()->getStockShader( gl::ShaderDef().color() );
    gl::GlslProgRef stockTexture	= gl::context()->getStockShader( gl::ShaderDef().texture( GL_TEXTURE_2D ) );
    gl::GlslProgRef debug			= loadGlslProg( gl::GlslProg::Format().version( version )
                                                   .vertex( vertPassThrough ).fragment( fragDeferredDebug )
                                                   .define( "TEX_COORD" ).define( "MAX_NUM_CLASSIC_MATERIALS", ci::toString(25) ).define("HAS_CLASSIC_MATERIALS") );
    debug->uniformBlock("ClassicMaterials", 3);
    debug->uniformBlock("ActiveCamera", 5);
    
    
    // Create geometry as VBO meshes
    gl::VboMeshRef cylinder		= gl::VboMesh::create( geom::Cylinder().subdivisionsAxis( 5 ).subdivisionsHeight( 1 ) );
    gl::VboMeshRef cube			= gl::VboMesh::create( geom::Cube().size( vec3( 2.0f ) ) );
    gl::VboMeshRef icosahedron	= gl::VboMesh::create( geom::Icosahedron() );
    gl::VboMeshRef rect			= gl::VboMesh::create( geom::Rect() );
    gl::VboMeshRef sphere		= gl::VboMesh::create( geom::Sphere().subdivisions( 64 ) );
    gl::VboMeshRef sphereInst	= gl::VboMesh::create( geom::Sphere().subdivisions( 64 ) );
    gl::VboMeshRef sphereLow	= gl::VboMesh::create( geom::Sphere().subdivisions( 12 ) );
    
    // Create batches of VBO meshes and GLSL programs
    
    mBatchFxaaRect					= gl::Batch::create( rect,		postFxaa );
    mBatchStockColorRect			= gl::Batch::create( rect,		stockColor );
    mBatchStockColorSphere			= gl::Batch::create( sphereLow,	stockColor );
    mBatchStockTextureRect			= gl::Batch::create( rect,		stockTexture );
    mBatchDebugRect					= gl::Batch::create( rect,		debug );
}

void DeferredRenderer::resize()
{
    setupGBufferLayout();
    setupShaders();
    setupBatches();
    
    // FBOs are created in the resize event handler so they always match the
    // window's aspect ratio. For this reason, you must call resize() manually
    // in your initialization to get things rolling.
    
    // Choose window size based on selected quality
    ivec2 winSize	= toPixels( mParentWindow->getSize());
    int32_t h		= winSize.y;
    int32_t w		= winSize.x;
    
    
    // Texture formats for color buffers
    GLuint colorInternalFormat = GL_RGB10_A2;
    gl::Texture2d::Format colorTextureFormatLinear = gl::Texture2d::Format()
    .internalFormat( colorInternalFormat )
    .magFilter( GL_LINEAR )
    .minFilter( GL_LINEAR )
    .wrap( GL_CLAMP_TO_EDGE )
    .dataType( GL_FLOAT );
    gl::Texture2d::Format colorTextureFormatNearest = gl::Texture2d::Format()
    .internalFormat( colorInternalFormat )
    .magFilter( GL_NEAREST )
    .minFilter( GL_NEAREST )
    .wrap( GL_CLAMP_TO_EDGE )
    .dataType( GL_FLOAT );
    
    // Texture format for depth buffers
    gl::Texture2d::Format depthTextureFormat = gl::Texture2d::Format()
    .internalFormat( GL_DEPTH_COMPONENT32F )
    .magFilter( GL_LINEAR )
    .minFilter( GL_LINEAR )
    .wrap( GL_CLAMP_TO_EDGE )
    .dataType( GL_FLOAT );
    
    // Set up the G-buffer
    {
        const ivec2 sz = ivec2( vec2( w, h ));
        mTextureFboGBufferVector.clear();
        for ( size_t i = 0; i < mTextureFormatFboGBufferVector.size(); i++ ) {
            mTextureFboGBufferVector.push_back(gl::Texture2d::create( sz.x, sz.y, mTextureFormatFboGBufferVector[i]));
        }
        
        gl::Fbo::Format fboFormat;
        fboFormat.depthTexture( depthTextureFormat );
        for ( size_t i = 0; i < mTextureFboGBufferVector.size(); i++ ) {
            fboFormat.attachment( GL_COLOR_ATTACHMENT0 + (GLenum)i, mTextureFboGBufferVector[i] );
        }
        mFboGBuffer = gl::Fbo::create( sz.x, sz.y, fboFormat );
        
        const gl::ScopedFramebuffer scopedFramebuffer( mFboGBuffer );
        const gl::ScopedViewport scopedViewport( ivec2( 0 ), mFboGBuffer->getSize() );
        gl::clear(ColorAf(0.0, 0.0, 0.0, 0.0));
    }
    
    
    // Set up the ping pong frame buffer. We'll use this FBO to render
    // the scene and perform post-processing passes.
    {
        gl::Fbo::Format fboFormat;
        fboFormat.disableDepth();
        for ( size_t i = 0; i < 2; ++i ) {
            mTextureFboPingPong[ i ] = gl::Texture2d::create( w, h, colorTextureFormatNearest );
            fboFormat.attachment( GL_COLOR_ATTACHMENT0 + (GLenum)i, mTextureFboPingPong[ i ] );
        }
        mFboPingPong = gl::Fbo::create( w, h, fboFormat );
        const gl::ScopedFramebuffer scopedFramebuffer( mFboPingPong );
        const gl::ScopedViewport scopedViewport( ivec2( 0 ), mFboPingPong->getSize() );
        gl::clear(ColorAf(0.0, 0.0, 0.0, 0.0));
    }
    
    // Update uniforms
    setupUniforms();
}

// Setters/ Getters

DeferredRenderer& DeferredRenderer::isDebugEnabled(bool aIsDebugEnabled)
{
    mIsDebugEnabled = aIsDebugEnabled;
    return *this;
}

bool DeferredRenderer::getIsDebugEnabled() const
{
    return mIsDebugEnabled;
}

void DeferredRenderer::setIsDebugEnabled(bool aIsDebugEnabled)
{
    mIsDebugEnabled = aIsDebugEnabled;
}

DeferredRenderer& DeferredRenderer::isFxaaEnabled(bool aIsFxaaEnabled)
{
    mIsFxaaEnabled = aIsFxaaEnabled;
    return *this;
}

bool DeferredRenderer::getIsFxaaEnabled() const
{
    return mIsDebugEnabled;
}

void DeferredRenderer::setIsFxaaEnabled(bool aIsFxaaEnabled)
{
    mIsFxaaEnabled = aIsFxaaEnabled;
}



// gbuffer attachment enabled

DeferredRenderer& DeferredRenderer::isDepthEnabled(bool aIsDepthEnabled)
{
    mIsDepthEnabled = aIsDepthEnabled;
    return *this;
}

DeferredRenderer& DeferredRenderer::isAlbedoEnabled(bool aIsAlbedoEnabled)
{
    mIsAlbedoEnabled = aIsAlbedoEnabled;
    return *this;
}

DeferredRenderer& DeferredRenderer::isNormalEnabled(bool aIsNormalEnabled)
{
    mIsNormalEnabled = aIsNormalEnabled;
    return *this;
}

DeferredRenderer& DeferredRenderer::isPositionEnabled(bool aIsPositionEnabled)
{
    mIsPositionEnabled = aIsPositionEnabled;
    return *this;
}

DeferredRenderer& DeferredRenderer::isTexCoordEnabled(bool aIsTexCoordEnabled)
{
    mIsTexCoordEnabled = aIsTexCoordEnabled;
    return *this;
}

DeferredRenderer& DeferredRenderer::isSpecularEnabled(bool aIsSpecularEnabled)
{
    mIsSpecularEnabled = aIsSpecularEnabled;
    return *this;
}

DeferredRenderer& DeferredRenderer::isEmissionEnabled(bool aIsEmissionEnabled)
{
    mIsEmissionEnabled = aIsEmissionEnabled;
    return *this;
}

DeferredRenderer& DeferredRenderer::isOcclusionEnabled(bool aIsOcclusionEnabled)
{
    mIsOcclusionEnabled = aIsOcclusionEnabled;
    return *this;
}

bool DeferredRenderer::getIsDepthEnabled() const
{
    return mIsDepthEnabled;
}

bool DeferredRenderer::getIsAlbedoEnabled() const
{
    return mIsAlbedoEnabled;
}

bool DeferredRenderer::getIsMaterialIdEnabled() const
{
    return mIsMaterialIdEnabled;
}

bool DeferredRenderer::getIsNormalEnabled() const
{
    return mIsNormalEnabled;
}

bool DeferredRenderer::getIsPositionEnabled() const
{
    return mIsPositionEnabled;
}

bool DeferredRenderer::getIsTexCoordEnabled() const
{
    return mIsTexCoordEnabled;
}

bool DeferredRenderer::getIsSpecularEnabled() const
{
    return mIsSpecularEnabled;
}

bool DeferredRenderer::getIsEmissionEnabled() const
{
    return mIsEmissionEnabled;
}

bool DeferredRenderer::getIsOcclusionEnabled() const
{
    return mIsOcclusionEnabled;
}

void DeferredRenderer::setIsDepthEnabled(bool aIsDepthEnabled)
{
    mIsDepthEnabled = aIsDepthEnabled;
}

void DeferredRenderer::setIsAlbedoEnabled(bool aIsAlbedoEnabled)
{
    mIsAlbedoEnabled = aIsAlbedoEnabled;
}

void DeferredRenderer::setIsNormalEnabled(bool aIsNormalEnabled)
{
    mIsNormalEnabled = aIsNormalEnabled;
}

void DeferredRenderer::setIsPositionEnabled(bool aIsPositionEnabled)
{
    mIsPositionEnabled = aIsPositionEnabled;
}

void DeferredRenderer::setIsTexCoordEnabled(bool aIsTexCoordEnabled)
{
    mIsTexCoordEnabled = aIsTexCoordEnabled;
}

void DeferredRenderer::setIsSpecularEnabled(bool aIsSpecularEnabled)
{
    mIsSpecularEnabled = aIsSpecularEnabled;
}

void DeferredRenderer::setIsEmissionEnabled(bool aIsEmissionEnabled)
{
    mIsEmissionEnabled = aIsEmissionEnabled;
}

void DeferredRenderer::setIsOcclusionEnabled(bool aIsOcclusionEnabled)
{
    mIsOcclusionEnabled = aIsOcclusionEnabled;
}