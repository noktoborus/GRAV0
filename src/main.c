#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "share.h"


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

const double G = ((double)6.6742799999999995e-11);
double dt = 1.0;

const double MAXWEIGHT = 1000.0;
const double MAXSIZE = 20.0;

#if 0
#define SYSCOUNT 9
struct obj syste[SYSCOUNT] = 
{
	{260.0, 260.0, 100.0, 1.0f, 0.0f, 0.0f},
	{260.0, 300.0, 100.0, 0.0f, 1.0f, 0.0f},
	{260.0, 340.0, 100.0, 0.0f, 0.0f, 1.0f},
	{300.0, 260.0, 100.0, 0.5f, 0.5f, 0.5f},
	{300.0, 300.0, 100.0, 0.5, 0.5f, 1.0f},
	{300.0, 340.0, 100.0, 0.3f, 0.3f, 0.3f},
	{340.0, 260.0, 100.0, 0.7f, 0.7f, 0.7f},
	{340.0, 300.0, 100.0, 0.2f, 1.0f, 0.0f},
	{340.0, 340.0, 100.0, 1.0f, 0.7f, 0.5f}
};

#else
#define SYSCOUNT 7
struct obj syste[SYSCOUNT] = 
{
	{100.0, 20.0, 456.0, 1.0f, 0.0f, 0.0f},
	{67.0, 34.0, 93.0, 0.0f, 1.0f, 0.0f},
	{59.0, 70.0, 69.0, 0.0f, 0.0f, 1.0f},
	{300.0, 300.0, 1000.0, 0.5f, 0.5f, 0.5f},
	{325.0, 325.0, 100.0, 0.5, 0.5f, 1.0f},
	{600.0, 600.0, 700.0, 0.3f, 0.3f, 0.3f},
	{0.0, 600.0f, 10.0, 0.7f, 0.7f, 0.7f}
};
#endif

const unsigned int objs_count = SYSCOUNT;
struct path
{
	double X;
	double Y;
} *path[SYSCOUNT];

unsigned int paths[SYSCOUNT];
struct vec vecs[SYSCOUNT];

int opt_paths;
int opt_paths_draw;
int opt_notpause;
int opt_debinfo;
int opt_points;

void
drawLine (int x, int y, char *line)
{
	glRasterPos2i (x, y);
	while (*line)
		glutBitmapCharacter (GLUT_BITMAP_9_BY_15, *(line++));
}

void
display ()
{
	void *tmp;
	int m;
	unsigned int g;
	float sz;
	char line[1024];
	int y = glutGet (GLUT_WINDOW_HEIGHT);
	m = sizeof (syste) / sizeof (struct obj);
	glClear (GL_COLOR_BUFFER_BIT);

	glColor3f (0.0f, 0.0f, 1.0f);
	drawLine (580, 5, "X");
	drawLine (5, y -= 20, "Y");

	while (m--)
	{
		glColor4f (syste[m].R, syste[m].G, syste[m].B, syste[m].A);
		if (opt_debinfo)
		{
			snprintf (line, 1024, "M: %4u X: %4.10f Y: %4.10f", syste[m].weight, syste[m].X, syste[m].Y);
			drawLine (2, y -= 20, line);
			snprintf (line, 1024, "X: %4.10f Y: %4.10f", vecs[m].X, vecs[m].Y);
			drawLine (82, y-= 20, line);
		}
		glPushMatrix ();
		glTranslated ((float)syste[m].X, (float)syste[m].Y, 0.0f);

		sz = MAXSIZE * (syste[m].weight / MAXWEIGHT);
		glutSolidSphere (sz, 15, 15);
		glPopMatrix ();
		if (opt_paths && (!paths[m] || floor(path[m][paths[m] - 1].X) != floor(syste[m].X)
			   	|| floor(path[m][paths[m] - 1].Y) != floor(syste[m].Y)))
		{
			if (!(paths[m] % 1024))
			{
				tmp = realloc (path[m], sizeof (struct path) * (paths[m] + 1024));
				if (tmp)
				{
					path[m] = (struct path*)tmp;
				}
			}
			if (path[m])
			{
				path[m][paths[m]].X = syste[m].X;
				path[m][paths[m]].Y = syste[m].Y;
				paths[m]++;
			}
		}
		if (opt_paths_draw)
		{
			if ((g = paths[m]))
			{
				if (opt_points)
					glBegin (GL_POINTS);
				else
					glBegin (GL_LINES);
				while (g--)
				{
					glVertex3f ((GLfloat)path[m][g].X, (GLfloat)path[m][g].Y, 0.0f);
				}
				glEnd ();
			}
		}
	}
	glutSwapBuffers ();
}	

void
resize (int w, int h)
{
	glViewport (0, 0, w, h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glOrtho (0, (GLfloat)h, // left, right
		0, ((GLfloat)w), // bottom, top
		1000.0, -1000.0); // front, back
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
}

void
redisplay (int val)
{
	glutPostRedisplay ();
	glutTimerFunc (val, redisplay, val);
}

void
timer (int val)
{
	int c = 5000;
	if (opt_notpause) while (c--) recalc ();
	glutTimerFunc (val, timer, val);
}

void
key (unsigned char key, int x, int y)
{
	if (key == '1')
		opt_paths_draw ^= 1;
	else
	if (key == '2')
		opt_paths ^= 1;
	else
	if (key == ' ')
		opt_notpause ^= 1;
	else
	if (key == 'i')
		opt_debinfo ^= 1;
	else
	if (key == 'p')
		opt_points ^= 1;
}

int
main (int argc, char *argv[])
{
	int c = objs_count;
	opt_paths = 1;
	opt_paths_draw = 1;
	opt_notpause = 1;
	opt_debinfo = 1;
	opt_points = 1;
	while (c--)
	{
		path[c] = NULL;
		paths[c] = 0;
	}
	grav_init ();
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize (600, 600);
	glutCreateWindow ("hellp");

	glutKeyboardFunc (key);

	glutReshapeFunc (resize);
	glutDisplayFunc (display);
	glutTimerFunc (1, redisplay, 10);
	glutTimerFunc (1, timer, 1);
	// GL setup
	glShadeModel (GL_SMOOTH);
	//glLineWidth (2);
	//glPointSize (2);
	//
	//
	glutMainLoop ();
	return 0;
}


