#include "ShapeGenerator.h"
#include <cmath>
#include <glm/gtc/constants.hpp>

// Un quadrilatero viene rappresentato da due triangoli.
static void addQuad(MeshData& mesh, unsigned a, unsigned b, unsigned c, unsigned d) {
    mesh.indices.insert(mesh.indices.end(), {a,b,c, a,c,d});
}
MeshData ShapeGenerator::cube() {
    MeshData m;
    const glm::vec3 v[] = {{-1,-1,1},{1,-1,1},{1,1,1},{-1,1,1},
                            {-1,-1,-1},{1,-1,-1},{1,1,-1},{-1,1,-1}};
    const glm::vec4 colors[] = {{1,0,0,1},{0,1,0,1},{0,0,1,1},{1,0,1,1},
                                 {1,1,1,1},{1,1,0,1},{0,1,1,1},{1,0.5f,0,1}};
    for (int i=0;i<8;++i) m.vertices.push_back({v[i],colors[i]});
    addQuad(m,0,1,2,3); addQuad(m,1,5,6,2); addQuad(m,5,4,7,6);
    addQuad(m,4,0,3,7); addQuad(m,4,5,1,0); addQuad(m,3,2,6,7);
    return m;
}
MeshData ShapeGenerator::plane() {
    MeshData m;
    glm::vec4 yellow(1,1,0,1);
    m.vertices = {{{-0.5f,0,0.5f},yellow},{{0.5f,0,0.5f},yellow},
                  {{0.5f,0,-0.5f},yellow},{{-0.5f,0,-0.5f},yellow}};
    addQuad(m,0,1,2,3); return m;
}
MeshData ShapeGenerator::pyramid() {
    MeshData m;
    m.vertices = {{{-1,0,1},{1,0,0,1}},{{1,0,1},{0,1,0,1}},
                  {{1,0,-1},{0,0,1,1}},{{-1,0,-1},{1,1,0,1}},
                  {{0,1,0},{1,1,1,1}}};
    addQuad(m,0,1,2,3);
    m.indices.insert(m.indices.end(),{0,4,1, 1,4,2, 2,4,3, 3,4,0});
    return m;
}
MeshData ShapeGenerator::sphere(int slices,int stacks) {
    MeshData m;
    for (int j=0;j<=stacks;++j) {
        float phi=glm::pi<float>()*j/stacks;
        for (int i=0;i<=slices;++i) {
            float theta=glm::two_pi<float>()*i/slices;
            glm::vec3 v(std::sin(phi)*std::cos(theta),std::cos(phi),
                        std::sin(phi)*std::sin(theta));
            m.vertices.push_back({v,{0.6f,1,0.5f,1}});
        }
    }
    for (int j=0;j<stacks;++j) for (int i=0;i<slices;++i) {
        unsigned a=j*(slices+1)+i, b=a+slices+1;
        m.indices.insert(m.indices.end(),{a,b,a+1, a+1,b,b+1});
    }
    return m;
}
MeshData ShapeGenerator::torus(int slices,int rings) {
    MeshData m;
    const float major=1.0f, minor=0.35f;
    for(int j=0;j<=rings;++j) {
        float v=glm::two_pi<float>()*j/rings;
        for(int i=0;i<=slices;++i) {
            float u=glm::two_pi<float>()*i/slices;
            float r=major+minor*std::cos(v);
            m.vertices.push_back({{r*std::cos(u),minor*std::sin(v),r*std::sin(u)},
                                  {0.1f,0.3f,1.0f,1}});
        }
    }
    for(int j=0;j<rings;++j) for(int i=0;i<slices;++i) {
        unsigned a=j*(slices+1)+i,b=a+slices+1;
        m.indices.insert(m.indices.end(),{a,a+1,b, a+1,b+1,b});
    }
    return m;
}
