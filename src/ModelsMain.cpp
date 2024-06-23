#include "glad.h"
#include <GLFW/glfw3.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <filesystem.h>
#include <Headers/shader_m.h>
#include <Headers/Camera.h>
#include <Headers/model.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window, const Model& modelo);
//funciones
bool CheckCollision(const Camera& camera, const Model& model);
void resolveCollision(Camera& camera, const Model& model);

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

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
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
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
    Model corridor(FileSystem::getPath("Resources/Objects/delusional/delusionalHallway.gltf"));

    // draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window, corridor);

        // render
        // ------
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //esto va dentro del main
        if (!CheckCollision(camera, corridor)) {
            // Si hay colisión, realiza alguna acción
            std::cout << "¡Colision detectada!" << std::endl;
            resolveCollision(camera, corridor);
        }

        // don't forget to enable shader before setting uniforms
        ourShader.use();

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 200000.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        /*model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        exterior.Draw(ourShader);*/

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        corridor.Draw(ourShader);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

//esto va en el main
void processInput(GLFWwindow* window, const Model& modelo)
{
    camera.Position.y = 0.0f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        glm::vec3 front = camera.Front;
        glm::vec3 newPos = camera.Position + front * camera.MovementSpeed * deltaTime;
        if (CheckCollision(newPos, modelo))
            camera.Position = newPos;
        else resolveCollision(camera, modelo);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        glm::vec3 front = camera.Front;
        glm::vec3 newPos = camera.Position - front * camera.MovementSpeed * deltaTime;
        if (CheckCollision(newPos, modelo))
            camera.Position = newPos;
        else resolveCollision(camera, modelo);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        glm::vec3 right = camera.Right;
        glm::vec3 newPos = camera.Position - right * camera.MovementSpeed * deltaTime;
        if (CheckCollision(newPos, modelo))
            camera.Position = newPos;
        else resolveCollision(camera, modelo);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        glm::vec3 right = camera.Right;
        glm::vec3 newPos = camera.Position + right * camera.MovementSpeed * deltaTime;
        if (CheckCollision(newPos, modelo))
            camera.Position = newPos;
        else resolveCollision(camera, modelo);
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

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
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

//funciones
 bool CheckCollision(const Camera & camera, const Model & model) {
     // Obtenemos la posición de la cámara
     glm::vec3 cameraPos = camera.Position;

     // Iteramos sobre las mallas del modelo
     for (const auto& mesh : model.meshes) 
     {
         // Obtenemos la posición y dimensiones de la caja de colisión de la malla
         glm::vec3 minBox = mesh.boundingBox.min;
         glm::vec3 maxBox = mesh.boundingBox.max;

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

 void resolveCollision(Camera & camera, const Model & model) {
     // Obtenemos la posición de la cámara
     glm::vec3 cameraPos = camera.Position;

     // Iteramos sobre las mallas del modelo
     for (const auto& mesh : model.meshes) 
     {
         // Obtenemos la posición y dimensiones de la caja de colisión de la malla
         glm::vec3 minBox = mesh.boundingBox.min;
         glm::vec3 maxBox = mesh.boundingBox.max;

        
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