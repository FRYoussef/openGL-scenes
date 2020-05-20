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
	case SCENE_1:
		setScene1();
		break;
	case SCENE_2:
		setScene2();
		break;
	case SCENE_3:
		setScene3();
		break;
	default:
		break;
	}
}

void Scene::setScene1() {
	Sphere* face = new Sphere(100.0);
	face->setColor(glm::fvec3(1, 0.5, 0));

	Cylinder* eye1 = new Cylinder(15, 0, 30);
	eye1->setColor(glm::fvec3(0, 0, 1));
	glm::dmat4 mEye1 = eye1->modelMat();
	mEye1 = translate(mEye1, dvec3(-40, 30, 80));
	eye1->setModelMat(mEye1);

	Cylinder* eye2 = new Cylinder(15, 0, 30);
	eye2->setColor(glm::fvec3(0.2, 1, 1));
	glm::dmat4 mEye2 = eye2->modelMat();
	mEye2 = translate(mEye2, dvec3(40, 30, 80));
	eye2->setModelMat(mEye2);

	PartialDisk* smile = new PartialDisk(70.0, 80.0, 90, 180);
	smile->setColor(glm::fvec3(0, 1, 0));
	glm::dmat4 mSmile = smile->modelMat();
	mSmile = translate(mSmile, dvec3(0, 10, 80));
	smile->setModelMat(mSmile);

	Disk* hat = new Disk(60.0, 110.0);
	hat->setColor(glm::fvec3(1, 0, 0));
	glm::dmat4 mHat = hat->modelMat();
	mHat = translate(mHat, dvec3(0, 70, 0));
	mHat= rotate(mHat, radians(70.0), dvec3(1.0, 0, 0));
	hat->setModelMat(mHat);

	gObjects.push_back(face);
	gObjects.push_back(eye1);
	gObjects.push_back(eye2);
	gObjects.push_back(smile);
	gObjects.push_back(hat);
}


void Scene::setScene2() {

	SquaredRing* sqRing = new SquaredRing();

	Cube* cube = new Cube(100.0);

	//gObjects.push_back(sqRing);
	gObjects.push_back(cube);
}

void Scene::setScene3() {
	//Ex 18
	CompoundEntity* airplane = new CompoundEntity();
	gObjects.push_back(airplane);

	CompoundEntity* propellers = new CompoundEntity();
	Cylinder* cyl1 = new Cylinder(9, 15, 50);
	Cylinder* cyl2 = new Cylinder(15, 9, 50);

	cyl1->setColor(glm::fvec3(0, 0, 1));
	glm::dmat4 mCyl1 = cyl1->modelMat();
	mCyl1 = translate(mCyl1, dvec3(-50, 80, 90));
	mCyl1 = rotate(mCyl1, radians(90.0), dvec3(0, 1.0, 0));
	cyl1->setModelMat(mCyl1);

	cyl2->setColor(glm::fvec3(0, 0, 1));
	glm::dmat4 mCyl2 = cyl2->modelMat();
	mCyl2 = translate(mCyl2, dvec3(0, 80, 90));
	mCyl2 = rotate(mCyl2, radians(90.0), dvec3(0, 1.0, 0));
	cyl2->setModelMat(mCyl2);

	propellers->addEntity(cyl1);
	propellers->addEntity(cyl2);

	CompoundEntity* chasis = new CompoundEntity();
	Sphere* ball = new Sphere(80.0);
	glm::dmat4 mBall = ball->modelMat();
	mBall = translate(mBall, dvec3(0, 80, 0));
	ball->setModelMat(mBall);
	ball->setColor(glm::fvec3(1, 0, 0));
	chasis->addEntity(ball);
	chasis->addEntity(propellers);

	Cube* cube = new Cube(100.0);
	cube->setMColor(dvec4(0.0, 1.0, 0.0, 1.0));
	glm::dmat4 mCube = cube->modelMat();

	mCube = translate(mCube, dvec3(0, 80, 0));
	mCube = scale(mCube, dvec3(2.5, 0.2, 1));

	cube->setModelMat(mCube);
	airplane->addEntity(chasis);
	airplane->addEntity(cube);
	
	/* Ex 21
	Cone* cone = new Cone(200, 80, 400);
	cone->setMColor(dvec4(0.0, 0.0, 1.0, 1.0));
	gObjects.emplace_back(cone);
	*/


	//Ex 22
	Esfera* sphere = new Esfera(200, 250, 250);
	sphere->setMColor(dvec4(0.0, 1.0, 1.0, 1.0));
	gObjects.emplace_back(sphere);

	/*Ex 23
	glm::dmat4 mSphere1 = sphere->modelMat();
	mSphere1 = translate(mSphere1, dvec3(300, 0, 60));
	sphere->setModelMat(mSphere1);

	Sphere* sphere2 = new Sphere(100);
	glm::dmat4 mSphere2 = sphere2->modelMat();
	mSphere2 = translate(mSphere2, dvec3(60, 0, 300));
	sphere2->setModelMat(mSphere2);
	sphere2->setColor(dvec4(0.0, 1.0, 1.0, 1.0));
	gObjects.emplace_back(sphere2);
	*/

	//Ex 24
	glm::dmat4 mAirplane = airplane->modelMat();
	mAirplane = translate(mAirplane, dvec3(0, 200, 0));
	mAirplane = scale(mAirplane, dvec3(0.5, 0.5, 0.5));
	airplane->setModelMat(mAirplane);

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

//-------------------------------------------------------------------------
void Scene::setGL() 
{
	// OpenGL basic setting
	glClearColor(0.7, 0.8, 0.9, 0.0);  // background color (alpha=1 -> opaque)
	glEnable(GL_DEPTH_TEST);  // enable Depth test
}
//-------------------------------------------------------------------------
void Scene::resetGL() 
{
	glClearColor(.0, .0, .0, .0);  // background color (alpha=1 -> opaque)
	glDisable(GL_DEPTH_TEST);  // disable Depth test 	
}
//-------------------------------------------------------------------------

void Scene::render(Camera const& cam) const {
	sceneDirLight(cam);
	
	cam.upload();
	for (Abs_Entity* el : gObjects)
		el->render(cam.viewMat());

	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (Abs_Entity* el : gTransObjects)
		el->render(cam.viewMat());

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

void Scene::sceneDirLight(Camera const& cam) const {
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glm::fvec4 posDir = { 1, 1, 1, 0 };
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(value_ptr(cam.viewMat()));
	glLightfv(GL_LIGHT0, GL_POSITION, value_ptr(posDir));
	glm::fvec4 ambient = { 0, 0, 0, 1 };
	glm::fvec4 diffuse = { 1, 1, 1, 1 };
	glm::fvec4 specular = { 0.5, 0.5, 0.5, 1 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, value_ptr(ambient));
	glLightfv(GL_LIGHT0, GL_DIFFUSE, value_ptr(diffuse));
	glLightfv(GL_LIGHT0, GL_SPECULAR, value_ptr(specular));
}
//-------------------------------------------------------------------------


