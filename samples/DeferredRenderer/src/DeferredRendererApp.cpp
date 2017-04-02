#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "cinder/CameraUi.h"

#include "core/Material.h"
#include "objects/Mesh.h"
#include "objects/Light.h"
#include "objects/Camera.h"
#include "objects/Scene.h"
#include "objects/Instancer.h"
#include "renderers/deferred/DeferredRenderer.h"
#include "renderers/basic/BasicRenderer.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class DeferredRendererApp : public App {
public:
    void setup() override;
    void mouseDown( MouseEvent event ) override;
    void resize() override;
    void update() override;
    void draw() override;
    
    PerspectiveCameraRef    mPerspCamera;
    
    SceneRef                mScene;
    DeferredRendererRef     mDeferredRenderer;
    BasicRendererRef        mBasicRenderer;
    InstancerRef            mInstancer;
    CameraUi                mCameraUi;
};

void DeferredRendererApp::setup()
{
    mScene = Scene::create();
    mScene->setName("Scene 1");
    
    mPerspCamera = PerspectiveCamera::create();
    mPerspCamera->setPerspective(44.0, getWindow()->getAspectRatio() ,0.01, 150.0);
    mPerspCamera->translate(vec3(0.0, 0.0, 30.0));
    mScene->addChild(mPerspCamera);
    mScene->setActiveCamera(mPerspCamera);
    //CameraUi
    mCameraUi = CameraUi( &mPerspCamera->getCinderCamera(), getWindow(), -1 );
    mPerspCamera->setCameraUi(mCameraUi);
    
    mBasicRenderer = BasicRenderer::create();
    mBasicRenderer->setup(getWindow());
    mBasicRenderer->setClearColor(Colorf(0.0, 0.0, 0.0));
    
    mDeferredRenderer = DeferredRenderer::create();
    //mDeferredRenderer->setIsDebugEnabled(true);
    //mDeferredRenderer->setIsTexCoordEnabled(true);
    //mDeferredRenderer->setIsPositionEnabled(true);
    mDeferredRenderer->setup(getWindow());
    mDeferredRenderer->setClearColor(Colorf(0.0, 0.0, 0.0));
    mDeferredRenderer->setActiveScene(mScene);
    
    MaterialColorRef planeMat = MaterialColor::create();
    planeMat->getMaterialData().diffuseColor(Colorf(1.0, 0.0, 0.0));
    planeMat->setName("floor_mat");
    mScene->addToMaterialPallet(planeMat);
    MeshRef planeMesh = Mesh::create(gl::VboMesh::create(geom::Plane().size(vec2(1.0))), planeMat);
    //planeMesh->rotateX(90);
    //planeMesh->rotateY(180);
    planeMesh->scale(vec3(60.0));
    planeMesh->translate(vec3(0.0, -10.0, 0.0));
    planeMesh->setName("floor");
    mScene->addChild(planeMesh);
    
    planeMat = planeMat->clone();
    planeMat->setName("wall_mat");
    planeMat->getMaterialData().setDiffuseColor(Colorf(0.0, 0.0, 1.0));
    mScene->addToMaterialPallet(planeMat);
    planeMesh = Mesh::create(gl::VboMesh::create(geom::Plane().size(vec2(1.0))), planeMat);
    planeMesh->rotateX(90);
    planeMesh->scale(vec3(90.0));
    planeMesh->translate(vec3(0.0, 0.0, -50));
    planeMesh->setName("wall");
    mScene->addChild(planeMesh);
    
    auto sphereMat = MaterialNormalViz::create();
    sphereMat->getMaterialData().diffuseColor(Colorf(1.0, 1.0, 0.0)).shininess(222).specularIntensity(1.0).emissionIntensity(0.0);
    mScene->addToMaterialPallet(sphereMat);
    auto sphereMesh = Mesh::create(gl::VboMesh::create(geom::Sphere().subdivisions(22).radius(1.0)), sphereMat);
    sphereMesh->setName("sphere");
    sphereMesh->scale(vec3(3.0));
    sphereMesh->translate(vec3(5.0, -7, -20.0));
    mScene->addChild(sphereMesh);
    
    auto boxMat = MaterialColor::create();
    auto texture = gl::Texture2d::create(loadImage(loadAsset("pizi.jpg")));
    texture->setMinFilter(GL_NEAREST);
    texture->setMagFilter(GL_NEAREST);
    boxMat->getMaterialData().setDiffuseColor(Colorf(0.0, 1.0, 0.0));
    boxMat->setDiffuseMap(texture);
    mScene->addToMaterialPallet(boxMat);
    MeshRef boxMesh = Mesh::create(gl::VboMesh::create(geom::Cube().size(vec3(1.0, 1.0, 1.0))), boxMat);
    boxMesh->setName("box");
    boxMesh->scale(vec3(2.0));
    //boxMesh->rotateX(90);
    mScene->addChild(boxMesh);
    
    mInstancer = Instancer::create(8000);
    mInstancer->setReferenceMesh(boxMesh);
    for (int i=0; i<1000; i++){
        InstanceRef instance = Instance::create();
        instance->color(ColorAf(Rand::randFloat(), Rand::randFloat(), Rand::randFloat(), 1.0)).scale(vec3(Rand::randFloat()*0.4)).translate(vec3(Rand::randFloat(-10.0, 10.0), Rand::randFloat(-10.0, 10.0), Rand::randFloat(-10.0, 10.0))).rotateX(Rand::randFloat(-180.0, 180.0)).rotateY(Rand::randFloat(-180.0, 180.0)).rotateZ(Rand::randFloat(-180.0, 180.0));
        mInstancer->addInstance(instance);
        instance->setMaterial(sphereMat);
    }
    mInstancer->setup();
    mScene->addChild(mInstancer);
    
    //    auto dirLight = DirectionalLight::create();
    //    dirLight->setAmbientIntensity(0.2);
    //    dirLight->setDiffuseIntensity(0.6);
    //    dirLight->setColor(Colorf(1.0, 1.0, 1.0));
    //    dirLight->setDirection(normalize(vec3(1, -1, 0)));
    //    mScene->addChild(dirLight);
    //
    auto dirLight = DirectionalLight::create();
    dirLight->setAmbientIntensity(0.1);
    dirLight->setDiffuseIntensity(0.8);
    dirLight->setColor(Colorf(1.0, 1.0, 1.0));
    dirLight->setDirection(normalize(vec3(-1, -1, 0)));
    mScene->addChild(dirLight);
    
    auto pointLight = PointLight::create();
    pointLight->setAmbientIntensity(0.2);
    pointLight->setDiffuseIntensity(2.0);
    pointLight->setColor(Colorf(1.0, 1.0, 1.0));
    pointLight->translate(vec3(0,-5,-20));
    Attenuation atten;
    atten.constant(0.2).exp(0.3).linear(0.2);
    pointLight->setAttenuation(atten);
    mScene->addChild(pointLight);
    console()<<pointLight->getRadius()<<endl;
    
    
}

void DeferredRendererApp::mouseDown( MouseEvent event )
{
}

void DeferredRendererApp::resize()
{
    mPerspCamera->setPerspective(44.0, getWindow()->getAspectRatio(), 0.01, 200);
}

void DeferredRendererApp::update()
{
    mScene->getChildByName("box")->rotateX(1.0);
    mScene->getChildByName("box")->rotateY(0.7);
    
    const auto& instances = mInstancer->getInstances();
    for (const auto& instance : instances){
        instance->rotateY(2.0);
    }
    //mScene->getCastedChildByTag<DirectionalLight>("directional_light", false)->setDirection(vec3(ci::math<float>::sin(getElapsedFrames()/120.0), -1.0, 0.0));
    mScene->getCastedChildByTag<PointLight>("point_light", false)->translate(vec3(0.0, -0.01, 0.0));
    //mPerspCamera->translate(vec3(0.0, 0.0, -0.01));
    //mPerspCamera->rotateY(.01);
    
    mScene->update();
    //mBasicRenderer->update();
    mDeferredRenderer->update();
}

void DeferredRendererApp::draw()
{
    gl::clear(Colorf(0.0, 0.0, 0.0));
    //mBasicRenderer->draw(mScene);
    mDeferredRenderer->draw();
//    gl::setMatrices(mScene->getActiveCamera()->getCinderCamera());
//    gl::drawSphere(mScene->getCastedChildByTag<PointLight>("point_light", false)->getPosition(), mScene->getCastedChildByTag<PointLight>("point_light", false)->getRadius());
    console()<<"FPS= "<<getAverageFps()<<std::endl;
}

CINDER_APP( DeferredRendererApp, RendererGl(RendererGl :: Options (). msaa ( 0 )), []( App::Settings* settings )
{
    //settings->disableFrameRate();
    //settings->setAlwaysOnTop();
    //settings->setBorderless();
    //settings->setResizable(false);
    settings->setWindowSize( 1024, 768 );
    //settings->setMultiTouchEnabled();
    
} )
