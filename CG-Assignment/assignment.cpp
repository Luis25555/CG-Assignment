
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLU32.lib")

//variables
float angle = 0.2;// angle for rotation of whole picture
float tSpeed = 0.2;//transformation speed
#define WINDOW_TITLE "OpenGL Window"
void drawSpehere(float rad);
void drawCylinder(double br, double tr, double h);
void drawCylinder2(double br, double tr, double h);
void drawDisk(double inr, double otr);

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
		else if ((wParam == VK_SPACE)) { tSpeed = 0; }
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



	//all
	glPushMatrix();

	glRotatef(angle, 0, 1, 0);
	angle += tSpeed;

	//cone
	glPushMatrix();
	glTranslatef(0, -0.81, 0);
	glRotatef(-90, 1, 0, 0);
	glColor3f(0.804, 0.635, 0.435);
	drawCylinder(0, 0.2, 0.5);
	glPopMatrix(); //cone

	//cone
	glPushMatrix();
	glTranslatef(0, -0.81, 0);
	glRotatef(-90, 1, 0, 0);
	glColor3f(0.898, 0.816, 0.749);
	drawCylinder2(0, 0.2, 0.5);
	glPopMatrix(); //cone


	//sphere 2
	glPushMatrix();
	glTranslatef(0, -0.18, 0);
	glColor3f(0.953, 0.627, 0.741);
	drawSpehere(0.19);
	glPopMatrix(); // sphere 2

	//sphere 1
	glPushMatrix();
	glTranslatef(0, -0.3, 0);
	glColor3f(0.98, 0.965, 0.89);
	drawSpehere(0.21);
	glPopMatrix(); // sphere 1

	//topings

	//stick
	glPushMatrix();
	glTranslatef(0, -0.3, 0);
	glRotatef(-45, 1, 0, 0);
	glColor3f(0.804, 0.635, 0.435);
	drawCylinder2(0.03, 0.03, 0.4);
	glPopMatrix(); //cone

	//sphere cherry
	glPushMatrix();
	glTranslatef(0.13, -0.07, 0);
	glColor3f(1, 0, 0);
	drawSpehere(0.06);
	glLineWidth(2);
	glBegin(GL_LINES);

	glVertex3f(0, 0, 0);
	glVertex3f(0.08, 0.12, 0);
	glVertex3f(0.08, 0.12, 0);
	glVertex3f(0.15, 0.15, 0);
	glEnd();
	glPopMatrix(); // sphere 1 cherry




	glPopMatrix(); //all

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
		700, 10, 800, 800,
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
	gluQuadricDrawStyle(sphere, GLU_FILL);
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

