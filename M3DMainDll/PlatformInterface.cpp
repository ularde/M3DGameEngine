#include "pch.h"
#include "PlatformInterface.h"
#include "Platforms.h"

//MainInterface has only one instance
M3DMainInterface* mainInterfaceObject = NULL;
//The instance of offline game inside the engine
MBasicPlatform* instance = NULL;
//But there may be many instances of SubInterface
M3DSubInterface* subInterfaceObjects[1024] = { NULL };
//Index of current SubInterface being operated
unsigned int currentSubInterfaceIndex = 0;

M3DMainInterface* PICreateMainInterface(unsigned int gPlatform) {
    mainInterfaceObject = (M3DMainInterface*)malloc(sizeof(M3DMainInterface));
    mainInterfaceObject->currentStatue = M3D_Interface_Operating;
    switch (gPlatform) {
    case M3D_WinClient:
        instance = new MOfflineGame(M3D_Windows);
        break;
    case M3D_WinServer:
        //
        break;
    case M3D_WinEditor:
        instance = new MEditor(M3D_Windows);
        break;
    case M3D_LinuxClient:
        instance = new MOfflineGame(M3D_Linux);
        break;
    case M3D_LinuxEditor:
        instance = new MEditor(M3D_Linux);
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
        return 0;
    }
}

void PIDestroyMainInterface() {
    exit(0);
}

void PIRunMainInterface() {
    if (instance != NULL) {
        while (!instance->GetWhetherShouldExit()) {
            instance->Update();
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

void PIEditor_SubmitKeyboardInput(int key, int mode) {
    if (instance != NULL) {
        reinterpret_cast<MEditor*>(instance)->SubmitKeyboardInput(key, mode);
    }
}

void PIEditor_SubmitCursorPosInput(double x, double y) {
    if (instance != NULL) {
        reinterpret_cast<MEditor*>(instance)->SubmitCursorPosInput(x, y);
    }
}

void PIEditor_SubmitFramebufferSizeInput(int w, int  h) {
    if (instance != NULL) {
        reinterpret_cast<MEditor*>(instance)->FramebufferSizeCallback(w, h);
    }
}

void PIEditor_SetHWND(HWND hwnd) {
    gHWND = hwnd;
}

void PIEditor_Tick(HDC dc) {
    if (instance != NULL) {
        reinterpret_cast<MEditor*>(instance)->Update();
        SwapBuffers(dc);
    }
}

void PIEditor_SetDefaultFBO(unsigned int ID) {
    if (instance != NULL) {
        reinterpret_cast<MEditor*>(instance)->SetDefaultFBO(ID);
    }
}

void PIEditor_SetGetProcAddress(void* ptr) {
    edGetProcAddress = ptr;
}

void PIEditor_LoadScene(const char* path) {
    if (instance != NULL) {
        reinterpret_cast<MEditor*>(instance)->LoadSceneWithFullPath(path);
    }
}

void PIEditor_ExitGame() {
    if (instance != NULL) {
        reinterpret_cast<MEditor*>(instance)->ExitGame();
    }
}

void PIEditor_StartGame() {
    if (instance != NULL) {
        reinterpret_cast<MEditor*>(instance)->StartGame();
    }
}

void PIEditor_SubmitMouseButtonInput(int button, int flag) {
    if (instance != NULL) {
        reinterpret_cast<MEditor*>(instance)->SubmitMouseButtonInput(button, flag);
    }
}

void PIEditor_SubmitMouseButtonFlags(unsigned short flags) {
    if (instance != NULL) {
        reinterpret_cast<MEditor*>(instance)->SubmitMouseButtonFlags(flags);
    }
}


int PIEditor_GetCursorDisabled() {
    if (instance != NULL) {
        if (reinterpret_cast<MEditor*>(instance)->mCursorDisabled) {
            return 1;
        }
        else {
            return 0;
        }
    }
    else {
        return 0;
    }
}

void PIEditor_SetEditMode(unsigned int ID) {
    if (instance != NULL) {
        MEditor* editor = reinterpret_cast<MEditor*>(instance);
        editor->SetEditMode((MEditMode)ID);
    }
}

void PIEditor_SubmitScrollOffset(double x, double y) {
    if (instance != NULL) {
        MEditor* editor = reinterpret_cast<MEditor*>(instance);
        editor->SubmitScrollOffset(x, y);
    }
}

void PIEditor_CreateEntity(const char* def) {
    if (instance != NULL) {
        MEditor* editor = reinterpret_cast<MEditor*>(instance);
        editor->CreateEntity(def);
    }
}

void PIEditor_CreateRigidStatic(const char* model) {
    if (instance != NULL) {
        MEditor* editor = reinterpret_cast<MEditor*>(instance);
        editor->CreateRigidStatic(model);
    }
}

void PIEditor_SaveScene(const char* path) {
    if (instance != NULL) {
        reinterpret_cast<MEditor*>(instance)->SaveSceneWithFullPath(path);
    }
}
