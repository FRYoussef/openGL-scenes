#include "Mesh.h"
#include "CheckML.h"
#include <fstream>
#include <gtc/matrix_transform.hpp>  
#include <iostream>
using namespace std;
using namespace glm;

//-------------------------------------------------------------------------

void Mesh::draw() const
{
    glDrawArrays(mPrimitive, 0, size());   // primitive graphic, first index and number of elements to be rendered
    /* EJERCICIO 8
    if (vIndexes.empty()) {
        glDrawArrays(mPrimitive, 0, size());   // primitive graphic, first index and number of elements to be rendered
    }
    else {

        unsigned int stripIndices[10];
        for (int i = 0; i < 10; i++) {
            stripIndices[i] = vIndexes[i];
        }
 
        glDrawElements(mPrimitive, 10, GL_UNSIGNED_INT, stripIndices);
    }*/
  
    
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
        glTexCoordPointer(2, GL_DOUBLE, 0, vTexCoords.data());  // components number (rgba=4), type of each component, stride, pointer  
    }
    if(vNormals.size() > 0){
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_DOUBLE, 0, vNormals.data());
    }

	draw();

    glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
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

    /*GLdouble vertex_X = 0;
    GLdouble vertex_Y = 0;
    // circle diagonals
    GLdouble ang[4] = {135, 225, 45, 315};
    
    for (int i = 0; i < 4; i++) {
        vertex_X = w * cos(radians(ang[i]));
        vertex_Y = h * sin(radians(ang[i]));
        mesh->vVertices.emplace_back(vertex_X, vertex_Y, 0.0);
    }*/

    mesh->vVertices.emplace_back(-(w / 2), 0, (h / 2));
    mesh->vVertices.emplace_back((w / 2), 0, (h / 2));
    mesh->vVertices.emplace_back(-(w / 2), 0, -(h / 2));
    mesh->vVertices.emplace_back((w / 2), 0, -(h / 2));
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

Mesh* Mesh::generateStarTexCoord(GLdouble re, GLuint np, GLdouble h) {
    Mesh* mesh = generate3DStar(re, np, h);

    mesh->vTexCoords.reserve(mesh->mNumVertices);

    GLdouble ri = 0.25;
    GLdouble c = 0.5;
    GLdouble vertex_X = 0;
    GLdouble vertex_Y = 0;
    GLdouble ang = 90.0;
    GLdouble incr = 360.0 / np;
    re = 0.5;

    mesh->vTexCoords.emplace_back(c, c);

    for (int i = 0; i < np; i++) {
        vertex_X = c + re * cos(radians(ang));
        vertex_Y = c + re * sin(radians(ang));
        mesh->vTexCoords.emplace_back(vertex_X, vertex_Y);

        vertex_X = c + ri * cos(radians((ang + ang + incr) / 2));
        vertex_Y = c + ri * sin(radians((ang + ang + incr) / 2));
        mesh->vTexCoords.emplace_back(vertex_X, vertex_Y);

        ang += incr;
    }

    // First vertex
    mesh->vTexCoords.emplace_back(mesh->vTexCoords[1].x, mesh->vTexCoords[1].y);

    return mesh;
}

Mesh* Mesh::generateRectangleTexCoord(GLdouble w, GLdouble h, GLuint rw, GLuint rh) {
    Mesh* mesh = generateRectangle(w, h);

    mesh->vTexCoords.reserve(mesh->mNumVertices);

    mesh->vTexCoords.emplace_back(0.0, rh);
    mesh->vTexCoords.emplace_back(0.0, 0.0);
    mesh->vTexCoords.emplace_back(rw, rh);
    mesh->vTexCoords.emplace_back(rw, 0.0);

    return mesh;
}

Mesh* Mesh::generateContCube(GLdouble ld) {
    Mesh* mesh = new Mesh();

    mesh->mPrimitive = GL_TRIANGLE_STRIP;
    mesh->mNumVertices = 14;
    mesh->vVertices.reserve(mesh->mNumVertices);

    GLdouble size = ld / 2;

    //V0
    mesh->vVertices.emplace_back(-size, size, size);

    //V1
    mesh->vVertices.emplace_back(-size, -size, size);


    //V2
    mesh->vVertices.emplace_back(size, size, size);


    //V3
    mesh->vVertices.emplace_back(size, -size, size);


    //V4
    mesh->vVertices.emplace_back(size, size, -size);


    //V5
    mesh->vVertices.emplace_back(size, -size, -size);

    
    //V6
    mesh->vVertices.emplace_back(-size, size, -size);


    //V7
    mesh->vVertices.emplace_back(-size, -size, -size);

   
    mesh->vVertices.emplace_back(mesh->vVertices[0]);
    mesh->vVertices.emplace_back(mesh->vVertices[1]);

    // suelo
    mesh->vVertices.emplace_back(mesh->vVertices[1]);
    mesh->vVertices.emplace_back(mesh->vVertices[7]);
    mesh->vVertices.emplace_back(mesh->vVertices[3]);
    mesh->vVertices.emplace_back(mesh->vVertices[5]);

    return mesh;
}

Mesh* Mesh::generateBoxTextCoord(GLdouble nl) {
    Mesh* mesh = generateContCube(nl);

    mesh->vTexCoords.reserve(mesh->mNumVertices);

    mesh->vTexCoords.emplace_back(0.0, 1);
    mesh->vTexCoords.emplace_back(0.0, 0.0);
    mesh->vTexCoords.emplace_back(1, 1);
    mesh->vTexCoords.emplace_back(1, 0.0);

    mesh->vTexCoords.emplace_back(0.0, 1);
    mesh->vTexCoords.emplace_back(0.0, 0.0);
    mesh->vTexCoords.emplace_back(1, 1);
    mesh->vTexCoords.emplace_back(1, 0.0);

    mesh->vTexCoords.emplace_back(0.0, 1);
    mesh->vTexCoords.emplace_back(0.0, 0.0);
    mesh->vTexCoords.emplace_back(1, 1);
    mesh->vTexCoords.emplace_back(1, 0.0);

    mesh->vTexCoords.emplace_back(0.0, 1);
    mesh->vTexCoords.emplace_back(0.0, 0.0);
    mesh->vTexCoords.emplace_back(1, 1);
    mesh->vTexCoords.emplace_back(1, 0.0);

    mesh->vTexCoords.emplace_back(0.0, 1);
    mesh->vTexCoords.emplace_back(0.0, 0.0);
    mesh->vTexCoords.emplace_back(1, 1);
    mesh->vTexCoords.emplace_back(1, 0.0);

    return mesh;
}

vector<Mesh*> Mesh::generate3dObject(GLuint times, GLdouble w, GLdouble h) {
    vector<Mesh*> gMesh;
    GLdouble vertex_X = 0, vertex_Y = h, vertex_Z = 0, ang = 0, incr = 360.0 / (2 * (times-1));

    for (int i = 0; i < times; i++) {

        Mesh* mesh = new Mesh();
        mesh->mPrimitive = GL_TRIANGLE_STRIP;
        mesh->mNumVertices = 4;
        mesh->vVertices.reserve(mesh->mNumVertices);

        //V0
        vertex_X = (w / 2) * sin(radians(ang));
        vertex_Z = (w / 2) * cos(radians(ang));
        mesh->vVertices.emplace_back(vertex_X, vertex_Y, vertex_Z);
        
        //V1
        vertex_X = (w / 2) * sin(radians(ang));
        vertex_Z = (w / 2) * cos(radians(ang));
        mesh->vVertices.emplace_back(vertex_X, 0, vertex_Z);

        //V2
        vertex_X = -(w / 2) * sin(radians(ang));
        vertex_Z = -(w / 2) * cos(radians(ang));
        mesh->vVertices.emplace_back(vertex_X, vertex_Y, vertex_Z);

        //V3
        vertex_X = -(w / 2) * sin(radians(ang));
        vertex_Z = -(w / 2) * cos(radians(ang));
        mesh->vVertices.emplace_back(vertex_X, 0, vertex_Z);
   
        mesh->vTexCoords.reserve(mesh->mNumVertices);

        mesh->vTexCoords.emplace_back(0.0, 1);
        mesh->vTexCoords.emplace_back(0.0, 0.0);
        mesh->vTexCoords.emplace_back(1, 1);
        mesh->vTexCoords.emplace_back(1, 0.0);

        mesh->vTexCoords.emplace_back(0.0, 1);
        mesh->vTexCoords.emplace_back(0.0, 0.0);
        mesh->vTexCoords.emplace_back(1, 1);
        mesh->vTexCoords.emplace_back(1, 0.0);

        gMesh.push_back(mesh);

        ang += incr;
    }

    return gMesh;
}

Mesh* Mesh::generateSquaredRing() {
    Mesh* mesh = new Mesh();

    mesh->mPrimitive = GL_TRIANGLE_STRIP;
    mesh->mNumVertices = 10;
    mesh->vVertices.reserve(mesh->mNumVertices);

    mesh->vVertices.emplace_back(30.0, 30.0, 0.0);
    mesh->vVertices.emplace_back(10.0, 10.0, 0.0);
    mesh->vVertices.emplace_back(70.0, 30.0, 0.0);
    mesh->vVertices.emplace_back(90.0, 10.0, 0.0);
    mesh->vVertices.emplace_back(70.0, 70.0, 0.0);
    mesh->vVertices.emplace_back(90.0, 90.0, 0.0);
    mesh->vVertices.emplace_back(30.0, 70.0, 0.0);
    mesh->vVertices.emplace_back(10.0, 90.0, 0.0);
    mesh->vVertices.emplace_back(30.0, 30.0, 0.0);
    mesh->vVertices.emplace_back(10.0, 10.0, 0.0);


    mesh->vColors.emplace_back(0.0, 0.0, 0.0, 1.0);
    mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
    mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
    mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);
    mesh->vColors.emplace_back(1.0, 1.0, 0.0, 1.0);
    mesh->vColors.emplace_back(1.0, 0.0, 1.0, 1.0);
    mesh->vColors.emplace_back(0.0, 1.0, 1.0, 1.0);
    mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
    mesh->vColors.emplace_back(0.0, 0.0, 0.0, 1.0);
    mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);


    for (int i = 0; i < 10; i++)
        mesh->vIndices.emplace_back(i % 8);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    
    glVertexPointer(3, GL_FLOAT, 0, &mesh->vVertices);
    glColorPointer(4, GL_FLOAT, 0, &mesh->vColors);

    mesh->draw();
    
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    mesh->vNormals.reserve(mesh->size());

    for (int i = 0; i < mesh->size(); i++)
        mesh->vNormals.emplace_back(glm::dvec3(0, 0, 1));
       
    return mesh;
}

void IndexMesh::render() const{
    if(vVertices.size() > 0){
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_DOUBLE, 0, vVertices.data());

        if(vIndexes != nullptr){
            glEnableClientState(GL_INDEX_ARRAY);
            glIndexPointer(GL_UNSIGNED_INT, 0, vIndexes);
        }
        if (vColors.size() > 0) {
            glEnableClientState(GL_COLOR_ARRAY);
            glColorPointer(4, GL_DOUBLE, 0, vColors.data());
        }
        if (vTexCoords.size() > 0) {
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glTexCoordPointer(2, GL_DOUBLE, 0, vTexCoords.data());
        }
        if(vNormals.size() > 0){
            glEnableClientState(GL_NORMAL_ARRAY);
            glNormalPointer(GL_DOUBLE, 0, vNormals.data());
        }

        draw();

        glDisableClientState(GL_INDEX_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
    }
}

void IndexMesh::draw() const{
    glDrawElements(mPrimitive, nNumIndices, GL_UNSIGNED_INT, vIndexes);
}

void IndexMesh::buildNormalVectors() {
    int i1, i2, i3;
    glm::dvec3 a, b, c, normal;

    vNormals.reserve(size());

    // init vNormals
    for(int i = 0; i < size(); i++)
        vNormals.emplace_back(dvec3(0.0, 0.0, 0.0));

    //calculate normals
    for(int i = 0; i < nNumIndices; i += 3){
        // take triangle ABC from indexes
        i1 = vIndexes[i]; a = vVertices[i1];
        i2 = vIndexes[i+1]; b = vVertices[i2];
        i3 = vIndexes[i+2]; c = vVertices[i3];

        normal = glm::cross(b - a, c - a);
        vNormals[i1] += normal;
        vNormals[i2] += normal;
        vNormals[i3] += normal;
    }

    // normalize
    for(int i = 0; i < size(); i++)
        vNormals[i] = glm::normalize(vNormals[i]);
}

IndexMesh* IndexMesh::generateIndexCubeWithLids(GLdouble l){
    IndexMesh* mesh = new IndexMesh();

    mesh->mPrimitive = GL_TRIANGLES;
    mesh->mNumVertices = 8;
    mesh->vVertices.reserve(mesh->size());
    mesh->vColors.reserve(mesh->size());

    GLdouble half = l / 2;

    // colors
    for(int i = 0; i < mesh->size(); i++)
        mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);

    // vertexes
    mesh->vVertices.emplace_back(half, half, half);
    mesh->vVertices.emplace_back(half, -half, half);
    mesh->vVertices.emplace_back(half, half, -half);
    mesh->vVertices.emplace_back(half, -half, -half);
    mesh->vVertices.emplace_back(-half, half, -half);
    mesh->vVertices.emplace_back(-half, -half, -half);
    mesh->vVertices.emplace_back(-half, half, half);
    mesh->vVertices.emplace_back(-half, -half, half);


    mesh->nNumIndices = 36;
    mesh->vIndexes = new GLuint[mesh->nNumIndices]{
        0, 1, 2,
        2, 1, 3,
        2, 3, 4,
        4, 3, 5,
        4, 5, 6,
        6, 5, 7,
        6, 7, 0,
        0, 7, 1,
        4, 6, 2,
        2, 6, 0,
        1, 7, 3,
        3, 7, 5
    };

    // normals
    //mesh->vNormals.emplace_back(1, 0, 0);

    mesh->buildNormalVectors();

    return mesh;
}

MbR* MbR::generateIndexMeshByRevolution(int mm, int nn, glm::dvec3* perfil){
    MbR* mesh = new MbR(mm, nn, perfil);
    mesh->mPrimitive = GL_TRIANGLES;
    mesh->mNumVertices = mm*nn;
    mesh->nNumIndices = nn * (mm - 1) * 6;
    mesh->vVertices.reserve(mesh->size());
    mesh->vColors.reserve(mesh->size());
    mesh->vIndexes = new GLuint[mesh->nNumIndices];

    dvec3* vertices = new dvec3[mesh->size()];

    for(int i=0; i<nn; i++) {

        GLdouble theta = i * 360 / nn;
        GLdouble c = cos(radians(theta));
        GLdouble s = sin(radians(theta));

        for (int j = 0; j < mm; j++) {
            int indice = i * mm + j;
            GLdouble x = c * perfil[j].x + s * perfil[j].z;
            GLdouble z = -s * perfil[j].x + c * perfil[j].z;
            vertices[indice] = dvec3(x, perfil[j].y, z);
        }
    }
    for (int i = 0; i < mesh->size(); i++){
        mesh->vVertices.emplace_back(vertices[i]);
    }
    int indiceMayor = 0;
    for (int i=0; i<nn; i++){
        for(int j=0; j<mm-1; j++) {
            int indice = i*mm+j;
            mesh->vIndexes[indiceMayor] = indice;
            indiceMayor++;
            mesh->vIndexes[indiceMayor] = (indice + mm) % (nn * mm);
            indiceMayor++;
            mesh->vIndexes[indiceMayor] = (indice + mm + 1) % (nn * mm);
            indiceMayor++;
            mesh->vIndexes[indiceMayor] = (indice + mm + 1) % (nn * mm);
            indiceMayor++;
            mesh->vIndexes[indiceMayor] = indice + 1;
            indiceMayor++;
            mesh->vIndexes[indiceMayor] = indice;
            indiceMayor++;
        }
    }
    mesh->buildNormalVectors();
    return mesh;
}

IndexMesh* IndexMesh::generateGrid(GLdouble side, GLuint chunks) {
    IndexMesh* mesh = new IndexMesh();

    mesh->mPrimitive = GL_TRIANGLES;
    mesh->mNumVertices = (chunks + 1) * (chunks + 1);
    mesh->vVertices.reserve(mesh->size());

    // caras = chk * chk; triangulos = caras * 2; indice = triang * 3;
    mesh->nNumIndices = chunks * chunks * 6;
    mesh->vIndexes = new GLuint[mesh->nNumIndices];

    // vertex
    GLdouble chkSize = side / chunks;
    GLdouble incX = 0.0, incY = 0.0;

    for(int i = 0; i < chunks + 1; i++){
        for(int j = 0; j < chunks + 1; j++){
            mesh->vVertices.emplace_back(incX, incY, 0.0);
            incY += chkSize;
        }
        incX += chkSize;
        incY = 0.0;
    }

    // index
    int index = 0;
    for(int i = 0; i < chunks; i++){
        for(int j = 0; j < chunks; j++){
            // first triangle
            mesh->vIndexes[index++] = j + i * (chunks + 1);
            mesh->vIndexes[index++] = j + (i + 1) * (chunks + 1);
            mesh->vIndexes[index++] = (j + 1) + (i + 1)  * (chunks + 1);

            // second triangle
            mesh->vIndexes[index++] = (j + 1) + (i + 1) * (chunks + 1);
            mesh->vIndexes[index++] = (j + 1) + i * (chunks + 1);
            mesh->vIndexes[index++] = j + i * (chunks + 1);
        }
    }

    mesh->buildNormalVectors();
    return mesh;
}

IndexMesh* IndexMesh::generateGridTex(GLdouble side, GLuint chunks) {
    IndexMesh* mesh = generateGrid(side, chunks);
    mesh->vTexCoords.reserve(mesh->size());

    GLdouble chkSize = side / chunks;
    GLdouble incX = 0.0, incY = 0.0;

    for(int i = 0; i < chunks + 1; i++){
        for(int j = 0; j < chunks + 1; j++){
            mesh->vTexCoords.emplace_back(incX/side, incY/side);
            incY += chkSize;
        }
        incX += chkSize;
        incY = 0.0;
    }

    return mesh;
}
