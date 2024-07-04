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


    // load models
    // -----------
    Model corridor(FileSystem::getPath("resources/objects/delusional/delusional.gltf"));

    // build and compile our shader program
    // ------------------------------------
    Shader lightingShader("lighting.vs", "lighting.fs");
    
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


        Running = processInput(App, corridor, 0, stepSound,4, glm::vec3(0.0f), glm::vec3(1.0f));
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

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.setMat4("model", model);

        // don't forget to enable shader before setting uniforms
        lightingShader.use();
        
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        lightingShader.setMat4("model", model);
        corridor.Draw(lightingShader);

        
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