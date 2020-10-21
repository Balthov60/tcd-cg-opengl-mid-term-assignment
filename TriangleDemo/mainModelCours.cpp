// Std. Includes
#include <string>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include <fstream>
#include <string>
#include <vector>

// GL includes

#include "CGUtils.hpp"

using namespace std;


// Assimp includes
#include <assimp/cimport.h> // scene importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations
#include "ShaderProgram.hpp"
#include "VertexBuffer.hpp"
#include "VertexArray.hpp"


/*----------------------------------------------------------------------------
MESH TO LOAD
----------------------------------------------------------------------------*/
// this mesh is a dae file format but you should be able to use any other format too, obj is typically what is used
// put the mesh in your project directory, or provide a filepath for it here
#define MESH_NAME "res/models/monkey/monkeyhead_smooth.dae"
// #define MESH_NAME "res/models/test/Koltuk.obj"
// #define MESH_NAME "res/models/nanosuit.obj"

/*----------------------------------------------------------------------------
----------------------------------------------------------------------------*/

#pragma region SimpleTypes
typedef struct
{
    GLsizei mPointCount = 0;
    std::vector<vec3> mVertices;
    std::vector<vec3> mNormals;
    std::vector<vec2> mTextureCoords;
} ModelData;
#pragma endregion SimpleTypes

using namespace std;

ModelData mesh_data;
unsigned int mesh_vao = 0;
int width = 800;
int height = 600;

GLuint loc1, loc2, loc3;
GLfloat rotate_y = 0.0f;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

/*----------------------------------------------------------------------------
MESH LOADING FUNCTION
----------------------------------------------------------------------------*/

ModelData load_mesh(const char* file_name) {
    ModelData modelData;

    /* Use assimp to read the model file, forcing it to be read as    */
    /* triangles. The second flag (aiProcess_PreTransformVertices) is */
    /* relevant if there are multiple meshes in the model file that   */
    /* are offset from the origin. This is pre-transform them so      */
    /* they're in the right position.                                 */
    const aiScene* scene = aiImportFile(
        file_name,
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

VertexArray * vaoO;
void generateObjectBufferMesh() {
    mesh_data = load_mesh(MESH_NAME);
    
    vector<VertexBuffer> vbos;
    vbos.push_back(VertexBuffer(mesh_data.mVertices));
    vbos.push_back(VertexBuffer(mesh_data.mNormals));
    
    
    vaoO = new VertexArray(vbos);
    vaoO->use();
    
    //    This is for texture coordinates which you don't currently need, so I have commented it out
    //    glEnableVertexAttribArray (loc3);
    //    glBindBuffer (GL_ARRAY_BUFFER, vt_vbo);
    //    glVertexAttribPointer (loc3, 2, GL_FLOAT, GL_FALSE, 0, NULL);
}

int main( )
{
    
    GLFWwindow * window = CGUtils::GetInstance().initAndGetWindow();
    
    //init function
    ShaderProgram shaderProgram("simpleVertexShader.txt", "simpleFragmentShader.txt");
    generateObjectBufferMesh();
    shaderProgram.validate();
    
    // Game loop
    while(!glfwWindowShouldClose(window))
    {
        /* Idle */
        GLfloat currentFrame = glfwGetTime();
        if (lastFrame == 0) {
            lastFrame = currentFrame;
        }
        float delta = (currentFrame - lastFrame);
        lastFrame = currentFrame;

        // Rotate the model slowly around the y axis at 20 degrees per second
        rotate_y += 20.0f * delta;
        rotate_y = fmodf(rotate_y, 360.0f);
        
        
        /* Event */
        glfwPollEvents();
        
        /* Display */
        glClearColor(0.2f, radians(rotate_y / 10), 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        shaderProgram.use();

        // Root of the Hierarchy
        mat4 view = identity<mat4>();
        mat4 persp_proj = perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
        mat4 model = identity<mat4>();
        model = rotate(model, radians(rotate_y), vec3(0, 0, 1));
        view = translate(view, vec3(0.0, 0.0, -10.0f));

        // update uniforms
        shaderProgram.linkMatrixUniformVariable(persp_proj, "proj");
        shaderProgram.linkMatrixUniformVariable(view, "view");
        shaderProgram.linkMatrixUniformVariable(model, "model");

        // Draw
        glDrawArrays(GL_TRIANGLES, 0, mesh_data.mPointCount);

        
        // Set up the child matrix
        mat4 modelChild = identity<mat4>();
        modelChild = rotate(modelChild, radians(180.0f), vec3(0, 0, 1));
        modelChild = rotate(modelChild, radians(rotate_y), vec3(0, 1, 0));
        modelChild = translate(modelChild, vec3(0.0f, -1.9f, 0.0f));

        // Apply the root matrix to the child matrix
        modelChild = model * modelChild;

        // Update the appropriate uniform and draw the mesh again
        shaderProgram.linkMatrixUniformVariable(modelChild, "model");

        glDrawArrays(GL_TRIANGLES, 0, mesh_data.mPointCount);
         
        glfwSwapBuffers(window);
    }
    
    glfwTerminate();
    
    return 0;
}
