#include <iostream>
#include <fstream>
#include <string>
#include <GL/glew.h>
#include <GL/glut.h>

int clickCount = 0;
GLuint VBO[2];

float position[] = {
	-0.3f, 0.3f, 0.0f,
	0.2f, -0.7f, 0.0f,
	-0.8f, -0.7f, 0.0f,

	0.7f, 0.7f, 0.0f,
	0.7f, -0.7f, 0.0f,
	-0.7f, 0.7f, 0.0f,

	0.2f, 0.5f, 0.0f,
	0.2f, 0.1f, 0.0f,
	-0.2f, 0.5f, 0.0f,

	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
};

float color[] = {
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,

	1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,

	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,

	0.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f
};

void renderScene() {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	//draw something here
	if (clickCount < 3)
		glDrawArrays(GL_TRIANGLES, 0, 9);
	else
		glDrawArrays(GL_TRIANGLES, 0, 12);

	glutSwapBuffers();
}

void init(int width, int heigth) {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(width, heigth);
	glutCreateWindow("Simple OpenGL Window");
	glewInit();
}

GLuint loadShaders(std::string vsName, std::string fsName) {
	GLuint v = glCreateShader(GL_VERTEX_SHADER);
	GLuint f = glCreateShader(GL_FRAGMENT_SHADER);

	std::ifstream vs(vsName);
	std::ifstream fs(fsName);

	std::string vv;
	while (vs) {
		std::string temp;
		std::getline(vs, temp);
		vv += temp + "\n";
	}
	std::string ff;
	while (fs) {
		std::string temp;
		std::getline(fs, temp);
		ff += temp + "\n";
	}

	const char* t = vv.c_str();
	glShaderSource(v, 1, &t, NULL);
	glCompileShader(v);
	t = ff.c_str();
	glShaderSource(f, 1, &t, NULL);
	glCompileShader(f);

	vs.close();
	fs.close();

	GLuint p = glCreateProgram();

	glAttachShader(p,v);
	glAttachShader(p,f);

	glLinkProgram(p);

	return p;
}

void myMouse(int button, int state, int x, int y) {
	if (clickCount < 3 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		float newX = (float)((double)x / glutGet(GLUT_WINDOW_WIDTH) * 2 - 1);
		float newY = (float)(-((double)y / glutGet(GLUT_WINDOW_HEIGHT) * 2 - 1));
		position[9 * 3 + 3 * clickCount] = newX;
		std::cout << "x : " << position[9 * 3 + 3 * clickCount] << std::endl;
		position[9 * 3 + 3 * clickCount + 1] = newY;
		std::cout << "y : " << position[9 * 3 + 3 * clickCount] << std::endl;
		clickCount++;

		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 12 * 3, position, GL_DYNAMIC_DRAW);
	}
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	init(480, 480);
	GLuint programID = loadShaders("VertexShader.txt", "FragmentShader.txt");
	glUseProgram(programID);
	glutMouseFunc(myMouse);
	glutDisplayFunc(renderScene);

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(2, VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 12 * 3, position, GL_DYNAMIC_DRAW);

	GLuint attrib = glGetAttribLocation(programID, "vtxPosition");
	glVertexAttribPointer(attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(attrib);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 12 * 3, color, GL_DYNAMIC_DRAW);

	attrib = glGetAttribLocation(programID, "inColor");
	glVertexAttribPointer(attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(attrib);

	glutMainLoop();
	return 1;
}