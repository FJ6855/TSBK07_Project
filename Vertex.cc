#include "Vertex.h"

Vertex::Vertex()
{
    pos = vec3();
    normal = vec3();
    texCoord = vec2();
}

Vertex::Vertex(vec3 p, vec3 n, vec2 t)
{
    pos = p;
    normal = n;
    texCoord = t;
}
