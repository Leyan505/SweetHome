#include "glad.h"
//#include <GLFW/glfw3.h>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/ext.hpp"

#include <filesystem.h>
#include <Headers/shader_m.h>
#include <Headers/Camera.h>
#include <Headers/model.h>

#include <iostream>


//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(sf::Window &window, const Model& modelo, int mode, sf::Sound &stepSound, glm::vec3 position , glm::vec3 scale);
bool CheckCollision(const Camera& camera, const Model& model, glm::vec3 position, glm::vec3 scale);
void resolveCollision(Camera& camera, const Model& model, glm::vec3 position, glm::vec3 scale);

// settings
sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
const unsigned int SCR_WIDTH = desktopMode.width;
const unsigned int SCR_HEIGHT = desktopMode.height;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, -16.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
   // create a window
    sf::Window window( desktopMode, "SweetHome", sf::Style::Default, sf::ContextSettings(24));
    window.setFramerateLimit(165);
    window.setActive(true);
    window.setMouseCursorGrabbed(true);
    window.setMouseCursorVisible(false);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)sf::Context::getFunction))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(false);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Shader ourShader("1.model_loading.vs", "1.model_loading.fs");

    // load models
    // -----------
    //Model exterior(FileSystem::getPath("Resources/Objects/coralineMonsterHouse/coralineMonsterHouse.gltf"));
    Model corridor(FileSystem::getPath("resources/objects/delusional/delusional.gltf"));
    //Model mae(FileSystem::getPath("Resources/Objects/circoCoraline/circo.gltf"));

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


    bool running = true;
    // render loop
    // -----------
    while (window.isOpen())
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
            processInput(window, mae, 1, maePos, maeScale);
            std::cout << "¡Colision detectada!" << std::endl;
        }*/
        //else{
            // input
            // -----
                processInput(window, corridor, 0, stepSound, glm::vec3(0.0f), glm::vec3(1.0f));
            //}
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
        lightingShader.setVec3("pointLights["+ std::to_string(sizeof(pointLightPositions)/sizeof(glm::vec3)-1) +"].ambient", 0.02f, 0.03f, 0.9f);
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

        // don't forget to enable shader before setting uniforms
        lightingShader.use();

        // view/projection transformations
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // render the loaded model
        model = glm::mat4(1.0f);
        /*model = glm::translate(model, maePos); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        mae.Draw(ourShader);*/
        
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        lightingShader.setMat4("model", model);
        corridor.Draw(lightingShader);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        /*glfwSwapBuffers(window);
        glfwPollEvents();*/
        window.display();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
            {
                running = false;
                window.close();
            }
            if (event.type == sf::Event::MouseMoved) {
                sf::Vector2i center(window.getSize().x / 2, window.getSize().y / 2);

                float xoffset = sf::Mouse::getPosition(window).x - (float)center.x;
                float yoffset = (float)center.y - sf::Mouse::getPosition(window).y;

                camera.ProcessMouseMovement(xoffset, yoffset);

                sf::Mouse::setPosition(sf::Vector2<int>((int)window.getSize().x / 2, (int)window.getSize().y / 2), window);
            }
            if (event.type == sf::Event::Resized)
            {
                // adjust the viewport when the window is resized
                glViewport(0, 0, event.size.width, event.size.height);
            }
        }
    }

    return 0;
}

void processInput(sf::Window &window, const Model& modelo, int mode, sf::Sound &stepSound, glm::vec3 position = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f))
{
    camera.Position.y = 0.0f;
    std::cout << glm::to_string(camera.Position) << std::endl;
    if (mode == 1)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        glm::vec3 front = camera.Front;
        glm::vec3 newPos = camera.Position + front * camera.MovementSpeed * deltaTime;
        if (!CheckCollision(newPos, modelo, position, scale))
        {
            camera.Position = newPos;
            stepSound.pause();
            stepSound.play();

        }
        else resolveCollision(camera, modelo, position, scale);
        
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            glm::vec3 front = camera.Front;
            glm::vec3 newPos = camera.Position - front * camera.MovementSpeed * deltaTime;
            if (!CheckCollision(newPos, modelo, position, scale))
            {
                camera.Position = newPos;
                stepSound.pause();
                stepSound.play();
            }
            else resolveCollision(camera, modelo, position, scale);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            glm::vec3 right = camera.Right;
            glm::vec3 newPos = camera.Position - right * camera.MovementSpeed * deltaTime;
            if (!CheckCollision(newPos, modelo, position, scale))
            {
                camera.Position = newPos;
                stepSound.pause();
                stepSound.play();
            }
            else resolveCollision(camera, modelo, position, scale);
        }
        if  (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            glm::vec3 right = camera.Right;
            glm::vec3 newPos = camera.Position + right * camera.MovementSpeed * deltaTime;
            if (!CheckCollision(newPos, modelo, position, scale))
            {
                camera.Position = newPos;
                stepSound.pause();
                stepSound.play();
            }
            else resolveCollision(camera, modelo, position, scale);
        }
    }
    else
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            glm::vec3 front = camera.Front;
            glm::vec3 newPos = camera.Position + front * camera.MovementSpeed * deltaTime;
            if (CheckCollision(newPos, modelo, position, scale))
            {
                camera.Position = newPos;
                stepSound.pause();
                stepSound.play();
            }
            else resolveCollision(camera, modelo, position, scale);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            glm::vec3 front = camera.Front;
            glm::vec3 newPos = camera.Position - front * camera.MovementSpeed * deltaTime;
            if (CheckCollision(newPos, modelo, position, scale))
            {
                camera.Position = newPos;
                stepSound.pause();
                stepSound.play();
            }
            else resolveCollision(camera, modelo, position, scale);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            glm::vec3 right = camera.Right;
            glm::vec3 newPos = camera.Position - right * camera.MovementSpeed * deltaTime;
            if (CheckCollision(newPos, modelo, position, scale))
            {
                camera.Position = newPos;
                stepSound.pause();
                stepSound.play();
            }
            else resolveCollision(camera, modelo, position, scale);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            glm::vec3 right = camera.Right;
            glm::vec3 newPos = camera.Position + right * camera.MovementSpeed * deltaTime;
            if (CheckCollision(newPos, modelo, position, scale))
            {
                camera.Position = newPos;
                stepSound.pause();
                stepSound.play();
            }
            else resolveCollision(camera, modelo, position, scale);
        }
    }

    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)))
        window.close();
}


// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
/*void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}*/

//funciones
 bool CheckCollision(const Camera & camera, const Model & model, glm::vec3 position = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f)) {
     // Obtenemos la posición de la cámara
     glm::vec3 cameraPos = camera.Position;

     // Iteramos sobre las mallas del modelo
     for (const auto& mesh : model.meshes) 
     {  
         // Obtenemos la posición y dimensiones de la caja de colisión de la malla
         glm::vec3 minBox = mesh.boundingBox.min;
         glm::vec3 maxBox = mesh.boundingBox.max;

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position); // translate it down so it's at the center of the scene
        model = glm::scale(model, scale);	// it's a bit too big for our scene, so scale it down
        minBox = model * glm::vec4(minBox,1.0f);
        maxBox = model * glm::vec4(maxBox, 1.0f);

        if(position == glm::vec3(0.0f))
        {
            minBox = minBox+0.1f;
            maxBox = maxBox-0.1f;
        }


         // Comprobamos si la posición de la cámara está dentro de la caja de colisión
         if (cameraPos.x >= minBox.x && cameraPos.x <= maxBox.x &&
             cameraPos.y >= minBox.y && cameraPos.y <= maxBox.y &&
             cameraPos.z >= minBox.z && cameraPos.z <= maxBox.z) 
         {
             // Si la posición de la cámara está dentro de la caja de colisión, hay colisión
             return true;
         }

     }

     // Si ninguna malla tiene colisión con la cámara, retornamos false
     return false;
 }

 void resolveCollision(Camera & camera, const Model & model, glm::vec3 position, glm::vec3 scale = glm::vec3(1.0f)) {

     // Iteramos sobre las mallas del modelo
     for (const auto& mesh : model.meshes) 
     {
        glm::vec3 minBox = mesh.boundingBox.min;
        glm::vec3 maxBox = mesh.boundingBox.max;

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position); // translate it down so it's at the center of the scene
        model = glm::scale(model, scale);	// it's a bit too big for our scene, so scale it down
        minBox = model * glm::vec4(minBox,1.0f);
        maxBox = model * glm::vec4(maxBox, 1.0f);

        
        // Resolvemos la colisión ajustando ligeramente la dirección del movimiento
        // Encuentra el vector desde la cámara hasta el punto medio de la caja de colisión
        glm::vec3 collisionVector = (minBox + maxBox) * 0.5f - camera.Position;

        // Normaliza el vector de colisión para que solo indique la dirección
        collisionVector = glm::normalize(collisionVector);

        // Ajusta la posición de la cámara para que esté justo fuera de la caja de colisión
        glm::vec3 closestPoint = glm::clamp(camera.Position, minBox, maxBox);
        camera.Position = (closestPoint + collisionVector * 0.0001f); // Deslizamiento mínimo
        break;
     }
 }