#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "stb_image/stb_image.h"
#include <cmath> 

Camera myCamera(glm::vec3(0.0f, 0.0f, 3.0f));

float deltaTime = 0.0f;
float currentFrameTime = 0.0f;
float previousFrameTime = 0.0f;

float previousMousePositonX = 400.0f;
float previousMousePositonY = 300.0f;
const double PI = 3.14;
bool firstMouse = true;

void FrameBufferSizeCallback(GLFWwindow* givenWindow, int givenWidth, int givenHeight)
{
	glViewport(0, 0, givenWidth, givenHeight);
}

void ProcessInput(GLFWwindow* givenWindow)
{
	if (glfwGetKey(givenWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(givenWindow, true);

	if (glfwGetKey(givenWindow, GLFW_KEY_W) == GLFW_PRESS)
		myCamera.ProcessKeyboard(FORWARD, deltaTime);

	if (glfwGetKey(givenWindow, GLFW_KEY_S) == GLFW_PRESS)
		myCamera.ProcessKeyboard(BACKWARD, deltaTime);

	if (glfwGetKey(givenWindow, GLFW_KEY_A) == GLFW_PRESS)
		myCamera.ProcessKeyboard(LEFT, deltaTime);

	if (glfwGetKey(givenWindow, GLFW_KEY_D) == GLFW_PRESS)
		myCamera.ProcessKeyboard(RIGHT, deltaTime);
}

void MousPositionCallback(GLFWwindow* givenWindow, double givenMousePositionX, double givenMousePositionY)
{
	//std::cout << givenMousePositionX << std::endl;
	//std::cout << givenMousePositionY << std::endl;
	if (firstMouse)
	{
		previousMousePositonX = givenMousePositionX;
		previousMousePositonY = givenMousePositionY;

		firstMouse = false;
	}
	float offsetX = givenMousePositionX - previousMousePositonX;
	float offsetY = previousMousePositonY - givenMousePositionY;
	previousMousePositonX = givenMousePositionX;
	previousMousePositonY = givenMousePositionY;
	myCamera.ProcessMouseMovement(offsetX, offsetY);
}

void ScrollCallback(GLFWwindow* givenWindow, double givenScrollOffsetX, double givenScrollOffsetY)
{
	myCamera.ProcessMouseScroll(givenScrollOffsetY);
}

unsigned int GenerateTexture(const char* givenTextureFilePath)
{
	unsigned int textureId;
	glGenTextures(1, &textureId);

	int imageWidth, imageHeight, numberOfchannels;
	unsigned char* imageData;
	imageData = stbi_load(givenTextureFilePath, &imageWidth, &imageHeight, &numberOfchannels, 0);

	stbi_set_flip_vertically_on_load(true);

	int imageFormat = 3;

	if (numberOfchannels == 1)
		imageFormat = GL_RED;
	else if (numberOfchannels == 3)
		imageFormat = GL_RGB;
	else if (numberOfchannels == 4)
		imageFormat = GL_RGBA;

	if (imageData)
	{
		glBindTexture(GL_TEXTURE_2D, textureId);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, imageFormat, imageWidth, imageHeight, 0, imageFormat, GL_UNSIGNED_BYTE, imageData);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
		std::cout << "Could not read the image!!!" << std::endl;

	stbi_image_free(imageData);
	return textureId;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++

unsigned int quadVBO = 0;
void renderQuad()
{
	if (quadVBO == 0)
	{
		// positions
		glm::vec3 pos1(-0.5f, -0.5f, -0.5f);//2
		glm::vec3 pos2(0.5f, -0.5f, -0.5f);//3
		glm::vec3 pos3(0.5f, 0.5f, -0.5f);//4
		glm::vec3 pos4(-0.5f, 0.5f, -0.5f);//1

		glm::vec3 pos5(-0.5f, -0.5f, 0.5f);//2
		glm::vec3 pos6(0.5f, -0.5f, 0.5f);//3
		glm::vec3 pos7(0.5f, 0.5f, 0.5f);//4
		glm::vec3 pos8(-0.5f, 0.5f, 0.5f);//1

		glm::vec3 pos9(-0.5f, 0.5f, 0.5f);//2
		glm::vec3 pos10(-0.5f, 0.5f, -0.5f);//3
		glm::vec3 pos11(-0.5f, -0.5f, -0.5f);//4
		glm::vec3 pos12(-0.5f, -0.5f, 0.5f);//1

		glm::vec3 pos13(0.5f, 0.5f, 0.5f);//2
		glm::vec3 pos14(0.5f, 0.5f, -0.5f);//3
		glm::vec3 pos15(0.5f, -0.5f, -0.5f);//4
		glm::vec3 pos16(0.5f, -0.5f, 0.5f);//1

		glm::vec3 pos17(-0.5f, -0.5f, -0.5f);//2
		glm::vec3 pos18(0.5f, -0.5f, -0.5f);//3
		glm::vec3 pos19(0.5f, -0.5f, 0.5f);//4
		glm::vec3 pos20(-0.5f, -0.5f, 0.5f);//1

		glm::vec3 pos21(-0.5f, 0.5f, -0.5f);//2
		glm::vec3 pos22(0.5f, 0.5f, -0.5f);//3
		glm::vec3 pos23(0.5f, 0.5f, 0.5f);//4
		glm::vec3 pos24(-0.5f, 0.5f, 0.5f);//1


		// normal vector
		glm::vec3 nm(0.0f, 0.0f, 1.0f);
		//--------------------
		glm::vec3 nm1(0.0f, 0.0f, -1.0f);
		glm::vec3 nm2(-1.0f, 0.0f, 0.0f);
		glm::vec3 nm3(1.0f, 0.0f, 0.0f);
		glm::vec3 nm4(0.0f, -1.0f, 0.0f);
		glm::vec3 nm5(0.0f, 1.0f, 0.0f);

		// texture coordinates
		glm::vec2 uv1(0.0f, 1.0f);
		glm::vec2 uv2(0.0f, 0.0f);

		glm::vec2 uv3(1.0f, 0.0f);
		glm::vec2 uv4(1.0f, 1.0f);





		// calculate tangent/bitangent vectors of both triangles
		glm::vec3 tangent1, bitangent1;
		glm::vec3 tangent2, bitangent2;
		//---------------------------
		glm::vec3 tangent3, bitangent3;
		glm::vec3 tangent4, bitangent4;
		glm::vec3 tangent5, bitangent5;
		glm::vec3 tangent6, bitangent6;
		glm::vec3 tangent7, bitangent7;
		glm::vec3 tangent8, bitangent8;
		glm::vec3 tangent9, bitangent9;
		glm::vec3 tangent10, bitangent10;
		glm::vec3 tangent11, bitangent11;
		glm::vec3 tangent12, bitangent12;

		// triangle 1
		// ----------
		glm::vec3 edge1 = pos1 - pos4;
		glm::vec3 edge2 = pos2 - pos4;
		glm::vec2 deltaUV1 = uv2 - uv1;
		glm::vec2 deltaUV2 = uv3 - uv1;

		float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

		bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

		// triangle 2
		// ----------
		edge1 = pos2 - pos4;
		edge2 = pos3 - pos4;
		deltaUV1 = uv3 - uv1;
		deltaUV2 = uv4 - uv1;

		f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);


		bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

		//trigono 3
		edge1 = pos5 - pos8;
		edge2 = pos6 - pos8;
		deltaUV1 = uv2 - uv1;
		deltaUV2 = uv3 - uv1;

		f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		tangent3.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent3.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent3.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);


		bitangent3.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent3.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent3.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);


		//trigono 4
		edge1 = pos6 - pos8;
		edge2 = pos7 - pos8;
		deltaUV1 = uv3 - uv1;
		deltaUV2 = uv4 - uv1;

		f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		tangent4.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent4.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent4.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);


		bitangent4.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent4.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent4.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

		//trigono 5
		edge1 = pos9 - pos12;
		edge2 = pos10 - pos12;
		deltaUV1 = uv3 - uv2;
		deltaUV2 = uv4 - uv2;

		f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		tangent5.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent5.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent5.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);


		bitangent5.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent5.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent5.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);


		//trigono 6
		edge1 = pos10 - pos12;
		edge2 = pos11 - pos12;
		deltaUV1 = uv4 - uv2;
		deltaUV2 = uv1 - uv2;

		f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		tangent6.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent6.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent6.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);


		bitangent6.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent6.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent6.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

		//trigono 7
		edge1 = pos13 - pos16;
		edge2 = pos14 - pos16;
		deltaUV1 = uv3 - uv2;
		deltaUV2 = uv4 - uv2;

		f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		tangent7.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent7.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent7.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);


		bitangent7.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent7.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent7.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);


		//trigono 8
		edge1 = pos14 - pos16;
		edge2 = pos15 - pos16;
		deltaUV1 = uv4 - uv2;
		deltaUV2 = uv1 - uv2;

		f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		tangent8.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent8.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent8.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);


		bitangent8.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent8.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent8.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

		//trigono 9
		edge1 = pos17 - pos20;
		edge2 = pos18 - pos20;
		deltaUV1 = uv1 - uv2;
		deltaUV2 = uv4 - uv2;

		f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		tangent9.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent9.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent9.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);


		bitangent9.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent9.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent9.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);


		//trigono 10
		edge1 = pos18 - pos20;
		edge2 = pos19 - pos20;
		deltaUV1 = uv4 - uv2;
		deltaUV2 = uv3 - uv2;

		f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		tangent10.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent10.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent10.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);


		bitangent10.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent10.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent10.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

		//trigono 11
		edge1 = pos21 - pos24;
		edge2 = pos22 - pos24;
		deltaUV1 = uv1 - uv2;
		deltaUV2 = uv4 - uv2;

		f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		tangent11.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent11.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent11.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);


		bitangent11.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent11.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent11.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);


		//trigono 12
		edge1 = pos22 - pos24;
		edge2 = pos23 - pos24;
		deltaUV1 = uv4 - uv2;
		deltaUV2 = uv3 - uv2;

		f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		tangent12.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent12.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent12.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);


		bitangent12.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent12.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent12.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);






		float quadVertices[] = {
			// positions            // normal         // texcoords  // tangent                          // bitangent
			pos4.x, pos4.y, pos4.z, nm1.x, nm1.y, nm1.z, uv4.x, uv4.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
			pos1.x, pos1.y, pos1.z, nm1.x, nm1.y, nm1.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
			pos2.x, pos2.y, pos2.z, nm1.x, nm1.y, nm1.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

			pos4.x, pos4.y, pos4.z, nm1.x, nm1.y, nm1.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
			pos2.x, pos2.y, pos2.z, nm1.x, nm1.y, nm1.z, uv2.x, uv2.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
			pos3.x, pos3.y, pos3.z, nm1.x, nm1.y, nm1.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
			//-----------------------------
			pos8.x, pos8.y, pos8.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent3.x, tangent3.y, tangent3.z, bitangent3.x, bitangent3.y, bitangent3.z,
			pos5.x, pos5.y, pos5.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent3.x, tangent3.y, tangent3.z, bitangent3.x, bitangent3.y, bitangent3.z,
			pos6.x, pos6.y, pos6.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent3.x, tangent3.y, tangent3.z, bitangent3.x, bitangent3.y, bitangent3.z,

			pos8.x, pos8.y, pos8.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent4.x, tangent4.y, tangent4.z, bitangent4.x, bitangent4.y, bitangent4.z,
			pos6.x, pos6.y, pos6.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent4.x, tangent4.y, tangent4.z, bitangent4.x, bitangent4.y, bitangent4.z,
			pos7.x, pos7.y, pos7.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent4.x, tangent4.y, tangent4.z, bitangent4.x, bitangent4.y, bitangent4.z,

			//--------------------------------

			pos12.x, pos12.y, pos12.z, nm2.x, nm2.y, nm2.z, uv2.x, uv2.y, tangent5.x, tangent5.y, tangent5.z, bitangent5.x, bitangent5.y, bitangent5.z,
			pos9.x, pos9.y, pos9.z, nm2.x, nm2.y, nm2.z, uv3.x, uv3.y, tangent5.x, tangent5.y, tangent5.z, bitangent5.x, bitangent5.y, bitangent5.z,
			pos10.x, pos10.y, pos10.z, nm2.x, nm2.y, nm2.z, uv4.x, uv4.y, tangent5.x, tangent5.y, tangent5.z, bitangent5.x, bitangent5.y, bitangent5.z,

			pos12.x, pos12.y, pos12.z, nm2.x, nm2.y, nm2.z, uv2.x, uv2.y, tangent6.x, tangent6.y, tangent6.z, bitangent6.x, bitangent6.y, bitangent6.z,
			pos10.x, pos10.y, pos10.z, nm2.x, nm2.y, nm2.z, uv4.x, uv4.y, tangent6.x, tangent6.y, tangent6.z, bitangent6.x, bitangent6.y, bitangent6.z,
			pos11.x, pos11.y, pos11.z, nm2.x, nm2.y, nm2.z, uv1.x, uv1.y, tangent6.x, tangent6.y, tangent6.z, bitangent6.x, bitangent6.y, bitangent6.z,

			//-------------------------------
			pos16.x, pos16.y, pos16.z, nm3.x, nm3.y, nm3.z, uv2.x, uv2.y, tangent7.x, tangent7.y, tangent7.z, bitangent7.x, bitangent7.y, bitangent7.z,
			pos13.x, pos13.y, pos13.z, nm3.x, nm3.y, nm3.z, uv3.x, uv3.y, tangent7.x, tangent7.y, tangent7.z, bitangent7.x, bitangent7.y, bitangent7.z,
			pos14.x, pos14.y, pos14.z, nm3.x, nm3.y, nm3.z, uv4.x, uv4.y, tangent7.x, tangent7.y, tangent7.z, bitangent7.x, bitangent7.y, bitangent7.z,

			pos16.x, pos16.y, pos16.z, nm3.x, nm3.y, nm3.z, uv2.x, uv2.y, tangent8.x, tangent8.y, tangent8.z, bitangent8.x, bitangent8.y, bitangent8.z,
			pos14.x, pos14.y, pos14.z, nm3.x, nm3.y, nm3.z, uv4.x, uv4.y, tangent8.x, tangent8.y, tangent8.z, bitangent8.x, bitangent8.y, bitangent8.z,
			pos15.x, pos15.y, pos15.z, nm3.x, nm3.y, nm3.z, uv1.x, uv1.y, tangent8.x, tangent8.y, tangent8.z, bitangent8.x, bitangent8.y, bitangent8.z,

			//--------------------------------------

			pos20.x, pos20.y, pos20.z, nm4.x, nm4.y, nm4.z, uv2.x, uv2.y, tangent9.x, tangent9.y, tangent9.z, bitangent9.x, bitangent9.y, bitangent9.z,
			pos17.x, pos17.y, pos17.z, nm4.x, nm4.y, nm4.z, uv1.x, uv1.y, tangent9.x, tangent9.y, tangent9.z, bitangent9.x, bitangent9.y, bitangent9.z,
			pos18.x, pos18.y, pos18.z, nm4.x, nm4.y, nm4.z, uv4.x, uv4.y, tangent9.x, tangent9.y, tangent9.z, bitangent9.x, bitangent9.y, bitangent9.z,

			pos20.x, pos20.y, pos20.z, nm4.x, nm4.y, nm4.z, uv2.x, uv2.y, tangent10.x, tangent10.y, tangent10.z, bitangent10.x, bitangent10.y, bitangent10.z,
			pos18.x, pos18.y, pos18.z, nm4.x, nm4.y, nm4.z, uv4.x, uv4.y, tangent10.x, tangent10.y, tangent10.z, bitangent10.x, bitangent10.y, bitangent10.z,
			pos19.x, pos19.y, pos19.z, nm4.x, nm4.y, nm4.z, uv3.x, uv3.y, tangent10.x, tangent10.y, tangent10.z, bitangent10.x, bitangent10.y, bitangent10.z,

			//------------------------------------

			pos24.x, pos24.y, pos24.z, nm5.x, nm5.y, nm5.z, uv2.x, uv2.y, tangent11.x, tangent11.y, tangent11.z, bitangent11.x, bitangent11.y, bitangent11.z,
			pos21.x, pos21.y, pos21.z, nm5.x, nm5.y, nm5.z, uv1.x, uv1.y, tangent11.x, tangent11.y, tangent11.z, bitangent11.x, bitangent11.y, bitangent11.z,
			pos22.x, pos22.y, pos22.z, nm5.x, nm5.y, nm5.z, uv4.x, uv4.y, tangent11.x, tangent11.y, tangent11.z, bitangent11.x, bitangent11.y, bitangent11.z,

			pos24.x, pos24.y, pos24.z, nm5.x, nm5.y, nm5.z, uv2.x, uv2.y, tangent12.x, tangent12.y, tangent12.z, bitangent12.x, bitangent12.y, bitangent12.z,
			pos22.x, pos22.y, pos22.z, nm5.x, nm5.y, nm5.z, uv4.x, uv4.y, tangent12.x, tangent12.y, tangent12.z, bitangent12.x, bitangent12.y, bitangent12.z,
			pos23.x, pos23.y, pos23.z, nm5.x, nm5.y, nm5.z, uv3.x, uv3.y, tangent12.x, tangent12.y, tangent12.z, bitangent12.x, bitangent12.y, bitangent12.z








		};
		// configure plane VAO
		glGenBuffers(1, &quadVBO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	}
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));
	glDrawArrays(GL_TRIANGLES, 0, 36);
}




float  z_orio = 10.0f;
float y_orio = 0.0f;

bool flag = false;
bool flag1 = false;

bool flag2 = false;
bool flag3 = false;

bool flag4 = false;
bool flag5 = false;

bool flag6 = false;
bool flag7 = false;

float x =  0.0f, y = 15.0f, z = 0.0f;
float x1 = 2.0f, q = 15.0f, z1 = 0.0f;
float x2 =-2.0f, q2 = 15.0f,z2 = 0.0f;
float x3 = 0.0f, q3 = 15.0f,z3 = 2.0f;

float f1 = 0.25f;
float speed = 0.005;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {

		int randomChoice = rand() % 4;

		switch (randomChoice) {
		case 0:
			flag = true;
			break;
		case 1:
			flag2 = true;
			break;
		case 2:
			flag4 = true;
			break;
		case 3:
			flag6 = true;
			break;
		}
	}
	if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		speed += 0.002f;
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		speed -= 0.002f;
	}
	if (speed < 0.001f) {
		speed = 0.001f;
	}
	if (speed > 0.01f) {
		speed = 0.01f;
	}
}

void thesi() {
	if (flag) {

		y -= speed;
		f1 = 0.0f;
		if (y <= y_orio) {
			y = y_orio;
			flag = false;
			flag1 = true;
		}
	}
	else if (flag1) {	
		z += speed;
		f1 = 0.0f;
		if (z >= z_orio) {
			x = (rand() % 21) - 10;// (rand() % 31) - 15;
			z = (rand() % 21) - 10;// (rand() % 31) - 15;
			y = 15.0f;
			flag1 = false;
			f1 = 0.25f;
		}
	}
}

void thesi1() {
	if (flag2) {
		q -= speed;
		if (q <= y_orio) {
			q = y_orio;
			flag2 = false;
			flag3 = true;
		}
	}
	else if (flag3) {
		z1 += speed;
		if (z1 >= z_orio) {
			x1 = (rand() % 21) - 10;// (rand() % 31) - 15;
			z1 = (rand() % 21) - 10;// (rand() % 31) - 15;
			q = 15.0f;
			flag3 = false;
		}
	}
}


void thesi2() {
	if (flag4) {
		q2 -= speed;
		if (q2 <= y_orio) {
			q2 = y_orio;
			flag4 = false;
			flag5 = true;
		}
	}
	else if (flag5) {
		z2 += speed;
		if (z2 >= z_orio) {
			x2 = (rand() % 21) - 10;// (rand() % 31) - 15;
			z2 = (rand() % 21) - 10;// (rand() % 31) - 15;
			q2 = 15.0f;
			flag5 = false;
		}
	}
}

void thesi3() {
	if (flag6) {
		q3 -= speed;
		if (q3 <= y_orio) {
			q3 = y_orio;
			flag6 = false;
			flag7 = true;
		}
	}
	else if (flag7) {
		z3 += speed;
		if (z3 >= z_orio) {
			x3 = (rand() % 21) - 10;// (rand() % 31) - 15;
			z3 = (rand() % 21) - 10;// (rand() % 31) - 15;
			q3 = 15.0f;
			flag7 = false;
		}
	}
}


int main()
{

	// Initialization

	if (!glfwInit())
	{
		std::cout << "Failed to initialize glfw!" << std::endl;
		return -1;
	}

	GLFWwindow* window = glfwCreateWindow(800, 600, "Ergasia3_Hercules_Gousis_AM:58650", NULL, NULL);

	if (!window)
	{
		std::cout << "Failed to initialize the window!" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	// Set the callback function
	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);
	glfwSetCursorPosCallback(window, MousPositionCallback);
	glfwSetScrollCallback(window, ScrollCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize glew!" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetKeyCallback(window, key_callback);
	std::cout << glGetString(GL_VERSION) << std::endl;

	//Data

	//--Vertex Data
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
		
	};

	glm::vec3 cubePositions[] =
	{
	  glm::vec3(0.0f,  0.0f,  0.0f)
	
	};


	// first buffer (vbo)
	unsigned int vertexBufferId;
	glGenBuffers(1, &vertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	Shader shaderProgram("res/Shaders/VertexShader1_31.txt", "res/Shaders/FragmentShader1_31.txt");
	Shader shaderProgram1("res/Shaders/VertexShader1_31.txt", "res/Shaders/FragmentShader2_31.txt");
	Shader shaderProgram2("res/Shaders/VertexShader1_31.txt", "res/Shaders/FragmentShader3_31.txt");
	Shader lightSourceShaderProgram("res/Shaders/LightSourceVertexShader3_31.txt", "res/Shaders/LightSourceFragmentShader5_31.txt");
	Shader shaderProgram3("res/Shaders/VertexShader2_31.txt", "res/Shaders/FragmentShader4_31.txt");
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glm::mat4 identity = glm::mat4(1.0f);

	glEnable(GL_DEPTH_TEST);

	glDepthFunc(GL_LESS);

	unsigned int containerTextureId = GenerateTexture("res/Images/container.jpg");
	unsigned int awesomeFaceTextureId = GenerateTexture("res/Images/awesomeface.png");
	unsigned int container2TextureId = GenerateTexture("res/Images/container2.png");
	unsigned int container2TextureId1 = GenerateTexture("res/Images/matrix.jpg");
	unsigned int container2SpecularTextureId = GenerateTexture("res/Images/container2_specular.png");
	unsigned int brickwallTexture = GenerateTexture("res/Images/brickwall.jpg");
	unsigned int brickwallNormalTexture = GenerateTexture("res/Images/brickwall_normal.jpg");

	// Game loop

	while (!glfwWindowShouldClose(window))
	{
		ProcessInput(window);

		float time = glfwGetTime();

		currentFrameTime = time;
		deltaTime = currentFrameTime - previousFrameTime;
		previousFrameTime = currentFrameTime;

		

		glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		thesi();
		thesi1();
		thesi2();
		thesi3();
		//glm::vec3 lightSourcePosition(0.0f, -0.5f, -1.0f);

		glm::vec3 lightSourceDirection1(0.0f, -0.5f, -1.0f); // only for directional light
		glm::vec3 lightSourceDirection2(-1.0f, -0.5f, 0.0f);
		glm::vec3 lightSourceDirection3(0.0f, -0.5f, 1.0f);
		glm::vec3 lightSourceDirection4(1.0f, -0.5f, 0.0f);

		glm::mat4 view;
		glm::mat4 model;
		glm::mat4 projection;

		view = myCamera.GetViewMatrix();// now we get the view matrix form the camrera class -------------
		//view = glm::translate(identity, glm::vec3(0.0f, -10.0f, -40.0f));
		//view = glm::rotate(view, glm::radians(40.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//int viewLocation = glGetUniformLocation(shaderProgram.ID, "view");
		//glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
		
		shaderProgram.setMat4("view", view);
		// 2D
		//projection = glm::ortho(-5.0f, 5.0f, -6.0f, 6.0f, -15.0f, 18.0f); // the values were chosen to fit the cube positions in each axis

		// 3D
		projection = glm::perspective(glm::radians(myCamera.Zoom), 800.0f / 600.0f, 0.3f, 100.0f); // now we get the zoom form the camera class ---------------------
		shaderProgram.setMat4("projection", projection);
		
//--------------------------------------------------------------------------
		float f;
		float sine_value = sin(2 * PI * time);

		if (sine_value > 0) {
			f = 0.0f;
		}
		else
		{
			f = 1.0f;
		};
		//std::cout << sine_value << std::endl;
		glm::vec3 lightSourceColor(f, f, f);
		lightSourceShaderProgram.use();
		glm::vec3 lightSourcePosition(x, y, z);

		lightSourceShaderProgram.setVec3("lightSourceColor", lightSourceColor);
		lightSourceShaderProgram.setMat4("view", view);
		lightSourceShaderProgram.setMat4("projection", projection);
		model = glm::translate(identity, lightSourcePosition);
		lightSourceShaderProgram.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

	//---------------------------------------------------------------------------------------

		shaderProgram1.use();
		shaderProgram1.setMat4("view", view);
		shaderProgram1.setMat4("projection", projection);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, containerTextureId);
		shaderProgram1.setInt("containerTexture", 3);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, awesomeFaceTextureId);
		shaderProgram1.setInt("awesomeFaceTexture", 4);

		shaderProgram1.setFloat("material.shininess", 128.0f * 2.0f);
		shaderProgram1.setVec3("cameraPosition", myCamera.Position);

		// Light Source		       
		shaderProgram1.setVec3("lightSource.direction1", lightSourceDirection1);
		shaderProgram1.setVec3("lightSource.direction2", lightSourceDirection2);
		shaderProgram1.setVec3("lightSource.direction3", lightSourceDirection3);
		shaderProgram1.setVec3("lightSource.direction4", lightSourceDirection4);

	    // only for directional light			 
		shaderProgram1.setVec3("lightSource.ambient", glm::vec3(f1, f1, f1));
		shaderProgram1.setVec3("lightSource.diffuse", glm::vec3(f1, f1, f1));
		shaderProgram1.setVec3("lightSource.specular", glm::vec3(f1, f1, f1));
		
		//------------------------------------


		shaderProgram1.setVec3("lightSource.position", lightSourcePosition);
		shaderProgram1.setVec3("lightSource.ambientColor", glm::vec3(f, f, f));
		shaderProgram1.setVec3("lightSource.diffuseColor", glm::vec3(f, f, f));
		shaderProgram1.setVec3("lightSource.specularColor", glm::vec3(f, f, f));

		shaderProgram1.setFloat("lightSource.constant", 1.0f);
		shaderProgram1.setFloat("lightSource.linear", 0.045f);
		shaderProgram1.setFloat("lightSource.quadratic", 0.0075f);
		glm::vec3  mpambis(x1, q, z1);
		model = glm::translate(identity, mpambis);
		shaderProgram1.setMat4("model", model);
		glm::mat3 normalTransformation1 = glm::transpose(glm::inverse(glm::mat3(model)));
		shaderProgram1.setMat3("normalTransformation", normalTransformation1);
		glDrawArrays(GL_TRIANGLES, 0, 36);


    //===============================================
	 
		shaderProgram2.use();
		shaderProgram2.setMat4("view", view);
		shaderProgram2.setMat4("projection", projection);
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, container2TextureId);
		shaderProgram2.setInt("material.container2Texture", 5);
		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, container2SpecularTextureId);
		shaderProgram2.setInt("material.container2SpecularTexture", 6);

		shaderProgram2.setFloat("material.shininess", 128.0f * 2.0f);
		shaderProgram2.setVec3("cameraPosition", myCamera.Position);
		// Light Source

		shaderProgram2.setVec3("lightSource.direction1", lightSourceDirection1);
		shaderProgram2.setVec3("lightSource.direction2", lightSourceDirection2);
		shaderProgram2.setVec3("lightSource.direction3", lightSourceDirection3);
		shaderProgram2.setVec3("lightSource.direction4", lightSourceDirection4);
		// only for directional light
		shaderProgram2.setVec3("lightSource.ambient", glm::vec3(f1, f1, f1));
		shaderProgram2.setVec3("lightSource.diffuse", glm::vec3(f1, f1, f1));
		shaderProgram2.setVec3("lightSource.specular", glm::vec3(f1, f1, f1));

		//------------------------------------


		shaderProgram2.setVec3("lightSource.position", lightSourcePosition);

		shaderProgram2.setVec3("lightSource.ambientColor", glm::vec3(f, f, f));
		shaderProgram2.setVec3("lightSource.diffuseColor", glm::vec3(f, f, f));
		shaderProgram2.setVec3("lightSource.specularColor", glm::vec3(f, f, f));

		shaderProgram2.setFloat("lightSource.constant", 1.0f);
		shaderProgram2.setFloat("lightSource.linear", 0.045f);
		shaderProgram2.setFloat("lightSource.quadratic", 0.0075f);
		glm::vec3  mpambis1(x2, q2, z2);
		model = glm::translate(identity, mpambis1);
		shaderProgram2.setMat4("model", model);

		glm::mat3 normalTransformation2 = glm::transpose(glm::inverse(glm::mat3(model)));
		shaderProgram2.setMat3("normalTransformation", normalTransformation2);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	 
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		shaderProgram3.use();
		shaderProgram3.setInt("diffuseMap", 0);
		shaderProgram3.setInt("normalMap", 1);
		shaderProgram3.setVec3("zenia1", lightSourceDirection1);
		shaderProgram3.setVec3("zenia2", lightSourceDirection2);
		shaderProgram3.setVec3("zenia3", lightSourceDirection3);
		shaderProgram3.setVec3("zenia4", lightSourceDirection4);

		shaderProgram3.setVec3("lightSourcePosition", lightSourcePosition);

		shaderProgram3.setVec3("viewPosition", myCamera.Position);
		shaderProgram3.setVec3("entasi", glm::vec3(f1, f1, f1));
		shaderProgram3.setVec3("entasi2", glm::vec3(f, f, f));

		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D, brickwallTexture);
		shaderProgram3.setInt("diffuseMap", 7);
		glActiveTexture(GL_TEXTURE8);
		glBindTexture(GL_TEXTURE_2D, brickwallNormalTexture);
		shaderProgram3.setInt("normalMap", 8);
		shaderProgram3.setMat4("view", view);
		shaderProgram3.setMat4("projection", projection);
		glm::vec3  mpambis2(x3, q3, z3);
		model = glm::translate(identity, mpambis2);
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		shaderProgram3.setMat4("model", model);

		renderQuad();
		
		
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



	// Render Using the shader program
		shaderProgram.use();
		glActiveTexture(GL_TEXTURE0);
	    glBindTexture(GL_TEXTURE_2D, container2TextureId);
		shaderProgram.setInt("material.container2Texture", 0);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, container2TextureId1);
		shaderProgram.setInt("material.container2Texture1", 2);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, container2SpecularTextureId);
		shaderProgram.setInt("material.container2SpecularTexture", 1);
		shaderProgram.setFloat("material.shininess", 128.0f * 2.0f);
		shaderProgram.setFloat("time", time/2);
		shaderProgram.setVec3("cameraPosition", myCamera.Position);

		// Material
		//shaderProgram.setFloat("material.shininess", 128.0f * 2.0f);

		// Light Source
		//shaderProgram.setVec3("lightSource.position", lightSourcePosition);
		shaderProgram.setVec3("lightSource.direction1", lightSourceDirection1); 
		shaderProgram.setVec3("lightSource.direction2", lightSourceDirection2);
		shaderProgram.setVec3("lightSource.direction3", lightSourceDirection3);
		shaderProgram.setVec3("lightSource.direction4", lightSourceDirection4);
		
		// only for directional light
		shaderProgram.setVec3("lightSource.ambient", glm::vec3(f1, f1, f1));
		shaderProgram.setVec3("lightSource.diffuse", glm::vec3(f1, f1, f1));
		shaderProgram.setVec3("lightSource.specular", glm::vec3(f1, f1, f1));
	//------------------------------------


		shaderProgram.setVec3("lightSource.position", lightSourcePosition);
		shaderProgram.setVec3("lightSource.ambientColor", glm::vec3(f, f, f));
		shaderProgram.setVec3("lightSource.diffuseColor", glm::vec3(f, f, f));
		shaderProgram.setVec3("lightSource.specularColor", glm::vec3(f, f, f));
		shaderProgram.setFloat("lightSource.constant", 1.0f);
		shaderProgram.setFloat("lightSource.linear", 0.045f);
		shaderProgram.setFloat("lightSource.quadratic", 0.0075f);
			model = glm::translate(identity, cubePositions[0]);
			model = glm::scale(model, glm::vec3(20.0f, 1.0f, 20.0f));
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

			shaderProgram.setMat4("model", model);
			glm::mat3 normalTransformation = glm::transpose(glm::inverse(glm::mat3(model)));
			shaderProgram.setMat3("normalTransformation", normalTransformation);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		
	//------------------------------------------------------------------------------------------
		// Render Using the light source shader program
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

