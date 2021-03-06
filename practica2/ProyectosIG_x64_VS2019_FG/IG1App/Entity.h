//#pragma once
#ifndef _H_Entities_H_
#define _H_Entities_H_

#include <GL/freeglut.h>
#include <glm.hpp>
#include <GL/gl.h>
#include "Texture.h"
#include "Mesh.h"
#include "Light.h"

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
	void setMColor(glm::dvec4 const& mCol);
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

class QuadricEntity: public Abs_Entity {
public:
	QuadricEntity() {q = gluNewQuadric();};
	~QuadricEntity() { gluDeleteQuadric(q); };
	void setColor(glm::fvec3 color){ this->color = color;};

protected:
	glm::fvec3 color = glm::fvec3(-1.0, -1.0, -1.0);
	GLUquadricObj* q;
};

class Sphere: public QuadricEntity {
public:
	Sphere(GLdouble r);
	void render(glm::dmat4 const& modelViewMat) const;

protected:
	GLdouble r;
};

class Cylinder: public QuadricEntity {
public:
	Cylinder(GLdouble baseR, GLdouble topR, GLdouble height);
	void render(glm::dmat4 const& modelViewMat) const;

protected:
	GLdouble baseR;
	GLdouble topR;
	GLdouble height;
};

class Disk: public QuadricEntity {
public:
	Disk(GLdouble innerR, GLdouble outerR);
	void render(glm::dmat4 const& modelViewMat) const;

protected:
	GLdouble innerR;
	GLdouble outerR;
};

class PartialDisk: public QuadricEntity {
public:
	PartialDisk(GLdouble innerR, GLdouble outerR, GLint startAngle, GLint sweepAngle);
	void render(glm::dmat4 const& modelViewMat) const;

protected:
	GLdouble innerR;
	GLdouble outerR;
	GLint startAngle;
	GLint sweepAngle;
};

class SquaredRing : public Abs_Entity
{
public:
	explicit SquaredRing();
	void render(glm::dmat4 const& modelViewMat) const;
	
};


class EntityWithIndexMesh : public Abs_Entity
{
public:
	explicit EntityWithIndexMesh() {};
	void render(glm::dmat4 const& modelViewMat) const;
};

class Cube: public EntityWithIndexMesh {
public:
	Cube(GLdouble l);
	void render(glm::dmat4 const& modelViewMat) const;
	void setCopper(bool b) { copper = b; }
	void update();
protected:
	GLdouble l;
	bool copper = false;
};

class CompoundEntity: public Abs_Entity {
public:
	CompoundEntity() {};
	~CompoundEntity();
	void render(glm::dmat4 const& modelViewMat) const;
	void addEntity(Abs_Entity* ae);
	void update();
	std::vector<Abs_Entity*> getGObjects(){ return gObjects; };
protected:
	std::vector<Abs_Entity*> gObjects;
};

class Cone: public EntityWithIndexMesh {
	public:
		Cone(GLdouble h, GLdouble r, GLint n);
	protected:
		GLdouble h;
		GLdouble r;
		GLint n;
};

class EntityWithMaterial : public Abs_Entity{
	public:
		EntityWithMaterial () : Abs_Entity() { };
		virtual ~EntityWithMaterial () { };
		void setMaterial(Material*matl) { material = matl; };
	protected:
		Material *material = nullptr;
};

class Esfera : public EntityWithMaterial {
	public:
		Esfera(GLdouble r, GLint p, GLint m);
		void render(glm::dmat4 const& modelViewMat) const;
		void update();
		void setGold(bool b) { gold = b; };
	protected:
		GLdouble r;
		GLint p;
		GLint n;
		bool gold = false;
};

class EntityWithLight: public CompoundEntity {
public:
	EntityWithLight();
	~EntityWithLight() {if(light != nullptr) { delete light; light = nullptr; };};
	void render(glm::dmat4 const& modelViewMat) const;
	void switch_light(bool b);
protected:
	SpotLight *light = nullptr;
	bool enabledLight = false;
};

class Airplane: public EntityWithLight {
public:
	Airplane(GLfloat r) : EntityWithLight() {planetR = r; };
	~Airplane() { };
	void update();
protected:
	GLdouble angleX = 0.0;
	GLdouble traslationAngle = 90.0;
	GLdouble planetR;
	GLdouble propellersAngle = 0.0;
};

class Grid: public EntityWithIndexMesh {
public:
	Grid(GLdouble _side, GLuint _chunks);
	~Grid() { };
	void render(glm::dmat4 const& modelViewMat) const;
protected:
	GLdouble side;
	GLuint chunks;
};

class GridCube: public CompoundEntity {
public:
	GridCube(GLdouble _side, GLuint _chunks, Texture* vTx, Texture* hTx);
	~GridCube() {};
protected:
	GLdouble side;
	GLuint chunks;
};

class SirenCube: public EntityWithLight {
public:
	SirenCube(GLdouble _side, GLuint _chunks, GLdouble _rd, Texture* vTx, Texture* hTx);
	~SirenCube() {};
	void update();
protected:
	GLdouble side;
	GLuint chunks;
	GLdouble radius;
	GLdouble rotationRadius;
	GLdouble angleX = 0.0;
	GLdouble traslationAngle = 90.0;
	GLdouble lightAng = 0.0;
};

#endif //_H_Entities_H_