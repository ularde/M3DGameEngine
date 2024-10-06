#include "pch.h"
#include "RenderPipeline.h"

void MRenderPipeline::LoadIdentity(MMatrixType type) {
	LoadMatrix(type, glm::mat4(1.0f));
}

void MRenderPipeline::LoadMatrix(MMatrixType type, const glm::mat4& matrix) {
	switch (type)
	{
	case MMatrixType::PROJECTION:
		gCurrentProjection = matrix;
		break;
	case MMatrixType::VIEW:
		gCurrentView = matrix;
		break;
	case MMatrixType::MODEL:
		gCurrentModel = matrix;
		break;
	case MMatrixType::TEXTURE:
		break;
	default:
		break;
	}
	SendMatricesToShader();
}

void MRenderPipeline::MultMatrix(MMatrixType type, const glm::mat4& matrix) {
	switch (type)
	{
	case MMatrixType::PROJECTION:
		gCurrentProjection *= matrix;
		break;
	case MMatrixType::VIEW:
		gCurrentView *= matrix;
		break;
	case MMatrixType::MODEL:
		gCurrentModel *= matrix;
		break;
	case MMatrixType::TEXTURE:
		break;
	default:
		break;
	}
	SendMatricesToShader();
}

void MRenderPipeline::PushMatrix(MMatrixType type) {
	switch (type)
	{
	case MMatrixType::PROJECTION:
		gProjectionStack = gCurrentProjection;
		break;
	case MMatrixType::VIEW:
		gViewStack = gCurrentView;
		break;
	case MMatrixType::MODEL:
		gModelStack = gCurrentModel;
		break;
	case MMatrixType::TEXTURE:
		break;
	default:
		break;
	}
	SendMatricesToShader();
}

void MRenderPipeline::PopMatrix(MMatrixType type) {
	switch (type)
	{
	case MMatrixType::PROJECTION:
		gCurrentProjection = gProjectionStack;
		break;
	case MMatrixType::VIEW:
		gCurrentView = gViewStack;
		break;
	case MMatrixType::MODEL:
		gCurrentModel = gModelStack;
		break;
	case MMatrixType::TEXTURE:
		break;
	default:
		break;
	}
	SendMatricesToShader();
}
