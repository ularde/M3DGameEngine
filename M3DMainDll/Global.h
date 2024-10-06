#pragma once
#define M3D_CFG_BADATTRIBUTION 0x00
#define M3D_M3DS_BADMAINMODULE 0x01

#define M3D_MAX_SHADER_INFOLOG_LENGTH 4096
#define M3D_MAX_IFSTREAM_ERROR_LENGTH 1024
#define M3D_MAX_GAMECONSOLE_LUACMD_LENGTH 4096
#define M3D_MAX_GAMECONSOLE_VALUE_LENGTH 4096
#define M3D_MAX_COMMON_STRING_LENGTH 1024

#define M3D_MAX_BONE_COUNT 4

#define M3D_ASSET_TEXTURE 0
#define M3D_ASSET_MODEL 1
#define M3D_ASSET_SCENE 2
#define M3D_ASSET_UIFORM 3
#define M3D_ASSET_MATERIAL 4

#define M3D_SURFACE_TYPE_METAL 0
#define M3D_SURFACE_TYPE_STONE 1
#define M3D_SURFACE_TYPE_WOOD 2
#define M3D_SURFACE_TYPE_PLASTIC 3
#define M3D_SURFACE_TYPE_MUD 4

#define M3D_T_SPEED 100.0F

extern GLuint publicID;

enum class MPlatformMode {
	SCENE,
	FORM
};

extern struct MVec3 {
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
};

extern struct MVec4 {
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 0.0f;
};

extern enum class MAxis {
	X_AXIS,
	Y_AXIS,
	Z_AXIS
};

extern enum class MEditMode {
	NONE,
	TRANSLATE,
	ROTATE,
	SCALE,
	TRANSLATING,
	ROTATING,
	SCALING,
	CREATING,
};