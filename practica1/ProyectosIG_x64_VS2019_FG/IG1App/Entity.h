//#pragma once
#ifndef _H_Entities_H_
#define _H_Entities_H_

#include "GLinclude.h"
#include "Texture.h"
#include "Mesh.h"
#include "Viewport.h"
#include "Camera.h"


//-------------------------------------------------------------------------

class Abs_Entity  // abstract class
{
public:
	Abs_Entity(): mModelMat(1.0), mColor(1) {};  // 4x4 identity matrix
	virtual ~Abs_Entity() { delete mMesh; mMesh = nullptr;};

	Abs_Entity(const Abs_Entity& e) = delete;  // no copy constructor
	Abs_Entity& operator=(const Abs_Entity& e) = delete;  // no copy assignment

	virtual void render(glm::dmat4 const& modelViewMat) const = 0;  // abstract method

	// modeling matrix
	glm::dmat4 const& modelMat() const { return mModelMat; };
	void setModelMat(glm::dmat4 const& aMat) { mModelMat = aMat; }
	void setMColor(glm::dvec4 const& mCol) { mColor = mCol; };
	void setTexture(Texture* tex) { mTexture = tex; }
	void setiTexture(Texture* tex) { iTexture = tex; }
	Texture* getTexture() { return mTexture; }
	virtual void update() {};
	
protected:

	Mesh* mMesh = nullptr;   // the mesh
	glm::dmat4 mModelMat;    // modeling matrix
	glm::dvec4 mColor;
	Texture* mTexture = nullptr;
	Texture* iTexture = nullptr;
	// transfers modelViewMat to the GPU
	virtual void upload(glm::dmat4 const& mModelViewMat) const; 
};
//-------------------------------------------------------------------------

class RGBAxis : public Abs_Entity 
{
public:
	explicit RGBAxis(GLdouble l);
	~RGBAxis();
	void render(glm::dmat4 const& modelViewMat) const;
};

class CPolygon : public Abs_Entity
{
public:
	explicit CPolygon(GLuint numL, GLdouble rd);
	void render(glm::dmat4 const& modelViewMat) const;

protected:
	GLuint numL;
	GLdouble rd;
};

class Sierpinski : public Abs_Entity
{
public:
	explicit Sierpinski(GLuint numP, GLdouble rd);
	void render(glm::dmat4 const& modelViewMat) const;

protected:
	GLuint numP;
	GLdouble rd;
};

class TriangleRGB : public Abs_Entity
{
public:
	explicit TriangleRGB(GLdouble rd);
	void render(glm::dmat4 const& modelViewMat) const;
	void update();

protected:
	GLdouble rd;
	GLdouble traslationAng;
	GLdouble rotationAng;
};

class RectangleRGB : public Abs_Entity
{
public:
	explicit RectangleRGB(GLdouble w, GLdouble h);
	void render(glm::dmat4 const& modelViewMat) const;

protected:
	GLdouble w;
	GLdouble h;
};

class _3DStar : public Abs_Entity
{
public:
	explicit _3DStar(GLdouble re, GLdouble np, GLdouble h);
	void render(glm::dmat4 const& modelViewMat) const;
	void update();

protected:
	GLdouble re;
	GLdouble np;
	GLdouble h;
	GLdouble angleY;
	GLdouble angleZ;
};

class Floor : public Abs_Entity
{
public:
	explicit Floor(GLdouble w, GLdouble h, GLuint rw, GLuint rh);
	void render(glm::dmat4 const& modelViewMat) const;

protected:
	GLdouble w;
	GLdouble h;
	GLuint rw;
	GLuint rh;
};

class Box : public Abs_Entity
{
public:
	explicit Box(GLdouble ld);
	void render(glm::dmat4 const& modelViewMat) const;

protected:
	GLdouble ld;
};

class Picture : public Abs_Entity
{
public:
	explicit Picture(GLdouble ld1, GLdouble ld2);
	void render(glm::dmat4 const& modelViewMat) const;

protected:
	GLdouble ld1;
	GLdouble ld2;
};

class Grass : public Abs_Entity
{
public:
	explicit Grass(GLuint times, GLdouble width, GLdouble height);
	void render(glm::dmat4 const& modelViewMat) const;

protected:
	GLdouble width;
	GLdouble height;
	std::vector<Mesh*> gMesh;
};

class Glass : public Abs_Entity
{
public:
	explicit Glass(GLdouble ld);
	void render(glm::dmat4 const& modelViewMat) const;

protected:
	GLdouble ld;
};
//-------------------------------------------------------------------------

class Background { // EXTRA 1
public:
	explicit Background();
	~Background();
	void render() const;
	void setSizeVP(int xw, int yh);
	void setTexture(const std::string &tx);
	void settTexture(const std::string& tx); //EXTRA 2
	void setTwoUnits() { mMesh->setTwoUnits(); }; //EXTRRA 2
protected:
	Mesh* mMesh = nullptr;   // the mesh
	Texture* mTexture = nullptr;
	Texture* tTexture = nullptr; //EXTRA 2
	Viewport* mViewPort = nullptr; // Viewport position and size
	Camera* mCamera = nullptr; 	// Camera position, view volume and projection
};

#endif //_H_Entities_H_