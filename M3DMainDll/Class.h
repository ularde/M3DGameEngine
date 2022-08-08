#pragma once
#include <Python.h>
#include <jemalloc/jemalloc.h>

class MPythonBoundObject {
public:
	void* operator new(size_t size) {
		return je_malloc(size);
	}
	void operator delete[](void* ptr) {
		return je_free(ptr);
	}
protected:
	PyObject* pythonModule = NULL;

	void InitializePythonModule(const std::string& module_path);
	void UninitializePythonModule();
};

class MNonPythonBoundObject {
public:
	void* operator new(size_t size) {
		return je_malloc(size);
	}
	void operator delete[](void* ptr) {
		return je_free(ptr);
	}
};