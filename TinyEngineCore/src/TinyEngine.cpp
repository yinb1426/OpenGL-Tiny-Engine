#include "Application.h"

using namespace TinyEngine;

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    auto application = std::make_unique<Application>(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL");
    application->Loop();
    application->Destroy();
    return 0;
    /*
    Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));

    // glfw: initialize and configure
    // ------------------------------
    OpenGLContext context(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL");

    ResourceManager resourceManager;

    resourceManager.AddShader("Triangle Shader", std::make_shared<Shader>(SHADER_PATH "/triangle.vert.glsl", SHADER_PATH "/triangle.frag.glsl"));
    resourceManager.AddShader("Rock Shader", std::make_shared<Shader>(SHADER_PATH "/rock.vert.glsl", SHADER_PATH "/rock.frag.glsl"));
    resourceManager.AddShader("Cube Shader2", std::make_shared<Shader>(SHADER_PATH "/cube2.vert.glsl", SHADER_PATH "/cube2.frag.glsl"));
    resourceManager.AddTexture("Container", std::make_shared<Texture>(TEXTURE_PATH "/container.jpg"));
    resourceManager.AddTexture("Awesomeface", std::make_shared<Texture>(TEXTURE_PATH "/awesomeface.png"));
    resourceManager.AddModel("Rock", std::make_shared<Model>(MODEL_PATH "/rock/rock.obj"));
    resourceManager.AddMaterial("Rock Material", std::make_shared<Material>(MATERIAL_PATH "/rock.json", resourceManager.GetShader("Rock Shader")));

    std::shared_ptr<Material> material = resourceManager.GetMaterial("Rock Material");
    std::shared_ptr<Model> model = resourceManager.GetModel("Rock");

    std::shared_ptr<Scene> scene = std::make_shared<Scene>(camera);
    scene->AddGameObject("rock", {
        model,
        Transform(glm::vec3(1.0f, 0.0f, 0.0f)),
        material
    });

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
        stateManager.Enable(GL_DEPTH_TEST);

        // render
        // ------
        stateManager.ClearPerFrame(0.2f, 0.3f, 0.3f, 1.0f);
        
        // draw our first triangle
        //glm::mat4 modelMatrix = transform.GetModelMatrix(); // glm::mat4(1.0f);
        //glm::mat4 viewMatrix = camera.GetViewMatrix();
        //glm::mat4 projectionMatrix = camera.GetProjectionMtarix((float)SCR_WIDTH / (float)SCR_HEIGHT);

        //material->SetMatrices(modelMatrix, viewMatrix, projectionMatrix);
        //material->SetUniform(resourceManager.GetTextureMap());
        //model->Draw(material->GetShader(), resourceManager.GetTextureMap());
        scene->Render(context.GetWindowAspect(), resourceManager.GetTextureMap());

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        context.SwapBuffersAndPollEvents();
    }
    return 0;
    */
}