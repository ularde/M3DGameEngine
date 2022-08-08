#include "pch.h"
#include "Test.h"


GLuint VAO, VBO, EBO;

glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(0.0f,  0.0f, 0.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

void InitTest(MBasicPlatform* platform) {
    glBindProgramPipeline(platform->pipelineID);

	vs = new MVertexShaderProgram(platform, "Shaders\\test_vert.glsl");
	fs = new MFragmentShaderProgram(platform, "Shaders\\test_frag.glsl");

	vs->UseProgramStage();
    fs->UseProgramStage();
    platform->camera = new MFreeCamera(platform, vs, fs, 2.0f, glm::vec3(0.0f, 0.0f, 30.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    float vertices[] = {
         1.0f,  1.0f, 0.0f,  // top right
         1.0f, -1.0f, 0.0f,  // bottom right
        -1.0f, -1.0f, 0.0f,  // bottom left
        -1.0f,  1.0f, 0.0f   // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
    //glBindProgramPipeline(0);

}

void RunTest(MBasicPlatform* platform) {
    static float z = 0.0f;
    z += 0.3f;
    // pass projection matrix to shader (note that in this case it could change every frame)
    //glEnable(GL_DEPTH_TEST);
    glBindProgramPipeline(platform->pipelineID);
    vs->UseProgramStage();
    fs->UseProgramStage();

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)platform->GetWindowWidth() / (float)platform->GetWindowHeight(), 0.1f, 100.0f);
    vs->UniformMat4("projection", projection);

    glm::mat4 view = platform->camera->GetViewMatrix();
    vs->UniformMat4("view", view);
    platform->camera->ProcessMovement();


    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(z), glm::vec3(0.0f, 1.0f, 0.0f));
    vs->UniformMat4("model", model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glm::mat4 model2 = glm::mat4(1.0f);
    model2 = glm::translate(model2, glm::vec3(5.0f, 0.0f, 0.0f));
    model2 = glm::rotate(model2, glm::radians(z), glm::vec3(0.0f, 1.0f, 0.0f));
    vs->UniformMat4("model", model2);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glm::mat4 model3 = glm::mat4(1.0f);
    model3 = glm::translate(model3, glm::vec3(10.0f, 0.0f, 0.0f));
    model3 = glm::rotate(model3, glm::radians(z), glm::vec3(0.0f, 1.0f, 0.0f));
    vs->UniformMat4("model", model3);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    model3 = glm::mat4(1.0f);
    model3 = glm::translate(model3, glm::vec3(0.0f, 5.0f, 0.0f));
    model3 = glm::rotate(model3, glm::radians(z), glm::vec3(0.0f, 1.0f, 0.0f));
    vs->UniformMat4("model", model3);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    model3 = glm::mat4(1.0f);
    model3 = glm::translate(model3, glm::vec3(5.0f, 5.0f, 0.0f));
    model3 = glm::rotate(model3, glm::radians(z), glm::vec3(0.0f, 1.0f, 0.0f));
    vs->UniformMat4("model", model3);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    model3 = glm::mat4(1.0f);
    model3 = glm::translate(model3, glm::vec3(10.0f, 5.0f, 0.0f));
    model3 = glm::rotate(model3, glm::radians(z), glm::vec3(0.0f, 1.0f, 0.0f));
    vs->UniformMat4("model", model3);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindProgramPipeline(0);
}
