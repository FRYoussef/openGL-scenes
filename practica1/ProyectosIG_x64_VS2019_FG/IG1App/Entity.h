//#pragma once
#ifndef _H_Entities_H_
#define _H_Entities_H_

#include <GL/freeglut.h>
#include <glm.hpp>
#include "Texture.h"
#include "Mesh.h"

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
	void setModelMat(glm::dmat4 const& aMat) { mModelMat = aMat; };
	void setMColor(glm::dvec4 const& mCol) { mColor = mCol; };
	void setTexture(Texture* tex) { mTexture = tex; }
	virtual void update() {};
	
protected:

	Mesh* mMesh = nullptr;   // the mesh
	glm::dmat4 mModelMat;    // modeling matrix
	glm::dvec4 mColor;
	Texture* mTexture = nullptr;
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

//-------------------------------------------------------------------------

#endif //_H_Entities_H_