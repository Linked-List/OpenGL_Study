#include <iostream>
#include <fstream>
#include <string>
#include <GL/glew.h>
#include <GL/glut.h>

#define TESS 1000.

GLuint VBO[2];
GLsizei len;

char lg[500];

float position[] = {
	-0.9f, 0.5f, 0.0f,
	-0.5f, 1.3f, 0.0f,
	0.6f, -1.2f, 0.0f,
	0.9f, -0.7f, 0.0f
};

float color[] = {
	1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.7f
};

void renderScene() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glPatchParameteri(GL_PATCH_VERTICES, 4);
	glDrawArrays(GL_PATCHES, 0, 4);

	glutSwapBuffers();
}

GLuint loadShader(std::string fileName, GLenum shaderType) {
	GLuint t = glCreateShader(shaderType);

	std::ifstream ts(fileName);

	std::string tt;
	while (ts) {
		std::string temp;
		std::getline(ts, temp);
		tt += temp + "\n";
	}

	const char* ttArr = tt.c_str();
	glShaderSource(t, 1, &ttArr, NULL);
	glCompileShader(t);

	ts.close();

	return t;
}

GLuint init(int width, int heigth) {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(width, heigth);
	glutCreateWindow("Simple OpenGL Window");
	glewInit();

	GLuint p = glCreateProgram();
	
	GLuint v = loadShader("VertexShader.txt", GL_VERTEX_SHADER);
	GLuint tc = loadShader("TessellationControlShader.txt", GL_TESS_CONTROL_SHADER);
	GLuint te = loadShader("TessellationEvaluationShader.txt", GL_TESS_EVALUATION_SHADER);
	GLuint f = loadShader("FragmentShader.txt", GL_FRAGMENT_SHADER);

	glAttachShader(p, v);
	glAttachShader(p, tc);
	glAttachShader(p, te);
	glAttachShader(p, f);

	glGetShaderInfoLog(v, 500, &len, (GLchar*)lg);
	printf("Vertex Shader : %s\n", lg);
	glGetShaderInfoLog(tc, 500, &len, (GLchar*)lg);
	printf("Tessellation Control Shader : %s\n", lg);
	glGetShaderInfoLog(te, 500, &len, (GLchar*)lg);
	printf("Tessellation Evaluation Shader : %s\n", lg);
	glGetShaderInfoLog(f, 500, &len, (GLchar*)lg);
	printf("Fragment Shader : %s\n", lg);

	glLinkProgram(p);

	return p;
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	GLuint programID = init(480, 480);
	glUseProgram(programID);
	glutDisplayFunc(renderScene);

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(2, VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_DYNAMIC_DRAW);

	GLuint attrib = glGetAttribLocation(programID, "position");
	glVertexAttribPointer(attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(attrib);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_DYNAMIC_DRAW);

	attrib = glGetAttribLocation(programID, "color");
	glVertexAttribPointer(attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(attrib);

	glUniform1f(glGetUniformLocation(programID, "uOuterl"), TESS);

	glutMainLoop();
	return 1;
}