#include "Octree.h"
#include "MakeOctree.h"
#include "Sphere.h"
#include <math.h>
#include <GL/glut.h>

static Octree* octree;

static Vec3f pos(0, 0, -300.0f);
static float xRot = 10.0f;
static float yRot = 0;
// Mouse buttons
static int mouseButton[3] = { GLUT_UP, GLUT_UP, GLUT_UP };
static int numTris = 20;
static Sphere BALL;
static Leaves LEAVES;

void BuildOctree();
float MyRand(float range)
{
    return ((float)rand() / (float)RAND_MAX * 2.0 * range - range);
}

void key(unsigned char key, int, int)
{
    switch (key)
    {
    case 'P':
    case 'p':
        BuildOctree();
		BALL.setPos(Vec3f(MyRand(70),MyRand(70),MyRand(70)));
        break;
    }
}

void mousedown(int button, int state, int x, int y)
{
  mouseButton[button] = state;
}

void mousemove(int x, int y)
{
  static int oldx = x;
  static int oldy = y;
  int xdiff = x - oldx;
  int ydiff = y - oldy;
  oldx = x;
  oldy = y;
  
  if (mouseButton[GLUT_LEFT_BUTTON] == GLUT_DOWN)
  {
    // Rotate
    yRot += (float)xdiff;
    xRot += (float)ydiff;
  }
  else if (mouseButton[GLUT_MIDDLE_BUTTON] == GLUT_DOWN)
  {
    // Pan
    pos = Vec3f(pos.x + (float)xdiff, pos.y - (float)ydiff, pos.z);
    // screen y-coord is "upside-down"
  }
  else if (mouseButton[GLUT_RIGHT_BUTTON] == GLUT_DOWN)
  {
    // Zoom
    pos = Vec3f(pos.x, pos.y, pos.z + (float)ydiff);
  }
}

void DrawTris(Tris& tris)
{
    glBegin(GL_TRIANGLES);

    for (unsigned int i = 0; i < tris.size(); i++)
    {
        tris[i].Draw();
    }

    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(pos.x, pos.y, pos.z);
    glRotatef(xRot, 1.0f, 0, 0);
    glRotatef(yRot, 0, 1.0f, 0);
	glColor3f(0,0.7,1);
	BALL.Draw();
	octree->getIntersectingLeaves(Capsule(BALL,Vec3f()),&LEAVES);
    octree->Draw();
	LEAVES.clear();

	
	

    glutSwapBuffers();

    glutPostRedisplay();
}

// return float in range -range to +range -- so may be zero

void BuildOctree()
{
    delete octree;
    Tris tris;
    for (int i = 0; i < numTris; i++)
    {
        Tri tri;
        tri.m_verts[0].m_point.x = MyRand(100.0);
        tri.m_verts[0].m_point.y = MyRand(100.0);
        tri.m_verts[0].m_point.z = MyRand(100.0);

        static const float SIZE = 8;
        tri.m_verts[1].m_point.x = tri.m_verts[0].m_point.x + SIZE; 
        tri.m_verts[1].m_point.y = tri.m_verts[0].m_point.y; 
        tri.m_verts[1].m_point.z = tri.m_verts[0].m_point.z; 

        tri.m_verts[2].m_point.x = tri.m_verts[0].m_point.x; 
        tri.m_verts[2].m_point.y = tri.m_verts[0].m_point.y; 
        tri.m_verts[2].m_point.z = tri.m_verts[0].m_point.z + SIZE; 

        tris.push_back(tri);
    }
    BBox box(-100, 100, -100, 100, -100, 100);
    octree = MakeOctree(tris, box, 0);
}

int main(int argc, char** argv)
{
	int width = 400;
	int height = 400;

	float BallX = MyRand(70);
	float BallZ = MyRand(70);
	float BallY = MyRand(70);

	BALL.setPos(Vec3f(BallX,BallY,BallZ));
	BALL.setRadius(15);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Hello!");
	glutDisplayFunc(display);
    glutMouseFunc(mousedown);
    glutMotionFunc(mousemove);
    glutPassiveMotionFunc(mousemove);
    glutKeyboardFunc(key);

    glClearColor(0, 0, 0, 0 );

	glViewport(0, 0, width, height );

	glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
	gluPerspective( 60.0, 1.0, 1.0, 1024.0 );
	
    glEnable(GL_DEPTH_TEST);

    BuildOctree();

	glutMainLoop();
}

