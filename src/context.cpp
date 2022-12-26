#include "context.h"
// #include "image.h"

ContextUPtr Context::Create() {
    auto context = ContextUPtr(new Context());
    if (!context->Init())
        return nullptr;
    return std::move(context);
}

bool Context::Init() {

    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
        0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
        0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

        0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
        0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
    };

    uint32_t indices[] = {
        0,  2,  1,  2,  0,  3,
        4,  5,  6,  6,  7,  4,
        8,  9, 10, 10, 11,  8,
        12, 14, 13, 14, 12, 15,
        16, 17, 18, 18, 19, 16,
        20, 22, 21, 22, 20, 23,
    };

    m_vertexLayout = VertexLayout::Create();

    m_vertexBuffer = Buffer::CreateWithData(GL_ARRAY_BUFFER, GL_STATIC_DRAW,
        vertices, sizeof(float)* 120);
        // vertices, sizeof(vertices));

    m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
    m_vertexLayout->SetAttrib(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, sizeof(float) * 3);

    m_indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW,
        indices, sizeof(uint32_t)*36);
    
    // Shader Class
    ShaderPtr vertShader = Shader::CreateFromFile("./shader/texture.vs", GL_VERTEX_SHADER);
    ShaderPtr fragShader = Shader::CreateFromFile("./shader/texture.fs", GL_FRAGMENT_SHADER);
    if (!vertShader || !fragShader)
        return false;
    SPDLOG_INFO("vertex shader id: {}", vertShader->Get());
    SPDLOG_INFO("fragment shader id: {}", fragShader->Get());
    
    // Program
    m_program = Program::Create({fragShader, vertShader});
    if (!m_program)
        return false;
    SPDLOG_INFO("program id: {}", m_program->Get()); 

    // auto loc = glGetUniformLocation(m_program->Get(), "color");
    // m_program->Use(); // 꼭 while문 안에 (Render 함수 안에) 있지 않아도 문제 없는 것 같다? uniform 사용 전에만 하면 되나봄
    // glUniform4f(loc, 1.0f, 1.0f, 0.0f, 1.0f);

    glClearColor(1.0f, 0.85f, 0.89f, 1.0f);
 
    auto image = Image::Load("./image/container.jpg");

    // auto image = Image::Create(512, 512);
    // image->SetCheckImage(16, 16);

    if (!image) 
        return false;
    SPDLOG_INFO("image: {}x{}, {} channels",
        image->GetWidth(), image->GetHeight(), image->GetChannelCount());

    m_texture = Texture::CreateFromImage(image.get());

    auto image2 = Image::Load("./image/awesomeface.png");
    m_texture2 = Texture::CreateFromImage(image2.get());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture->Get());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_texture2->Get());

    m_program->Use();
    // glUniform1i(glGetUniformLocation(m_program->Get(), "tex"), 0);
    // glUniform1i(glGetUniformLocation(m_program->Get(), "tex2"), 1);
    m_program->SetUniform("tex", 0);
    m_program->SetUniform("tex2", 1);

    // // x축으로 -55도 회전
    // auto model = glm::rotate(glm::mat4(1.0f), glm::radians(55.0f), glm::vec3(1.0f, 2.0f, 0.0f));
    // // 카메라는 원점으로부터 z축 방향으로 -3만큼 떨어짐
    // auto view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
    // // 종횡비 4:3, 세로화각 45도의 원근 투영
    // auto projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.01f, 10.0f);

    // auto transform = projection * view * model;
    // auto transformLoc = glGetUniformLocation(m_program->Get(), "transform");
    // glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
    // m_program->SetUniform("transform", transform);

    return true;
}

void Context::Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

    m_program->Use();

    std::vector<glm::vec3> cubePositions = {
        glm::vec3( 0.0f, 0.0f, 0.0f),
        glm::vec3( 2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f, 2.0f, -2.5f),
        glm::vec3( 1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f),
    };

    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // projection
    auto projection = glm::perspective(glm::radians(45.0f),
        (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.01f, 30.0f); //(2.2f, 15.0f)nearplane, farplane에 딱 걸침
    
    // view
    float angle = glfwGetTime() * glm::pi<float>() * 0.5f;
    float x = sinf(angle) * 10.0f;
    float z = cosf(angle) * 10.0f;
    glm::vec3 cameraPos = glm::vec3(x, 0.0f, z);
    auto cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    auto cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    
    auto view = glm::lookAt(cameraPos, cameraTarget, cameraUp);

    // model
    for (size_t i = 0; i < cubePositions.size(); i++){
        auto& pos = cubePositions[i];
        auto model = glm::translate(glm::mat4(1.0f), pos);
        model = glm::rotate(model,
            // glm::radians((float)glfwGetTime() * 120.0f + 20.0f * (float)i),
            glm::radians((float)glfwGetTime() * 120.0f + (float)i * 20.0f),
            glm::vec3(1.0f, 0.5f, 0.0f));
        auto transform = projection * view * model;
        // auto transform = projection * model * view;
        m_program->SetUniform("transform", transform);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
}