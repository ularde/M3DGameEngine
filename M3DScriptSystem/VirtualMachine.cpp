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
		if (type2 == M3DS_TYPE_INT_IMM) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (type2 <= type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (type2 <= instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (type2 <= instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (type2 <= instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (type2 <= instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (type2 <= instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (type2 <= instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (type2 <= instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (type2 <= instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
		else if (type2 == M3DS_TYPE_SPEC_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->specialRegs[type2] <= type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->specialRegs[type2] <= instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->specialRegs[type2] <= instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->specialRegs[type2] <= instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->specialRegs[type2] <= instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->specialRegs[type2] <= instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->specialRegs[type2] <= instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->specialRegs[type2] <= instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->specialRegs[type2] <= instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
		else if (type2 == M3DS_TYPE_CHAR_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->charRegs[type2] <= type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->charRegs[type2] <= instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->charRegs[type2] <= instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->charRegs[type2] <= instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->charRegs[type2] <= instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->charRegs[type2] <= instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->charRegs[type2] <= instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->charRegs[type2] <= instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->charRegs[type2] <= instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
		else if (type2 == M3DS_TYPE_INT_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->integerRegs[type2] <= type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->integerRegs[type2] <= instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->integerRegs[type2] <= instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->integerRegs[type2] <= instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->integerRegs[type2] <= instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->integerRegs[type2] <= instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->integerRegs[type2] <= instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->integerRegs[type2] <= instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->integerRegs[type2] <= instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
		else if (type2 == M3DS_TYPE_LONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->longRegs[type2] <= type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->longRegs[type2] <= instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->longRegs[type2] <= instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->longRegs[type2] <= instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->longRegs[type2] <= instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->longRegs[type2] <= instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->longRegs[type2] <= instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->longRegs[type2] <= instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->longRegs[type2] <= instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
		else if (type2 == M3DS_TYPE_LONGLONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->longlongRegs[type2] <= type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->longlongRegs[type2] <= instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->longlongRegs[type2] <= instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->longlongRegs[type2] <= instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->longlongRegs[type2] <= instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->longlongRegs[type2] <= instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->longlongRegs[type2] <= instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->longlongRegs[type2] <= instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->longlongRegs[type2] <= instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
		else if (type2 == M3DS_TYPE_TEMPLONGLONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->templonglongRegs[type2] <= type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->templonglongRegs[type2] <= instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->templonglongRegs[type2] <= instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->templonglongRegs[type2] <= instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->templonglongRegs[type2] <= instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->templonglongRegs[type2] <= instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->templonglongRegs[type2] <= instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->templonglongRegs[type2] <= instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->templonglongRegs[type2] <= instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
		else if (type2 == M3DS_TYPE_FLOAT_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->floatRegs[type2] <= type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->floatRegs[type2] <= instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->floatRegs[type2] <= instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->floatRegs[type2] <= instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->floatRegs[type2] <= instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->floatRegs[type2] <= instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->floatRegs[type2] <= instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->floatRegs[type2] <= instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->floatRegs[type2] <= instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
		else if (type2 == M3DS_TYPE_DOUBLE_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->doubleRegs[type2] <= type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->doubleRegs[type2] <= instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->doubleRegs[type2] <= instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->doubleRegs[type2] <= instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->doubleRegs[type2] <= instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->doubleRegs[type2] <= instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->doubleRegs[type2] <= instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->doubleRegs[type2] <= instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->doubleRegs[type2] <= instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
	}
	else if (type1 == M3DS_TYPE_INT_REG) {
		if (type2 == M3DS_TYPE_INT_IMM) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (type2 <= type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (type2 <= instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (type2 <= instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (type2 <= instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (type2 <= instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (type2 <= instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (type2 <= instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (type2 <= instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (type2 <= instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_SPEC_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->specialRegs[type2] <= type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->specialRegs[type2] <= instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->specialRegs[type2] <= instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->specialRegs[type2] <= instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->specialRegs[type2] <= instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->specialRegs[type2] <= instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->specialRegs[type2] <= instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->specialRegs[type2] <= instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->specialRegs[type2] <= instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_CHAR_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->charRegs[type2] <= type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->charRegs[type2] <= instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->charRegs[type2] <= instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->charRegs[type2] <= instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->charRegs[type2] <= instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->charRegs[type2] <= instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->charRegs[type2] <= instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->charRegs[type2] <= instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->charRegs[type2] <= instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_INT_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->integerRegs[type2] <= type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->integerRegs[type2] <= instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->integerRegs[type2] <= instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->integerRegs[type2] <= instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->integerRegs[type2] <= instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->integerRegs[type2] <= instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->integerRegs[type2] <= instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->integerRegs[type2] <= instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->integerRegs[type2] <= instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_LONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->longRegs[type2] <= type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->longRegs[type2] <= instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->longRegs[type2] <= instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->longRegs[type2] <= instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->longRegs[type2] <= instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->longRegs[type2] <= instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->longRegs[type2] <= instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->longRegs[type2] <= instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->longRegs[type2] <= instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_LONGLONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->longlongRegs[type2] <= type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->longlongRegs[type2] <= instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->longlongRegs[type2] <= instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->longlongRegs[type2] <= instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->longlongRegs[type2] <= instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->longlongRegs[type2] <= instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->longlongRegs[type2] <= instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->longlongRegs[type2] <= instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->longlongRegs[type2] <= instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_TEMPLONGLONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->templonglongRegs[type2] <= type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->templonglongRegs[type2] <= instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->templonglongRegs[type2] <= instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->templonglongRegs[type2] <= instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->templonglongRegs[type2] <= instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->templonglongRegs[type2] <= instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->templonglongRegs[type2] <= instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->templonglongRegs[type2] <= instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->templonglongRegs[type2] <= instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_FLOAT_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->floatRegs[type2] <= type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->floatRegs[type2] <= instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->floatRegs[type2] <= instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->floatRegs[type2] <= instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->floatRegs[type2] <= instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->floatRegs[type2] <= instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->floatRegs[type2] <= instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->floatRegs[type2] <= instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->floatRegs[type2] <= instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_DOUBLE_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->doubleRegs[type2] <= type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->doubleRegs[type2] <= instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->doubleRegs[type2] <= instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->doubleRegs[type2] <= instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->doubleRegs[type2] <= instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->doubleRegs[type2] <= instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->doubleRegs[type2] <= instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->doubleRegs[type2] <= instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->doubleRegs[type2] <= instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
	}
	else if (type1 == M3DS_TYPE_SPEC_REG) {
		if (type2 == M3DS_TYPE_INT_IMM) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (type2 <= type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (type2 <= instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (type2 <= instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (type2 <= instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (type2 <= instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (type2 <= instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (type2 <= instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (type2 <= instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (type2 <= instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_SPEC_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->specialRegs[type2] <= type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->specialRegs[type2] <= instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->specialRegs[type2] <= instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->specialRegs[type2] <= instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->specialRegs[type2] <= instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->specialRegs[type2] <= instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->specialRegs[type2] <= instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->specialRegs[type2] <= instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->specialRegs[type2] <= instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_CHAR_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->charRegs[type2] <= type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->charRegs[type2] <= instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->charRegs[type2] <= instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->charRegs[type2] <= instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->charRegs[type2] <= instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->charRegs[type2] <= instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->charRegs[type2] <= instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->charRegs[type2] <= instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->charRegs[type2] <= instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_INT_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->integerRegs[type2] <= type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->integerRegs[type2] <= instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->integerRegs[type2] <= instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->integerRegs[type2] <= instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->integerRegs[type2] <= instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->integerRegs[type2] <= instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->integerRegs[type2] <= instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->integerRegs[type2] <= instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->integerRegs[type2] <= instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_LONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->longRegs[type2] <= type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->longRegs[type2] <= instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->longRegs[type2] <= instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->longRegs[type2] <= instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->longRegs[type2] <= instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->longRegs[type2] <= instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->longRegs[type2] <= instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->longRegs[type2] <= instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->longRegs[type2] <= instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_LONGLONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->longlongRegs[type2] <= type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->longlongRegs[type2] <= instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->longlongRegs[type2] <= instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->longlongRegs[type2] <= instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->longlongRegs[type2] <= instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->longlongRegs[type2] <= instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->longlongRegs[type2] <= instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->longlongRegs[type2] <= instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->longlongRegs[type2] <= instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_TEMPLONGLONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->templonglongRegs[type2] <= type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->templonglongRegs[type2] <= instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->templonglongRegs[type2] <= instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->templonglongRegs[type2] <= instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->templonglongRegs[type2] <= instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->templonglongRegs[type2] <= instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->templonglongRegs[type2] <= instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->templonglongRegs[type2] <= instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->templonglongRegs[type2] <= instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_FLOAT_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->floatRegs[type2] <= type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->floatRegs[type2] <= instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->floatRegs[type2] <= instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->floatRegs[type2] <= instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->floatRegs[type2] <= instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->floatRegs[type2] <= instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->floatRegs[type2] <= instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->floatRegs[type2] <= instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->floatRegs[type2] <= instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_DOUBLE_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->doubleRegs[type2] <= type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->doubleRegs[type2] <= instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->doubleRegs[type2] <= instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->doubleRegs[type2] <= instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->doubleRegs[type2] <= instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->doubleRegs[type2] <= instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->doubleRegs[type2] <= instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->doubleRegs[type2] <= instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->doubleRegs[type2] <= instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
	}
	instance->IPJumped = true;
}

inline void Op_JLT(M3DS_ModuleVM* instance,int type1, int var1, int type2, int var2, int type3, int var3) {
	instance->IP += 7;
	if (type1 == M3DS_TYPE_INT_IMM) {
		if (type2 == M3DS_TYPE_INT_IMM) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (type2 < type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (type2 < instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (type2 < instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (type2 < instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (type2 < instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (type2 < instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (type2 < instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (type2 < instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (type2 < instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
		else if (type2 == M3DS_TYPE_SPEC_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->specialRegs[type2] < type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->specialRegs[type2] < instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->specialRegs[type2] < instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->specialRegs[type2] < instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->specialRegs[type2] < instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->specialRegs[type2] < instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->specialRegs[type2] < instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->specialRegs[type2] < instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->specialRegs[type2] < instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
		else if (type2 == M3DS_TYPE_CHAR_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->charRegs[type2] < type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->charRegs[type2] < instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->charRegs[type2] < instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->charRegs[type2] < instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->charRegs[type2] < instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->charRegs[type2] < instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->charRegs[type2] < instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->charRegs[type2] < instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->charRegs[type2] < instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
		else if (type2 == M3DS_TYPE_INT_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->integerRegs[type2] < type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->integerRegs[type2] < instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->integerRegs[type2] < instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->integerRegs[type2] < instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->integerRegs[type2] < instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->integerRegs[type2] < instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->integerRegs[type2] < instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->integerRegs[type2] < instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->integerRegs[type2] < instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
		else if (type2 == M3DS_TYPE_LONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->longRegs[type2] < type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->longRegs[type2] < instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->longRegs[type2] < instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->longRegs[type2] < instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->longRegs[type2] < instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->longRegs[type2] < instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->longRegs[type2] < instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->longRegs[type2] < instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->longRegs[type2] < instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
		else if (type2 == M3DS_TYPE_LONGLONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->longlongRegs[type2] < type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->longlongRegs[type2] < instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->longlongRegs[type2] < instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->longlongRegs[type2] < instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->longlongRegs[type2] < instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->longlongRegs[type2] < instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->longlongRegs[type2] < instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->longlongRegs[type2] < instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->longlongRegs[type2] < instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
		else if (type2 == M3DS_TYPE_TEMPLONGLONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->templonglongRegs[type2] < type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->templonglongRegs[type2] < instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->templonglongRegs[type2] < instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->templonglongRegs[type2] < instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->templonglongRegs[type2] < instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->templonglongRegs[type2] < instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->templonglongRegs[type2] < instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->templonglongRegs[type2] < instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->templonglongRegs[type2] < instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
		else if (type2 == M3DS_TYPE_FLOAT_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->floatRegs[type2] < type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->floatRegs[type2] < instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->floatRegs[type2] < instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->floatRegs[type2] < instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->floatRegs[type2] < instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->floatRegs[type2] < instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->floatRegs[type2] < instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->floatRegs[type2] < instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->floatRegs[type2] < instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
		else if (type2 == M3DS_TYPE_DOUBLE_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->doubleRegs[type2] < type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->doubleRegs[type2] < instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->doubleRegs[type2] < instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->doubleRegs[type2] < instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->doubleRegs[type2] < instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->doubleRegs[type2] < instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->doubleRegs[type2] < instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->doubleRegs[type2] < instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->doubleRegs[type2] < instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
	}
	else if (type1 == M3DS_TYPE_INT_REG) {
		if (type2 == M3DS_TYPE_INT_IMM) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (type2 < type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (type2 < instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (type2 < instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (type2 < instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (type2 < instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (type2 < instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (type2 < instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (type2 < instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (type2 < instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_SPEC_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->specialRegs[type2] < type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->specialRegs[type2] < instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->specialRegs[type2] < instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->specialRegs[type2] < instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->specialRegs[type2] < instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->specialRegs[type2] < instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->specialRegs[type2] < instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->specialRegs[type2] < instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->specialRegs[type2] < instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_CHAR_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->charRegs[type2] < type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->charRegs[type2] < instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->charRegs[type2] < instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->charRegs[type2] < instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->charRegs[type2] < instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->charRegs[type2] < instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->charRegs[type2] < instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->charRegs[type2] < instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->charRegs[type2] < instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_INT_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->integerRegs[type2] < type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->integerRegs[type2] < instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->integerRegs[type2] < instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->integerRegs[type2] < instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->integerRegs[type2] < instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->integerRegs[type2] < instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->integerRegs[type2] < instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->integerRegs[type2] < instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->integerRegs[type2] < instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_LONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->longRegs[type2] < type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->longRegs[type2] < instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->longRegs[type2] < instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->longRegs[type2] < instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->longRegs[type2] < instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->longRegs[type2] < instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->longRegs[type2] < instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->longRegs[type2] < instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->longRegs[type2] < instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_LONGLONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->longlongRegs[type2] < type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->longlongRegs[type2] < instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->longlongRegs[type2] < instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->longlongRegs[type2] < instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->longlongRegs[type2] < instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->longlongRegs[type2] < instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->longlongRegs[type2] < instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->longlongRegs[type2] < instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->longlongRegs[type2] < instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_TEMPLONGLONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->templonglongRegs[type2] < type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->templonglongRegs[type2] < instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->templonglongRegs[type2] < instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->templonglongRegs[type2] < instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->templonglongRegs[type2] < instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->templonglongRegs[type2] < instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->templonglongRegs[type2] < instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->templonglongRegs[type2] < instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->templonglongRegs[type2] < instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_FLOAT_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->floatRegs[type2] < type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->floatRegs[type2] < instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->floatRegs[type2] < instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->floatRegs[type2] < instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->floatRegs[type2] < instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->floatRegs[type2] < instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->floatRegs[type2] < instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->floatRegs[type2] < instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->floatRegs[type2] < instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_DOUBLE_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->doubleRegs[type2] < type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->doubleRegs[type2] < instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->doubleRegs[type2] < instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->doubleRegs[type2] < instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->doubleRegs[type2] < instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->doubleRegs[type2] < instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->doubleRegs[type2] < instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->doubleRegs[type2] < instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->doubleRegs[type2] < instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
	}
	else if (type1 == M3DS_TYPE_SPEC_REG) {
		if (type2 == M3DS_TYPE_INT_IMM) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (type2 < type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (type2 < instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (type2 < instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (type2 < instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (type2 < instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (type2 < instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (type2 < instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (type2 < instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (type2 < instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_SPEC_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->specialRegs[type2] < type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->specialRegs[type2] < instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->specialRegs[type2] < instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->specialRegs[type2] < instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->specialRegs[type2] < instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->specialRegs[type2] < instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->specialRegs[type2] < instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->specialRegs[type2] < instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->specialRegs[type2] < instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_CHAR_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->charRegs[type2] < type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->charRegs[type2] < instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->charRegs[type2] < instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->charRegs[type2] < instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->charRegs[type2] < instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->charRegs[type2] < instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->charRegs[type2] < instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->charRegs[type2] < instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->charRegs[type2] < instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_INT_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->integerRegs[type2] < type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->integerRegs[type2] < instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->integerRegs[type2] < instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->integerRegs[type2] < instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->integerRegs[type2] < instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->integerRegs[type2] < instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->integerRegs[type2] < instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->integerRegs[type2] < instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->integerRegs[type2] < instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_LONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->longRegs[type2] < type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->longRegs[type2] < instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->longRegs[type2] < instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->longRegs[type2] < instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->longRegs[type2] < instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->longRegs[type2] < instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->longRegs[type2] < instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->longRegs[type2] < instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->longRegs[type2] < instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_LONGLONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->longlongRegs[type2] < type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->longlongRegs[type2] < instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->longlongRegs[type2] < instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->longlongRegs[type2] < instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->longlongRegs[type2] < instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->longlongRegs[type2] < instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->longlongRegs[type2] < instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->longlongRegs[type2] < instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->longlongRegs[type2] < instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_TEMPLONGLONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->templonglongRegs[type2] < type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->templonglongRegs[type2] < instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->templonglongRegs[type2] < instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->templonglongRegs[type2] < instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->templonglongRegs[type2] < instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->templonglongRegs[type2] < instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->templonglongRegs[type2] < instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->templonglongRegs[type2] < instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->templonglongRegs[type2] < instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_FLOAT_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->floatRegs[type2] < type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->floatRegs[type2] < instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->floatRegs[type2] < instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->floatRegs[type2] < instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->floatRegs[type2] < instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->floatRegs[type2] < instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->floatRegs[type2] < instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->floatRegs[type2] < instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->floatRegs[type2] < instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_DOUBLE_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->doubleRegs[type2] < type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->doubleRegs[type2] < instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->doubleRegs[type2] < instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->doubleRegs[type2] < instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->doubleRegs[type2] < instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->doubleRegs[type2] < instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->doubleRegs[type2] < instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->doubleRegs[type2] < instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->doubleRegs[type2] < instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
	}
	instance->IPJumped = true;
}

inline void Op_JGE(M3DS_ModuleVM* instance,int type1, int var1, int type2, int var2, int type3, int var3) {
	instance->IP += 7;
	if (type1 == M3DS_TYPE_INT_IMM) {
		if (type2 == M3DS_TYPE_INT_IMM) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (type2 >= type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (type2 >= instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (type2 >= instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (type2 >= instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (type2 >= instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (type2 >= instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (type2 >= instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (type2 >= instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (type2 >= instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
		else if (type2 == M3DS_TYPE_SPEC_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->specialRegs[type2] >= type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->specialRegs[type2] >= instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->specialRegs[type2] >= instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->specialRegs[type2] >= instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->specialRegs[type2] >= instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->specialRegs[type2] >= instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->specialRegs[type2] >= instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->specialRegs[type2] >= instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->specialRegs[type2] >= instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
		else if (type2 == M3DS_TYPE_CHAR_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->charRegs[type2] >= type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->charRegs[type2] >= instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->charRegs[type2] >= instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->charRegs[type2] >= instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->charRegs[type2] >= instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->charRegs[type2] >= instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->charRegs[type2] >= instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->charRegs[type2] >= instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->charRegs[type2] >= instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
		else if (type2 == M3DS_TYPE_INT_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->integerRegs[type2] >= type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->integerRegs[type2] >= instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->integerRegs[type2] >= instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->integerRegs[type2] >= instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->integerRegs[type2] >= instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->integerRegs[type2] >= instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->integerRegs[type2] >= instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->integerRegs[type2] >= instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->integerRegs[type2] >= instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
		else if (type2 == M3DS_TYPE_LONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->longRegs[type2] >= type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->longRegs[type2] >= instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->longRegs[type2] >= instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->longRegs[type2] >= instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->longRegs[type2] >= instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->longRegs[type2] >= instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->longRegs[type2] >= instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->longRegs[type2] >= instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->longRegs[type2] >= instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
		else if (type2 == M3DS_TYPE_LONGLONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->longlongRegs[type2] >= type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->longlongRegs[type2] >= instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->longlongRegs[type2] >= instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->longlongRegs[type2] >= instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->longlongRegs[type2] >= instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->longlongRegs[type2] >= instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->longlongRegs[type2] >= instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->longlongRegs[type2] >= instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->longlongRegs[type2] >= instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
		else if (type2 == M3DS_TYPE_TEMPLONGLONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->templonglongRegs[type2] >= type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->templonglongRegs[type2] >= instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->templonglongRegs[type2] >= instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->templonglongRegs[type2] >= instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->templonglongRegs[type2] >= instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->templonglongRegs[type2] >= instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->templonglongRegs[type2] >= instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->templonglongRegs[type2] >= instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->templonglongRegs[type2] >= instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
		else if (type2 == M3DS_TYPE_FLOAT_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->floatRegs[type2] >= type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->floatRegs[type2] >= instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->floatRegs[type2] >= instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->floatRegs[type2] >= instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->floatRegs[type2] >= instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->floatRegs[type2] >= instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->floatRegs[type2] >= instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->floatRegs[type2] >= instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->floatRegs[type2] >= instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
		else if (type2 == M3DS_TYPE_DOUBLE_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->doubleRegs[type2] >= type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->doubleRegs[type2] >= instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->doubleRegs[type2] >= instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->doubleRegs[type2] >= instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->doubleRegs[type2] >= instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->doubleRegs[type2] >= instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->doubleRegs[type2] >= instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->doubleRegs[type2] >= instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->doubleRegs[type2] >= instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
	}
	else if (type1 == M3DS_TYPE_INT_REG) {
		if (type2 == M3DS_TYPE_INT_IMM) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (type2 >= type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (type2 >= instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (type2 >= instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (type2 >= instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (type2 >= instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (type2 >= instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (type2 >= instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (type2 >= instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (type2 >= instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_SPEC_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->specialRegs[type2] >= type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->specialRegs[type2] >= instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->specialRegs[type2] >= instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->specialRegs[type2] >= instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->specialRegs[type2] >= instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->specialRegs[type2] >= instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->specialRegs[type2] >= instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->specialRegs[type2] >= instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->specialRegs[type2] >= instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_CHAR_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->charRegs[type2] >= type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->charRegs[type2] >= instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->charRegs[type2] >= instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->charRegs[type2] >= instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->charRegs[type2] >= instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->charRegs[type2] >= instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->charRegs[type2] >= instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->charRegs[type2] >= instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->charRegs[type2] >= instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_INT_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->integerRegs[type2] >= type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->integerRegs[type2] >= instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->integerRegs[type2] >= instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->integerRegs[type2] >= instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->integerRegs[type2] >= instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->integerRegs[type2] >= instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->integerRegs[type2] >= instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->integerRegs[type2] >= instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->integerRegs[type2] >= instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_LONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->longRegs[type2] >= type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->longRegs[type2] >= instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->longRegs[type2] >= instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->longRegs[type2] >= instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->longRegs[type2] >= instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->longRegs[type2] >= instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->longRegs[type2] >= instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->longRegs[type2] >= instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->longRegs[type2] >= instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_LONGLONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->longlongRegs[type2] >= type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->longlongRegs[type2] >= instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->longlongRegs[type2] >= instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->longlongRegs[type2] >= instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->longlongRegs[type2] >= instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->longlongRegs[type2] >= instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->longlongRegs[type2] >= instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->longlongRegs[type2] >= instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->longlongRegs[type2] >= instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_TEMPLONGLONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->templonglongRegs[type2] >= type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->templonglongRegs[type2] >= instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->templonglongRegs[type2] >= instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->templonglongRegs[type2] >= instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->templonglongRegs[type2] >= instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->templonglongRegs[type2] >= instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->templonglongRegs[type2] >= instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->templonglongRegs[type2] >= instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->templonglongRegs[type2] >= instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_FLOAT_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->floatRegs[type2] >= type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->floatRegs[type2] >= instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->floatRegs[type2] >= instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->floatRegs[type2] >= instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->floatRegs[type2] >= instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->floatRegs[type2] >= instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->floatRegs[type2] >= instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->floatRegs[type2] >= instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->floatRegs[type2] >= instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_DOUBLE_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->doubleRegs[type2] >= type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->doubleRegs[type2] >= instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->doubleRegs[type2] >= instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->doubleRegs[type2] >= instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->doubleRegs[type2] >= instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->doubleRegs[type2] >= instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->doubleRegs[type2] >= instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->doubleRegs[type2] >= instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->doubleRegs[type2] >= instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
	}
	else if (type1 == M3DS_TYPE_SPEC_REG) {
		if (type2 == M3DS_TYPE_INT_IMM) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (type2 >= type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (type2 >= instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (type2 >= instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (type2 >= instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (type2 >= instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (type2 >= instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (type2 >= instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (type2 >= instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (type2 >= instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_SPEC_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->specialRegs[type2] >= type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->specialRegs[type2] >= instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->specialRegs[type2] >= instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->specialRegs[type2] >= instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->specialRegs[type2] >= instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->specialRegs[type2] >= instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->specialRegs[type2] >= instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->specialRegs[type2] >= instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->specialRegs[type2] >= instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_CHAR_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->charRegs[type2] >= type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->charRegs[type2] >= instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->charRegs[type2] >= instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->charRegs[type2] >= instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->charRegs[type2] >= instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->charRegs[type2] >= instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->charRegs[type2] >= instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->charRegs[type2] >= instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->charRegs[type2] >= instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_INT_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->integerRegs[type2] >= type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->integerRegs[type2] >= instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->integerRegs[type2] >= instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->integerRegs[type2] >= instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->integerRegs[type2] >= instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->integerRegs[type2] >= instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->integerRegs[type2] >= instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->integerRegs[type2] >= instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->integerRegs[type2] >= instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_LONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->longRegs[type2] >= type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->longRegs[type2] >= instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->longRegs[type2] >= instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->longRegs[type2] >= instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->longRegs[type2] >= instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->longRegs[type2] >= instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->longRegs[type2] >= instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->longRegs[type2] >= instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->longRegs[type2] >= instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_LONGLONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->longlongRegs[type2] >= type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->longlongRegs[type2] >= instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->longlongRegs[type2] >= instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->longlongRegs[type2] >= instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->longlongRegs[type2] >= instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->longlongRegs[type2] >= instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->longlongRegs[type2] >= instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->longlongRegs[type2] >= instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->longlongRegs[type2] >= instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_TEMPLONGLONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->templonglongRegs[type2] >= type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->templonglongRegs[type2] >= instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->templonglongRegs[type2] >= instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->templonglongRegs[type2] >= instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->templonglongRegs[type2] >= instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->templonglongRegs[type2] >= instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->templonglongRegs[type2] >= instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->templonglongRegs[type2] >= instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->templonglongRegs[type2] >= instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_FLOAT_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->floatRegs[type2] >= type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->floatRegs[type2] >= instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->floatRegs[type2] >= instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->floatRegs[type2] >= instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->floatRegs[type2] >= instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->floatRegs[type2] >= instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->floatRegs[type2] >= instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->floatRegs[type2] >= instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->floatRegs[type2] >= instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_DOUBLE_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->doubleRegs[type2] >= type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->doubleRegs[type2] >= instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->doubleRegs[type2] >= instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->doubleRegs[type2] >= instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->doubleRegs[type2] >= instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->doubleRegs[type2] >= instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->doubleRegs[type2] >= instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->doubleRegs[type2] >= instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->doubleRegs[type2] >= instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
	}
	instance->IPJumped = true;
}

inline void Op_JGT(M3DS_ModuleVM* instance, int type1, int var1, int type2, int var2, int type3, int var3) {
	instance->IP += 7;
	if (type1 == M3DS_TYPE_INT_IMM) {
		if (type2 == M3DS_TYPE_INT_IMM) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (type2 > type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (type2 > instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (type2 > instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (type2 > instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (type2 > instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (type2 > instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (type2 > instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (type2 > instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (type2 > instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
		else if (type2 == M3DS_TYPE_SPEC_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->specialRegs[type2] > type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->specialRegs[type2] > instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->specialRegs[type2] > instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->specialRegs[type2] > instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->specialRegs[type2] > instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->specialRegs[type2] > instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->specialRegs[type2] > instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->specialRegs[type2] > instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->specialRegs[type2] > instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
		else if (type2 == M3DS_TYPE_CHAR_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->charRegs[type2] > type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->charRegs[type2] > instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->charRegs[type2] > instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->charRegs[type2] > instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->charRegs[type2] > instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->charRegs[type2] > instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->charRegs[type2] > instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->charRegs[type2] > instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->charRegs[type2] > instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
		else if (type2 == M3DS_TYPE_INT_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->integerRegs[type2] > type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->integerRegs[type2] > instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->integerRegs[type2] > instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->integerRegs[type2] > instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->integerRegs[type2] > instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->integerRegs[type2] > instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->integerRegs[type2] > instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->integerRegs[type2] > instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->integerRegs[type2] > instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
		else if (type2 == M3DS_TYPE_LONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->longRegs[type2] > type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->longRegs[type2] > instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->longRegs[type2] > instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->longRegs[type2] > instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->longRegs[type2] > instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->longRegs[type2] > instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->longRegs[type2] > instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->longRegs[type2] > instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->longRegs[type2] > instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
		else if (type2 == M3DS_TYPE_LONGLONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->longlongRegs[type2] > type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->longlongRegs[type2] > instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->longlongRegs[type2] > instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->longlongRegs[type2] > instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->longlongRegs[type2] > instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->longlongRegs[type2] > instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->longlongRegs[type2] > instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->longlongRegs[type2] > instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->longlongRegs[type2] > instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
		else if (type2 == M3DS_TYPE_TEMPLONGLONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->templonglongRegs[type2] > type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->templonglongRegs[type2] > instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->templonglongRegs[type2] > instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->templonglongRegs[type2] > instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->templonglongRegs[type2] > instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->templonglongRegs[type2] > instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->templonglongRegs[type2] > instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->templonglongRegs[type2] > instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->templonglongRegs[type2] > instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
		else if (type2 == M3DS_TYPE_FLOAT_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->floatRegs[type2] > type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->floatRegs[type2] > instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->floatRegs[type2] > instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->floatRegs[type2] > instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->floatRegs[type2] > instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->floatRegs[type2] > instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->floatRegs[type2] > instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->floatRegs[type2] > instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->floatRegs[type2] > instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
		else if (type2 == M3DS_TYPE_DOUBLE_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->doubleRegs[type2] > type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->doubleRegs[type2] > instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->doubleRegs[type2] > instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->doubleRegs[type2] > instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->doubleRegs[type2] > instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->doubleRegs[type2] > instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->doubleRegs[type2] > instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->doubleRegs[type2] > instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->doubleRegs[type2] > instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
	}
	else if (type1 == M3DS_TYPE_INT_REG) {
		if (type2 == M3DS_TYPE_INT_IMM) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (type2 > type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (type2 > instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (type2 > instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (type2 > instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (type2 > instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (type2 > instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (type2 > instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (type2 > instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (type2 > instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_SPEC_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->specialRegs[type2] > type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->specialRegs[type2] > instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->specialRegs[type2] > instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->specialRegs[type2] > instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->specialRegs[type2] > instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->specialRegs[type2] > instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->specialRegs[type2] > instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->specialRegs[type2] > instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->specialRegs[type2] > instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_CHAR_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->charRegs[type2] > type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->charRegs[type2] > instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->charRegs[type2] > instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->charRegs[type2] > instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->charRegs[type2] > instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->charRegs[type2] > instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->charRegs[type2] > instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->charRegs[type2] > instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->charRegs[type2] > instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_INT_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->integerRegs[type2] > type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->integerRegs[type2] > instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->integerRegs[type2] > instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->integerRegs[type2] > instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->integerRegs[type2] > instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->integerRegs[type2] > instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->integerRegs[type2] > instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->integerRegs[type2] > instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->integerRegs[type2] > instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_LONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->longRegs[type2] > type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->longRegs[type2] > instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->longRegs[type2] > instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->longRegs[type2] > instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->longRegs[type2] > instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->longRegs[type2] > instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->longRegs[type2] > instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->longRegs[type2] > instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->longRegs[type2] > instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_LONGLONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->longlongRegs[type2] > type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->longlongRegs[type2] > instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->longlongRegs[type2] > instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->longlongRegs[type2] > instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->longlongRegs[type2] > instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->longlongRegs[type2] > instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->longlongRegs[type2] > instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->longlongRegs[type2] > instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->longlongRegs[type2] > instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_TEMPLONGLONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->templonglongRegs[type2] > type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->templonglongRegs[type2] > instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->templonglongRegs[type2] > instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->templonglongRegs[type2] > instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->templonglongRegs[type2] > instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->templonglongRegs[type2] > instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->templonglongRegs[type2] > instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->templonglongRegs[type2] > instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->templonglongRegs[type2] > instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_FLOAT_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->floatRegs[type2] > type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->floatRegs[type2] > instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->floatRegs[type2] > instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->floatRegs[type2] > instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->floatRegs[type2] > instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->floatRegs[type2] > instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->floatRegs[type2] > instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->floatRegs[type2] > instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->floatRegs[type2] > instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_DOUBLE_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->doubleRegs[type2] > type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->doubleRegs[type2] > instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->doubleRegs[type2] > instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->doubleRegs[type2] > instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->doubleRegs[type2] > instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->doubleRegs[type2] > instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->doubleRegs[type2] > instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->doubleRegs[type2] > instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->doubleRegs[type2] > instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
	}
	else if (type1 == M3DS_TYPE_SPEC_REG) {
		if (type2 == M3DS_TYPE_INT_IMM) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (type2 > type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (type2 > instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (type2 > instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (type2 > instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (type2 > instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (type2 > instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (type2 > instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (type2 > instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (type2 > instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_SPEC_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->specialRegs[type2] > type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->specialRegs[type2] > instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->specialRegs[type2] > instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->specialRegs[type2] > instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->specialRegs[type2] > instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->specialRegs[type2] > instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->specialRegs[type2] > instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->specialRegs[type2] > instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->specialRegs[type2] > instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_CHAR_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->charRegs[type2] > type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->charRegs[type2] > instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->charRegs[type2] > instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->charRegs[type2] > instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->charRegs[type2] > instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->charRegs[type2] > instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->charRegs[type2] > instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->charRegs[type2] > instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->charRegs[type2] > instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_INT_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->integerRegs[type2] > type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->integerRegs[type2] > instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->integerRegs[type2] > instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->integerRegs[type2] > instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->integerRegs[type2] > instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->integerRegs[type2] > instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->integerRegs[type2] > instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->integerRegs[type2] > instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->integerRegs[type2] > instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_LONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->longRegs[type2] > type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->longRegs[type2] > instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->longRegs[type2] > instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->longRegs[type2] > instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->longRegs[type2] > instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->longRegs[type2] > instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->longRegs[type2] > instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->longRegs[type2] > instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->longRegs[type2] > instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_LONGLONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->longlongRegs[type2] > type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->longlongRegs[type2] > instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->longlongRegs[type2] > instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->longlongRegs[type2] > instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->longlongRegs[type2] > instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->longlongRegs[type2] > instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->longlongRegs[type2] > instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->longlongRegs[type2] > instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->longlongRegs[type2] > instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_TEMPLONGLONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->templonglongRegs[type2] > type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->templonglongRegs[type2] > instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->templonglongRegs[type2] > instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->templonglongRegs[type2] > instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->templonglongRegs[type2] > instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->templonglongRegs[type2] > instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->templonglongRegs[type2] > instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->templonglongRegs[type2] > instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->templonglongRegs[type2] > instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_FLOAT_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->floatRegs[type2] > type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->floatRegs[type2] > instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->floatRegs[type2] > instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->floatRegs[type2] > instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->floatRegs[type2] > instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->floatRegs[type2] > instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->floatRegs[type2] > instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->floatRegs[type2] > instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->floatRegs[type2] > instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_DOUBLE_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->doubleRegs[type2] > type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->doubleRegs[type2] > instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->doubleRegs[type2] > instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->doubleRegs[type2] > instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->doubleRegs[type2] > instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->doubleRegs[type2] > instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->doubleRegs[type2] > instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->doubleRegs[type2] > instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->doubleRegs[type2] > instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
	}
	instance->IPJumped = true;
}

inline void Op_JEQ(M3DS_ModuleVM* instance, int type1, int var1, int type2, int var2, int type3, int var3) {
	instance->IP += 7;
	if (type1 == M3DS_TYPE_INT_IMM) {
		if (type2 == M3DS_TYPE_INT_IMM) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (type2 == type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (type2 == instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (type2 == instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (type2 == instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (type2 == instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (type2 == instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (type2 == instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (type2 == instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (type2 == instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
		else if (type2 == M3DS_TYPE_SPEC_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->specialRegs[type2] == type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->specialRegs[type2] == instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->specialRegs[type2] == instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->specialRegs[type2] == instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->specialRegs[type2] == instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->specialRegs[type2] == instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->specialRegs[type2] == instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->specialRegs[type2] == instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->specialRegs[type2] == instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
		else if (type2 == M3DS_TYPE_CHAR_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->charRegs[type2] == type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->charRegs[type2] == instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->charRegs[type2] == instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->charRegs[type2] == instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->charRegs[type2] == instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->charRegs[type2] == instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->charRegs[type2] == instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->charRegs[type2] == instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->charRegs[type2] == instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
		else if (type2 == M3DS_TYPE_INT_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->integerRegs[type2] == type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->integerRegs[type2] == instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->integerRegs[type2] == instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->integerRegs[type2] == instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->integerRegs[type2] == instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->integerRegs[type2] == instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->integerRegs[type2] == instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->integerRegs[type2] == instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->integerRegs[type2] == instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
		else if (type2 == M3DS_TYPE_LONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->longRegs[type2] == type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->longRegs[type2] == instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->longRegs[type2] == instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->longRegs[type2] == instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->longRegs[type2] == instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->longRegs[type2] == instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->longRegs[type2] == instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->longRegs[type2] == instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->longRegs[type2] == instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
		else if (type2 == M3DS_TYPE_LONGLONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->longlongRegs[type2] == type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->longlongRegs[type2] == instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->longlongRegs[type2] == instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->longlongRegs[type2] == instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->longlongRegs[type2] == instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->longlongRegs[type2] == instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->longlongRegs[type2] == instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->longlongRegs[type2] == instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->longlongRegs[type2] == instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
		else if (type2 == M3DS_TYPE_TEMPLONGLONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->templonglongRegs[type2] == type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->templonglongRegs[type2] == instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->templonglongRegs[type2] == instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->templonglongRegs[type2] == instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->templonglongRegs[type2] == instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->templonglongRegs[type2] == instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->templonglongRegs[type2] == instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->templonglongRegs[type2] == instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->templonglongRegs[type2] == instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
		else if (type2 == M3DS_TYPE_FLOAT_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->floatRegs[type2] == type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->floatRegs[type2] == instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->floatRegs[type2] == instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->floatRegs[type2] == instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->floatRegs[type2] == instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->floatRegs[type2] == instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->floatRegs[type2] == instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->floatRegs[type2] == instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->floatRegs[type2] == instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
		else if (type2 == M3DS_TYPE_DOUBLE_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->doubleRegs[type2] == type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->doubleRegs[type2] == instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->doubleRegs[type2] == instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->doubleRegs[type2] == instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->doubleRegs[type2] == instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->doubleRegs[type2] == instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->doubleRegs[type2] == instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->doubleRegs[type2] == instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->doubleRegs[type2] == instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
	}
	else if (type1 == M3DS_TYPE_INT_REG) {
		if (type2 == M3DS_TYPE_INT_IMM) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (type2 == type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (type2 == instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (type2 == instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (type2 == instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (type2 == instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (type2 == instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (type2 == instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (type2 == instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (type2 == instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_SPEC_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->specialRegs[type2] == type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->specialRegs[type2] == instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->specialRegs[type2] == instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->specialRegs[type2] == instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->specialRegs[type2] == instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->specialRegs[type2] == instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->specialRegs[type2] == instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->specialRegs[type2] == instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->specialRegs[type2] == instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_CHAR_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->charRegs[type2] == type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->charRegs[type2] == instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->charRegs[type2] == instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->charRegs[type2] == instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->charRegs[type2] == instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->charRegs[type2] == instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->charRegs[type2] == instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->charRegs[type2] == instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->charRegs[type2] == instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_INT_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->integerRegs[type2] == type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->integerRegs[type2] == instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->integerRegs[type2] == instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->integerRegs[type2] == instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->integerRegs[type2] == instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->integerRegs[type2] == instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->integerRegs[type2] == instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->integerRegs[type2] == instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->integerRegs[type2] == instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_LONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->longRegs[type2] == type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->longRegs[type2] == instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->longRegs[type2] == instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->longRegs[type2] == instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->longRegs[type2] == instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->longRegs[type2] == instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->longRegs[type2] == instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->longRegs[type2] == instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->longRegs[type2] == instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_LONGLONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->longlongRegs[type2] == type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->longlongRegs[type2] == instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->longlongRegs[type2] == instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->longlongRegs[type2] == instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->longlongRegs[type2] == instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->longlongRegs[type2] == instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->longlongRegs[type2] == instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->longlongRegs[type2] == instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->longlongRegs[type2] == instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_TEMPLONGLONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->templonglongRegs[type2] == type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->templonglongRegs[type2] == instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->templonglongRegs[type2] == instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->templonglongRegs[type2] == instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->templonglongRegs[type2] == instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->templonglongRegs[type2] == instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->templonglongRegs[type2] == instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->templonglongRegs[type2] == instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->templonglongRegs[type2] == instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_FLOAT_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->floatRegs[type2] == type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->floatRegs[type2] == instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->floatRegs[type2] == instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->floatRegs[type2] == instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->floatRegs[type2] == instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->floatRegs[type2] == instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->floatRegs[type2] == instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->floatRegs[type2] == instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->floatRegs[type2] == instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_DOUBLE_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->doubleRegs[type2] == type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->doubleRegs[type2] == instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->doubleRegs[type2] == instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->doubleRegs[type2] == instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->doubleRegs[type2] == instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->doubleRegs[type2] == instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->doubleRegs[type2] == instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->doubleRegs[type2] == instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->doubleRegs[type2] == instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
	}
	else if (type1 == M3DS_TYPE_SPEC_REG) {
		if (type2 == M3DS_TYPE_INT_IMM) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (type2 == type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (type2 == instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (type2 == instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (type2 == instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (type2 == instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (type2 == instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (type2 == instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (type2 == instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (type2 == instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_SPEC_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->specialRegs[type2] == type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->specialRegs[type2] == instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->specialRegs[type2] == instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->specialRegs[type2] == instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->specialRegs[type2] == instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->specialRegs[type2] == instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->specialRegs[type2] == instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->specialRegs[type2] == instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->specialRegs[type2] == instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_CHAR_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->charRegs[type2] == type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->charRegs[type2] == instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->charRegs[type2] == instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->charRegs[type2] == instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->charRegs[type2] == instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->charRegs[type2] == instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->charRegs[type2] == instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->charRegs[type2] == instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->charRegs[type2] == instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_INT_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->integerRegs[type2] == type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->integerRegs[type2] == instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->integerRegs[type2] == instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->integerRegs[type2] == instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->integerRegs[type2] == instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->integerRegs[type2] == instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->integerRegs[type2] == instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->integerRegs[type2] == instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->integerRegs[type2] == instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_LONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->longRegs[type2] == type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->longRegs[type2] == instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->longRegs[type2] == instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->longRegs[type2] == instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->longRegs[type2] == instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->longRegs[type2] == instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->longRegs[type2] == instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->longRegs[type2] == instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->longRegs[type2] == instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_LONGLONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->longlongRegs[type2] == type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->longlongRegs[type2] == instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->longlongRegs[type2] == instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->longlongRegs[type2] == instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->longlongRegs[type2] == instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->longlongRegs[type2] == instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->longlongRegs[type2] == instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->longlongRegs[type2] == instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->longlongRegs[type2] == instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_TEMPLONGLONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->templonglongRegs[type2] == type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->templonglongRegs[type2] == instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->templonglongRegs[type2] == instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->templonglongRegs[type2] == instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->templonglongRegs[type2] == instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->templonglongRegs[type2] == instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->templonglongRegs[type2] == instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->templonglongRegs[type2] == instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->templonglongRegs[type2] == instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_FLOAT_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->floatRegs[type2] == type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->floatRegs[type2] == instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->floatRegs[type2] == instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->floatRegs[type2] == instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->floatRegs[type2] == instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->floatRegs[type2] == instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->floatRegs[type2] == instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->floatRegs[type2] == instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->floatRegs[type2] == instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_DOUBLE_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->doubleRegs[type2] == type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->doubleRegs[type2] == instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->doubleRegs[type2] == instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->doubleRegs[type2] == instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->doubleRegs[type2] == instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->doubleRegs[type2] == instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->doubleRegs[type2] == instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->doubleRegs[type2] == instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->doubleRegs[type2] == instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
	}
	instance->IPJumped = true;
}

inline void Op_JNE(M3DS_ModuleVM* instance, int type1, int var1, int type2, int var2, int type3, int var3) {
	instance->IP += 7;
	if (type1 == M3DS_TYPE_INT_IMM) {
		if (type2 == M3DS_TYPE_INT_IMM) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (type2 != type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (type2 != instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (type2 != instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (type2 != instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (type2 != instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (type2 != instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (type2 != instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (type2 != instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (type2 != instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
		else if (type2 == M3DS_TYPE_SPEC_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->specialRegs[type2] != type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->specialRegs[type2] != instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->specialRegs[type2] != instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->specialRegs[type2] != instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->specialRegs[type2] != instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->specialRegs[type2] != instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->specialRegs[type2] != instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->specialRegs[type2] != instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->specialRegs[type2] != instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
		else if (type2 == M3DS_TYPE_CHAR_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->charRegs[type2] != type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->charRegs[type2] != instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->charRegs[type2] != instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->charRegs[type2] != instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->charRegs[type2] != instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->charRegs[type2] != instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->charRegs[type2] != instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->charRegs[type2] != instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->charRegs[type2] != instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
		else if (type2 == M3DS_TYPE_INT_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->integerRegs[type2] != type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->integerRegs[type2] != instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->integerRegs[type2] != instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->integerRegs[type2] != instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->integerRegs[type2] != instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->integerRegs[type2] != instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->integerRegs[type2] != instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->integerRegs[type2] != instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->integerRegs[type2] != instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
		else if (type2 == M3DS_TYPE_LONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->longRegs[type2] != type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->longRegs[type2] != instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->longRegs[type2] != instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->longRegs[type2] != instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->longRegs[type2] != instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->longRegs[type2] != instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->longRegs[type2] != instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->longRegs[type2] != instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->longRegs[type2] != instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
		else if (type2 == M3DS_TYPE_LONGLONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->longlongRegs[type2] != type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->longlongRegs[type2] != instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->longlongRegs[type2] != instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->longlongRegs[type2] != instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->longlongRegs[type2] != instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->longlongRegs[type2] != instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->longlongRegs[type2] != instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->longlongRegs[type2] != instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->longlongRegs[type2] != instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
		else if (type2 == M3DS_TYPE_TEMPLONGLONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->templonglongRegs[type2] != type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->templonglongRegs[type2] != instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->templonglongRegs[type2] != instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->templonglongRegs[type2] != instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->templonglongRegs[type2] != instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->templonglongRegs[type2] != instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->templonglongRegs[type2] != instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->templonglongRegs[type2] != instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->templonglongRegs[type2] != instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
		else if (type2 == M3DS_TYPE_FLOAT_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->floatRegs[type2] != type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->floatRegs[type2] != instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->floatRegs[type2] != instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->floatRegs[type2] != instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->floatRegs[type2] != instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->floatRegs[type2] != instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->floatRegs[type2] != instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->floatRegs[type2] != instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->floatRegs[type2] != instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
		else if (type2 == M3DS_TYPE_DOUBLE_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->doubleRegs[type2] != type3) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->doubleRegs[type2] != instance->charRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->doubleRegs[type2] != instance->specialRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->doubleRegs[type2] != instance->integerRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->doubleRegs[type2] != instance->longRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->doubleRegs[type2] != instance->longlongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->doubleRegs[type2] != instance->templonglongRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->doubleRegs[type2] != instance->floatRegs[type3]) {
					instance->IP = var1;
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->doubleRegs[type2] != instance->doubleRegs[type3]) {
					instance->IP = var1;
				}
			}
		}
	}
	else if (type1 == M3DS_TYPE_INT_REG) {
		if (type2 == M3DS_TYPE_INT_IMM) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (type2 != type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (type2 != instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (type2 != instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (type2 != instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (type2 != instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (type2 != instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (type2 != instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (type2 != instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (type2 != instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_SPEC_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->specialRegs[type2] != type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->specialRegs[type2] != instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->specialRegs[type2] != instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->specialRegs[type2] != instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->specialRegs[type2] != instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->specialRegs[type2] != instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->specialRegs[type2] != instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->specialRegs[type2] != instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->specialRegs[type2] != instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_CHAR_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->charRegs[type2] != type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->charRegs[type2] != instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->charRegs[type2] != instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->charRegs[type2] != instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->charRegs[type2] != instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->charRegs[type2] != instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->charRegs[type2] != instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->charRegs[type2] != instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->charRegs[type2] != instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_INT_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->integerRegs[type2] != type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->integerRegs[type2] != instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->integerRegs[type2] != instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->integerRegs[type2] != instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->integerRegs[type2] != instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->integerRegs[type2] != instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->integerRegs[type2] != instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->integerRegs[type2] != instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->integerRegs[type2] != instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_LONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->longRegs[type2] != type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->longRegs[type2] != instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->longRegs[type2] != instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->longRegs[type2] != instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->longRegs[type2] != instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->longRegs[type2] != instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->longRegs[type2] != instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->longRegs[type2] != instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->longRegs[type2] != instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_LONGLONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->longlongRegs[type2] != type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->longlongRegs[type2] != instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->longlongRegs[type2] != instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->longlongRegs[type2] != instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->longlongRegs[type2] != instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->longlongRegs[type2] != instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->longlongRegs[type2] != instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->longlongRegs[type2] != instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->longlongRegs[type2] != instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_TEMPLONGLONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->templonglongRegs[type2] != type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->templonglongRegs[type2] != instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->templonglongRegs[type2] != instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->templonglongRegs[type2] != instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->templonglongRegs[type2] != instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->templonglongRegs[type2] != instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->templonglongRegs[type2] != instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->templonglongRegs[type2] != instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->templonglongRegs[type2] != instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_FLOAT_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->floatRegs[type2] != type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->floatRegs[type2] != instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->floatRegs[type2] != instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->floatRegs[type2] != instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->floatRegs[type2] != instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->floatRegs[type2] != instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->floatRegs[type2] != instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->floatRegs[type2] != instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->floatRegs[type2] != instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_DOUBLE_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->doubleRegs[type2] != type3) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->doubleRegs[type2] != instance->charRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->doubleRegs[type2] != instance->specialRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->doubleRegs[type2] != instance->integerRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->doubleRegs[type2] != instance->longRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->doubleRegs[type2] != instance->longlongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->doubleRegs[type2] != instance->templonglongRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->doubleRegs[type2] != instance->floatRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->doubleRegs[type2] != instance->doubleRegs[type3]) {
					instance->IP = instance->integerRegs[var1];
				}
			}
		}
	}
	else if (type1 == M3DS_TYPE_SPEC_REG) {
		if (type2 == M3DS_TYPE_INT_IMM) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (type2 != type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (type2 != instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (type2 != instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (type2 != instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (type2 != instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (type2 != instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (type2 != instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (type2 != instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (type2 != instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_SPEC_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->specialRegs[type2] != type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->specialRegs[type2] != instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->specialRegs[type2] != instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->specialRegs[type2] != instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->specialRegs[type2] != instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->specialRegs[type2] != instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->specialRegs[type2] != instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->specialRegs[type2] != instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->specialRegs[type2] != instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_CHAR_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->charRegs[type2] != type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->charRegs[type2] != instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->charRegs[type2] != instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->charRegs[type2] != instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->charRegs[type2] != instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->charRegs[type2] != instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->charRegs[type2] != instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->charRegs[type2] != instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->charRegs[type2] != instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_INT_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->integerRegs[type2] != type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->integerRegs[type2] != instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->integerRegs[type2] != instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->integerRegs[type2] != instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->integerRegs[type2] != instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->integerRegs[type2] != instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->integerRegs[type2] != instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->integerRegs[type2] != instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->integerRegs[type2] != instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_LONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->longRegs[type2] != type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->longRegs[type2] != instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->longRegs[type2] != instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->longRegs[type2] != instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->longRegs[type2] != instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->longRegs[type2] != instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->longRegs[type2] != instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->longRegs[type2] != instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->longRegs[type2] != instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_LONGLONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->longlongRegs[type2] != type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->longlongRegs[type2] != instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->longlongRegs[type2] != instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->longlongRegs[type2] != instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->longlongRegs[type2] != instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->longlongRegs[type2] != instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->longlongRegs[type2] != instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->longlongRegs[type2] != instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->longlongRegs[type2] != instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_TEMPLONGLONG_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->templonglongRegs[type2] != type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->templonglongRegs[type2] != instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->templonglongRegs[type2] != instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->templonglongRegs[type2] != instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->templonglongRegs[type2] != instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->templonglongRegs[type2] != instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->templonglongRegs[type2] != instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->templonglongRegs[type2] != instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->templonglongRegs[type2] != instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_FLOAT_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->floatRegs[type2] != type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->floatRegs[type2] != instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->floatRegs[type2] != instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->floatRegs[type2] != instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->floatRegs[type2] != instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->floatRegs[type2] != instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->floatRegs[type2] != instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->floatRegs[type2] != instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->floatRegs[type2] != instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
		else if (type2 == M3DS_TYPE_DOUBLE_REG) {
			if (type3 == M3DS_TYPE_INT_IMM) {
				if (instance->doubleRegs[type2] != type3) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_CHAR_REG) {
				if (instance->doubleRegs[type2] != instance->charRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_SPEC_REG) {
				if (instance->doubleRegs[type2] != instance->specialRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_INT_REG) {
				if (instance->doubleRegs[type2] != instance->integerRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONG_REG) {
				if (instance->doubleRegs[type2] != instance->longRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_LONGLONG_REG) {
				if (instance->doubleRegs[type2] != instance->longlongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_TEMPLONGLONG_REG) {
				if (instance->doubleRegs[type2] != instance->templonglongRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_FLOAT_REG) {
				if (instance->doubleRegs[type2] != instance->floatRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
			else if (type3 == M3DS_TYPE_DOUBLE_REG) {
				if (instance->doubleRegs[type2] != instance->doubleRegs[type3]) {
					instance->IP = instance->specialRegs[var1];
				}
			}
		}
	}
	instance->IPJumped = true;
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

void M3DS_ExecuteModuleInstructions(unsigned int vm_instance_ID, std::vector<int> instructions) {
	unsigned int iCount = instructions.size();
	while (vmInstances[vm_instance_ID]->IP < iCount) {
		switch (instructions[vmInstances[vm_instance_ID]->IP]) {
		case M3DS_OP_NOP:
			Op_NOP(vmInstances[vm_instance_ID]);
			break;
		case M3DS_OP_RET:
			Op_RET(vmInstances[vm_instance_ID]);
			break;
		case M3DS_OP_JEQ:
			Op_JEQ(vmInstances[vm_instance_ID], instructions[vmInstances[vm_instance_ID]->IP + 1], instructions[vmInstances[vm_instance_ID]->IP + 2], instructions[vmInstances[vm_instance_ID]->IP + 3], instructions[vmInstances[vm_instance_ID]->IP + 4], instructions[vmInstances[vm_instance_ID]->IP + 5], instructions[vmInstances[vm_instance_ID]->IP + 6]);
			break;
		case M3DS_OP_JNE:
			Op_JNE(vmInstances[vm_instance_ID], instructions[vmInstances[vm_instance_ID]->IP + 1], instructions[vmInstances[vm_instance_ID]->IP + 2], instructions[vmInstances[vm_instance_ID]->IP + 3], instructions[vmInstances[vm_instance_ID]->IP + 4], instructions[vmInstances[vm_instance_ID]->IP + 5], instructions[vmInstances[vm_instance_ID]->IP + 6]);
			break;
		case M3DS_OP_JLE:
			Op_JLE(vmInstances[vm_instance_ID], instructions[vmInstances[vm_instance_ID]->IP + 1], instructions[vmInstances[vm_instance_ID]->IP + 2], instructions[vmInstances[vm_instance_ID]->IP + 3], instructions[vmInstances[vm_instance_ID]->IP + 4], instructions[vmInstances[vm_instance_ID]->IP + 5], instructions[vmInstances[vm_instance_ID]->IP + 6]);
			break;
		case M3DS_OP_JLT:
			Op_JLT(vmInstances[vm_instance_ID], instructions[vmInstances[vm_instance_ID]->IP + 1], instructions[vmInstances[vm_instance_ID]->IP + 2], instructions[vmInstances[vm_instance_ID]->IP + 3], instructions[vmInstances[vm_instance_ID]->IP + 4], instructions[vmInstances[vm_instance_ID]->IP + 5], instructions[vmInstances[vm_instance_ID]->IP + 6]);
			break;
		case M3DS_OP_JGE:
			Op_JGE(vmInstances[vm_instance_ID], instructions[vmInstances[vm_instance_ID]->IP + 1], instructions[vmInstances[vm_instance_ID]->IP + 2], instructions[vmInstances[vm_instance_ID]->IP + 3], instructions[vmInstances[vm_instance_ID]->IP + 4], instructions[vmInstances[vm_instance_ID]->IP + 5], instructions[vmInstances[vm_instance_ID]->IP + 6]);
			break;
		case M3DS_OP_JGT:
			Op_JGT(vmInstances[vm_instance_ID], instructions[vmInstances[vm_instance_ID]->IP + 1], instructions[vmInstances[vm_instance_ID]->IP + 2], instructions[vmInstances[vm_instance_ID]->IP + 3], instructions[vmInstances[vm_instance_ID]->IP + 4], instructions[vmInstances[vm_instance_ID]->IP + 5], instructions[vmInstances[vm_instance_ID]->IP + 6]);
			break;
		case M3DS_OP_JMP:
			Op_JMP(vmInstances[vm_instance_ID], instructions[vmInstances[vm_instance_ID]->IP + 1], instructions[vmInstances[vm_instance_ID]->IP + 2]);
			break;
		case M3DS_OP_SAVE:
			Op_SAVE(vmInstances[vm_instance_ID]);
			break;
		case M3DS_OP_LOOP:
			Op_LOOP(vmInstances[vm_instance_ID], instructions[vmInstances[vm_instance_ID]->IP + 1], instructions[vmInstances[vm_instance_ID]->IP + 2], instructions[vmInstances[vm_instance_ID]->IP + 3], instructions[vmInstances[vm_instance_ID]->IP + 4]);
			break;
		default:
			break;
		}
	}
}