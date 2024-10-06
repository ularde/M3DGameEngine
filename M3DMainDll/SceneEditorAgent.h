#pragma once
#include "Class.h"
#include "Global.h"
#include <PxQueryReport.h>
#include <PxPhysicsAPI.h>

class MScene;
class MActor;
class MRigidStatic;

class MSceneEditorAgent :public MUnscriptableObject {
public:
	MSceneEditorAgent(MBasicPlatform* platform, MScene* parent);
	~MSceneEditorAgent();
	void CreateRigidStatic(const std::string& model);
	void CreateEntity(const std::string& def);
	void SetEditMode(MEditMode mode);
	bool IsAnyAxisFocused();
	bool GetAnyAxisSelected();
	bool GetAxisSelectionFlag(MAxis axis);
	void SelectAxis(MAxis axis, bool flag);
	void Tick();
	void SetTransforming();
	void CancelTransforming();
	MEditMode GetEditMode() { return gCurrentEditMode; }
protected:
	bool mXAxisSelected = false, mYAxisSelected = false, mZAxisSelected = false;
	MEditMode gCurrentEditMode = MEditMode::NONE;
	MScene* mParent = NULL;
	MActor* mCreatingActor = NULL;
};