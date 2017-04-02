#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

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

class BasicInstancerApp : public App {
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
};

void BasicInstancerApp::setup()
{
    mBasicRenderer = BasicRenderer::create();
    mBasicRenderer->setup(getWindow());
    mBasicRenderer->setClearColor(Colorf(0.0, 0.0, 0.0));
    
    mDeferredRenderer = DeferredRenderer::create();
    mDeferredRenderer->setIsTexCoordEnabled(true);
    mDeferredRenderer->setup(getWindow());
    mDeferredRenderer->setClearColor(Colorf(0.0, 0.0, 0.0));
    
    
    mScene = Scene::create();
    mScene->setName("Scene 1");
    
    mPerspCamera = PerspectiveCamera::create();
    mPerspCamera->setPerspective(33.0, getWindow()->getAspectRatio() ,1, 10000.0);
    mPerspCamera->translate(vec3(0.0, 0.0, 30.0));
    mScene->addChild(mPerspCamera);
    mScene->setActiveCamera(mPerspCamera);
    
    mDeferredRenderer->setActiveScene(mScene);
    
    
    
    MaterialColorRef planeMat = MaterialColor::create();
    planeMat->getMaterialData().setDiffuseColor(Colorf(1.0, 0.0, 0.0));
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
    sphereMat->getMaterialData().setDiffuseColor(Colorf(1.0, 1.0, 1.0));
    sphereMat->setDiffuseMap(gl::Texture2d::create(loadImage(loadAsset("pizi.jpg"))));
    mScene->addToMaterialPallet(sphereMat);
    auto sphereMesh = Mesh::create(gl::VboMesh::create(geom::Cube()), sphereMat);
    sphereMesh->setName("sphere");
    sphereMesh->scale(vec3(3.0));
    sphereMesh->translate(vec3(5.0, -7, -20.0));
    
    auto mInstancer = Instancer::create(8000);
    mInstancer->setReferenceMesh(sphereMesh);
    for (int i=0; i<7777; i++){
        InstanceRef instance = Instance::create();
        instance->color(ColorAf(Rand::randFloat(), Rand::randFloat(), Rand::randFloat(), 1.0)).scale(vec3(Rand::randFloat()*0.2)).translate(vec3(Rand::randFloat(-10.0, 10.0), Rand::randFloat(-10.0, 10.0), Rand::randFloat(-10.0, 10.0))).rotateX(Rand::randFloat(-180.0, 180.0)).rotateY(Rand::randFloat(-180.0, 180.0)).rotateZ(Rand::randFloat(-180.0, 180.0));
        mInstancer->addInstance(instance);
    }
    mInstancer->setup();
    mScene->addChild(mInstancer);
}

void BasicInstancerApp::mouseDown( MouseEvent event )
{
}

void BasicInstancerApp::resize()
{
    //mPerspCamera->setPerspective(33.0, getWindow()->getAspectRatio(), 1, 1000);
}

void BasicInstancerApp::update()
{
    //    mScene->getChildByName("floor")->rotateX(.10);

    
    mScene->update();
    mBasicRenderer->update();
    mDeferredRenderer->update();
    
    
}

void BasicInstancerApp::draw()
{
    mBasicRenderer->draw(mScene);
    //mDeferredRenderer->draw();
    console()<<"FPS= "<<getAverageFps()<<std::endl;
}

CINDER_APP( BasicInstancerApp, RendererGl(RendererGl :: Options (). msaa ( 0 )), []( App::Settings* settings )
{
    //settings->disableFrameRate();
    //settings->setAlwaysOnTop();
    //settings->setBorderless();
    //settings->setResizable(false);
    settings->setWindowSize( 1024, 768 );
    //settings->setMultiTouchEnabled();
    
} )