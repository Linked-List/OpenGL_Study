#include <iostream>
#include <fstream>
#include <string>
#include <GL/glew.h>
#include <GL/glut.h>

#define TESS 10

GLuint VBO[2];
GLsizei len;

char lg[500];

float position[] = {
	-0.9f, 0.0f, 0.0f,
	-0.5f, -1.3f, 0.0f,
	0.6f, 1.2f, 0.0f,
	1.0f, 0.0f, 0.0f
};

float color[] = {
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f
};

void renderScene() {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glDrawArrays(GL_LINES_ADJACENCY, 0, 4);

	glutSwapBuffers();
}

void init(int width, int heigth) {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(width, heigth);
	glutCreateWindow("Simple OpenGL Window");
	glewInit();
}

GLuint loadShaders(std::string vsName, std::string fsName, std::string gsName) {
	GLuint v = glCreateShader(GL_VERTEX_SHADER);
	GLuint f = glCreateShader(GL_FRAGMENT_SHADER);
	GLuint g = glCreateShader(GL_GEOMETRY_SHADER);

	std::ifstream vs(vsName);
	std::ifstream fs(fsName);
	std::ifstream gs(gsName);

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
	std::string gg;
	while (gs) {
		std::string temp;
		std::getline(gs, temp);
		gg += temp + "\n";
	}

	const char* t = vv.c_str();
	glShaderSource(v, 1, &t, NULL);
	glCompileShader(v);
	t = ff.c_str();
	glShaderSource(f, 1, &t, NULL);
	glCompileShader(f);
	t = gg.c_str();
	glShaderSource(g, 1, &t, NULL);
	glCompileShader(g);

	vs.close();
	fs.close();
	gs.close();

	GLuint p = glCreateProgram();

	glAttachShader(p,v);
	glAttachShader(p, g);
	glAttachShader(p,f);

	glGetShaderInfoLog(v, 500, &len, (GLchar*)lg);
	printf("Vertex Shader : %s\n", lg);
	glGetShaderInfoLog(g, 500, &len, (GLchar*)lg);
	printf("Geometry Shader : %s\n", lg);
	glGetShaderInfoLog(f, 500, &len, (GLchar*)lg);
	printf("Fragment Shader : %s\n", lg);

	glLinkProgram(p);

	return p;
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	init(480, 480);
	GLuint programID = loadShaders("VertexShader.txt", "FragmentShader.txt", "GeometryShader.txt");
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

	glUniform1i(glGetUniformLocation(programID, "tess"), TESS);

	glutMainLoop();
	return 1;
}