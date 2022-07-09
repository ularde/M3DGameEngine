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

extern "C" __declspec(dllexport) M3DMainInterface * PICreateMainInterface(unsigned int platform);
extern "C" __declspec(dllexport) void PIDestroyMainInterface();
extern "C" __declspec(dllexport) void PIRunMainInterface();
extern "C" __declspec(dllexport) M3DSubInterface * PICreateSubInterface(unsigned int type);
extern "C" __declspec(dllexport) void PIDestroySubInterface(unsigned int ID);