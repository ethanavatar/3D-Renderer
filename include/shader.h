#ifndef DL_SHADER_H
#define DL_SHADER_H

#include <glad/glad.h>

typedef struct {
    GLuint program;
} Shader;

Shader *shader_create(const char *const vertex_source,
                     const char *const fragment_source);

void shader_destroy(Shader *shader);

void shader_use(Shader *shader);

void shader_set_ivec2(Shader *shader,
                      const char *const name,
                      GLint x, GLint y);

void shader_set_float(Shader *shader,
                      const char *const name,
                      GLfloat value);

#endif // !DL_SHADER_H
