#ifndef MESH_H
#define MESH_H

#include "includers.h"

using namespace std;

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture {
    unsigned int id;
    string type;
    string path;
};

struct Material {
    glm::vec3 Diffuse;
    glm::vec3 Specular;
    glm::vec3 Ambient;
    float Shininess;
};

class Mesh {
public:
    vector<Vertex>       vertices;
    vector<unsigned int> indices;
    vector<Texture>      textures;
    Material material;


    bool txtDifuse = false;
    bool txtSpecular = false;
    bool txtNormal = false;
   
 
    unsigned int VAO;

    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures,Material material)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;
        this->material = material;

        setupMesh();
    }

    void Draw(Shader& shader)
    {

     
        for (unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); 
            string name = textures[i].type;
            if (name == "texture_diffuse")
                txtDifuse = true;
            if (name == "texture_specular")
                txtSpecular = true;
            if (name == "texture_normal")
                txtNormal = true;
           
            glUniform1i(glGetUniformLocation(shader.ID, (name).c_str()), i);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }

        shader.setBool("txtDiffuse", txtDifuse);
        shader.setBool("txtSpecular", txtSpecular);
        shader.setBool("txtNormal", txtNormal);
        shader.setVec3("material.Diffuse", material.Diffuse);
        shader.setVec3("material.Specular", material.Specular);
        shader.setVec3("material.Specular", material.Ambient);
        shader.setFloat("material.Shininess", material.Shininess);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glActiveTexture(GL_TEXTURE0);
    }

private:
    unsigned int VBO, EBO;

    void setupMesh()
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
       
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

        glBindVertexArray(0);
    }
};
#endif