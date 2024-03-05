#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/GL.h>
#include <cyCodeBase/cyMatrix.h>
#include <cyCodeBase/cyGL.h>
#include <cyCodeBase/cyTriMesh.h>
#pragma comment (lib, "glew32.lib")

constexpr int Width = 600;
constexpr int Height = 600;

bool bIsRotating = false;
bool bIsZooming = false;
cy::Vec2<int> LastMousePosition {0, 0};
cy::Vec2<int> CurrentMousePosition {0, 0};

GLuint VertexArray;
GLuint VertexBuffer;

cy::TriMesh Object;
std::string ObjectName;
cy::GLSLProgram Program;
GLuint ProgramID;

cy::Vec3f CameraPosition {0.0f, 0.0f, 60.0f};
cy::Vec3f ObjectPosition {0.0f, 0.0f, 0.0f};
cy::Matrix4f CameraRotationMatrix {cy::Matrix4f::Identity()};
cy::Matrix4f ModelScaleMatrix {cy::Matrix4f::Identity()};
cy::Matrix4f ProjectionMatrix;
bool bIsPerspective {true};

void Idle()
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
	case 112: // p
		{
			if (bIsPerspective)
			{
				const auto Right = Object.GetBoundMax().x;
				const auto Left = Object.GetBoundMin().x;
				const auto Top = Object.GetBoundMax().y;
				const auto Bottom = Object.GetBoundMin().y;

				ProjectionMatrix.Column(0) = {2 / (Right - Left), 0, 0, 0};
				ProjectionMatrix.Column(1) = {0, 2 / (Top - Bottom), 0, 0};
				ProjectionMatrix.Column(2) = {0, 0, 1 / (1000.0f - 0.1f), 0};
				ProjectionMatrix.Column(3) = {-(Right + Left) / (Right - Left), -(Top + Bottom) / (Top - Bottom), -0.1f / (1000.0f - 0.1f), 1};
			}
			else
			{
				ProjectionMatrix = cy::Matrix4f::Perspective(cy::ToRadians(40.0f), static_cast<float>(Width) / static_cast<float>(Height), 0.1f, 1000.0f);
			}

			bIsPerspective = !bIsPerspective;
		}
		break;
	default:
		break;
	}
}

void HandleSpecialKeys(int InKey, int InX, int InY)
{
	switch (InKey)
	{
		case GLUT_KEY_F6:
			Program.BuildFiles("SimpleShader.vert", "SimpleShader.frag");
		break;
	}
}

void HandleMouseButton(int InButton, int InState, int InX, int InY)
{
	if (InButton == GLUT_LEFT_BUTTON && InState == GLUT_DOWN)
	{
		bIsRotating = true;
		LastMousePosition = {InX, InY};
		CurrentMousePosition = {InX, InY};
	}
	else if (InButton == GLUT_RIGHT_BUTTON && InState == GLUT_DOWN)
	{
		bIsZooming = true;
		LastMousePosition = {InX, InY};
		CurrentMousePosition = {InX, InY};
	}
	if (InState == GLUT_UP)
	{
		bIsRotating = false;
		bIsZooming = false;
	}
}

void HandleMouseMove(int InX, int InY)
{
	if (bIsRotating || bIsZooming)
	{
		CurrentMousePosition = {InX, InY};
	}
}

void Initialize()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	ProjectionMatrix = cy::Matrix4f::Perspective(cy::ToRadians(40.0f), static_cast<float>(Width) / static_cast<float>(Height), 0.1f, 1000.0f);

	Object.LoadFromFileObj(ObjectName.c_str(), true);
	Object.ComputeBoundingBox();
	ObjectPosition = (Object.GetBoundMax() + Object.GetBoundMin()) * 0.5f;

	Program.BuildFiles("SimpleShader.vert", "SimpleShader.frag");
	ProgramID = Program.GetID();

	glGenVertexArrays(1, &VertexArray);
	glBindVertexArray(VertexArray);

	glGenBuffers(1, &VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cy::Vec3f) * Object.NV(), &Object.V(0), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

cy::Vec2<float> ToNormalizedDeviceCoordinate(const cy::Vec2<int>& InCoordinates)
{
	return {1.0f * static_cast<float>(InCoordinates.x) / Width * 2 - 1.0f, 1.0f * static_cast<float>(InCoordinates.y) / Height * 2 - 1.0f};
}

cy::Vec3<float> ToArcballVector(const cy::Vec2<int>& InCoordinate)
{
	constexpr float ArcballRadius = 1.0f;

	auto ArcballVector = cy::Vec3<float>{ToNormalizedDeviceCoordinate(InCoordinate), 0.0f};
	ArcballVector.y = -ArcballVector.y;
	const auto XSquared = ArcballVector.x * ArcballVector.x;
	const auto YSquared = ArcballVector.y * ArcballVector.y;

	if (XSquared + YSquared <= ArcballRadius * ArcballRadius)
	{
		ArcballVector.z = sqrt(ArcballRadius * ArcballRadius - (XSquared + YSquared));
	}
	else
	{
		ArcballVector = cy::Normalize(ArcballVector) * ArcballRadius;
	}

	return ArcballVector;
}

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(VertexArray);
	glEnableVertexAttribArray(0);
	glUseProgram(ProgramID);

	if (CurrentMousePosition != LastMousePosition)
	{
		if (bIsRotating)
		{
			const auto LastArcballVector = ToArcballVector(LastMousePosition);
			const auto CurrentArcballVector = ToArcballVector(CurrentMousePosition);
			const float RotationAngle = acos(std::min(1.0f, LastArcballVector.Dot(CurrentArcballVector)));
			const auto RotationAxis = LastArcballVector.Cross(CurrentArcballVector).GetNormalized();
			LastMousePosition = CurrentMousePosition;
			CameraRotationMatrix *= cy::Matrix4f::Rotation(RotationAxis, RotationAngle);
		}

		if (bIsZooming)
		{
			CameraPosition.z += static_cast<float>(CurrentMousePosition.y) - static_cast<float>(LastMousePosition.y);
			CameraPosition.z = cy::Clamp(CameraPosition.z, 10.0f, 1000.0f);
			LastMousePosition = CurrentMousePosition;
		}
	}

	cy::Matrix4f ViewMatrix = cy::Matrix4f::View(CameraPosition, (Object.GetBoundMax() + Object.GetBoundMin()) * 0.5f, {0.0f, 1.0f, 0.0f});
	ViewMatrix *= CameraRotationMatrix;
	const cy::Matrix4f ModelMatrix = ModelScaleMatrix;
	cy::Matrix4f ModelViewProjectionMatrix = ProjectionMatrix * ViewMatrix * ModelScaleMatrix;
	glUniformMatrix4fv(glGetUniformLocation(ProgramID, "ModelViewProjectionMatrix"), 1, GL_FALSE, &ModelViewProjectionMatrix(0, 0));

	glDrawArrays(GL_POINTS, 0, sizeof(cy::Vec3f) * Object.NV());
	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	ObjectName = argc >= 2
					? argv[1]
					: "assets/teapot.obj";

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitContextVersion(4, 6);
	glutInitWindowSize(Width, Height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Transformations");
	glewInit();
	glutDisplayFunc(Render);
	glutKeyboardFunc(HandleKeys);
	glutSpecialFunc(HandleSpecialKeys);
	glutMouseFunc(HandleMouseButton);
	glutMotionFunc(HandleMouseMove);
	glutIdleFunc(Idle);
	Initialize();
	glutMainLoop();
	return 0;
}