//
// Created by arapo on 01.05.2023.
//

#ifndef FH_CG_SHADERS_H
#define FH_CG_SHADERS_H

const char* vShader  = "#version 330\n"
                         "\n"
                         "layout(location = 0)in vec3 vert;\n"
                         "layout(location = 1)in vec2 uv;\n"
                         "layout(location = 2)in vec3 normal;\n"
                         "\n"
                         "out vec3 Normal;\n"
                         "\n"
                         "uniform mat4 rotation;\n"
                           "uniform mat4 scale;\n"
                       "uniform mat4 transform;\n"

                       "\n"
                         ""
                         "void main()\n"
                         "{\n"
                         "    Normal = normal;\n  "
                         "    gl_Position = transform * rotation * scale * vec4(vert, 1.0);\n"
                         "}";

const char *fragmentShader1Source = "#version 330 core\n"
                                    "in vec3 Normal;\n"
                                    "out vec4 fragColor;\n"
                                    "void main()\n"
                                    "{\n"
                                    "    fragColor = vec4(0.5 + 0.5 * Normal, 1.0);\n"
                                    "}\n\0";

#endif //FH_CG_SHADERS_H
