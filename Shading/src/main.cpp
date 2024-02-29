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

GLuint VertexArray;
GLuint VertexBuffer;
GLuint IndexBuffer;
std::vector<unsigned int> Indices;

cy::TriMesh Object;
cy::GLSLProgram Program;
GLuint ProgramID;

cy::Vec3f CameraPosition {0.0f, 0.0f, 60.0f};
cy::Vec3f ObjectPosition {0.0f, 0.0f, 0.0f};
cy::Matrix4f ProjectionMatrix;

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	cy::Matrix4f ViewMatrix = cy::Matrix4f::View(CameraPosition, (Object.GetBoundMax() + Object.GetBoundMin()) * 0.5f, {0.0f, 1.0f, 0.0f});
	cy::Matrix4f ModelViewProjectionMatrix = ProjectionMatrix * ViewMatrix * cy::Matrix4f::RotationX(cy::ToRadians(315));
	glUniformMatrix4fv(glGetUniformLocation(ProgramID, "ModelViewProjectionMatrix"), 1, GL_FALSE, &ModelViewProjectionMatrix(0, 0));

	glEnableVertexAttribArray(0);
	glBindVertexArray(VertexArray);
	glUseProgram(ProgramID);
	glBindBuffer(GL_ARRAY_BUFFER, VertexArray);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);
	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
	glutSwapBuffers();
}

void Idle()
{
	glutPostRedisplay();
}

void Initialize()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	ProjectionMatrix = cy::Matrix4f::Perspective(cy::ToRadians(40.0f), static_cast<float>(Width) / static_cast<float>(Height), 0.1f, 1000.0f);

	Object.LoadFromFileObj("assets/teapot.obj", true);
	Object.ComputeBoundingBox();
	ObjectPosition = (Object.GetBoundMax() + Object.GetBoundMin()) * 0.5f;

	for (int FaceIndex = 0; FaceIndex < static_cast<int>(Object.NF()); FaceIndex++)
	{
		for (unsigned int Index : Object.F(FaceIndex).v)
		{
			Indices.push_back(Index);
		}
	}

	Program.BuildFiles("SimpleShader.vert", "SimpleShader.frag");
	ProgramID = Program.GetID();

	glGenVertexArrays(1, &VertexArray);
	glBindVertexArray(VertexArray);

	glGenBuffers(1, &VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cy::Vec3f) * Object.NV(), &Object.V(0), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glGenBuffers(1, &IndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), Indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
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
	glutIdleFunc(Idle);
	Initialize();
	glutMainLoop();
	return 0;
}