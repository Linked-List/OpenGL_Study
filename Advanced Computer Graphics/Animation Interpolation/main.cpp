#include <iostream>
#include <fstream>
#include <string>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#define TESS 0.005f

GLsizei len;
char lg[500];
GLuint VBO[2];
GLuint uLoc;
GLuint pLoc[5];
GLuint mLoc;
GLfloat u = 0.0f;
int drawMode = 0;
int pointNo = 1;

float direction[] = { 1.0f, 0.0f, 0.0f };

GLfloat position[] = {
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

GLfloat color[] = {
	1.0, 0.0, 0.0,
	1.0, 0.0, 0.0,
	1.0, 0.0, 0.0,
	1.0, 0.0, 0.0,
	1.0, 0.0, 0.0,
	1.0, 0.0, 0.0,

	0.0, 1.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 1.0, 0.0,

	0.0, 0.0, 1.0,
	0.0, 0.0, 1.0,
	0.0, 0.0, 1.0,
	0.0, 0.0, 1.0,
	0.0, 0.0, 1.0,
	0.0, 0.0, 1.0,

	1.0, 1.0, 0.0,
	1.0, 1.0, 0.0,
	1.0, 1.0, 0.0,
	1.0, 1.0, 0.0,
	1.0, 1.0, 0.0,
	1.0, 1.0, 0.0,

	1.0, 0.0, 1.0,
	1.0, 0.0, 1.0,
	1.0, 0.0, 1.0,
	1.0, 0.0, 1.0,
	1.0, 0.0, 1.0,
	1.0, 0.0, 1.0,

	0.0, 1.0, 1.0,
	0.0, 1.0, 1.0,
	0.0, 1.0, 1.0,
	0.0, 1.0, 1.0,
	0.0, 1.0, 1.0,
	0.0, 1.0, 1.0,
};

float controlPoints[] = {
	-1.0f, 1.0f, 0.0f,
	0.2f, -1.3f, -1.0f,
	-0.9f, 0.7f, 0.5f,
	0.6f, 1.2f, 0.7f,
	1.0f, -1.0f, 0.0f,
};
/*
float controlPoints[] = {
	-0.9f, 0.0f, 0.0f,
	0.7f, -1.0f, -1.0f,
	0.9f, 0.0f, 0.0f,
	0.7f, 1.0f, 1.0f,
	-0.9f, 0.0f, 0.0f,
};
/*
float controlPoints[] = {
	-0.9f, 0.0f, 0.0f,
	-0.5f, -1.3f, 0.0f,
	0.6f, 1.2f, 0.0f,
	1.0f, 0.0f, 0.0f
};*/

void renderScene() {
	//draw something here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, 36);
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
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	GLuint p = glCreateProgram();

	GLuint v = loadShader("VertexShader.txt", GL_VERTEX_SHADER);
	GLuint f = loadShader("FragmentShader.txt", GL_FRAGMENT_SHADER);

	glAttachShader(p, v);
	glAttachShader(p, f);

	glGetShaderInfoLog(v, 500, &len, (GLchar*)lg);
	printf("Vertex Shader : %s\n", lg);
	glGetShaderInfoLog(f, 500, &len, (GLchar*)lg);
	printf("Fragment Shader : %s\n", lg);

	glLinkProgram(p);

	return p;
}

void myKeyboard(unsigned char key, int x, int y) {
	if (key >= '1' && key <= '5') {
		pointNo = key-'0';
		printf("Control Point No : %d\n", pointNo);
	}
	else if (key == 'm') {
		drawMode = (drawMode + 1) % 2;
		glUniform1i(mLoc, drawMode);

		u = 0.0f;
		glVertexAttrib1f(uLoc, u);

		if (drawMode == 0)
			printf("Changed Draw Mode to Bezier Interpolation\n");
		else
			printf("Changed Draw Mode to Linear Interpolation\n");
	}
}

void myMouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		GLfloat newX = (double)x / glutGet(GLUT_WINDOW_WIDTH) * 2 - 1;
		GLfloat newY = -((double)y / glutGet(GLUT_WINDOW_HEIGHT) * 2 - 1);
		if (pointNo >= 1 && pointNo <= 5) {
			printf("Change Control Point No. %d from (%.3f,%.3f,%.3f) to (%.3f,%.3f,%.3f)\n",pointNo, controlPoints[(pointNo-1)*3], controlPoints[(pointNo - 1) * 3+1], controlPoints[(pointNo - 1) * 3+2],newX,newY, controlPoints[(pointNo - 1) * 3+2]);
			controlPoints[(pointNo - 1) * 3] = newX;
			controlPoints[(pointNo-1) * 3 + 1] = newY;
			glUniform3f(pLoc[pointNo - 1], controlPoints[(pointNo - 1) * 3], controlPoints[(pointNo - 1) * 3+1], controlPoints[(pointNo - 1) * 3+2]);
			
			u = 0.0f;
			glVertexAttrib1f(uLoc, u);
		}
	}
}

void myTimer(int value) {
	u = u + TESS > 1.0f ? 0.0f : u + TESS;
	glVertexAttrib1f(uLoc, u);
	glutPostRedisplay();
	glutTimerFunc(10, myTimer, 0);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	GLuint programID = init(480, 480);
	glUseProgram(programID);
	glutDisplayFunc(renderScene);
	glutMouseFunc(myMouse);
	glutKeyboardFunc(myKeyboard);

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


	pLoc[0] = glGetUniformLocation(programID, "p0");
	pLoc[1] = glGetUniformLocation(programID, "p1");
	pLoc[2] = glGetUniformLocation(programID, "p2");
	pLoc[3] = glGetUniformLocation(programID, "p3");
	pLoc[4] = glGetUniformLocation(programID, "p4");

	glUniform3f(pLoc[0], controlPoints[0], controlPoints[1], controlPoints[2]);
	glUniform3f(pLoc[1], controlPoints[3], controlPoints[4], controlPoints[5]);
	glUniform3f(pLoc[2], controlPoints[6], controlPoints[7], controlPoints[8]);
	glUniform3f(pLoc[3], controlPoints[9], controlPoints[10], controlPoints[11]);
	glUniform3f(pLoc[4], controlPoints[12], controlPoints[13], controlPoints[14]);

	uLoc = glGetAttribLocation(programID, "u");
	glVertexAttrib1f(uLoc, u);

	mLoc = glGetUniformLocation(programID, "mode");
	glUniform1i(mLoc, drawMode);

	glVertexAttrib3f(glGetAttribLocation(programID, "direction"), direction[0], direction[1], direction[2]);
	
	glutTimerFunc(50, myTimer, 0);
	glutMainLoop();
	return 1;
}