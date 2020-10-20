//
//  Model.hpp
//  TriangleDemo
//
//  Created by Balthazar Frolin on 19/10/2020.
//

#pragma

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.hpp"
#include "CGUtils.hpp"

class Model
{
public:
    
    Model(string path) {
        this->loadModel(MODEL_FOLDER_PATH + path);
    }
    
    void draw(ShaderProgram shaderProgram)
    {
        for (int i = 0; i < meshes.size(); i++) {
            this->meshes[i].draw(shaderProgram);
        }
    }
    
private:
    const static string MODEL_FOLDER_PATH;
    
    vector<Mesh> meshes;
    
    void loadModel(string path)
    {
        /* The second flag (aiProcess_PreTransformVertices) is relevant if there are multiple meshes in the model file that are offset from the origin. This is pre-transform them so they're in the right */
        GLuint flags = aiProcess_Triangulate | aiProcess_PreTransformVertices;
        const aiScene* scene = aiImportFile(path.c_str(), flags);

        if (!scene) {
            fprintf(stderr, "ERROR: reading mesh %s\n", path.c_str());
            exit(1);
        }

        printf("  %i materials\n", scene->mNumMaterials);
        printf("  %i meshes\n", scene->mNumMeshes);
        printf("  %i textures\n", scene->mNumTextures);

        for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
            aiMesh * mesh = scene->mMeshes[i];
            printf("    %i vertices in mesh %i\n", mesh->mNumVertices, i);
            createMesh(mesh);
        }

        aiReleaseImport(scene);
    }
    void createMesh(aiMesh* aiMesh)
    {
        MeshData meshData;

        meshData.verticesCount = aiMesh->mNumVertices;
        for (unsigned int v_i = 0; v_i < aiMesh->mNumVertices; v_i++) {
            if (aiMesh->HasPositions()) {
                const aiVector3D* vp = &(aiMesh->mVertices[v_i]);
                meshData.vertices.push_back(vec3(vp->x, vp->y, vp->z));
            }
            if (aiMesh->HasNormals()) {
                const aiVector3D* vn = &(aiMesh->mNormals[v_i]);
                meshData.normals.push_back(vec3(vn->x, vn->y, vn->z));
            }
            if (aiMesh->HasTextureCoords(0)) {
                const aiVector3D* vt = &(aiMesh->mTextureCoords[0][v_i]);
                meshData.textureCoords.push_back(vec2(vt->x, vt->y));
            }
            if (aiMesh->HasTangentsAndBitangents()) {
                /* You can extract tangents and bitangents here              */
                /* Note that you might need to make Assimp generate this     */
                /* data for you. Take a look at the flags that aiImportFile  */
                /* can take.                                                 */
            }
            
        }
    }
};

const string Model::MODEL_FOLDER_PATH = "res/models/";
