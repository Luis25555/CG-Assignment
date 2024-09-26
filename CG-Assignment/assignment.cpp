
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLU32.lib")

/* Read me for camera control

Q rotate camera to left
E rotate camera to right
w rotate camera to top
s rotate camera to bottom

A move camera left
D move camera right
SPACE to reset everything

press 1 for front view
press 2 for back view
press 3 for top view
press 4 for left side view
press 5 for right side view

B raise arms
V lower arms

M raise ankle
N lower ankle

F1 raise weapon
F2 fire gun
F3 change texture
F4 switch on/off light
8,9,10 change light directions
Z walking
X stop walking
*/

/*
polygon counts

weapon 12 quads
sheild 6 quads


top part 
head 7
arm 18 x 2 = 36
jetpack 
2 polygons
8 quads
2 cylinder
body 
8 quads
4 polygons


middle part total 
24 quads

botom part 
18 QUADS mid

legs
74 QUADS  
4 Triangles
4 cylinders


bullet (1 sphere)

*/


//variables
float angle = 0;// angle for rotation of whole picture
float anglex = 0;
float angley = 0;
float tSpeed = 0.001;//transformation speed
float twSpeed = 1;
float walkSpeed = 0.002;
float rSpeed = 0.6;
float rxSpeed = 0.7;
float rySpeed = 0.6;
float armangle = -45;
float tx = 0, ty = 0, tz = 0;
bool isOrtho = true;
float ONear = -10.0;
float OFar = 10.0;
float PNear = 10.0;
float PFar = 100.0;
float ptx = 0, pty = 0, ptSpeed = 0.5; // projection translation matrix
float ptrx = 45, ptry = -45, prSpeed = 1;//prjection rotation angle
float twx = 0, twy = 0, twz = 0;
bool walk = false;
bool chg = false;
float anglehand = 0;
float bulletT = 0;
void bullet();
bool changetext = false;
bool lighton = false;
int lightNo = 1;

GLfloat diffuse[] = { 0.8, 0.8, 0.8, 1.0 };

GLfloat diffuseLight[] = { 1.0f, 0.0f, 0.0f, 1.0f };
GLfloat ambientLight[] = { 0.2f, 0.0f, 0.0f, 1.0f };

GLuint loadTexture(LPCSTR fileName);

BITMAP BMP;				//bitmap structure
HBITMAP hBMP = NULL;

/*
hg = height of the cube
wd = width of the cube
lg = length of the cube
*/

#define WINDOW_TITLE "OpenGL Window"

//function declaration
void background();
void drawSpehere(float rad, float r, float g, float b);
void drawCylinder(double br, double tr, double h, float r, float g, float b);
void drawCylinder2(double br, double tr, double h, float r, float g, float b);
void drawDisk(double inr, double otr, float r, float g, float b);
void linecube(float hg, float wd, float lg, float r, float g, float b, float size); //draw line cube
void cube(float hg, float wd, float lg, float r, float g, float b); //draw rectangular cube
void shoes(float lg, float wd, float hg, float r, float g, float b);
void body(float wd, float lg, float hg, float r, float g, float b);
void bodybtm(float wd, float lg, float hg, float r, float g, float b);
void head();
void neck();
void arm(float lr);
void weapon();
void sheild();
void jetPack(float wd, float lg, float hg);
void booster();
float raiseweapon = 0;
bool rWeapon = false;
bool fire = false;
void leg();
void rightleg();
void waist();
/*
hg = height of the cube
wd = width of the cube
lg = length of the cube
*/
void projection();

LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) PostQuitMessage(0);
		else if ((wParam == VK_LEFT)) { twx -= twSpeed; }
		else if ((wParam == VK_RIGHT)) { twx += twSpeed; }
		else if ((wParam == VK_SPACE)) { tx = 0;tz = 0;ty = 0; ptrx = 45; ptry = -45; ptx = 0;pty = 0; angle = 0; twx = 0; twz = 0; anglex = 0; armangle = 0;anglehand = 0; angley = 0;raiseweapon = 0; }
		else if ((wParam == '1')) { ptrx = 0; ptry = 0; ptx = 0;pty = 0; }
		else if ((wParam == '2')) { ptrx = 0; ptry = 180; ptx = 0;pty = 0; }
		else if ((wParam == '3')) { ptrx = 90; ptry = 0; ptx = 0;pty = 0; }
		else if ((wParam == '4')) { ptrx = 0; ptry = 90; ptx = 0;pty = 0; }
		else if ((wParam == '5')) { ptrx = 0; ptry = -90; ptx = 0;pty = 0; }
		else if ((wParam == 'W')) pty += ptSpeed;
		else if ((wParam == 'S')) pty -= ptSpeed;
		else if ((wParam == 'A')) ptx -= ptSpeed;
		else if ((wParam == 'D')) ptx += ptSpeed;
		else if ((wParam == 'Q')) ptry -= prSpeed;
		else if ((wParam == 'E')) ptry += prSpeed;
		else if ((wParam == VK_UP))  twz += (tSpeed*100);
		else if ((wParam == VK_DOWN))  twz -= (tSpeed * 100);
		else if ((wParam == 'K')) { angle += rSpeed;tx += tSpeed;ty += tSpeed; }
		else if ((wParam == 'J')) { angle -= rSpeed;tx -= tSpeed;ty -= tSpeed; }
		else if ((wParam == 'U')) { anglex += rSpeed; tz -= tSpeed; }
		else if ((wParam == 'I')) { anglex -= rSpeed; tz += tSpeed; }
		else if ((wParam == 'Z')) { walk = true; }
		else if ((wParam == 'X')) { walk = false;  angle = 0; }
		else if ((wParam == 'O')) { isOrtho = true; }
		else if ((wParam == 'P')) { isOrtho = false; ptrx = 0; ptry = 0; ptx = 0;pty = 0;}
		else if ((wParam == 'N')) { armangle += rSpeed; }
		else if ((wParam == 'M')) { armangle -= rSpeed; }
		else if ((wParam == 'V')) { anglehand += rSpeed; }
		else if ((wParam == 'B')) { anglehand -= rSpeed; }
		else if ((wParam == VK_F1)) { rWeapon = true; }
		else if ((wParam == VK_F2)) { fire = true; }
		else if ((wParam == VK_F3)) { changetext = !changetext; }
		else if ((wParam == VK_F4)) { lighton = !lighton; }
		else if ((wParam == '8')) { lightNo = 1; }
		else if ((wParam == '9')) { lightNo = 2; }
		else if ((wParam == '0')) { lightNo = 3; }
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
//--------------------------------------------------------------------

bool initPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.cAlphaBits = 8;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 0;

	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;

	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;

	// choose pixel format returns the number most similar pixel format available
	int n = ChoosePixelFormat(hdc, &pfd);

	// set pixel format returns whether it sucessfully set the pixel format
	if (SetPixelFormat(hdc, n, &pfd))
	{
		return true;
	}
	else
	{
		return false;
	}
}
//--------------------------------------------------------------------

void display()
{
	//--------------------------------
	//	OpenGL drawing
	//--------------------------------

	glClearColor(0.5294, 0.8078, 0.9216, 1);

	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	//glDisable(GL_DEPTH_TEST); // Background should be drawn without depth test

	//glBegin(GL_QUADS);
	//glColor3f(0.2f, 0.5f, 0.7f); // Top color (light blue)
	//glVertex3f(-20, 20, -5);
	//glVertex3f(20, 20, -5);

	//glColor3f(0.0f, 0.0f, 0.2f); // Bottom color (dark blue)
	//glVertex3f(20, -20, -5);
	//glVertex3f(-20, -20, -5);
	//glEnd();

	if (lighton == true) {

		glEnable(GL_LIGHTING);         // Enable lighting
		glEnable(GL_LIGHT0);

	}
	else {
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
	}
	
	switch (lightNo) {

	case 1: {
		GLfloat  keyLightPosition[] = { 2.0f, 3.0f, 5.0f, 1.0f };
		GLfloat keyLightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glLightfv(GL_LIGHT0, GL_POSITION, keyLightPosition);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, keyLightDiffuse);
	}break;
	case 2: {
		GLfloat fillLightPosition[] = { -2.0f, 1.0f, 4.0f, 1.0f };
		GLfloat fillLightDiffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
		glLightfv(GL_LIGHT0, GL_POSITION, fillLightPosition);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, fillLightDiffuse);
	}break;
	case 3: {
		GLfloat backLightPosition[] = { 0.0f, 4.0f, -3.0f, 1.0f };
		GLfloat backLightDiffuse[] = { 0.7f, 0.7f, 0.7f, 1.0f };
		glLightfv(GL_LIGHT0, GL_POSITION, backLightPosition);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, backLightDiffuse);
	}break;
	}
	
	glEnable(GL_DEPTH_TEST); //enable the depth test
	glMatrixMode(GL_MODELVIEW_MATRIX);
	glEnable(GL_TEXTURE_2D);
	glLoadIdentity();
	projection();
	
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glPushMatrix();
	glTranslatef(-10, -29, 0);
	cube(20, 25, 25,0, 0.5019, 0);
	glPopMatrix();

	glPushMatrix(); //all
	

	glTranslatef(twx, twy, twz);
	glPushMatrix();
	glTranslatef(-3, 4, -0.5);
	arm(-1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(3, 4, -0.5);
	if (rWeapon && raiseweapon > -45.0f) {
		raiseweapon -= 1.0f;  // Decrement the raiseWeapon angle (smoothly rotate towards -45)
		if (raiseweapon < -45.0f) {
			raiseweapon = -45.0f;  // Clamp the rotation to -45 degrees to stop
		}
	}
	glRotatef(raiseweapon, 1, 0, 0);
	
	arm(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 7, 0);
	head();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2, 2, -2);
	glRotatef(-90, 0, 1, 0);
	glTranslatef(1, 0, -5);
	glPushMatrix();
	glTranslatef(0, 1, 0);
	body(4, 4, 4, 1, 1, 1);
	glTranslatef(0, 2,0 );
	jetPack(0.3, 4, 2);

	glTranslatef(-2, 2, -2);
	glRotatef(-90, 0, 1, 0);
	glTranslatef(3.25, -1.4, -1.7);
	booster();
	glTranslatef(1.5, 0, 0);
	booster();
	glPopMatrix();
	bodybtm(4, 4, 1, 1, 1, 0);
	glPopMatrix();
	 
	waist();
	leg();
	rightleg();
	
	




	//--------------------------------
	//	End of OpenGL drawing
	//--------------------------------
}
//--------------------------------------------------------------------

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = WindowProcedure;
	wc.lpszClassName = WINDOW_TITLE;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wc)) return false;

	HWND hWnd = CreateWindow(WINDOW_TITLE, WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
		700, 10, 900, 900,
		NULL, NULL, wc.hInstance, NULL);

	//--------------------------------
	//	Initialize window for OpenGL
	//--------------------------------

	HDC hdc = GetDC(hWnd);

	//	initialize pixel format for the window
	initPixelFormat(hdc);

	//	get an openGL context
	HGLRC hglrc = wglCreateContext(hdc);

	//	make context current
	if (!wglMakeCurrent(hdc, hglrc)) return false;

	//--------------------------------
	//	End initialization
	//--------------------------------

	ShowWindow(hWnd, nCmdShow);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		display();

		SwapBuffers(hdc);
	}

	UnregisterClass(WINDOW_TITLE, wc.hInstance);

	return true;
}
//--------------------------------------------------------------------

void drawCylinder(double br, double tr, double h, float r, float g, float b) {
	glColor3f(r, g, b);
	GLUquadricObj* cylinder = NULL;
	cylinder = gluNewQuadric();
	glLineWidth(2);
	gluQuadricDrawStyle(cylinder, GLU_LINE);
	gluCylinder(cylinder, br, tr, h, 30, 20);
	gluDeleteQuadric(cylinder);

}

void drawCylinder2(double br, double tr, double h, float r, float g, float b) {
	glColor3f(r, g, b);
	GLUquadricObj* cylinder = NULL;
	cylinder = gluNewQuadric();
	gluQuadricDrawStyle(cylinder, GLU_FILL);
	gluCylinder(cylinder, br, tr, h, 30, 30);
	gluDeleteQuadric(cylinder);

}

void drawSpehere(float rad, float r, float g, float b) {
	glColor3f(r, g, b);
	GLUquadricObj* sphere = NULL;
	sphere = gluNewQuadric();
	gluQuadricDrawStyle(sphere, GLU_LINE);
	gluSphere(sphere, rad, 30, 30);
	gluDeleteQuadric(sphere);
}

void drawDisk(double inr, double otr, float r, float g, float b) {
	glColor3f(r, g, b);
	GLUquadricObj* disk = NULL;
	disk = gluNewQuadric();
	gluQuadricDrawStyle(disk, GLU_LINE);
	gluDisk(disk, inr, otr, 30, 30);
	gluDeleteQuadric(disk);
}

void cube(float hg, float wd, float lg  , float r, float g, float b) {
	
	glColor3f(r, g, b);
	
	glBegin(GL_POLYGON);//front
	glTexCoord2f(0, 1);
	glVertex3f(0, hg, 0);	// Top-left

	glTexCoord2f(1, 1);
	glVertex3f(wd,hg , 0);  // Top-right

	glTexCoord2f(1, 0);
	glVertex3f(wd, 0, 0);   // Bottom-right

	glTexCoord2f(0, 0);
	glVertex3f(0, 0, 0);	// Bottom-left
	glEnd();
	
	glBegin(GL_POLYGON);//btm

	glTexCoord2f(0, 1);
	glVertex3f(0, 0, 0);   // front-left

	glTexCoord2f(1, 1);
	glVertex3f(0, 0, lg);      // back-left

	glTexCoord2f(1, 0);
	glVertex3f(wd,0,lg);   // Back-right

	glTexCoord2f(0, 0);
	glVertex3f(wd, 0, 0);      // front -right
	glEnd();


	glBegin(GL_POLYGON);//right
	
	glTexCoord2f(0, 1);
	glVertex3f(wd, 0, 0);   // front btm

	glTexCoord2f(1, 1);
	glVertex3f(wd,hg , 0);      //front top

	glTexCoord2f(1, 0);
	glVertex3f(wd, hg, lg);  //back-top

	glTexCoord2f(0, 0);
	glVertex3f(wd, 0, lg);      //back-btm
	glEnd();

	glBegin(GL_POLYGON);//back

	glTexCoord2f(0, 1);
	glVertex3f(wd, 0, lg);  // btm - right

	glTexCoord2f(1, 1);
	glVertex3f(wd, hg, lg); //top-right

	glTexCoord2f(1, 0);
	glVertex3f(0, hg, lg);	//top-left

	glTexCoord2f(0, 0);
	glVertex3f(0, 0, lg);   //btm-left
	glEnd();

	glBegin(GL_POLYGON);//left
	
	glTexCoord2f(0, 1);
	glVertex3f(0, 0, lg);   // back - btm

	glTexCoord2f(1, 1);
	glVertex3f(0,0,0 );      //front - btm

	glTexCoord2f(1, 0);
	glVertex3f(0, hg, 0);  //front - top

	glTexCoord2f(0, 0);
	glVertex3f(0, hg,lg );      //top-right
	glEnd();


	glBegin(GL_POLYGON); //top

	glTexCoord2f(0, 1);
	glVertex3f(0, hg, lg);   // back - btm

	glTexCoord2f(1, 1);
	glVertex3f(wd, hg, lg);      //front - btm

	glTexCoord2f(1, 0);
	glVertex3f(wd, hg, 0);  //front - top

	glTexCoord2f(0, 0);
	glVertex3f(0, hg, 0);      //top-right
	glEnd();


	


}

void linecube(float hg, float wd, float lg, float r, float g, float b , float size) {

	glColor3f(r, g, b);
	glLineWidth(size);
	glBegin(GL_LINE_LOOP);//front
	glVertex3f(0, hg, 0);	// Top-left
	glVertex3f(wd, hg, 0);  // Top-right
	glVertex3f(wd, 0, 0);   // Bottom-right
	glVertex3f(0, 0, 0);	// Bottom-left
	glEnd();

	glBegin(GL_LINE_LOOP);//btm
	glVertex3f(0, 0, 0);   // front-left
	glVertex3f(0, 0, lg);      // back-left
	glVertex3f(wd, 0, lg);   // Back-right
	glVertex3f(wd, 0, 0);      // front -right
	glEnd();


	glBegin(GL_LINE_LOOP);//right

	glVertex3f(wd, 0, 0);   // front btm
	glVertex3f(wd, hg, 0);      //front top
	glVertex3f(wd, hg, lg);  //back-top
	glVertex3f(wd, 0, lg);      //back-btm
	glEnd();

	glBegin(GL_LINE_LOOP);//back

	glVertex3f(wd, 0, lg);  // btm - right
	glVertex3f(wd, hg, lg); //top-right
	glVertex3f(0, hg, lg);	//top-left
	glVertex3f(0, 0, lg);   //btm-left
	glEnd();

	glBegin(GL_LINE_LOOP);//left

	glVertex3f(0, 0, lg);   // back - btm
	glVertex3f(0, 0, 0);      //front - btm
	glVertex3f(0, hg, 0);  //front - top
	glVertex3f(0, hg, lg);      //top-right
	glEnd();


	glBegin(GL_LINE_LOOP); //top

	glVertex3f(0, hg, lg);   // back - btm
	glVertex3f(wd, hg, lg);      //front - btm
	glVertex3f(wd, hg, 0);  //front - top
	glVertex3f(0, hg, 0);      //top-right
	glEnd();





}


void projection() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (isOrtho) {
		glOrtho(-10.0, 10.0, -10.0, 10.0, ONear, OFar);
	}
	else {
		gluPerspective(60.0, 1.0, 0.1, 100.0);
		glFrustum(-20.0, 20.0, -20.0, 20.0, PNear, PFar);
	}

	glTranslatef(ptx, pty, 0);
	//glRotatef(ptrx, 1, 0, 0);
	glRotatef(ptry, 0, 1, 0);

}

void shoes(float lg,float wd,float hg, float r,float g,float b) {
	glColor3f(0, 0, 0);

	//front
	glBegin(GL_LINE_LOOP);
	glVertex3f(0, 0, 0); //btm left
	glVertex3f(wd, 0, 0); // btm right
	glVertex3f(wd, hg, 0);// top right
	glEnd();

	// right plane
	glBegin(GL_LINE_LOOP);
	glVertex3f(wd, hg, 0); //top right
	glVertex3f(wd, 0, 0); // btm right
	glVertex3f(wd, 0, lg);// back btm right
	glVertex3f(wd, hg, lg);//back top right
	glEnd();

	//back
	glBegin(GL_LINE_LOOP);
	glVertex3f(wd, hg, lg); //top right
	glVertex3f(wd, 0, lg); // btm right
	glVertex3f(0, 0, lg);// btm left
	glEnd();

	// left plane
	glBegin(GL_LINE_LOOP);
	glVertex3f(0, 0, lg); //btm left
	glVertex3f(wd, hg, lg); // btm right
	glVertex3f(wd, hg, 0);// back btm right
	glVertex3f(0, 0, 0);//back top right
	glEnd();
	// btm plane
	glBegin(GL_LINE_LOOP);
	glVertex3f(0, 0, 0); //btm left
	glVertex3f(0, 0, lg); // btm right
	glVertex3f(wd, 0, lg);// back btm right
	glVertex3f(wd, 0, 0);//back top right

	glEnd();

	glColor3f(r, g, b);

	//front
	glBegin(GL_TRIANGLES);
	glVertex3f(0, 0, 0); //btm left
	glVertex3f(wd, 0, 0); // btm right
	glVertex3f(wd, hg, 0);// top right
	glEnd();

	// right plane
	glBegin(GL_QUADS);
	glVertex3f(wd, hg, 0); //top right
	glVertex3f(wd, 0, 0); // btm right
	glVertex3f(wd, 0, lg);// back btm right
	glVertex3f(wd, hg, lg);//back top right
	glEnd();

	
	// left plane
	glBegin(GL_QUADS);
	glVertex3f(0, 0, lg); //btm left
	glVertex3f(wd, hg, lg); // btm right
	glVertex3f(wd, hg, 0);// back btm right
	glVertex3f(0, 0, 0);//back top right
	glEnd();
	// btm plane
	glBegin(GL_QUADS);
	glVertex3f(0, 0, 0); //btm left
	glVertex3f(0, 0, lg); // btm right
	glVertex3f(wd, 0, lg);// back btm right
	glVertex3f(wd, 0, 0);//back top right

	glEnd();

	//back
	glBegin(GL_TRIANGLES);
	glVertex3f(wd, hg, lg); //top right
	glVertex3f(wd, 0, lg); // btm right
	glVertex3f(0, 0, lg);// btm left
	glEnd();

}
void bodybtm(float wd,float lg,float hg,float r, float g, float b) {
	//glColor3f(r, g, b);
	//top
	GLuint textureArr[2];

	if (changetext == false) {
		textureArr[1] = loadTexture("metal2.bmp");
	}
	else { textureArr[1] = loadTexture("redmetal.bmp"); }


	glBegin(GL_QUADS);
	glVertex3f(0, hg, 0);
	glVertex3f(wd, hg, 0);
	glVertex3f(wd, hg, lg);
	glVertex3f(0, hg, lg);
	glEnd();

	
	glBegin(GL_QUADS);
	glVertex3f(0, hg, lg);
	glVertex3f((wd*0.25), 0, lg);
	glVertex3f((wd * 0.25), 0, 0);
	glVertex3f(0, hg, 0);
	glEnd();

	
	glBegin(GL_POLYGON);
	glVertex3f(0, hg, 0);
	glVertex3f(wd*0.25, 0, 0);
	glVertex3f((wd * 0.75), 0, 0);
	glVertex3f(wd, hg, 0);
	glEnd();


	glBegin(GL_QUADS);
	glVertex3f(wd, hg, 0);
	glVertex3f((wd * 0.75), 0, 0);
	glVertex3f((wd * 0.75), 0, lg);
	glVertex3f(wd, hg, lg);
	glEnd();

	
	glBegin(GL_POLYGON);
	glVertex3f(wd, hg, lg);
	glVertex3f(0 , hg, lg);
	glVertex3f((wd * 0.25), 0, lg);
	glVertex3f((wd * 0.75), 0, lg);
	glEnd();

	
	glBegin(GL_QUADS);
	glVertex3f((wd*0.25), 0, 0);
	glVertex3f((wd*0.75), 0, 0);
	glVertex3f((wd * 0.75), 0, lg);
	glVertex3f((wd * 0.25), 0, lg);
	glEnd();
	glDeleteTextures(1, &textureArr[1]);
}

void body(float wd, float lg, float hg, float r, float g, float b) {
	
	GLuint textureArr[2];
	
	if (changetext == false) {
		textureArr[1] = loadTexture("metal2.bmp");
	}
	else { textureArr[1] = loadTexture("redmetal.bmp"); }
	//glColor3f(r, g, b);
	//top
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);
	glVertex3f(0, 0, 0);

	glTexCoord2f(1, 1);
	glVertex3f(wd, 0, 0);

	glTexCoord2f(1, 0);
	glVertex3f(wd, 0, lg);

	glTexCoord2f(0, 0);
	glVertex3f(0, 0, lg);


	glEnd();
	
	
	glBegin(GL_QUADS);

	glTexCoord2f(0, 1);
	glVertex3f(0, 0, lg);

	glTexCoord2f(1, 1);
	glVertex3f(0, 0, 0);

	glTexCoord2f(1, 0);
	glVertex3f(0, hg, 0);

	glTexCoord2f(0, 0);
	glVertex3f(0, hg, lg);
	glEnd();

	
	glBegin(GL_POLYGON);

	glTexCoord2f(0, 1);
	glVertex3f(0, hg, lg);

	glTexCoord2f(1, 1);
	glVertex3f(0, 0, lg);

	glTexCoord2f(1, 0);
	glVertex3f(wd, 0, lg);

	glTexCoord2f(0, 0);
	glVertex3f((wd*0.85), hg, lg);
	glEnd();
	
	
	
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);
	glVertex3f((wd * 0.85), hg, lg);
	glTexCoord2f(1, 1);
	glVertex3f(wd, 0, lg);
	glTexCoord2f(1, 0);
	glVertex3f(wd, 0, 0);
	glTexCoord2f(0, 0);
	glVertex3f((wd * 0.85), hg, 0);
	glEnd();
	
	
	glColor3f(r, g, b);
	
	glBegin(GL_POLYGON);

	glTexCoord2f(0, 1);
	glVertex3f((wd * 0.85), hg, 0);

	glTexCoord2f(1, 1);
	glVertex3f(wd, 0, 0);

	glTexCoord2f(1, 0);
	glVertex3f(0, 0, 0);

	glTexCoord2f(0, 0);
	glVertex3f(0, hg, 0);
	glEnd();


	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);
	glVertex3f(0,hg,0);
	glTexCoord2f(1, 1);
	glVertex3f(0, hg, lg);
	glTexCoord2f(1, 0);
	glVertex3f((wd*0.85), hg, lg);
	glTexCoord2f(0, 0);
	glVertex3f((wd * 0.85), hg, 0);
	glEnd();


	glDeleteTextures(1, &textureArr[1]);
}

void neck() {

	cube(0.5,1.5,1.5,1,1,1);

}

void head() {

	cube(2,1,1,0,1,1);
	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	glTranslatef(-0.5, 1, -0.5);
	drawCylinder2(0.45, 0.45, 2, 1, 1, 1);
	glPopMatrix();

}

void arm(float lr) {
	GLuint textureArr[2];

	

	glRotatef((anglehand ), 1, 0, 0);
	glRotatef((angle*lr), 1, 0, 0);
	if (walk == true) {
		twz += walkSpeed;
		if (chg == false) { angle += rSpeed; anglex -= rxSpeed; angley += rySpeed; }
		else { angle -= rSpeed; anglex += rxSpeed;angley -= rySpeed; }

		if (angley >= 30) { chg = true; }
		if (angley <= -30) { chg = false; }

	}
	cube(2, 2, 2, 0.0588, 0.3216, 0.7294);
	if (changetext == false) {
		textureArr[1] = loadTexture("metal2.bmp");
	}else{ textureArr[1] = loadTexture("redmetal.bmp"); }
	
	glTranslatef(0.5, -3, 0.5);
	cube(3, 1, 1, 1, 1, 1); // Cube 1
	glTranslatef(0, -2.5, 0);
	glPushMatrix();
	glTranslatef(0.5, 3, 0.5); 
	glRotatef(armangle, 1, 0, 0);    
	glTranslatef(-0.5,-3, -0.5); 
	cube(3, 1, 1, 1, 1, 1); 
	glDeleteTextures(1, &textureArr[1]);
	if (lr == 1) { weapon(); if(fire == true)bullet(); }
	else { sheild(); }
	glPopMatrix();



	
}

void weapon() {

	GLuint textureArr[2];

	textureArr[1] = loadTexture("weapon.bmp");

	glTranslatef(-0.1, -1.2, -0.1);
	
	glTranslatef(0, 0.5, 0);
	cube(1.2, 1.2, 1.2, 0.443, 0.475, 0.494);
	
	glTranslatef(0.35, -5, 0.5);
	cube(5, 0.5, 0.5, 0.443, 0.475, 0.494);

	glDeleteTextures(1, &textureArr[1]);
}

void sheild() {
	GLuint textureArr[2];
	if (changetext == true) {
		textureArr[1] = loadTexture("redhex.bmp");
	}
	else
	{
		textureArr[1] = loadTexture("sheild.bmp");
	}
	glRotatef(90, 1, 0, 0);
	glTranslatef(-2, -4, 0);

	cube(6, 4, 0.5, 1, 1, 1);
	glDeleteTextures(1, &textureArr[1]);
}

void jetPack(float wd, float lg, float hg) {
	
	/*glTranslatef(0, 0, -0.1);*/
	glColor3f(0.443, 0.475, 0.494);

	glBegin(GL_POLYGON);
	glVertex3f(0, 0, (lg * 0.2));
	glVertex3f(0, 0, (lg * 0.8));
	glVertex3f(0, (hg * 0.2), lg * 0.9);
	glVertex3f(0, (hg * 0.4), lg * 0.9);
	glVertex3f(0, (hg * 0.6), lg * 0.8);
	glVertex3f(0, (hg * 0.6), (lg * 0.2));
	glVertex3f(0, (hg * 0.4), (lg * 0.1));
	glVertex3f(0, (hg * 0.2), (lg * 0.1));
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(0, (hg * 0.2), (lg * 0.1));
	glVertex3f(-wd, (hg * 0.2), (lg * 0.1));
	glVertex3f(-wd, (hg * 0.4), (lg * 0.1));
	glVertex3f(0, (hg * 0.4), lg * 0.1);

	glEnd();


	glBegin(GL_QUADS);
	glVertex3f(0, (hg * 0.4), lg * 0.1);
	glVertex3f(-wd, (hg * 0.4), (lg * 0.1));
	glVertex3f(-wd, (hg * 0.6), (lg * 0.2));
	glVertex3f(0, (hg * 0.6), lg * 0.2);

	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(0, (hg * 0.6), lg * 0.2);
	glVertex3f(-wd, (hg * 0.6), (lg * 0.2));
	glVertex3f(-wd, (hg * 0.6), (lg * 0.8));
	glVertex3f(0, (hg * 0.6), lg * 0.8);

	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(0, (hg * 0.6), lg * 0.8);
	glVertex3f(-wd, (hg * 0.6), (lg * 0.8));
	glVertex3f(-wd, (hg * 0.4), (lg * 0.9));
	glVertex3f(0, (hg * 0.4), lg * 0.9);

	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(0, (hg * 0.4), lg * 0.9);
	glVertex3f(-wd, (hg * 0.4), (lg * 0.9));
	glVertex3f(-wd, (hg * 0.2), (lg * 0.9));
	glVertex3f(0, (hg * 0.2), lg * 0.9);

	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(0, (hg * 0.2), lg * 0.9);
	glVertex3f(-wd, (hg * 0.2), lg * 0.9);
	glVertex3f(-wd, 0, (lg * 0.8));
	glVertex3f(0, (hg * 0), lg * 0.8);

	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(-wd, 0, (lg * 0.8));
	glVertex3f(0, 0, lg * 0.8);
	glVertex3f(0, 0, lg * 0.2);
	glVertex3f(-wd,0, (lg * 0.2));


	glEnd();


	glBegin(GL_QUADS);
	glVertex3f(0, 0, lg * 0.2);
	glVertex3f(-wd, 0, lg * 0.2);
	glVertex3f(-wd, (hg * 0.2), (lg * 0.1));
	glVertex3f(0, (hg * 0.2), lg * 0.1);

	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(-wd, 0, (lg * 0.2));
	glVertex3f(-wd, 0, (lg * 0.8));
	glVertex3f(-wd, (hg * 0.2), lg * 0.9);
	glVertex3f(-wd, (hg * 0.4), lg * 0.9);
	glVertex3f(-wd, (hg * 0.6), lg * 0.8);
	glVertex3f(-wd, (hg * 0.6), (lg * 0.2));
	glVertex3f(-wd, (hg * 0.4), (lg * 0.1));
	glVertex3f(-wd, (hg * 0.2), (lg * 0.1));
	glEnd();




	
}

void booster() {
	
	drawCylinder2(0.5, 0.5, 0.3, 0.698, 0.745, 0.71);
	
}

void bullet() {

	glTranslatef(0.2, 4, 0.25);
	if (fire == true) {
		glTranslatef(0, -bulletT, 0);
		bulletT += tSpeed*100;
	}
	drawSpehere(0.2, 1, 0, 0);
	if (bulletT >= 20) {
		bulletT = 0;
		fire = false;
	}
}

void leg() {
	GLuint textureArr[2];
	if (changetext == false) {
		textureArr[1] = loadTexture("metal2.bmp");
	}
	else { textureArr[1] = loadTexture("redmetal.bmp"); }

	glPushMatrix();//whole leg
	glTranslatef(0, -1.5, 0.5);
	glRotatef(angle, 1, 0, 0);
	if (walk == true) {
		twz += walkSpeed;
		if (chg == false) { angle += rSpeed; anglex -= rxSpeed; angley += rySpeed; }
		else { angle -= rSpeed; anglex += rxSpeed;angley -= rySpeed; }

		if (angley >= 30) { chg = true; }
		if (angley <= -30) { chg = false; }


	}


	glTranslatef(0, 1.5, -0.5);
	//glTranslatef(tx, 0, 0);
	//thigh
	glPushMatrix();//thigh
	glTranslatef(-0.32, -1.5, 0.5);
	linecube(1.5, 0.75, 1, 0, 0, 0, 1);
	cube(1.5, 0.75, 1, 1, 1, 1);
	glPopMatrix();//thigh

	//connector
	glPushMatrix();//conn
	glTranslatef(-0.55, -2, 0.41);

	linecube(0.5, 1.2, 1.2, 0, 0, 0, 1);
	cube(0.5, 1.2, 1.2, 1, 1, 1);
	glPopMatrix();//conn

	glPushMatrix();//bigleg
	glTranslatef(-0.32, -5, 0.5);
	linecube(3, 0.75, 1, 0, 0, 0, 1);
	cube(3, 0.75, 1, 1, 1, 1);
	glPopMatrix();//bigleg



	//knee cylinder inner	
	glPushMatrix();//knee cyl inner
	glTranslatef(0.2, -5.5, 1);
	glRotatef(90, 0, 1, 0);
	drawCylinder(0.5, 0.5, 0.5, 1, 1, 1);

	drawDisk(0, 0.5, 1, 1, 0);

	glTranslatef(0, 0, 0.5);
	drawDisk(0, 0.5, 1, 0, 0);

	glPopMatrix();//knee cyl inner
	//knee cylinder outer
	glPushMatrix();//knee cyl outer
	glTranslatef(-0.6, -5.5, 1);
	glRotatef(90, 0, 1, 0);
	drawCylinder(0.5, 0.5, 0.5, 1, 1, 1);
	drawDisk(0, 0.5, 1, 1, 0);
	//disk inner
	glTranslatef(0, 0, 0.5);
	drawDisk(0, 0.5, 1, 0, 0);
	//disk inner
	glPopMatrix();//knee cyl outer

	//btm leg
	glPushMatrix(); //btm leg
	glTranslatef(0, -5.5, 1);

	glRotatef(anglex, 1, 0, 0);
	glTranslatef(0, 5.5, -1);




	//small leg
	glPushMatrix();//small leg

	glTranslatef(-0.32, -8, 0.5);
	linecube(2, 0.75, 1, 0, 0, 0, 1);
	cube(2, 0.75, 1, 1, 1, 1);
	glPushMatrix();//knee cap
	glTranslatef(0.1, 1.7, 0.5);
	glRotatef(30, 1, 0, 0);
	cube(2, 0.5, 0.5, 1, 1, 1);
	linecube(2, 0.5, 0.5, 0, 0, 0, 1);

	glTranslatef(0, -1.5, 1);
	glRotatef(angley, 1, 0, 0);
	glTranslatef(0, 1.5, -1);


	//ankle
	glPushMatrix();
	glRotatef(-30, 1, 0, 0);
	glTranslatef(-0.5, -2.7, -1);

	linecube(1, 1.5, 2.5, 0, 0, 0, 1);
	cube(1, 1.5, 2.5, 1, 1, 1);
	glTranslatef(0, 0, 3.5);
	glRotatef(90, 0, 1, 0);
	shoes(1.5, 1, 1, 1, 1, 0);


	glPopMatrix();//ankle
	glPopMatrix();//knee cap

	glDeleteTextures(1, &textureArr[1]);
}

void rightleg() {
	GLuint textureArr[2];
	if (changetext == false) {
		textureArr[1] = loadTexture("metal2.bmp");
	}
	else { textureArr[1] = loadTexture("redmetal.bmp"); }

	{
		glPushMatrix();//whole leg
		glTranslatef(1.65, 0, 0);
		glTranslatef(0, -1.5, 0.5);
		glRotatef(-angle, 1, 0, 0);
		glTranslatef(0, 1.5, -0.5);
		//glTranslatef(tx, 0, 0);
		//thigh
		glPushMatrix();//thigh
		glTranslatef(-0.32, -1.5, 0.5);
		linecube(1.5, 0.75, 1, 0, 0, 0, 1);
		cube(1.5, 0.75, 1, 1, 1, 1);
		glPopMatrix();//thigh

		//connector
		glPushMatrix();//conn
		glTranslatef(-0.55, -2, 0.41);

		linecube(0.5, 1.2, 1.2, 0, 0, 0, 1);
		cube(0.5, 1.2, 1.2, 1, 1, 1);
		glPopMatrix();//conn

		glPushMatrix();//bigleg
		glTranslatef(-0.32, -5, 0.5);
		linecube(3, 0.75, 1, 0, 0, 0, 1);
		cube(3, 0.75, 1, 1, 1, 1);
		glPopMatrix();//bigleg



		//knee cylinder inner	
		glPushMatrix();//knee cyl inner
		glTranslatef(0.2, -5.5, 1);
		glRotatef(90, 0, 1, 0);
		drawCylinder(0.5, 0.5, 0.5, 1, 1, 1);

		drawDisk(0, 0.5, 1, 1, 0);

		glTranslatef(0, 0, 0.5);
		drawDisk(0, 0.5, 1, 0, 0);

		glPopMatrix();//knee cyl inner
		//knee cylinder outer
		glPushMatrix();//knee cyl outer
		glTranslatef(-0.6, -5.5, 1);
		glRotatef(90, 0, 1, 0);
		drawCylinder(0.5, 0.5, 0.5, 1, 1, 1);
		drawDisk(0, 0.5, 1, 1, 0);
		//disk inner
		glTranslatef(0, 0, 0.5);
		drawDisk(0, 0.5, 1, 0, 0);
		//disk inner
		glPopMatrix();//knee cyl outer

		//btm leg
		glPushMatrix(); //btm leg
		glTranslatef(0, -5.5, 1);

		glRotatef(-anglex, 1, 0, 0);
		glTranslatef(0, 5.5, -1);




		//small leg
		glPushMatrix();//small leg

		glTranslatef(-0.32, -8, 0.5);
		linecube(2, 0.75, 1, 0, 0, 0, 1);
		cube(2, 0.75, 1, 1, 1, 1);
		glPushMatrix();//knee cap
		glTranslatef(0.1, 1.7, 0.5);
		glRotatef(30, 1, 0, 0);
		cube(2, 0.5, 0.5, 1, 1, 1);
		linecube(2, 0.5, 0.5, 0, 0, 0, 1);
		//ankle

		glTranslatef(0, -1.5, 1);
		glRotatef(angley, 1, 0, 0);
		glTranslatef(0, 1.5, -1);

		glPushMatrix();
		glRotatef(-30, 1, 0, 0);
		glTranslatef(-0.5, -2.7, -1);

		linecube(1, 1.5, 2.5, 0, 0, 0, 1);
		cube(1, 1.5, 2.5, 1, 1, 1);
		glTranslatef(0, 0, 3.5);
		glRotatef(90, 0, 1, 0);
		shoes(1.5, 1, 1, 1, 1, 0);

		glPopMatrix();//ankle
		glPopMatrix();//knee cap
		glPopMatrix();//smallleg
		glPopMatrix(); //btm leg
		glPopMatrix();//whole leg
	}

	glPopMatrix();//smallleg



	glPopMatrix(); //btm leg
	glDeleteTextures(1, &textureArr[1]);
}

void waist() {
	GLuint textureArr[2];
	if (changetext == false) {
		textureArr[1] = loadTexture("metal2.bmp");
	}
	else { textureArr[1] = loadTexture("redmetal.bmp"); }
	{
		// btm left cube
		glPushMatrix();

		glTranslatef(-0.1, 0, 0);

		cube(1, 1, 2, 0.8, 0.761, 0.737);
		linecube(1, 1, 2, 0, 0, 0, 1);
		// btm right cube

		glPushMatrix(); //right
		glTranslatef(1, 0, 0);
		cube(1, 1, 2, 0.8, 0.761, 0.737);
		linecube(1, 1, 2, 0, 0, 0, 1);
		glPopMatrix();//right


		//top left cube
		glPushMatrix();
		glTranslatef(0, 1, 0);
		cube(1, 1, 2, 0.8, 0.761, 0.737);
		linecube(1, 1, 2, 0, 0, 0, 1);
		glPopMatrix();

		//top right cube
		glPushMatrix();
		glTranslatef(1, 1, 0);
		cube(1, 1, 2, 0.8, 0.761, 0.737);
		linecube(1, 1, 2, 0, 0, 0, 1);
		glPopMatrix();

		glPopMatrix();
	}

	//bottom body block
	{
		//middle cube
		glPushMatrix();
		glTranslatef(0.75, -1.5, 0);
		linecube(1.5, 0.3, 2, 0, 0, 0, 1);
		cube(1.5, 0.3, 2, 1, 1, 1);
		glPopMatrix();
		//left cube
		glPushMatrix();
		glTranslatef(0.45, -1.5, 0);
		linecube(1.5, 0.3, 2, 0, 0, 0, 1);
		cube(1.5, 0.3, 2, 1, 1, 1);
		glPopMatrix();
		//right cube
		glPushMatrix();
		glTranslatef(1.05, -1.5, 0);
		linecube(1.5, 0.3, 2, 0, 0, 0, 1);
		cube(1.5, 0.3, 2, 1, 1, 1);
		glPopMatrix();

		glDeleteTextures(1, &textureArr[1]);
	}
}

GLuint loadTexture(LPCSTR fileName) {


	//step 1
	GLuint texture = 0;		//texture name


	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	//step 3
	HBITMAP hBMP = (HBITMAP)LoadImage(GetModuleHandle(NULL), fileName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	GetObject(hBMP, sizeof(BMP), &BMP);

	//step 4
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BMP.bmWidth, BMP.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);


	DeleteObject(hBMP);
	return texture;
}


void background() {

	//glColor3f(1, 1, 1);
	//glBegin(GL_POLYGON);//back
	//glTexCoord2f(0, 1);
	//glVertex3f(-20, -20, -9.9);	// Top-left

	//glTexCoord2f(1, 1);
	//glVertex3f(20, -20, -9.9);  // Top-right

	//glTexCoord2f(1, 0);
	//glVertex3f(20, 20, -9.9);   // Bottom-right

	//glTexCoord2f(0, 0);
	//glVertex3f(-20, 20, -9.9);	// Bottom-left
	//glEnd();

	//glBegin(GL_POLYGON);//left

	//glTexCoord2f(0, 1);
	//glVertex3f(-20, 20, -9.9);   // front-left

	//glTexCoord2f(1, 1);
	//glVertex3f(-20, 20, 5);      // back-left

	//glTexCoord2f(1, 0);
	//glVertex3f(-20, -20, 5);   // Back-right

	//glTexCoord2f(0, 0);
	//glVertex3f(-20, -20, -5);      // front -right
	//glEnd();


	//glBegin(GL_POLYGON);//right

	//glTexCoord2f(0, 1);
	//glVertex3f(-20, -20, -5);   // front btm

	//glTexCoord2f(1, 1);
	//glVertex3f(-20, -20, 5);      //front top

	//glTexCoord2f(1, 0);
	//glVertex3f(20, -20, 5);  //back-top

	//glTexCoord2f(0, 0);
	//glVertex3f(20, -20, -5);      //back-btm
	//glEnd();

	//glBegin(GL_POLYGON);//back

	//glTexCoord2f(0, 1);
	//glVertex3f(wd, 0, lg);  // btm - right

	//glTexCoord2f(1, 1);
	//glVertex3f(wd, hg, lg); //top-right

	//glTexCoord2f(1, 0);
	//glVertex3f(0, hg, lg);	//top-left

	//glTexCoord2f(0, 0);
	//glVertex3f(0, 0, lg);   //btm-left
	//glEnd();

	//glBegin(GL_POLYGON);//left

	//glTexCoord2f(0, 1);
	//glVertex3f(0, 0, lg);   // back - btm

	//glTexCoord2f(1, 1);
	//glVertex3f(0, 0, 0);      //front - btm

	//glTexCoord2f(1, 0);
	//glVertex3f(0, hg, 0);  //front - top

	//glTexCoord2f(0, 0);
	//glVertex3f(0, hg, lg);      //top-right
	//glEnd();


	//glBegin(GL_POLYGON); //top

	//glTexCoord2f(0, 1);
	//glVertex3f(0, hg, lg);   // back - btm

	//glTexCoord2f(1, 1);
	//glVertex3f(wd, hg, lg);      //front - btm

	//glTexCoord2f(1, 0);
	//glVertex3f(wd, hg, 0);  //front - top

	//glTexCoord2f(0, 0);
	//glVertex3f(0, hg, 0);      //top-right
	//glEnd();



}