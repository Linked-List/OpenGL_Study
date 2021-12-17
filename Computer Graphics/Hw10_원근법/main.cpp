#include <stdlib.h>
#include <string>
#include <fstream>
#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/wglew.h>
#include <glm/geometric.hpp>

using namespace std;

int drawMode = 0;
GLuint programID;
GLint vtxPosition;
GLfloat vertices[2][3] = {
		{0.0,0.0,0.0},
		{0.0, 0.0,0.0},
};


void renderScene() {
	glClear(GL_COLOR_BUFFER_BIT);
	//draw something here
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	switch (drawMode) {
	case 0:
		break;
	case 1:
		glDrawArrays(GL_POINTS, 0, 1);
		break;
	case 2:
		glDrawArrays(GL_LINES, 0, 2);
	}
	
	glutSwapBuffers();
}

void myMouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		double newX = (double)x / glutGet(GLUT_WINDOW_WIDTH) * 2 - 1;
		double newY = -((double)y / glutGet(GLUT_WINDOW_HEIGHT) * 2 - 1);
		if (drawMode == 0) {
			vertices[0][0] = newX;
			vertices[0][1] = newY;
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
			drawMode++;
		}
		else if (drawMode == 1) {
			vertices[1][0] = newX;
			vertices[1][1] = newY;
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
			drawMode++;
		}
	}
}

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path)
{
	//create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	GLint Result = GL_FALSE;
	int InfoLogLength;
	//Read the vertex shader code from the file
	string VertexShaderCode;
	ifstream VertexShaderStream(vertex_file_path, ios::in);
	if (VertexShaderStream.is_open())
	{
		string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	//Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);
	//Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	vector<char> VertexShaderErrorMessage(InfoLogLength);
	glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, VertexShaderErrorMessage.data());
	fprintf(stdout, "%s\n", VertexShaderErrorMessage.data());
	//Read the fragment shader code from the file
	string FragmentShaderCode;
	ifstream FragmentShaderStream(fragment_file_path, ios::in);
	if (FragmentShaderStream.is_open())
	{
		string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}
	//Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);
	//Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	vector<char> FragmentShaderErrorMessage(InfoLogLength);
	glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, FragmentShaderErrorMessage.data());
	fprintf(stdout, "%s\n", FragmentShaderErrorMessage.data());
	//Link the program
	fprintf(stdout, "Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);
	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	vector<char> ProgramErrorMessage(glm::max(InfoLogLength, int(1)));
	glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, ProgramErrorMessage.data());
	fprintf(stdout, "%s\n", ProgramErrorMessage.data());
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);
	return ProgramID;
}

void init() {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(480, 480);
	glutCreateWindow("Simple OpenGL Window");

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glewInit();
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);


	programID = LoadShaders("VertexShader.txt", "FragmentShader.txt");
	glUseProgram(programID);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	init();

	vtxPosition = glGetAttribLocation(programID, "vtxPosition");

	GLuint Buffers[1];

	glGenBuffers(1, Buffers);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(vtxPosition, 3, GL_FLOAT, GL_FALSE, 0, ((void*)(0)));
	glEnableVertexAttribArray(vtxPosition);

	glutMouseFunc(myMouse);
	glutDisplayFunc(renderScene);

	glutMainLoop();
	return 1;
}

