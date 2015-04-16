#ifndef VERTEX_H
#define VERTEX_H

#include "MathUtils.h"

class Vertex
{
public:
    Vertex();
    Vertex(vec3 p, vec3 n, vec2 t);

    vec3 pos;
    vec3 normal;
    vec2 texCoord;
};

#endif
