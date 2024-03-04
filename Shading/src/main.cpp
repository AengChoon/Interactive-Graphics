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
GLuint LightVertexArray;
GLuint VertexBuffer;
GLuint LightVertexBuffer;
GLuint NormalBuffer;
GLuint IndexBuffer;
std::vector<unsigned int> Indices;

cy::TriMesh Object;
cy::GLSLProgram Program;
cy::GLSLProgram LightProgram;
GLuint ProgramID;
GLuint LightProgramID;

cy::Vec3f CameraPosition {0.0f, 0.0f, 60.0f};
cy::Vec3f ObjectPosition {0.0f, 0.0f, 0.0f};
cy::Vec3f LightPosition {-5.0f, 5.0f, 30.0f};
cy::Matrix4f ProjectionMatrix;

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glBindVertexArray(VertexArray);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glUseProgram(ProgramID);

	cy::Matrix4f ViewMatrix = cy::Matrix4f::View(CameraPosition, (Object.GetBoundMax() + Object.GetBoundMin()) * 0.5f, {0.0f, 1.0f, 0.0f});
	
	cy::Matrix4f ModelViewMatrix = ViewMatrix * cy::Matrix4f::RotationX(cy::ToRadians(300.0f));
	glUniformMatrix4fv(glGetUniformLocation(ProgramID, "ModelViewMatrix"), 1, GL_FALSE, &ModelViewMatrix(0, 0));

	cy::Matrix4f ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
	glUniformMatrix4fv(glGetUniformLocation(ProgramID, "ModelViewProjectionMatrix"), 1, GL_FALSE, &ModelViewProjectionMatrix(0, 0));

	cy::Vec3f LightViewPosition = (ViewMatrix *  LightPosition).XYZ();
	glUniform3f(glGetUniformLocation(ProgramID, "LightViewPosition"), LightViewPosition[0], LightViewPosition[1], LightViewPosition[2]);

	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glUseProgram(0);

	glBindVertexArray(LightVertexArray);
	glEnableVertexAttribArray(0);
	glUseProgram(LightProgramID);

	cy::Matrix4f LightModelViewProjectionMatrix = ProjectionMatrix * ViewMatrix * cy::Matrix4f::Translation(LightPosition);
	glUniformMatrix4fv(glGetUniformLocation(LightProgramID, "ModelViewProjectionMatrix"), 1, GL_FALSE, &LightModelViewProjectionMatrix(0, 0));

	glDrawArrays(GL_TRIANGLES, 0, 36);

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
	Object.ComputeNormals();
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

	glGenBuffers(1, &NormalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, NormalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cy::Vec3f) * Object.NVN(), &Object.VN(0), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glGenBuffers(1, &IndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), Indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	constexpr float LightVertices[] =
	{
        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f, -0.5f,  0.5f, 
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f, -0.5f,  0.5f, 

        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f, 

         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  

        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f,  0.5f, -0.5f, 
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
    };

	LightProgram.BuildFiles("LightShader.vert", "LightShader.frag");
	LightProgramID = LightProgram.GetID();

	glGenVertexArrays(1, &LightVertexArray);
	glBindVertexArray(LightVertexArray);

	glGenBuffers(1, &LightVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, LightVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(LightVertices), LightVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
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