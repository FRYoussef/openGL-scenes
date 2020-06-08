#include "Scene.h"
#include "CheckML.h"
#include <gtc/matrix_transform.hpp>  
#include <gtc/type_ptr.hpp>
#include <string>

#if defined(WIN32) || defined(_WIN32)
const std::string PATH_SEPARATOR = "\\";
#else
const std::string PATH_SEPARATOR = "/";
#endif

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
	RectangleRGB* rRGB = new RectangleRGB(700.0, 500.0);


	gObjects.push_back(circle);
	gObjects.push_back(triangle);
	gObjects.push_back(s);
	gObjects.push_back(tRGB);
	gObjects.push_back(rRGB);

	// transformaciones
	rRGB->setModelMat(glm::translate(rRGB->modelMat(), dvec3(0, 0, -100)));
	rRGB->setModelMat(glm::rotate(rRGB->modelMat(), radians(90.0), dvec3(1, 0, 0)));
	tRGB->setModelMat(glm::translate(tRGB->modelMat(), dvec3(250, 0, 0)));
	tRGB->setModelMat(glm::rotate(tRGB->modelMat(), radians(25.0), dvec3(0, 0, 1)));
}

void Scene::scene3D() {
	std::string path = "";
	Texture* tx = new Texture();
	path = ".." + PATH_SEPARATOR + "Bmps" + PATH_SEPARATOR + "baldosaP.bmp";
	tx->load(path);

	Texture* txFloor = new Texture();
	path = ".." + PATH_SEPARATOR + "Bmps" + PATH_SEPARATOR + "baldosaC.bmp";
	txFloor->load(path);

	Texture* txBox = new Texture();
	path = ".." + PATH_SEPARATOR + "Bmps" + PATH_SEPARATOR + "container.bmp";
	txBox->load(path);

	Texture* txiBox = new Texture();
	path = ".." + PATH_SEPARATOR + "Bmps" + PATH_SEPARATOR + "papelE.bmp";
	txiBox->load(path);

	Texture* txGlass = new Texture();
	path = ".." + PATH_SEPARATOR + "Bmps" + PATH_SEPARATOR + "windowV.bmp";
	txGlass->load(path, 100);

	Texture* txGrass = new Texture();
	path = ".." + PATH_SEPARATOR + "Bmps" + PATH_SEPARATOR + "grass.bmp";
	txGrass->load(path, u8vec3(0, 0, 0), 0.0);

	gTextures.push_back(tx);
	gTextures.push_back(txFloor);
	gTextures.push_back(txBox);
	gTextures.push_back(txGlass);
	gTextures.push_back(txGrass);

	_3DStar* star = new _3DStar(50.0, 6.0, 50.0);
	star->setTexture(tx);
	star->setModelMat(glm::translate(star->modelMat(), dvec3(-150.0, 200.0, -150.0)));

	Floor* floor = new Floor(500.0, 500.0, 5, 5);
	floor->setTexture(txFloor);
	floor->setMColor(glm::dvec4(0.5, 0.5, 0.5, 1));
	floor->setModelMat(glm::rotate(floor->modelMat(), radians(90.0), dvec3(1, 0, 0)));

	Box* box = new Box(100.0);
	box->setTexture(txBox);
	box->setiTexture(txiBox);
	box->setModelMat(glm::translate(box->modelMat(), dvec3(-150.0, 51.0, -150.0)));

	Picture* picture = new Picture(80.0, 50.0);
	picture->setModelMat(glm::translate(picture->modelMat(),  dvec3(0, 1, 0)));

	

	Box* glass = new Box(500.0);
	glass->setTexture(txGlass);
	glass->setModelMat(glm::translate(glass->modelMat(), dvec3(0.0, 249.0, 0.0)));

	Grass* grass = new Grass(10, 200.0, 100.0);
	grass->setTexture(txGrass);
	grass->setModelMat(glm::translate(grass->modelMat(), dvec3(150.0, 1, -150.0)));

	gObjects.push_back(picture);
	gObjects.push_back(box);
	gObjects.push_back(star);
	gObjects.push_back(floor);
	//gObjects.push_back(glass);
	gObjects.push_back(grass);
	gTransObjects.push_back(glass);
	//gTransObjects.push_back(grass);
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
	for (Abs_Entity* el : gTransObjects)
	{
		delete el;  el = nullptr;
	}
	gTransObjects.resize(0);

}
//-------------------------------------------------------------------------
void Scene::setGL() 
{
	// OpenGL basic setting
	glClearColor(0.0, 0.0, 0.0, 0.0);  // background color (alpha=1 -> opaque)
	glEnable(GL_DEPTH_TEST);  // enable Depth test
	glEnable(GL_TEXTURE_2D);
}
//-------------------------------------------------------------------------
void Scene::resetGL() 
{
	glClearColor(.0, .0, .0, .0);  // background color (alpha=1 -> opaque)
	glDisable(GL_DEPTH_TEST);  // disable Depth test
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

	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (Abs_Entity* el : gTransObjects)
	{
		el->render(cam.viewMat());
	}
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

void Scene::update() {
	for (int i = 0; i < gObjects.size(); i++)
		gObjects.at(i)->update();
	for (int i = 0; i < gTransObjects.size(); i++)
		gTransObjects.at(i)->update();

	
}

void Scene::setState(int id) {
	if (mId != id) {
		mId = id;
		free();
		init();
	}
}


std::vector<Abs_Entity*> Scene::getgObjects() {
	return this->gObjects;
}
//-------------------------------------------------------------------------


