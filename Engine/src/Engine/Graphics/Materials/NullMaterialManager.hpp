#pragma once

#include "pch.h"
#include "iMaterialManager.h"

namespace MyEngine
{
	class NullMaterialManager : public iMaterialManager
	{
	public:
		NullMaterialManager() {};
		virtual ~NullMaterialManager() {};

		virtual void BindMaterial(std::string materialName)
		{
			LOG_ERROR("MaterialManager not set!");
		}

		virtual void UnbindMaterial()
		{
			LOG_ERROR("MaterialManager not set!");
		}

		virtual void LoadMaterial(std::string materialName, const sMaterialInfo& material)
		{
			LOG_ERROR("MaterialManager not set!");
		}

		virtual sMaterialInfo& GetMaterialByName(std::string materialName)
		{
			LOG_ERROR("MaterialManager not set!");
			return emptyMat;
		}

		virtual std::map<std::string, sMaterialInfo>& GetMaterials()
		{
			LOG_ERROR("MaterialManager not set!");
			return emptyMap;
		}

		virtual void DeleteMaterial(std::string materialName)
		{
			LOG_ERROR("MaterialManager not set!");
			return;
		}

		std::map<std::string, sMaterialInfo> emptyMap = {};
		sMaterialInfo emptyMat = sMaterialInfo();
	};
}