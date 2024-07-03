#include "glad.h"
#include "cScreen.hpp"
#include "GameLogic.hpp"

class corridorScreen : public cScreen
{
    private:
        bool first;
    public:
        corridorScreen(void);
	    virtual int Run(sf::RenderWindow &App);
};
corridorScreen::corridorScreen(void)
{
    first = true;
}
int corridorScreen::Run(sf::RenderWindow &App)
{
	// tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(false);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    //Shader ourShader("1.model_loading.vs", "1.model_loading.fs");

    // load models
    // -----------
    //Model exterior(FileSystem::getPath("Resources/Objects/coralineMonsterHouse/coralineMonsterHouse.gltf"));
    Model corridor(FileSystem::getPath("resources/objects/delusional/delusional.gltf"));
   // Model mae(FileSystem::getPath("Resources/Objects/table/table.gltf"));

    // draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
     // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("lighting.vs", "lighting.fs");
    Shader lightCubeShader("light_cube.vs", "light_cube.fs");

    // positions of the point lights
    glm::vec3 pointLightPositions[] = {
        glm::vec3( 0.85f,  0.74f,  -7.93f),
        glm::vec3( -1.09f, 0.74f, -4.70f),
        glm::vec3(0.85f,  0.74f, -0.25f),
        glm::vec3( -1.09f,  0.74f, 4.16f),
        glm::vec3( 0.85f,  0.74f, 8.80f),
        glm::vec3( -0.09f,  0.74f, -16.98f),
        glm::vec3( -0.09f,  0.0f, 18.63f)
    };
    // first, configure the cube's VAO (and VBO)
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // shader configuration
    // --------------------
    lightingShader.use();
    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 1);

    sf::Clock clock;

    sf::Music music;
    if(!music.openFromFile(FileSystem::getPath("resources/audio/explorationCoraline.mp3")))
        return 1;
    music.setVolume(25.0f);
    music.setLoop(true);
    music.play();

    sf::SoundBuffer buffer;
    if(!buffer.loadFromFile(FileSystem::getPath("resources/audio/step.mp3")))
        return -1;
    sf::Sound stepSound;
    stepSound.setBuffer(buffer);
    stepSound.setVolume(25.0f);

    srand((unsigned) time(NULL)); 

    camera.Position = glm::vec3(0.0f, 0.0f, -16.0f);

    bool Running = true;

	while (Running)
	{
		// per-frame time logic
        // --------------------
        //float currentFrame = static_cast<float>(glfwGetTime());
         float currentFrame = static_cast<float>(clock.getElapsedTime().asSeconds());
         deltaTime = currentFrame - lastFrame;
         lastFrame = currentFrame;

        // render
        // ------
        
        glm::vec3 maePos = glm::vec3(0.0f, -1.5f, 0.0f);
        glm::vec3 maeScale = glm::vec3(1.0f, 10.0f, 1.0f);

       /*if (CheckCollision(camera, mae, maePos, maeScale)) {
            processInput(window, mae, 1, stepSound, maePos, maeScale);
            std::cout << "¡Colision detectada!" << std::endl;
        }
        else{*/
            // input
            // -----
                Running = processInput(App, corridor, 0, stepSound,4, glm::vec3(0.0f), glm::vec3(1.0f));
            //}
            if (!Running)
            {
                App.pushGLStates();
                App.resetGLStates(); 
                return 0;
            }
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);
        lightingShader.setFloat("material.shininess", 32.0f);

        /*
           Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index 
           the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
           by defining light types as classes and set their values in there, or by using a more efficient uniform approach
           by using 'Uniform buffer objects', but that is something we'll discuss in the 'Advanced GLSL' tutorial.
        */
        // directional light
        lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        lightingShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("dirLight.diffuse", 0.04f, 0.04f, 0.04f);
        lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        for (int i = 0; i < (sizeof(pointLightPositions)/sizeof(glm::vec3))-1; i++)
        {
            lightingShader.setVec3("pointLights["+ std::to_string(i) +"].position", pointLightPositions[i]);
            lightingShader.setVec3("pointLights["+ std::to_string(i) +"].ambient", 0.02f, 0.03f, 0.05f);
            lightingShader.setVec3("pointLights["+ std::to_string(i) +"].diffuse", 0.3f, 0.3f, 0.5f);
            lightingShader.setVec3("pointLights["+ std::to_string(i) +"].specular", 1.0f, 1.0f, 1.0f);
            lightingShader.setFloat("pointLights["+ std::to_string(i) +"].constant", 1.0f);
            lightingShader.setFloat("pointLights["+ std::to_string(i) +"].linear", 0.14f);
            lightingShader.setFloat("pointLights["+ std::to_string(i) +"].quadratic", 0.07f);
        }
        
        lightingShader.setVec3("pointLights["+ std::to_string(sizeof(pointLightPositions)/sizeof(glm::vec3)-1) +"].position", pointLightPositions[sizeof(pointLightPositions)/sizeof(glm::vec3)-1]);
        float random = (rand()) / static_cast <float> (RAND_MAX);
        lightingShader.setVec3("pointLights["+ std::to_string(sizeof(pointLightPositions)/sizeof(glm::vec3)-1) +"].ambient", 0.02f, 0.03f, random);
        lightingShader.setVec3("pointLights["+ std::to_string(sizeof(pointLightPositions)/sizeof(glm::vec3)-1) +"].diffuse", 0.3f, 0.3f, 0.5f);
        lightingShader.setVec3("pointLights["+ std::to_string(sizeof(pointLightPositions)/sizeof(glm::vec3)-1) +"].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointLights["+ std::to_string(sizeof(pointLightPositions)/sizeof(glm::vec3)-1) +"].constant", 1.0f);
        lightingShader.setFloat("pointLights["+ std::to_string(sizeof(pointLightPositions)/sizeof(glm::vec3)-1) +"].linear", 0.14f);
        lightingShader.setFloat("pointLights["+ std::to_string(sizeof(pointLightPositions)/sizeof(glm::vec3)-1) +"].quadratic", 0.07f);

        // spotLight
        lightingShader.setVec3("spotLight.position", camera.Position);
        lightingShader.setVec3("spotLight.direction", camera.Front);
        lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("spotLight.constant", 1.0f);
        lightingShader.setFloat("spotLight.linear", 0.09f);
        lightingShader.setFloat("spotLight.quadratic", 0.032f);
        lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));     

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.setMat4("model", model);

        // also draw the lamp object(s)
         lightCubeShader.use();
         lightCubeShader.setMat4("projection", projection);
         lightCubeShader.setMat4("view", view);
    
        /* // we now draw as many light bulbs as we have point lights.
         glBindVertexArray(lightCubeVAO);
         for (unsigned int i = 0; i < sizeof(pointLightPositions)/sizeof(glm::vec3); i++)
         {
             model = glm::mat4(1.0f);
             model = glm::translate(model, pointLightPositions[i]);
             model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
             lightCubeShader.setMat4("model", model);
             glDrawArrays(GL_TRIANGLES, 0, 36);
         }*/

        // don't forget to enable shader before setting uniforms
        lightingShader.use();
        
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        lightingShader.setMat4("model", model);
        corridor.Draw(lightingShader);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        /*glfwSwapBuffers(window);
        glfwPollEvents();*/
        
        App.display();

        sf::Event event;
		while (App.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
            {
                return -1;
            }
            if (event.type == sf::Event::MouseMoved) {
                sf::Vector2i center(App.getSize().x / 2, App.getSize().y / 2);

                float xoffset = sf::Mouse::getPosition(App).x - (float)center.x;
                float yoffset = (float)center.y - sf::Mouse::getPosition(App).y;

                camera.ProcessMouseMovement(xoffset, yoffset);

                sf::Mouse::setPosition(sf::Vector2<int>((int)App.getSize().x / 2, (int)App.getSize().y / 2), App);
            }
            if (event.type == sf::Event::Resized)
            {
                // adjust the viewport when the window is resized
                glViewport(0, 0, event.size.width, event.size.height);
            }
		}
	}

	//Never reaching this point normally, but just in case, exit the application
	return -1;
}