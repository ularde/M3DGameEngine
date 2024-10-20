#include "pch.h"
#include "GuiContext.h"
#include "SharedFunc.h"
#include "Platforms.h"
#include "RenderGuiPipeline.h"

struct MCharcater;
struct MFace;

MGuiContext::MGuiContext(MBasicPlatform* platform, FT_Library ftl, MRenderGuiPipeline* pipeline) {
	gPlatform = platform;
	mGuiPipeline = pipeline;
	mFreetypeLib = ftl;
}

MGuiContext::~MGuiContext()
{
}

void MGuiContext::AddFace(const std::string& name, const std::string& path, unsigned int height) {
	FT_Face face = 0;
	if (FT_New_Face(mFreetypeLib, GetFullAssetPathA(path), 0, &face)) {
		gPlatform->ConsoleWriteLine(1.0f, 1.0f, 1.0f, "Failed to load font " + path);
	}
	else {
		MFace mface{ this,face,name,height };
		FT_Set_Pixel_Sizes(face, 0, height);
		glPixelStoref(GL_UNPACK_ALIGNMENT, 1);
		for (int c = 0; c < 128; c++) {
			if (FT_Load_Char(face, (FT_ULong)c, FT_LOAD_RENDER)) {
				gPlatform->ConsoleWriteLine(1.0f, 1.0f, 1.0f, "Failed to load Glyph from" + path);
			}
			GLuint textureID = 0;
			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			MCharacter character{ textureID,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				static_cast<unsigned int>(face->glyph->advance.x) };
			mface.mCharacters[c] = character;
		}
		glBindTexture(GL_TEXTURE_2D, 0);
		mMFaceMap[name] = mface;
	}
	FT_Done_Face(face);
}

void MGuiContext::RenderButtonText(const std::string& face, const std::string& text, float x, float y, const glm::vec3& color, float scale) {
	float nx = x;
	float out = x;
	static bool initFlag = false;
	static GLuint tVAO, tVBO;
	if (!initFlag) {
		glGenVertexArrays(1, &tVAO);
		glGenBuffers(1, &tVBO);
		glBindVertexArray(tVAO);
		glBindBuffer(GL_ARRAY_BUFFER, tVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, 0, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		initFlag = true;
	}
	//draw text
	mGuiPipeline->BeginRendering();
	mGuiPipeline->SetColor(color);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(tVAO);
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++) {
		MCharacter ch = mMFaceMap[face].mCharacters[(const int)*c];
		float xpos = nx + ch.mBearing.x * scale;
		float ypos = y - (ch.mSize.y - ch.mBearing.y) * scale;
		float w = ch.mSize.x * scale;
		float h = ch.mSize.y * scale;
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};
		glBindTexture(GL_TEXTURE_2D, ch.mTextureID);
		glBindBuffer(GL_ARRAY_BUFFER, tVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		nx += (ch.mAdvance >> 6) * scale;
		out = nx + w;
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	mGuiPipeline->EndRendering();
}

glm::vec2 MGuiContext::GetButtonTextX(const std::string& face, const std::string& text, float x, float y, float scale) {
	float xo = x;
	float xn = x;
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++) {
		MCharacter ch = mMFaceMap[face].mCharacters[(const int)*c];
		float xpos = xn + ch.mBearing.x * scale;
		float ypos = y - (ch.mSize.y - ch.mBearing.y) * scale;
		float w = ch.mSize.x * scale;
		float h = ch.mSize.y * scale;
		xn += (ch.mAdvance >> 6) * scale;
		xo = xn + w;
	}
	return glm::vec2(x, xo);
}

glm::vec2 MGuiContext::GetButtonTextY(const std::string& face, const std::string& text, float x, float y, float scale) {
	MCharacter ch = mMFaceMap[face].mCharacters[(int)'A'];
	float nyl = y - (ch.mSize.y - ch.mBearing.y) * scale;
	float nyh = y - (ch.mSize.y - ch.mBearing.y) * scale + ch.mSize.y * scale;
	return glm::vec2(nyl, nyh);
}

void MGuiContext::RenderText(const std::string& face, const std::string& text, float x, float y, const glm::vec3& color, float scale) {
	static bool initFlag = false;
	static GLuint tVAO, tVBO;
	if (!initFlag) {
		glGenVertexArrays(1, &tVAO);
		glGenBuffers(1, &tVBO);
		glBindVertexArray(tVAO);
		glBindBuffer(GL_ARRAY_BUFFER, tVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, 0, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		initFlag = true;
	}
	//draw text
	mGuiPipeline->BeginRendering();
	mGuiPipeline->SetColor(color);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(tVAO);
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++) {
		MCharacter ch = mMFaceMap[face].mCharacters[(const int)*c];
		float xpos = x + ch.mBearing.x * scale;
		float ypos = y - (ch.mSize.y - ch.mBearing.y) * scale;
		float w = ch.mSize.x * scale;
		float h = ch.mSize.y * scale;
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};
		glBindTexture(GL_TEXTURE_2D, ch.mTextureID);
		glBindBuffer(GL_ARRAY_BUFFER, tVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		x += (ch.mAdvance >> 6) * scale; 
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	mGuiPipeline->EndRendering();
}

static int bn = 0;

MButtonEvent MGuiContext::RenderButton(const std::string& face, const std::string& text,
	float x, float y,
	const glm::vec3& color1, const glm::vec3& color2,
	float scale, float border, MButtonStyle style) {
	MButtonEvent event{ false,false,false };
	static bool mouseOnFlag = false;
	MCharacter ch = mMFaceMap[face].mCharacters[(int)'A'];
	float nx = GetButtonTextX(face, text, x, y, scale).y;
	float nyl = GetButtonTextY(face, text, x, y, scale).x;
	float nyh = GetButtonTextY(face, text, x, y, scale).y;
	//process event
	float cx = gPlatform->GetCursorPosX();
	float cy = gPlatform->gFramebufferHeight - gPlatform->GetCursorPosY();
	if (cx >= x - border && cx <= nx + border && cy >= abs(nyl - border) && cy <= nyh + border) {
		mouseOnFlag = true;
		event.mouseOn = true;
		RenderButtonText(face, text, x, y, color2, scale);
		if (gPlatform->GetMouseLeftButtonDown() && bn == 0) {
			bn++;
			event.click = true;
		}
		if (gPlatform->GetMouseLeftButtonUp() && bn >= 1) {
			bn = 0;
		}
	}
	else {
		mouseOnFlag = false;
		event.mouseOn = false;
		RenderButtonText(face, text, x, y, color1, scale);
	}
	return event;
}
