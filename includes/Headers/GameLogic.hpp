#pragma once

#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>

#include <filesystem.h>
#include <Headers/shader_m.h>
#include <Headers/Camera.h>
#include <Headers/model.h>

#include <iostream>

sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
const unsigned int SCR_WIDTH = desktopMode.width;
const unsigned int SCR_HEIGHT = desktopMode.height;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool processInput(sf::Window &App, const Model &modelo, int mode, sf::Sound &stepSound, int indice, glm::vec3 position, glm::vec3 scale);
bool CheckCollision(const Camera &camera, const Model &model, glm::vec3 position, glm::vec3 scale);
void resolveCollision(Camera &camera, const Model &model, glm::vec3 position, glm::vec3 scale);

bool processInput(sf::Window &App, const Model &modelo, int mode, sf::Sound &stepSound, int indice, glm::vec3 position = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f))
{
    std::cout << glm::to_string(camera.Position) << std::endl;
    if (indice == 2)
    {
        camera.Position.y = 7.0f;
        camera.MovementSpeed = 25.0f;
    }
    else if(indice==3){
        //camera.Position.x= 40.0f;
        camera.Position.y = 40.0f;
        camera.MovementSpeed = 30.0f;
    }
    else
    {
        camera.Position.y = 0.0f;
    }
    std::cout << glm::to_string(camera.Position) << std::endl;
    if (mode == 1)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            glm::vec3 front = camera.Front;
            glm::vec3 newPos = camera.Position + front * camera.MovementSpeed * deltaTime;
            if (!CheckCollision(newPos, modelo, position, scale))
            {
                camera.Position = newPos;
                stepSound.pause();
                stepSound.play();
            }
            else if (indice != 2 && indice != 3)
            {
                resolveCollision(camera, modelo, position, scale);
            } 
    
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            glm::vec3 front = camera.Front;
            glm::vec3 newPos = camera.Position - front * camera.MovementSpeed * deltaTime;
            if (!CheckCollision(newPos, modelo, position, scale))
            {
                camera.Position = newPos;
                stepSound.pause();
                stepSound.play();
            }
            else if (indice != 2 && indice != 3)
            {
                resolveCollision(camera, modelo, position, scale);
            }*/
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            glm::vec3 right = camera.Right;
            glm::vec3 newPos = camera.Position - right * camera.MovementSpeed * deltaTime;
            if (!CheckCollision(newPos, modelo, position, scale))
            {
                camera.Position = newPos;
                stepSound.pause();
                stepSound.play();
            }
            else if (indice != 2 && indice != 3)
            {
                resolveCollision(camera, modelo, position, scale);
            }*/
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            glm::vec3 right = camera.Right;
            glm::vec3 newPos = camera.Position + right * camera.MovementSpeed * deltaTime;
            if (!CheckCollision(newPos, modelo, position, scale))
            {
                camera.Position = newPos;
                stepSound.pause();
                stepSound.play();
            }
            else if (indice != 2 && indice != 3)
            {
                resolveCollision(camera, modelo, position, scale);
            }*/
        }
    }
    else
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            glm::vec3 front = camera.Front;
            glm::vec3 newPos = camera.Position + front * camera.MovementSpeed * deltaTime;
            if (CheckCollision(newPos, modelo, position, scale))
            {
                camera.Position = newPos;
                stepSound.pause();
                stepSound.play();
            }
            else if (indice != 2 && indice != 3)
            {
                resolveCollision(camera, modelo, position, scale);
            }*/
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            glm::vec3 front = camera.Front;
            glm::vec3 newPos = camera.Position - front * camera.MovementSpeed * deltaTime;
            if (CheckCollision(newPos, modelo, position, scale))
            {
                camera.Position = newPos;
                stepSound.pause();
                stepSound.play();
            }
            else if (indice != 2 && indice != 3)
            {
                resolveCollision(camera, modelo, position, scale);
            }*/
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            glm::vec3 right = camera.Right;
            glm::vec3 newPos = camera.Position - right * camera.MovementSpeed * deltaTime;
            if (CheckCollision(newPos, modelo, position, scale))
            {
                camera.Position = newPos;
                stepSound.pause();
                stepSound.play();
            }
            else if (indice != 2 && indice != 3)
            {
                resolveCollision(camera, modelo, position, scale);
             }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            glm::vec3 right = camera.Right;
            glm::vec3 newPos = camera.Position + right * camera.MovementSpeed * deltaTime;
            if (CheckCollision(newPos, modelo, position, scale))
            {
                camera.Position = newPos;
                stepSound.pause();
                stepSound.play();
            }
            else if (indice != 2 && indice != 3)
            {
                resolveCollision(camera, modelo, position, scale);
            }*/
        }
    }

    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)))
        return false;

    return true;
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