#include <math.h>
#include <time.h>
#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "classes/ModelLoader.h"
#include "classes/Physics.h"
#include "classes/Shaders.h"
#include "classes/GameObject.h"
#include "classes/Floor.h"

float deltaX, deltaY;
float deltaS, deltaSt;

float defaultSpeed = 1.0f;
float speed = 1.0f;

float lastMousePosY = HEIGHT / 2;
float lastMousePosX = WIDTH / 2;

double lastTime;
int nFrames;

bool canRelease = 1;
bool canReshape = 1;
bool release = 0;
bool fullscreen = 0;
bool gravity = 0;
bool down = 0;
bool up = 0;

GLint jumpTicks = MAX_JUMPTICKS;
GLint invertVal = 1;

GLuint prog;
GLenum mode = GL_TRIANGLES;
GLint menuId;

AABB playerBounds;
GameObject objects[GAMEOBJECTS];

matrix projMatrix;
matrix modelViewMatrix;

double currentTime;

void wireMenuActions(int value)
{
	switch(value)
	{
	case 1:
		mode = GL_LINE_STRIP;
		break;
	case 0:
		mode = GL_TRIANGLES;
		break;
	default:
		break;
	}
}

void gravMenuActions(int value)
{
	switch(value)
	{
	case 1:
		if(!gravity)
		{
			down = 1;
			fdy = -VELOCITY;
		}
		gravity = 1;
		break;
	case 0:
		gravity = 0;
		break;
	default:
		break;
	}
}

void invMenuActions(int value)
{
	switch(value)
	{
	case 1:
		invertVal = -1;
		break;
	case 0:
		invertVal = 1;
		break;
	}
}

int createBoolMenu(void (*f)(int))
{
	int bMenu;
	bMenu = glutCreateMenu(f);

	glutAddMenuEntry("On", 1);
	glutAddMenuEntry("Off", 0);

	return bMenu;
}

int createMenu()
{
	int menu, wireMenu, gravMenu, invMenu;

	wireMenu = createBoolMenu(wireMenuActions);
	gravMenu = createBoolMenu(gravMenuActions);
	invMenu = createBoolMenu(invMenuActions);

	menu = glutCreateMenu(NULL);
	glutAddSubMenu("Gravity", gravMenu);
	glutAddSubMenu("Wireframe", wireMenu);
	glutAddSubMenu("Invert Mouse", invMenu);

	return menu;
}

void menuStatus(int status,int x, int y)
{
	if(status == GLUT_MENU_IN_USE)
		release = 1;
	else if(status == GLUT_MENU_NOT_IN_USE && release)
		release = 0;
}

void direction(float delta_h, float delta_v)
{
	angle += delta_h;
	vx = sin(angle);
	vz = -cos(angle);
	vy += sin(delta_v) * cos(delta_v);
}

void position(float delta_x, float delta_z, float s)
{
	fdx = delta_z * vx * s;
	fdz = delta_z * vz * s;
	sdx = delta_x * -vz * s;
	sdz = delta_x * vx * s;

	checkAABBPhysics(&playerBounds, objects, GAMEOBJECTS);

	if(canMove[0])
	{
		x += fdx;
		z += fdz;
	}
	if(canMove[2])
	{
		x += sdx;
		z += sdz;
	}
}

void setCoords(GameObject* o)
{
	srand(time(NULL));

	float cx, cy, cz;
	int i;

	for(i = 0; i < o->pamt; i ++)
	{
		cx = (float)rand() / ((float)RAND_MAX / FIELD_WIDTH);
		cy = (float)rand() / ((float)RAND_MAX / FIELD_HEIGHT);
		cz = (float)rand() / ((float)RAND_MAX / FIELD_DEPTH);

		cx -= FIELD_WIDTH / 2.0f;
		cy -= FIELD_HEIGHT / 2.0f;
		cz -= FIELD_DEPTH / 2.0f;

		moveObject(o, i, cx, cy, cz);
	}
}

void loadgame()
{
	prog = loadShaderProgram("res/shaders/vertex.vs", "res/shaders/fragment.fs");

	objects[0] = createFloor("res/images/floor.png", 1, prog);

	int indexAmnt;
	int* box_indices;
	Vertex* box_verts = loadObj("res/models/box.obj", 5.0f, &box_indices, &indexAmnt);
	objects[1] = createObject("res/images/box.png", box_verts, box_indices, indexAmnt, indexAmnt, OBJECT_AMT, prog);

	int* t_indices;
	Vertex* t_verts = loadObj("res/models/terminator_fix.obj", 1.0f, &t_indices, &indexAmnt);
	objects[2] = createObject("res/images/terminator.png", t_verts, t_indices, indexAmnt, indexAmnt, 2, prog);

	setCoords(&objects[1]);
	setCoords(&objects[2]);

	moveObjectAll(&objects[2], 0.0f, 5.25f, 0.0f);

	menuId = createMenu();
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glClearColor(0.6f, 0.6f, 1.0f, 1.0f);

	initCamera();
	playerBounds = initAABB(-PLAYER_LENGTH / 2.0f, -PLAYER_HEIGHT / 2.0f, -PLAYER_LENGTH / 2.0f,
								PLAYER_LENGTH / 2.0f, 1.5f, PLAYER_LENGTH / 2.0f);
	moveAABB(&playerBounds, x, y, z);

	lastTime = glutGet(GLUT_ELAPSED_TIME);
}

void cleanup()
{
	glDeleteProgram(prog);
	glutDetachMenu(GLUT_RIGHT_BUTTON);
	glutDestroyMenu(menuId);
	deleteObjects(objects, GAMEOBJECTS);
}

void input(int key, int ix, int iy)
{
	if(key == GLUT_KEY_LSHIFT && !gravity){ fdy = -VELOCITY; down = 1; }
}

void input_r(int key, int ix, int iy)
{
	if(key == GLUT_KEY_LSHIFT && !gravity) down = 0;
}

void keyboard(unsigned char key, int kx, int ky)
{
	if(key == 'w') deltaS = 0.5f;
	if(key == 's') deltaS = -0.5f;
	if(key == 'a') deltaSt = -0.5f;
	if(key == 'd') deltaSt = 0.5f;
	if(key == 'v' && !gravity) noClip = 1;

	if(key == QUIT)
	{
		cleanup();
		exit(0);
	}
	if(key == ESC && canRelease)
	{
		release = !release;
		canRelease = 0;
	}

	if(key == 'r') speed = defaultSpeed * 5.0;
	if(key == ' ' && ((gravity && !down) || !gravity))
	{
		fdy = VELOCITY;
		up = 1;
	}
	if(key == 'f' && canReshape)
	{
		fullscreen = !fullscreen;

		if(fullscreen)
			glutFullScreen();
		else
			glutReshapeWindow(WIDTH, HEIGHT);

		canReshape = 0;
	}
}

void keyboard_r(unsigned char key, int kx, int ky)
{
	if(key == ESC) canRelease = 1;
	if(key == 'w' || key == 's') deltaS = 0.0f;
	if(key == 'a' || key == 'd') deltaSt = 0.0f;
	if(key == 'r') speed = defaultSpeed;
	if(key == 'v') noClip = 0;
	if(key == ' ')
	{
		up = 0;
		if(gravity)
		{
			down = 1;
			fdy = -VELOCITY;
		}
	}
	if(key == 'f') canReshape = 1;
}

void resetPointer()
{
	int ww = glutGet(GLUT_WINDOW_WIDTH);
	int wh = glutGet(GLUT_WINDOW_HEIGHT);

	glutWarpPointer(ww / 2, wh / 2);

	lastMousePosY = ww / 2.0;
	lastMousePosX = wh / 2.0;

	deltaX = 0.0f;
	deltaY = 0.0f;
}

void mouseMove(int mx, int my)
{
	if(!release)
	{
		int ww = glutGet(GLUT_WINDOW_WIDTH);
		int wh = glutGet(GLUT_WINDOW_HEIGHT);

		if(fabs(ww / 2.0f - mx) > MOUSE_CONSTRAINT ||
				fabs(wh / 2.0f - my) > MOUSE_CONSTRAINT)
					resetPointer();
		else
		{
			float vMotion = (lastMousePosY - my);
			float hMotion = (lastMousePosX - mx);

			if(vMotion > MOUSE_CONSTRAINT)
				vMotion = 3.0f;
			else if(vMotion < -MOUSE_CONSTRAINT)
				vMotion = -3.0f;
			if(hMotion > MOUSE_CONSTRAINT)
				hMotion = 3.0f;
			else if(hMotion < -MOUSE_CONSTRAINT)
				hMotion = -3.0f;

			deltaX = -hMotion / MOUSE_MOVE_FACTOR;
			deltaY = vMotion / MOUSE_MOVE_FACTOR * invertVal;

			lastMousePosY = my;
			lastMousePosX = mx;

			glutPostRedisplay();
		}
	}
}

void mouse(int button, int state, int mx, int my)
{
	if(button == GLUT_LEFT_BUTTON && release)
		release = 0;

	mouseMove(mx, my);
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glUseProgram(prog);

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(prog, "tex"), 0);

	glLoadIdentity();
	glRotatef(angle, 0.0, 1.0, 0.0);

	const double w = glutGet(GLUT_WINDOW_WIDTH);
	const double h = glutGet(GLUT_WINDOW_HEIGHT);

	glGetFloatv(GL_PROJECTION_MATRIX, projMatrix);
	glUniformMatrix4fv(glGetUniformLocation(prog, "projMatrix"), 1, GL_FALSE, projMatrix);

	int i, j;
	for(i = 0; i < GAMEOBJECTS; i ++)
		for(j = 0; j < objects[i].pamt; j ++)
		{
				glLoadIdentity();

				gluPerspective(45.0, w / h, 0.1, 1000.0);
				gluLookAt(x, y, z, x + vx, y + vy, z + vz, 0.0f, 1.0f,  0.0f);
				glTranslatef(objects[i].pos[j].x, objects[i].pos[j].y, objects[i].pos[j].z);

				glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMatrix);
				glUniformMatrix4fv(glGetUniformLocation(prog, "modelViewMatrix"), 1, GL_FALSE, modelViewMatrix);

				glBindTexture(GL_TEXTURE_2D, objects[i].tex);
				glBindVertexArray(objects[i].vao);
				glDrawElements(mode, objects[i].vamt, GL_UNSIGNED_INT, NULL);
		}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	int _h = (h > 0 ? h : 1);
	glViewport(0, 0, w, _h);
}

void checkGravPhysics()
{
	if(gravity)
	{
		checkAABBHeights(&playerBounds, objects, GAMEOBJECTS);

		if(!up && !down && canMove[1])
		{
			down = 1;
			fdy = -VELOCITY;
		}
		if(up)
		{
			if(canMove[1])
				y += fabs(fdy);

			jumpTicks ++;
			if(jumpTicks == MAX_JUMPTICKS || !canMove[1])
			{
				up = 0;
				down = 1;
				fdy = -VELOCITY;
			}
		}
		if(down)
		{
			if(canMove[1])
				y += -fabs(fdy);

			if(jumpTicks > 0)
				jumpTicks --;

			if(!canMove[1])
			{
				down = 0;
				jumpTicks = 0;
			}
		}
	}
	else
	{
		if(!up && !down)
			fdy *= 0.5f;

		if(fabs(fdy) >= 0.001f)
		{
			checkAABBHeights(&playerBounds, objects, GAMEOBJECTS);
			if(canMove[1])
				y += fdy;
		}
		else
			fdy = 0.0f;
	}
}

void update()
{
	if(deltaX || deltaY)
		direction(deltaX, deltaY);
	if(deltaS || deltaSt)
		position(deltaSt, deltaS, speed);

	checkGravPhysics();

	if(release)
		glutSetCursor(GLUT_CURSOR_INHERIT);
	else
		glutSetCursor(GLUT_CURSOR_NONE);

	deltaX = 0.0f;
	deltaY = 0.0f;

	currentTime = glutGet(GLUT_ELAPSED_TIME);
	nFrames ++;

	if(currentTime - lastTime >= 1000.0)
	{
		printf("%d fps (%f ms/frame)\n", nFrames, 1000.0 / (double)nFrames);
		defaultSpeed = (float)nFrames / 60.0f;
		nFrames = 0;
		lastTime += 1000.0;
	}
	glutPostRedisplay();
}

int main(int argc, string argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);

	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(100, 50);
	glutCreateWindow("First Person World");

	glutDisplayFunc(render);
	glutReshapeFunc(reshape);
	glutIdleFunc(update);
	glutMouseFunc(mouse);
	glutPassiveMotionFunc(mouseMove);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboard_r);
	glutSpecialFunc(input);
	glutSpecialUpFunc(input_r);
	glutMenuStatusFunc(menuStatus);
	glutIgnoreKeyRepeat(GL_TRUE);
	glutSetCursor(GLUT_CURSOR_NONE);

	GLenum status = glewInit();
	if(status != GLEW_OK)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(status));
		return 1;
	}

	printf("OpenGL version: %s\n", glGetString(GL_VERSION));
	printf("GLSL version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	printf("GLEW verison: %s\n", glewGetString(GLEW_VERSION));

	loadgame();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glutMainLoop();

	cleanup();

	return 0;
}
