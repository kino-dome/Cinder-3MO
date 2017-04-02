#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

#include "objects/Mesh.h"
#include "core/Material.h"
#include "objects/Scene.h"
#include "renderers/basic/BasicRenderer.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class BasicRendererApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
    void keyDown( KeyEvent event) override;
	void update() override;
	void draw() override;
    void resize() override;
    
    BasicRendererRef        mRenderer;
    SceneRef                mScene;
    MeshRef                 mMesh;
    ClassicMaterialRef      mMaterial;
    PerspectiveCameraRef    mCamera;
};

void BasicRendererApp::setup()
{
    Rand::randomize();
    mRenderer = BasicRenderer::create();
    mRenderer->setup(getWindow());
    mRenderer->setClearColor(ColorAf(0.0, 0.0, 0.0, 1.0));
    
    mScene = Scene::create();
    
    mMaterial = MaterialColor::create();
    mMaterial->getMaterialData().setDiffuseColor(Colorf(1.0, 0.0, 0.0));
    auto texture = gl::Texture::create(loadImage(loadAsset("pizi.jpg")));
    auto geom = gl::VboMesh::create(geom::Cube().size(vec3(3.0, 3.0, 3.0)));
    texture->setMagFilter(GL_NEAREST);
    texture->setMinFilter(GL_NEAREST);
    MaterialTextureRef materialTexture = MaterialTexture::create();
    materialTexture->setDiffuseMap(texture);
    MaterialLambertRef materialLambert = MaterialLambert::create();
    materialLambert->setDiffuseMap(texture);
    MaterialNormalVizRef materialNormalViz = MaterialNormalViz::create();
    
    MeshRef motherMesh = Mesh::create(gl::VboMesh::create(geom::Torus()), materialNormalViz);
    motherMesh->translate(vec3(10.0, 0.0, 0.0)).name("motherMesh");
    mScene->addChild(motherMesh);
    
    //auto materialPhong = MaterialPhong::create();
    
    mMesh = Mesh::create(geom, materialLambert);
    mMesh->setName("sphere");
    motherMesh->addChild(mMesh);
    
    
    MeshRef childMesh = Mesh::create(gl::VboMesh::create(geom::Cylinder().radius(1.0).height(5.0)), mMaterial);
    childMesh->translate(vec3(-6.0, 0.0,0.0)).scale(vec3(0.3, 0.3, 0.3)).name("sphere");
    mMesh->addChild(childMesh);
    mMesh->translate(vec3(-10.0, -6.0, 0.0));
    
    
    mCamera = PerspectiveCamera::create();
    mCamera->setPerspective(33.0, getWindowWidth()/(float)getWindowHeight(), 1, 10000.0);
    mCamera->translate(vec3(0.0, 0.0, 30.0));
    mCamera->setTargetObject(motherMesh);
    
    mScene->addChild(mCamera);
    
    MaterialWireframeRef materialWireframe = MaterialWireframe::create();
    materialWireframe->getMaterialData().diffuseColor(Color::white());
    MeshRef sphereMesh = Mesh::create(gl::VboMesh::create(geom::Sphere().radius(5.0).subdivisions(33)), materialWireframe);
    sphereMesh->translate(vec3(0.0, 0.0, -10.0)).name("properSphere");
    mScene->addChild(sphereMesh);
    childMesh->setTargetObject(motherMesh);
    
    auto sphereList = mScene->getChildrenByName("sphere", true); // recursive search for the name "sphere"
    console()<<sphereList.size()<<endl;
    sphereList = mScene->getChildrenByName("sphere", false); // searches only the first layer
    console()<<sphereList.size()<<endl;
    sphereList = mScene->getChildrenByType("mesh", true); // searches all layers
    console()<<sphereList.size()<<endl;
    
    mMesh = motherMesh;
    
    //mScene->update();
    //ci::app::console()<<mCamera->getShouldUpdate()<<std::endl;
}

void BasicRendererApp::mouseDown( MouseEvent event )
{
    auto sphereMesh = mScene->getCastedChildByName<Mesh>("properSphere", false);
    auto wireframeClone = sphereMesh->getMaterial()->cast<MaterialWireframe>()->clone();
    wireframeClone->setLineWidth(11);
    vector<MeshRef> meshes = mScene->getCastedChildrenByType<Mesh>("mesh", true);
    meshes[2]->setMaterial(wireframeClone);
}

void BasicRendererApp::keyDown(cinder::app::KeyEvent event)
{
    if (event.getChar() == '1'){
        auto mesh = mMesh->clone(true);
        mesh -> setName("box");
        mesh -> rotateX(Rand::randFloat(0.0, 90));
        mesh -> rotateY(Rand::randFloat(0.0, 90));
        mesh -> rotateZ(Rand::randFloat(0.0, 90));
        //mesh->scale(vec3(Rand::randFloat(), Rand::randFloat(), Rand::randFloat()));
        mesh->translate(vec3(0.0, 0.0, -20.0));
        mMesh = mesh;
    }
    else if (event.getChar() == '2'){
        
    }
}

void BasicRendererApp::resize()
{
    mCamera->setPerspective(33.0, getWindow()->getAspectRatio(), 1, 1000);
}

void BasicRendererApp::update()
{
    
    mCamera->lookAt(mMesh->getModelWorld().getModelTranslation());
    
    vector<MeshRef> meshes = mScene->getCastedChildrenByType<Mesh>("mesh", true);
    meshes[0]->translate(vec3(0.0, 0.01, 0.0));
    meshes[1]->rotateZ(0.8);
    meshes[1]->rotateX(-0.8);
    meshes[1]->rotateY(0.5);
    meshes[2]->scale(vec3(1.001, 1.001, 1.001));
    meshes[2]->getMaterial()->cast<ClassicMaterial>()->getMaterialData().setDiffuseColor(Colorf(Rand::randFloat(), Rand::randFloat(), Rand::randFloat()));
    //meshes[1]->rotateX(0.01);
    //meshes[2]->lookAt(vec3(0.0, 0.0, 0.0));
    
    
    auto sphereMesh = mScene->getChildByName("properSphere", false);
    sphereMesh->rotateY(0.6);
    
    auto boxGroups = mScene->getChildrenByName("box");
    for (int i=0; i<boxGroups.size(); i++){
        boxGroups[i]->rotateZ(0.8);
        boxGroups[i]->rotateX(-0.8);
        boxGroups[i]->rotateY(0.5);
//        boxGroups[i]->scale(vec3(1.01, 1.0, 1.0));
    }
    
    mScene->update();
    mRenderer->update();
    
}

void BasicRendererApp::draw()
{
    gl::clear(Color::black());
    mRenderer->draw(mScene);
    vector<MeshRef> meshes = mScene->getCastedChildrenByType<Mesh>("mesh", true);
    gl::setMatrices(mCamera->getCinderCamera());
    gl::translate(vec3(0.0));
    gl::scale(vec3(10));
    gl::color(Colorf(1.0, 1.0, 1.0));
    gl::drawVector(vec3(0, 0, 0), meshes[1]->getViewDirection());
    //getWindow()->setAlwaysOnTop();
}

CINDER_APP( BasicRendererApp, RendererGl(RendererGl :: Options (). msaa ( 0 )), []( App::Settings* settings )
{
    //settings->disableFrameRate();
    //settings->setAlwaysOnTop();
    //settings->setBorderless();
    //settings->setResizable(false);
    settings->setWindowSize( 1024, 768 );
    //settings->setMultiTouchEnabled();
    
} )
