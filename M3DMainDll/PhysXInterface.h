#pragma once
#include <stdio.h>
#include <jemalloc/jemalloc.h>
#include <PxPhysicsAPI.h>

static FILE* physxLog = NULL;

inline void MPhysX_WriteToLog(const char* error, const char* message, const char* file, int line) {
    if (physxLog) {
        fprintf(physxLog, "[PhysX] Error : %s Message : %s File : %s Line : %d\n", error, message, file, line);
    }
}

class MPxAllocatorCallback : public physx::PxAllocatorCallback {
public:
    virtual ~MPxAllocatorCallback() {
        printf("PhysX Interface : Uninitializing PhysX memory allocator...\n");
    }
    virtual void* allocate(size_t size, const char* typeName, const char* filename, int line) {
        return je_aligned_alloc(16, size);
    }
    virtual void deallocate(void* ptr) {
        return je_free(ptr);
    }
};



class MUserErrorCallback : public physx::PxErrorCallback {
public:
    virtual void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line) {
        switch (code)
        {
        case physx::PxErrorCode::eNO_ERROR:
            MPhysX_WriteToLog("eNO_ERROR", message, file, line);
            break;
        case physx::PxErrorCode::eDEBUG_INFO:
            MPhysX_WriteToLog("eDEBUG_INFO", message, file, line);
            break;
        case physx::PxErrorCode::eDEBUG_WARNING:
            MPhysX_WriteToLog("eDEBUG_WARNING", message, file, line);
            break;
        case physx::PxErrorCode::eINVALID_PARAMETER:
            MPhysX_WriteToLog("eINVALID_PARAMETER", message, file, line);
            break;
        case physx::PxErrorCode::eINVALID_OPERATION:
            MPhysX_WriteToLog("eINVALID_OPERATION", message, file, line);
            break;
        case physx::PxErrorCode::eOUT_OF_MEMORY:
            MPhysX_WriteToLog("eOUT_OF_MEMORY", message, file, line);
            break;
        case physx::PxErrorCode::eINTERNAL_ERROR:
            MPhysX_WriteToLog("eINTERNAL_ERROR", message, file, line);
            break;
        case physx::PxErrorCode::eABORT:
            MPhysX_WriteToLog("eABORT", message, file, line);
            break;
        case physx::PxErrorCode::ePERF_WARNING:
            MPhysX_WriteToLog("ePERF_WARNING", message, file, line);
            break;
        case physx::PxErrorCode::eMASK_ALL:
            MPhysX_WriteToLog("eMASK_ALL", message, file, line);
            break;
        default:
            break;
        }
    }
};