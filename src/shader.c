#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h>

#include "shader.h"

static GLint last_status;
static char last_error[512];

static void log_shader_error(GLuint shader, GLenum type, char *error_header) {
    glGetShaderiv(shader, type, &last_status);
    if (last_status) {
        return;
    }
    
    glGetShaderInfoLog(shader, 512, NULL, last_error);
    fprintf(stderr, "%s %s\n", error_header, last_error);
}


Shader *shader_create(const char *const vertex_source,
                      const char *const fragment_source) {
    
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_source, NULL);
    glCompileShader(vertex_shader);

    char *header = "Error while compiling vertex shader:";
    log_shader_error(vertex_shader, GL_COMPILE_STATUS, header);
    
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_source, NULL);
    glCompileShader(fragment_shader);

    header = "Error while compiling fragment shader:";
    log_shader_error(fragment_shader, GL_COMPILE_STATUS, header);

    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    header = "Error while linking shader program:";
    log_shader_error(shader_program, GL_LINK_STATUS, header);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    Shader *shader = malloc(sizeof(Shader));
    shader->program = shader_program;

    return shader;
}

void shader_destroy(Shader *shader) {
    glDeleteProgram(shader->program);
    free(shader);
}

void shader_use(Shader *shader) {
    glUseProgram(shader->program);
}

void shader_set_ivec2(Shader *shader,
                      const char *const name,
                      GLint x, GLint y) {
    glUniform2i(glGetUniformLocation(shader->program, name), x, y);
}

void shader_set_float(Shader *shader,
                      const char *const name,
                      GLfloat value) {
    glUniform1f(glGetUniformLocation(shader->program, name), value);
}
