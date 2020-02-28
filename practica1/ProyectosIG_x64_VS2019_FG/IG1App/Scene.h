//#pragma once
#ifndef _H_Scene_H_
#define _H_Scene_H_

#include <GL/freeglut.h>
#include <glm.hpp>
#include "Texture.h"
#include "Camera.h"
#include "Entity.h"

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

    void render(Camera const& cam) const;
	void update();
	void setState(int id);
	std::vector<Abs_Entity*> getgObjects();
	static const int SCENE_2D = 0;
	static const int SCENE_3D = 1;

protected:
	void free();
	void setGL();
	void resetGL();
	std::vector<Texture*> gTextures;
	std::vector<Abs_Entity*> gObjects;  // Entities (graphic objects) of the scene
	std::vector<Abs_Entity*> gTransObjects;
	void scene2D();
	void scene3D();
	int mId = 0;
};
//-------------------------------------------------------------------------

#endif //_H_Scene_H_

