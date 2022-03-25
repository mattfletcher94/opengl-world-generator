#define _USE_MATH_DEFINES

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp> 
#include "glm/ext/vector_float3.hpp"
#include <glm/ext/matrix_transform.hpp> 
#include <glm/ext/matrix_clip_space.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include <math.h>
#include <vector>
#include "lib/ShaderProgram.h"
#include "lib/Camera.h"
#include "lib/VAO.h"
#include "lib/EntityAbstract.h"
#include "lib/EntityTerrain.h"
#include "lib/TextureLoader.h"
#include "lib/FBO.h"
#include "lib/TerrainGenerator.h"
#include "vendors/imgui-1.60/imgui.h"
#include "vendors/imgui-1.60/imgui_impl_glfw_gl3.h"
#include "lib/EntityLightSource.h"


// Keyboard controls
bool wPressed = false;
bool dPressed = false;
bool sPressed = false;
bool aPressed = false;
bool iPressed = false;
bool lPressed = false;
bool kPressed = false;
bool jPressed = false;
bool upPressed = false;
bool downPressed = false;
void keyCallbackFunction(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// W key
	if(key == GLFW_KEY_W && action == 1) {
		wPressed = true;
	}
	if(key == GLFW_KEY_W && action == 0) {
		wPressed = false;
	}
	// D key
	if(key == GLFW_KEY_D && action == 1) {
		dPressed = true;
	}
	if(key == GLFW_KEY_D && action == 0) {
		dPressed = false;
	}
	// S key
	if(key == GLFW_KEY_S && action == 1) {
		sPressed = true;
	}
	if(key == GLFW_KEY_S && action == 0) {
		sPressed = false;
	}
	// A key
	if(key == GLFW_KEY_A && action == 1) {
		aPressed = true;
	}
	if(key == GLFW_KEY_A && action == 0) {
		aPressed = false;
	}

	// I Key
	if(key == GLFW_KEY_I && action == 1) {
		iPressed = true;
	}
	if(key == GLFW_KEY_I && action == 0) {
		iPressed = false;
	}

	// L Key
	if(key == GLFW_KEY_L && action == 1) {
		lPressed = true;
	}
	if(key == GLFW_KEY_L && action == 0) {
		lPressed = false;
	}

	// K key
	if(key == GLFW_KEY_K && action == 1) {
		kPressed = true;
	}
	if(key == GLFW_KEY_K && action == 0) {
		kPressed = false;
	}
	// J key
	if(key == GLFW_KEY_J && action == 1) {
		jPressed = true;
	}
	if(key == GLFW_KEY_J && action == 0) {
		jPressed = false;
	}


	// UP key
	if(key == GLFW_KEY_UP && action == 1) {
		upPressed = true;
	}
	if(key == GLFW_KEY_UP && action == 0) {
		upPressed = false;
	}

	// Down key
	if(key == GLFW_KEY_DOWN && action == 1) {
		downPressed = true;
	}
	if(key == GLFW_KEY_DOWN && action == 0) {
		downPressed = false;
	}
}


// GLOBALS
const float WINDOW_WIDTH = 2000.0f;
const float WINDOW_HEIGHT = 1200.0f;
const float FPS = 1.0f / 60.0f;
float FOG_DENSITY = 0.0005f;
float FOG_DENSITY_SLIDER = 5;
float SUN_POSITION_X = 0.0f;
float SUN_POSITION_Y = 75.0f;
float SUN_POSITION_Z = 0.0f;
glm::vec3 SKY_COLOR = glm::vec3(0.1f, 0.5f, 0.75f);
glm::vec3 SUN_AMBIENT = glm::vec3(0.2f, 0.2f, 0.2f);
glm::vec3 SUN_DIFFUSE = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 SUN_SPECULAR = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 SUN_ATTENUATION = glm::vec3(1.0f,	0.0014f,0.000007f);
char TERRAIN_HEIGHTMAP_PATH[512] = "res/heightmaps/heightmap-1.png";
char TERRAIN_BLENDMAP_PATH[512] = "res/blendmaps/blendmap-1.png";
float TERRAIN_SIZE = 200.0f;
float TERRAIN_MAX_HEIGHT = 40.0f;
float BLOOM_THRESHOLD = 0.9f;
int BLOOM_BLUR_STRENGTH = 6;

// Create camera
Camera camera = Camera(WINDOW_WIDTH, WINDOW_HEIGHT);

// Store FBOs in a map
std::map<std::string, FBO> FBOs;

// On resize
void framebufferSizeCallbackFunction(GLFWwindow* window, int width, int height) {
	camera.width = (float)width;
	camera.height = (float)height;
	FBOs.at("postProcessing").dispose();
	FBOs.at("postProcessingBloom").dispose();
	FBOs.at("postProcessingFinal").dispose();
	FBOs.at("postProcessing") = FBO((float)width, (float)height);
	FBOs.at("postProcessingBloom") = FBO((float)width, (float)height);
	FBOs.at("postProcessingFinal") = FBO((float)width, (float)height);
	glViewport(0, 0, width, height);
}


/*
 * Main
 */
int main(int argc,char** argv) {
	srand(time(NULL));


	// Init flfw
	glfwInit();

    // Create a windowed mode window and its OpenGL context 
	glfwWindowHint(GLFW_SAMPLES, 8); // Antialiasing
    GLFWwindow* window = glfwCreateWindow((int)WINDOW_WIDTH, (int)WINDOW_HEIGHT, "Coursework 2", NULL, NULL);
	glfwSetKeyCallback(window, keyCallbackFunction);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallbackFunction);

    // Make the window's context current
    glfwMakeContextCurrent(window);

	// Init glew
    glewInit();

	// ---------------------------
	// ---------------------------
	// ---------------------------
	
	// Create the FBO
	FBOs.insert({ "postProcessing", FBO(WINDOW_WIDTH, WINDOW_HEIGHT) });
	FBOs.insert({ "postProcessingBloom", FBO(WINDOW_WIDTH, WINDOW_HEIGHT) });
	FBOs.insert({ "postProcessingFinal", FBO(WINDOW_WIDTH, WINDOW_HEIGHT) });


	// Load textures
	GLuint textureGrass = TextureLoader::Load("res/textures/grass-texture.jpg");
	GLuint textureDirt = TextureLoader::Load("res/textures/dirt-texture.jpg");
	GLuint textureSnow = TextureLoader::Load("res/textures/white-texture.png");
	GLuint texturePath = TextureLoader::Load("res/textures/path-texture.jpg");
	GLuint textureBlendMap = TextureLoader::Load(TERRAIN_BLENDMAP_PATH);

	// Generate a terrain
	TerrainGenerator terrainGenerator = TerrainGenerator(TERRAIN_HEIGHTMAP_PATH, (int)TERRAIN_SIZE, (int)TERRAIN_MAX_HEIGHT);
	VAO vaoTerrain = terrainGenerator.generateVAO();

	// Create box VAO
	VAO vaoSphere = VAO("res/objects/sphere.obj");


	// Bind textures and VAOs
	vaoTerrain.setTexture0(textureGrass);
	vaoTerrain.setTexture1(textureDirt);
	vaoTerrain.setTexture2(textureSnow);
	vaoTerrain.setTexture3(texturePath);
	vaoTerrain.setTexture4(textureBlendMap);

	// Add entities to terrain VAO
	vaoTerrain.addEntity(new EntityTerrain(glm::vec3(-TERRAIN_SIZE / 2.0f, 0.0f, -TERRAIN_SIZE / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f));

	// Create light source model
	EntityLightSource* sunEntity = new EntityLightSource(glm::vec3(SUN_POSITION_X, SUN_POSITION_Y, SUN_POSITION_Z), glm::vec3(0.0f, 0.0f, 0.0f), 0.1f);
	vaoSphere.addEntity(sunEntity);


	// Position the camera
	camera.posY = TERRAIN_MAX_HEIGHT + 50.0f;
	camera.posZ = -(TERRAIN_SIZE );
	camera.posX = -(TERRAIN_SIZE);
	camera.rotY = 135.0f;

    // Load and create the shader programs
	ShaderProgram modelsShader("shaders/models.vert","shaders/models.frag");
	ShaderProgram terrainShader("shaders/terrain.vert", "shaders/terrain.frag");
	ShaderProgram lightSourceShader("shaders/lightSource.vert","shaders/lightSource.frag");
	ShaderProgram postProcessingShader("shaders/postProcessing.vert","shaders/postProcessing.frag");
	ShaderProgram blurShader("shaders/blur.vert","shaders/blur.frag");
	ShaderProgram brightnessShader("shaders/brightness.vert","shaders/brightness.frag");

	// Set up post processing shader
	postProcessingShader.start();
	postProcessingShader.setUniformInt("screenTexture", 0);
	postProcessingShader.setUniformInt("bloomTexture", 1);
	postProcessingShader.stop();
	
	// Set up blur shader
	blurShader.start();
	blurShader.setUniformInt("uniformTexture", 0);
	blurShader.stop();
	
	// Set up brightness shader
	brightnessShader.start();
	brightnessShader.setUniformInt("screenTexture", 0);
	brightnessShader.setUniform1f("uniformBloomThreshold", BLOOM_THRESHOLD);
	brightnessShader.stop();

	// Set up models shader
	modelsShader.start();
    modelsShader.setUniform1f("light.constant", SUN_ATTENUATION.x);
    modelsShader.setUniform1f("light.linear", SUN_ATTENUATION.y);
    modelsShader.setUniform1f("light.quadratic", SUN_ATTENUATION.z);
	modelsShader.setUniformInt("texture0", 0);
	modelsShader.stop();

	// Set up terrain shader
	terrainShader.start();
	terrainShader.setUniform1f("light.constant", SUN_ATTENUATION.x);
	terrainShader.setUniform1f("light.linear", SUN_ATTENUATION.y);
	terrainShader.setUniform1f("light.quadratic", SUN_ATTENUATION.z);
	terrainShader.setUniformInt("blendMap.backgroundTexture", 0);
	terrainShader.setUniformInt("blendMap.redTexture", 1);
	terrainShader.setUniformInt("blendMap.greenTexture", 2);
	terrainShader.setUniformInt("blendMap.blueTexture", 3);
	terrainShader.setUniformInt("blendMap.blendMapTexture", 4);
	terrainShader.stop();


	// Setup ImGui binding
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplGlfwGL3_Init(window, true);
	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();


	// Number of seconds since the last loop 
	double lastUpdateTime = 0; 

	// Number of seconds since the last frame
	double lastFrameTime = 0;  

    // render loop
    while (!glfwWindowShouldClose(window)) {

		// Calculate delta time
		double now = glfwGetTime();
		double deltaTime = now - lastUpdateTime;

		// Execute 60 times per second
		if ((now - lastFrameTime) >= FPS) {
			
			// Poll events
			glfwPollEvents();

			// If imgui needs keyboard, let it use it
			// else use my standard functions
			if (io.WantCaptureKeyboard) {
				glfwSetKeyCallback(window, ImGui_ImplGlfw_KeyCallback);
			} else {
				glfwSetKeyCallback(window, keyCallbackFunction);
			}


			// Bind FBO
			FBOs.at("postProcessing").bind();

			// Prepate rendering
			glEnable(GL_DEPTH_TEST);  
			glClearColor(SKY_COLOR.r, SKY_COLOR.g, SKY_COLOR.b, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);

			// Handle keyboard input to move the camera
			float deltaPosX = (aPressed ? -1.0f : 0) + (dPressed ? 1.0f  : 0);
			float deltaPosZ = (sPressed ? 1.0f : 0) + (wPressed ? -1.0f  : 0);
			float deltaPosY = (upPressed ? 1.0f : 0) + (downPressed ? -1.0f  : 0);
			float deltaRotY = (jPressed ? -1.5f : 0) + (lPressed ? 1.5f  : 0);
			float deltaRotX = (iPressed ? -1.5f : 0) + (kPressed ? 1.5f  : 0);
			camera.rotY += deltaRotY;
			camera.rotX += deltaRotX;
			camera.posX += (deltaPosZ * sin(glm::radians(-camera.rotY))) + (deltaPosX * cos(glm::radians(camera.rotY)));
			camera.posZ += (deltaPosZ * cos(glm::radians(-camera.rotY))) + (deltaPosX * sin(glm::radians(camera.rotY)));
			camera.posY += deltaPosY;

			// Move sun
			sunEntity->posX = SUN_POSITION_X;
			sunEntity->posY = SUN_POSITION_Y;
			sunEntity->posZ = SUN_POSITION_Z;

			// Update models shader
			modelsShader.start();
			modelsShader.setMatrix4("camera.view", camera.getViewMatrix());
			modelsShader.setMatrix4("camera.projection", camera.getProjectionMatrix());
			modelsShader.setUniform3f("sky.color", SKY_COLOR.r, SKY_COLOR.g, SKY_COLOR.b);
			modelsShader.setUniform1f("sky.density", FOG_DENSITY);
			modelsShader.setUniform3f("light.position", SUN_POSITION_X, SUN_POSITION_Y, SUN_POSITION_Z);
			modelsShader.setUniform3f("light.ambient", SUN_AMBIENT.r, SUN_AMBIENT.g, SUN_AMBIENT.b);
			modelsShader.setUniform3f("light.diffuse", SUN_DIFFUSE.r, SUN_DIFFUSE.g, SUN_DIFFUSE.b);
			modelsShader.setUniform3f("light.specular", SUN_SPECULAR.r, SUN_SPECULAR.g, SUN_SPECULAR.b);
			modelsShader.stop();

			// Updater terrain shader
			terrainShader.start();
			terrainShader.setMatrix4("camera.view", camera.getViewMatrix());
			terrainShader.setMatrix4("camera.projection", camera.getProjectionMatrix());
			terrainShader.setUniform3f("sky.color", SKY_COLOR.r, SKY_COLOR.g, SKY_COLOR.b);
			terrainShader.setUniform1f("sky.density", FOG_DENSITY);
			terrainShader.setUniform3f("light.position", SUN_POSITION_X, SUN_POSITION_Y, SUN_POSITION_Z);
			terrainShader.setUniform3f("light.ambient", SUN_AMBIENT.r, SUN_AMBIENT.g, SUN_AMBIENT.b);
			terrainShader.setUniform3f("light.diffuse", SUN_DIFFUSE.r, SUN_DIFFUSE.g, SUN_DIFFUSE.b);
			terrainShader.setUniform3f("light.specular", SUN_SPECULAR.r, SUN_SPECULAR.g, SUN_SPECULAR.b);
			terrainShader.stop();

			// Updater light source shader
			lightSourceShader.start();
			lightSourceShader.setUniform3f("sky.color", SKY_COLOR.r, SKY_COLOR.g, SKY_COLOR.b);
			lightSourceShader.setUniform1f("sky.density", FOG_DENSITY);
			lightSourceShader.setMatrix4("camera.view", camera.getViewMatrix());
			lightSourceShader.setMatrix4("camera.projection", camera.getProjectionMatrix());
			lightSourceShader.stop();

			// Render all terrains
			vaoTerrain.bind();
			for (EntityAbstract* entity : vaoTerrain.entities) {
				entity->render(vaoTerrain.indicesLength, terrainShader);
			}
			vaoTerrain.unbind();

			// Render all light sources
			vaoSphere.bind();
			for (EntityAbstract* entity : vaoSphere.entities) {
				entity->render(vaoSphere.indicesLength, lightSourceShader);
			}
			vaoSphere.unbind();

			// Now an image of the frame has been drawn to the postProcessingFBO
			// So unbind the fbo to go back to the default one.
			FBOs.at("postProcessing").unbind();

			// Nice and simple opengl calls to reset screen
			glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
			glClear(GL_COLOR_BUFFER_BIT);

			// Now do the brightness post processing
			FBOs.at("postProcessingBloom").bind(); 
			brightnessShader.start();
			brightnessShader.setUniform1f("uniformBloomThreshold", BLOOM_THRESHOLD);
			FBOs.at("postProcessing").render(); 
			brightnessShader.stop();
			FBOs.at("postProcessingBloom").unbind(); 

			// Blur using guassian blur
			FBOs.at("postProcessingBloom").bind();
			blurShader.start();
			for (int i = 0; i < BLOOM_BLUR_STRENGTH; i++) {
				int horizontal = (i % 2) ? 1.0 : 0.0;
				blurShader.setUniform1f("uniformIsHorizontal", horizontal);
				FBOs.at("postProcessingBloom").render();
			}
			blurShader.stop();
			FBOs.at("postProcessingBloom").unbind();

			// Render the final texture
			postProcessingShader.start();
			glBindVertexArray(FBOs.at("postProcessingFinal").quadVaoId);
			glDisable(GL_DEPTH_TEST);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, FBOs.at("postProcessing").texture0Id);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, FBOs.at("postProcessingBloom").texture0Id);
			glDrawArrays(GL_TRIANGLES, 0, 6); 
			postProcessingShader.stop();


			// Create ImGui frame
			ImGui_ImplGlfwGL3_NewFrame();
			ImGui::SetNextWindowSize(ImVec2(510.0f, 430.0f), 0);
			ImGui::SetNextWindowPos(ImVec2(WINDOW_WIDTH - 520.0f, 10.0f), true);
			ImGui::Begin("Customize your world");

			// Sky and lighting
			ImGui::ColorEdit3("Sky Colour", (float*)&SKY_COLOR);
			ImGui::ColorEdit3("Sun Ambient Colour", (float*)&SUN_AMBIENT);
			ImGui::ColorEdit3("Sun Specular Colour", (float*)&SUN_SPECULAR);
			ImGui::ColorEdit3("Sun Diffuse Colour", (float*)&SUN_DIFFUSE);
			ImGui::SliderFloat("Sun Position X", &SUN_POSITION_X, -300.0f, 300.0f);
			ImGui::SliderFloat("Sun Position Y", &SUN_POSITION_Y, 0.0f, 1000.0f);
			ImGui::SliderFloat("Sun Position Z", &SUN_POSITION_Z, -300.0f, 300.0f);
			if (ImGui::SliderFloat("Fog Density", &FOG_DENSITY_SLIDER, 0.0f, 100.0f)) {
				FOG_DENSITY = FOG_DENSITY_SLIDER / 10000;
			}
			ImGui::SliderFloat("Bloom Threshold", &BLOOM_THRESHOLD, 0.0f, 1.0f);
			ImGui::SliderInt("Bloom Blur Strength", &BLOOM_BLUR_STRENGTH, 0.0f, 10.0f);



			// Separator
			ImGui::Dummy(ImVec2(0.0f, 10.0f));
			ImGui::Separator();
			ImGui::Dummy(ImVec2(0.0f, 10.0f));

			// Terrain generation
			ImGui::SliderFloat("Terrain Size", &TERRAIN_SIZE, 50.0f, 1000.0f);
			ImGui::SliderFloat("Terrain Max Height", &TERRAIN_MAX_HEIGHT, 0.0f, 100.0f);
			ImGui::InputText("Terrain Height Map Path", TERRAIN_HEIGHTMAP_PATH, IM_ARRAYSIZE(TERRAIN_HEIGHTMAP_PATH));
			ImGui::InputText("Terrain Blend Map Path", TERRAIN_BLENDMAP_PATH, IM_ARRAYSIZE(TERRAIN_BLENDMAP_PATH));
			if (ImGui::Button("Regenerate Terrain")) {

				// Set new terrain generator params
				terrainGenerator.heightMapPath = TERRAIN_HEIGHTMAP_PATH;
				terrainGenerator.terrainSize = TERRAIN_SIZE;
				terrainGenerator.terrainMaxHeight = TERRAIN_MAX_HEIGHT;

				// Dispose of old terrain
				vaoTerrain.dispose();

				// Dipose of old blendmap texture
				glDeleteTextures(1, &textureBlendMap);

				// Generate the new terrain
				vaoTerrain = terrainGenerator.generateVAO();

				// Generate new blendmap texture
				textureBlendMap = TextureLoader::Load(TERRAIN_BLENDMAP_PATH);

				// Set terrain vao textures
				vaoTerrain.setTexture0(textureGrass);
				vaoTerrain.setTexture1(textureDirt);
				vaoTerrain.setTexture2(textureSnow);
				vaoTerrain.setTexture3(texturePath);
				vaoTerrain.setTexture4(textureBlendMap);

				// Add an entity to render
				vaoTerrain.addEntity(new EntityTerrain(glm::vec3(-TERRAIN_SIZE / 2.0f, 0.0f, -TERRAIN_SIZE / 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f));

			}

			ImGui::End();
			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());


			// Swap buffers
			glfwSwapBuffers(window);

			// only set lastFrameTime when you actually draw something
			lastFrameTime = now;
		}

		// set lastUpdateTime every iteration
		lastUpdateTime = now;

    }

	// Destroy resources
	vaoTerrain.dispose();
	vaoSphere.dispose();
	FBOs.at("postProcessing").dispose();
	FBOs.at("postProcessingBloom").dispose();
	FBOs.at("postProcessingFinal").dispose();

	// Destroy shaders
	modelsShader.dispose();
	terrainShader.dispose();
	lightSourceShader.dispose();
	postProcessingShader.dispose();
	blurShader.dispose();
	brightnessShader.dispose();

	// Destroy textures
	glDeleteTextures(1, &textureGrass);
	glDeleteTextures(1, &textureDirt);
	glDeleteTextures(1, &textureSnow);
	glDeleteTextures(1, &texturePath);
	glDeleteTextures(1, &textureBlendMap);


	// Destroy ImGui
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	
	// Terminate glfw
    glfwTerminate();

    return 0;
}

