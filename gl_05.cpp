#define GLEW_STATIC
#include <GL/glew.h>
#include "shprogram.h"
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include <iostream>
using namespace std;
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>

#define MOVE_SPEED 0.3f
#define RADIUS 3.0f

const GLuint WIDTH = 800, HEIGHT = 600;
GLfloat posX = 0.0f, posY = 1.0f, posZ = -4.0f, lookX = posX, lookY = posY, lookZ = posZ+3.0f, lookAngleH = 90.0f, lookAngleV = 0.0f;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	//cout << "key: " << key << endl;

	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		GLfloat trans;
		bool move = false;
		
		switch (key)
		{		
		case GLFW_KEY_W:
			trans = MOVE_SPEED * cos(glm::radians(lookAngleH));
			posX += trans;
			lookX += trans;
			trans = MOVE_SPEED * sin(glm::radians(lookAngleH));
			posZ += trans;
			lookZ += trans;
			move = true;
			break;

		case GLFW_KEY_S:
			trans = MOVE_SPEED * cos(glm::radians(lookAngleH));
			posX -= trans;
			lookX -= trans;
			trans = MOVE_SPEED * sin(glm::radians(lookAngleH));
			posZ -= trans;
			lookZ -= trans;
			move = true;
			break;

		case GLFW_KEY_A:
			trans = MOVE_SPEED * cos(glm::radians(lookAngleH - 90.0f));
			posX += trans;
			lookX += trans;
			trans = MOVE_SPEED * sin(glm::radians(lookAngleH - 90.0f));
			posZ += trans;
			lookZ += trans;
			move = true;
			break;

		case GLFW_KEY_D:
			trans = MOVE_SPEED * cos(glm::radians(lookAngleH + 90.0f));
			posX += trans;
			lookX += trans;
			trans = MOVE_SPEED * sin(glm::radians(lookAngleH + 90.0f));
			posZ += trans;
			lookZ += trans;
			move = true;
			break;

		case GLFW_KEY_R:
			posY += MOVE_SPEED;
			lookY += MOVE_SPEED;
			move = true;
			break;

		case GLFW_KEY_F:
			posY -= MOVE_SPEED;
			lookY -= MOVE_SPEED;
			move = true;
			break;

		case GLFW_KEY_RIGHT:
			lookAngleH = (lookAngleH > 360.0f ? (lookAngleH - 355.0f) : (lookAngleH + 5.0f));
			trans = (RADIUS * cos(glm::radians(lookAngleV)));
			lookX = posX + trans * cos(glm::radians(lookAngleH));
			lookZ = posZ + trans * sin(glm::radians(lookAngleH));
			break;

		case GLFW_KEY_LEFT:
			lookAngleH = (lookAngleH < -360.0f ? (lookAngleH + 355.0f) : (lookAngleH - 5.0f));
			trans = (RADIUS * cos(glm::radians(lookAngleV)));
			lookX = posX + trans * cos(glm::radians(lookAngleH));
			lookZ = posZ + trans * sin(glm::radians(lookAngleH));
			break;

		case GLFW_KEY_UP:
			if (lookAngleV < 90.0f)
			{
				lookAngleV += 5.0f;
				trans = (RADIUS * cos(glm::radians(lookAngleV)));
				lookY = posY + RADIUS * sin(glm::radians(lookAngleV));
				lookX = posX + trans * cos(glm::radians(lookAngleH));
				lookZ = posZ + trans * sin(glm::radians(lookAngleH));
			}
			break;

		case GLFW_KEY_DOWN:
			if (lookAngleV > -90.0f)
			{
				lookAngleV -= 5.0f;
				trans = (RADIUS * cos(glm::radians(lookAngleV)));
				lookY = posY + RADIUS * sin(glm::radians(lookAngleV));
				lookX = posX + trans * cos(glm::radians(lookAngleH));
				lookZ = posZ + trans * sin(glm::radians(lookAngleH));
			}
			break;

		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;

		default:
			break;
		}
		if (move)
			cout << "x: " << posX << " y: " << posY << " z: " << posZ << endl;
	}
}

GLuint LoadMipmapTexture(GLuint texId, const char* fname)
{
	int width, height;
	unsigned char* image = SOIL_load_image(fname, &width, &height, 0, SOIL_LOAD_RGB);
	if (image == nullptr)
		throw exception("Failed to load texture file");

	GLuint texture;
	glGenTextures(1, &texture);

	glActiveTexture(texId);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
}

ostream& operator<<(ostream& os, const glm::mat4& mx)
{
	for (int row = 0; row < 4; ++row)
	{
		for (int col = 0; col < 4; ++col)
			cout << mx[row][col] << ' ';
		cout << endl;
	}
	return os;
}

int main()
{
	{
		glm::mat4 trans;
		cout << trans << endl;
		trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0)); //drugi parametr - kat w radianach, trzeci - os obrotu przechodzacej przez srodek uklady wsp. (tutaj - obrot o 90 stopnia wokol osi Z)
		cout << trans << endl;
	}
	if (glfwInit() != GL_TRUE)
	{
		cout << "GLFW initialization failed" << endl;
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	try
	{
		GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "GKOM - OpenGL 05", nullptr, nullptr);
		if (window == nullptr)
			throw exception("GLFW window not created");
		glfwMakeContextCurrent(window);
		glfwSetKeyCallback(window, key_callback);

		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
			throw exception("GLEW Initialization failed");

		glViewport(0, 0, WIDTH, HEIGHT);

		glEnable(GL_DEPTH_TEST); //wlacza Z-bufor, ktory sprawdza ktore piksele rysujemy najpierw, zeby tekstury sie nie nakladaly

		// Let's check what are maximum parameters counts
		GLint nrAttributes;
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
		cout << "Max vertex attributes allowed: " << nrAttributes << std::endl;
		glGetIntegerv(GL_MAX_TEXTURE_COORDS, &nrAttributes);
		cout << "Max texture coords allowed: " << nrAttributes << std::endl;

		// Build, compile and link shader program
		ShaderProgram theProgram("gl_05.vert", "gl_05.frag");

		// Set up vertex data
		/*GLfloat vertices[] = { //powinny byc tu tez trzy kolejne wartosci do kazdego wierzcholka - jego normalna, dzieki ktorej mozna okreslic oswietlenie
			// coordinates			// color			// texture		// normals
			0.25f, 0.5f, -0.5f,		1.0f, 0.0f, 0.0f,	1.0f, 0.0f,		0.0f, 0.0f, -1.0f,	//0
			-0.75f, 0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	0.0f, 0.0f,		0.0f, 0.0f, -1.0f,	//1
			- 0.25f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f,	0.0f, 1.0f,		0.0f, 0.0f, -1.0f,	//2
			0.75f, -0.5f, -0.5f,	1.0f, 0.0f, 1.0f,	1.0f, 1.0f,		0.0f, 0.0f, -1.0f,	//3

			-0.25f, -0.5f, 0.5f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f,		0.0f, 0.0f, 1.0f,	//4
			-0.75f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f,		0.0f, 0.0f, 1.0f,	//5
			0.25f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f,		0.0f, 0.0f, 1.0f,	//6
			0.75f, -0.5f, 0.5f,		1.0f, 0.0f, 1.0f,	1.0f, 0.0f,		0.0f, 0.0f, 1.0f,	//7

			-0.25f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,	//8
			-0.25f, -0.5f, 0.5f,	0.0f, 0.0f, 1.0f,	0.0f, 1.0f,		0.0f, -1.0f, 0.0f,	//9
			0.75f, -0.5f, 0.5f,		1.0f, 0.0f, 1.0f,	1.0f, 1.0f,		0.0f, -1.0f, 0.0f,	//10
			0.75f, -0.5f, -0.5f,	1.0f, 0.0f, 1.0f,	1.0f, 0.0f,		0.0f, -1.0f, 0.0f,	//11

			-0.75f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f,		0.0f, 1.0f, 0.0f,	//12
			-0.75f, 0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	0.0f, 1.0f,		0.0f, 1.0f, 0.0f,	//13
			0.25f, 0.5f, -0.5f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f,		0.0f, 1.0f, 0.0f,	//14
			0.25f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	1.0f, 0.0f,		0.0f, 1.0f, 0.0f,	//15

			-0.25f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f,		-0.8944f, -0.4472f, 0.0f,	//16
			-0.75f, 0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	0.0f, 1.0f,		-0.8944f, -0.4472f, 0.0f,	//17
			-0.25f, -0.5f, 0.5f,	0.0f, 0.0f, 1.0f,	1.0f, 0.0f,		-0.8944f, -0.4472f, 0.0f,	//18
			-0.75f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 1.0f,		-0.8944f, -0.4472f, 0.0f,	//19

			0.75f, -0.5f, -0.5f,	1.0f, 0.0f, 1.0f,	1.0f, 0.0f,		0.8944f, 0.4472f, 0.0f,	//20
			0.75f, -0.5f, 0.5f,		1.0f, 0.0f, 1.0f,	0.0f, 0.0f,		0.8944f, 0.4472f, 0.0f,	//21
			0.25f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	0.0f, 1.0f,		0.8944f, 0.4472f, 0.0f,	//22
			0.25f, 0.5f, -0.5f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f,		0.8944f, 0.4472f, 0.0f	//23
		};

		GLuint indices[] = {
			0, 1, 2,
			0, 2, 3,
			4, 5, 6,
			4, 6, 7,
			8, 9, 10,
			8, 10, 11,
			12, 13, 14,
			12, 14, 15,
			16, 17, 18,
			18, 17, 19,
			20, 21, 22,
			20, 22, 23
		};*/

		GLfloat vertices[179*11] = {
			// coordinates			// color			// texture		// normals
			2.0f, 0.2f, 1.0f,		0.4f, 0.4f, 0.4f,	1.0f, 0.0f,		0.0f, 1.0f, 0.0f,	//0
			2.0f, 0.2f, -1.0f,		0.4f, 0.4f, 0.4f,	1.0f, 0.0f,		0.0f, 1.0f, 0.0f,	//1
			-2.0f, 0.2f, -1.0f,		0.4f, 0.4f, 0.4f,	1.0f, 0.0f,		0.0f, 1.0f, 0.0f,	//2
			-2.0f, 0.2f, 1.0f,		0.4f, 0.4f, 0.4f,	1.0f, 0.0f,		0.0f, 1.0f, 0.0f,	//3

			2.0f, 0.2f, -1.0f,		0.4f, 0.4f, 0.4f,	1.0f, 0.0f,		0.0f, 0.0f, -1.0f,	//4
			2.0f, 0.0f, -1.0f,		0.4f, 0.4f, 0.4f,	1.0f, 0.0f,		0.0f, 0.0f, -1.0f,	//5
			-2.0f, 0.0f, -1.0f,		0.4f, 0.4f, 0.4f,	1.0f, 0.0f,		0.0f, 0.0f, -1.0f,	//6
			-2.0f, 0.2f, -1.0f,		0.4f, 0.4f, 0.4f,	1.0f, 0.0f,		0.0f, 0.0f, -1.0f,	//7

			-2.0f, 0.2f, -1.0f,		0.4f, 0.4f, 0.4f,	1.0f, 0.0f,		-1.0f, 0.0f, 0.0f,	//8
			-2.0f, 0.0f, -1.0f,		0.4f, 0.4f, 0.4f,	1.0f, 0.0f,		-1.0f, 0.0f, 0.0f,	//9
			-2.0f, 0.0f, 1.0f,		0.4f, 0.4f, 0.4f,	1.0f, 0.0f,		-1.0f, 0.0f, 0.0f,	//10
			-2.0f, 0.2f, 1.0f,		0.4f, 0.4f, 0.4f,	1.0f, 0.0f,		-1.0f, 0.0f, 0.0f,	//11

			-2.0f, 0.2f, 1.0f,		0.4f, 0.4f, 0.4f,	1.0f, 0.0f,		0.0f, 0.0f, 1.0f,	//12
			-2.0f, 0.0f, 1.0f,		0.4f, 0.4f, 0.4f,	1.0f, 0.0f,		0.0f, 0.0f, 1.0f,	//13
			2.0f, 0.0f, 1.0f,		0.4f, 0.4f, 0.4f,	1.0f, 0.0f,		0.0f, 0.0f, 1.0f,	//14
			2.0f, 0.2f, 1.0f,		0.4f, 0.4f, 0.4f,	1.0f, 0.0f,		0.0f, 0.0f, 1.0f,	//15

			2.0f, 0.2f, 1.0f,		0.4f, 0.4f, 0.4f,	1.0f, 0.0f,		1.0f, 0.0f, 0.0f,	//16
			2.0f, 0.0f, 1.0f,		0.4f, 0.4f, 0.4f,	1.0f, 0.0f,		1.0f, 0.0f, 0.0f,	//17
			2.0f, 0.0f, -1.0f,		0.4f, 0.4f, 0.4f,	1.0f, 0.0f,		1.0f, 0.0f, 0.0f,	//18
			2.0f, 0.2f, -1.0f,		0.4f, 0.4f, 0.4f,	1.0f, 0.0f,		1.0f, 0.0f, 0.0f,	//19

			-1.2f, 1.0f, -0.8f,		0.05f, 0.9f, 0.05f,	1.0f, 0.0f,		0.0f, 0.0f, -1.0f,	//20
			-0.8f, 0.2f, -0.8f,		0.05f, 0.9f, 0.05f,	1.0f, 0.0f,		0.0f, 0.0f, -1.0f,	//21
			-1.6f, 0.2f, -0.8f,		0.05f, 0.9f, 0.05f,	1.0f, 0.0f,		0.0f, 0.0f, -1.0f,	//22

			-1.2f, 1.0f, -0.4f,		0.05f, 0.9f, 0.05f,	1.0f, 0.0f,		0.0f, 0.0f, 1.0f,	//23
			-0.8f, 0.2f, -0.4f,		0.05f, 0.9f, 0.05f,	1.0f, 0.0f,		0.0f, 0.0f, 1.0f,	//24
			-1.6f, 0.2f, -0.4f,		0.05f, 0.9f, 0.05f,	1.0f, 0.0f,		0.0f, 0.0f, 1.0f,	//25

			-1.2f, 1.0f, 0.4f,		0.05f, 0.9f, 0.05f,	1.0f, 0.0f,		0.0f, 0.0f, -1.0f,	//26
			-0.8f, 0.2f, 0.4f,		0.05f, 0.9f, 0.05f,	1.0f, 0.0f,		0.0f, 0.0f, -1.0f,	//27
			-1.6f, 0.2f, 0.4f,		0.05f, 0.9f, 0.05f,	1.0f, 0.0f,		0.0f, 0.0f, -1.0f,	//28

			-1.2f, 1.0f, 0.8f,		0.05f, 0.9f, 0.05f,	1.0f, 0.0f,		0.0f, 0.0f, 1.0f,	//29
			-0.8f, 0.2f, 0.8f,		0.05f, 0.9f, 0.05f,	1.0f, 0.0f,		0.0f, 0.0f, 1.0f,	//30
			-1.6f, 0.2f, 0.8f,		0.05f, 0.9f, 0.05f,	1.0f, 0.0f,		0.0f, 0.0f, 1.0f,	//31

			1.696f, 1.24f, 0.296f,	0.3f, 0.3f, 0.3f,	1.0f, 0.0f,		0.0f, 1.0f, 0.0f,	//32
			1.696f, 1.24f, -0.296f,	0.3f, 0.3f, 0.3f,	1.0f, 0.0f,		0.0f, 1.0f, 0.0f,	//33
			1.104f, 1.24f, -0.296f,	0.3f, 0.3f, 0.3f,	1.0f, 0.0f,		0.0f, 1.0f, 0.0f,	//34
			1.104f, 1.24f, 0.296f,	0.3f, 0.3f, 0.3f,	1.0f, 0.0f,		0.0f, 1.0f, 0.0f,	//35

			1.696f, 0.36f, 0.296f,	0.3f, 0.3f, 0.3f,	1.0f, 0.0f,		0.0f, -1.0f, 0.0f,	//36
			1.696f, 0.36f, -0.296f,	0.3f, 0.3f, 0.3f,	1.0f, 0.0f,		0.0f, -1.0f, 0.0f,	//37
			1.104f, 0.36f, -0.296f,	0.3f, 0.3f, 0.3f,	1.0f, 0.0f,		0.0f, -1.0f, 0.0f,	//38
			1.104f, 0.36f, 0.296f,	0.3f, 0.3f, 0.3f,	1.0f, 0.0f,		0.0f, -1.0f, 0.0f,	//39

			1.696f, 1.24f, -0.296f,	0.3f, 0.3f, 0.3f,	1.0f, 0.0f,		0.0f, 0.0f, -1.0f,	//40
			1.696f, 0.36f, -0.296f,	0.3f, 0.3f, 0.3f,	1.0f, 0.0f,		0.0f, 0.0f, -1.0f,	//41
			1.104f, 0.36f, -0.296f,	0.3f, 0.3f, 0.3f,	1.0f, 0.0f,		0.0f, 0.0f, -1.0f,	//42
			1.104f, 1.24f, -0.296f,	0.3f, 0.3f, 0.3f,	1.0f, 0.0f,		0.0f, 0.0f, -1.0f,	//43

			1.104f, 1.24f, -0.296f,	0.3f, 0.3f, 0.3f,	1.0f, 0.0f,		-1.0f, 0.0f, 0.0f,	//44
			1.104f, 0.36f, -0.296f,	0.3f, 0.3f, 0.3f,	1.0f, 0.0f,		-1.0f, 0.0f, 0.0f,	//45
			1.104f, 0.36f, 0.296f,	0.3f, 0.3f, 0.3f,	1.0f, 0.0f,		-1.0f, 0.0f, 0.0f,	//46
			1.104f, 1.24f, 0.296f,	0.3f, 0.3f, 0.3f,	1.0f, 0.0f,		-1.0f, 0.0f, 0.0f,	//47

			1.104f, 1.24f, 0.296f,	0.3f, 0.3f, 0.3f,	1.0f, 0.0f,		0.0f, 0.0f, 1.0f,	//48
			1.104f, 0.36f, 0.296f,	0.3f, 0.3f, 0.3f,	1.0f, 0.0f,		0.0f, 0.0f, 1.0f,	//49
			1.696f, 0.36f, 0.296f,	0.3f, 0.3f, 0.3f,	1.0f, 0.0f,		0.0f, 0.0f, 1.0f,	//50
			1.696f, 1.24f, 0.296f,	0.3f, 0.3f, 0.3f,	1.0f, 0.0f,		0.0f, 0.0f, 1.0f,	//51

			1.696f, 1.24f, 0.296f,	0.3f, 0.3f, 0.3f,	1.0f, 0.0f,		1.0f, 0.0f, 0.0f,	//52
			1.696f, 0.36f, 0.296f,	0.3f, 0.3f, 0.3f,	1.0f, 0.0f,		1.0f, 0.0f, 0.0f,	//53
			1.696f, 0.36f, -0.296f,	0.3f, 0.3f, 0.3f,	1.0f, 0.0f,		1.0f, 0.0f, 0.0f,	//54
			1.696f, 1.24f, -0.296f,	0.3f, 0.3f, 0.3f,	1.0f, 0.0f,		1.0f, 0.0f, 0.0f,	//55

			1.743f, 0.305f, 0.93f,	1.0f, 0.5f, 0.0f,	1.0f, 0.0f,		1.0f, 0.0f, 0.0f,	//56
			1.743f, 0.2f, 0.93f,	1.0f, 0.5f, 0.0f,	1.0f, 0.0f,		1.0f, 0.0f, 0.0f,	//57
			1.743f, 0.2f, -0.344f,	1.0f, 0.5f, 0.0f,	1.0f, 0.0f,		1.0f, 0.0f, 0.0f,	//58
			1.743f, 0.305f, -0.344f,1.0f, 0.5f, 0.0f,	1.0f, 0.0f,		1.0f, 0.0f, 0.0f,	//59

			1.07f, 0.305f, -0.344f,	1.0f, 0.5f, 0.0f,	1.0f, 0.0f,		-1.0f, 0.0f, 0.0f,	//60
			1.07f, 0.2f, -0.344f,	1.0f, 0.5f, 0.0f,	1.0f, 0.0f,		-1.0f, 0.0f, 0.0f,	//61
			1.07f, 0.2f, 0.93f,		1.0f, 0.5f, 0.0f,	1.0f, 0.0f,		-1.0f, 0.0f, 0.0f,	//62
			1.07f, 0.305f, 0.93f,	1.0f, 0.5f, 0.0f,	1.0f, 0.0f,		-1.0f, 0.0f, 0.0f,	//63

			1.4f, 0.369f, 0.6f,		0.4f, 0.0f, 1.0f,	1.0f, 0.0f,		0.0f, 1.0f, 0.0f	//64
		};

		float angle = 0.0f;
		for (int i = 65 * 11; i < 89 * 11; i += 11)
		{
			vertices[i] = 1.3f - 0.237f * sin(glm::radians(angle));	//x
			if (i % 2 == 1)
				vertices[i + 1] = 0.369f;	//y
			else
				vertices[i + 1] = 0.305f;	//y
			vertices[i + 2] = 0.6f + 0.237f * cos(glm::radians(angle)); //z
			vertices[i + 3] = 0.4f;	//r
			vertices[i + 4] = 0.0f;	//g
			vertices[i + 5] = 1.0f;	//b
			vertices[i + 6] = 1.0f;	//tx
			vertices[i + 7] = 0.0f;	//ty
			vertices[i + 8] = sin(glm::radians(angle));	//nx
			vertices[i + 9] = 0.0f;	//ny
			vertices[i + 10] = cos(glm::radians(angle));//nz
			
			angle += 20.0f;
		}

		GLuint indices[] = {
			0, 1, 2,
			0, 2, 3,	//Podstawa-top
			4, 5, 6,
			4, 6, 7,	//Podstawa-right
			8, 9, 10,
			8, 10, 11,	//Podstawa-back
			12, 13, 14,
			12, 14, 15,	//Podstawa-left
			16, 17, 18,
			16, 18, 19, //Podstawa-front
			20, 21, 22,
			20, 22, 25,
			20, 25, 23,
			20, 23, 24,
			20, 24, 21,	//Trzymak-right
			26, 27, 28,
			26, 28, 31,
			26, 31, 29,
			26, 29, 30,
			26, 30, 27,
			29, 31, 30,	//Trzymak-left
			32, 33, 34,
			32, 34, 35,
			39, 38, 37,
			39, 37, 36,
			40, 41, 42,
			40, 42, 43,
			44, 45, 46,
			44, 46, 47,
			48, 49, 50,
			48, 50, 51,
			52, 53, 54,
			52, 54, 55,	//Mlot
			56, 57, 58,
			56, 58, 59,
			60, 61, 62,
			60, 62, 63,
			59, 58, 61,
			59, 61, 60,
			63, 62, 57,
			63, 57, 56,
			63, 56, 59,
			63, 59, 60	//Odlewnia
		};

		GLuint VBO, EBO, VAO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// vertex geometry data
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		// vertex color data
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		// vertex texture coordinates
		// TEKSTURY glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		// TEKSTURY glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

		glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

							  // Set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// prepare textures
		GLuint texture0 = LoadMipmapTexture(GL_TEXTURE0, "iipw.png");
		GLuint texture1 = LoadMipmapTexture(GL_TEXTURE1, "weiti.png");

		// main event loop
		while (!glfwWindowShouldClose(window))
		{
			// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
			glfwPollEvents();

			// Clear the colorbuffer
			glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //czyszczenie bufora koloru i bufora glebokosci

			// Bind Textures using texture units
			/* TEKSTURY
			glActiveTexture(GL_TEXTURE0);
			
			glBindTexture(GL_TEXTURE_2D, texture0);
			glUniform1i(glGetUniformLocation(theProgram.get_programID(), "Texture0"), 0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texture1);
			glUniform1i(glGetUniformLocation(theProgram.get_programID(), "Texture1"), 1);
			*/


			glm::mat4 trans;
			static GLfloat rot_angle = 0.0f;
			trans = glm::rotate(trans, -glm::radians(rot_angle), glm::vec3(1.0, 0.0, 0.0)); //obrot wokol osi X
			rot_angle += 0.1f; //predkosc obrotu
			if (rot_angle >= 360.0f)
				rot_angle -= 360.0f;
			GLuint transformLoc = glGetUniformLocation(theProgram.get_programID(), "transform");
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));




			//----------------
			//glm::mat4 camRot;
			//camRot = glm::rotate(camRot, glm::radians(rot_angle), glm::vec3(0.0f, 1.0f, 0.0f));
			//glm::vec3 cameraPos = glm::vec3(camRot * glm::vec4(0.0f, 0.0f, -3.0f, 1.0f));
			glm::vec3 cameraPos = glm::vec3(posX, posY, posZ);

			glUniform3fv(glGetUniformLocation(theProgram.get_programID(), "viewPos"), 1, glm::value_ptr(cameraPos)); //chyba niepotrzebne
			//----------------

			glm::mat4 view;
			glm::mat4 projection;

			//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -10.0f)); //to zamiast glm::lookAt i linijek miedzy kreskami
			//view = glm::lookAt(cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			view = glm::lookAt(cameraPos, glm::vec3(lookX, lookY, lookZ), glm::vec3(0.0f, 1.0f, 0.0f));
			projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

			GLint viewLoc = glGetUniformLocation(theProgram.get_programID(), "view");
			GLint projectionLoc = glGetUniformLocation(theProgram.get_programID(), "projection");
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
			



			// Draw our first triangle
			theProgram.Use();

			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, _countof(indices), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			// Swap the screen buffers
			glfwSwapBuffers(window);
		}
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}
	catch (exception ex)
	{
		cout << ex.what() << endl;
	}
	glfwTerminate();

	return 0;
}