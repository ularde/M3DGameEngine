#pragma once

extern enum M3D_OS_ID {
	M3D_Windows,
	M3D_Linux
};

extern enum M3DPlatform {
	M3D_WinEditor,
	M3D_WinClient,
	M3D_WinServer,
	M3D_LinuxEditor,
	M3D_LinuxClient,
	M3D_LinuxServer
};

extern enum M3DSubInterfaceType {
	M3D_MaterialEditor,
	M3D_ParticleEditor,
	M3D_ActorEditor_Main,
	M3D_ActorEditor_Player,
	M3D_AnimationEditor_Main,
	M3D_AnimationEditor_Player,
	M3D_UIEditor_Main,
	M3D_UIEditor_Emulator
};

extern enum M3DInterfaceStatue {
	M3D_Interface_Operating,
	M3D_Interface_Interrupted,
	M3D_Interface_Stopped
};

extern struct M3DMainInterface {
	unsigned int currentStatue;
};

extern struct M3DSubInterface {
	unsigned int ID;
	unsigned int currentStatue;
	unsigned int interfaceType;
};


extern "C" __declspec(dllexport) M3DMainInterface * PICreateMainInterface(unsigned int gPlatform);
extern "C" __declspec(dllexport) void PIDestroyMainInterface();
extern "C" __declspec(dllexport) void PIRunMainInterface();
extern "C" __declspec(dllexport) M3DSubInterface * PICreateSubInterface(unsigned int type);
extern "C" __declspec(dllexport) void PIDestroySubInterface(unsigned int ID);
extern "C" __declspec(dllexport) void PIEditor_SubmitKeyboardInput(int key, int mode);
extern "C" __declspec(dllexport) void PIEditor_SubmitCursorPosInput(double x, double y);
extern "C" __declspec(dllexport) void PIEditor_SubmitFramebufferSizeInput(int w, int h);
extern "C" __declspec(dllexport) void PIEditor_SetHWND(HWND hwmd);
extern "C" __declspec(dllexport) void PIEditor_Tick(HDC DC);
extern "C" __declspec(dllexport) void PIEditor_SetDefaultFBO(unsigned int ID);
extern "C" __declspec(dllexport) void PIEditor_SetGetProcAddress(void* ptr);
extern "C" __declspec(dllexport) void PIEditor_LoadScene(const char* path);
extern "C" __declspec(dllexport) void PIEditor_ExitGame();
extern "C" __declspec(dllexport) void PIEditor_StartGame();
extern "C" __declspec(dllexport) void PIEditor_SubmitMouseButtonInput(int button, int flag);
extern "C" __declspec(dllexport) void PIEditor_SubmitMouseButtonFlags(unsigned short flags);
extern "C" __declspec(dllexport) int PIEditor_GetCursorDisabled();
extern "C" __declspec(dllexport) void PIEditor_SetEditMode(unsigned int ID);
extern "C" __declspec(dllexport) void PIEditor_SubmitScrollOffset(double x, double y);
extern "C" __declspec(dllexport) void PIEditor_CreateEntity(const char* def);
extern "C" __declspec(dllexport) void PIEditor_CreateRigidStatic(const char* model);
extern "C" __declspec(dllexport) void PIEditor_SaveScene(const char* path);