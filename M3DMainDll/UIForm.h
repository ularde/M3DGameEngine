#pragma once
#include "BasicContainer.h"
#include "UIControl.h"

class MUIControl;

class MUIForm :public MBasicContainer {
public:
	MUIForm(bool immediately_load, MBasicPlatform* platform_, const std::string& path_);
	~MUIForm();
	virtual void Update(double dt)override;
	virtual void Render()override;
	virtual void Release()override;
	virtual void InitializeLuaInstance()override;
	void TestAPI() { printf("Hello, API.\n"); }
	void SaveForm(const std::string& path);
	void SaveFormWithFullPath(const std::string& path);
	void StartSimulation();
	void ExitSimulation();
	bool IsEditorMode() { return this->mEditorModeFlag; }
	void OnCreate();
	void OnDestroy();
	void OnPreUpdate(double dt);
	void OnPreRender(double dt);
	void OnPostRender();
	std::vector<MUIControl*> mControls;
	std::map<std::string, MUIControl*>mControlMap;
protected:
	bool mEditorModeFlag = false;
	bool mLoaded = false;
	tinyxml2::XMLDocument mFormDoc;
	void AddLabel(const std::string& name, tinyxml2::XMLElement* root);
	void AddButton(const std::string& name, tinyxml2::XMLElement* root);
	void AddInputBox(const std::string& name, tinyxml2::XMLElement* root);
	void AddTabBox(const std::string& name, tinyxml2::XMLElement* root);
	void LoadForm();
	void GenerateFunctionNames();
private:
	std::string nOnCreate, nOnPreUpdate,
		nOnPreRender, nOnDestroy,
		nOnPostRender;
};