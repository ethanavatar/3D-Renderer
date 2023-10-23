// C standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <stdbool.h>

// OpenGL
#include <glad/glad.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

// stb
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// Local includes
#include "util.h"
#include "time.hpp"
#include "shader.h"

// ----------------------------------------------------------------------------
const GLenum DRAW_MODE = GL_STATIC_DRAW;

void error(int32_t error, const char *description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

void input(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

void resize(GLFWwindow *window, int32_t width, int32_t height) {
    (void) window;

    glViewport(0, 0, width, height);
}

int32_t main(void) {

    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwSetErrorCallback(error);

    int width = 1200;
    int height = 900;

    char *title = "Doom Renderer Demo";
    GLFWwindow *window = glfwCreateWindow(width, height, title, NULL, NULL);
    glfwSetWindowAspectRatio(window, 4, 3);

    if (window == NULL) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        fprintf(stderr, "Failed to initialize GLAD\n");
        exit(EXIT_FAILURE);
    }

    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, resize);

    const char *const vertex_shader_source = read_file("assets/shaders/vertex.glsl");
    const char *const fragment_shader_source = read_file("assets/shaders/fragment.glsl");
    Shader *shader = shader_create(vertex_shader_source, fragment_shader_source);
        
    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };
    uint32_t indices[] = {
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    /// Texture

    uint32_t texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int32_t image_width, image_height, channels;
    uint8_t *data = stbi_load("assets/container.jpg", &image_width, &image_height, &channels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        fprintf(stderr, "Failed to load texture\n");
    }
    stbi_image_free(data);

    uint32_t texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);  
    data = stbi_load("assets/awesomeface.png", &image_width, &image_height, &channels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        fprintf(stderr, "Failed to load texture\n");
    }
    stbi_image_free(data);


    /// Vertex Array Object

    uint32_t vbo;
    glGenBuffers(1, &vbo);
    
    uint32_t ebo;
    glGenBuffers(1, &ebo);

    uint32_t vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, DRAW_MODE);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, DRAW_MODE);
    
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    uint64_t program_start_milis = get_milis();
    uint64_t program_time_milis = 0;
    uint64_t last_program_milis = program_start_milis;
    float program_time_secs = 0.0f;

    while(!glfwWindowShouldClose(window)) {

        // Update stuff
        
        input(window);

        program_time_milis = get_milis() - program_start_milis;
        program_time_secs = program_time_milis / 1000.0f;

        float delta_time = program_time_milis - last_program_milis;
        (void) delta_time;
        
        last_program_milis = program_time_milis;
        
        glfwGetFramebufferSize(window, &width, &height);

        char *title_format = "Doom Renderer Demo - %dx%d %.2f ms";
        char title[256];
        snprintf(title, 256, title_format, width, height, delta_time);
        glfwSetWindowTitle(window, title);

        // Clear stuff
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw stuff
        shader_use(shader);

        shader_set_float(shader, "iTime", program_time_secs);
        shader_set_ivec2(shader, "iResolution", width, height);
        glUniform1i(glGetUniformLocation(shader->program, "texture1"), 0);
        glUniform1i(glGetUniformLocation(shader->program, "texture2"), 1);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glBindVertexArray(vao);
 
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        
        // Push stuff
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    shader_destroy(shader);

    glfwDestroyWindow(window);
    glfwTerminate();

    exit(EXIT_SUCCESS);
}
