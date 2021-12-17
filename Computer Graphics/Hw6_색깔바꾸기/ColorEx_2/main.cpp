#include <stdlib.h>
#include <string>
#include <fstream>
#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/wglew.h>
#include <glm/geometric.hpp>

using namespace std;

int c1 = 0;
int c2 = 0;

GLuint programID;
GLint colorPosition;
GLint vtxPosition;

GLfloat pointData[4][4] = {
	{-0.5,0.5,0.0,1.0},
	{1.0,0.0,0.0,1.0},
	{0.5,-0.5,0.0,1.0},
	{1.0,0.0,0.0,1.0}
};



void renderScene() {
	glClear(GL_COLOR_BUFFER_BIT);
	//draw something here
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_LINES, 0, 2);
	
	glutSwapBuffers();
}

void myKeyboard(unsigned char key, int x, int y) {
	if (key == 's') {
		c1 = (c1 + 1) % 3;
		switch (c1) {
		case 0:
			pointData[1][0] = 1.0;
			pointData[1][1] = 0.0;
			pointData[1][2] = 0.0;
			break;
		case 1:
			pointData[1][0] = 0.0;
			pointData[1][1] = 1.0;
			pointData[1][2] = 0.0;
			break;
		case 2:
			pointData[1][0] = 0.0;
			pointData[1][1] = 0.0;
			pointData[1][2] = 1.0;
		}
		glBufferData(GL_ARRAY_BUFFER, sizeof(pointData), pointData, GL_DYNAMIC_DRAW);
		glutPostRedisplay();
	}
	else if (key == 'e') {
		c2 = (c2 + 1) % 3;
		switch (c2) {
		case 0:
			pointData[3][0] = 1.0;
			pointData[3][1] = 0.0;
			pointData[3][2] = 0.0;
			break;
		case 1:
			pointData[3][0] = 0.0;
			pointData[3][1] = 1.0;
			pointData[3][2] = 0.0;
			break;
		case 2:
			pointData[3][0] = 0.0;
			pointData[3][1] = 0.0;
			pointData[3][2] = 1.0;
		}
		glBufferData(GL_ARRAY_BUFFER, sizeof(pointData), pointData, GL_DYNAMIC_DRAW);
		glutPostRedisplay();
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

	vtxPosition = glGetAttribLocation(programID, "vertex");
	colorPosition = glGetAttribLocation(programID, "in_color");

	GLuint Buffers[1];

	glGenBuffers(1, Buffers);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pointData), pointData, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(vtxPosition, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, ((GLvoid*)(0)));
	glEnableVertexAttribArray(vtxPosition);
	glVertexAttribPointer(colorPosition, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (GLvoid*)(sizeof(GLfloat) * 4));
	glEnableVertexAttribArray(colorPosition);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	init();

	glutDisplayFunc(renderScene);
	glutKeyboardFunc(myKeyboard);
	glutMainLoop();
	return 1;
}
