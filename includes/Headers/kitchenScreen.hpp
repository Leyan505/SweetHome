#include <iostream>
#include "cScreen.hpp"
#include "GameLogic.hpp"

class kitchenScreen : public cScreen
{
private:
public:
    kitchenScreen(void);
    virtual int Run(sf::RenderWindow &App);
};

kitchenScreen::kitchenScreen(void)
{
}

int kitchenScreen::Run(sf::RenderWindow &App)
{
    stbi_set_flip_vertically_on_load(false);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // ------------------------

    // load models
    // -----------
    // Model exterior(FileSystem::getPath("Resources/Objects/coralineMonsterHouse/coralineMonsterHouse.gltf"));
    Model kitchen(FileSystem::getPath("Resources/Objects/kitchen-room/room.gltf"));
    Model table(FileSystem::getPath("Resources/Objects/kitchen-table/table.gltf"));
    Model cake(FileSystem::getPath("Resources/Objects/kitchen-cake/cake.gltf"));
    Model fridge(FileSystem::getPath("Resources/Objects/kitchen-fridge/fridge.gltf"));
    Model door(FileSystem::getPath("Resources/Objects/kitchen-door/door.gltf"));
    Model furniture(FileSystem::getPath("Resources/Objects/kitchen-furniture/furniture.gltf"));
    Model furniture2(FileSystem::getPath("Resources/Objects/kitchen-furniture2/furniture2.gltf"));
    Model boots(FileSystem::getPath("Resources/Objects/kitchen-boots/boots.gltf"));
    Model stove(FileSystem::getPath("Resources/Objects/kitchen-stove/stove.gltf"));
    Model table2(FileSystem::getPath("Resources/Objects/kitchen-table2/table2.gltf"));
    // draw in wireframe
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // shaders de luz
    Shader lightingShader("lighting_kitchen.vs", "lighting_kitchen.fs");
    Shader lightCubeShader("light_cube.vs", "light_cube.fs");

    // positions of the point lights
    glm::vec3 pointLightPositions[] = {
        glm::vec3(3.85f, 11.0f, 17.5f),
        glm::vec3(-64.80f, 3.5f, 31.52f),
        glm::vec3(38.76f, -9.63f, -41.54f)};

    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};
    // first, configure the cube's VAO (and VBO)
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // shader configuration
    // --------------------
    lightingShader.use();
    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 1);

    sf::Clock clock;

    sf::Music music;
    sf::Music door_sound;
    sf::Music cake_sound;

    if (!music.openFromFile(FileSystem::getPath("resources/audio/end_credits.mp3")))
        return -1;
    music.setVolume(15.0f);
    music.setLoop(true);
    music.play();

    if (!door_sound.openFromFile(FileSystem::getPath("resources/audio/door_knock.mp3")))
        return -1;
    door_sound.setVolume(90.0f);
    door_sound.setLoop(true);
    // door_sound.play();

     if (!cake_sound.openFromFile(FileSystem::getPath("resources/audio/coraline_cake.mp3")))
        return -1;
    cake_sound.setVolume(90.0f);
    cake_sound.setLoop(true);
    // door_sound.play();

    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(FileSystem::getPath("resources/audio/step.mp3")))
        return -1;
    sf::Sound stepSound;
    stepSound.setBuffer(buffer);
    stepSound.setVolume(25.0f);
    int sonido_puerta = 0;

    bool Running = true;
    camera.Position = glm::vec3(45.5f, 4.0f, -6.5f);
    while (Running)
    {

        float currentFrame = static_cast<float>(clock.getElapsedTime().asSeconds());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // render
        // ------

        glm::vec3 maePos = glm::vec3(0.0f, 10.0f, 30.0f);
        glm::vec3 maeScale = glm::vec3(1.0f, 1.0f, 1.0f);

        glm::vec3 kitchenPos = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 kitchenScale = glm::vec3(1.0f, 1.0f, 1.0f);

        // y altura, z en x
        glm::vec3 stovePos = glm::vec3(-73.0f, 0.0f, -10.0f);
        glm::vec3 stoveScale = glm::vec3(1.0f, 1.0f, 1.0f);

        glm::vec3 fridgePos = glm::vec3(-74.0f, 0.0f, 17.0f);
        glm::vec3 fridgeScale = glm::vec3(1.0f, 1.0f, 1.0f);

        glm::vec3 doorPos = glm::vec3(-39.0f, 0.0f, -45.0f);
        glm::vec3 doorScale = glm::vec3(1.2f, 1.0f, 1.0f);

        glm::vec3 tablePos = glm::vec3(-4.0f, 0.0f, 26.0f);
        glm::vec3 tableScale = glm::vec3(1.0f, 1.0f, 1.0f);

        glm::vec3 tablePos2 = glm::vec3(-68.0f, 0.0f, 33.0f);
        glm::vec3 tableScale2 = glm::vec3(1.0f, 1.0f, 1.0f);

        glm::vec3 cakePos = glm::vec3(-4.0f, 0.0f, 25.0f);
        glm::vec3 cakeScale = glm::vec3(1.0f, 1.0f, 1.0f);

        glm::vec3 furniturePos = glm::vec3(0.0f, 0.0f, -35.0f);
        glm::vec3 furnitureScale = glm::vec3(1.0f, 1.0f, 1.0f);

        glm::vec3 bootsPos = glm::vec3(-11.0f, 0.0f, -45.0f);
        glm::vec3 bootsScale = glm::vec3(1.0f, 1.0f, 1.0f);

        glm::vec3 furniture2Pos = glm::vec3(67.0f, 22.0f, -1.0f);
        glm::vec3 furniture2Scale = glm::vec3(1.0f, 1.0f, 1.0f);

        if (CheckCollision(camera, fridge, fridgePos, fridgeScale))
        {
            Running = processInput(App, fridge, 1, stepSound, 2, fridgePos, fridgeScale);
            std::cout << "¡Colision detectada!" << std::endl;
        }
        else if (CheckCollision(camera, door, doorPos, doorScale))
        {
            Running = processInput(App, door, 1, stepSound, 2, doorPos, doorScale);
            std::cout << "¡Colision detectada!" << std::endl;
            door_sound.pause();
            door_sound.play();
        }
        else if (CheckCollision(camera, table, tablePos, tableScale))
        {
            Running = processInput(App, table, 1, stepSound, 2, tablePos, tableScale);
            std::cout << "¡Colision detectada!" << std::endl;
            cake_sound.pause();
            cake_sound.play();
        }
        else if (CheckCollision(camera, cake, cakePos, cakeScale))
        {
            Running = processInput(App, cake, 1, stepSound, 2, cakePos, cakeScale);
            std::cout << "¡Colision detectada!" << std::endl;
        }
        else if (CheckCollision(camera, stove, stovePos, stoveScale))
        {
            Running = processInput(App, stove, 1, stepSound, 2, stovePos, stoveScale);
            std::cout << "¡Colision detectada!" << std::endl;
        }
        else if (CheckCollision(camera, furniture, furniturePos, furnitureScale))
        {
            Running = processInput(App, furniture, 1, stepSound, 2, furniturePos, furnitureScale);
            std::cout << "¡Colision detectada!" << std::endl;
        }
        else if (CheckCollision(camera, boots, bootsPos, bootsScale))
        {
            Running = processInput(App, boots, 1, stepSound, 2, bootsPos, bootsScale);
            std::cout << "¡Colision detectada!" << std::endl;
        }
        else if (CheckCollision(camera, furniture2, furniture2Pos, furniture2Scale))
        {
            Running = processInput(App, furniture2, 1, stepSound, 2, furniture2Pos, furniture2Scale);
            std::cout << "¡Colision detectada!" << std::endl;
        }
        else if (CheckCollision(camera, table2, tablePos2, tableScale2))
        {
            Running = processInput(App, table2, 1, stepSound, 2, tablePos2, tableScale2);
            std::cout << "¡Colision detectada!" << std::endl;
        }
        else
        {
            //     // input
            //     // -----
            door_sound.pause();
            cake_sound.pause();
            Running = processInput(App, kitchen, 0, stepSound, 2, kitchenPos, kitchenScale);
        }
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
        lightingShader.setFloat("material.shininess", 40.0f);

        lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        // iluminacion del ambiente, todos los objetos lo tienen
        lightingShader.setVec3("dirLight.ambient", 0.1f, 0.1f, 0.1f);
        // iluminacion donde si tiene el objeto
        lightingShader.setVec3("dirLight.diffuse", 0.04f, 0.04f, 0.04f);
        lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
       

        lightingShader.setVec3("pointLights[" + std::to_string(0) + "].position", pointLightPositions[0]);
        // cambio de color tercero de ambiente y diffuse
        lightingShader.setVec3("pointLights[" + std::to_string(0) + "].ambient", 1.1f, 1.1f, 1.1f);
        lightingShader.setVec3("pointLights[" + std::to_string(0) + "].diffuse", 1.0f, 1.0f, 1.1f);
        lightingShader.setVec3("pointLights[" + std::to_string(0) + "].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointLights[" + std::to_string(0) + "].constant", 1.0f);
        lightingShader.setFloat("pointLights[" + std::to_string(0) + "].linear", 0.014f);
        lightingShader.setFloat("pointLights[" + std::to_string(0) + "].quadratic", 0.0007f);

        lightingShader.setVec3("pointLights[" + std::to_string(1) + "].position", pointLightPositions[1]);
        // cambio de color tercero de ambiente y diffuse
        lightingShader.setVec3("pointLights[" + std::to_string(1) + "].ambient", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("pointLights[" + std::to_string(1) + "].diffuse", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("pointLights[" + std::to_string(1) + "].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointLights[" + std::to_string(1) + "].constant", 1.0f);
        lightingShader.setFloat("pointLights[" + std::to_string(1) + "].linear", 0.022f);
        lightingShader.setFloat("pointLights[" + std::to_string(1) + "].quadratic", 0.0019f);

        lightingShader.setVec3("pointLights[" + std::to_string(2) + "].position", pointLightPositions[2]);
        // cambio de color tercero de ambiente y diffuse
        lightingShader.setVec3("pointLights[" + std::to_string(2) + "].ambient", 1.1f, 1.1f, 1.1f);
        lightingShader.setVec3("pointLights[" + std::to_string(2) + "].diffuse", 1.0f, 1.0f, 1.1f);
        lightingShader.setVec3("pointLights[" + std::to_string(2) + "].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointLights[" + std::to_string(2) + "].constant", 1.0f);
        lightingShader.setFloat("pointLights[" + std::to_string(2) + "].linear", 0.014f);
        lightingShader.setFloat("pointLights[" + std::to_string(2) + "].quadratic", 0.0007f);


        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 2000.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);
        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.setMat4("model", model);

        // we now draw as many light bulbs as we have point lights.
        glBindVertexArray(lightCubeVAO);
        for (unsigned int i = 0; i < sizeof(pointLightPositions) / sizeof(glm::vec3); i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(1.0f)); // Make it a smaller cube
            lightCubeShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // also draw the lamp object(s)
        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);

        // render the loaded model
        // glm::mat4 model = glm::mat4(1.0f);
        // model = glm::translate(model, maePos); // translate it down so it's at the center of the scene
        // model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        // ourShader.setMat4("model", model);
        // mae.Draw(ourShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, kitchenPos); // translate it down so it's at the center of the scene
        model = glm::scale(model, kitchenScale);   // it's a bit too big for our scene, so scale it down
        lightingShader.setMat4("model", model);
        kitchen.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, fridgePos); // translate it down so it's at the center of the scene
        model = glm::scale(model, fridgeScale);   // it's a bit too big for our scene, so scale it down
        lightingShader.setMat4("model", model);
        fridge.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, doorPos); // translate it down so it's at the center of the scene
        model = glm::scale(model, doorScale);   // it's a bit too big for our scene, so scale it down
        lightingShader.setMat4("model", model);
        door.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, tablePos); // translate it down so it's at the center of the scene
        model = glm::scale(model, tableScale);   // it's a bit too big for our scene, so scale it down
        lightingShader.setMat4("model", model);
        table.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, cakePos); // translate it down so it's at the center of the scene
        model = glm::scale(model, cakeScale);   // it's a bit too big for our scene, so scale it down
        lightingShader.setMat4("model", model);
        cake.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, stovePos); // translate it down so it's at the center of the scene
        model = glm::scale(model, stoveScale);   // it's a bit too big for our scene, so scale it down
        lightingShader.setMat4("model", model);
        stove.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, furniturePos); // translate it down so it's at the center of the scene
        model = glm::scale(model, furnitureScale);   // it's a bit too big for our scene, so scale it down
        lightingShader.setMat4("model", model);
        furniture.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, bootsPos); // translate it down so it's at the center of the scene
        model = glm::scale(model, bootsScale);   // it's a bit too big for our scene, so scale it down
        lightingShader.setMat4("model", model);
        boots.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, furniture2Pos); // translate it down so it's at the center of the scene
        model = glm::scale(model, furniture2Scale);   // it's a bit too big for our scene, so scale it down
        lightingShader.setMat4("model", model);
        furniture2.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, tablePos2); // translate it down so it's at the center of the scene
        model = glm::scale(model, tableScale2);   // it's a bit too big for our scene, so scale it down
        lightingShader.setMat4("model", model);
        table2.Draw(lightingShader);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        /*glfwSwapBuffers(App);
        glfwPollEvents();*/
        App.display();

        sf::Event event;
        while (App.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                Running = false;
                App.close();
            }
            if (event.type == sf::Event::MouseMoved)
            {
                sf::Vector2i center(App.getSize().x / 2, App.getSize().y / 2);

                float xoffset = sf::Mouse::getPosition(App).x - (float)center.x;
                float yoffset = (float)center.y - sf::Mouse::getPosition(App).y;

                camera.ProcessMouseMovement(xoffset, yoffset);

                sf::Mouse::setPosition(sf::Vector2<int>((int)App.getSize().x / 2, (int)App.getSize().y / 2), App);
                // mouse_callback(App, event);
            }
            if (event.type == sf::Event::Resized)
            {
                // adjust the viewport when the App is resized
                glViewport(0, 0, event.size.width, event.size.height);
            }
        }
    }
    return -1;
}