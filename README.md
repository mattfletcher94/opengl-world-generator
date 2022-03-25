
# 2020-COMP3016-Coursework2 - World Generator  
![enter image description here](https://i.ibb.co/xmFM7Ds/World-1.png)


### Table of Contents  
- [1.0 Description and Project Background](#10-description-and-project-background)  
- [2.0 Interaction](#20-interaction)  
- [3.0 Developer Insights](#30-developer-insights)  
- [4.0 Libraries and acknowledgements](#40-libraries-and-acknowledgements)
- [5.0 Original Project Proposal](#50-original-project-proposal)

### 1.0 Description and Project Background
Written using C++ and OpenGL, the world generator allows you to generate a small and simple world by providing a grayscale height-map and an RGB blend-map.

I was inspired by the procedural generation in the game Diablo III. They use this technique to generate dungeons in which the player can explore.

I started with a Visual Studio project which set up the groundwork for working with C++ and OpenGL. The project came with GLFW and GLEW pre-installed and that was all. 

My project is nothing like Diablo III, but it has set the groundwork for an island to be procedural generated. Currently the user provides the application with a height-map and a blend-map. If this were a game, these resources could be generated at run time by using clever algorithms, as after all, they are just made up of RGB colours.

Many simple OpenGL projects use a global light which radiates in all directions to light the scene. I made my application different by implementing a point light with an attenuation factor. This provides the illusion of a spotlight pointing down at the island, getting weaker as the light is positioned further away.

**Note**: The program was ran on a Microsoft Windows 10 Pro, 64-Bit computer, and compiled using Microsoft Visual Studio Enterprise 2019. The program has not been tested on any computer except my own.

Please feel free to check out the video report that demos and evaluates this project:
[https://www.youtube.com/watch?v=7nspIj7_fQo](https://www.youtube.com/watch?v=7nspIj7_fQo)

### 2.0 Interaction
To run the application, you may open the Visual Studio Solution (double click the Cousework2.sln file) and then run the application by pressing the run button (green arrow with the text Local Windows Debugger).  You may also navigate to the x64/Release/ folder and double click on the Coursework2.exe to run the program. Please take note that if you make changes to the application and regenerate the release build in Visual Studio, it's important to manually copy over the 'res' and 'shaders' folders to the x64/Release folder before running the exe file.

After generating your world, you may want explore it by positioning the camera. Use the W, A, S and D keys to move the camera, the I, J, K and L keys to rotate the camera, and the up and down arrow keys to increase or decrease the height of the camera.

The application provides a set of controls on the right-hand side of the screen to customize the environment. 
![enter image description here](https://i.ibb.co/XjpG8T5/World-Controls.png)

#### 2.1 Interaction - Lighting
The top half of the controls are focused on environmental factors such as lighting and bloom. The descriptions of each control are listed below:

 - **Sky Colour**: The sky colour is set using a colour picker which returns an RGB value.
 - **Sun Ambient Colour**: Use the colour picker to set the ambient colour radiated from the sun. 
 - **Sun Specular Colour**: Use the colour picker to set the specular colour radiated from the sun. 
 - **Sun Diffuse Colour**: Use the colour picker to set the diffuse colour radiated from the sun. 
 - **Sun Position X**: Drag the slider with your mouse to adjust the x position of the sun's origin.
 - **Sun Position Y**: Drag the slider with your mouse to adjust the y position of the sun's origin.
 -  **Sun Position Z**: Drag the slider with your mouse to adjust the z position of the sun's origin.
 -  **Fog Density**: Drag the slider to increase or decrease the amount of fog in your world. Fog takes on the colour of the sky.
 -  **Bloom Threshold**: Decrease this value to allow darker pixels to be affected by the bloom effect. Having this value at 1.0 will stop all bloom effects.
 -  **Bloom Blur Strength**: Increasing this value will increase the effectiveness of bloom by increasing the amount of gaussian blur applied to the scene.
 - 
#### 2.1 Interaction - Terrain Generation
The bottom half of the controls are focused on building and generating a new terrain. ***Note that after making changes in this section, you must hit the 'Regenerate Terrain' button***. The descriptions of each control are listed below:
 -  **Terrain Size**: Drag the slider with your mouse to adjust the size of your terrain.
 -  **Terrain Max Height**: This is the maximum height your terrain can reach (higher max height = higher hills).
 -  **Terrain Height Map Path**: Enter the relative path to your grayscale height-map. The height-map image size must be to the power of 2 (e.g., 128x128, 256x256). The darker parts of your height-map will have a low height, whereas the brighter parts will be higher.
 -  **Terrain Blend Map Path**: Enter the relative path to your blend-map. The blend-map image size must be to the power of 2 (e.g., 128x128, 256x256). The blend-map should be formatted as follows: 
 -- Black (#000000) will be rendered as grass.
 -- Blue (#0000ff) will be rendered as a path.
 -- Red (#ff0000) will be rendered as dirt.
 -- Green (#00ff00) will be rendered as snow.
 -- Colours in between will be rendered as a mixture. E.g., a colour between red and green (yellow) will be rendered as 'dirty snow'
 -  **Regenerate Terrain Button**: Once you have changed the fields above, you must click the Regenerate Terrain button to see your changes.

### 3.0 Developer Insights
Throughout this section, I will walk you through some of the choices I made in regard to the software development. 

#### 3.1 Render Engine
As with any OpenGL application, it's important to create an efficient rendering engine. One that can run with a stable frame rate and one that carefully considers memory management.

The render engine is built around three main classes - the VAO class, the abstract Entity class, and the Concrete Entity class. With these, a batch rendering system can be used that reduces the number of bind calls made to OpenGL.

![Render Engine UML](https://i.ibb.co/6vCp2D0/UML-Render-Engine.png)

Firstly, a VAO can be created by either passing through a path to an OBJ file, or passing through the raw data (vertices, texture coords, indices and normals). 

Secondly, a concrete entity class must be implemented by extending the EntityAbstract class and implementing the render function. Now separate entities can be created with a unique position, rotation and scale.

Finally, instances of the entity can now be added to the VAO to be rendered. For example, to render a specific VAO and Entity the following code would be used:
```cpp
// Outside render loop
VAO myVAO = VAO("file.obj"); // Create the VAO
myVAO.setTexture(textureId) // Add a texture to this VAO
myVAO.addEntity(new myConcreteEntity(pos, rot, scale)); // Add an entity 

// Inside render loop
myVAO.bind();
for (EntityAbstract* entity : myVAO.entities) {
	entity->render(myVAO.indicesLength, myShaderProgram);
}
myVAO.unbind();
```
The code above will first bind the VAO, then iterate through all entities assigned to that VAO and call the render function.

#### 3.2 Terrain Generator
The terrain generator generates a terrain by using a heightmap to calculate the vertices, indices, normals and texture coords. I'm happy with how the height maps work in my application. 

The blend-map functionality however, could be improved. Currently the user is limited to the textures I have provided (grass, dirt, path & snow). The project could be improved by allowing the user to add and set their own textures to the blend map. This would allow the user to customize their world further.


#### 3.3 Lighting
I decided to use a point light in this application rather than a global light source. This gives the illusion of a spotlight shining on the world. The light also has an attenuation factor which makes the scene darker as the light moves further away. 

#### 3.4 Post Processing and Frame Buffer Objects
Frame buffer objects (FBOs) allowed the scene to be rendered to a 2D image which can be used for post processing effects such as a black and white filter or bloom. To help with the creation and deletion of FBOs, an FBO class was created.
![FBO Class](https://i.ibb.co/JvSvkLy/UML-FBO.png)

To create the bloom effect, three FBOs were required. Firstly, the original scene is rendered onto the first FBO. Next, the first FBO is rendered onto the second FBO after being passed through a brightness shader. This shader only renders pixels over a certain brightness threshold. Next, the second FBO, which only contains the bright pixels, is passed through a gaussian blur shader and then rendered onto its self. 
Now we're left with a 2D texture of the bright pixels with gaussian blur applied (bloom). Finally, the bloom texture is rendered over the top of the original scene texture.

I could've improved the bloom effect by adding HDR support into my application. Currently pixels cannot have a higher brightness than 1.0, this means that textures such as the path texture can be affected by bloom which is not always the expected behaviour. With HDR support they can surpass this value, meaning only really bright pixels would be affected by bloom.

#### 3.5 Memory Management
It's very important to free up resources when they're not being used. The VAO class, ShaderProgram class and FBO class all have dispose methods which delete all their resources from memory. Also before closing the application, all textures are deleted and finally the glfwTerminate function is called.

### 4.0 Libraries and acknowledgements

I used this [OBJ Loader Library](https://github.com/Bly7/OBJ-Loader) to load OBJ files. I am using the library strictly for educational purposes.

I have also used the [imgui c++ library](https://github.com/ocornut/imgui) for the GUI based stuff in the application. I am using the library strictly for educational purposes.

I give credit to https://learnopengl.com/Advanced-Lighting/Bloom for giving me a good understanding of how to implement bloom into my application, and providing the gaussian blur algorithm.

### 5.0 Original Project Proposal
This is my original project proposal.
#### 5.1 Title
Procedural Generated Island

#### 5.2 Description
This project will be focused on randomly generating a small floating island. I would like to use the knowledge I learnt about file reading from coursework one to read in a height map and then process it to help generate the island. There will be a star that radiates light from the sky, with UI controls to change the light settings in real time. 

#### 5.3 Libraries
I will be using ImGUI for easy UI building.

*Later down the line, I added the OBJ_LOADER library as mentioned above.*

#### 5.4 User Interaction
The user, with the mouse, will be able to pan, rotate and zoom on the island to view it. They will interact with basic UI controls to change the light settings.

*I ended up using the keyboard instead of the mouse for user interaction* 

#### 5.5 Updates:
After submitting my original proposal, Swen gave me a response and I answered with the following:

##### 5.5.1 Swen said:

*Given the task it might be good to look into assigning textures to the heightmap as well or include bloom for the light. To go for a more advanced version once the mvp works, it would be good to be able to load/generate different islands during runtime as well.*

##### 5.5.2 Matt said:
*I agree that adding textures to the island will be a good idea. I will definitely investigate adding bloom after I have completed the MVP. Generating different islands at run time is also something I would like to add to my project.*