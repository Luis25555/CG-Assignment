
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLU32.lib")

//variables
float angle = 0.2;// angle for rotation of whole picture
float tSpeed = 0.2;//transformation speed
float tx = 0, ty = 0, tz = 0;
bool isOrtho = true;
float ONear = -10.0;
float OFar = 10.0;
float PNear = 1.0;
float PFar = 21.0;
float ptx = -2, pty = 0, ptSpeed = 0.5; // projection translation matrix
float ptrx = 90, ptry = 0, prSpeed = 1;//prjection rotation angle

#define WINDOW_TITLE "OpenGL Window"
void drawSpehere(float rad);
void drawCylinder(double br, double tr, double h);
void drawCylinder2(double br, double tr, double h);
void drawDisk(double inr, double otr);
void cube2(float size); //draw rectangular cube
void cube(float size); // draw square cube
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
		else if ((wParam == VK_LEFT))  tSpeed = 0.2;
		else if ((wParam == VK_SPACE)) { tx = 0;tz = 0;ty = 0; }
		else if ((wParam == 'W')) ptrx += prSpeed;
		else if ((wParam == 'S')) ptrx -= prSpeed;
		else if ((wParam == 'A')) ptx -= ptSpeed;
		else if ((wParam == 'D')) ptx += ptSpeed;
		else if ((wParam == 'Q')) ptry -= prSpeed;
		else if ((wParam == 'E')) ptry += prSpeed;
		else if ((wParam == VK_UP))  tz += tSpeed;
		else if ((wParam == VK_DOWN))  tz -= tSpeed;


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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST); //enable the depth test
	glMatrixMode(GL_MODELVIEW_MATRIX);
	//glLoadIdentity();
	projection();
	glPushMatrix(); //all
	glTranslatef(tx, ty, tz);
	
	//body block
	// top left cube
	cube2(1);
	
	glPushMatrix(); //right
	glTranslatef(0, 0, 1);
	// top right cube
	cube2(1);
	glPopMatrix();//right
	
	//btm left cube
	glPushMatrix();

	glTranslatef(0, 1, 0);
	cube2(1);

	glPopMatrix();
	
	//btm right cube
	glPushMatrix();

	glTranslatef(0, 1, 1);
	cube2(1);

	glPopMatrix();
	
	
	glPopMatrix();//all
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

void drawCylinder(double br, double tr, double h) {
	GLUquadricObj* cylinder = NULL;
	cylinder = gluNewQuadric();
	glLineWidth(2);
	gluQuadricDrawStyle(cylinder, GLU_LINE);
	gluCylinder(cylinder, br, tr, h, 30, 20);
	gluDeleteQuadric(cylinder);

}

void drawCylinder2(double br, double tr, double h) {
	GLUquadricObj* cylinder = NULL;
	cylinder = gluNewQuadric();
	gluQuadricDrawStyle(cylinder, GLU_FILL);
	gluCylinder(cylinder, br, tr, h, 30, 30);
	gluDeleteQuadric(cylinder);

}

void drawSpehere(float rad) {
	GLUquadricObj* sphere = NULL;
	sphere = gluNewQuadric();
	gluQuadricDrawStyle(sphere, GLU_LINE);
	gluSphere(sphere, rad, 30, 30);
	gluDeleteQuadric(sphere);
}

void drawDisk(double inr, double otr) {
	GLUquadricObj* disk = NULL;
	disk = gluNewQuadric();
	gluQuadricDrawStyle(disk, GLU_LINE);
	gluDisk(disk, inr, otr, 30, 30);
	gluDeleteQuadric(disk);
}

void cube2(float size) {
	//cube 2
	//glLoadIdentity();
	
	glPushMatrix();
	
	glRotatef(angle, 0, 0, 1);
	glTranslatef(tx, ty, 0);
	
	//glTranslatef();
	glBegin(GL_LINE_LOOP);//front
	glVertex3f((2 * size), size, size);// Top-left
	glVertex3f((2 * size), 0, size);   // Top-right
	glVertex3f((2 * size), 0, 0);      // Bottom-right
	glVertex3f((2 * size), size, 0);   // Bottom-left
	glEnd();

	glBegin(GL_LINE_LOOP);//top
	glVertex3f((2 * size), size, 0);   // front-right
	glVertex3f((2 * size), size, size);      // front-left
	glVertex3f((4 * size), size, size);   // Back-left
	glVertex3f((4 * size), size, 0);      // Back-right
	glEnd();


	glBegin(GL_LINE_LOOP);//right
	glVertex3f((2*size), size, 0);   // top-left
	glVertex3f((2 * size), 0, 0);      //btm-left
	glVertex3f((4 * size), 0, 0);  //btm-right
	glVertex3f((4 * size), size, 0);      //top-right
	glEnd();

	glBegin(GL_LINE_LOOP);//btm
	glVertex3f((2 * size), 0, 0);   // top-left
	glVertex3f((2 * size), 0, size);      //btm-left
	glVertex3f((4 * size), 0, size);  //btm-right
	glVertex3f((4 * size), 0, 0);      //top-right
	glEnd();

	glBegin(GL_LINE_LOOP);//left
	glVertex3f((2 * size), 0, size);   // top-left
	glVertex3f((2 * size), size, size);      //btm-left
	glVertex3f((4 * size), size, size);  //btm-right
	glVertex3f((4 * size), 0, size);      //top-right
	glEnd();


	glBegin(GL_LINE_LOOP); //back
	glVertex3f((4 * size), 0, 0);// Bottom-right
	glVertex3f((4 * size), 0, size);   // Bottom-left
	glVertex3f((4 * size), size, size);      // Top-right
	glVertex3f((4 * size), size, 0);   // Top-left
	glEnd();


	glPopMatrix();


}


void cube(float size) {
	glBegin(GL_QUADS);
	//face1
	glColor3f(1, 1, 0);
	glVertex3f(0, 0, size);
	glVertex3f(size, 0, size);
	glVertex3f(size, 0, 0);
	glVertex3f(0, 0, 0);

	//face 2
	glColor3f(1, 1, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, size, 0);
	glVertex3f(0, size, size);
	glVertex3f(0, 0, size);

	//face3
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, size);
	glVertex3f(size, 0, size);
	glVertex3f(size, size, size);
	glVertex3f(0, size, size);

	//face4
	glColor3f(0, 1, 0);
	glVertex3f(0, size, size);
	glVertex3f(0, size, 0);
	glVertex3f(size, size, 0);
	glVertex3f(size, size, size);

	//face5
	glColor3f(1, 0, 1);
	glVertex3f(size, size, size);
	glVertex3f(size, 0, size);
	glVertex3f(size, 0, 0);
	glVertex3f(size, size, 0);

	//face 6
	glColor3f(0, 0, 1);
	glVertex3f(size, size, 0);
	glVertex3f(size, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, size, 0);

	glEnd();
}

void projection() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//glTranslatef(ptx, pty, 0);
	//glRotatef(pry, 0, 1, 0);

	if (isOrtho) {
		glOrtho(-10.0, 10.0, -10.0, 10.0, ONear, OFar);
	}
	else {
		gluPerspective(20, 1, -1, 1);
		glFrustum(-10.0, 10.0, -10.0, 10.0, PNear, PFar);
	}

	glTranslatef(ptx, pty, 0);
	glRotatef(ptrx, 1, 0, 0);
	glRotatef(ptry, 0, 1, 0);

}