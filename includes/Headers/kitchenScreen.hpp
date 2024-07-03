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
    // -------------------------
    Shader ourShader("1.model_loading.vs", "1.model_loading.fs");

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

    sf::Clock clock;

	sf::Music music;
    sf::Music door_sound;

    if(!music.openFromFile(FileSystem::getPath("resources/audio/end_credits.mp3")))
        return -1;
    music.setVolume(15.0f);
    music.setLoop(true);
    music.play();

    if(!door_sound.openFromFile(FileSystem::getPath("resources/audio/door_knock.mp3")))
        return -1;
    door_sound.setVolume(90.0f);
    door_sound.setLoop(true);
    //door_sound.play();

    sf::SoundBuffer buffer;
    if(!buffer.loadFromFile(FileSystem::getPath("resources/audio/step.mp3")))
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

    //y altura, z en x
        glm::vec3 stovePos = glm::vec3(-73.0f, 0.0f, -10.0f);
        glm::vec3 stoveScale = glm::vec3(1.0f, 1.0f, 1.0f);

        glm::vec3 fridgePos = glm::vec3(-74.0f, 0.0f, 17.0f);
        glm::vec3 fridgeScale = glm::vec3(1.0f, 1.0f, 1.0f);

        glm::vec3 doorPos = glm::vec3(-39.0f, 0.0f, -45.0f);
        glm::vec3 doorScale = glm::vec3(1.2f, 1.0f, 1.0f);

        glm::vec3 tablePos = glm::vec3(-4.0f, 0.0f, 26.0f);
        glm::vec3 tableScale = glm::vec3(1.0f, 1.0f, 1.0f);

        glm::vec3 tablePos2 = glm::vec3(-49.0f, 0.0f, 27.0f);
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
            Running = processInput(App, fridge, 1,stepSound, 2, fridgePos, fridgeScale);
            std::cout << "¡Colision detectada!" << std::endl;
        }
        else if (CheckCollision(camera, door, doorPos, doorScale))
        {
            Running = processInput(App, door, 1,stepSound,2, doorPos, doorScale);
            std::cout << "¡Colision detectada!" << std::endl;
            door_sound.pause();
            door_sound.play();
        }
        else if (CheckCollision(camera, table, tablePos, tableScale))
        {
            Running = processInput(App, table, 1, stepSound,2, tablePos, tableScale);
            std::cout << "¡Colision detectada!" << std::endl;
        }
        else if (CheckCollision(camera, cake, cakePos, cakeScale))
        {
            Running = processInput(App, cake, 1,stepSound, 2,cakePos, cakeScale);
            std::cout << "¡Colision detectada!" << std::endl;
        }
        else if (CheckCollision(camera, stove, stovePos, stoveScale))
        {
            Running = processInput(App, stove, 1, stepSound,2, stovePos, stoveScale);
            std::cout << "¡Colision detectada!" << std::endl;
        }
        else if (CheckCollision(camera, furniture, furniturePos, furnitureScale))
        {
            Running = processInput(App, furniture, 1, stepSound,2, furniturePos, furnitureScale);
            std::cout << "¡Colision detectada!" << std::endl;
        }
        else if (CheckCollision(camera, boots, bootsPos, bootsScale))
        {
            Running = processInput(App, boots, 1,stepSound, 2, bootsPos, bootsScale);
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
            Running = processInput(App, kitchen, 0,stepSound,2, kitchenPos, kitchenScale);
        }
        if (!Running)
            {
                App.pushGLStates();
                App.resetGLStates(); 
                return 0;
            }
            
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // don't forget to enable shader before setting uniforms
        ourShader.use();

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 200000.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // render the loaded model
        // glm::mat4 model = glm::mat4(1.0f);
        // model = glm::translate(model, maePos); // translate it down so it's at the center of the scene
        // model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        // ourShader.setMat4("model", model);
        // mae.Draw(ourShader);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, kitchenPos); // translate it down so it's at the center of the scene
        model = glm::scale(model, kitchenScale);     // it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        kitchen.Draw(ourShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, fridgePos); // translate it down so it's at the center of the scene
        model = glm::scale(model, fridgeScale);   // it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        fridge.Draw(ourShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, doorPos); // translate it down so it's at the center of the scene
        model = glm::scale(model, doorScale);   // it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        door.Draw(ourShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, tablePos); // translate it down so it's at the center of the scene
        model = glm::scale(model, tableScale);   // it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        table.Draw(ourShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, cakePos); // translate it down so it's at the center of the scene
        model = glm::scale(model, cakeScale);   // it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        cake.Draw(ourShader);

         model = glm::mat4(1.0f);
        model = glm::translate(model, stovePos); // translate it down so it's at the center of the scene
        model = glm::scale(model, stoveScale);   // it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        stove.Draw(ourShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, furniturePos); // translate it down so it's at the center of the scene
        model = glm::scale(model, furnitureScale);   // it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        furniture.Draw(ourShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, bootsPos); // translate it down so it's at the center of the scene
        model = glm::scale(model, bootsScale);   // it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        boots.Draw(ourShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, furniture2Pos); // translate it down so it's at the center of the scene
        model = glm::scale(model, furniture2Scale);   // it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        furniture2.Draw(ourShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, tablePos2); // translate it down so it's at the center of the scene
        model = glm::scale(model, tableScale2);   // it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        table2.Draw(ourShader);

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