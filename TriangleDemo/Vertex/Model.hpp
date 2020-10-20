//
//  Model.hpp
//  TriangleDemo
//
//  Created by Balthazar Frolin on 19/10/2020.
//

#pragma

#include "SOIL2.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.hpp"
#include "CGUtils.hpp"

using namespace std;

GLint TextureFromFile( const char *path, string directory );

class Model
{
public:
    
    /**
            Load model from modelPath (path is related to model source path)
     */
    Model(GLchar *path) {
        this->loadModel(MODEL_FOLDER_PATH + path);
    }
    
    /**
            Draw all mesh in the model
     */
    void draw(ShaderProgram shaderProgram)
    {
        for (GLuint i = 0; i < this->meshes.size(); i++) {
            this->meshes[i].draw(shaderProgram);
        }
    }
    
private:
    const static string MODEL_FOLDER_PATH;

    /*  Model Data  */
    vector<Mesh> meshes;
    string directory;
    vector<Texture> textures_loaded;
    
    void loadModel(string path)
    {
        ModelData modelData;

        /* The second flag (aiProcess_PreTransformVertices) is relevant if there are multiple meshes in the model file that are offset from the origin. This is pre-transform them so they're in the right */
        GLuint flags = aiProcess_Triangulate | aiProcess_PreTransformVertices
        const aiScene* scene = aiImportFile(file_name,
            aiProcess_Triangulate | aiProcess_PreTransformVertices
        );

        if (!scene) {
            fprintf(stderr, "ERROR: reading mesh %s\n", file_name);
            return modelData;
        }

        printf("  %i materials\n", scene->mNumMaterials);
        printf("  %i meshes\n", scene->mNumMeshes);
        printf("  %i textures\n", scene->mNumTextures);

        for (unsigned int m_i = 0; m_i < scene->mNumMeshes; m_i++) {
            const aiMesh* mesh = scene->mMeshes[m_i];
            printf("    %i vertices in mesh\n", mesh->mNumVertices);
            modelData.mPointCount += mesh->mNumVertices;
            for (unsigned int v_i = 0; v_i < mesh->mNumVertices; v_i++) {
                if (mesh->HasPositions()) {
                    const aiVector3D* vp = &(mesh->mVertices[v_i]);
                    modelData.mVertices.push_back(vec3(vp->x, vp->y, vp->z));
                }
                if (mesh->HasNormals()) {
                    const aiVector3D* vn = &(mesh->mNormals[v_i]);
                    modelData.mNormals.push_back(vec3(vn->x, vn->y, vn->z));
                }
                if (mesh->HasTextureCoords(0)) {
                    const aiVector3D* vt = &(mesh->mTextureCoords[0][v_i]);
                    modelData.mTextureCoords.push_back(vec2(vt->x, vt->y));
                }
                if (mesh->HasTangentsAndBitangents()) {
                    /* You can extract tangents and bitangents here              */
                    /* Note that you might need to make Assimp generate this     */
                    /* data for you. Take a look at the flags that aiImportFile  */
                    /* can take.                                                 */
                }
                
            }
        }

        aiReleaseImport(scene);
        return modelData;
    }
    
    // Processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode( aiNode* node, const aiScene* scene )
    {
        // Process each mesh located at the current node
        cout << "load node with : " << node->mNumMeshes << " meshes" << endl;
        for ( GLuint i = 0; i < node->mNumMeshes; i++ )
        {
            // The node object only contains indices to index the actual objects in the scene.
            // The scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            
            this->meshes.push_back( this->processMesh( mesh, scene ) );
        }
        
        // After we've processed all of the meshes (if any) we then recursively process each of the children nodes
        for ( GLuint i = 0; i < node->mNumChildren; i++ )
        {
            this->processNode( node->mChildren[i], scene );
        }
    }
    
    Mesh processMesh( aiMesh *mesh, const aiScene *scene )
    {
        // Data to fill
        vector<Vertex> vertices;
        vector<GLuint> indices;
        vector<Texture> textures;
        
        // Walk through each of the mesh's vertices
        for ( GLuint i = 0; i < mesh->mNumVertices; i++ )
        {
            Vertex vertex;
            glm::vec3 vector; // We declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
            
            // Positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.position = vector;
            
            // Normals
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;
            
            // Texture Coordinates
            if( mesh->mTextureCoords[0] ) // Does the mesh contain texture coordinates?
            {
                glm::vec2 vec;
                // A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
                // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.texCoords = vec;
            }
            else
            {
                vertex.texCoords = glm::vec2( 0.0f, 0.0f );
            }
            
            vertices.push_back( vertex );
        }
        
        // Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
        for ( GLuint i = 0; i < mesh->mNumFaces; i++ )
        {
            aiFace face = mesh->mFaces[i];
            // Retrieve all indices of the face and store them in the indices vector
            for ( GLuint j = 0; j < face.mNumIndices; j++ )
            {
                indices.push_back( face.mIndices[j] );
            }
        }
        
        // Process materials
        if( mesh->mMaterialIndex >= 0 )
        {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            // We assume a convention for sampler names in the shaders. Each diffuse texture should be named
            // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
            // Same applies to other texture as the following list summarizes:
            // Diffuse: texture_diffuseN
            // Specular: texture_specularN
            // Normal: texture_normalN
            
            // 1. Diffuse maps
            vector<Texture> diffuseMaps = this->loadMaterialTextures( material, aiTextureType_DIFFUSE, "texture_diffuse" );
            textures.insert( textures.end( ), diffuseMaps.begin( ), diffuseMaps.end( ) );
            
            // 2. Specular maps
            vector<Texture> specularMaps = this->loadMaterialTextures( material, aiTextureType_SPECULAR, "texture_specular" );
            textures.insert( textures.end( ), specularMaps.begin( ), specularMaps.end( ) );
        }
        
        // Return a mesh object created from the extracted mesh data
        return Mesh( vertices, indices, textures );
    }
    
    // Checks all material textures of a given type and loads the textures if they're not loaded yet.
    // The required info is returned as a Texture struct.
    vector<Texture> loadMaterialTextures( aiMaterial *mat, aiTextureType type, string typeName )
    {
        vector<Texture> textures;
        
        for ( GLuint i = 0; i < mat->GetTextureCount( type ); i++ )
        {
            aiString str;
            mat->GetTexture( type, i, &str );
            
            // Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
            GLboolean skip = false;
            
            for ( GLuint j = 0; j < textures_loaded.size( ); j++ )
            {
                if( textures_loaded[j].path == str )
                {
                    textures.push_back( textures_loaded[j] );
                    skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
                    
                    break;
                }
            }
            
            if( !skip )
            {   // If texture hasn't been loaded already, load it
                Texture texture;
                texture.id = TextureFromFile( str.C_Str( ), this->directory );
                texture.type = typeName;
                texture.path = str;
                textures.push_back( texture );
                
                this->textures_loaded.push_back( texture );  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
            }
        }
        
        return textures;
    }
};

GLint TextureFromFile( const char *path, string directory )
{
    //Generate texture ID and load texture data
    string filename = string( path );
    filename = directory + '/' + filename;
    GLuint textureID;
    glGenTextures( 1, &textureID );
    
    int width, height;
    
    unsigned char *image = SOIL_load_image( filename.c_str( ), &width, &height, 0, SOIL_LOAD_RGB );
    
    // Assign texture to ID
    glBindTexture( GL_TEXTURE_2D, textureID );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
    glGenerateMipmap( GL_TEXTURE_2D );
    
    // Parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture( GL_TEXTURE_2D, 0 );
    SOIL_free_image_data( image );
    
    return textureID;
}

const string Model::MODEL_FOLDER_PATH = "res/models/";
