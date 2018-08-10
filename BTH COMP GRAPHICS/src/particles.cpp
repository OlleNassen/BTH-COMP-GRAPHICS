#include "particles.hpp"

namespace scene
{
namespace particles
{

emitter::emitter()
{
    constexpr float quad_a[] =
	{
		// positions
		-0.5f, -0.5f,
		-0.5f,  0.5f,
		 0.5f, -0.5f,

		 0.5f, -0.5f,
		-0.5f,  0.5f,
		 0.5f,  0.5f
	};

    vertex_buffer.data(sizeof(quad_a), quad_a, GL_STATIC_DRAW);
    v_array.attribute_pointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    position_buffer.data(sizeof(glm::vec3) * particles.size(),
        nullptr, GL_STREAM_DRAW);
    v_array.attribute_pointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    color_buffer.data(sizeof(glm::vec3) * particles.size(),
        nullptr, GL_STREAM_DRAW);
    v_array.attribute_pointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
}

particle& emitter::find_unused_particle()
{
    for(unsigned i = LastUsedParticle; i < particles.size(); ++i)
    {
        if (particles[i].life < 0)
        {
            LastUsedParticle = i;
            return particles[i];
        }
    }

    for(unsigned i = 0; i<particles.size(); i++)
    {
        if (particles[i].life < 0)
        {
            LastUsedParticle = i;
            return particles[i];
        }
    }

    return particles.front();
}






void emitter::update_current(milliseconds delta_time,
    const glm::mat4& world_transform, glm::mat4& transform)
{
    using namespace std::chrono;
	using float_seconds = duration<float>;
	float delta = duration_cast<float_seconds>(delta_time).count();


    int new_particles = delta_time.count();
    for (int i = 0; i < new_particles; ++i)
    {
        auto& p = find_unused_particle();
        p.life = 5.0f; // This particle will live 5 seconds.
        p.position = glm::vec3(0.0f,100.0f,0.0f);

        float spread = 1.5f;
        glm::vec3 maindir = glm::vec3(0.0f, 10.0f, 0.0f);

        glm::vec3 randomdir
        {
            (rand()%2000 - 1000.0f)/1000.0f,
            (rand()%2000 - 1000.0f)/1000.0f,
            (rand()%2000 - 1000.0f)/1000.0f
        };

        p.speed = maindir + randomdir * spread;

        p.color = {
            (rand() % 100)/100.0f,
            (rand() % 100)/100.0f,
            (rand() % 100)/100.0f};
        p.size = (rand()%1000)/2000.0f + 0.1f;
    }





    int ParticlesCount = 0;
    for (auto& p : particles)
    {
        p.life -= delta;
        if(p.is_alive())
        {
            p.speed += glm::vec3{0.0f,-9.81f, 0.0f} * delta * 0.5f;
            p.position += p.speed * delta;

            positions[ParticlesCount] = p.position;
            colors[ParticlesCount] = p.color;
        }

        ++ParticlesCount;
    }

    position_buffer.data(sizeof(glm::vec3) * particles.size(),
        nullptr, GL_STREAM_DRAW);
    position_buffer.sub_data(0, sizeof(glm::vec3) * positions.size(),
        positions.data());

    color_buffer.data(sizeof(glm::vec3) * particles.size(),
        nullptr, GL_STREAM_DRAW);
    color_buffer.sub_data(0, sizeof(glm::vec3) * colors.size(),
        colors.data());
}

void emitter::render_current(const shader& shader,
    const glm::mat4& world_transform) const
{
	shader.uniform("model", world_transform);
	vertex_buffer.bind();
    v_array.attribute_pointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    position_buffer.bind();
    v_array.attribute_pointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    color_buffer.bind();
    v_array.attribute_pointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glVertexAttribDivisor(0, 0);
    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);

	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, particles.size());

	glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

	glBindVertexArray(0);
}

}
}
