#include "Mesh.h"
#include "CheckML.h"
#include <fstream>
using namespace std;
using namespace glm;

//-------------------------------------------------------------------------

void Mesh::draw() const 
{
  glDrawArrays(mPrimitive, 0, size());   // primitive graphic, first index and number of elements to be rendered
}
//-------------------------------------------------------------------------

void Mesh::render() const 
{
  if (vVertices.size() > 0) {  // transfer data
    // transfer the coordinates of the vertices
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_DOUBLE, 0, vVertices.data());  // number of coordinates per vertex, type of each coordinate, stride, pointer 
    if (vColors.size() > 0) { // transfer colors
      glEnableClientState(GL_COLOR_ARRAY);
      glColorPointer(4, GL_DOUBLE, 0, vColors.data());  // components number (rgba=4), type of each component, stride, pointer  
    }
    if (vTexCoords.size() > 0) { // transfer colors
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glColorPointer(2, GL_DOUBLE, 0, vTexCoords.data());  // components number (rgba=4), type of each component, stride, pointer  
    }
	draw();

    glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  }
}
//-------------------------------------------------------------------------

Mesh * Mesh::createRGBAxes(GLdouble l)
{
  Mesh* mesh = new Mesh();

  mesh->mPrimitive = GL_LINES;

  mesh->mNumVertices = 6;
  mesh->vVertices.reserve(mesh->mNumVertices);

  // X axis vertices
  mesh->vVertices.emplace_back(0.0, 0.0, 0.0);
  mesh->vVertices.emplace_back(l, 0.0, 0.0);
  // Y axis vertices
  mesh->vVertices.emplace_back(0, 0.0, 0.0);
  mesh->vVertices.emplace_back(0.0, l, 0.0);
  // Z axis vertices
  mesh->vVertices.emplace_back(0.0, 0.0, 0.0);
  mesh->vVertices.emplace_back(0.0, 0.0, l);

  mesh->vColors.reserve(mesh->mNumVertices);
  // X axis color: red  (Alpha = 1 : fully opaque)
  mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
  mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
  // Y axis color: green
  mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
  mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
  // Z axis color: blue
  mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);
  mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);
 
  return mesh;
}
//-------------------------------------------------------------------------

Mesh* Mesh::generatePolygon(GLuint numL, GLdouble rd) {
    Mesh* mesh = new Mesh();
    mesh->mPrimitive = GL_LINE_LOOP;
    mesh->mNumVertices = numL;
    mesh->vVertices.reserve(mesh->mNumVertices);
    GLdouble vertex_X = 0;
    GLdouble vertex_Y = 0;
    GLdouble ang = 90.0;
    GLdouble incr = 360.0 / numL;

    for (int i = 0; i < numL; i++) {
        vertex_X = rd * cos(radians(ang));
        vertex_Y = rd * sin(radians(ang));
         
        mesh->vVertices.emplace_back(vertex_X, vertex_Y, 0.0);

        ang += incr;
    }


    return mesh;
}

Mesh* Mesh::generateSierpinski(GLuint numP, GLdouble rd) {
    Mesh* triangle = generatePolygon(3, rd);
    Mesh* mesh = new Mesh();

    mesh->mPrimitive = GL_POINTS;
    mesh->mNumVertices = numP;
    mesh->vVertices.reserve(mesh->mNumVertices);

    mesh->vVertices = triangle->vVertices;

    dvec3 p0 = mesh->vVertices[rand() % 2];
    dvec3 p1 = mesh->vVertices[2];
   
    mesh->vVertices.emplace_back((p0.x + p1.x) / 2, 
                                 (p0.y + p1.y) / 2, 
                                 (p0.z + p1.z) / 2);

    for (int i = 4; i < numP; i++)
    {
        p0 = mesh->vVertices[rand() % 3];
        p1 = mesh->vVertices[i-1];
        mesh->vVertices.emplace_back((p0.x + p1.x) / 2,
                                    (p0.y + p1.y) / 2,
                                    (p0.z + p1.z) / 2);
        
    }
    delete triangle; triangle = nullptr;
    return mesh;
}


Mesh* Mesh::generateTriangleRGB(GLdouble rd) {
    Mesh* triangle = generatePolygon(3, rd);
    Mesh* mesh = new Mesh();

    mesh->mPrimitive = GL_TRIANGLES;
    mesh->mNumVertices = 3;
    mesh->vVertices = triangle->vVertices;

    mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
    mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
    mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);

    return mesh;
}

Mesh* Mesh::generateRectangle(GLdouble w, GLdouble h) {
    
    Mesh* mesh = new Mesh();

    mesh->mPrimitive = GL_TRIANGLE_STRIP;
    mesh->mNumVertices = 4;
    mesh->vVertices.reserve(mesh->mNumVertices);

    GLdouble vertex_X = 0;
    GLdouble vertex_Y = 0;
    // circle diagonals
    GLdouble ang[4] = {135, 225, 45, 315};
    
    for (int i = 0; i < 4; i++) {
        vertex_X = w * cos(radians(ang[i]));
        vertex_Y = h * sin(radians(ang[i]));
        mesh->vVertices.emplace_back(vertex_X, vertex_Y, 0.0);
    }
    return mesh;
}

Mesh* Mesh::generateRGBRectangle(GLdouble w, GLdouble h) {
    Mesh* rectangle = generateRectangle(w, h);
    Mesh* mesh = new Mesh();

    mesh->mPrimitive = GL_TRIANGLE_STRIP;
    mesh->mNumVertices = rectangle->mNumVertices;
    mesh->vVertices.reserve(mesh->mNumVertices);
    mesh->vVertices = rectangle->vVertices;

    mesh->vColors.emplace_back(0.11764, 0.5647, 1.0, 1.0);
    mesh->vColors.emplace_back(1.0, 0.3882, 0.2784, 1.0);
    mesh->vColors.emplace_back(0.5960, 0.9843, 0.5961, 1.0);
    mesh->vColors.emplace_back(0.11764, 0.5647, 1.0, 1.0);

    return mesh;
}


Mesh* Mesh::generate3DStar(GLdouble re, GLdouble np, GLdouble h) {
    Mesh* mesh = new Mesh();

    mesh->mPrimitive = GL_TRIANGLE_FAN;
    mesh->mNumVertices = 2 * np + 2;
    mesh->vVertices.reserve(mesh->mNumVertices);

    GLdouble ri = re / 2;
    GLdouble vertex_X = 0;
    GLdouble vertex_Y = 0;
    GLdouble ang = 90.0;
    GLdouble incr = 360.0 / np;

    //Center
    mesh->vVertices.emplace_back(0.0, 0.0, 0.0);

    for (int i = 0; i < np; i++) {
        vertex_X = re * cos(radians(ang));
        vertex_Y = re * sin(radians(ang));
        mesh->vVertices.emplace_back(vertex_X, vertex_Y, h);

        vertex_X = ri * cos(radians((ang + ang + incr) / 2));
        vertex_Y = ri * sin(radians((ang + ang + incr) / 2));
        mesh->vVertices.emplace_back(vertex_X, vertex_Y, h);

        ang += incr;
    }

    // First vertex
    mesh->vVertices.emplace_back(mesh->vVertices[1].x, mesh->vVertices[1].y, h);

    return mesh;
}