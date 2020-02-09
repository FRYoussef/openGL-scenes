#include "Scene.h"
#include "CheckML.h"
#include <gtc/matrix_transform.hpp>  
#include <gtc/type_ptr.hpp>

using namespace glm;
//-------------------------------------------------------------------------

void Scene::init()
{ 
	setGL();  // OpenGL settings

	// allocate memory and load resources
    // Lights
    // Textures

    // Graphics objects (entities) of the scene
	
	gObjects.push_back(new RGBAxis(400.0));
	CPolygon* circle = new CPolygon(300, 250.0);

	circle->setMColor(glm::dvec4(1,0,1,1));
	CPolygon* triangle = new CPolygon(3, 250.0);
	triangle->setMColor(glm::dvec4(1, 1, 0, 1));

	Sierpinski* s = new Sierpinski(4000, 250.0);
	s->setMColor(glm::dvec4(1, 1, 0, 1));

	TriangleRGB* tRGB = new TriangleRGB(250.0);
	RectangleRGB* rRGB = new RectangleRGB(500.0, 250.0);


	gObjects.push_back(circle);
	gObjects.push_back(triangle);
	gObjects.push_back(s);
	gObjects.push_back(tRGB);
	gObjects.push_back(rRGB);
}
//-------------------------------------------------------------------------
void Scene::free() 
{ // release memory and resources   

	for (Abs_Entity* el : gObjects)
	{
		delete el;  el = nullptr;
	}
}
//-------------------------------------------------------------------------
void Scene::setGL() 
{
	// OpenGL basic setting
	glClearColor(1.0, 1.0, 1.0, 1.0);  // background color (alpha=1 -> opaque)
	glEnable(GL_DEPTH_TEST);  // enable Depth test 

}
//-------------------------------------------------------------------------
void Scene::resetGL() 
{
	glClearColor(.0, .0, .0, .0);  // background color (alpha=1 -> opaque)
	glDisable(GL_DEPTH_TEST);  // disable Depth test 	
}
//-------------------------------------------------------------------------

void Scene::render(Camera const& cam) const 
{
	cam.upload();

	for (Abs_Entity* el : gObjects)
	{
	  el->render(cam.viewMat());
	}
}
//-------------------------------------------------------------------------


