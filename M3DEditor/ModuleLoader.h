#pragma once
#include <Windows.h>

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

typedef M3DMainInterface* (_stdcall* _PICREATEMAININTERFACE)(unsigned int);
inline _PICREATEMAININTERFACE PICreateMainInterface;
typedef void(_stdcall* _PIDESTROYMAININSTANCE)(void);
inline _PIDESTROYMAININSTANCE PIDestroyMainInterface;
typedef void(_stdcall* _PIRUNMAININSTANCE)(void);
inline _PIRUNMAININSTANCE PIRunMainInstance;
typedef M3DSubInterface* (_stdcall* _PICREATESUBINTERFACE)(unsigned int);
inline _PICREATESUBINTERFACE PICreateSubInterface;
typedef void(_stdcall* _PIDestroySubInterface)(unsigned int);
inline _PIDestroySubInterface PIDestroySubInterface;
typedef void(_stdcall* _PIEditor_SubmitKeyboardInput)(int, int);
inline _PIEditor_SubmitKeyboardInput PIEditor_SubmitKeyboardInput;
typedef void(_stdcall* _PIEditor_SubmitCursorPosInput)(double, double);
inline _PIEditor_SubmitCursorPosInput PIEditor_SubmitCursorPosInput;
typedef void(_stdcall* _PIEditor_SubmitMouseButtonFlags)(unsigned short);
inline _PIEditor_SubmitMouseButtonFlags PIEditor_SubmitMouseButtonFlags;
typedef void(_stdcall* _PIEditor_SubmitFramebufferSizeInput)(int, int);
inline _PIEditor_SubmitFramebufferSizeInput PIEditor_SubmitFramebufferSizeInput;
typedef void(_stdcall* _PIEditor_Tick)(HDC);
inline _PIEditor_Tick PIEditor_Tick;
typedef void(_stdcall* _PIEditor_SetGetProcAddress)(void*);
inline _PIEditor_SetGetProcAddress PIEditor_SetGetProcAddress;
typedef void(_stdcall* _PIEditor_SetDefaultFBO)(unsigned int);
inline _PIEditor_SetDefaultFBO PIEditor_SetDefaultFBO;
typedef void(_stdcall* _PIEditor_LoadScene)(const char*);
inline _PIEditor_LoadScene PIEditor_LoadScene;
typedef void(_stdcall* _PIEditor_ExitGame)();
inline _PIEditor_ExitGame PIEditor_ExitGame;
typedef void(_stdcall* _PIEditor_StartGame)();
inline _PIEditor_StartGame PIEditor_StartGame;
typedef void(_stdcall* _PIEditor_SetHWND)(HWND);
inline _PIEditor_SetHWND PIEditor_SetHWND;
typedef void(_stdcall* _PIEditor_SubmitMouseButtonInput)(int, int);
inline _PIEditor_SubmitMouseButtonInput PIEditor_SubmitMouseButtonInput;
typedef int(_stdcall* _PIEditor_GetCursorDisabled)();
inline _PIEditor_GetCursorDisabled PIEditor_GetCursorDisabled;
typedef void(_stdcall* _PIEditor_SetEditMode)(unsigned int);
inline _PIEditor_SetEditMode PIEditor_SetEditMode;
typedef void(_stdcall* _PIEditor_SubmitScrollOffset)(double, double);
inline _PIEditor_SubmitScrollOffset PIEditor_SubmitScrollOffset;
typedef void(_stdcall* _PIEditor_CreateRigidStatic)(const char*);
inline _PIEditor_CreateRigidStatic PIEditor_CreateRigidStatic;
typedef void(_stdcall* _PIEditor_CreateEntity)(const char*);
inline _PIEditor_CreateEntity PIEditor_CreateEntity;
typedef void(_stdcall* _PIEditor_SaveScene)(const char*);
inline _PIEditor_SaveScene PIEditor_SaveScene;