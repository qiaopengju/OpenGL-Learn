#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

//#define STB_IMAGE_IMPLEMENTATION
#include "deps/stb_image.h"  // image depends lib

using namespace std;

static unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

class Model{
public:
    vector<Texture> textures_loaded;    // 已加载的贴图
    vector<Mesh> meshes;
    string directory;
    bool gammaCorrection;               // gammma校正

    Model();
    Model(string const &path, bool gamma = false);
    void Draw(Shader &shader);
private:
    /* 加载模型，将Assimp数据结构转换为我们的数据结构 */
    void loadModel(string const &path);

    /* 递归处理每个Assimp节点 */
    void processNode(aiNode *node, const aiScene *scene);

    /* 将Assimp模型数据转换为我们的模型数据 */
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);

    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
};

#endif