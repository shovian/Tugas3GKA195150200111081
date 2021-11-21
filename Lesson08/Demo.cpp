#include "Demo.h"



Demo::Demo() {

}


Demo::~Demo() {
}



void Demo::Init() {
	// build and compile our shader program
	// ------------------------------------
	shadowmapShader = BuildShader("multipleLight.vert", "multipleLight.frag", nullptr);
	BuildTexturedCube();
	BuildTexturedPlane();
}

void Demo::DeInit() {
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &cubeEBO);
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteBuffers(1, &planeVBO);
	glDeleteBuffers(1, &planeEBO);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Demo::ProcessInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void Demo::Update(double deltaTime) {
}

void Demo::Render() {
	glViewport(0, 0, this->screenWidth, this->screenHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_DEPTH_TEST);

	// Pass perspective projection matrix
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)this->screenWidth / (GLfloat)this->screenHeight, 0.1f, 100.0f);
	GLint projLoc = glGetUniformLocation(this->shadowmapShader, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// LookAt camera (position, target/direction, up)
	glm::vec3 cameraPos = glm::vec3(3, 7, 8);
	glm::vec3 cameraFront = glm::vec3(0, -1, -1);
	glm::mat4 view = glm::lookAt(cameraPos, cameraFront, glm::vec3(0, 1, 0));
	GLint viewLoc = glGetUniformLocation(this->shadowmapShader, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	// set lighting attributes
	GLint viewPosLoc = glGetUniformLocation(this->shadowmapShader, "viewPos");
	glUniform3f(viewPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);

	glUniform3f(glGetUniformLocation(this->shadowmapShader, "dirLight.direction"), 0.0f, -1.0f, -1.0f);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "dirLight.ambient"), 0.1f, 0.1f, 0.1f);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "dirLight.diffuse"), 1.0f, 0.0f, 0.0f);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "dirLight.specular"), 0.1f, 0.1f, 0.1f);
	// point light 1
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "pointLights[0].position"), 0.0f, 3.0f, 0.0f);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "pointLights[0].ambient"), 1.0f, 0.0f, 1.0f);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "pointLights[0].diffuse"), 1.0f, 0.0f, 0.0f);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "pointLights[0].specular"), 1.0f, 0.0f, 0.0f);
	glUniform1f(glGetUniformLocation(this->shadowmapShader, "pointLights[0].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(this->shadowmapShader, "pointLights[0].linear"), 0.09f);
	glUniform1f(glGetUniformLocation(this->shadowmapShader, "pointLights[0].quadratic"), 0.032f);
	// point light 2
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "pointLights[1].position"), -2.0f, 3.0f, 0.0f);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "pointLights[1].ambient"), 0.0f, 1.0f, 0.0f);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "pointLights[1].diffuse"), 0.0f, 1.0f, 0.0f);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "pointLights[1].specular"), 0.0f, 1.0f, 0.0f);
	glUniform1f(glGetUniformLocation(this->shadowmapShader, "pointLights[1].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(this->shadowmapShader, "pointLights[1].linear"), 0.09f);
	glUniform1f(glGetUniformLocation(this->shadowmapShader, "pointLights[1].quadratic"), 0.032f);
	// point light 3
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "pointLights[2].position"), 2.0f, 3.0f, 0.0f);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "pointLights[2].ambient"), 0.0f, 0.0f, 1.0f);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "pointLights[2].diffuse"), 0.0f, 0.0f, 1.0f);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "pointLights[2].specular"), 0.0f, 0.0f, 1.0f);
	glUniform1f(glGetUniformLocation(this->shadowmapShader, "pointLights[2].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(this->shadowmapShader, "pointLights[2].linear"), 0.09f);
	glUniform1f(glGetUniformLocation(this->shadowmapShader, "pointLights[2].quadratic"), 0.032f);
	// point light 4
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "pointLights[3].position"), 0.0f, 3.0f, 2.0f);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "pointLights[3].ambient"), 0.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "pointLights[3].diffuse"), 0.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "pointLights[3].specular"), 0.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(this->shadowmapShader, "pointLights[3].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(this->shadowmapShader, "pointLights[3].linear"), 0.09f);
	glUniform1f(glGetUniformLocation(this->shadowmapShader, "pointLights[3].quadratic"), 0.032f);
	// spotLight
	glUniform3fv(glGetUniformLocation(this->shadowmapShader, "spotLight.position"), 1, &cameraPos[0]);
	glUniform3fv(glGetUniformLocation(this->shadowmapShader, "spotLight.direction"), 1, &cameraFront[0]);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "spotLight.ambient"), 1.0f, 0.0f, 1.0f);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "spotLight.diffuse"), 1.0f, 0.0f, 1.0f);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "spotLight.specular"), 1.0f, 0.0f, 1.0f);
	glUniform1f(glGetUniformLocation(this->shadowmapShader, "spotLight.constant"), 1.0f);
	glUniform1f(glGetUniformLocation(this->shadowmapShader, "spotLight.linear"), 0.09f);
	glUniform1f(glGetUniformLocation(this->shadowmapShader, "spotLight.quadratic"), 0.032f);
	glUniform1f(glGetUniformLocation(this->shadowmapShader, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
	glUniform1f(glGetUniformLocation(this->shadowmapShader, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));
	
	DrawTexturedCube();
	DrawTexturedPlane();

	glDisable(GL_DEPTH_TEST);
}

void Demo::BuildTexturedCube()
{
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &cube_texture);
	glBindTexture(GL_TEXTURE_2D, cube_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("crate_diffusemap.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &stexture);
	glBindTexture(GL_TEXTURE_2D, stexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	image = SOIL_load_image("crate_specularmap.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// front
		-0.5, 0.4, 0.5, 0, 0, 0.0f,  0.0f,  1.0f,  // 0
		0.5, 0.4, 0.5, 1, 0, 0.0f,  0.0f,  1.0f,   // 1
		0.5,  0.5, 0.5, 1, 1, 0.0f,  0.0f,  1.0f,   // 2
		-0.5,  0.5, 0.5, 0, 1, 0.0f,  0.0f,  1.0f,  // 3

		// right
		0.5,  0.5,  0.5, 0, 0, 1.0f,  0.0f,  0.0f,  // 4
		0.5,  0.5, -0.5, 1, 0, 1.0f,  0.0f,  0.0f,  // 5
		0.5, 0.4, -0.5, 1, 1, 1.0f,  0.0f,  0.0f,  // 6
		0.5, 0.4,  0.5, 0, 1, 1.0f,  0.0f,  0.0f,  // 7

		// back
		-0.5, 0.4, -0.5, 0, 0, 0.0f,  0.0f,  -1.0f, // 8 
		0.5,  0.4, -0.5, 1, 0, 0.0f,  0.0f,  -1.0f, // 9
		0.5,   0.5, -0.5, 1, 1, 0.0f,  0.0f,  -1.0f, // 10
		-0.5,  0.5, -0.5, 0, 1, 0.0f,  0.0f,  -1.0f, // 11

		// left
		-0.5, 0.4, -0.5, 0, 0, -1.0f,  0.0f,  0.0f, // 12
		-0.5, 0.4,  0.5, 1, 0, -1.0f,  0.0f,  0.0f, // 13
		-0.5,  0.5,  0.5, 1, 1, -1.0f,  0.0f,  0.0f, // 14
		-0.5,  0.5, -0.5, 0, 1, -1.0f,  0.0f,  0.0f, // 15

		// upper
		0.5, 0.5,  0.5, 0, 0,  0.0f,  1.0f,  0.0f,   // 16
		-0.5, 0.5,  0.5, 1, 0,  0.0f,  1.0f,  0.0f,  // 17
		-0.5, 0.5, -0.5, 1, 1,  0.0f,  1.0f,  0.0f,  // 18
		0.5, 0.5, -0.5, 0, 1,  0.0f,  1.0f,  0.0f,   // 19

		// bottom
		-0.5, 0.4, -0.5, 0, 0, 0.0f,  -1.0f,  0.0f, // 20
		0.5, 0.4, -0.5, 1, 0, 0.0f,  -1.0f,  0.0f,  // 21
		0.5, 0.4,  0.5, 1, 1, 0.0f,  -1.0f,  0.0f,  // 22
		-0.5, 0.4,  0.5, 0, 1, 0.0f,  -1.0f,  0.0f, // 23
		//kolong bawah

		// front
		-0.5, 0.1, 0.4, 0, 0, 0.0f,  0.0f,  1.0f,  // 0
		0.5, 0.1, 0.4, 1, 0, 0.0f,  0.0f,  1.0f,   // 1
		0.5,  0.2, 0.4, 1, 1, 0.0f,  0.0f,  1.0f,   // 2
		-0.5,  0.2, 0.4, 0, 1, 0.0f,  0.0f,  1.0f,  // 3

		// right
		0.5,  0.2,  0.4, 0, 0, 1.0f,  0.0f,  0.0f,  // 4
		0.5,  0.2, -0.5, 1, 0, 1.0f,  0.0f,  0.0f,  // 5
		0.5, 0.1, -0.5, 1, 1, 1.0f,  0.0f,  0.0f,  // 6
		0.5, 0.1,  0.4, 0, 1, 1.0f,  0.0f,  0.0f,  // 7

		// back
		-0.5, 0.1, -0.5, 0, 0, 0.0f,  0.0f,  -1.0f, // 8 
		0.5,  0.1, -0.5, 1, 0, 0.0f,  0.0f,  -1.0f, // 9
		0.5,   0.2, -0.5, 1, 1, 0.0f,  0.0f,  -1.0f, // 10
		-0.5,  0.2, -0.5, 0, 1, 0.0f,  0.0f,  -1.0f, // 11

		// left
		-0.5, 0.1, -0.5, 0, 0, -1.0f,  0.0f,  0.0f, // 12
		-0.5, 0.1,  0.4, 1, 0, -1.0f,  0.0f,  0.0f, // 13
		-0.5,  0.2,  0.4, 1, 1, -1.0f,  0.0f,  0.0f, // 14
		-0.5,  0.2, -0.5, 0, 1, -1.0f,  0.0f,  0.0f, // 15

		// upper
		0.5, 0.2,  0.4, 0, 0,  0.0f,  1.0f,  0.0f,   // 16
		-0.5, 0.2,  0.4, 1, 0,  0.0f,  1.0f,  0.0f,  // 17
		-0.5, 0.2, -0.5, 1, 1,  0.0f,  1.0f,  0.0f,  // 18
		0.5, 0.2, -0.5, 0, 1,  0.0f,  1.0f,  0.0f,   // 19

		// bottom
		-0.5, 0.1, -0.5, 0, 0, 0.0f,  -1.0f,  0.0f, // 20
		0.5, 0.1, -0.5, 1, 0, 0.0f,  -1.0f,  0.0f,  // 21
		0.5, 0.1,  0.4, 1, 1, 0.0f,  -1.0f,  0.0f,  // 22
		-0.5, 0.1,  0.4, 0, 1, 0.0f,  -1.0f,  0.0f, // 23
		//belakang

		// front
		-0.5, 0.2, -0.4, 0, 0, 0.0f,  0.0f,  1.0f,  // 0
		0.5, 0.2, -0.4, 1, 0, 0.0f,  0.0f,  1.0f,   // 1
		0.5,  0.4, -0.4, 1, 1, 0.0f,  0.0f,  1.0f,   // 2
		-0.5,  0.4, -0.4, 0, 1, 0.0f,  0.0f,  1.0f,  // 3

		// right
		0.5,  0.4,  -0.4, 0, 0, 1.0f,  0.0f,  0.0f,  // 4
		0.5,  0.4, -0.5, 1, 0, 1.0f,  0.0f,  0.0f,  // 5
		0.5, 0.2, -0.5, 1, 1, 1.0f,  0.0f,  0.0f,  // 6
		0.5, 0.2,  -0.4, 0, 1, 1.0f,  0.0f,  0.0f,  // 7

		// back
		-0.5, 0.2, -0.5, 0, 0, 0.0f,  0.0f,  -1.0f, // 8 
		0.5,  0.2, -0.5, 1, 0, 0.0f,  0.0f,  -1.0f, // 9
		0.5,   0.4, -0.5, 1, 1, 0.0f,  0.0f,  -1.0f, // 10
		-0.5,  0.4, -0.5, 0, 1, 0.0f,  0.0f,  -1.0f, // 11

		// left
		-0.5, 0.2, -0.5, 0, 0, -1.0f,  0.0f,  0.0f, // 12
		-0.5, 0.2,  -0.4, 1, 0, -1.0f,  0.0f,  0.0f, // 13
		-0.5,  0.4,  -0.4, 1, 1, -1.0f,  0.0f,  0.0f, // 14
		-0.5,  0.4, -0.5, 0, 1, -1.0f,  0.0f,  0.0f, // 15

		// upper
		0.5, 0.4,  -0.4, 0, 0,  0.0f,  1.0f,  0.0f,   // 16
		-0.5, 0.4,  -0.4, 1, 0, 0.0f, 1.0f, 0.0f,  // 17
		-0.5, 0.4, -0.5, 1, 1, 0.0f, 1.0f, 0.0f,  // 18
		0.5, 0.4, -0.5, 0, 1, 0.0f, 1.0f, 0.0f,   // 19

		// bottom
		-0.5, 0.2, -0.5, 0, 0, 0.0f, -1.0f, 0.0f, // 20
		0.5, 0.2, -0.5, 1, 0, 0.0f, -1.0f, 0.0f,  // 21
		0.5, 0.2,  -0.4, 1, 1, 0.0f, -1.0f, 0.0f,  // 22
		-0.5, 0.2,  -0.4, 0, 1, 0.0f, -1.0f, 0.0f, // 23
			//kanan
			// front
			0.4, 0.2, 0.4, 0, 0, 0.0f, 0.0f, 1.0f,  // 0
		0.5, 0.2, 0.4, 1, 0, 0.0f, 0.0f, 1.0f,   // 1
		0.5, 0.4, 0.4, 1, 1, 0.0f, 0.0f, 1.0f,   // 2
			0.4, 0.4, 0.4, 0, 1, 0.0f, 0.0f, 1.0f,  // 3

		// right
		0.5, 0.4, 0.4, 0, 0, 1.0f, 0.0f, 0.0f,  // 4
		0.5, 0.4, -0.5, 1, 0, 1.0f, 0.0f, 0.0f,  // 5
		0.5, 0.2, -0.5, 1, 1, 1.0f, 0.0f, 0.0f,  // 6
		0.5, 0.2, 0.4, 0, 1, 1.0f, 0.0f, 0.0f,  // 7

		// back
			0.4, 0.2, -0.5, 0, 0, 0.0f, 0.0f, -1.0f, // 8 
		0.5, 0.2, -0.5, 1, 0, 0.0f, 0.0f, -1.0f, // 9
		0.5, 0.4, -0.5, 1, 1, 0.0f, 0.0f, -1.0f, // 10
			0.4, 0.4, -0.5, 0, 1, 0.0f, 0.0f, -1.0f, // 11

		// left
			0.4, 0.2, -0.5, 0, 0, -1.0f, 0.0f, 0.0f, // 12
			0.4, 0.2, 0.4, 1, 0, -1.0f, 0.0f, 0.0f, // 13
			0.4, 0.4, 0.4, 1, 1, -1.0f, 0.0f, 0.0f, // 14
			0.4, 0.4, -0.5, 0, 1, -1.0f, 0.0f, 0.0f, // 15

		// upper
		0.5, 0.4, 0.4, 0, 0, 0.0f, 1.0f, 0.0f,   // 16
			0.4, 0.4, 0.4, 1, 0, 0.0f, 1.0f, 0.0f,  // 17
			0.4, 0.4, -0.5, 1, 1, 0.0f, 1.0f, 0.0f,  // 18
		0.5, 0.4, -0.5, 0, 1, 0.0f, 1.0f, 0.0f,   // 19

		// bottom
			0.4, 0.2, -0.5, 0, 0, 0.0f, -1.0f, 0.0f, // 20
		0.5, 0.2, -0.5, 1, 0, 0.0f, -1.0f, 0.0f,  // 21
		0.5, 0.2, 0.4, 1, 1, 0.0f, -1.0f, 0.0f,  // 22
			0.4, 0.2, 0.4, 0, 1, 0.0f, -1.0f, 0.0f, // 23
			// kiri
			// front
			-0.5, 0.2, 0.4, 0, 0, 0.0f, 0.0f, 1.0f,  // 0
			-0.4, 0.2, 0.4, 1, 0, 0.0f, 0.0f, 1.0f,   // 1
			-0.4, 0.4, 0.4, 1, 1, 0.0f, 0.0f, 1.0f,   // 2
			-0.5, 0.4, 0.4, 0, 1, 0.0f, 0.0f, 1.0f,  // 3

		// right
			-0.4, 0.4, 0.4, 0, 0, 1.0f, 0.0f, 0.0f,  // 4
			-0.4, 0.4, -0.5, 1, 0, 1.0f, 0.0f, 0.0f,  // 5
			-0.4, 0.2, -0.5, 1, 1, 1.0f, 0.0f, 0.0f,  // 6
			-0.4, 0.2, 0.4, 0, 1, 1.0f, 0.0f, 0.0f,  // 7

			// back
			-0.5, 0.2, -0.5, 0, 0, 0.0f, 0.0f, -1.0f, // 8 
			-0.4, 0.2, -0.5, 1, 0, 0.0f, 0.0f, -1.0f, // 9
			-0.4, 0.4, -0.5, 1, 1, 0.0f, 0.0f, -1.0f, // 10
			-0.5, 0.4, -0.5, 0, 1, 0.0f, 0.0f, -1.0f, // 11

		// left
			-0.5, 0.2, -0.5, 0, 0, -1.0f, 0.0f, 0.0f, // 12
			-0.5, 0.2, 0.4, 1, 0, -1.0f, 0.0f, 0.0f, // 13
			-0.5, 0.4, 0.4, 1, 1, -1.0f, 0.0f, 0.0f, // 14
			-0.5, 0.4, -0.5, 0, 1, -1.0f, 0.0f, 0.0f, // 15

		// upper
			-0.4, 0.4, 0.4, 0, 0, 0.0f, 1.0f, 0.0f,   // 16
			-0.5, 0.4, 0.4, 1, 0, 0.0f, 1.0f, 0.0f,  // 17
			-0.5, 0.4, -0.5, 1, 1, 0.0f, 1.0f, 0.0f,  // 18
			-0.4, 0.4, -0.5, 0, 1, 0.0f, 1.0f, 0.0f,   // 19

			// bottom
			-0.5, 0.2, -0.5, 0, 0, 0.0f, -1.0f, 0.0f, // 20
			-0.4, 0.2, -0.5, 1, 0, 0.0f, -1.0f, 0.0f,  // 21
			-0.4, 0.2, 0.4, 1, 1, 0.0f, -1.0f, 0.0f,  // 22
			-0.5, 0.2, 0.4, 0, 1, 0.0f, -1.0f, 0.0f, // 23
			// tengah
			// front
			-0.05, 0.2, 0.4, 0, 0, 0.0f, 0.0f, 1.0f,  // 0
			0.05, 0.2, 0.4, 1, 0, 0.0f, 0.0f, 1.0f,   // 1
			0.05, 0.4, 0.4, 1, 1, 0.0f, 0.0f, 1.0f,   // 2
			-0.05, 0.4, 0.4, 0, 1, 0.0f, 0.0f, 1.0f,  // 3

		// right
			0.05, 0.4, 0.4, 0, 0, 1.0f, 0.0f, 0.0f,  // 4
			0.05, 0.4, -0.5, 1, 0, 1.0f, 0.0f, 0.0f,  // 5
			0.05, 0.2, -0.5, 1, 1, 1.0f, 0.0f, 0.0f,  // 6
			0.05, 0.2, 0.4, 0, 1, 1.0f, 0.0f, 0.0f,  // 7

			// back
			-0.05, 0.2, -0.5, 0, 0, 0.0f, 0.0f, -1.0f, // 8 
			0.05, 0.2, -0.5, 1, 0, 0.0f, 0.0f, -1.0f, // 9
			0.05, 0.4, -0.5, 1, 1, 0.0f, 0.0f, -1.0f, // 10
			-0.05, 0.4, -0.5, 0, 1, 0.0f, 0.0f, -1.0f, // 11

		// left
			-0.05, 0.2, -0.5, 0, 0, -1.0f, 0.0f, 0.0f, // 12
			-0.05, 0.2, 0.4, 1, 0, -1.0f, 0.0f, 0.0f, // 13
			-0.05, 0.4, 0.4, 1, 1, -1.0f, 0.0f, 0.0f, // 14
			-0.05, 0.4, -0.5, 0, 1, -1.0f, 0.0f, 0.0f, // 15

		// upper
			0.05, 0.4, 0.4, 0, 0, 0.0f, 1.0f, 0.0f,   // 16
			-0.05, 0.4, 0.4, 1, 0, 0.0f, 1.0f, 0.0f,  // 17
			-0.05, 0.4, -0.5, 1, 1, 0.0f, 1.0f, 0.0f,  // 18
				0.05, 0.4, -0.5, 0, 1, 0.0f, 1.0f, 0.0f,   // 19

			// bottom
			-0.05, 0.2, -0.5, 0, 0, 0.0f, -1.0f, 0.0f, // 20
			0.05, 0.2, -0.5, 1, 0, 0.0f, -1.0f, 0.0f,  // 21
			0.05, 0.2, 0.4, 1, 1, 0.0f, -1.0f, 0.0f,  // 22
			-0.05, 0.2, 0.4, 0, 1, 0.0f, -1.0f, 0.0f, // 23
			// kaki depan kanan
			// front
			0.4, -0.5, 0.4, 0, 0, 0.0f, 0.0f, 1.0f,  // 0
			0.5, -0.5, 0.4, 1, 0, 0.0f, 0.0f, 1.0f,   // 1
			0.5, 0.1, 0.4, 1, 1, 0.0f, 0.0f, 1.0f,   // 2
				0.4, 0.1, 0.4, 0, 1, 0.0f, 0.0f, 1.0f,  // 3

			// right
			0.5, 0.1, 0.4, 0, 0, 1.0f, 0.0f, 0.0f,  // 4
			0.5, 0.1, 0.3, 1, 0, 1.0f, 0.0f, 0.0f,  // 5
			0.5, -0.5, 0.3, 1, 1, 1.0f, 0.0f, 0.0f,  // 6
			0.5, -0.5, 0.4, 0, 1, 1.0f, 0.0f, 0.0f,  // 7

			// back
				0.4, -0.5, 0.3, 0, 0, 0.0f, 0.0f, -1.0f, // 8 
			0.5, -0.5, 0.3, 1, 0, 0.0f, 0.0f, -1.0f, // 9
			0.5, 0.1, 0.3, 1, 1, 0.0f, 0.0f, -1.0f, // 10
			0.4, 0.1, 0.3, 0, 1, 0.0f, 0.0f, -1.0f, // 11

			// left
				0.4, -0.5, 0.3, 0, 0, -1.0f, 0.0f, 0.0f, // 12
			0.4, -0.5, 0.4, 1, 0, -1.0f, 0.0f, 0.0f, // 13
				0.4, 0.1, 0.4, 1, 1, -1.0f, 0.0f, 0.0f, // 14
				0.4, 0.1, 0.3, 0, 1, -1.0f, 0.0f, 0.0f, // 15

			// upper
			0.5, 0.1, 0.4, 0, 0, 0.0f, 1.0f, 0.0f,   // 16
				0.4, 0.1, 0.4, 1, 0, 0.0f, 1.0f, 0.0f,  // 17
				0.4, 0.1, 0.3, 1, 1, 0.0f, 1.0f, 0.0f,  // 18
			0.5, 0.1, 0.3, 0, 1, 0.0f, 1.0f, 0.0f,   // 19

			// bottom
				0.4, -0.5, 0.3, 0, 0, 0.0f, -1.0f, 0.0f, // 20
			0.5, -0.5, 0.3, 1, 0, 0.0f, -1.0f, 0.0f,  // 21
			0.5, -0.5, 0.4, 1, 1, 0.0f, -1.0f, 0.0f,  // 22
				0.4, -0.5, 0.4, 0, 1, 0.0f, -1.0f, 0.0f, // 23
				// kaki depan kiri
			// front
				-0.5, -0.5, 0.4, 0, 0, 0.0f, 0.0f, 1.0f,  // 0
				-0.4, -0.5, 0.4, 1, 0, 0.0f, 0.0f, 1.0f,   // 1
				-0.4, 0.1, 0.4, 1, 1, 0.0f, 0.0f, 1.0f,   // 2
				-0.5, 0.1, 0.4, 0, 1, 0.0f, 0.0f, 1.0f,  // 3

			// right
				-0.4, 0.1, 0.4, 0, 0, 1.0f, 0.0f, 0.0f,  // 4
				-0.4, 0.1, 0.3, 1, 0, 1.0f, 0.0f, 0.0f,  // 5
				-0.4, -0.5, 0.3, 1, 1, 1.0f, 0.0f, 0.0f,  // 6
				-0.4, -0.5, 0.4, 0, 1, 1.0f, 0.0f, 0.0f,  // 7

				// back
				-0.5, -0.5, 0.3, 0, 0, 0.0f, 0.0f, -1.0f, // 8 
				-0.4, -0.5, 0.3, 1, 0, 0.0f, 0.0f, -1.0f, // 9
				-0.4, 0.1, 0.3, 1, 1, 0.0f, 0.0f, -1.0f, // 10
				-0.5, 0.1, 0.3, 0, 1, 0.0f, 0.0f, -1.0f, // 11

				// left
				-0.5, -0.5, 0.3, 0, 0, -1.0f, 0.0f, 0.0f, // 12
				-0.5, -0.5, 0.4, 1, 0, -1.0f, 0.0f, 0.0f, // 13
				-0.5, 0.1, 0.4, 1, 1, -1.0f, 0.0f, 0.0f, // 14
				-0.5, 0.1, 0.3, 0, 1, -1.0f, 0.0f, 0.0f, // 15

			// upper
				-0.4, 0.1, 0.4, 0, 0, 0.0f, 1.0f, 0.0f,   // 16
				-0.5, 0.1, 0.4, 1, 0, 0.0f, 1.0f, 0.0f,  // 17
				-0.5, 0.1, 0.3, 1, 1, 0.0f, 1.0f, 0.0f,  // 18
				-0.4, 0.1, 0.3, 0, 1, 0.0f, 1.0f, 0.0f,   // 19

				// bottom
				-0.5, -0.5, 0.3, 0, 0, 0.0f, -1.0f, 0.0f, // 20
				-0.4, -0.5, 0.3, 1, 0, 0.0f, -1.0f, 0.0f,  // 21
				-0.4, -0.5, 0.4, 1, 1, 0.0f, -1.0f, 0.0f,  // 22
				-0.5, -0.5, 0.4, 0, 1, 0.0f, -1.0f, 0.0f, // 23
				// kaki belakang kiri
			// front
				-0.5, -0.5, -0.4, 0, 0, 0.0f, 0.0f, 1.0f,  // 0
				-0.4, -0.5, -0.4, 1, 0, 0.0f, 0.0f, 1.0f,   // 1
				-0.4, 0.1, -0.4, 1, 1, 0.0f, 0.0f, 1.0f,   // 2
				-0.5, 0.1, -0.4, 0, 1, 0.0f, 0.0f, 1.0f,  // 3

			// right
				-0.4, 0.1, -0.4, 0, 0, 1.0f, 0.0f, 0.0f,  // 4
				-0.4, 0.1, -0.5, 1, 0, 1.0f, 0.0f, 0.0f,  // 5
				-0.4, -0.5, -0.5, 1, 1, 1.0f, 0.0f, 0.0f,  // 6
				-0.4, -0.5, -0.4, 0, 1, 1.0f, 0.0f, 0.0f,  // 7

				// back
				-0.5, -0.5, -0.5, 0, 0, 0.0f, 0.0f, -1.0f, // 8 
				-0.4, -0.5, -0.5, 1, 0, 0.0f, 0.0f, -1.0f, // 9
				-0.4, 0.1, -0.5, 1, 1, 0.0f, 0.0f, -1.0f, // 10
				-0.5, 0.1, -0.5, 0, 1, 0.0f, 0.0f, -1.0f, // 11

				// left
				-0.5, -0.5, -0.5, 0, 0, -1.0f, 0.0f, 0.0f, // 12
				-0.5, -0.5, -0.4, 1, 0, -1.0f, 0.0f, 0.0f, // 13
				-0.5, 0.1, -0.4, 1, 1, -1.0f, 0.0f, 0.0f, // 14
				-0.5, 0.1, -0.5, 0, 1, -1.0f, 0.0f, 0.0f, // 15

			// upper
				-0.4, 0.1, -0.4, 0, 0, 0.0f, 1.0f, 0.0f,   // 16
				-0.5, 0.1, -0.4, 1, 0, 0.0f, 1.0f, 0.0f,  // 17
				-0.5, 0.1, -0.5, 1, 1, 0.0f, 1.0f, 0.0f,  // 18
				-0.4, 0.1, -0.5, 0, 1, 0.0f, 1.0f, 0.0f,   // 19

				// bottom
				-0.5, -0.5, -0.5, 0, 0, 0.0f, -1.0f, 0.0f, // 20
				-0.4, -0.5, -0.5, 1, 0, 0.0f, -1.0f, 0.0f,  // 21
				-0.4, -0.5, -0.4, 1, 1, 0.0f, -1.0f, 0.0f,  // 22
				-0.5, -0.5, -0.4, 0, 1, 0.0f, -1.0f, 0.0f, // 23
				// kaki belakang kanan
			// front
				0.4, -0.5, -0.4, 0, 0, 0.0f, 0.0f, 1.0f,  // 0
				0.5, -0.5, -0.4, 1, 0, 0.0f, 0.0f, 1.0f,   // 1
					0.5, 0.1, -0.4, 1, 1, 0.0f, 0.0f, 1.0f,   // 2
					0.4, 0.1, -0.4, 0, 1, 0.0f, 0.0f, 1.0f,  // 3

			// right
				0.5, 0.1, -0.4, 0, 0, 1.0f, 0.0f, 0.0f,  // 4
				0.5, 0.1, -0.5, 1, 0, 1.0f, 0.0f, 0.0f,  // 5
					0.5, -0.5, -0.5, 1, 1, 1.0f, 0.0f, 0.0f,  // 6
					0.5, -0.5, -0.4, 0, 1, 1.0f, 0.0f, 0.0f,  // 7

				// back
					0.4, -0.5, -0.5, 0, 0, 0.0f, 0.0f, -1.0f, // 8 
					0.5, -0.5, -0.5, 1, 0, 0.0f, 0.0f, -1.0f, // 9
				0.5, 0.1, -0.5, 1, 1, 0.0f, 0.0f, -1.0f, // 10
				0.4, 0.1, -0.5, 0, 1, 0.0f, 0.0f, -1.0f, // 11

				// left
					0.4, -0.5, -0.5, 0, 0, -1.0f, 0.0f, 0.0f, // 12
				0.4, -0.5, -0.4, 1, 0, -1.0f, 0.0f, 0.0f, // 13
					0.4, 0.1, -0.4, 1, 1, -1.0f, 0.0f, 0.0f, // 14
					0.4, 0.1, -0.5, 0, 1, -1.0f, 0.0f, 0.0f, // 15

			// upper
				0.5, 0.1, -0.4, 0, 0, 0.0f, 1.0f, 0.0f,   // 16
					0.4, 0.1, -0.4, 1, 0, 0.0f, 1.0f, 0.0f,  // 17
					0.4, 0.1, -0.5, 1, 1, 0.0f, 1.0f, 0.0f,  // 18
					0.5, 0.1, -0.5, 0, 1, 0.0f, 1.0f, 0.0f,   // 19

				// bottom
					0.4, -0.5, -0.5, 0, 0, 0.0f, -1.0f, 0.0f, // 20
					0.5, -0.5, -0.5, 1, 0, 0.0f, -1.0f, 0.0f,  // 21
					0.5, -0.5, -0.4, 1, 1, 0.0f, -1.0f, 0.0f,  // 22
					0.4, -0.5, -0.4, 0, 1, 0.0f, -1.0f, 0.0f, // 23
	};

	unsigned int indices[] = { 0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22,   // bottom
		24,25,26,24,26,27,28,29,30,28,30,31,32,33,34,32,34,35,36,38,37,36,39,38,40,42,41,40,43,42,44,46,45,44,47,46,
		48,49,50,48,50,51,52,53,54,52,54,55,56,57,58,56,58,59,60,62,61,60,63,62,64,66,65,64,67,66,68,70,69,68,71,70
		,72,73,74,72,74,75,76,77,78,76,78,79,80,81,82,80,82,83,84,86,85,84,87,86,88,90,89,88,91,90,92,94,93,92,95,94
		,96,97,98,96,98,99,100,101,102,100,102,103,104,105,106,104,106,107,108,110,109,108,111,110,112,114,113,112,115,114,116,118,117,116,119,118
		,120,121,122,120,122,123,124,125,126,124,126,127,128,129,130,128,130,131,132,134,133,132,135,134,136,138,137,136,139,138,140,142,141,140,143,142
		,144,145,146,144,146,147,148,149,150,148,150,151,152,153,154,152,154,155,156,158,157,156,159,158,160,162,161,160,163,162,164,166,165,164,167,166
		,168,169,170,168,170,171,172,173,174,172,174,175,176,177,178,176,178,179,180,182,181,180,183,182,184,186,185,184,187,186,188,190,189,188,191,190
		,192,193,194,192,194,195,196,197,198,196,198,199,200,201,202,200,202,203,204,206,205,204,207,206,208,210,209,208,211,210,212,214,213,212,215,214
		,216,217,218,216,218,219,220,221,222,220,222,223,224,225,226,224,226,227,228,230,229,228,231,230,232,234,233,232,235,234,236,238,237,236,239,238
	};

	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glGenBuffers(1, &cubeEBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(cubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// define normal pointer layout 2
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Demo::BuildTexturedPlane()
{
	// Load and create a texture 
	glGenTextures(1, &plane_texture);
	glBindTexture(GL_TEXTURE_2D, plane_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("marble_diffusemap.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &stexture2);
	glBindTexture(GL_TEXTURE_2D, stexture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	image = SOIL_load_image("marble_specularmap.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Build geometry
	GLfloat vertices[] = {
		// format position, tex coords
		// bottom
		-50.0, -0.5, -50.0,  0,  0, 0.0f,  1.0f,  0.0f,
		50.0, -0.5, -50.0, 50,  0, 0.0f,  1.0f,  0.0f,
		50.0, -0.5,  50.0, 50, 50, 0.0f,  1.0f,  0.0f,
		-50.0, -0.5,  50.0,  0, 50, 0.0f,  1.0f,  0.0f,


	};

	GLuint indices[] = { 0,  2,  1,  0,  3,  2 };

	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glGenBuffers(1, &planeEBO);

	glBindVertexArray(planeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planeEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Normal attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO
}

void Demo::DrawTexturedCube()
{
	UseShader(this->shadowmapShader);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, cube_texture);
	glUniform1i(glGetUniformLocation(this->shadowmapShader, "material.diffuse"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, stexture);
	glUniform1i(glGetUniformLocation(this->shadowmapShader, "material.specular"), 1);

	GLint shininessMatLoc = glGetUniformLocation(this->shadowmapShader, "material.shininess");
	glUniform1f(shininessMatLoc, 0.4f);

	glBindVertexArray(cubeVAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 0, 0));

	GLint modelLoc = glGetUniformLocation(this->shadowmapShader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 1000, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawTexturedPlane()
{
	UseShader(this->shadowmapShader);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, plane_texture);
	glUniform1i(glGetUniformLocation(this->shadowmapShader, "material.diffuse"), 2);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, stexture2);
	glUniform1i(glGetUniformLocation(this->shadowmapShader, "material.specular"), 3);

	GLint shininessMatLoc = glGetUniformLocation(this->shadowmapShader, "material.shininess");
	glUniform1f(shininessMatLoc, 0.4f);

	glBindVertexArray(planeVAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	GLint modelLoc = glGetUniformLocation(this->shadowmapShader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

int main(int argc, char** argv) {
	RenderEngine &app = Demo();
	app.Start("Multiple Lighting Demo", 800, 600, false, false);
}