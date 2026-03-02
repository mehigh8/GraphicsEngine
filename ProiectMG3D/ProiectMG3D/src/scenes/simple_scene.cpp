#include "scenes/simple_scene.h"

#include <iostream>

SimpleScene::SimpleScene(std::string scene_name) : Scene(scene_name)
{
}

void SimpleScene::Init()
{
	Scene::Init();
    window = glfwCreateWindow(screen_width, screen_height, "ProiectMG3D", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    glViewport(0, 0, screen_width, screen_height);

    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

    glfwSetKeyCallback(window, Keyboard::KeyCallback);

    glfwSetCursorPosCallback(window, Mouse::CursorPosCallback);
    glfwSetMouseButtonCallback(window, Mouse::MouseButtonCallback);
    glfwSetScrollCallback(window, Mouse::MouseWheelCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);

    //ImGUI initialization
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    //CreateSceneContent();
    //LoadScene("scenes/SimpleSceneNoLight.xml");
    LoadScene("scenes/SimpleSceneLight.xml");
}

void SimpleScene::CreateSceneContent()
{
    mainCamera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));

    // Objects
    {
        // Create a cube
        std::vector<VertexData> vertices
        {
            VertexData(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(1, 0, 0), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f)), // bottom
            VertexData(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1, 0, 0), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
            VertexData(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(1, 0, 0), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 1.0f)),
            VertexData(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1, 0, 0), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)),
            VertexData(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0, 1, 0), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)), // front
            VertexData(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0, 1, 0), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)),
            VertexData(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0, 1, 0), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f)),
            VertexData(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0, 1, 0), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)),
            VertexData(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0, 0, 1), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)), // right
            VertexData(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0, 0, 1), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)),
            VertexData(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0, 0, 1), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)),
            VertexData(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0, 0, 1), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)),
            VertexData(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1, 1, 0), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)), // back
            VertexData(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1, 1, 0), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)),
            VertexData(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(1, 1, 0), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)),
            VertexData(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(1, 1, 0), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)),
            VertexData(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(1, 0, 1), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)), // up
            VertexData(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1, 0, 1), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)),
            VertexData(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(1, 0, 1), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)),
            VertexData(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(1, 0, 1), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)),
            VertexData(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0, 1, 1), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)), // left
            VertexData(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0, 1, 1), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)),
            VertexData(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0, 1, 1), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)),
            VertexData(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0, 1, 1), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f))
        };
        std::vector<unsigned int> indices
        {
            0, 1, 2,
            2, 1, 3,
            4, 5, 6,
            6, 5, 7,
            8, 9, 10,
            10, 9, 11,
            12, 13, 14,
            14, 13, 15,
            16, 17, 18,
            18, 17, 19,
            20, 21, 22,
            22, 21, 23
        };

        Mesh* mesh = new Mesh("cube");
        mesh->Init(vertices, indices);
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        // Import
        Mesh* mesh = new Mesh("bamboo");
        mesh->Load("models/bamboo", "bamboo.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Shaders
    {
        Shader* shader = new Shader("base");
        std::string shaderName = "shaders/base";
        shader->CreateProgram(shaderName);
        shaders[shader->GetShaderID()] = shader;
    }

    {
        Shader* shader = new Shader("basic_texture");
        std::string shaderName = "shaders/basic_texture";
        shader->CreateProgram(shaderName);
        shaders[shader->GetShaderID()] = shader;
    }

    {
        Shader* shader = new Shader("phong");
        std::string shaderName = "shaders/phong";
        shader->CreateProgram(shaderName);
        shaders[shader->GetShaderID()] = shader;
    }

    {
        Shader* shader = new Shader("light");
        std::string shaderName = "shaders/light";
        shader->CreateProgram(shaderName);
        shaders[shader->GetShaderID()] = shader;
    }

    // Textures
    {
        Texture* texture = new Texture("crate");
        if (texture->LoadTexture("textures/crate.png")) {
            textures[texture->GetName()] = texture;
        }
        else {
            std::cout << "Failed to load texture" << std::endl;
        }
    }

    // UI
    Text* simpleText = new Text("text1", ImVec2(50, 100), "Sample text", ImVec4(1.0f, 1.0f, 1.0f, 1.0f), 24.0f);
    Slider* slider = new Slider("slider1", ImVec2(50, 50), ImVec2(300, 20), 0.5f, ImVec4(0.2f, 0.2f, 0.2f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
    InputText* inputText = new InputText("input1", ImVec2(50, 150), 300, ImVec4(0.2f, 0.2f, 0.2f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Default text");
    Button* button = new Button("button1", ImVec2(50, 200), ImVec2(300, 50), "Update text", nullptr);
    Dropdown* dropdown = new Dropdown("dropdown1", ImVec2(50, 300), 300);
    dropdown->AddItem("red");
    dropdown->AddItem("green");
    dropdown->AddItem("blue");

    uiElements[simpleText->GetID()] = simpleText;
    uiElements[slider->GetID()] = slider;
    uiElements[inputText->GetID()] = inputText;
    uiElements[button->GetID()] = button;
    uiElements[dropdown->GetID()] = dropdown;

    /*GameObject* cube = new GameObject("cube");
    cube->mesh = meshes["cube"];
    cube->shader = shaders["base"];
    gameObjects[cube->GetObjectID()] = cube;*/

    GameObject* crate = new GameObject("crate");
    crate->position = glm::vec3(5.0f, 0.0f, 0.0f);
    crate->mesh = meshes["cube"];
    crate->shader = shaders["phong"];
    Material* crateMaterial = new Material();
    crateMaterial->texture = textures["crate"];
    crateMaterial->shininess = 32;
    crate->material = crateMaterial;
    gameObjects[crate->GetObjectID()] = crate;

    GameObject* bamboo = new GameObject("bamboo");
    bamboo->position = glm::vec3(-5.0f, 0.0f, 0.0f);
    bamboo->scale = glm::vec3(0.1f, 0.1f, 0.1f);
    bamboo->mesh = meshes["bamboo"];
    bamboo->shader = shaders["phong"];
    gameObjects[bamboo->GetObjectID()] = bamboo;

    GameObject* movableLight = new GameObject("movableLight");
    movableLight->position = glm::vec3(0.0f, 0.5f, -2.5f);
    movableLight->scale = glm::vec3(0.25f, 0.25f, 0.25f);
    movableLight->mesh = meshes["cube"];
    movableLight->shader = shaders["light"];
    Light* movableLightScript = new Light(glm::vec3(1.0f, 1.0f, 1.0f));
    movableLight->light = movableLightScript;
    gameObjects[movableLight->GetObjectID()] = movableLight;

    GameObject* ground = new GameObject("ground");
    ground->position = glm::vec3(0.0f, -3.5f, 0.0f);
    ground->scale = glm::vec3(5.0f, 5.0f, 5.0f);
    ground->mesh = meshes["cube"];
    ground->shader = shaders["phong"];
    Material* groundMaterial = new Material();
    groundMaterial->ambient = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    groundMaterial->diffuse = groundMaterial->ambient;
    groundMaterial->specular = groundMaterial->ambient;
    groundMaterial->shininess = 32;
    ground->material = groundMaterial;
    gameObjects[ground->GetObjectID()] = ground;

    GameObject* cube = new GameObject("cube");
    cube->rotation = glm::vec3(0.0f, glm::radians(45.0f), 0.0f);
    cube->mesh = meshes["cube"];
    cube->shader = shaders["phong"];
    Material* cubeMaterial = new Material();
    cubeMaterial->ambient = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
    cubeMaterial->diffuse = cubeMaterial->ambient;
    cubeMaterial->specular = cubeMaterial->ambient;
    cubeMaterial->shininess = 32;
    cube->material = cubeMaterial;
    gameObjects[cube->GetObjectID()] = cube;

    GameObject* cube2 = new GameObject("cube2");
    cube2->position = glm::vec3(4.0f, 0.0f, -2.0f);
    cube2->mesh = meshes["cube"];
    cube2->shader = shaders["phong"];
    Material* cube2Material = new Material();
    cube2Material->ambient = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    cube2Material->diffuse = cube2Material->ambient;
    cube2Material->specular = cube2Material->ambient;
    cube2Material->shininess = 32;
    cube2->material = cube2Material;
    gameObjects[cube2->GetObjectID()] = cube2;

    GameObject* stillLight = new GameObject("stillLight");
    stillLight->position = glm::vec3(4.0f, 0.0f, 0.0f);
    stillLight->scale = glm::vec3(0.25f, 0.25f, 0.25f);
    stillLight->mesh = meshes["cube"];
    stillLight->shader = shaders["light"];
    Light* stillLightScript = new Light(glm::vec3(0.0f, 1.0f, 1.0f));
    stillLight->light = stillLightScript;
    gameObjects[stillLight->GetObjectID()] = stillLight;

    GameObject* stillLight2 = new GameObject("stillLight2");
    stillLight2->position = glm::vec3(-4.0f, 0.0f, 0.0f);
    stillLight2->scale = glm::vec3(0.25f, 0.25f, 0.25f);
    stillLight2->mesh = meshes["cube"];
    stillLight2->shader = shaders["light"];
    Light* stillLight2Script = new Light(glm::vec3(1.0f, 1.0f, 0.0f));
    stillLight2->light = stillLight2Script;
    gameObjects[stillLight2->GetObjectID()] = stillLight2;

    GameObject* spotLight = new GameObject("spotLight");
    spotLight->position = glm::vec3(0.0f, 1.0f, 0.5f);
    spotLight->scale = glm::vec3(0.25f, 0.25f, 0.25f);
    spotLight->mesh = meshes["cube"];
    spotLight->shader = shaders["light"];
    Light* spotLightScript = new Light(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::radians(45.0f));
    spotLight->light = spotLightScript;
    gameObjects[spotLight->GetObjectID()] = spotLight;
}

void SimpleScene::StartFrame()
{
    Scene::StartFrame();
    CalculateDeltaTime();
    ProcessInput(window);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void SimpleScene::UpdateLogic()
{
    for (auto it = gameObjects.begin(); it != gameObjects.end(); it++)
        it->second->CalculateModelMatrix();

    gameObjects["cube"]->model = glm::rotate(gameObjects["cube"]->model, (float)glfwGetTime() * glm::radians(45.0f), mainCamera->worldUp);

    glm::mat4 view = mainCamera->GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(mainCamera->fov), (float)screen_width / (float)screen_height, mainCamera->zNear, mainCamera->zFar);

    /*gameObjects["cube"]->shader->Use();

    gameObjects["cube"]->shader->SendUniformMat4("model", gameObjects["cube"]->model);
    gameObjects["cube"]->shader->SendUniformMat4("view", view);
    gameObjects["cube"]->shader->SendUniformMat4("projection", projection);

    gameObjects["cube"]->mesh->Render();

    gameObjects["crate"]->shader->Use();

    gameObjects["crate"]->shader->SendUniformMat4("model", gameObjects["crate"]->model);
    gameObjects["crate"]->shader->SendUniformMat4("view", view);
    gameObjects["crate"]->shader->SendUniformMat4("projection", projection);

    gameObjects["crate"]->material->texture->BindToActiveTexture(GL_TEXTURE0);
    gameObjects["crate"]->shader->SendUniformInt("tex", 0);

    gameObjects["crate"]->mesh->Render();

    gameObjects["bamboo"]->shader->Use();

    gameObjects["bamboo"]->shader->SendUniformMat4("model", gameObjects["bamboo"]->model);
    gameObjects["bamboo"]->shader->SendUniformMat4("view", view);
    gameObjects["bamboo"]->shader->SendUniformMat4("projection", projection);

    gameObjects["bamboo"]->shader->SendUniformInt("tex", 0);

    gameObjects["bamboo"]->mesh->Render();*/

    gameObjects["movableLight"]->shader->Use();

    gameObjects["movableLight"]->shader->SendUniformMat4("model", gameObjects["movableLight"]->model);
    gameObjects["movableLight"]->shader->SendUniformMat4("view", view);
    gameObjects["movableLight"]->shader->SendUniformMat4("projection", projection);

    gameObjects["movableLight"]->shader->SendUniformFloat3("light_color", gameObjects["movableLight"]->light->color);

    gameObjects["movableLight"]->mesh->Render();

    // With illumination
    gameObjects["stillLight"]->shader->Use();

    gameObjects["stillLight"]->shader->SendUniformMat4("model", gameObjects["stillLight"]->model);
    gameObjects["stillLight"]->shader->SendUniformMat4("view", view);
    gameObjects["stillLight"]->shader->SendUniformMat4("projection", projection);

    gameObjects["stillLight"]->shader->SendUniformFloat3("light_color", gameObjects["stillLight"]->light->color);

    gameObjects["stillLight"]->mesh->Render();

    gameObjects["stillLight2"]->shader->Use();

    gameObjects["stillLight2"]->shader->SendUniformMat4("model", gameObjects["stillLight2"]->model);
    gameObjects["stillLight2"]->shader->SendUniformMat4("view", view);
    gameObjects["stillLight2"]->shader->SendUniformMat4("projection", projection);

    gameObjects["stillLight2"]->shader->SendUniformFloat3("light_color", gameObjects["stillLight2"]->light->color);

    gameObjects["stillLight2"]->mesh->Render();

    gameObjects["spotLight"]->shader->Use();

    gameObjects["spotLight"]->shader->SendUniformMat4("model", gameObjects["spotLight"]->model);
    gameObjects["spotLight"]->shader->SendUniformMat4("view", view);
    gameObjects["spotLight"]->shader->SendUniformMat4("projection", projection);

    gameObjects["spotLight"]->shader->SendUniformFloat3("light_color", gameObjects["spotLight"]->light->color);

    gameObjects["spotLight"]->mesh->Render();

    int point_lights_count = 3;
    glm::vec3 point_light_positions[10] = { gameObjects["movableLight"]->position, gameObjects["stillLight"]->position, gameObjects["stillLight2"]->position };
    glm::vec3 point_light_colors[10] = { gameObjects["movableLight"]->light->color, gameObjects["stillLight"]->light->color, gameObjects["stillLight2"]->light->color };

    int spot_lights_count = 1;
    glm::vec3 spot_light_positions[10] = { gameObjects["spotLight"]->position };
    glm::vec3 spot_light_colors[10] = { gameObjects["spotLight"]->light->color };
    glm::vec3 spot_light_directions[10] = { gameObjects["spotLight"]->light->direction };
    float spot_light_angles[10] = { gameObjects["spotLight"]->light->angle };
    
    gameObjects["crate"]->shader->Use();

    gameObjects["crate"]->shader->SendUniformMat4("model", gameObjects["crate"]->model);
    gameObjects["crate"]->shader->SendUniformMat4("view", view);
    gameObjects["crate"]->shader->SendUniformMat4("projection", projection);

    gameObjects["crate"]->material->texture->BindToActiveTexture(GL_TEXTURE0);
    gameObjects["crate"]->shader->SendUniformInt("material.tex", 0);
    gameObjects["crate"]->shader->SendUniformInt("material.useTexture", 1);
    gameObjects["crate"]->shader->SendUniformFloat3("ambient_color", global_light);

    gameObjects["crate"]->shader->SendUniformInt("point_lights_count", point_lights_count);
    gameObjects["crate"]->shader->SendUniformFloat3Multiple("point_light_positions", point_lights_count, point_light_positions);
    gameObjects["crate"]->shader->SendUniformFloat3Multiple("point_light_colors", point_lights_count, point_light_colors);

    gameObjects["crate"]->shader->SendUniformInt("spot_lights_count", spot_lights_count);
    gameObjects["crate"]->shader->SendUniformFloat3Multiple("spot_light_positions", spot_lights_count, spot_light_positions);
    gameObjects["crate"]->shader->SendUniformFloat3Multiple("spot_light_colors", spot_lights_count, spot_light_colors);
    gameObjects["crate"]->shader->SendUniformFloat3Multiple("spot_light_directions", spot_lights_count, spot_light_directions);
    gameObjects["crate"]->shader->SendUniformFloatMultiple("spot_light_angles", spot_lights_count, spot_light_angles);

    gameObjects["crate"]->mesh->Render();

    gameObjects["bamboo"]->shader->Use();

    gameObjects["bamboo"]->shader->SendUniformMat4("model", gameObjects["bamboo"]->model);
    gameObjects["bamboo"]->shader->SendUniformMat4("view", view);
    gameObjects["bamboo"]->shader->SendUniformMat4("projection", projection);

    gameObjects["bamboo"]->shader->SendUniformInt("material.tex", 0);
    gameObjects["bamboo"]->shader->SendUniformInt("material.useTexture", 1);
    gameObjects["bamboo"]->shader->SendUniformFloat3("ambient_color", global_light);

    gameObjects["bamboo"]->shader->SendUniformInt("point_lights_count", point_lights_count);
    gameObjects["bamboo"]->shader->SendUniformFloat3Multiple("point_light_positions", point_lights_count, point_light_positions);
    gameObjects["bamboo"]->shader->SendUniformFloat3Multiple("point_light_colors", point_lights_count, point_light_colors);

    gameObjects["bamboo"]->shader->SendUniformInt("spot_lights_count", spot_lights_count);
    gameObjects["bamboo"]->shader->SendUniformFloat3Multiple("spot_light_positions", spot_lights_count, spot_light_positions);
    gameObjects["bamboo"]->shader->SendUniformFloat3Multiple("spot_light_colors", spot_lights_count, spot_light_colors);
    gameObjects["bamboo"]->shader->SendUniformFloat3Multiple("spot_light_directions", spot_lights_count, spot_light_directions);
    gameObjects["bamboo"]->shader->SendUniformFloatMultiple("spot_light_angles", spot_lights_count, spot_light_angles);

    gameObjects["bamboo"]->mesh->Render();

    gameObjects["ground"]->shader->Use();

    gameObjects["ground"]->shader->SendUniformMat4("model", gameObjects["ground"]->model);
    gameObjects["ground"]->shader->SendUniformMat4("view", view);
    gameObjects["ground"]->shader->SendUniformMat4("projection", projection);

    gameObjects["ground"]->shader->SendUniformFloat3("material.ambient", gameObjects["ground"]->material->ambient);
    gameObjects["ground"]->shader->SendUniformFloat3("material.diffuse", gameObjects["ground"]->material->diffuse);
    gameObjects["ground"]->shader->SendUniformFloat3("material.specular", gameObjects["ground"]->material->specular);
    gameObjects["ground"]->shader->SendUniformInt("material.shininess", gameObjects["ground"]->material->shininess);
    gameObjects["ground"]->shader->SendUniformInt("material.useTexture", 0);
    gameObjects["ground"]->shader->SendUniformFloat3("ambient_color", global_light);

    gameObjects["ground"]->shader->SendUniformInt("point_lights_count", point_lights_count);
    gameObjects["ground"]->shader->SendUniformFloat3Multiple("point_light_positions", point_lights_count, point_light_positions);
    gameObjects["ground"]->shader->SendUniformFloat3Multiple("point_light_colors", point_lights_count, point_light_colors);

    gameObjects["ground"]->shader->SendUniformInt("spot_lights_count", spot_lights_count);
    gameObjects["ground"]->shader->SendUniformFloat3Multiple("spot_light_positions", spot_lights_count, spot_light_positions);
    gameObjects["ground"]->shader->SendUniformFloat3Multiple("spot_light_colors", spot_lights_count, spot_light_colors);
    gameObjects["ground"]->shader->SendUniformFloat3Multiple("spot_light_directions", spot_lights_count, spot_light_directions);
    gameObjects["ground"]->shader->SendUniformFloatMultiple("spot_light_angles", spot_lights_count, spot_light_angles);

    gameObjects["ground"]->mesh->Render();

    gameObjects["cube"]->shader->Use();

    gameObjects["cube"]->shader->SendUniformMat4("model", gameObjects["cube"]->model);
    gameObjects["cube"]->shader->SendUniformMat4("view", view);
    gameObjects["cube"]->shader->SendUniformMat4("projection", projection);

    gameObjects["cube"]->shader->SendUniformFloat3("material.ambient", gameObjects["cube"]->material->ambient);
    gameObjects["cube"]->shader->SendUniformFloat3("material.diffuse", gameObjects["cube"]->material->diffuse);
    gameObjects["cube"]->shader->SendUniformFloat3("material.specular", gameObjects["cube"]->material->specular);
    gameObjects["cube"]->shader->SendUniformInt("material.shininess", gameObjects["cube"]->material->shininess);
    gameObjects["cube"]->shader->SendUniformInt("material.useTexture", 0);
    gameObjects["cube"]->shader->SendUniformFloat3("ambient_color", global_light);

    gameObjects["cube"]->shader->SendUniformInt("point_lights_count", point_lights_count);
    gameObjects["cube"]->shader->SendUniformFloat3Multiple("point_light_positions", point_lights_count, point_light_positions);
    gameObjects["cube"]->shader->SendUniformFloat3Multiple("point_light_colors", point_lights_count, point_light_colors);

    gameObjects["cube"]->shader->SendUniformInt("spot_lights_count", spot_lights_count);
    gameObjects["cube"]->shader->SendUniformFloat3Multiple("spot_light_positions", spot_lights_count, spot_light_positions);
    gameObjects["cube"]->shader->SendUniformFloat3Multiple("spot_light_colors", spot_lights_count, spot_light_colors);
    gameObjects["cube"]->shader->SendUniformFloat3Multiple("spot_light_directions", spot_lights_count, spot_light_directions);
    gameObjects["cube"]->shader->SendUniformFloatMultiple("spot_light_angles", spot_lights_count, spot_light_angles);

    gameObjects["cube"]->mesh->Render();

    gameObjects["cube2"]->shader->Use();

    gameObjects["cube2"]->shader->SendUniformMat4("model", gameObjects["cube2"]->model);
    gameObjects["cube2"]->shader->SendUniformMat4("view", view);
    gameObjects["cube2"]->shader->SendUniformMat4("projection", projection);

    gameObjects["cube2"]->shader->SendUniformFloat3("material.ambient", gameObjects["cube2"]->material->ambient);
    gameObjects["cube2"]->shader->SendUniformFloat3("material.diffuse", gameObjects["cube2"]->material->diffuse);
    gameObjects["cube2"]->shader->SendUniformFloat3("material.specular", gameObjects["cube2"]->material->specular);
    gameObjects["cube2"]->shader->SendUniformInt("material.shininess", gameObjects["cube2"]->material->shininess);
    gameObjects["cube2"]->shader->SendUniformInt("material.useTexture", 0);
    gameObjects["cube2"]->shader->SendUniformFloat3("ambient_color", global_light);

    gameObjects["cube2"]->shader->SendUniformInt("point_lights_count", point_lights_count);
    gameObjects["cube2"]->shader->SendUniformFloat3Multiple("point_light_positions", point_lights_count, point_light_positions);
    gameObjects["cube2"]->shader->SendUniformFloat3Multiple("point_light_colors", point_lights_count, point_light_colors);

    gameObjects["cube2"]->shader->SendUniformInt("spot_lights_count", spot_lights_count);
    gameObjects["cube2"]->shader->SendUniformFloat3Multiple("spot_light_positions", spot_lights_count, spot_light_positions);
    gameObjects["cube2"]->shader->SendUniformFloat3Multiple("spot_light_colors", spot_lights_count, spot_light_colors);
    gameObjects["cube2"]->shader->SendUniformFloat3Multiple("spot_light_directions", spot_lights_count, spot_light_directions);
    gameObjects["cube2"]->shader->SendUniformFloatMultiple("spot_light_angles", spot_lights_count, spot_light_angles);

    gameObjects["cube2"]->mesh->Render();

    for (auto it = uiElements.begin(); it != uiElements.end(); it++)
        it->second->Render();
}

void SimpleScene::EndFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    Scene::EndFrame();
}

void SimpleScene::CalculateDeltaTime()
{
    double currentFrameTime = glfwGetTime();
    deltaTime = currentFrameTime - lastFrameTime;
    lastFrameTime = currentFrameTime;
}

void SimpleScene::ProcessInput(GLFWwindow* window)
{
    if (Keyboard::KeyWentDown(GLFW_KEY_ESCAPE))
        glfwSetWindowShouldClose(window, true);

    for (auto it = uiElements.begin(); it != uiElements.end(); it++)
        if (it->second->isAccessed == true)
            return;

    if (Keyboard::Key(GLFW_KEY_W))
        mainCamera->Move(CameraDirection::FORWARD, deltaTime);
    if (Keyboard::Key(GLFW_KEY_S))
        mainCamera->Move(CameraDirection::BACKWARD, deltaTime);
    if (Keyboard::Key(GLFW_KEY_A))
        mainCamera->Move(CameraDirection::LEFT, deltaTime);
    if (Keyboard::Key(GLFW_KEY_D))
        mainCamera->Move(CameraDirection::RIGHT, deltaTime);
    if (Keyboard::Key(GLFW_KEY_E))
        mainCamera->Move(CameraDirection::UP, deltaTime);
    if (Keyboard::Key(GLFW_KEY_Q))
        mainCamera->Move(CameraDirection::DOWN, deltaTime);

    double mouseDX = Mouse::GetDX();
    double mouseDY = Mouse::GetDY();
    if (mouseDX != 0 || mouseDY != 0)
        mainCamera->Rotate(mouseDX, mouseDY);

    double scrollDY = Mouse::GetScrollDY();
    if (scrollDY != 0)
        mainCamera->Zoom(scrollDY);

    if (Keyboard::KeyWentDown(GLFW_KEY_P))
        GetSlider("slider1")->UpdateValue(GetSlider("slider1")->GetValue() + 0.1f);
    if (Keyboard::KeyWentDown(GLFW_KEY_O))
        GetSlider("slider1")->UpdateValue(GetSlider("slider1")->GetValue() - 0.1f);

    if (Keyboard::KeyWentDown(GLFW_KEY_C)) {
        glfwSetInputMode(window, GLFW_CURSOR, cursorEnabled ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
        cursorEnabled = !cursorEnabled;
    }

    if (Keyboard::Key(GLFW_KEY_UP))
        gameObjects["movableLight"]->position += glm::vec3(0.0f, 0.0f, -deltaTime);
    if (Keyboard::Key(GLFW_KEY_DOWN))
        gameObjects["movableLight"]->position += glm::vec3(0.0f, 0.0f, deltaTime);
    if (Keyboard::Key(GLFW_KEY_LEFT))
        gameObjects["movableLight"]->position += glm::vec3(-deltaTime, 0.0f, 0.0f);
    if (Keyboard::Key(GLFW_KEY_RIGHT))
        gameObjects["movableLight"]->position += glm::vec3(deltaTime, 0.0f, 0.0f);

    if (Keyboard::KeyWentDown(GLFW_KEY_I)) {
        std::cout << "Saving scene to SimpleScene.xml\n";
        SaveScene("scenes/SimpleScene.xml");
    }
}

void SimpleScene::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    screen_height = height;
    screen_width = width;
}