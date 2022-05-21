#pragma comment(lib,"legacy_stdio_definitions.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glaux.lib")


#include <afxwin.h>
#include "gl/gl.h"
#include "gl/glu.h"
#include "glaux.h" //(may be not included to MSVS)
#include "App.h"

int hspos = 50, vspos = 50;


BOOL CApp::InitInstance()
{
	m_pMainWnd = new CMainWin;
	m_pMainWnd->ShowWindow(m_nCmdShow);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}

CApp App;

CMainWin::CMainWin()
{
	Create(NULL, "OpenGL test", WS_OVERLAPPEDWINDOW |
		WS_VSCROLL | WS_HSCROLL);
	PIXELFORMATDESCRIPTOR pfd;
	pfd.dwFlags = PFD_DOUBLEBUFFER;
	CClientDC dc(this);
	int nPixelFormat = ChoosePixelFormat(dc.m_hDC, &pfd);
	SetPixelFormat(dc.m_hDC, nPixelFormat, &pfd);
	m_hrc = wglCreateContext(dc.m_hDC);
	wglMakeCurrent(dc.m_hDC, m_hrc);

}

BEGIN_MESSAGE_MAP(CMainWin, CFrameWnd)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

void CMainWin::OnSize(unsigned int type, int x, int y)
{
	CClientDC dc(this);
	wglMakeCurrent(dc.m_hDC, m_hrc);
	GLdouble gldAspect = (GLdouble)x / (GLdouble)y;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, gldAspect, 1.0, 10.0);
	glViewport(0, 0, x, y);
	wglMakeCurrent(NULL, NULL);
}

void CMainWin::OnPaint(void)
{
	CPaintDC pDC(this);
	wglMakeCurrent(pDC.m_hDC, m_hrc);
	GLInit();
	OnOpenGL();
	SwapBuffers(pDC.m_hDC);
	wglMakeCurrent(NULL, NULL);
}

void CMainWin::GLInit(void)
{
	GLdouble marengo[3] = { 1.0,1.0,0.0 };
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -5.0);
	glColor3dv(marengo);
	glScalef(0.25, 0.25, 0.25);
}

void CMainWin::OnOpenGL(void)
{
	glRotated(360.0 * hspos / 100, 0, 1, 0);
	glRotated(360.0 * vspos / 100, 1, 0, 0);
	OnRocket();
}

void CMainWin::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	char str[255];
	switch (nSBCode)
	{
	case SB_LINEDOWN: vspos++; break;
	case SB_LINEUP:vspos--; break;
	case SB_PAGEDOWN: vspos += 5; break;
	case SB_PAGEUP:vspos -= 5; break;
	case SB_THUMBTRACK: vspos = nPos; break;
	case SB_THUMBPOSITION: vspos = nPos; break;
	}
	SetScrollPos(SB_VERT, vspos);
	Invalidate(NULL);

}

void CMainWin::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	char str[255];
	switch (nSBCode)
	{
	case SB_LINERIGHT: hspos++; break;
	case SB_LINELEFT:hspos--; break;
	case SB_PAGERIGHT: hspos += 5; break;
	case SB_PAGELEFT:hspos -= 5; break;
	case SB_THUMBTRACK: hspos = nPos; break;
	case SB_THUMBPOSITION: hspos = nPos; break;
	}
	SetScrollPos(SB_HORZ, vspos);
	Invalidate(NULL);
}


void CMainWin::OnRocket(void)
{

	glPushMatrix();
	glTranslatef(0.0, 4.0, 0.0);

	glColor3f(0.752941, 0.752941, 0.752941);
	auxWireBox(5.5, 0.5, 3.5);  // base

	glColor3f(0.0, 0.0, 1.0);
	glTranslatef(-1.55, -1.25, 0.0);
	auxWireCylinder(0.8, 3.5);  // base cylind


	glTranslatef(0.0, -2.5, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	auxWireCone(0.8, 1.0);// parameters - radius, height
	glRotatef(90.0, -1.0, 0.0, 0.0);
	glTranslatef(0.0, 2.5, 0.0);



	glColor3f(0.0, 0.0, 0.0);
	glTranslatef(3.2, 0.5, 0.0);
	auxWireBox(2.0, 1.0, 3.0); //  base weight balancer

	glColor3f(1.0, 0.0, 0.0);
	glTranslatef(-3.2, -2.3, 0.8);
	glRotatef(90.0, 1, 0, 0);
	auxWireCylinder(0.15, 1.0); // Handler

	glColor3f(0.0, 0.0, 1.0);
	glTranslatef(0.0, 1.3, 0.0);
	glRotatef(90.0, 0, 0, -1);
	auxWireCylinder(0.3, 5.0);  //  first arm

	glTranslatef(0.0, 1.25, 0.0);
	glColor3f(0.0, 1.0, 0.0);
	auxWireBox(0.5, 0.5, 0.5); // arm weight balancer


	glColor3f(1.0, 0.0, 0.0);
	glTranslatef(0.0, -5.5, 0.0);
	auxWireBox(1, 0.4, 0.5);

	glColor3f(0.196078, 0.6, 0.8);
	glTranslatef(0.2, -1.2, 0.0);
	auxWireCylinder(0.1, 0.6);
	glTranslatef(-0.4, 0.0, 0.0);
	auxWireCylinder(0.1, 0.6);


	//

	glColor3f(1.0, 0.0, 0.0);
	glTranslatef(3.1, 5.45, 0.0);
	glRotatef(90.0, 0, 0, -1);
	auxWireCylinder(0.15, 1.0);// handler

	glColor3f(0.0, 0.0, 1.0);
	glTranslatef(0.0, 1.3, 0.0);
	glRotatef(90.0, 0, 0, 1);
	auxWireCylinder(0.3, 5.0); // arm

	glTranslatef(0.0, 1.25, 0.0);
	glColor3f(0.0, 1.0, 0.0);
	auxWireBox(0.5, 0.5, 0.5);  // arm weight balancer


	glColor3f(1.0, 0.0, 0.0);
	glTranslatef(0.0, -5.5, 0.0);
	auxWireBox(1, 0.4, 0.5);

	glColor3f(0.196078, 0.6, 0.8);
	glTranslatef(0.2, -1.2, 0.0);
	auxWireCylinder(0.1, 0.6);
	glTranslatef(-0.4, 0.0, 0.0);
	auxWireCylinder(0.1, 0.6);


	/*
	glBegin(GL_LINES);		// axis of coordinates
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(3.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 0.0f);//X
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0f, 3.0f, 0.0f); glVertex3f(0.0f, 0.0f, 0.0f);//Y
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0f, 0.0f, 3.0f); glVertex3f(0.0f, 0.0f, 0.0f);//Z
	glEnd();*/

	glPopMatrix();





}
