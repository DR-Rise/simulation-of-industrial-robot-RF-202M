#pragma comment(lib,"legacy_stdio_definitions.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glaux.lib")

#include <afxwin.h>
#include "gl/gl.h"
#include "gl/glu.h"
#include "glaux.h" //(may be not included to MSVS)
#include "App.h"
#include<math.h>

int hspos = 50, vspos = 50;
int rotElbow = 0, rotShoulder = 0, rotColumn=0;
long timerAngle = 0;

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
	SetTimer(1, 100, NULL);
}

BEGIN_MESSAGE_MAP(CMainWin,CFrameWnd)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_CHAR()
//	ON_WM_TIMER()
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
	glRotated(timerAngle, 0, 1, 0);

	RobotTask();
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

void CMainWin::OnChassis(void)
{ // the base of chassis - rectangle
	glPushMatrix();  // remember the CS
	glRotatef(90.0, 1.0, 0.0, 0.0);//rotation of CS
	glColor3f(0.0, 0.5, 1.0);
	glRecti(0, 0, 6, 6);   //plane of chassis
	glPopMatrix();   // restoration of saved CS
	// wheels of chassis
	glPushMatrix(); // remember the CS
	glColor3f(1.0, 0.5, 0.0);
	auxWireTorus(0.5, 1.0); // wheel 1
	glTranslatef(6.0, 0.0, 0.0); // translation
	auxWireTorus(0.5, 1.0); // wheel 2
	glTranslatef(0.0, 0.0, 6.0); // translation
	auxWireTorus(0.5, 1.0); // wheel 3
	glTranslatef(-6.0, 0.0, 0.0); // translation
	auxWireTorus(0.5, 1.0); // wheel 4
	glPopMatrix(); // restoration of saved CS
}

void CMainWin::OnRocket(void)
{
	glPushMatrix();
	glScalef(0.3, 0.3, 0.3);
	glPushMatrix();// save of initial position of base of rocket
	glColor3f(0.0, 0.0, 1.0);
	glTranslatef(0.0, -1.0, 0.0);
	auxWireCylinder(1.5, 11.0);// parameters - radius, height
	glTranslatef(0.0, -10.0, 0.0); // moving to the top of cylinder
	glRotatef(90.0, 1.0, 0.0, 0.0);// rotation of cone for 90 around Õ
	auxWireCone(1.5, 3.0);// parameters - radius, height
	glPopMatrix();// restoration of CS
	glPushMatrix();
	for (int i = 0; i < 4; i++)	// loop with wings making
	{
		glRotatef(90.0 * i, 0.0, 1.0, 0.0);
		glColor3f(1.0, 0.0, 0.0);
		glBegin(GL_TRIANGLES); // wing
		glVertex3f(1.5f, 0.0f, 0.0f); glVertex3f(3.5f, 0.0f, 0.0f);
		glVertex3f(1.5f, -3.0f, 0.0f);
		glEnd();
	}
	glPopMatrix(); 
	glPopMatrix();
}

void CMainWin::OnRobot(void) // start
{
	glPushMatrix();
	glTranslatef(0.0, -3.0, 0.0);// initial transition of scene
	glRotatef(360.0 * vspos / 100, 0, 1, 0);  // 3D-rotation around Y-axis
	glRotatef(180, 0, 0, 1); // initial rotation of scene for 180 degrees around Z-axis
	glLineWidth(2.0);
	// base
	glPushMatrix();// save the location of robot’s base
	glRotatef(90.0, 1.0, 0.0, 0.0);// rotate the cone for 90 degrees around X-axis
	glColor3f(0.0, 0.0, 1.0);
	auxWireCone(1.5, 1.0);// first parameter - radius, second - height
	glPopMatrix();// restoration of coordinate system
	glTranslatef(0.0, -1.0, 0.0);// change of height for -1.0 along Y-axis 
	glColor3f(0.0, 0.5, 0.5);
	auxWireCylinder(0.6, 4.0);// making of cylinder – robot’s column
	// shoulder
	glPushMatrix();// save the location of cylinder’s end
	// Cylinder of shoulder is rotated for 90 degrees around X-axis
	glTranslatef(0.0, -3.0, 0.0); // moving to the end of column
	glRotatef(90.0, 1.0, 0.0, 0.0); // rotation for 90 degrees around X-axis
	glColor3f(1.0, 0.0, 0.0);
	auxWireCylinder(0.6, 2.0); // cylinder of shoulder joint
	// prism of shoulder form
	glRotatef(180.0 * rotShoulder / 100, 0.0, 1.0, 0.0); // moving of shoulder by horizontal 
	// track
	glTranslatef(1.0, 0.5, 0.0); // moving to the end of êð³ïëåííÿ 
	glColor3f(0.5, 0.5, 0.5);
	auxWireBox(2.0, 1.0, 1.2); // prism of shoulder 
	glTranslatef(1.0, -0.5, 0.0);	// moving to the end of shoulder prism
	glColor3f(1.0, 0.0, 0.0);
	auxWireCylinder(0.6, 1.0);	// ending cylinder of shoulder
	// Prism of elbow form
	glRotatef(90.0 * rotElbow / 100, 0.0, 1.0, 0.0); // Rotation of elbow by horizontal track bar
	glTranslatef(1.0, 0.5, 0.0);	// moving to the center of elbow
	glColor3f(0.0, 0.5, 0.5);
	auxWireBox(2.0, 1.0, 0.8);	// prism of elbow 
	// Ending cylinder of elbow
	glTranslatef(1.0, -0.5, 0.0);	// moving to the end of elbow prism
	glColor3f(1.0, 0.0, 0.0);
	auxWireCylinder(0.4, 1.0);	// ending cylinder of elbow
	// Robot’s Arm (gripper) 
	glColor3f(1.0, 0.0, 0.0);
	glTranslatef(0.5, 0.5, 0.0);	// moving to the center of gripper base
	auxWireBox(1.0, 0.7, 0.4);	// prism of gripper base
	glColor3f(0.0, 0.0, 1.0);
	glRotatef(90.0, 0.0, 0.0, 1.0);	// rotate 90 degrees around Z
	glTranslatef(0.2, -1.5, 0.0); 	// moving to the end of gripper base
	auxWireCylinder(0.1, 0.4);	// cylinder of first finger
	glTranslatef(-0.4, 0.0, 0.0);	// moving to other side of gripper base
	auxWireCylinder(0.1, 0.4);	// cylinder of second  finger
	glPopMatrix();	glPopMatrix();
}  	// return to the column base


void CMainWin::OnChar(UINT nChar, UINT nRep, UINT nFlag)
{
	// TODO: Add your implementation code here.
	if (nChar == 'a')rotColumn+=5;
	if (nChar == 'z')rotColumn -=5;
	if (nChar == 's')rotShoulder += 5;
	if (nChar == 'x')rotShoulder -= 5;
	if (nChar == 'd')rotElbow += 5;
	if (nChar == 'c')rotElbow -= 5;
	Invalidate(NULL);
}


void CMainWin::RobotTask()
{
	// TODO: Add your implementation code here.
	// Coordinate axis
	glPushMatrix();
	// Base
	




	glColor3f(1.0, 0.0, 0.0);
	//glTranslatef(0.0, 0.25, 0.0);
	auxWireBox(2.5, 0.5, 1.5);
	//baseCyln
	glColor3f(0.0, 0.0, 1.0);
	glTranslatef(0.0, -1.25, 0.0);
	auxWireCylinder(0.6, 3.0);
	glTranslatef(0.0, -2.25, 0.0);
	auxWireBox(0.75, 0.75, 0.75);
	glRotatef(90.0, 0.0, 0.0, -1.0);
	glColor3f(1.0, 0.0, 0.0);
	auxWireCylinder(0.1, 0.8);
	glColor3f(0.0, 0.0, 1.0);
	glTranslatef(0.0, 1.25, 0.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	auxWireBox(2.0, 0.5, 0.75);
	glColor3f(0.0, 1.0, 0.0);
	glTranslatef(-1.0, 0.0, 0.0);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	auxWireCylinder(0.2, 1);
	glTranslatef(0.0, -2.0, 0.0);
	glRotatef(90.0, 0.0, 0.0, -1.0);
	glRotatef(90.0, 0.0, 0.0, -1.0);
	auxWireCylinder(0.2, 1);
	glTranslatef(0.0, 1.0, 0.0);
	auxWireCylinder(0.2, 1);
	glTranslatef(0.0, 1.0, 0.0);
	auxWireCylinder(0.2, 1);

	glTranslatef(0.0, 1.20, 0.0);
	glColor3f(0.0, 0.0, 1.0);
	auxWireBox(1, 0.4, 0.4);
	glTranslatef(0.2, 0.00, 0.0);
	glColor3f(1.0, 0.0, 0.0);
	auxWireCylinder(0.1, 0.8);
	glTranslatef(-0.4, 0.0, 0.0);
	auxWireCylinder(0.1, 0.8);
	

	glPopMatrix();
}


void CMainWin::OnTimer(UINT nID)
{
	// TODO: Add your implementation code here.
	timerAngle+=2;
	Invalidate(NULL);
}
