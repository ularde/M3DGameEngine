#ifndef _DEBUG
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
#endif // DEBUG


#include <iostream>
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

typedef M3DMainInterface* (CALLBACK* _PICREATEMAININTERFACE)(unsigned int);
_PICREATEMAININTERFACE PICreateMainInterface;
typedef void (CALLBACK* _PIDESTROYMAININSTANCE)(void);
_PIDESTROYMAININSTANCE PIDestroyMainInterface;
typedef void (CALLBACK* _PIRUNMAININSTANCE)(void);
_PIRUNMAININSTANCE PIRunMainInstance;

int main() {
	HMODULE hMainDllModule = LoadLibrary(L".\\M3DMainDll.dll");
	if (!hMainDllModule) {
		MessageBeep(MB_ICONERROR);
		MessageBox(NULL, L"Failed to load DLL file: M3DMainDll.dll", L"Game Launcher", MB_ICONERROR);
		return -1;
	}
	else {
		PICreateMainInterface = (_PICREATEMAININTERFACE)GetProcAddress(hMainDllModule, "PICreateMainInterface");
		PIDestroyMainInterface = (_PIDESTROYMAININSTANCE)GetProcAddress(hMainDllModule, "PIDestroyMainInterface");
		PIRunMainInstance = (_PIRUNMAININSTANCE)GetProcAddress(hMainDllModule, "PIRunMainInterface");
		if (!PICreateMainInterface || !PIDestroyMainInterface || !PIRunMainInstance) {
			MessageBeep(MB_ICONERROR);
			MessageBox(NULL, L"Failed to load functions from DLL file M3DMainDll.dll. Perhaps the file has been broken. Please replace it with another copy.", L"Game Launcher", MB_ICONERROR);
		}
		else {
			if (!PICreateMainInterface(M3D_WinClient)) {
				MessageBeep(MB_ICONERROR);
				MessageBox(NULL, L"Failed to create interface provided by Engine.", L"Game Launcher", MB_ICONERROR);
			}
			else {
				PIRunMainInstance();
			}
			PIDestroyMainInterface();
		}
		return 0;
	}
}