#include "Scene.h"
#include "CheckML.h"
#include <gtc/matrix_transform.hpp>  
#include <gtc/type_ptr.hpp>

using namespace glm;
//-------------------------------------------------------------------------

void Scene::init()
{ 
	setGL();  // OpenGL settings
	gObjects.push_back(new RGBAxis(400.0));
	switch (mId) {
	case SCENE_2D:
		scene2D();
		break;
	case SCENE_3D:
		scene3D();
		break;
	default:
		break;
	}
}

void Scene::scene2D() {
	CPolygon* circle = new CPolygon(300, 250.0);

	circle->setMColor(glm::dvec4(1, 0, 1, 1));
	CPolygon* triangle = new CPolygon(3, 250.0);
	triangle->setMColor(glm::dvec4(1, 1, 0, 1));

	Sierpinski* s = new Sierpinski(4000, 250.0);
	s->setMColor(glm::dvec4(1, 1, 0, 1));

	TriangleRGB* tRGB = new TriangleRGB(30.0);
	RectangleRGB* rRGB = new RectangleRGB(450.0, 355.0);


	gObjects.push_back(circle);
	gObjects.push_back(triangle);
	gObjects.push_back(s);
	gObjects.push_back(tRGB);
	gObjects.push_back(rRGB);

	// transformaciones
	rRGB->setModelMat(glm::translate(rRGB->modelMat(), dvec3(0, 0, -100)));
	tRGB->setModelMat(glm::translate(tRGB->modelMat(), dvec3(250, 0, 0)));
	tRGB->setModelMat(glm::rotate(tRGB->modelMat(), radians(25.0), dvec3(0, 0, 1)));
}

void Scene::scene3D() {
	Texture* tx = new Texture();
	tx->load("..\\Bmps\\baldosaP.bmp");

	Texture* txFloor = new Texture();
	txFloor->load("..\\Bmps\\baldosaC.bmp");

	gTextures.push_back(tx);
	gTextures.push_back(txFloor);

	_3DStar* star = new _3DStar(50.0, 4.0, 50.0);
	star->setTexture(tx);
	star->setModelMat(glm::translate(star->modelMat(), dvec3(0, 200, 0)));

	Floor* floor = new Floor(500.0, 500.0, 5, 5);
	floor->setTexture(txFloor);
	floor->setMColor(glm::dvec4(0.5, 0.5, 0.5, 1));

	Box* box = new Box(100.0);

	gObjects.push_back(box);
	gObjects.push_back(star);
	gObjects.push_back(floor);
}

//-------------------------------------------------------------------------
void Scene::free() 
{ // release memory and resources   

	for (Abs_Entity* el : gObjects)
	{
		delete el;  el = nullptr;
	}
	gObjects.resize(0);
	for (Texture* tx : gTextures) {
		delete tx; tx = nullptr;
	}
	gTextures.resize(0);
}
//-------------------------------------------------------------------------
void Scene::setGL() 
{
	// OpenGL basic setting
	glClearColor(0.0, 0.0, 0.0, 0.0);  // background color (alpha=1 -> opaque)
	glEnable(GL_DEPTH_TEST);  // enable Depth test
	if (mId == SCENE_3D)
		glEnable(GL_TEXTURE_2D);
}
//-------------------------------------------------------------------------
void Scene::resetGL() 
{
	glClearColor(.0, .0, .0, .0);  // background color (alpha=1 -> opaque)
	glDisable(GL_DEPTH_TEST);  // disable Depth test 	
	if (mId == SCENE_3D)
		glDisable(GL_TEXTURE_2D);
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

void Scene::update() {
	for (int i = 0; i < gObjects.size(); i++)
		gObjects.at(i)->update();
}

void Scene::setState(int id) {
	if (mId != id) {
		mId = id;
		free();
		init();
	}
}
//-------------------------------------------------------------------------


