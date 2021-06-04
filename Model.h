#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//#define STB_IMAGE_IMPLEMENTATION
//#include "deps/stb_image.h"  // image depends lib

#include "Mesh.h"
#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

static unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

class Model{
public:
    vector<Texture> textures_loaded;    // 已加载的贴图
    vector<Mesh> meshes;
    string directory;
    bool gammaCorrection;               // gammma校正

    Model(string const &path, bool gamma = false) : gammaCorrection(gamma){
        loadModel(path);
    }
    void Draw(Shader &shader){
        for (int i = 0;i < meshes.size(); i++) meshes[i].Draw(shader);
    }
private:
    /* 加载模型，将Assimp数据结构转换为我们的数据结构 */
    void loadModel(string const &path){
        // read file via ASSIMP
        Assimp::Importer import;
        // face 2 triangle & flip UV & caculate normals, tangent, bitangent
        const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_CalcTangentSpace);
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
            cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
            return;
        }
        directory = path.substr(0, path.find_last_of('/'));
        // 处理节点
        processNode(scene->mRootNode, scene);
    }

    /* 递归处理每个Assimp节点 */
    void processNode(aiNode *node, const aiScene *scene){
        // 处理节点所有Mesh
        for (int i = 0; i < node->mNumMeshes; i++){
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        // 递归遍历子节点
        for (int i = 0; i < node->mNumChildren; i++){
            processNode(node->mChildren[i], scene);
        }
    }

    /* 将Assimp模型数据转换为我们的模型数据 */
    Mesh processMesh(aiMesh *mesh, const aiScene *scene){
        // data to fill
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture> textures;
        // 遍历网络所有节点
        for (int i = 0; i < mesh->mNumVertices; i++){ 
            Vertex vertex;
            glm::vec3 vector;
            // positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            // normals
            if (mesh->HasNormals()){
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }
            // texture coordinates
            if (mesh->mTextureCoords[0]){
                glm::vec2 vec;
                // a vertex dcan contain up to 8 different texture coordinates. 
                // We thus make the assumption that we just use one coord
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
                // tangent
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.Tangent = vector;
                // bitangent
                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.Bitangent = vector;

            } else{ // 如果没有贴图坐标就将贴图坐标置零
                vertex.TexCoords = glm::vec2(0.f);
            }

            vertices.push_back(vertex); // 存入节点
        }
        // 遍历所有face，得到索引
        for (int i = 0; i < mesh->mNumFaces; i++){
            aiFace face = mesh->mFaces[i];
            // 存储该面的所有索引
            for (int j  = 0; j < face.mNumIndices; j++){
                indices.push_back(face.mIndices[j]);
            }
        }
        // 处理材质
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        // Assume sampler name in shaders:
        // Diffuse: material.texture_diffuseN
        // Specular: material.texture_specularN
        // Normal: material.texture_normalN

        // 1.diffuse maps
        vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2.specular maps
        vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        // 3.normal maps
        vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "texture_map");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        // 4.height maps
        vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

        return Mesh(vertices, indices, textures);
    }

    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName){
        vector<Texture> textures;
        for (int i = 0; i < mat->GetTextureCount(type); i++){ // 加载所有该类型的texture
            aiString str;
            mat->GetTexture(type, i, &str);
            /* !!! 优化：查看贴图之前是否加载过，只有未加载过的贴图会被加载 */
            bool skip = false;
            for (int j = 0; j < textures_loaded.size(); j++){
                if (strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0){ // 如果之前加载过该贴图
                    textures.push_back(textures_loaded[j]); // 加载之前加载过的texture
                    skip = true;
                    break;
                }
            }
            if (!skip){ // 之前未加载过，加载该贴图
                Texture texture;
                texture.id = TextureFromFile(str.C_Str(), this->directory);
                texture.path = str.C_Str();
                texture.type = typeName;
                textures.push_back(texture);
                textures_loaded.push_back(texture);
            }
        }
        return textures;
    }
};

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma){
    string filename = string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data  = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data){
        GLenum format;
        if (nrComponents == 1) format = GL_RED;
        else if (nrComponents == 3) format = GL_RGB;
        else if (nrComponents == 4) format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        /* set the texture wrapping parameters 设置超出纹理坐标的环绕方式*/
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        /* set teh texture filtering paramenters 设置采样参数 */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else{
        cout << "Texture filed to load at path: " << path << endl;
        stbi_image_free(data);
    }
    return textureID;
}
#endif