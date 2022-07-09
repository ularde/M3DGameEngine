#include "pch.h"
#include "VirtualMachine.h"

inline void Op_NOP(M3DS_ModuleVM* instance) {
	instance->IP++;
}

inline void Op_RET(M3DS_ModuleVM* instance) {
	instance->IP++;
	if (instance->IPJumped) {
		instance->IP = instance->lastIP;
		instance->IPJumped = false;
	}
}

inline void Op_SAVE(M3DS_ModuleVM* instance) {
	instance->IP++;
	instance->lastIP = instance->IP;
}

inline void Op_JMP(M3DS_ModuleVM* instance, int type1, int var1) {
	instance->IP += 3;
	instance->lastIP = instance->IP;
	if (type1 == M3DS_TYPE_INT_IMM) {
		instance->IP = var1;
	}
	else if (type1 == M3DS_TYPE_INT_REG) {
		instance->IP = instance->integerRegs[var1];
	}
	else if (type1 == M3DS_TYPE_SPEC_REG) {
		instance->IP = instance->specialRegs[var1];
	}
	instance->IPJumped = true;
}

inline void Op_LOOP(M3DS_ModuleVM* instance,int type1, int var1, int type2, int var2) {
	instance->IP += 5;
	instance->lastIP = instance->IP - 5;
	if (type1 == M3DS_TYPE_SPEC_REG && type2 == M3DS_TYPE_INT_REG) {
		if (instance->specialRegs[var1] != 0) {
			instance->specialRegs[var1] -= 1;
			instance->IP = instance->integerRegs[var2];
		}
		else {
			instance->lastIP += 5;
		}
	}
	else if (type1 == M3DS_TYPE_SPEC_REG && type2 == M3DS_TYPE_SPEC_REG) {
		if (instance->specialRegs[var1] != 0) {
			instance->specialRegs[var1] -= 1;
			instance->IP = instance->specialRegs[var2];
		}
		else {
			instance->lastIP += 5;
		}
	}
	else if (type1 == M3DS_TYPE_SPEC_REG && type2 == M3DS_TYPE_INT_IMM) {
		if (instance->specialRegs[var1] != 0) {
			instance->specialRegs[var1] -= 1;
			instance->IP = var2;
		}
		else {
			instance->lastIP += 5;
		}
	}
	instance->IPJumped = true;
}

inline void Op_JLE(M3DS_ModuleVM* instance,int type1, int var1, int type2, int var2, int type3, int var3) {
	instance->IP += 7;
	if (type1 == M3DS_TYPE_INT_IMM) {

	}
	else if (type1 == M3DS_TYPE_INT_REG) {

	}
	else if (type1 == M3DS_TYPE_SPEC_REG) {

	}
	instance->IPJumped = true;
}

inline void Op_JLT(M3DS_ModuleVM* instance,int type1, int var1, int type2, int var2, int type3, int var3) {

}

inline void Op_JGE(M3DS_ModuleVM* instance,int type1, int var1, int type2, int var2, int type3, int var3) {

}

inline void Op_JGT(M3DS_ModuleVM* instance, int type1, int var1, int type2, int var2, int type3, int var3) {

}

inline void Op_JEQ(M3DS_ModuleVM* instance, int type1, int var1, int type2, int var2, int type3, int var3) {

}

inline void Op_MOV(M3DS_ModuleVM* instance, int type1, int var1, int type2, int var2) {

}

inline void Op_ISET(M3DS_ModuleVM* instance, int type1, int var1, int tye2, int var2) {

}

inline void Op_CSET(M3DS_ModuleVM* instance, int type1, int var1, int tye2, int var2) {

}

inline void Op_LSET(M3DS_ModuleVM* instance, int type1, int var1, int tye2, int var2) {

}

inline void Op_LLSET(M3DS_ModuleVM* instance, int type1, int var1, int tye2, int var2, int type3, int var3) {

}

inline void Op_FSET(M3DS_ModuleVM* instance, int type1, int var1, int tye2, int var2) {

}

inline void Op_DSET(M3DS_ModuleVM* instance, int type1, int var1, int tye2, int var2) {

}

void M3DS_ExecuteModuleInstructions(std::vector<int> instructions) {
	M3DS_ModuleVM* vmInstance = (M3DS_ModuleVM*)malloc(sizeof(M3DS_ModuleVM));
	unsigned int iCount = instructions.size();
	while (vmInstance->IP < iCount) {
		switch (instructions[vmInstance->IP]) {
		case M3DS_OP_NOP:
			Op_NOP(vmInstance);
			break;
		case M3DS_OP_RET:
			Op_RET(vmInstance);
			break;
		default:
			break;
		}
	free(vmInstance);
}