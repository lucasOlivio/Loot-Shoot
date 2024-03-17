#include "pch.h"

#include "GraphicsUtils.h"

#include "Engine/Utils/Math.h"

namespace MyEngine
{
	void GraphicsUtils::DrawModel(const sRenderModelInfo& renderInfo, std::shared_ptr<ShaderManager> pShader)
	{
		pShader->SetUniformFloat("bUseColorTexture", renderInfo.useColorTexture);

		// Debug variables
		pShader->SetUniformFloat("bUseDefaultColor", renderInfo.useDefaultColor);
		pShader->SetUniformVec4("defaultColor", renderInfo.defaultColor);

		// Translates the matrix for every tile
		glm::mat4 matModel = renderInfo.matModel;
		for (int x = 0; x < renderInfo.tileAxis[0]; x++)
		{
			for (int y = 0; y < renderInfo.tileAxis[1]; y++)
			{
				for (int z = 0; z < renderInfo.tileAxis[2]; z++)
				{
					// Calculate the translation for the current tile
					glm::vec3 translation = glm::vec3(
						x * renderInfo.tileOffset.x,
						y * renderInfo.tileOffset.y,
						z * renderInfo.tileOffset.z
					);

					glm::mat4 tileModelMatrix = glm::translate(matModel, translation);

					pShader->SetUniformMatrix4f("matModel", tileModelMatrix);

					// Also calculate and pass the "inverse transpose" for the model matrix
					glm::mat4 matModelIT = glm::inverse(glm::transpose(tileModelMatrix));
					pShader->SetUniformMatrix4f("matModel_IT", matModelIT);

					glBindVertexArray(renderInfo.VAO_ID); //  enable VAO (and everything else)
					glDrawElements(GL_TRIANGLES,
									renderInfo.numberOfIndices,
									GL_UNSIGNED_INT,
									0);
					glBindVertexArray(0); 			  // disable VAO (and everything else)
				}
			}
		}
	}

	void GraphicsUtils::DrawParticle(const sRenderParticleInfo& renderInfo, std::shared_ptr<ShaderManager> pShader)
	{
		pShader->SetUniformInt("isParticle", true);
		pShader->SetUniformVec4("defaultColor", renderInfo.color);
		pShader->SetUniformFloat("particleAlpha", renderInfo.alpha);

		pShader->SetUniformMatrix4f("matModel", renderInfo.matModel);

		// Also calculate and pass the "inverse transpose" for the model matrix
		glm::mat4 matModelIT = glm::inverse(glm::transpose(renderInfo.matModel));
		pShader->SetUniformMatrix4f("matModel_IT", matModelIT);

		glBindVertexArray(IMPOSTOR_VAO_ID); //  enable VAO (and everything else)
		glDrawElements(GL_TRIANGLES,
			IMPOSTOR_NUM_INDICES,
			GL_UNSIGNED_INT,
			0);
		glBindVertexArray(0); 			  // disable VAO (and everything else)
	}
}
