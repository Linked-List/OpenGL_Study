#include <stdlib.h>
#include <string>
#include <fstream>
#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/wglew.h>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

GLuint programID;

GLuint nVAO;
GLuint Buffers[2];

GLfloat cubeVertices[] = {
	0.1f, 0.1f,-0.1f, // triangle 2 : begin
	-0.1f,-0.1f,-0.1f,
	-0.1f, 0.1f,-0.1f, // triangle 2 : end
	0.1f, 0.1f,-0.1f,
	0.1f,-0.1f,-0.1f,
	-0.1f,-0.1f,-0.1f,

	-0.1f,-0.1f,-0.1f, // triangle 1 : begin
	-0.1f,-0.1f, 0.1f,
	-0.1f, 0.1f, 0.1f, // triangle 1 : end
	-0.1f,-0.1f,-0.1f,
	-0.1f, 0.1f, 0.1f,
	-0.1f, 0.1f,-0.1f,

	-0.1f, 0.1f, 0.1f,
	-0.1f,-0.1f, 0.1f,
	0.1f,-0.1f, 0.1f,
	0.1f, 0.1f, 0.1f,
	-0.1f, 0.1f, 0.1f,
	0.1f,-0.1f, 0.1f,

	0.1f, 0.1f, 0.1f,
	0.1f,-0.1f,-0.1f,
	0.1f, 0.1f,-0.1f,
	0.1f,-0.1f,-0.1f,
	0.1f, 0.1f, 0.1f,
	0.1f,-0.1f, 0.1f,

	0.1f,-0.1f, 0.1f,
	-0.1f,-0.1f,-0.1f,
	0.1f,-0.1f,-0.1f,
	0.1f,-0.1f, 0.1f,
	-0.1f,-0.1f, 0.1f,
	-0.1f,-0.1f,-0.1f,

	0.1f, 0.1f, 0.1f,
	0.1f, 0.1f,-0.1f,
	-0.1f, 0.1f,-0.1f,
	0.1f, 0.1f, 0.1f,
	-0.1f, 0.1f,-0.1f,
	-0.1f, 0.1f, 0.1f,
};

GLfloat cubeColors[] = {
	1.0, 0.0, 0.0, 1.0,
	1.0, 0.0, 0.0, 1.0,
	1.0, 0.0, 0.0, 1.0,
	1.0, 0.0, 0.0, 1.0,
	1.0, 0.0, 0.0, 1.0,
	1.0, 0.0, 0.0, 1.0,

	0.0, 1.0, 0.0, 1.0,
	0.0, 1.0, 0.0, 1.0,
	0.0, 1.0, 0.0, 1.0,
	0.0, 1.0, 0.0, 1.0,
	0.0, 1.0, 0.0, 1.0,
	0.0, 1.0, 0.0, 1.0,

	0.0, 0.0, 1.0, 1.0,
	0.0, 0.0, 1.0, 1.0,
	0.0, 0.0, 1.0, 1.0,
	0.0, 0.0, 1.0, 1.0,
	0.0, 0.0, 1.0, 1.0,
	0.0, 0.0, 1.0, 1.0,

	1.0, 1.0, 0.0, 1.0,
	1.0, 1.0, 0.0, 1.0,
	1.0, 1.0, 0.0, 1.0,
	1.0, 1.0, 0.0, 1.0,
	1.0, 1.0, 0.0, 1.0,
	1.0, 1.0, 0.0, 1.0,

	1.0, 0.0, 1.0, 1.0,
	1.0, 0.0, 1.0, 1.0,
	1.0, 0.0, 1.0, 1.0,
	1.0, 0.0, 1.0, 1.0,
	1.0, 0.0, 1.0, 1.0,
	1.0, 0.0, 1.0, 1.0,

	0.0, 1.0, 1.0, 1.0,
	0.0, 1.0, 1.0, 1.0,
	0.0, 1.0, 1.0, 1.0,
	0.0, 1.0, 1.0, 1.0,
	0.0, 1.0, 1.0, 1.0,
	0.0, 1.0, 1.0, 1.0,
};

GLuint matLoc;
GLuint vtxPosition;
GLuint colorPosition;

int moving = -1;

void renderScene() {
	glBindVertexArray(nVAO);
	//draw something here
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	glutSwapBuffers();
}

void rotateCube(GLfloat x, GLfloat y, GLfloat z) {
	glm::vec3 myZ = glm::normalize(glm::vec3(x, y, -z));
	glm::vec3 myY(0, 1, 0);
	glm::vec3 myX = glm::normalize(glm::cross(myY, myZ));
	myY = glm::normalize(glm::cross(myZ, myX));
	glm::mat4 transform(myX[0], myX[1], myX[2], 0.0f, myY[0], myY[1], myY[2], 0.0f, myZ[0], myZ[1], myZ[2], 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

	glUniformMatrix4fv(matLoc, 1, GL_FALSE, glm::value_ptr(transform));
}

void myMouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		GLfloat newX = (double)x / glutGet(GLUT_WINDOW_WIDTH) * 2 - 1;
		GLfloat newY = -((double)y / glutGet(GLUT_WINDOW_HEIGHT) * 2 - 1);
		rotateCube(newX, newY, 1);
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
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);


	programID = LoadShaders("VertexShader.txt", "FragmentShader.txt");
	glUseProgram(programID);

	vtxPosition = glGetAttribLocation(programID, "vertex");
	colorPosition = glGetAttribLocation(programID, "in_color");

	glGenVertexArrays(1, &nVAO);
	glBindVertexArray(nVAO);

	glGenBuffers(2, Buffers);

	// vertex Data
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vtxPosition, 3, GL_FLOAT, GL_FALSE, 0, ((GLvoid*)(0)));
	glEnableVertexAttribArray(vtxPosition);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeColors), cubeColors, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(colorPosition, 4, GL_FLOAT, GL_FALSE, 0, ((GLvoid*)(0)));
	glEnableVertexAttribArray(colorPosition);

	matLoc = glGetUniformLocation(programID, "trans");

	rotateCube(1, 2, 3);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	init();

	glutDisplayFunc(renderScene);
	glutMouseFunc(myMouse);
	glutMainLoop();

	return 1;
}
