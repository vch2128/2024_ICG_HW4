#include <bits/stdc++.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "header/cube.h"
#include "header/object.h"
#include "header/shader.h"
#include "header/stb_image.h"

#include <fstream>

void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
unsigned int loadCubemap(std::vector<string> &mFileName);

struct material_t{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float gloss;
};

struct light_t{
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

struct model_t{
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation;
    Object* object;
};

struct camera_t{
    glm::vec3 position;
    glm::vec3 up;
    float rotationY;
};

// settings
int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

// cube map 
unsigned int cubemapTexture;
unsigned int cubemapVAO, cubemapVBO;

// shader programs 
int alienProgramIndex = 0;
int ufoProgramIndex = 1;

std::vector<shader_program_t*> shaderPrograms;
shader_program_t* cubemapShader;

// additional dependencies
light_t light;
material_t material;
camera_t camera;
model_t alien;
model_t ufo;

// model matrix
int moveDir = -1;
glm::mat4 alienModel;
glm::mat4 ufoModel;
glm::mat4 cameraModel;

// for HW4 parameter
bool show_alien = false;
float increase = 0.0;
float explode = 0.0;
bool alienRotate = false;
bool alienStay = false;

//////////////////////////////////////////////////////////////////////////
// Parameter setup, 
// You can change any of the settings if you want

void camera_setup(){
    camera.position = glm::vec3(0.0, 20.0, 100.0);
    camera.up = glm::vec3(0.0, 1.0, 0.0);
    camera.rotationY = 0;
}

void light_setup(){
    light.position = glm::vec3(0.0, 1000.0, 0.0);
    light.ambient = glm::vec3(1.0);
    light.diffuse = glm::vec3(1.0);
    light.specular = glm::vec3(1.0);
}

void material_setup(){
    material.ambient = glm::vec3(1.0);
    material.diffuse = glm::vec3(1.0);
    material.specular = glm::vec3(1.0);
    material.gloss = 32.0;
}
//////////////////////////////////////////////////////////////////////////

void model_setup(){

// Load the object and texture for each model here 

#if defined(__linux__) || defined(__APPLE__)
    std::string objDir = "../../src/asset/obj/";
    std::string textureDir = "../../src/asset/texture/";
#else
    std::string objDir = "..\\..\\src\\asset\\obj\\";
    std::string textureDir = "..\\..\\src\\asset\\texture\\";
#endif
    // alien
    alienModel = glm::mat4(1.0f);
    alien.position = glm::vec3(-100.0f, 0.0f, 0.0f);
    alien.scale = glm::vec3(0.18f, 0.18f, 0.18f);
    alien.rotation = glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f);
    alien.object = new Object(objDir + "alien.obj");
    alien.object->load_to_buffer();
    alien.object->load_texture(textureDir + "alien.jpg");
    // ufo
    ufoModel = glm::mat4(1.0f);
    ufo.position = glm::vec3(-550.0f, 0.0f, 0.0f);
    ufo.scale = glm::vec3(0.17f, 0.17f, 0.17f);
    ufo.rotation = glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f);
    ufo.object = new Object(objDir + "ufo.obj");
    ufo.object->load_to_buffer();
    ufo.object->load_texture(textureDir + "ufo.jpg");
}


void shader_setup(){

// Setup the shader program for each shading method

#if defined(__linux__) || defined(__APPLE__)
    std::string shaderDir = "../../src/shaders/";
#else
    std::string shaderDir = "..\\..\\src\\shaders\\";
#endif

    std::vector<std::string> shadingMethod = {
        "default", "gouraud", "alert", "fur", "split", "grow", "noise", "explosion"
    };

    for(int i=0; i<shadingMethod.size(); i++){
        std::string vpath = shaderDir + shadingMethod[i] + ".vert";
        std::string gpath = shaderDir + shadingMethod[i] + ".geom";
        std::string fpath = shaderDir + shadingMethod[i] + ".frag";

        shader_program_t* shaderProgram = new shader_program_t();
        shaderProgram->create();
        shaderProgram->add_shader(vpath, GL_VERTEX_SHADER);
        if(std::ifstream(gpath)){
            shaderProgram->add_shader(gpath, GL_GEOMETRY_SHADER);
        }
        shaderProgram->add_shader(fpath, GL_FRAGMENT_SHADER);
        shaderProgram->link_shader();
        shaderPrograms.push_back(shaderProgram);
    } 
}


void cubemap_setup(){

// Setup all the necessary things for cubemap rendering
// Including: cubemap texture, shader program, VAO, VBO

#if defined(__linux__) || defined(__APPLE__)
    std::string cubemapDir = "../../src/asset/texture/skybox/";
    std::string shaderDir = "../../src/shaders/";
#else
    std::string cubemapDir = "..\\..\\src\\asset\\texture\\skybox\\";
    std::string shaderDir = "..\\..\\src\\shaders\\";
#endif

    // setup texture for cubemap
    std::vector<std::string> faces
    {
        cubemapDir + "right.jpg",
        cubemapDir + "left.jpg",
        cubemapDir + "top.jpg",
        cubemapDir + "bottom.jpg",
        cubemapDir + "front.jpg",
        cubemapDir + "back.jpg"
    };
    cubemapTexture = loadCubemap(faces);   

    // setup shader for cubemap
    std::string vpath = shaderDir + "cubemap.vert";
    std::string fpath = shaderDir + "cubemap.frag";
    
    cubemapShader = new shader_program_t();
    cubemapShader->create();
    cubemapShader->add_shader(vpath, GL_VERTEX_SHADER);
    cubemapShader->add_shader(fpath, GL_FRAGMENT_SHADER);
    cubemapShader->link_shader();

    glGenVertexArrays(1, &cubemapVAO);
    glGenBuffers(1, &cubemapVBO);
    glBindVertexArray(cubemapVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubemapVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubemapVertices), &cubemapVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
}

void setup(){

    // Initialize shader model camera light material
    light_setup();
    model_setup();
    shader_setup();
    camera_setup();
    cubemap_setup();
    material_setup();

    // Enable depth test, face culling ...
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    // Debug: enable for debugging
    // glEnable(GL_DEBUG_OUTPUT);
    // glDebugMessageCallback([](  GLenum source, GLenum type, GLuint id, GLenum severity, 
    //                             GLsizei length, const GLchar* message, const void* userParam) {

    // std::cerr << "GL CALLBACK: " << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "") 
    //           << "type = " << type 
    //           << ", severity = " << severity 
    //           << ", message = " << message << std::endl;
    // }, nullptr);
}

void update(){
    
// Update the alien position, camera position, rotation, etc.

    alien.position.y += moveDir;
    if(alien.position.y > 20.0 || alien.position.y < -20.0){
        moveDir = -moveDir;
    }
    static float alienRotationAngle = 0.0f;
    
    alienModel = glm::mat4(1.0f);
    alienModel = glm::scale(alienModel, alien.scale);

    if(!alienRotate){
        alienModel = glm::translate(alienModel, alien.position);
        alienModel = glm::rotate(alienModel, alien.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    }
    else{
        // alien rotate around UFO
        alienModel = glm::translate(alienModel, ufo.position);

        if(!alienStay){
            alienRotationAngle += glm::radians(1.0f);
            if (alienRotationAngle > glm::two_pi<float>()) {
                alienRotationAngle -= glm::two_pi<float>(); 
            }
        }

        alienModel = glm::rotate(alienModel, alienRotationAngle, glm::vec3(0.0f, -1.0f, 0.0f));
        alienModel = glm::translate(alienModel, glm::vec3(-180.0f, 0.0f, 0.0f));

        alienModel = glm::rotate(alienModel, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        alienModel = glm::rotate(alienModel, alien.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        
    }
    
    // rotate the ufo
    static float ufoRotationAngle = 0.0f; 
    ufoRotationAngle += glm::radians(2.0f);
    if (ufoRotationAngle > glm::two_pi<float>()) {
        ufoRotationAngle -= glm::two_pi<float>(); 
    }
    
    ufoModel = glm::mat4(1.0f);
    ufoModel = glm::scale(ufoModel, ufo.scale);
    ufoModel = glm::translate(ufoModel, ufo.position);
    ufoModel = glm::rotate(ufoModel, ufoRotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));
    ufoModel = glm::rotate(ufoModel, ufo.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));

    camera.rotationY = (camera.rotationY > 360.0) ? 0.0 : camera.rotationY;
    cameraModel = glm::mat4(1.0f);
    cameraModel = glm::rotate(cameraModel, glm::radians(camera.rotationY), camera.up);
    cameraModel = glm::translate(cameraModel, camera.position);

    increase += 0.04;
    explode = explode + increase;
}

void render(){

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Calculate view, projection matrix
    glm::mat4 view = glm::lookAt(glm::vec3(cameraModel[3]), glm::vec3(0.0), camera.up);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
    
    if (show_alien){
        shaderPrograms[alienProgramIndex]->use();
        shaderPrograms[alienProgramIndex]->set_uniform_value("model", alienModel);
        shaderPrograms[alienProgramIndex]->set_uniform_value("view", view);
        shaderPrograms[alienProgramIndex]->set_uniform_value("projection", projection);
        float alien_time = glfwGetTime();
        shaderPrograms[alienProgramIndex]->set_uniform_value("time", alien_time);
        shaderPrograms[alienProgramIndex]->set_uniform_value("material.ambient", material.ambient);
        shaderPrograms[alienProgramIndex]->set_uniform_value("material.diffuse", material.diffuse);
        shaderPrograms[alienProgramIndex]->set_uniform_value("material.specular", material.specular);
        shaderPrograms[alienProgramIndex]->set_uniform_value("material.gloss", material.gloss);
        shaderPrograms[alienProgramIndex]->set_uniform_value("light.position", light.position);
        shaderPrograms[alienProgramIndex]->set_uniform_value("light.ambient", light.ambient);
        shaderPrograms[alienProgramIndex]->set_uniform_value("light.diffuse", light.diffuse);
        shaderPrograms[alienProgramIndex]->set_uniform_value("light.specular", light.specular);
        shaderPrograms[alienProgramIndex]->set_uniform_value("CameraPos", glm::vec3(cameraModel[3]));
        shaderPrograms[alienProgramIndex]->set_uniform_value("explode", explode);
        alien.object->render();
        shaderPrograms[alienProgramIndex]->release();
        // for fur it need to render default alien as well
        if(alienProgramIndex == 3){
            shaderPrograms[0]->use();  
            shaderPrograms[0]->set_uniform_value("model", alienModel);
            shaderPrograms[0]->set_uniform_value("view", view);
            shaderPrograms[0]->set_uniform_value("projection", projection);
            alien.object->render();
            shaderPrograms[0]->release();
        }
    }

    // render UFO
    shaderPrograms[ufoProgramIndex]->use();
    shaderPrograms[ufoProgramIndex]->set_uniform_value("model", ufoModel);
    shaderPrograms[ufoProgramIndex]->set_uniform_value("view", view);
    shaderPrograms[ufoProgramIndex]->set_uniform_value("projection", projection);
    float ufo_time = glfwGetTime();
    shaderPrograms[ufoProgramIndex]->set_uniform_value("time", ufo_time);
    shaderPrograms[ufoProgramIndex]->set_uniform_value("material.ambient", material.ambient);
    shaderPrograms[ufoProgramIndex]->set_uniform_value("material.diffuse", material.diffuse);
    shaderPrograms[ufoProgramIndex]->set_uniform_value("material.specular", material.specular);
    shaderPrograms[ufoProgramIndex]->set_uniform_value("material.gloss", material.gloss);
    shaderPrograms[ufoProgramIndex]->set_uniform_value("light.position", light.position);
    shaderPrograms[ufoProgramIndex]->set_uniform_value("light.ambient", light.ambient);
    shaderPrograms[ufoProgramIndex]->set_uniform_value("light.diffuse", light.diffuse);
    shaderPrograms[ufoProgramIndex]->set_uniform_value("light.specular", light.specular);
    shaderPrograms[ufoProgramIndex]->set_uniform_value("CameraPos", glm::vec3(cameraModel[3]));
    shaderPrograms[ufoProgramIndex]->set_uniform_value("explode", explode);
    ufo.object->render();
    shaderPrograms[ufoProgramIndex]->release();

    // Rendering cubemap environment
    glDepthFunc(GL_LEQUAL);
    cubemapShader->use();
    glm::mat4 cubemapView = glm::mat4(glm::mat3(view));
    cubemapShader->set_uniform_value("view", cubemapView);
    cubemapShader->set_uniform_value("projection", projection);

    glBindVertexArray(cubemapVAO);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
    
    cubemapShader->release();
}


int main() {
    
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "HW4-111550113", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSwapInterval(1);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // set viewport
    glfwGetFramebufferSize(window, &SCR_WIDTH, &SCR_HEIGHT);
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    // Setup texture, model, shader ...e.t.c
    setup();
    
    // Render loop, main logic can be found in update, render function
    while (!glfwWindowShouldClose(window)) {
        update(); 
        render(); 
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// Add key callback
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // shader program selection
    if (key == GLFW_KEY_0 && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
        show_alien = false;
        ufoProgramIndex = 1;  // gouraud
    }
    if (key == GLFW_KEY_1 && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
        show_alien = false;
        ufoProgramIndex = 5;  // grow
    }
    if (key == GLFW_KEY_2 && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
        show_alien = false;
        ufoProgramIndex = 2;  // alert
    }
    if (key == GLFW_KEY_3 && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
        show_alien = true;
        alienProgramIndex = 1;  // gouraud
        alien.position = ufo.position + glm::vec3(-180.0f, 0.0f, 0.0f);
        ufoProgramIndex = 2;  // alert
    }
    if (key == GLFW_KEY_4 && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
        show_alien = true;
        alienProgramIndex = 3;  // fur
        ufoProgramIndex = 2;  // alert
    }
    // K
    if (key == GLFW_KEY_5 && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
        show_alien = true;
        alienProgramIndex = 3;  // fur
        ufoProgramIndex = 6;  // noise
    }
    if (key == GLFW_KEY_6 && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
        show_alien = true;
        alienStay = true;  // alien stop rotating
        alienProgramIndex = 4;   // noise
        ufoProgramIndex = 6;   // split
    }
    if (key == GLFW_KEY_7 && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
        show_alien = true;
        alienProgramIndex = 7;
        ufoProgramIndex = 7;
        explode = 0;
        increase = 0;
    }
    // camera movement
    float cameraSpeed = 0.5f;
    if (key == GLFW_KEY_W && (action == GLFW_REPEAT || action == GLFW_PRESS))
        camera.position.z -= 10.0;
    if (key == GLFW_KEY_S && (action == GLFW_REPEAT || action == GLFW_PRESS))
        camera.position.z += 10.0;
    if (key == GLFW_KEY_A && (action == GLFW_REPEAT || action == GLFW_PRESS))
        camera.rotationY -= 10.0;
    if (key == GLFW_KEY_D && (action == GLFW_REPEAT || action == GLFW_PRESS))
        camera.rotationY += 10.0;
    
    // UFO movement
    if (key == GLFW_KEY_J && (action == GLFW_REPEAT || action == GLFW_PRESS))
        ufo.position.x -= 10.0;
    if (key == GLFW_KEY_L && (action == GLFW_REPEAT || action == GLFW_PRESS))
        ufo.position.x += 10.0;

    // alien rotate around UFO
    if (key == GLFW_KEY_K && (action == GLFW_REPEAT || action == GLFW_PRESS))
        alienRotate = !alienRotate;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
}

// Loading cubemap texture
unsigned int loadCubemap(vector<std::string>& faces){

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        stbi_set_flip_vertically_on_load(false);
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return texture;
}  