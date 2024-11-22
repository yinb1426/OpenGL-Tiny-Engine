#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Camera.h>
#include <Config.h>
#include <ResourceManager.h>
#include <OpenGLContext.h>
#include <StateManager.h>
#include "Buffer.h"
using namespace TinyEngine;

void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    Camera camera(glm::vec3(3.0f, 1.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -135.0f, -15.0f);

    // glfw: initialize and configure
    // ------------------------------
    OpenGLContext context(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL");

    ResourceManager resourceManager;

    resourceManager.AddShader("Triangle Shader", std::make_shared<Shader>(SHADER_PATH "triangle.vert.glsl", SHADER_PATH "triangle.frag.glsl"));
    resourceManager.AddShader("Cube Shader", std::make_shared<Shader>(SHADER_PATH "cube.vert.glsl", SHADER_PATH "cube.frag.glsl"));
    resourceManager.AddTexture("Container", std::make_shared<Texture>(TEXTURE_PATH "container.jpg"));

    std::shared_ptr<Shader> shaderProgram = resourceManager.GetShader("Cube Shader");
    std::shared_ptr<Texture> texture = resourceManager.GetTexture("Container");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };


    unsigned int VAO;
    Buffer VBO(BufferType::Vertex, DataUsage::Static, vertices, sizeof(vertices));

    glGenVertexArrays(1, &VAO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    VBO.Bind();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    VBO.Unbind();

    Buffer UBO(BufferType::Uniform, DataUsage::Static, nullptr, sizeof(glm::mat4) * 3);
    shaderProgram->BindUniformBlock("Matrices", 0);

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    StateManager stateManager;

    // render loop
    // -----------
    while (!context.ShouldClose())
    {
        // input
        // -----
        //processInput(window);

        // render
        // ------
        stateManager.ClearPerFrame(0.2f, 0.3f, 0.3f, 1.0f);

        // draw our first triangle
        shaderProgram->Use();
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        glm::mat4 viewMatrix = camera.GetViewMatrix();
        glm::mat4 projectionMatrix = camera.GetProjectionMtarix((float)SCR_WIDTH / (float)SCR_HEIGHT);

        UBO.UpdateBuffer(&modelMatrix[0][0], sizeof(glm::mat4), 0);
        UBO.UpdateBuffer(&viewMatrix[0][0], sizeof(glm::mat4), sizeof(glm::mat4));
        UBO.UpdateBuffer(&projectionMatrix[0][0], sizeof(glm::mat4), 2 * sizeof(glm::mat4));

        UBO.BindToBindingPoint(0);
        shaderProgram->SetUniform("texture0", 0);
        texture->Bind(0);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0); // no need to unbind it every time 

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        context.SwapBuffersAndPollEvents();
    }
    texture->Unbind();
    texture->Destroy();
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    VBO.DeleteBuffer();

    UBO.UnbindFromBindingPoint(0);
    UBO.DeleteBuffer();
    // shaderProgram->Destroy();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
