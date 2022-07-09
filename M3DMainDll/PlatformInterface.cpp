#include "pch.h"
#include "PlatformInterface.h"
#include "Platforms.h"

//MainInterface ֻ��һ��ʵ��
M3DMainInterface* mainInterfaceObject = NULL;
//�������ڲ�������Ϸ��ʵ��
MBasicPlatform* instance = NULL;
//�� SubInterface ���������1024��ʵ��
M3DSubInterface* subInterfaceObjects[1024] = { NULL };
//��ǰ SubInterface ����
unsigned int currentSubInterfaceIndex = 0;

M3DMainInterface* PICreateMainInterface(unsigned int platform) {
    mainInterfaceObject = (M3DMainInterface*)malloc(sizeof(M3DMainInterface));
    mainInterfaceObject->currentStatue = M3D_Interface_Operating;
    switch (platform) {
    case M3D_WinClient:
        instance = new MOfflineGame(M3D_Windows);
        break;
    case M3D_WinServer:
        //
        break;
    case M3D_WinEditor:
        //instance = new MEditor(M3D_Windows);
        break;
    case M3D_LinuxClient:
        instance = new MOfflineGame(M3D_Linux);
        break;
    case M3D_LinuxEditor:
        //instance = new MEditor(M3D_Linux);
        break;
    case M3D_LinuxServer:
        //
        break;
    default:
        break;
    }
    if (instance != NULL) {
        return mainInterfaceObject;
    }
    else {
        MessageBeep(MB_ICONERROR);
        MessageBox(NULL, L"Couldn't create game instance.", L"M3D Game Engine", MB_ICONERROR);
        return nullptr;
    }
}

void PIDestroyMainInterface() {
    free(mainInterfaceObject);
    delete[]instance;
    mainInterfaceObject = NULL;
}

void PIRunMainInterface() {
    if (instance != NULL) {
        while (!instance->GetWhetherShouldExit()) {
            instance->Tick();
        }
    }
}

M3DSubInterface* PICreateSubInterface(unsigned int type) {
    subInterfaceObjects[currentSubInterfaceIndex] = (M3DSubInterface*)malloc(sizeof(M3DSubInterface));
    subInterfaceObjects[currentSubInterfaceIndex]->interfaceType = type;
    subInterfaceObjects[currentSubInterfaceIndex]->currentStatue = M3D_Interface_Operating;
    subInterfaceObjects[currentSubInterfaceIndex]->ID = currentSubInterfaceIndex;
    currentSubInterfaceIndex += 1;
    return subInterfaceObjects[currentSubInterfaceIndex - 1];
}

void PIDestroySubInterface(unsigned int ID) {
    free(subInterfaceObjects[ID]);
    subInterfaceObjects[ID] = NULL;
}
