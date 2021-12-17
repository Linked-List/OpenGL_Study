#include <iostream>
#include <fstream>
#include <string>
#include <GL/glew.h>
#include <GL/glut.h>
#include <FreeImage/FreeImage.h>

using namespace std;

GLuint VBO[2];
GLsizei len;
GLuint uMode;

char lg[500];

GLuint tex;

float position[] = {
	-1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,

	-1.0f, 1.0f, 0.0f,
	-1.0f, -1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
};

float texCoord[] = {
	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,

	0.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f
};

void renderScene() {
	//draw something here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glDrawArrays(GL_TRIANGLES, 0, 6);
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

GLuint CreateTexture(char const* filename)
{
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename, 0);
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

void myKeyboard(unsigned char key, int x, int y) {
	if (key == '0') {
		glUniform1i(uMode, 0);
	}
	else if (key == '1') {
		glUniform1i(uMode, 1);
	}
	else if (key == '2') {
		glUniform1i(uMode, 2);
	}
	else if (key == '3') {
		glUniform1i(uMode, 3);
	}
	glutPostRedisplay();
}

GLuint init(int width, int heigth) {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(width, heigth);
	glutCreateWindow("Simple OpenGL Window");
	glewInit();

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

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	GLuint programID = init(480, 480);
	glUseProgram(programID);
	glutDisplayFunc(renderScene);
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(texCoord), texCoord, GL_DYNAMIC_DRAW);

	attrib = glGetAttribLocation(programID, "texCoord");
	glVertexAttribPointer(attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(attrib);

	GLuint texture = CreateTexture("testImage.png"); // Create Texture from .png
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(programID, "tex"), 0);
	glActiveTexture(GL_TEXTURE0); // Active Texture[0]

	uMode = glGetUniformLocation(programID, "mode");
	glUniform1i(uMode, 0);

	glutMainLoop();
	return 1;
}
