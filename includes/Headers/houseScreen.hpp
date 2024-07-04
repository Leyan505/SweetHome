#include "glad.h"
#include "cScreen.hpp"
#include "GameLogic.hpp"

class houseScreen : public cScreen
{
    private:
        bool first;
    public:
        houseScreen(void);
	    virtual int Run(sf::RenderWindow &App);
};
houseScreen::houseScreen(void)
{
    first = true;
}
int houseScreen::Run(sf::RenderWindow &App)
{
	// tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(false);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);


    // load models
    // -----------
    Model house(FileSystem::getPath("resources/objects/coralineMonsterHouse/coralineMonsterHouse.gltf"));
    
    
    // build and compile shaders
    // -------------------------
    Shader modelShader("1.model_loading.vs", "1.model_loading.fs");
    Shader lightingShader("lightingHouse.vs", "lightingHouse.fs");
    Shader skyboxShader("skybox.vs", "skybox.fs");
    
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    // positions of the point lights
    glm::vec3 pointLightPositions[] = {
        glm::vec3( -20.37f,  82.989f,  -24.94f),
        glm::vec3( 38.35f, 84.08f, -2.44f),
        glm::vec3(-43.38f,  81.17f, -55.96f),
        glm::vec3( -43.38f,  43.488f, -56.39f),
        glm::vec3( -74.20f,  75.27f, -91.09f),
        glm::vec3( -74.20f,  41.57f, -91.35f),
        glm::vec3( -27.54f, 75.50f, -110.18f),
        glm::vec3( -27.54f, 41.51f, -109.77f),
        glm::vec3( 5.79f, 75.86f, -110.54f),
        glm::vec3( 5.79f, 41.62f, -109.32f),
        glm::vec3( 91.79f, 92.59f, -72.72f),
        glm::vec3( 7.23f, 47.18f, 2.06f),
        glm::vec3( -89.62f, 37.36f, -16.50f),
        glm::vec3( -118.76f, 30.42f, 6.63f),
        glm::vec3( -185.5919f, 83.19f, -48.16f),
        glm::vec3( -89.53f, 71.62f, -25.89f)
        
    };
    // skybox VAO
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // load textures
    // -------------
    vector<std::string> faces
    {
        FileSystem::getPath("resources/textures/skybox/right.png"),
        FileSystem::getPath("resources/textures/skybox/left.png"),
        FileSystem::getPath("resources/textures/skybox/top.png"),
        FileSystem::getPath("resources/textures/skybox/bottom.png"),
        FileSystem::getPath("resources/textures/skybox/front.png"),
        FileSystem::getPath("resources/textures/skybox/back.png")
    };
    unsigned int cubemapTexture = loadCubemap(faces);

    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

    // shader configuration
    // --------------------
    lightingShader.use();
    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 1);

    sf::Clock clock;

    sf::Music music;
    if(!music.openFromFile(FileSystem::getPath("resources/audio/dreamsAreDangerous.mp3")))
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

    bool Running = true;
    float rot = 0.0f;
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
        camera.Position = glm::vec3(-24.00f, 121.68f, 450.07f);
        camera.Front = glm::vec3(0.078f, -0.20f, -0.97f);
        camera.Up = glm::vec3(-0.06f, 0.88f, 0.47f);


        Running = processInput(App, house, 0, stepSound, 5, glm::vec3(0.0f), glm::vec3(1.0f));
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

        lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        lightingShader.setVec3("dirLight.ambient", 0.01f, 0.1f, 0.1f);
        lightingShader.setVec3("dirLight.diffuse", 0.2f, 0.2f, 0.2f);
        lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

        for (int i = 0; i < (sizeof(pointLightPositions)/sizeof(glm::vec3))-1; i++)
        {
            if(i != 11){
                lightingShader.setVec3("pointLights["+ std::to_string(i) +"].position", pointLightPositions[i]);
                lightingShader.setVec3("pointLights["+ std::to_string(i) +"].ambient", 0.02f, 0.03f, 0.05f);
                lightingShader.setVec3("pointLights["+ std::to_string(i) +"].diffuse", 0.3f, 0.3f, 0.5f);
                lightingShader.setVec3("pointLights["+ std::to_string(i) +"].specular", 1.0f, 1.0f, 1.0f);
                lightingShader.setFloat("pointLights["+ std::to_string(i) +"].constant", 1.0f);
                lightingShader.setFloat("pointLights["+ std::to_string(i) +"].linear", 0.007f);
                lightingShader.setFloat("pointLights["+ std::to_string(i) +"].quadratic", 0.0002f);
            }
        }
        
        lightingShader.setVec3("pointLights["+ std::to_string(11) +"].position", pointLightPositions[11]);
        float random = (rand()) / static_cast <float> (RAND_MAX);
        lightingShader.setVec3("pointLights["+ std::to_string(11) +"].ambient", 1.0f, 1.0f, random);
        lightingShader.setVec3("pointLights["+ std::to_string(11) +"].diffuse", 0.3f, 0.3f, 0.5f);
        lightingShader.setVec3("pointLights["+ std::to_string(11) +"].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointLights["+ std::to_string(11) +"].constant", 1.0f);
        lightingShader.setFloat("pointLights["+ std::to_string(11) +"].linear", 0.007f);
        lightingShader.setFloat("pointLights["+ std::to_string(11) +"].quadratic", 0.0002f);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    
        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        // don't forget to enable shader before setting uniforms
        lightingShader.use();

        // view/projection transformations
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 2000.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);
        
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        model = glm::rotate(model, glm::radians(0.0f+rot), glm::vec3(0.0, 1.0f, 0.0f));
        rot+=0.1f;
        if(rot>360.0f) {rot = 0.0f;}
        lightingShader.setMat4("model", model);
        house.Draw(lightingShader);

        // draw skybox as last
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        skyboxShader.use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);
        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); 
        // set depth function back to default
        
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

