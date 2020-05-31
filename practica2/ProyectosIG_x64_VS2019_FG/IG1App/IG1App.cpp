#include "IG1App.h"
#include "CheckML.h"
#include <iostream>

using namespace std;

//-------------------------------------------------------------------------
// static single instance (singleton pattern)

IG1App IG1App::s_ig1app;  // default constructor (constructor with no parameters)

//-------------------------------------------------------------------------

void IG1App::close()
{
	if (!mStop) {  // if main loop has not stopped
		cout << "Closing glut...\n";
		glutLeaveMainLoop();  // stops main loop and destroy the OpenGL context
		mStop = true;   // main loop stopped  
	}
	free();
}
//-------------------------------------------------------------------------

void IG1App::run()   // enters the main event processing loop
{
	if (mWinId == 0) { // if not initialized
		init();       // initialize the application 
		glutMainLoop();      // enters the main event processing loop 
		mStop = true;  // main loop has stopped  
	}
}
//-------------------------------------------------------------------------

void IG1App::init()
{
	// create an OpenGL Context
	iniWinOpenGL();

	// create the scene after creating the context 
	// allocate memory and resources
	mViewPort = new Viewport(mWinW, mWinH); //glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT)
	mCamera = new Camera(mViewPort);
	mScene = new Scene;

	mCamera->set2D();
	mScene->init();
}
//-------------------------------------------------------------------------

void IG1App::iniWinOpenGL()
{  // Initialization
	cout << "Starting glut...\n";
	int argc = 0;
	glutInit(&argc, nullptr);

	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);  // GLUT_CORE_PROFILE
	glutInitContextFlags(GLUT_DEBUG);		// GLUT_FORWARD_COMPATIBLE

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutInitWindowSize(mWinW, mWinH);   // window size
	//glutInitWindowPosition (140, 140);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH /*| GLUT_STENCIL*/); // RGBA colors, double buffer, depth buffer and stencil buffer   

	mWinId = glutCreateWindow("IG1App");  // with its associated OpenGL context, return window's identifier 

	// Callback registration
	glutReshapeFunc(s_resize);
	glutKeyboardFunc(s_key);
	//glutSpecialFunc(s_specialKey);
	glutDisplayFunc(s_display);
	glutIdleFunc(s_update);
	glutMouseFunc(s_mouse);
	glutMotionFunc(s_motion);
	glutMouseWheelFunc(s_mouseWheel);

	cout << glGetString(GL_VERSION) << '\n';
	cout << glGetString(GL_VENDOR) << '\n';
}
//-------------------------------------------------------------------------

void IG1App::free()
{  // release memory and resources
	delete mScene; mScene = nullptr;
	delete mCamera; mCamera = nullptr;
	delete mViewPort; mViewPort = nullptr;
}
//-------------------------------------------------------------------------

void IG1App::display2Vistas() const {
	Camera auxCam = *mCamera;
	Viewport auxVP = *mViewPort;
	mViewPort->setSize(mWinW / 2, mWinH);
	auxCam.setSize(mViewPort->width(), mViewPort->height());

	// right camera
	mViewPort->setPos(mWinW / 2, 0);
	auxCam.setCenital();
	auxCam.setSize(mWinW / 2, mWinH);
	mScene->render(auxCam);


	// left camera
	mViewPort->setPos(0, 0);
	mCamera->setSize(mWinW / 2, mWinH);
	mScene->render(*mCamera);



	*mViewPort = auxVP;
}


void IG1App::display() const {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // clears the back buffer

	// double buffering
	if (m2Vistas)
		display2Vistas();
	else {
		mCamera->setSize(mWinW, mWinH);
		mScene->render(*mCamera);  // uploads the viewport and camera to the GPU
	}

	glutSwapBuffers();	// swaps the front and back buffer
}
//-------------------------------------------------------------------------

void IG1App::resize(int newWidth, int newHeight)
{
	mWinW = newWidth; mWinH = newHeight;

	// Resize Viewport to the new window size
	mViewPort->setSize(newWidth, newHeight);

	// Resize Scene Visible Area such that the scale is not modified
	mCamera->setSize(mViewPort->width(), mViewPort->height());
}
//-------------------------------------------------------------------------

void IG1App::key(unsigned char key, int x, int y)
{
	bool need_redisplay = true;

	switch (key) {
	case 27:  // Escape key 
		glutLeaveMainLoop();  // stops main loop and destroy the OpenGL context
	case '+':
		mCamera->setScale(+0.01);  // zoom in  (increases the scale)
		break;
	case '-':
		mCamera->setScale(-0.01);  // zoom out (decreases the scale)
		break;
	case 'l':
		mCamera->set3D();
		break;
	case 'o':
		mCamera->set2D();
		break;
	case 'p':
		mCamera->changePrj();
		break;
	case 'u':
		mScene->update();
		break;
	case 'U':
		this->activate = !this->activate;
		break;
	case '1':
		mScene->setState(Scene::SCENE_1);
		break;
	case '2':
		mScene->setState(Scene::SCENE_2);
		break;
	case '3':
		mScene->setState(Scene::SCENE_3);
		break;
	case 'f':
		mScene->getgObjects()[1]->getTexture()->save("..\\Bmps\\picture.bmp");
		break;
	case 'k':
		m2Vistas = !m2Vistas;
		break;
	case 'q':
		mScene->light0_switch(true);
		break;
	case 'w':
		mScene->light0_switch(false);
		break;
	case 'a':
		mScene->light1_switch(true);
		break;
	case 's':
		mScene->light1_switch(false);
		break;
	case 'z':
		mScene->light2_switch(true);
		break;
	case 'x':
		mScene->light2_switch(false);
		break;
	case 't':
		mScene->light_airplane_switch(true);
		break;
	case 'g':
		mScene->light_airplane_switch(false);
		break;
	case 'e':
		mScene->light0_switch(false);
		mScene->light1_switch(false);
		mScene->light2_switch(false);
		mScene->light_airplane_switch(false);
		break;
	case 'y':
		mScene->move();
		break;
	default:
		need_redisplay = false;
		break;
	} //switch

	if (need_redisplay)
		glutPostRedisplay(); // marks the window as needing to be redisplayed -> calls to display()
}
//-------------------------------------------------------------------------

/*void IG1App::specialKey(int key, int x, int y)
{
	bool need_redisplay = true;
	int mdf = glutGetModifiers(); // returns the modifiers (Shift, Ctrl, Alt)

	switch (key) {
	case GLUT_KEY_RIGHT:
		if (mdf == GLUT_ACTIVE_CTRL)
			mCamera->pitch(-1);   // rotates -1 on the X axis
		else
			mCamera->pitch(1);    // rotates 1 on the X axis
		break;
	case GLUT_KEY_LEFT:
		if (mdf == GLUT_ACTIVE_CTRL)
			mCamera->yaw(1);      // rotates 1 on the Y axis
		else
			mCamera->yaw(-1);     // rotate -1 on the Y axis
		break;
	case GLUT_KEY_UP:
		mCamera->roll(1);    // rotates 1 on the Z axis
		break;
	case GLUT_KEY_DOWN:
		mCamera->roll(-1);   // rotates -1 on the Z axis
		break;
	default:
		need_redisplay = false;
		break;
	}//switch

	if (need_redisplay)
		glutPostRedisplay(); // marks the window as needing to be redisplayed -> calls to display()
}*/

void IG1App::update(void) {
	int last = glutGet(GLUT_ELAPSED_TIME);
	if ((last - this->mLastUpdateTime) > 16 && this->activate) {
		mScene->update();
		this->mLastUpdateTime = last;
		glutPostRedisplay();
	}
}

void IG1App::mouse(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		mMouseCoord = glm::dvec2(x, mWinH - y);
		mMouseButt = button;
	}
}

void IG1App::motion(int x, int y) {
	glm::dvec2 mp = mMouseCoord;
	mMouseCoord = glm::dvec2(x, mWinH - y);
	mp = mMouseCoord - mp;

	if (mMouseButt == GLUT_LEFT_BUTTON) {
		mCamera->orbit(-mp.x * 0.05, -mp.y);
	}
	else if (mMouseButt == GLUT_RIGHT_BUTTON) {
		mCamera->moveUD(-mp.y);
		mCamera->moveLR(-mp.x);
	}
	glutPostRedisplay();
}

void IG1App::mouseWheel(int n, int d, int x, int y) {
	int m = glutGetModifiers();
	if (m == 0) {
		if (d == 1) mCamera->moveFB(5);
		else mCamera->moveFB(-5);
	}
	else if (m == GLUT_ACTIVE_CTRL) {
		if (d == 1) mCamera->setScale(+0.01);
		else mCamera->setScale(-0.01);
	}
	glutPostRedisplay();
}
//-------------------------------------------------------------------------

