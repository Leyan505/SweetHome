#include "glad.h"
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
#include <SFML/Graphics.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(sf::Window &window, sf::Event event);
void processInput(sf::Window &window, const Model& modelo, int mode, glm::vec3 position, glm::vec3 scale);
bool CheckCollision(const Camera& camera, const Model& model, glm::vec3 position, glm::vec3 scale);
void resolveCollision(Camera& camera, const Model& model, glm::vec3 position, glm::vec3 scale);

sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
const unsigned int SCR_WIDTH = desktopMode.width;
const unsigned int SCR_HEIGHT = desktopMode.height;

Camera camera(glm::vec3(0.0f, 0.0f, 5.90f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;


int main()
{
    sf::Window window(desktopMode, "SweetHome", sf::Style::Fullscreen, sf::ContextSettings(24));

    window.setFramerateLimit(60);
    window.setActive(true);
    window.setMouseCursorGrabbed(true);
    window.setMouseCursorVisible(false);

    if (!gladLoadGLLoader((GLADloadproc)sf::Context::getFunction))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    stbi_set_flip_vertically_on_load(false);

    glEnable(GL_DEPTH_TEST);

    Shader ourShader("1.model_loading.vs", "1.model_loading.fs");

    Model room(FileSystem::getPath("Resources/Objects/living-room/scene.gltf"));

    sf::Clock clock;

    bool running = true;

    while (window.isOpen())
    {
        float currentFrame = static_cast<float>(clock.getElapsedTime().asSeconds());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;



            processInput(window, room, 0, glm::vec3(0.0f), glm::vec3(1.0f));
    

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.use();

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 200000.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        ourShader.setMat4("model", model);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        ourShader.setMat4("model", model);
        room.Draw(ourShader);

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
            }
            if (event.type == sf::Event::Resized)
            {
                glViewport(0, 0, event.size.width, event.size.height);
            }
        }
    }
    return 0;
}

void processInput(sf::Window &window, const Model& modelo, int mode, glm::vec3 position, glm::vec3 scale)
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
            else
                resolveCollision(camera, modelo, position, scale);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            glm::vec3 front = camera.Front;
            glm::vec3 newPos = camera.Position - front * camera.MovementSpeed * deltaTime;
            if (!CheckCollision(newPos, modelo, position, scale))
                camera.Position = newPos;
            else
                resolveCollision(camera, modelo, position, scale);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            glm::vec3 right = camera.Right;
            glm::vec3 newPos = camera.Position - right * camera.MovementSpeed * deltaTime;
            if (!CheckCollision(newPos, modelo, position, scale))
                camera.Position = newPos;
            else
                resolveCollision(camera, modelo, position, scale);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            glm::vec3 right = camera.Right;
            glm::vec3 newPos = camera.Position + right * camera.MovementSpeed * deltaTime;
            if (!CheckCollision(newPos, modelo, position, scale))
                camera.Position = newPos;
            else
                resolveCollision(camera, modelo, position, scale);
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
            else
                resolveCollision(camera, modelo, position, scale);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            glm::vec3 front = camera.Front;
            glm::vec3 newPos = camera.Position - front * camera.MovementSpeed * deltaTime;
            if (CheckCollision(newPos, modelo, position, scale))
                camera.Position = newPos;
            else
                resolveCollision(camera, modelo, position, scale);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            glm::vec3 right = camera.Right;
            glm::vec3 newPos = camera.Position - right * camera.MovementSpeed * deltaTime;
            if (CheckCollision(newPos, modelo, position, scale))
                camera.Position = newPos;
            else
                resolveCollision(camera, modelo, position, scale);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            glm::vec3 right = camera.Right;
            glm::vec3 newPos = camera.Position + right * camera.MovementSpeed * deltaTime;
            if (CheckCollision(newPos, modelo, position, scale))
                camera.Position = newPos;
            else
                resolveCollision(camera, modelo, position, scale);
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        window.close();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
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
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

bool CheckCollision(const Camera & camera, const Model & model, glm::vec3 position, glm::vec3 scale)
{
    glm::vec3 cameraPos = camera.Position;

    for (const auto& mesh : model.meshes) 
    {  
        glm::vec3 minBox = mesh.boundingBox.min;
        glm::vec3 maxBox = mesh.boundingBox.max;

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::scale(model, scale);
        minBox = model * glm::vec4(minBox, 1.0f);
        maxBox = model * glm::vec4(maxBox, 1.0f);

        if (position == glm::vec3(0.0f))
        {
            minBox = minBox + 0.1f;
            maxBox = maxBox - 0.1f;
        }

        if (cameraPos.x >= minBox.x && cameraPos.x <= maxBox.x &&
            cameraPos.y >= minBox.y && cameraPos.y <= maxBox.y &&
            cameraPos.z >= minBox.z && cameraPos.z <= maxBox.z) 
        {
            return true;
        }
    }

    return false;
}

void resolveCollision(Camera & camera, const Model & model, glm::vec3 position, glm::vec3 scale)
{
    for (const auto& mesh : model.meshes) 
    {
        glm::vec3 minBox = mesh.boundingBox.min;
        glm::vec3 maxBox = mesh.boundingBox.max;

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::scale(model, scale);
        minBox = model * glm::vec4(minBox, 1.0f);
        maxBox = model * glm::vec4(maxBox, 1.0f);

        glm::vec3 collisionVector = (minBox + maxBox) * 0.5f - camera.Position;
        collisionVector = glm::normalize(collisionVector);
        glm::vec3 closestPoint = glm::clamp(camera.Position, minBox, maxBox);
        camera.Position = (closestPoint + collisionVector * 0.0001f);
        break;
    }
}
