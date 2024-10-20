#pragma once
#include <unordered_map>
#include <string>

class MGameContext;

extern MGameContext* gGameContext;

class MBasicPlatform;
class MEntity;
class MScene;
class MActor;
struct MButtonEvent;

extern void Game_SetCursorInvisible();
extern void Game_SetCursorVisible();
extern void Game_SetLocalCamera();
extern void Game_BindThisEntity();
extern void Game_BindEntity(const std::string& v);
extern void Game_ConsoleWriteLine(const std::string& v);
extern void Game_LoadScene(const std::string& v);
extern void Game_SpawnEntity(const std::string& v1, const std::string& v2);
extern void Game_SpawnEntityAutoName(const std::string& v1);
extern void Game_SpawnEntityEx(const std::string& v1, const std::string& v2, double x, double y, double z);
extern std::string Game_SpawnEntityAutoNameEx(const std::string& v2, double x, double y, double z);
extern int Game_GetKey(int n);
extern int Game_GetMouseLButtonDown();
extern int Game_GetMouseLButtonUp();
extern int Game_GetMouseRButtonDown();
extern int Game_GetMouseRButtonUp();
extern glm::ivec2 Game_GetWindowSize();
extern glm::vec3 Game_CastRayByCursorPosition(double x, double y);
extern glm::vec3 Game_GetRaycastingBlockPosition(const glm::vec3& o, const glm::vec3& r, double d);
extern double Game_GetCursorPosX();
extern double Game_GetCursorPosY();
extern double Game_GetScrollOffsetX();
extern double Game_GetScrollOffsetY();
extern void Entity_BindThisEntityComponent();
extern void Entity_BindEntityComponent(const std::string& name);
extern glm::vec3 RigidDynamic_GetPosition();
extern void RigidDynamic_Invalidate();
extern void RigidDynamic_Validate();
extern void RigidDynamic_SetPosition(double x, double y, double z);
extern void RigidDynamic_AddForce(double x, double y, double z, int mode);
extern void RigidDynamic_AddTorque(double x, double y, double z, int mode);
extern glm::vec3 CinematicCamera_GetPosition();
extern glm::vec3 CinematicCamera_GetFront();
extern glm::vec3 CinematicCamera_GetUp();
extern double CinematicCamera_GetFovy();
extern void CinematicCamera_SetPosition(double x, double y, double z);
extern void CinematicCamera_SetFront(double x, double y, double z);
extern void CinematicCamera_SetUp(double x, double y, double z);
extern void CinematicCamera_SetFovy(double fovy);
extern double Game_GetTime();
extern double Game_GetWorldTime();
extern void Game_SetWorldTime(double t);
extern void Game_SetDynamicTimeOfDay(int flag);
extern glm::vec2 Game_CreateVec2(float x, float y);
extern glm::vec3 Game_CreateVec3(float x, float y, float z);
extern glm::vec3 Game_CreateVec4(float x, float y, float z, float w);
extern void Game_SetGlobalVariableInt(const std::string& name, int v);
extern void Game_SetGlobalVariableDouble(const std::string& name, double v);
extern void Game_SetGlobalVariableFloat(const std::string& name, float v);
extern void Game_SetGlobalVariableString(const std::string& name, const std::string& v);
extern int Game_GetGlobalVariableInt(const std::string& name);
extern double Game_GetGlobalVariableDouble(const std::string& name);
extern float Game_GetGlobalVariableFloat(const std::string& name);
extern std::string Game_GetGlobalVariableString(const std::string& name);
extern void Gui_AddFace(const std::string& name, const std::string& path, unsigned int height);
extern MButtonEvent Gui_RenderButton(const std::string& face, const std::string& text,
	float x, float y,
	float c1r, float c1g, float c1b,
	float c2r, float c2g, float c2b,
	float scale, float border, int style);
extern void Gui_RenderText(const std::string& face, const std::string& text, float posx, float posy, float cr, float cg, float cb, float scale);

class MGameContext {
public:
	MGameContext(MBasicPlatform* platform);
	~MGameContext();
	void UpdateEntityTable();
	glm::vec3 CastRayByCursorPosition(double x, double y);
	glm::vec3 GetRaycastingBlockPosition(const glm::vec3& o, const glm::vec3& r, double d);
	void SetLocalCamera();
	void BindThisEntity();
	void BindThisEntityComponent();
	void BindEntity(const std::string& name);
	void BindEntityComponent(const std::string& name);
	glm::vec3 RigidDynamic_GetPosition();
	void RigidDynamic_Invalidate();
	void RigidDynamic_Validate();
	void RigidDynamic_SetPosition(double x, double y, double z);
	void RigidDynamic_AddForce(double x, double y, double z, int mode);
	void RigidDynamic_AddTorque(double x, double y, double z, int mode);
	void CinematicCamera_SetPosition(double x, double y, double z);
	void CinematicCamera_SetFront(double x, double y, double z);
	void CinematicCamera_SetUp(double x, double y, double z);
	void CinematicCamera_SetFovy(double fovy);
	glm::vec3 CinematicCamera_GetUp();
	glm::vec3 CinematicCamera_GetPosition();
	glm::vec3 CinematicCamera_GetFront();
	double CinematicCamera_GetFovy();
	void ConsoleWriteLine(const std::string& text);
	void LoadScene(const std::string& name);
	void SpawnEntity(const std::string& name, const std::string& def);
	void SpawnEntityAutoName(const std::string& def);
	void SpawnEntityEx(const std::string& name, const std::string& def, double xpos, double ypos, double zpos);
	void SetFullscreen();
	void SetCursorInvisible();
	void SetCursorVisible();
	int GetKey(int key);
	int GetMouseLButtonDown();
	int GetMouseLButtonUp();
	int GetMouseRButtonDown();
	int GetMouseRButtonUp();
	glm::vec2 GetCursorPos();
	glm::vec2 GetScrollOffset();
	double GetWorldTime();
	void SetWorldTime(double t);
	void SetDynamicTimeOfDay(int flag);
	MBasicPlatform* gPlatform = NULL;
	MScene* gCurrentScene = NULL;
	MEntity* gCurrentEntity = NULL;
	MEntity* gThisEntity = NULL;
	std::unordered_map<std::string, MEntity*>gEntityTable;
};