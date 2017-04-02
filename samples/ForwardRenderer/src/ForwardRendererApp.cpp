#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class ForwardRendererApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void ForwardRendererApp::setup()
{
}

void ForwardRendererApp::mouseDown( MouseEvent event )
{
}

void ForwardRendererApp::update()
{
}

void ForwardRendererApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( ForwardRendererApp, RendererGl )
