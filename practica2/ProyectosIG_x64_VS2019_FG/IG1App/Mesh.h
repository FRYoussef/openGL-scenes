//#pragma once
#ifndef _H_Mesh_H_
#define _H_Mesh_H_

#include <GL/freeglut.h>
#include <glm.hpp>

#include <vector>

//-------------------------------------------------------------------------

class Mesh 
{
public:

	static Mesh* createRGBAxes(GLdouble l); // creates a new 3D-RGB axes mesh
	static Mesh* generatePolygon(GLuint numL, GLdouble rd);
	static Mesh* generateSierpinski(GLuint numP, GLdouble rd);
	static Mesh* generateTriangleRGB(GLdouble rd);
	static Mesh* generateRectangle(GLdouble w, GLdouble h);
	static Mesh* generateRGBRectangle(GLdouble w, GLdouble h);
	static Mesh* generate3DStar(GLdouble re, GLdouble np, GLdouble h);
	static Mesh* generateStarTexCoord(GLdouble re, GLuint np, GLdouble h);
	static Mesh* generateRectangleTexCoord(GLdouble w, GLdouble h, GLuint rw, GLuint rh);
	static Mesh* generateContCube(GLdouble ld);
	static Mesh* generateBoxTextCoord(GLdouble nl);
	static std::vector<Mesh*> generate3dObject(GLuint times, GLdouble w, GLdouble h);
	static Mesh* generateSquaredRing();
	void setVColors(std::vector<glm::dvec4> vc) { vColors = vc; };
	Mesh() {};
	virtual ~Mesh() {};

	Mesh(const Mesh & m) = delete;  // no copy constructor
	Mesh & operator=(const Mesh & m) = delete;  // no copy assignment
			
	virtual void render() const;
	
	GLuint size() const { return mNumVertices; };   // number of elements
	std::vector<glm::dvec3> const& vertices() const { return vVertices; };
	std::vector<glm::dvec4> const& colors() const { return vColors; };
	
		
protected:
	
	GLuint mPrimitive = GL_TRIANGLES;   // graphic primitive: GL_POINTS, GL_LINES, GL_TRIANGLES, ...
	GLuint mNumVertices = 0;  // number of elements ( = vVertices.size())
	std::vector<glm::dvec3> vVertices;  // vertex array
	std::vector<glm::dvec4> vColors;    // color array
	std::vector<glm::dvec2> vTexCoords;    // texture array
	std::vector<unsigned int> vIndices;
	std::vector<glm::dvec3> vNormals;
	virtual void draw() const;
};

class IndexMesh: public Mesh {

	protected:
		GLuint* vIndexes = nullptr;
		GLuint nNumIndices = 0;

		virtual void draw() const;
		void buildNormalVectors();

	public:
		IndexMesh() { mPrimitive = GL_TRIANGLES; };
		~IndexMesh() { delete[] vIndexes; };
		virtual void render() const;
		static IndexMesh* generateIndexCubeWithLids(GLdouble l);
};

class MbR: public IndexMesh {
	protected:
		int n;
		glm::dvec3* perfil;
		int m;
		
	public:
		
		MbR(int nn, int mm, glm::dvec3* p) { n = nn; perfil = p; m = mm; };
		
		static MbR* generateIndexMeshByRevolution(int mm, int nn, glm::dvec3* perfil);
};
//-------------------------------------------------------------------------

#endif //_H_Scene_H_