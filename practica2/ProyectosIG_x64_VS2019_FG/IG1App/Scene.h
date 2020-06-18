//#pragma once
#ifndef _H_Scene_H_
#define _H_Scene_H_

#include <GL/freeglut.h>
#include <glm.hpp>
#include "Texture.h"
#include "Camera.h"
#include "Entity.h"
#include "Light.h"

#include <vector>

//-------------------------------------------------------------------------

class Scene	
{ 
public:
	Scene() {};
	~Scene() { free(); resetGL(); };

	Scene(const Scene& s) = delete;  // no copy constructor
	Scene& operator=(const Scene& s) = delete;  // no copy assignment
		
	void init();
	void light0_switch(bool b);
	void light1_switch(bool b);
	void light2_switch(bool b);
	void light3_switch(bool b);
	void light_airplane_switch(bool b);
    void render(Camera const& cam) const;
	void sceneDirLight(Camera const& cam) const;
	void scenePosLight(Camera const& cam) const;
	void sceneSpotLight(Camera const& cam) const;
	void update();
	void setState(int id);
	void move();
	void turn_off_lights();

	std::vector<Abs_Entity*> getgObjects();
	static const int SCENE_1 = 0;
	static const int SCENE_2 = 1;
	static const int SCENE_3 = 2;
	static const int SCENE_4 = 3;
	static const int SCENE_5 = 4;

protected:
	void free();
	void setGL();
	void resetGL();
	void setScene1();
	void setScene2();
	void setScene3();
	void setScene4();
	void setScene5();
	void setLights();
	std::vector<Texture*> gTextures;
	std::vector<Abs_Entity*> gObjects;  // Entities (graphic objects) of the scene
	std::vector<Abs_Entity*> gTransObjects;
	bool light0 = false;
	bool light1 = false;
	bool light2 = false;
	bool light3 = false;
	int mId = SCENE_1;
	DirLight* directionalLight = nullptr;
	PosLight* positionalLight = nullptr;
	SpotLight* spotSceneLight = nullptr;
	PosLight* mineLight = nullptr;
	Airplane* airplane = nullptr;
};
//-------------------------------------------------------------------------

#endif //_H_Scene_H_

