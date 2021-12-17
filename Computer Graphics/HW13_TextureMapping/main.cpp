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
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <FreeImage/FreeImage.h>
#include <iostream>

using namespace std;

GLuint programID;

GLuint nVAO;
GLuint Buffers[3];

GLfloat textureCoord[] = {
	0.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 0.0f,

	0.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 0.0f,

	0.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,
	1.0f, 0.0f,

	0.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 0.0f,

	0.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 0.0f,

	0.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 0.0f,
};

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

GLuint inVertex;
GLuint inColor;

GLubyte checkerboard[64][64][3];
GLuint mytex[2];
GLuint mysamp[1];

GLuint isTex;

void renderScene() {
	glBindVertexArray(nVAO);
	//draw something here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, mytex[0]);
	glUniform1i(isTex, 1);
	glDrawArrays(GL_TRIANGLES, 12, 6);
	
	glUniform1i(isTex, 0);
	glDrawArrays(GL_TRIANGLES, 0, 12);
	glDrawArrays(GL_TRIANGLES, 18, 18);
	glutSwapBuffers();
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

GLuint CreateTexture(char const* filename)
{
	// Determine the format of the image.
	// Note: The second paramter ('size') is currently unused, and we should use 0 for it.
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename, 0);

	// Image not found? Abort! Without this section we get a 0 by 0 image with 0 bits-per-pixel but we don't abort, which
	// you might find preferable to dumping the user back to the desktop.
	if (format == -1)
	{
		cout << "Could not find image: " << filename << " - Aborting." << endl;
		exit(-1);
	}

	// Found image, but couldn't determine the file format? Try again...
	if (format == FIF_UNKNOWN)
	{
		cout << "Couldn't determine file format - attempting to get from file extension..." << endl;

		// ...by getting the filetype from the filename extension (i.e. .PNG, .GIF etc.)
		// Note: This is slower and more error-prone that getting it from the file itself,
		// also, we can't use the 'U' (unicode) variant of this method as that's Windows only.
		format = FreeImage_GetFIFFromFilename(filename);

		// Check that the plugin has reading capabilities for this format (if it's FIF_UNKNOWN,
		// for example, then it won't have) - if we can't read the file, then we bail out =(
		if (!FreeImage_FIFSupportsReading(format))
		{
			cout << "Detected image format cannot be read!" << endl;
			exit(-1);
		}
	}

	// If we're here we have a known image format, so load the image into a bitap
	FIBITMAP* bitmap = FreeImage_Load(format, filename);

	// How many bits-per-pixel is the source image?
	int bitsPerPixel = FreeImage_GetBPP(bitmap);

	// Convert our image up to 32 bits (8 bits per channel, Red/Green/Blue/Alpha) -
	// but only if the image is not already 32 bits (i.e. 8 bits per channel).
	// Note: ConvertTo32Bits returns a CLONE of the image data - so if we
	// allocate this back to itself without using our bitmap32 intermediate
	// we will LEAK the original bitmap data, and valgrind will show things like this:
	//
	// LEAK SUMMARY:
	//  definitely lost: 24 bytes in 2 blocks
	//  indirectly lost: 1,024,874 bytes in 14 blocks    <--- Ouch.
	//
	// Using our intermediate and cleaning up the initial bitmap data we get:
	//
	// LEAK SUMMARY:
	//  definitely lost: 16 bytes in 1 blocks
	//  indirectly lost: 176 bytes in 4 blocks
	//
	// All above leaks (192 bytes) are caused by XGetDefault (in /usr/lib/libX11.so.6.3.0) - we have no control over this.
	//
	FIBITMAP* bitmap32;
	if (bitsPerPixel == 32)
	{
		cout << "Source image has " << bitsPerPixel << " bits per pixel. Skipping conversion." << endl;
		bitmap32 = bitmap;
	}
	else
	{
		cout << "Source image has " << bitsPerPixel << " bits per pixel. Converting to 32-bit colour." << endl;
		bitmap32 = FreeImage_ConvertTo32Bits(bitmap);
	}

	// Some basic image info - strip it out if you don't care
	int imageWidth = FreeImage_GetWidth(bitmap32);
	int imageHeight = FreeImage_GetHeight(bitmap32);
	cout << "Image: " << filename << " is size: " << imageWidth << "x" << imageHeight << "." << endl;

	// Get a pointer to the texture data as an array of unsigned bytes.
	// Note: At this point bitmap32 ALWAYS holds a 32-bit colour version of our image - so we get our data from that.
	// Also, we don't need to delete or delete[] this textureData because it's not on the heap (so attempting to do
	// so will cause a crash) - just let it go out of scope and the memory will be returned to the stack.
	GLubyte* textureData = FreeImage_GetBits(bitmap32);

	// Generate a texture ID and bind to it
	GLuint tempTextureID;
	glGenTextures(1, &tempTextureID);
	glBindTexture(GL_TEXTURE_2D, tempTextureID);

	// Construct the texture.
	// Note: The 'Data format' is the format of the image data as provided by the image library. FreeImage decodes images into
	// BGR/BGRA format, but we want to work with it in the more common RGBA format, so we specify the 'Internal format' as such.
	glTexImage2D(GL_TEXTURE_2D,    // Type of texture
		0,                // Mipmap level (0 being the top level i.e. full size)
		GL_RGBA,          // Internal format
		imageWidth,       // Width of the texture
		imageHeight,      // Height of the texture,
		0,                // Border in pixels
		GL_BGRA,          // Data format
		GL_UNSIGNED_BYTE, // Type of texture data
		textureData);     // The image data to use for this texture

						  // Specify our minification and magnification filters

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);


	glBindTexture(GL_TEXTURE_2D, 0);


	// Check for OpenGL texture creation errors
	GLenum glError = glGetError();
	if (glError)
	{
		cout << "There was an error loading the texture: " << filename << endl;

		switch (glError)
		{
		case GL_INVALID_ENUM:
			cout << "Invalid enum." << endl;
			break;

		case GL_INVALID_VALUE:
			cout << "Invalid value." << endl;
			break;

		case GL_INVALID_OPERATION:
			cout << "Invalid operation." << endl;

		default:
			cout << "Unrecognised GLenum." << endl;
			break;
		}

		cout << "See https://www.opengl.org/sdk/docs/man/html/glTexImage2D.xhtml for further details." << endl;
	}

	// Unload the 32-bit colour bitmap
	FreeImage_Unload(bitmap32);

	// If we had to do a conversion to 32-bit colour, then unload the original
	// non-32-bit-colour version of the image data too. Otherwise, bitmap32 and
	// bitmap point at the same data, and that data's already been free'd, so
	// don't attempt to free it again! (or we'll crash).
	if (bitsPerPixel != 32)
	{
		FreeImage_Unload(bitmap);
	}

	// Finally, return the texture ID
	return tempTextureID;
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

	
	inVertex = glGetAttribLocation(programID, "inVertex");
	inColor = glGetAttribLocation(programID, "inColor");
	GLuint texCoord = glGetAttribLocation(programID, "myTexCoord");

	glGenVertexArrays(1, &nVAO);
	glBindVertexArray(nVAO);

	glGenBuffers(3, Buffers);

	// vertex Data
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(inVertex, 3, GL_FLOAT, GL_FALSE, 0, ((GLvoid*)(0)));
	glEnableVertexAttribArray(inVertex);

	// color Data
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeColors), cubeColors, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(inColor, 4, GL_FLOAT, GL_FALSE, 0, ((GLvoid*)(0)));
	glEnableVertexAttribArray(inColor);

	// texCoord
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoord), textureCoord, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(texCoord,2,GL_FLOAT,GL_FALSE,0,0);
	glEnableVertexAttribArray(texCoord);

	// set World, view, proj
	glm::mat4 worldMat = glm::scale(glm::mat4(1.0), glm::vec3(3.0f, 3.0f, 3.0f)); // world
	glm::mat4 viewMat = glm::inverse(glm::translate(glm::mat4(1.0), glm::vec3(0.9f, 0.9f, 3.5f))); // view
	glm::mat4 projMat = glm::perspective(glm::radians(45.0f), 1.0f, 0.001f, 100.0f); // proj	
	glUniformMatrix4fv(glGetUniformLocation(programID, "worldMat"),1,GL_FALSE,glm::value_ptr(worldMat));
	glUniformMatrix4fv(glGetUniformLocation(programID, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
	glUniformMatrix4fv(glGetUniformLocation(programID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));

	
	// Create a checkerboard pattern
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 64; j++) {
			GLubyte c = (((i & 0x8) == 0) ^ ((j & 0x8) == 0)) * 255;
			checkerboard[i][j][0] = c;
			checkerboard[i][j][1] = c;
			checkerboard[i][j][2] = c;
		}
	}

	glGenTextures(2, mytex);
	glBindTexture(GL_TEXTURE_2D, mytex[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 64, 64, 0, GL_RGB, GL_UNSIGNED_BYTE, checkerboard); // Create Texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glUniform1i(glGetUniformLocation(programID, "myTex"), 0);
	glActiveTexture(GL_TEXTURE0); // Active Texture[0]

	mytex[1] = CreateTexture("konkuk.jpg"); // Create Texture from .jpg
	glBindTexture(GL_TEXTURE_2D, mytex[1]);
	glUniform1i(glGetUniformLocation(programID, "myTex2"), 1);
	glActiveTexture(GL_TEXTURE1); // Active Texture[1]

	isTex = glGetUniformLocation(programID, "isTex");
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	init();

	glutDisplayFunc(renderScene);
	glutMainLoop();
	return 1;
}
