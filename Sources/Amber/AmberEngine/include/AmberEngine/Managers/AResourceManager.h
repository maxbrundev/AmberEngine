#pragma once

namespace AmberEngine::ResourceManagement
{
	template<typename T>
	class AResourceManager
	{
	public:
		AResourceManager() = default;
		virtual ~AResourceManager() = default;

		T* LoadResource(const std::string& p_path, bool isRealPathRequired = true);
		
		void UnloadResource(const std::string& p_path);
		bool MoveResource(const std::string& p_previousPath, const std::string& p_newPath);
		void ReloadResource(const std::string& p_path);
		bool IsResourceRegistered(const std::string& p_path);
		void UnloadResources();
		T* RegisterResource(const std::string& p_path, T* p_instance);
		void UnregisterResource(const std::string& p_path);
		T* GetResource(const std::string& p_path, bool p_tryToLoadIfNotFound = true);
		T* operator[](const std::string& p_path);
		static void ProvideAssetPaths(const std::string& p_projectAssetsPath, const std::string& p_engineAssetsPath);
		std::unordered_map<std::string, T*>& GetResources();

	protected:
		virtual T* CreateResource(const std::string& p_path, bool isRealPathRequired = true)                  = 0;
		virtual void DestroyResource(T* p_resource)                           = 0;
		virtual void ReloadResource(T* p_resource, const std::string& p_path, bool isRealPathRequired = true) = 0;

		std::string GetRealPath(const std::string& p_path) const;

	private:
		inline static std::string PROJECT_ASSETS_PATH = "";
		inline static std::string ENGINE_ASSETS_PATH  = "";

		std::unordered_map<std::string, T*> m_resources;
	};
}

#include "AmberEngine/Managers/AResourceManager.inl"