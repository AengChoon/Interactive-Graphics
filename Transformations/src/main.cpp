#include <random>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/GL.h>
#include <cyCodeBase/cyMatrix.h>
#include <cyCodeBase/cyGL.h>
#include <cyCodeBase/cyTriMesh.h>
#pragma comment (lib, "glew32.lib")

constexpr int Width = 600;
constexpr int Height = 600;

void Render()
{
	std::random_device RandomDevice;
	std::uniform_real_distribution<float> Distribution{0.0f, 1.0f};

	glClearColor(Distribution(RandomDevice), Distribution(RandomDevice), Distribution(RandomDevice), 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv); //to initialize GLUT
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(Width, Height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Transformations");
	glutDisplayFunc(Render);
	glewInit();
	glutMainLoop();
	return 0;
}