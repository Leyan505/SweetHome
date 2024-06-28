#include "glad.h"
// #include <GLFW/glfw3.h>
#include <SFML/Window.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <filesystem.h>
#include <Headers/shader_m.h>
#include <Headers/Camera.h>
#include <Headers/model.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(sf::Window &window, sf::Event event);
// void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(sf::Window &window, const Model &modelo, int mode, glm::vec3 position, glm::vec3 scale);
// funciones
bool CheckCollision(const Camera &camera, const Model &model, glm::vec3 position, glm::vec3 scale);
void resolveCollision(Camera &camera, const Model &model, glm::vec3 position, glm::vec3 scale);

// settings
// settings
sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
const unsigned int SCR_WIDTH = desktopMode.width;
const unsigned int SCR_HEIGHT = desktopMode.height;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 5.90f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{

    // create a window
    sf::Window window(desktopMode, "SweetHome", sf::Style::Default, sf::ContextSettings(24));
    window.setFramerateLimit(60);
    window.setActive(true);
    window.setMouseCursorGrabbed(true);
    window.setMouseCursorVisible(false);

    /*if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }*/
    /*glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);*/

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
    // Model exterior(FileSystem::getPath("Resources/Objects/coralineMonsterHouse/coralineMonsterHouse.gltf"));
    Model kitchen(FileSystem::getPath("Resources/Objects/coraline3/coraline.gltf"));
   // Model fridge(FileSystem::getPath("Resources/Objects/fridge/cora2.gltf"));
    // draw in wireframe
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    sf::Clock clock;

    bool running = true;
    // render loop
    // -----------
    while (window.isOpen())
    {
        // per-frame time logic
        // --------------------
        // float currentFrame = static_cast<float>(glfwGetTime());
        float currentFrame = static_cast<float>(clock.getElapsedTime().asSeconds());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // render
        // ------

        // glm::vec3 maePos = glm::vec3(0.0f, -1.5f, 0.0f);
        // glm::vec3 maeScale = glm::vec3(1.0f, 10.0f, 1.0f);

        // if (CheckCollision(camera, fridge, glm::vec3(0.0f), glm::vec3(1.0f)))
        // {
        //     processInput(window, fridge, 1, glm::vec3(0.0f), glm::vec3(1.0f));
        //     std::cout << "¡Colision detectada!" << std::endl;
        // }
        // else
        // {
        //     // input
        //     // -----
        // }

            processInput(window, kitchen, 0, glm::vec3(0.0f, 0.0f, 0.f), glm::vec3(1.0f, 1.0f, 1.0f));
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
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));     // it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        kitchen.Draw(ourShader);

        // model = glm::mat4(1.0f);
        // model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        // model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));     // it's a bit too big for our scene, so scale it down
        // ourShader.setMat4("model", model);
        // fridge.Draw(ourShader);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        /*glfwSwapBuffers(window);
        glfwPollEvents();*/
        window.display();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                running = false;
                window.close();
            }
            if (event.type == sf::Event::MouseMoved)
            {
                sf::Vector2i center(window.getSize().x / 2, window.getSize().y / 2);

                float xoffset = sf::Mouse::getPosition(window).x - (float)center.x;
                float yoffset = (float)center.y - sf::Mouse::getPosition(window).y;

                camera.ProcessMouseMovement(xoffset, yoffset);

                sf::Mouse::setPosition(sf::Vector2<int>((int)window.getSize().x / 2, (int)window.getSize().y / 2), window);
                // mouse_callback(window, event);
            }
            if (event.type == sf::Event::Resized)
            {
                // adjust the viewport when the window is resized
                glViewport(0, 0, event.size.width, event.size.height);
            }
        }
    }
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    // glfwTerminate();
    return 0;
}

// esto va en el main
void processInput(sf::Window &window, const Model &modelo, int mode, glm::vec3 position = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f))
{
    camera.Position.y = 0.0f;
    if (mode == 1)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            glm::vec3 front = camera.Front;
            glm::vec3 newPos = camera.Position + front * camera.MovementSpeed * deltaTime;
            if (!CheckCollision(newPos, modelo, position, scale))
                camera.Position = newPos;
         
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            glm::vec3 front = camera.Front;
            glm::vec3 newPos = camera.Position - front * camera.MovementSpeed * deltaTime;
            if (!CheckCollision(newPos, modelo, position, scale))
                camera.Position = newPos;
    
                
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            glm::vec3 right = camera.Right;
            glm::vec3 newPos = camera.Position - right * camera.MovementSpeed * deltaTime;
            
            if (!CheckCollision(newPos, modelo, position, scale))
                 camera.Position = newPos;
         
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            glm::vec3 right = camera.Right;
            glm::vec3 newPos = camera.Position + right * camera.MovementSpeed * deltaTime;
            
            if (!CheckCollision(newPos, modelo, position, scale))
                 camera.Position = newPos;
   
        }
    }
    else
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            glm::vec3 front = camera.Front;
            glm::vec3 newPos = camera.Position + front * camera.MovementSpeed * deltaTime;
            
             if (CheckCollision(newPos, modelo, position, scale))
                 camera.Position = newPos;
            
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            glm::vec3 front = camera.Front;
            glm::vec3 newPos = camera.Position - front * camera.MovementSpeed * deltaTime;
            
             if (CheckCollision(newPos, modelo, position, scale))
                 camera.Position = newPos;
   
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            glm::vec3 right = camera.Right;
            glm::vec3 newPos = camera.Position - right * camera.MovementSpeed * deltaTime;
            
             if (CheckCollision(newPos, modelo, position, scale))
                 camera.Position = newPos;
          
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            glm::vec3 right = camera.Right;
            glm::vec3 newPos = camera.Position + right * camera.MovementSpeed * deltaTime;
            
             if (CheckCollision(newPos, modelo, position, scale))
                camera.Position = newPos;
          
        }
    }

    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)))
        window.close();
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_callback(sf::Window &window, sf::Event event)
{
    float xpos = event.mouseMove.x;
    float ypos = event.mouseMove.y;

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
/*void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}*/

// funciones
bool CheckCollision(const Camera &camera, const Model &model, glm::vec3 position = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f))
{
    // Obtenemos la posición de la cámara
    glm::vec3 cameraPos = camera.Position;

    // Iteramos sobre las mallas del modelo
    for (const auto &mesh : model.meshes)
    {
        // Obtenemos la posición y dimensiones de la caja de colisión de la malla
        glm::vec3 minBox = mesh.boundingBox.min;
        glm::vec3 maxBox = mesh.boundingBox.max;

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position); // translate it down so it's at the center of the scene
        model = glm::scale(model, scale);        // it's a bit too big for our scene, so scale it down
        minBox = model * glm::vec4(minBox, 1.0f);
        maxBox = model * glm::vec4(maxBox, 1.0f);

        if (position == glm::vec3(0.0f))
        {
            minBox = minBox + 0.1f;
            maxBox = maxBox - 0.1f;
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

void resolveCollision(Camera &camera, const Model &model, glm::vec3 position, glm::vec3 scale = glm::vec3(1.0f))
{

    // Iteramos sobre las mallas del modelo
    for (const auto &mesh : model.meshes)
    {
        glm::vec3 minBox = mesh.boundingBox.min;
        glm::vec3 maxBox = mesh.boundingBox.max;

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position); // translate it down so it's at the center of the scene
        model = glm::scale(model, scale);        // it's a bit too big for our scene, so scale it down
        minBox = model * glm::vec4(minBox, 1.0f);
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