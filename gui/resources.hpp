// resources.hpp - Base classes for resource management of GUI

#ifndef _RESOURCE_HEADER
#define _RESOURCE_HEADER

#include <string>
#include <vector>
#include <map>

struct ZipArchive;

extern "C" {
#include "../minuitwrp/minui.h"
}

// Base Objects
class Resource
{
public:
	Resource(xml_node<>* node, ZipArchive* pZip);
	virtual ~Resource() {}

public:
	std::string GetName() { return mName; }

private:
	std::string mName;

protected:
	static int ExtractResource(ZipArchive* pZip, std::string folderName, std::string fileName, std::string fileExtn, std::string destFile);
	static void LoadImage(ZipArchive* pZip, std::string file, gr_surface* source);
	static void CheckAndScaleImage(gr_surface source, gr_surface* destination, int retain_aspect);
};

class FontResource : public Resource
{
public:
	enum Type
	{
		TYPE_TWRP,
#ifndef TW_DISABLE_TTF
		TYPE_TTF,
#endif
	};

	FontResource(xml_node<>* node, ZipArchive* pZip);
	virtual ~FontResource();

public:
	void* GetResource() { return this ? mFont : NULL; }
	int GetHeight() { return gr_getMaxFontHeight(this ? mFont : NULL); }

protected:
	void* mFont;
	Type m_type;
};

class ImageResource : public Resource
{
public:
	ImageResource(xml_node<>* node, ZipArchive* pZip, int retain_aspect);
	virtual ~ImageResource();

public:
	gr_surface GetResource() { return this ? mSurface : NULL; }
	int GetWidth() { return gr_get_width(this ? mSurface : NULL); }
	int GetHeight() { return gr_get_height(this ? mSurface : NULL); }

protected:
	gr_surface mSurface;
};

class AnimationResource : public Resource
{
public:
	AnimationResource(xml_node<>* node, ZipArchive* pZip, int retain_aspect);
	virtual ~AnimationResource();

public:
	gr_surface GetResource() { return (!this || mSurfaces.empty()) ? NULL : mSurfaces.at(0); }
	gr_surface GetResource(int entry) { return (!this || mSurfaces.empty()) ? NULL : mSurfaces.at(entry); }
	int GetWidth() { return gr_get_width(this ? GetResource() : NULL); }
	int GetHeight() { return gr_get_height(this ? GetResource() : NULL); }
	int GetResourceCount() { return mSurfaces.size(); }

protected:
	std::vector<gr_surface> mSurfaces;
};

class ResourceManager
{
public:
	ResourceManager();
	virtual ~ResourceManager();
	void LoadResources(xml_node<>* resList, ZipArchive* pZip);

public:
	FontResource* FindFont(const std::string& name) const;
	ImageResource* FindImage(const std::string& name) const;
	AnimationResource* FindAnimation(const std::string& name) const;
	std::string FindString(const std::string& name) const;

private:
	std::vector<FontResource*> mFonts;
	std::vector<ImageResource*> mImages;
	std::vector<AnimationResource*> mAnimations;
	std::map<std::string, std::string> mStrings;
};

#endif  // _RESOURCE_HEADER
