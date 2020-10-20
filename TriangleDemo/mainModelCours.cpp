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


/*----------------------------------------------------------------------------
MESH TO LOAD
----------------------------------------------------------------------------*/
// this mesh is a dae file format but you should be able to use any other format too, obj is typically what is used
// put the mesh in your project directory, or provide a filepath for it here
#define MESH_NAME "res/models/monkey/monkeyhead_smooth.dae"
// #define MESH_NAME "res/models/test/Koltuk.obj"
 #define MESH_NAME "res/models/nanosuit.obj"

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
GLuint vao;

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

void generateObjectBufferMesh(ShaderProgram shaderProgram) {
    /*----------------------------------------------------------------------------
    LOAD MESH HERE AND COPY INTO BUFFERS
    ----------------------------------------------------------------------------*/

    //Note: you may get an error "vector subscript out of range" if you are using this code for a mesh that doesnt have positions and normals
    //Might be an idea to do a check for that before generating and binding the buffer.

    mesh_data = load_mesh(MESH_NAME);
    unsigned int vp_vbo = 0;
    loc1 = glGetAttribLocation(shaderProgram.program, "vertex_position");
    loc2 = glGetAttribLocation(shaderProgram.program, "vertex_normal");
    loc3 = glGetAttribLocation(shaderProgram.program, "vertex_texture");

    glGenBuffers(1, &vp_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vp_vbo);
    
    glBufferData(GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof(vec3), &mesh_data.mVertices[0], GL_STATIC_DRAW);
    unsigned int vn_vbo = 0;
    glGenBuffers(1, &vn_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vn_vbo);
    glBufferData(GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof(vec3), &mesh_data.mNormals[0], GL_STATIC_DRAW);

    //    This is for texture coordinates which you don't currently need, so I have commented it out
    //    unsigned int vt_vbo = 0;
    //    glGenBuffers (1, &vt_vbo);
    //    glBindBuffer (GL_ARRAY_BUFFER, vt_vbo);
    //    glBufferData (GL_ARRAY_BUFFER, monkey_head_data.mTextureCoords * sizeof (vec2), &monkey_head_data.mTextureCoords[0], GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao);

    glBindBuffer(GL_ARRAY_BUFFER, vp_vbo);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(loc1);
    glVertexAttribPointer(loc1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    
    glBindBuffer(GL_ARRAY_BUFFER, vn_vbo);
    glBindVertexArray(vao);
    
    glEnableVertexAttribArray(loc2);
    glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

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
    generateObjectBufferMesh(shaderProgram);
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
