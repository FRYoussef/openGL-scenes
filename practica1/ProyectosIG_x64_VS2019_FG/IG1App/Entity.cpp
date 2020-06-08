#include "Entity.h"
#include <gtc/matrix_transform.hpp>  
#include <gtc/type_ptr.hpp>

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
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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
		
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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

Background::Background () {
	mMesh = Mesh::generateRectangleTexCoord(2, 2, 1, 1);
	mTexture = new Texture();
	mViewPort = new Viewport(1, 1);
	mCamera = new Camera(mViewPort);
	mCamera->set2D();
	mCamera->setSize(2.0, 2.0);
}

Background::~Background() {
	delete mMesh;  mMesh = nullptr; 
	delete mTexture; mTexture = nullptr;
	delete mViewPort; mViewPort = nullptr; 
	delete mCamera; mCamera = nullptr;
}

void Background::render() const {
	if (mMesh != nullptr) {
		glPolygonMode(GL_FRONT, GL_LINE);

		if (mTexture != nullptr)
			mTexture->bind(GL_REPLACE);

		mCamera->upload();
		mMesh->render();

		if (mTexture != nullptr)
			mTexture->unbind();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void Background::setSizeVP(int xw, int yh) {
	mViewPort->setSize(xw, yh);
}

void Background::setTexture(const std::string tx) {
	mTexture->load(tx);
}