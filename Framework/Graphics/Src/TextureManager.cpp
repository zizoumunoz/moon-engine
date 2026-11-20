#include "Precompiled.h"
#include "TextureManager.h"

using namespace MoonEngine;
using namespace MoonEngine::Graphics;

namespace
{
	std::unique_ptr<TextureManager> s_textureManager;
}

void TextureManager::StaticInitialize(const std::filesystem::path& root)
{
	ASSERT(s_textureManager == nullptr, "TextureManager: Is already initialized");
	s_textureManager = std::make_unique<TextureManager>();
	s_textureManager->SetRootDirectory(root);
}

void TextureManager::StaticTerminate()
{
	s_textureManager.reset();
}

TextureManager* TextureManager::Get()
{
	ASSERT(s_textureManager != nullptr, "TextureManager: is not initialized");
	return s_textureManager.get();
}

TextureManager::~TextureManager()
{
	ASSERT(m_inventory.empty(), "TextureManager: is not terminated");
}

void TextureManager::SetRootDirectory(const std::filesystem::path& root)
{
	m_rootDirectory = root;
}

TextureId TextureManager::LoadTexture(const std::filesystem::path& fileName, bool useRootDir)
{
	const std::size_t textureId = std::filesystem::hash_value(fileName);
	auto [iter, success] = m_inventory.insert({ textureId, Entry() });
	if (success)
	{
		iter->second.texture = std::make_unique<Texture>();
		iter->second.texture->Initialize((useRootDir) ? m_rootDirectory / fileName : fileName);
		iter->second.refCount = 1;
	}
	else
	{
		++iter->second.refCount;
	}
	return textureId;
}

const Texture* TextureManager::GetTexture(TextureId id)
{
	auto iter = m_inventory.find(id);
	if (iter != m_inventory.end())
	{
		return iter->second.texture.get();
	}
	return nullptr;
}

void TextureManager::ReleaseTexture(TextureId id)
{
	auto iter = m_inventory.find(id);
	if (iter != m_inventory.end())
	{
		--iter->second.refCount;
		if (iter->second.refCount ==0)
		{
			iter->second.texture->Terminate();
			iter->second.texture.reset();
			m_inventory.erase(iter);
		}
	}
}

void TextureManager::BindVS(TextureId id, uint32_t slot) const
{
	auto iter = m_inventory.find(id);
	if (iter != m_inventory.end())
	{
		iter->second.texture->BindVS(slot);
	}

}

void TextureManager::BindPS(TextureId id, uint32_t slot) const
{
	auto iter = m_inventory.find(id);
	if (iter!= m_inventory.end())
	{
		iter->second.texture->BindPS(slot);
	}
}