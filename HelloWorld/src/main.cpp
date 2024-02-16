#include <GL/freeglut.h>
#include <GL/GL.h>
#include <random>

void HandleIdle()
{
	glutPostRedisplay();
}

void HandleKeys(unsigned char InKey, int InX, int InY)
{
	switch (InKey)
	{
	case 27: // ESC
		glutLeaveMainLoop();
		break;
	default:
		break;
	}
}

void render()
{
	std::random_device RandomDevice;
	std::uniform_real_distribution<float> Distribution{0.0f, 1.0f};

	glClearColor(Distribution(RandomDevice), Distribution(RandomDevice), Distribution(RandomDevice), 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Hello World");
	glutDisplayFunc(render);
	glutKeyboardFunc(HandleKeys);
	glutIdleFunc(HandleIdle);
	glutMainLoop();
	return 0;
}