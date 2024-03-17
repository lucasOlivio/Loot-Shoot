#include "pch.h"

#include "ParticleRenderSystem.h"

#include "Engine/Graphics/Renderer/RendererManagerLocator.h"
#include "Engine/Graphics/Particles/ParticleManagerLocator.h"

#include "Engine/Utils/TransformUtils.h"
#include "Engine/Utils/GraphicsUtils.h"

namespace MyEngine
{
    void ParticleRenderSystem::Init()
    {
    }

    void ParticleRenderSystem::Start(std::shared_ptr<Scene> pScene)
    {
    }

    void ParticleRenderSystem::Update(std::shared_ptr<Scene> pScene, float deltaTime)
    {
    }

    void ParticleRenderSystem::Render(std::shared_ptr<Scene> pScene)
    {
        std::shared_ptr<iParticleManager> pParticleManager = ParticleManagerLocator::Get();
        std::shared_ptr<iRendererManager> pRendererManager = RendererManagerLocator::Get();

        const std::vector<ParticleProps>& particles = pParticleManager->GetParticles();

        for (int i = 0; i < particles.size(); i++)
        {
            const ParticleProps& particle = particles[i];

            if (particle.lifetime <= 0.0f)
            {
                continue;
            }

            glm::mat4 matTransform = glm::mat4(1.0);

            TransformUtils::GetTransform(particle.position,
                                         particle.orientation,
                                         particle.scale,
                                         matTransform);

            sRenderParticleInfo renderInfo = sRenderParticleInfo();
            renderInfo.alpha = particle.alpha;
            renderInfo.color = particle.color;
            renderInfo.matModel = matTransform;

            pRendererManager->AddToRender(renderInfo);
        }
    }

    void ParticleRenderSystem::End(std::shared_ptr<Scene> pScene)
    {
    }

    void ParticleRenderSystem::Shutdown()
    {
    }
}
