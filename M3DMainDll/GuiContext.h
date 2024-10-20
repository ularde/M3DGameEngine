#pragma once
#include <string>
#include <map>
#include <ft2build.h>
#include <glm/glm.hpp>
#include FT_FREETYPE_H  

class MBasicPlatform;
class MRenderGuiPipeline;

enum class MButtonStyle {
	NORMAL,
	BORDERLESS
};

enum class MInputBoxStyle {
	NORMAL,
	BORDERLESS
};

class MGuiContext;

struct MCharacter {
	GLuint mTextureID = 0;
	glm::ivec2 mSize;
	glm::ivec2 mBearing;
	unsigned int mAdvance = 0;
};

struct MFace {
	MGuiContext* mGui = 0;
	FT_Face mFTFace = 0;
	std::string mName;
	unsigned int mHeight = 0;
	MCharacter mCharacters[128];
};

struct MButtonEvent {
	bool click = false;
	bool dbclick = false;
	bool mouseOn = false;
};

class MGuiContext {
public:
	MGuiContext(MBasicPlatform* platform, FT_Library ftl, MRenderGuiPipeline* pipeline);
	~MGuiContext();
	void AddFace(const std::string& name, const std::string& path, unsigned int height);
	//to do: as lua does not naturally support unicode string, i have to store wstrings in localization files.
	void RenderText(const std::string& face, const std::string& text,
		float x, float y,
		const glm::vec3& color, float scale);
	MButtonEvent RenderButton(const std::string& face, const std::string& text,
		float x, float y,
		const glm::vec3& color1, const glm::vec3& color2,
		float scale, float border, MButtonStyle style);
	//void DrawTextW(const std::wstring& face, const std::wstring& text, const glm::vec2& pos, const glm::vec3& color);
	void RenderLine();
	void RenderTransparentRectangle(const glm::vec4& color,const glm::vec2& c_pos);
	void RenderRectangle();
	MBasicPlatform* gPlatform = NULL;
private:
	void RenderButtonText(const std::string& face, const std::string& text,
		float x, float y,
		const glm::vec3& color, float scale);
	glm::vec2 GetButtonTextX(const std::string& face, const std::string& text,
		float x, float y, float scale);
	glm::vec2 GetButtonTextY(const std::string& face, const std::string& text,
		float x, float y, float scale);
	MRenderGuiPipeline* mGuiPipeline = NULL;
	FT_Library mFreetypeLib;
	std::map<std::string, MFace> mMFaceMap;
	std::map<std::string, FT_Face> mFaceMap;
};