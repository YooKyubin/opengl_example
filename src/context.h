#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "common.h"
#include "shader.h"
#include "program.h"
#include "buffer.h"
#include "vertex_layout.h"
#include "texture.h"
#include "mesh.h"
#include "model.h"
#include "framebuffer.h"
#include "shadow_map.h"


CLASS_PTR(Context)
class Context {
public:
    static ContextUPtr Create();
    void Render();    
    void ProcessInput(GLFWwindow* window);
    void Reshape(int width, int height);
    void MouseMove(double x, double y);
    void MouseButton(int button, int action, double x, double y);
    
    void DrawScene(const glm::mat4& view, const glm::mat4& projection, Program* program);

private:
    Context() {}
    bool Init();
    ProgramUPtr m_program;
    ProgramUPtr m_simpleProgram;
    ProgramUPtr m_boxProgram;
    ProgramUPtr m_textureProgram;
    ProgramUPtr m_postProgram;
    float m_gamma {1.0f};


    // VertexLayoutUPtr m_vertexLayout;
    // BufferUPtr m_vertexBuffer;
    // BufferUPtr m_indexBuffer;
    MeshUPtr m_box;
    MeshUPtr m_plane;

    // animation
	bool m_animation { true };

    // depth test
    bool m_depthTest {true};

    // clear color
    glm::vec4 m_clearColor { glm::vec4(1.0f, 0.85f, 0.89f, 1.0f) };

    // light parameter
    struct Light {
        bool directional { false };
        glm::vec3 position { glm::vec3(2.0f, 4.0f, 4.0f) };
        glm::vec3 direction { glm::vec3(-0.5f, -1.5f, -1.0f) };
        glm::vec2 cutoff { glm::vec2(50.0f, 5.0f) };
        float distance { 150.0f };
        glm::vec3 ambient { glm::vec3(0.1f, 0.1f, 0.1f) };
        glm::vec3 diffuse { glm::vec3(0.8f, 0.8f, 0.8f) };
        glm::vec3 specular { glm::vec3(1.0f, 1.0f, 1.0f) };
    };
    Light m_light;
    bool m_flashLightMode { false };
    bool m_blinn { true };

    // material parameter
    MaterialPtr m_planeMaterial;  
	MaterialPtr m_box1Material;  
	MaterialPtr m_box2Material;
    TexturePtr m_windowTexture; // 단일 텍스처만 설정하면 되기 때문에 Material일 필요가 없음

    // camera parameter
    glm::vec3 m_cameraPos { glm::vec3(0.0f, 2.5f, 8.0f) };
    glm::vec3 m_cameraFront { glm::vec3(0.0f, 0.0f, -1.0f) };
    glm::vec3 m_cameraUp { glm::vec3(0.0f, 1.0f, 0.0f) };
    float m_cameraPitch { -20.0f };
	float m_cameraYaw { 0.0f };
    bool m_cameraControl { false };
	glm::vec2 m_prevMousePos { glm::vec2(0.0f) };

    // framebuffer
    FramebufferUPtr m_framebuffer;
    
    // cube map
    CubeTextureUPtr m_cubeTexture;
	ProgramUPtr m_skyboxProgram;
    ProgramUPtr m_envMapProgram;

    // grass
    TexturePtr m_grassTexture;
	ProgramUPtr m_grassProgram;
	std::vector<glm::vec3> m_grassPos;
    BufferUPtr m_grassPosBuffer;
	VertexLayoutUPtr m_grassInstance;

    // shadow map
    ShadowMapUPtr m_shadowMap;
    ProgramUPtr m_lightingShadowProgram;

    // normal map
	TextureUPtr m_brickDiffuseTexture;
	TextureUPtr m_brickNormalTexture;
	ProgramUPtr m_normalProgram;

    int m_width { WINDOW_WIDTH };
    int m_height { WINDOW_HEIGHT };

    // deferred shading
	FramebufferUPtr m_deferGeoFramebuffer;
	ProgramUPtr m_deferGeoProgram;
    ProgramUPtr m_deferLightProgram;

	struct DeferLight {
	    glm::vec3 position;
	    glm::vec3 color;
	};
	std::vector<DeferLight> m_deferLights;

    // ssao
	FramebufferUPtr m_ssaoFramebuffer;
	ProgramUPtr m_ssaoProgram;
	ModelUPtr m_model;  // for test rendering
    TextureUPtr m_ssaoNoiseTexture;
};

#endif // __CONTEXT_H__