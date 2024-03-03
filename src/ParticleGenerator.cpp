#include "ParticleGenerator.h"
int lastUsedParticle = 0;
auto Toyengine::ParticleGenerator::firstUnusedParticle() -> int
{
    // first search from last used particle, this will usually return almost instantly
    for (unsigned int i = lastUsedParticle; i < this->m_Amount; ++i){
        if (this->m_Particles[i].getLife() <= 0.0f){
            lastUsedParticle = i;
            return i;
        }
    }
    // otherwise, do a linear search
    for (unsigned int i = 0; i < lastUsedParticle; ++i){
        if (this->m_Particles[i].getLife() <= 0.0f){
            lastUsedParticle = i;
            return i;
        }
    }
    // all particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
    lastUsedParticle = 0;
    return 0;
}

auto Toyengine::ParticleGenerator::respawnParticle(Particle& particle, GameEntity* entity, glm::vec2 offset) -> void
{
    const float random = (rand() % 100 - 50) / 10.0f;
    const float rColor = 0.5f + rand() % 100 / 100.0f;
    particle.setPosition(entity->getPosition() + random + offset);
    particle.setColor(glm::vec4(rColor, rColor, rColor, 1.0f));
    particle.setLife(1);
    particle.setVelocity(entity->getVelocity() * 0.1f);
}

Toyengine::ParticleGenerator::ParticleGenerator(const Shader& shader, const Texture2D& texture, unsigned int amount) :
    m_Amount(amount),
    m_Shader(shader),
    m_Texture(texture)
{
    // set up mesh and attribute properties
    unsigned int vertexBuffer;
    constexpr float particles[] = {
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };
    glGenVertexArrays(1, &this->m_VertexArray);
    glGenBuffers(1, &vertexBuffer);
    glBindVertexArray(this->m_VertexArray);
    // fill mesh buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particles), particles, GL_STATIC_DRAW);
    // set mesh attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), static_cast<void*>(nullptr));
    glBindVertexArray(0);

    // create this->amount default particle instances
    for (unsigned int i = 0; i < this->m_Amount; ++i)
    {
        auto particle = Particle();
        this->m_Particles.emplace_back(particle);
    }

}

auto Toyengine::ParticleGenerator::update(float deltaTime, GameEntity* entity, unsigned int newParticles,
    glm::vec2 offset) -> void
{
    // add new particles
    for (unsigned int i = 0; i < newParticles; ++i)
    {
        int unusedParticle = this->firstUnusedParticle();
        this->respawnParticle(this->m_Particles[unusedParticle], entity, offset);
    }
    // update all particles
    for (unsigned int i = 0; i < this->m_Amount; ++i)
    {
        Particle &p = this->m_Particles[i];
        p.setLife(p.getLife() - deltaTime); // reduce life
        if (p.getLife() > 0.0f)
        {	// particle is alive, thus update
            p.setPosition(p.getPosition() - p.getVelocity() * deltaTime);
            p.setColor(glm::vec4(p.getColor().r, p.getColor().g, p.getColor().b, p.getColor().a - deltaTime * 2.5f));
        }
    }
}

auto Toyengine::ParticleGenerator::draw() -> void
{
    // use additive blending to give it a 'glow' effect
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    this->m_Shader.activate();
    for (Particle particle : this->m_Particles)
    {
        if (particle.getLife() > 0.0f)
        {
            this->m_Shader.setVector2f("offset", particle.getPosition());
            this->m_Shader.setVector4f("color", particle.getColor());
            this->m_Texture.bind();
            glBindVertexArray(this->m_VertexArray);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }
    }
    // don't forget to reset to default blending mode
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
