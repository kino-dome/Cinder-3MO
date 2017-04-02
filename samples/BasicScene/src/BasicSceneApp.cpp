#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Timeline.h"

#include "objects/Scene.h"
#include "objects/Mesh.h"
#include "objects/Group.h"
#include "objects/Camera.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class BasicSceneApp : public App {
public:
    void setup() override;
    void mouseDown(MouseEvent event) override;
    void keyDown(KeyEvent event) override;
    void update() override;
    void draw() override;
    
    SceneRef                mScene;
    PerspectiveCameraRef    mCamera;
    MeshRef                 mSphereMesh, mFloorMesh, mBoxMesh, mConeMesh;
    GroupRef                mCamPivot; // GroupRef is a dummy container
    bool                    mShouldRotateCam;
    
    TimelineRef             mFloorTimeline;
    Anim<vec3>              mFloorScale;
};

void BasicSceneApp::setup()
{
    mScene = Scene::create();
    mScene->setName("scene");
    
    mCamPivot = Group::create();
    mCamPivot->setName("cam_pivot");
    
    mCamera = PerspectiveCamera::create();
    mCamera->setPerspective(33.0, getWindowWidth()/(float)getWindowHeight(), 1, 10000.0);
    mCamera->translate(vec3(0, 0, 30));
    //mCamPivot->addChild(mCamera);
    mScene->addChild(mCamPivot);
    mScene->addChild(mCamera);
    mScene->setActiveCamera(mCamera);
    
    mSphereMesh = Mesh::create(gl::VboMesh::create(geom::Sphere().radius(1).subdivisions(100)), MaterialNormalViz::create());
    mSphereMesh->setName("sphere_mesh_0");
    mScene->addChild(mSphereMesh);
    
    auto colorMat = MaterialColor::create();
    colorMat->getMaterialData().diffuseColor(Colorf(0.2, 0.2, 1.0));
    mFloorMesh = Mesh::create(gl::VboMesh::create(geom::Plane()), colorMat);
    //mFloorMesh->rotateY(90);
    mFloorMesh->translate(vec3(0.0, -3.0, 0.0));
    mFloorMesh->scale(vec3(10, 1, 20));
    mFloorMesh->setName("floor_mesh");
    mScene->addChild(mFloorMesh);
    
    auto textureMat = MaterialTexture::create();
    textureMat->getMaterialData().setDiffuseColor(Colorf(1.0, 1.0 ,1.0));
    auto texture = gl::Texture::create(loadImage(loadAsset("texture.jpg")));
    texture->setMagFilter(GL_NEAREST);
    texture->setMinFilter(GL_NEAREST);
    textureMat->setDiffuseMap(texture);
    mBoxMesh = Mesh::create(gl::VboMesh::create(geom::Cube()), textureMat);
    mBoxMesh->setName("box_mesh");
    mScene->addChild(mBoxMesh);
    
    auto wireframeMat= MaterialWireframe::create();
    mConeMesh = Mesh::create(gl::VboMesh::create(geom::Cone().direction(vec3(-1, 0, 0))), wireframeMat);
    mConeMesh->translate(vec3(5, 0, 0));
    mConeMesh->setName("cone_mesh");
    mCamPivot->addChild(mConeMesh);
    console()<<mConeMesh->getName()<<endl;
    console()<<mConeMesh->getParent()->getName()<<endl;
    console()<<mConeMesh->getTopParent()->getName()<<endl;
    
    auto coneClone = mConeMesh->clone(true);
    coneClone->translate(vec3(5, 0, 0)).scale(vec3(0.5));
    console()<<coneClone->getName()<<endl;
    console()<<coneClone->getParent()->getName()<<endl;
    console()<<coneClone->getTopParent()->getName()<<endl;
    
    mShouldRotateCam = false;
    mFloorTimeline = Timeline::create();
    mFloorTimeline->setInfinite(true);
    mFloorTimeline->setPingPong(true);
    mFloorTimeline->appendTo(&mFloorScale, vec3(5, 1, 10), vec3(20, 1, 40), 5.0f);
    timeline().add(mFloorTimeline);
}

void BasicSceneApp::mouseDown(MouseEvent event)
{
}

void BasicSceneApp::keyDown(KeyEvent event)
{
    if (event.getChar() == '1') {
        mCamera->setTargetObject(mSphereMesh);
    }
    else if (event.getChar() == '2') {
        mCamera->setTargetObject(mFloorMesh);
    }
    else if (event.getChar() == '3') {
        mCamera->setTargetObject(mBoxMesh);
    }
    else if (event.getChar() == 'a'){
        mShouldRotateCam = !mShouldRotateCam;
    }
}

void BasicSceneApp::update()
{
    mSphereMesh->rotateY(1);
    
    auto boxModel = mBoxMesh->getModel();
    mBoxMesh->setModel(boxModel.modelTranslation(vec3(sin(getElapsedSeconds()), cos(getElapsedSeconds()), 0) * 2.0f));
    mBoxMesh->rotateX(.1).rotateY(.2).rotateZ(.05);
    
    if (mShouldRotateCam){
        mCamPivot->rotateY(.5);
    }
    
    mFloorMesh->setScale(mFloorScale);
    
    mScene->update();
}

void BasicSceneApp::draw()
{
    gl::clear( Color( 0, 0, 0 ) );
    
    const ci::gl::ScopedViewport scopedViewport( ci::ivec2( 0 ), getWindow()->getBounds().getSize() );
    const ci::gl::ScopedMatrices scopedMatrices;
    ci::gl::enableDepthRead();
    ci::gl::enableDepthWrite();
    ci::gl::setMatrices(mCamera->getCinderCamera());
    mScene->draw();
    
}

CINDER_APP( BasicSceneApp, RendererGl(RendererGl :: Options (). msaa ( 16 )), []( App::Settings* settings )
{
    //settings->disableFrameRate();
    //settings->setAlwaysOnTop();
    //settings->setBorderless();
    //settings->setResizable(false);
    settings->setWindowSize( 1024, 768 );
    //settings->setMultiTouchEnabled();
    
} )
