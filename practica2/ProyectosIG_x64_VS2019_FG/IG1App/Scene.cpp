#include "Scene.h"
#include "CheckML.h"
#include <gtc/matrix_transform.hpp>  
#include <gtc/type_ptr.hpp>
#include <string>
#include <iostream>

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
	GLfloat amb[] = { 0, 0, 0, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
	
	gObjects.push_back(new RGBAxis(400.0));
	switch (mId) {
	case SCENE_1:
		setScene1();
		light0 = true;
		break;
	case SCENE_2:
		setScene2();
		light0 = true;
		break;
	case SCENE_3:
		setScene3();
		light0 = false;
		break;
	case SCENE_4:
		setScene4();
		light0 = false;
		break;
	case SCENE_5:
		setScene5();
		light0 = false;
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
	setLights();
	//Ex 18
	GLdouble planetR = 300.0;
	airplane = new Airplane(planetR);
	gObjects.push_back(airplane);

	CompoundEntity* propellers = new CompoundEntity();
	Cylinder* cyl1 = new Cylinder(9, 15, 50);
	Cylinder* cyl2 = new Cylinder(15, 9, 50);

	cyl1->setColor(glm::fvec3(0, 0, 1));
	glm::dmat4 mCyl1 = cyl1->modelMat();
	mCyl1 = translate(mCyl1, dvec3(-50, 0, 90));
	mCyl1 = rotate(mCyl1, radians(90.0), dvec3(0, 1.0, 0));
	cyl1->setModelMat(mCyl1);

	cyl2->setColor(glm::fvec3(0, 0, 1));
	glm::dmat4 mCyl2 = cyl2->modelMat();
	mCyl2 = translate(mCyl2, dvec3(0, 0, 90));
	mCyl2 = rotate(mCyl2, radians(90.0), dvec3(0, 1.0, 0));
	cyl2->setModelMat(mCyl2);

	propellers->addEntity(cyl1);
	propellers->addEntity(cyl2);

	CompoundEntity* chasis = new CompoundEntity();
	Sphere* ball = new Sphere(80.0);
	ball->setColor(glm::fvec3(1, 0, 0));
	chasis->addEntity(ball);
	chasis->addEntity(propellers);

	Cube* cube = new Cube(100.0);
	cube->setMColor(dvec4(0.0, 1.0, 0.0, 1.0));
	cube->setCopper(true);
	glm::dmat4 mCube = cube->modelMat();
	mCube = scale(mCube, dvec3(3.0, 0.2, 1));

	cube->setModelMat(mCube);
	airplane->addEntity(chasis);
	airplane->addEntity(cube);
	
	/* Ex 21
	Cone* cone = new Cone(200, 80, 400);
	cone->setMColor(dvec4(0.0, 0.0, 1.0, 1.0));
	gObjects.emplace_back(cone);
	*/


	//Ex 22
	Esfera* sphere = new Esfera(planetR, 250, 250);
	sphere->setMColor(dvec4(0.0, 1.0, 1.0, 1.0));
	sphere->setGold(false);
	
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
	mAirplane = translate(mAirplane, dvec3(0, 350, 0));
	mAirplane = scale(mAirplane, dvec3(0.3, 0.3, 0.3));
	airplane->setModelMat(mAirplane);
}

void Scene::setScene4() {
	setLights();

	Texture* tx1 = new Texture();
	std::string path = ".." + PATH_SEPARATOR + "Bmps" + PATH_SEPARATOR + "checker.bmp";
	tx1->load(path);

	Texture* tx2 = new Texture();
	path = ".." + PATH_SEPARATOR + "Bmps" + PATH_SEPARATOR + "stones.bmp";
	tx2->load(path);

	GridCube* cube = new GridCube(200.0, 10, tx1, tx2);

	gObjects.push_back(cube);
	
	gTextures.push_back(tx1);
	gTextures.push_back(tx2);
}

void Scene::setScene5() {
	setLights();

	GLdouble planetR = 200.0;
	Esfera* planet = new Esfera(planetR, 250, 250);
	planet->setMColor(dvec4(0.0, 1.0, 1.0, 1.0));

	Texture* tx1 = new Texture();
	std::string path = ".." + PATH_SEPARATOR + "Bmps" + PATH_SEPARATOR + "checker.bmp";
	tx1->load(path);

	Texture* tx2 = new Texture();
	path = ".." + PATH_SEPARATOR + "Bmps" + PATH_SEPARATOR + "stones.bmp";
	tx2->load(path);

	sirenCube = new SirenCube(200.0, 10, planetR, tx1, tx2);

	glm::dmat4 mCube = sirenCube->modelMat();
	mCube = translate(mCube, dvec3(0, planetR+30, 0));
	mCube = scale(mCube, dvec3(0.2, 0.2, 0.2));
	sirenCube->setModelMat(mCube);
	
	gObjects.emplace_back(planet);
	gObjects.push_back(sirenCube);
	gTextures.push_back(tx1);
	gTextures.push_back(tx2);
}

//-------------------------------------------------------------------------
void Scene::free() 
{ // release memory and resources   

	for (Abs_Entity* el : gObjects)
	{
		delete el;  el = nullptr;
	}
	gObjects.resize(0);
	airplane = nullptr;
	sirenCube = nullptr;

	for (Texture* tx : gTextures) {
		delete tx; tx = nullptr;
	}
	gTextures.resize(0);
	for (Abs_Entity* el : gTransObjects)
	{
		delete el;  el = nullptr;
	}
	gTransObjects.resize(0);

	if(directionalLight != nullptr){
		delete directionalLight; 
		directionalLight = nullptr;
	}
	if(positionalLight != nullptr){
		delete positionalLight; 
		positionalLight = nullptr;
	}
	if(spotSceneLight != nullptr){
		delete spotSceneLight; 
		spotSceneLight = nullptr;
	}
	if(mineLight != nullptr){
		delete mineLight; 
		mineLight = nullptr;
	}
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void Scene::setGL() 
{
	// OpenGL basic setting
	glClearColor(0.7, 0.8, 0.9, 1.0);  // background color (alpha=1 -> opaque)
	glEnable(GL_DEPTH_TEST);  // enable Depth test
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
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

void Scene::render(Camera const& cam) const {
	/*sceneDirLight(cam);
	scenePosLight(cam);
	sceneSpotLight(cam);*/

	switch (mId){
	case SCENE_3:
	case SCENE_5:
		directionalLight->upload(cam.viewMat());
		positionalLight->upload(cam.viewMat());
		spotSceneLight->upload(cam.viewMat());
		mineLight->upload(dmat4(1.0));
		break;
	case SCENE_4:
		spotSceneLight->upload(cam.viewMat());
		sceneDirLight(cam);
		break;
	default:
		sceneDirLight(cam);
		break;
	}

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
	if (light0) {
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
	else {
		glDisable(GL_LIGHT0);
	}
}

void Scene::scenePosLight(Camera const& cam) const {
	if (light1) {
		glEnable(GL_LIGHT1);
		glShadeModel(GL_SMOOTH);
		glm::fvec4 v = { 500.0, 500.0, 0.0, 1.0 };
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixd(value_ptr(cam.viewMat()));
		glLightfv(GL_LIGHT1, GL_POSITION, value_ptr(v));
		glm::fvec4 amb1 = { 0.0, 0.0, 0.0, 1.0 };
		glm::fvec4 dif1 = { 0.0, 1.0, 0.0, 1.0 };
		glm::fvec4 esp1 = { 0.5, 0.5, 0.5, 1.0 };
		glLightfv(GL_LIGHT1, GL_AMBIENT, value_ptr(amb1));
		glLightfv(GL_LIGHT1, GL_DIFFUSE, value_ptr(dif1));
		glLightfv(GL_LIGHT1, GL_SPECULAR, value_ptr(esp1));
	}
	else
		glDisable(GL_LIGHT1);
}

void Scene::sceneSpotLight(Camera const& cam) const {
	if (light2) {
		glEnable(GL_LIGHT2);
		glShadeModel(GL_SMOOTH);
		glm::fvec4 v = { 0.0, 0.0, 300.0, 1.0 };
		glm::fvec3 dir = { 0.0, 0.0, -1.0 };
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixd(value_ptr(cam.viewMat()));
		glLightfv(GL_LIGHT2, GL_POSITION, value_ptr(v));
		glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 180.0);
		glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 0.0);
		glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, value_ptr(dir));
		glm::fvec4 amb = { 0.0, 0.0, 0.0, 1.0 };
		glm::fvec4 dif = { 0.0, 1.0, 0.0, 1.0 };
		glm::fvec4 esp = { 0.5, 0.5, 0.5, 1.0 };
		glLightfv(GL_LIGHT2, GL_AMBIENT, value_ptr(amb));
		glLightfv(GL_LIGHT2, GL_DIFFUSE, value_ptr(dif));
		glLightfv(GL_LIGHT2, GL_SPECULAR, value_ptr(esp));
	}
	else
		glDisable(GL_LIGHT2);
}

void Scene::light0_switch(bool b) {
	light0 = b;
	if(directionalLight != nullptr){
		if(b)
			directionalLight->enable();
		else
			directionalLight->disable();
	}
}

void Scene::light1_switch(bool b) {
	light1 = b;
	if(positionalLight != nullptr){
		if(b)
			positionalLight->enable();
		else
			positionalLight->disable();
	}
}

void Scene::light2_switch(bool b) {
	light2 = b;
	if(spotSceneLight != nullptr){
		if(b)
			spotSceneLight->enable();
		else
			spotSceneLight->disable();
	}
}

void Scene::light3_switch(bool b) {
	light3 = b;
	if(mineLight != nullptr){
		if(b)
			mineLight->enable();
		else
			mineLight->disable();
	}
}

void Scene::entity_light_switch(bool b) {
	if(airplane != nullptr)
		airplane->switch_light(b);
	if(sirenCube != nullptr)
		sirenCube->switch_light(b);
}

void Scene::turn_off_lights() {
	GLfloat amb[] = { 0, 0, 0, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
	light0_switch(false);
	light1_switch(false);
	light2_switch(false);
	light3_switch(false);
	entity_light_switch(false);
}

void Scene::setLights() {
	switch (mId) {
	case SCENE_3:
	case SCENE_5:
		directionalLight = new DirLight();
		directionalLight->setPosDir(glm::fvec3(1, 1, 1));
		directionalLight->setAmbient(glm::fvec4(0, 0, 0, 1));
		directionalLight->setDiffuse(glm::fvec4(1, 1, 1, 1));
		directionalLight->setSpecular(glm::fvec4(0.5, 0.5, 0.5, 1));
		directionalLight->disable();

		positionalLight = new PosLight();
		positionalLight->setPosDir(glm::fvec3(500.0, 500.0, 0.0));
		positionalLight->setAmbient(glm::fvec4(0, 0, 0, 1));
		positionalLight->setDiffuse(glm::fvec4(0, 1, 0, 1));
		positionalLight->setSpecular(glm::fvec4(0.5, 0.5, 0.5, 1));
		positionalLight->disable();

		spotSceneLight = new SpotLight(glm::fvec3(0, 0, 400.0));
		spotSceneLight->setAmbient(glm::fvec4(0, 0, 0, 1));
		spotSceneLight->setDiffuse(glm::fvec4(0, 1, 0, 1));
		spotSceneLight->setSpecular(glm::fvec4(0.5, 0.5, 0.5, 1));
		// spotSceneLight->setSpot(glm::fvec3(0, 1.0, -1.0), 90.0, 3.0); // diagonal spot
		spotSceneLight->disable();

		mineLight = new PosLight();
		mineLight->setAmbient(glm::fvec4(0, 0, 0, 1));
		mineLight->setDiffuse(glm::fvec4(1, 1, 1, 1));
		mineLight->setSpecular(glm::fvec4(0.5, 0.5, 0.5, 1));
		mineLight->disable();
		break;

	case SCENE_4:
		directionalLight = new DirLight();
		directionalLight->setPosDir(glm::fvec3(0, 0, -1));
		directionalLight->setAmbient(glm::fvec4(0, 0, 0, 1));
		directionalLight->setDiffuse(glm::fvec4(1, 1, 1, 1));
		directionalLight->setSpecular(glm::fvec4(0.5, 0.5, 0.5, 1));
		directionalLight->disable();

		spotSceneLight = new SpotLight(glm::fvec3(0, 0, 300.0));
		spotSceneLight->setDiffuse(glm::fvec4(1, 1, 1, 1));
		spotSceneLight->setAmbient(glm::fvec4(0, 0, 0, 1));
		spotSceneLight->setSpecular(glm::fvec4(0.5, 0.5, 0.5, 1));
		spotSceneLight->setSpot(glm::fvec3(0, 0, -1.0), 45.0, 20);
		spotSceneLight->disable();

	 	break;

	default:
		break;
	}
}

void Scene::move() {
	sirenMove();
	airplaneMove();
}

void Scene::airplaneMove(){
	if(airplane != nullptr)
		airplane->update();
}

void Scene::sirenMove(){
	if(sirenCube != nullptr)
		sirenCube->update();
}
//-------------------------------------------------------------------------
