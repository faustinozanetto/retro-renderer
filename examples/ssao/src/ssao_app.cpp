#include "ssao_app.h"

#include <glm/ext/matrix_transform.hpp>
#include <random>

float lerp(float a, float b, float f)
{
    return a + f * (b - a);
}

ssao_app::ssao_app() : application("./")
{
    load_shaders();
    load_texture();
    setup_model();
    setup_camera();
    setup_ssao();
}

ssao_app::~ssao_app()
{
}

void ssao_app::on_update()
{
    m_shader->bind();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, static_cast<float>(glfwGetTime()), {0, 1, 0});
    m_shader->set_mat4("u_transform", model);
    m_shader->set_mat4("u_view", m_camera->get_view_matrix());
    m_shader->set_mat4("u_projection", m_camera->get_projection_matrix());
    retro::renderer::renderer::bind_texture(0, m_texture->get_handle_id());
    retro::renderer::renderer::submit_model(m_model);
    m_shader->un_bind();
}

void ssao_app::load_shaders()
{
    const std::string &shader_contents = retro::renderer::shader_loader::read_shader_from_file(
        "../resources/shaders/model-textured.rrs");
    const auto &shader_sources = retro::renderer::shader_loader::parse_shader_source(shader_contents);
    m_shader = std::make_shared<retro::renderer::shader>(shader_sources);
}

void ssao_app::load_texture()
{
    m_texture = std::make_shared<retro::renderer::texture>("../resources/models/tv/tv-albedo.png");
}

void ssao_app::setup_model()
{
    m_model = retro::renderer::model_loader::load_model_from_file("../resources/models/tv/tv.obj");
}

void ssao_app::setup_camera()
{
    m_camera = std::make_shared<retro::camera::camera>(retro::camera::camera_type::perspective, 45.0f, 0.01f, 1000.0f);
    m_camera->set_position({0.0f, 0.5f, 12.0f});
}

void ssao_app::setup_ssao()
{
    // 1. Setup ssao kernel
    std::uniform_real_distribution<float> random_floats(0.0, 1.0);
    std::default_random_engine generator;

    for (int i = 0; i < 64; i++)
    {
        glm::vec3 sample(random_floats(generator) * 2.0 - 1.0, random_floats(generator) * 2.0 - 1.0, random_floats(generator));

        sample = glm::normalize(sample);
        sample *= random_floats(generator);

        float scale = (float)i / 64.0;
        scale = lerp(0.1f, 1.0f, scale * scale);
        sample *= scale;

        m_ssao_kernel.push_back(sample);
    }

    // 2. Setup ssao noise
    std::vector<glm::vec3> ssao_noise;

    for (int i = 0; i < 16; i++)
    {
        glm::vec3 noise(random_floats(generator) * 2.0 - 1.0, random_floats(generator) * 2.0 - 1.0, 0.0);
        ssao_noise.push_back(noise);
    }

    // Create noise texture
    glGenTextures(1, &m_ssao_noise_texture);
    glBindTexture(GL_TEXTURE_2D, m_ssao_noise_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssao_noise[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // 3. Setup ssao buffer
    glGenFramebuffers(1, &m_ssao_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_ssao_fbo);

    glm::vec2 viewport_size = retro::renderer::renderer::get_viewport_size();
    glGenTextures(1, &m_ssao_color_buffer);
    glBindTexture(GL_TEXTURE_2D, m_ssao_color_buffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, viewport_size.x, viewport_size.y, 0, GL_RED, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

retro::core::application *retro::core::create_application()
{
    return new ssao_app();
}
