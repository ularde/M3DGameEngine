#include "pch.h"
#include "SharedFunc.h"
#include "UIForm.h"

MUIForm::MUIForm(bool immediately_load, MBasicPlatform* platform_, const std::string& path_) {
	this->gPlatform = platform_;
	this->path = path_;
	this->mClassName = "Form";
	if (this->mFormDoc.LoadFile(this->path.c_str()) != tinyxml2::XML_SUCCESS) {
		MessageBox(NULL, L"Failed to load form from an XML document.", L"M3D GameEngine", MB_ICONERROR);
	}
	else {
		this->InitializeLuaInstance();
		this->LoadForm();
	}
}

MUIForm::~MUIForm() {

}

void MUIForm::Update(double dt) {

}

void MUIForm::Render() {

}

void MUIForm::Release() {

}

void MUIForm::SaveForm(const std::string& path) {

}

void MUIForm::SaveFormWithFullPath(const std::string& path) {

}

void MUIForm::StartSimulation() {

}

void MUIForm::ExitSimulation() {

}

void MUIForm::OnCreate() {

}

void MUIForm::OnDestroy() {

}

void MUIForm::OnPreUpdate(double dt) {

}

void MUIForm::OnPreRender(double dt) {

}

void MUIForm::OnPostRender() {

}

void MUIForm::GenerateFunctionNames() {
	nOnCreate = mName + std::string("_OnCreate");
	nOnPreUpdate = mName + std::string("_OnPreUpdate");
	nOnPreRender = mName + std::string("_OnPreRender");
	nOnDestroy = mName + std::string("_OnDestroy");
	nOnPostRender = mName + std::string("_OnPostRender");
}

void MUIForm::AddLabel(const std::string& name, tinyxml2::XMLElement* root)
{
}

void MUIForm::AddButton(const std::string& name, tinyxml2::XMLElement* root)
{
}

void MUIForm::AddInputBox(const std::string& name, tinyxml2::XMLElement* root)
{
}

void MUIForm::AddTabBox(const std::string& name, tinyxml2::XMLElement* root)
{
}

void MUIForm::InitializeLuaInstance() {
	gPlatform->gLuaState->safe_script_file(GetFullAssetPathA(mLuaScriptPath));
	this->GenerateFunctionNames();
}

void MUIForm::LoadForm() {

}
