#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/GL.h>
#include <cyCodeBase/cyMatrix.h>
#include <cyCodeBase/cyGL.h>
#include <cyCodeBase/cyTriMesh.h>
#pragma comment (lib, "glew32.lib")

constexpr int Width = 600;
constexpr int Height = 600;

GLuint VertexArray;
GLuint VertexBuffer;

cy::TriMesh Object;
cy::GLSLProgram Program;
GLuint ProgramID;

cy::Vec3f Position {0.0f, 30.0f, 60.0f};
cy::Matrix4f ProjectionMatrix;

void Idle()
{
	glutPostRedisplay();
}

void Initialize()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	ProjectionMatrix = cy::Matrix4f::Perspective(cy::ToRadians(40.0f), static_cast<float>(Width) / static_cast<float>(Height), 0.1f, 1000.0f);

	Object.LoadFromFileObj("assets/teapot.obj", true);

	Program.BuildFiles("SimpleShader.vert", "SimpleShader.frag");
	ProgramID = Program.GetID();

	glGenVertexArrays(1, &VertexArray);
	glBindVertexArray(VertexArray);

	glGenBuffers(1, &VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cy::Vec3f) * Object.NV(), &Object.V(0), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBindVertexArray(0);
}

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	const cy::Matrix4f ViewMatrix = cy::Matrix4f::View(Position, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});
	const cy::Matrix4f ModelMatrix = cy::Matrix4f::Scale(1.0f);
	cy::Matrix4f ModelViewProjectionMatrix = ProjectionMatrix * ViewMatrix * ModelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(ProgramID, "ModelViewProjectionMatrix"), 1, GL_FALSE, &ModelViewProjectionMatrix(0, 0));

	glEnableVertexAttribArray(0);
	glBindVertexArray(VertexArray);
	glUseProgram(ProgramID);
	glDrawArrays(GL_POINTS, 0, sizeof(cy::Vec3f) * Object.NV());
	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(Width, Height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Transformations");
	glewInit();
	glutDisplayFunc(Render);
	glutIdleFunc(Idle);
	Initialize();
	glutMainLoop();
	return 0;
}