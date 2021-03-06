#include "Entity.h"
#include <gtc/matrix_transform.hpp>  
#include <gtc/type_ptr.hpp>
#include <GL/gl.h>

using namespace glm;

//-------------------------------------------------------------------------

void Abs_Entity::upload(dmat4 const& modelViewMat) const 
{ 
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(value_ptr(modelViewMat));  // transfers modelView matrix to the GPU
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

RGBAxis::RGBAxis(GLdouble l): Abs_Entity()
{
  mMesh = Mesh::createRGBAxes(l);
}
//-------------------------------------------------------------------------

RGBAxis::~RGBAxis() 
{ 
	delete mMesh; mMesh = nullptr; 
};
//-------------------------------------------------------------------------

void RGBAxis::render(dmat4 const& modelViewMat) const 
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication

		glEnable(GL_COLOR_MATERIAL);
		upload(aMat);
		glLineWidth(2);
		
		mMesh->render();
		
		glLineWidth(1);
	}
}
//-------------------------------------------------------------------------

CPolygon::CPolygon(GLuint numL, GLdouble rd) {
	this->numL = numL;
	this->rd = rd;
	mMesh = Mesh::generatePolygon(numL, rd);
}

void CPolygon::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		glLineWidth(2);
		glColor3d(mColor.r, mColor.g, mColor.b);

		if (mTexture != nullptr)
			mTexture->bind(GL_REPLACE);
		mMesh->render();
		

		//Resetear attrs
		if (mTexture != nullptr)
			mTexture->unbind();
		glLineWidth(1);
		glColor3d(1, 1, 1);
	}
}

Sierpinski::Sierpinski(GLuint numP, GLdouble rd) {
	this->numP = numP;
	this->rd = rd;
	mMesh = Mesh::generateSierpinski(numP, rd);
}

void Sierpinski::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		glPointSize(2);
		glColor4dv(value_ptr(mColor));
		mMesh->render();

		//Resetear attrs
		glPointSize(1);
		glColor4d(1, 1, 1, 1);
	}
}

TriangleRGB::TriangleRGB(GLdouble rd): traslationAng(0.0), rotationAng(25.0) {
	this->rd = rd;
	mMesh = Mesh::generateTriangleRGB(rd);
}

void TriangleRGB::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		glPolygonMode(GL_BACK, GL_LINE);
		mMesh->render();

		//Resetear attrs
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void TriangleRGB::update() {
	// rotar sobre el eje 
	rotationAng += 2.5;
	traslationAng += 3.0;
	GLdouble p_X = 250.0 * cos(radians(traslationAng));
	GLdouble p_Y = 250.0 * sin(radians(traslationAng));

	mModelMat = glm::translate(dmat4(1), dvec3(p_X, p_Y, 0));
	mModelMat = glm::rotate(mModelMat, radians(rotationAng), dvec3(0, 0, 1));
}

RectangleRGB::RectangleRGB(GLdouble w, GLdouble h) {
	this->w = w;
	this->h = h;
	mMesh = Mesh::generateRGBRectangle(w, h);
}

void RectangleRGB::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		glPolygonMode(GL_BACK, GL_LINE);
		mMesh->render();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

_3DStar::_3DStar(GLdouble re, GLdouble np, GLdouble h) {
	this->h = h;
	this->np = np;
	this->re = re;
	this->angleY = 0;
	this->angleZ = 0;
	mMesh = Mesh::generateStarTexCoord(re, np, h);
}

void _3DStar::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		
		if (mTexture != nullptr)
			mTexture->bind(GL_REPLACE);

		mMesh->render();
		
		aMat=rotate(aMat, radians(180.0), dvec3(1, 0, 0));
		upload(aMat);
		mMesh->render();

		if (mTexture != nullptr)
			mTexture->unbind();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void _3DStar::update() {
	// rotar sobre el eje 
	angleY += 3.0;
	angleZ += 3.0;

	
	mModelMat = glm::translate(dmat4(1), dvec3(-150.0, 200.0, -150.0));
	mModelMat = glm::rotate(mModelMat, radians(angleY), dvec3(0, 1, 0));
	mModelMat = glm::rotate(mModelMat, radians(angleZ), dvec3(0, 0, 1));
}

Floor::Floor(GLdouble w, GLdouble h, GLuint rw, GLuint rh) {
	this->h = h;
	this->w = w;
	this->rw = rw;
	this->rh = rh;
	mMesh = Mesh::generateRectangleTexCoord(w, h, rw, rh);
}

void Floor::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		
		
		if (mTexture != nullptr)
			mTexture->bind(GL_MODULATE);

		glColor4dv(value_ptr(mColor));
		aMat = rotate(aMat, radians(90.0), dvec3(1, 0, 0));
		upload(aMat);
		mMesh->render();

		if (mTexture != nullptr)
			mTexture->unbind();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

Box::Box(GLdouble ld) {
	this->ld = ld;
	mMesh = Mesh::generateBoxTextCoord(ld);
}

void Box::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		if (mTexture != nullptr)
			mTexture->bind(GL_REPLACE);

		upload(aMat);
		mMesh->render();

		
		glCullFace(GL_FRONT);

		if (iTexture != nullptr)
			iTexture->bind(GL_REPLACE);

		upload(aMat);
		mMesh->render();

		if (mTexture != nullptr)
			mTexture->unbind();

		if (iTexture != nullptr)
			iTexture->unbind();

		glDisable(GL_CULL_FACE);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}


Picture::Picture(GLdouble ld1, GLdouble ld2) {
	this->ld1 = ld1;
	this->ld2 = ld2;
	mMesh = Mesh::generateRectangleTexCoord(ld1, ld2, 1, 1);
	mTexture = new Texture();
}

void Picture::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication

		mTexture->loadColorBuffer(800,600);

		if (mTexture != nullptr)
			mTexture->bind(GL_REPLACE);

		upload(aMat);
		mMesh->render();
		

		if (mTexture != nullptr)
			mTexture->unbind();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

Grass::Grass(GLuint times, GLdouble width, GLdouble height) {
	this->width = width;
	this->height = height;
	mTexture = new Texture();
	gMesh = Mesh::generate3dObject(times, width, height);
}

void Grass::render(dmat4 const& modelViewMat) const
{
	
	for (int i = 0; i < gMesh.size(); i++) {
		if (gMesh.at(i) != nullptr) {
			dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
			glAlphaFunc(GL_GREATER, 0.0);
			glEnable(GL_ALPHA_TEST);
			upload(aMat);

			if (mTexture != nullptr)
				mTexture->bind(GL_REPLACE);

			gMesh.at(i)->render();

			if (mTexture != nullptr)
				mTexture->unbind();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDisable(GL_ALPHA_TEST);

		}
	}
}

Glass::Glass(GLdouble ld) {
	this->ld = ld;
	mMesh = Mesh::generateBoxTextCoord(ld);
}

void Glass::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication

		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if (mTexture != nullptr)
			mTexture->bind(GL_REPLACE);

		upload(aMat);
		mMesh->render();

		if (mTexture != nullptr)
			mTexture->unbind();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
	}
}

Sphere::Sphere(GLdouble r) {
	this->r = r;
}

void Sphere::render(glm::dmat4 const& modelViewMat) const {
	dmat4 aMat = modelViewMat * mModelMat;
	upload(aMat);

	glEnable(GL_COLOR_MATERIAL);
	glColor3f(this->color.r, this->color.g, this->color.b);

	gluQuadricDrawStyle(q, GLU_FILL);
	gluSphere(q, r, 50, 50);

	glColor3f(1.0, 1.0, 1.0);
	glDisable(GL_COLOR_MATERIAL);
}

Cylinder::Cylinder(GLdouble baseR, GLdouble topR, GLdouble height) {
	this->baseR = baseR;
	this->topR = topR;
	this->height = height;
}

void Cylinder::render(glm::dmat4 const& modelViewMat) const {
	dmat4 aMat = modelViewMat * mModelMat;
	upload(aMat);

	glEnable(GL_COLOR_MATERIAL);
	glColor3f(this->color.r, this->color.g, this->color.b);

	gluQuadricDrawStyle(q, GLU_FILL);
	gluCylinder(q, baseR, topR, height, 50, 50);

	glColor3f(1.0, 1.0, 1.0);
	glDisable(GL_COLOR_MATERIAL);
}

Disk::Disk(GLdouble innerR, GLdouble outerR) {
	this->innerR = innerR;
	this->outerR = outerR;
}

void Disk::render(glm::dmat4 const& modelViewMat) const {
	dmat4 aMat = modelViewMat * mModelMat;
	upload(aMat);

	glEnable(GL_COLOR_MATERIAL);
	glColor3f(this->color.r, this->color.g, this->color.b);

	gluQuadricDrawStyle(q, GLU_FILL);
	gluDisk(q, innerR, outerR, 50, 50);

	glColor3f(1.0, 1.0, 1.0);
	glDisable(GL_COLOR_MATERIAL);
}

PartialDisk::PartialDisk(GLdouble innerR, GLdouble outerR, GLint startAngle, GLint sweepAngle) {
	this->innerR = innerR;
	this->outerR = outerR;
	this->startAngle = startAngle;
	this->sweepAngle = sweepAngle;
}

void PartialDisk::render(glm::dmat4 const& modelViewMat) const {
	dmat4 aMat = modelViewMat * mModelMat;
	upload(aMat);

	glEnable(GL_COLOR_MATERIAL);
	glColor3f(this->color.r, this->color.g, this->color.b);

	gluQuadricDrawStyle(q, GLU_FILL);
	gluPartialDisk(q, innerR, outerR, 50, 50, startAngle, sweepAngle);

	glColor3f(1.0, 1.0, 1.0);
	glDisable(GL_COLOR_MATERIAL);

}

SquaredRing::SquaredRing() {
	mMesh = Mesh::generateSquaredRing();
}

void SquaredRing::render(glm::dmat4 const& modelViewMat) const {
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		
		upload(aMat);
		mMesh->render();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void EntityWithIndexMesh::render(glm::dmat4 const& modelViewMat) const {
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication

		upload(aMat);
		mMesh->render();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

Cube::Cube(GLdouble l) {
	l = l;
	mMesh = IndexMesh::generateIndexCubeWithLids(l);
}

void Cube::render(glm::dmat4 const& modelViewMat) const {

	if (mMesh != nullptr) {

		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);

		if (this->copper) {
			glm::fvec4 amb = { 0.19125, 0.0735, 0.0225, 1.0 };
			glm::fvec4 diff = { 0.7038, 0.27048, 0.0828, 1.0 };
			glm::fvec4 spec = { 0.256777, 0.137622, 0.086014, 1.0 };

			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, value_ptr(amb));
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, value_ptr(diff));
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, value_ptr(spec));

			GLfloat s[] = { 12.8 };
			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, s);

			mMesh->render();
		}
		else {
			glEnable(GL_COLOR_MATERIAL);
			glColor3f(this->mColor.r, this->mColor.g, this->mColor.b);
			mMesh->render();
			glColor3f(1.0, 1.0, 1.0);
			glDisable(GL_COLOR_MATERIAL);
		}
	}
}

void Cube::update() {
	//this->copper = !this->copper;
}

CompoundEntity::~CompoundEntity(){
	for (Abs_Entity* ae : gObjects){
		delete ae;  
		ae = nullptr;
	}
	gObjects.resize(0);
}

void CompoundEntity::addEntity(Abs_Entity* ae) {
	gObjects.push_back(ae);
}

void CompoundEntity::render(glm::dmat4 const& modelViewMat) const {
	dmat4 aMat = modelViewMat * mModelMat;
	upload(aMat);

	for (Abs_Entity* ae : gObjects)
		ae->render(aMat);
}

void CompoundEntity::update() {
	for (int i = 0; i < gObjects.size(); i++)
		gObjects.at(i)->update();
}

void Abs_Entity::setMColor(glm::dvec4 const& mCol) {
	std::vector<glm::dvec4> vc;
		for(int i = 0; i < mMesh->size(); i++)
		vc.emplace_back(mCol);
	mMesh->setVColors(vc);
};

Cone::Cone(GLdouble h, GLdouble r, GLint n) {
	h = h; 
	r = r; 
	n = n;
	int m = 3;
	dvec3* perfil = new dvec3[m];
	perfil[0] = dvec3(0.5, 0.0, 0.0);
	perfil[1] = dvec3(r, 0.0, 0.0);
	perfil[2] = dvec3(0.5, h, 0.0);
	mMesh = MbR::generateIndexMeshByRevolution(m, n, perfil);
};

Esfera::Esfera(GLdouble r, GLint p, GLint m) {
	p = p; 
	m = m; 
	r = r;

	dvec3* perfil = new dvec3[p];

	GLdouble ang = -90;
	GLdouble incr = 180.0 / p;

	for (int i = 0; i < p; i++) {
		GLdouble x = r * cos(radians(ang));
		GLdouble y = r * sin(radians(ang));
		perfil[i] = dvec3(x, y, 0.0);
		ang += incr;
	}
	mMesh = MbR::generateIndexMeshByRevolution(p, m, perfil);

	material = new Material();
	material->setGold();
};

// Ejercicio 28
/*void Esfera::render(glm::dmat4 const& modelViewMat) const {
	if (mMesh != nullptr) {
	
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);

		if (this->gold) {
			
			glm::fvec4 amb = { 0.24725, 0.1995, 0.0745, 1.0 };
			glm::fvec4 diff = { 0.75164, 0.60648, 0.22648, 1.0 };
			glm::fvec4 spec = { 0.628281, 0.555802, 0.366065, 1.0 };

			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, value_ptr(amb));
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, value_ptr(diff));
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, value_ptr(spec));

			GLfloat s[] = { 51.2 };
			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, s);
			
			mMesh->render();
		}
		else {
			glEnable(GL_COLOR_MATERIAL);
			//glColorMaterial(GL_FRONT_AND_BACK, GL_SHININESS);
			glColor3f(this->mColor.r, this->mColor.g, this->mColor.b);
			mMesh->render();
			glColor3f(1.0, 1.0, 1.0);
			glDisable(GL_COLOR_MATERIAL);
		}
	}
}*/

void Esfera::render(glm::dmat4 const& modelViewMat) const { 
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;
		upload(aMat);

		if(this->gold){
			if(material != nullptr)
				material->upload();
			mMesh->render();
		}
		else{
			glEnable(GL_COLOR_MATERIAL);
			glColorMaterial(GL_FRONT_AND_BACK, GL_SHININESS);
			glColor3f(this->mColor.r, this->mColor.g, this->mColor.b);
			mMesh->render();
			glColor3f(1.0, 1.0, 1.0);
			glDisable(GL_COLOR_MATERIAL);
		}
	}
}

void Esfera::update() {
	//this->gold = !this->gold;
}

EntityWithLight::EntityWithLight() {
	light = new SpotLight();
	light->setAmbient(glm::fvec4(0, 0, 0, 1));
	light->setDiffuse(glm::fvec4(1, 1, 1, 1));
	light->setSpecular(glm::fvec4(0.5, 0.5, 0.5, 1));
	light->setSpot(glm::fvec3(0, -1, 0), 45.0, 0);
	light->disable();
}

void EntityWithLight::render(glm::dmat4 const& modelViewMat) const {
	dmat4 aMat = modelViewMat * mModelMat;
	
	light->upload(aMat);

	upload(aMat);

	for (Abs_Entity* ae : gObjects)
		ae->render(aMat);
 }

void EntityWithLight::switch_light(bool b){
	enabledLight = b;
	if(enabledLight)
		light->enable();
	else
		light->disable();
}

void Airplane::update(){
	Abs_Entity *propellers = ((CompoundEntity *) gObjects[0])->getGObjects()[1];
	propellersAngle += 3.0;
	propellers->setModelMat(glm::rotate(dmat4(1), radians(propellersAngle), dvec3(0, 0, 1)));

	angleX += 3.0;
	traslationAngle += 3.0;

	GLdouble z = (50 + planetR) * cos(radians(traslationAngle));
	GLdouble y = (50 + planetR) * sin(radians(traslationAngle));

	mModelMat = translate(dmat4(1), dvec3(0, y, -z));
	mModelMat = scale(mModelMat, dvec3(0.3, 0.3, 0.3));
	mModelMat = glm::rotate(mModelMat, radians(angleX), dvec3(1, 0, 0));
}

Grid::Grid(GLdouble _side, GLuint _chunks){
	side = _side;
	chunks = _chunks;
	mMesh = IndexMesh::generateGridTex(side, chunks);
}

void Grid::render(dmat4 const& modelViewMat) const {
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;
		upload(aMat);

		if (mTexture != nullptr){
			glEnable(GL_COLOR_MATERIAL);
			mTexture->bind(GL_MODULATE);
			//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glColor4dv(value_ptr(dvec4(1.0, 1.0, 1.0, 1.0)));
		}
		else{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glLineWidth(2.0);
			glColor3f(this->mColor.r, this->mColor.g, this->mColor.b);
		}

		mMesh->render();

		if (mTexture != nullptr){
			glDisable(GL_COLOR_MATERIAL);
			mTexture->unbind();
			//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		}
		else{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glLineWidth(1.0);
			glColor3f(1.0, 1.0, 1.0);
		}
	}
}

GridCube::GridCube(GLdouble _side, GLuint _chunks, Texture* vTx, Texture* hTx){
	side = _side;
	chunks = _chunks;

	//up face
	Grid *face = new Grid(side, chunks);
	face->setTexture(vTx);
	face->setModelMat(glm::translate(face->modelMat(), dvec3(-side/2, side/2, side/2)));
	face->setModelMat(glm::rotate(face->modelMat(), radians(-90.0), dvec3(1, 0, 0)));
	addEntity(face);

	// down face
	face = new Grid(side, chunks);
	face->setTexture(vTx);
	face->setModelMat(glm::translate(face->modelMat(), dvec3(-side/2, -side/2, side/2)));
	face->setModelMat(glm::rotate(face->modelMat(), radians(-90.0), dvec3(1, 0, 0)));
	addEntity(face);

	// left face
	face = new Grid(side, chunks);
	face->setTexture(hTx);
	face->setModelMat(glm::translate(face->modelMat(), dvec3(side/2, -side/2, side/2)));
	face->setModelMat(glm::rotate(face->modelMat(), radians(90.0), dvec3(0, 1, 0)));
	addEntity(face);

	// right face
	face = new Grid(side, chunks);
	face->setTexture(hTx);
	face->setModelMat(glm::translate(face->modelMat(), dvec3(-side/2, -side/2, side/2)));
	face->setModelMat(glm::rotate(face->modelMat(), radians(90.0), dvec3(0, 1, 0)));
	addEntity(face);

	// front face
	face = new Grid(side, chunks);
	face->setTexture(hTx);
	face->setModelMat(glm::translate(face->modelMat(), dvec3(-side/2, -side/2, side/2)));
	addEntity(face);

	// back face
	face = new Grid(side, chunks);
	face->setTexture(hTx);
	face->setModelMat(glm::translate(face->modelMat(), dvec3(-side/2, side/2, -side/2)));
	face->setModelMat(glm::rotate(face->modelMat(), radians(180.0), dvec3(1, 0, 0)));
	addEntity(face);
}

SirenCube::SirenCube(GLdouble _side, GLuint _chunks, GLdouble _rd, Texture* vTx, Texture* hTx) {
	side = _side;
	chunks = _chunks;
	rotationRadius = _rd;
	radius = side / 4;

	GridCube *cube = new GridCube(_side, _chunks, vTx, hTx);
	addEntity(cube);

	Esfera* sphere = new Esfera(radius, 250, 250);
	sphere->setMColor(dvec4(1.0, 0, 0, 1.0));
	sphere->setModelMat(glm::translate(sphere->modelMat(), dvec3(0, side/2, 0)));
	addEntity(sphere);

	light->setPosDir(glm::fvec3(0, -side/2, 0));
	light->setSpot(glm::fvec3(0, -1, 1), 45, 0);
}

void SirenCube::update() {
	angleX += 3.0;
	traslationAngle += 3.0;
	lightAng += 10.0;

	GLdouble xLight = sin(radians(lightAng));
	GLdouble zLight = cos(radians(lightAng));
	light->setSpot(glm::fvec3(xLight, -1, zLight), 45, 0);

	GLdouble z = (30 + rotationRadius) * cos(radians(traslationAngle));
	GLdouble y = (30 + rotationRadius) * sin(radians(traslationAngle));

	mModelMat = translate(dmat4(1), dvec3(0, y, -z));
	mModelMat = scale(mModelMat, dvec3(0.2, 0.2, 0.2));
	mModelMat = glm::rotate(mModelMat, radians(angleX), dvec3(1, 0, 0));
}
