#include <iostream>
#include "cScreen.hpp"
#include "GameLogic.hpp"

class bedRoomScreen : public cScreen
{
private:
public:
    bedRoomScreen(void);
    virtual int Run(sf::RenderWindow &App);
};

bedRoomScreen::bedRoomScreen(void)
{
}

int bedRoomScreen::Run(sf::RenderWindow &App)
{
    stbi_set_flip_vertically_on_load(false);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Shader ourShader("1.model_loading.vs", "1.model_loading.fs");
    // load the main model of Bedroom
    Model Bedroom(FileSystem::getPath("Resources/Objects/BED-ROOM/BED-BedRoom.gltf"));
    // only load  the woods models for the collision
    Model Bed_Wood(FileSystem::getPath("Resources/Objects/ROOM-BED/ROOM-BED.gltf"));

    sf::Clock clock;

    sf::Music music1;
    sf::Music music2;
    // sf::Music music3;
    // sf::Music music4;
    if (!music1.openFromFile(FileSystem::getPath("resources/audio/CoralineSoundtrack1.mp3")))
        return 1;
    music1.setPosition(0, 1, 20);
    music1.setVolume(10.0f);
    music1.setLoop(true);
    music1.play();
    //Time beetween sound  (seconds)
    float timeBetweenSounds = 20.0f; // main music

    sf::sleep(sf::seconds(timeBetweenSounds));

    if (!music2.openFromFile(FileSystem::getPath("resources/audio/MothersWhistle.mp3")))
        return -1;
    music2.setPosition(0, 1, 10);
    music2.setVolume(70.0f); // adjust the volume
    music2.setLoop(true);
    music2.play();
    //sf::sleep(sf::seconds(timeBetweenSounds));
    music2.stop();

    // sf::sleep(sf::seconds(timeBetweenSounds - 5.0f));
    // if (!music3.openFromFile(FileSystem::getPath("resources/audio/motherschatting.mp3")))
    //     return -1;
    // music3.setPosition(0, 1, 10);
    // music3.setVolume(70.0f); // ajust the volume
    // music3.setLoop(true);
    // music3.play();
    // sf::sleep(sf::seconds(timeBetweenSounds));
    // music3.stop();

    // sf::sleep(sf::seconds(timeBetweenSounds - 5.0f));
    // if (!music3.openFromFile(FileSystem::getPath("resources/audio/WelcomeHome.mp3")))
    //     return -1;
    // music3.setPosition(0, 1, 10);
    // music3.setVolume(70.0f); // ajust the volume
    // music3.setLoop(true);
    // music3.play();
    // sf::sleep(sf::seconds(timeBetweenSounds));
    // music3.stop();


    // std::vector<std::string> musicFiles {
    //     (FileSystem::getPath("resources/audio/MothersWhistle.mp3")),
    //     (FileSystem::getPath("resources/audio/Motherschatting.mp3")),
    //     (FileSystem::getPath("resources/audio/FathersSong.mp3")),
    //     (FileSystem::getPath("resources/audio/welcomeHome.mp3"))};

    // std::vector<sf::Music> musics;

    // // Cargar cada música en el vector musics
    // for (const auto &file : musicFiles)
    // {
    //     sf::Music music;
    //     if (!music.openFromFile(file))
    //     {
    //         std::cerr << "Error al cargar el archivo: " << file << std::endl;
    //         continue; // O maneja el error de alguna otra manera
    //     }
    //     music.setVolume(15.0f);
    //     music.setLoop(true);
    //     musics.push_back(music);
    //     sf::sleep(sf::seconds(timeBetweenSounds));
    // }
    // if (!musics.empty())
    //     musics[0].play(); // Reproducir la primera música del vector

    // Main Loop
    while (true)
    {
        sf::SoundBuffer buffer;
        if (!buffer.loadFromFile(FileSystem::getPath("resources/audio/step.mp3")))
            return -1;
        sf::Sound stepSound;
        stepSound.setBuffer(buffer);
        stepSound.setVolume(25.0f);

        bool Running = true;

        camera.Position = glm::vec3(-169.205902, 40.000000, -53.526787);

        while (Running)
        {
            float currentFrame = static_cast<float>(clock.getElapsedTime().asSeconds());
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            // render
            // ------

            glm::vec3 maePos = glm::vec3(0.0f, 0.0f, 0.0f);
            glm::vec3 maeScale = glm::vec3(1.0f, 1.0f, 1.0f);
            glm::vec3 WoodPos = glm::vec3(0.0f, 0.0f, 0.0f);
            glm::vec3 WoodScale = glm::vec3(1.0f, 1.0f, 1.0f);

            if (CheckCollision(camera, Bed_Wood, WoodPos, WoodScale))
            {
                Running = processInput(App, Bed_Wood, 1, stepSound, 3, WoodPos, WoodScale);
                std::cout << "¡Colision detectada!" << std::endl;
            }

            else
            {
                //     // input
                //     // -----
                Running = processInput(App, Bedroom, 0, stepSound, 3, maePos, maeScale);
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
            // chili.Draw(ourShader);

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::mat4(1.0f);
            model = glm::translate(model, maePos); // translate it down so it's at the center of the scene
            model = glm::scale(model, maeScale);   // it's a bit too big for our scene, so scale it down
            ourShader.setMat4("model", model);
            Bed_Wood.Draw(ourShader);

            model = glm::mat4(1.0f);
            model = glm::translate(model, maePos); // translate it down so it's at the center of the scene
            model = glm::scale(model, maeScale);   // it's a bit too big for our scene, so scale it down
            ourShader.setMat4("model", model);
            Bedroom.Draw(ourShader);
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
}