#pragma once
#include "SensorCustomRenderAPI.h"


class SENSORCUSTOMRENDER_API FBlendCamera_ViewExtension : public ISensorCustomRenderExtension
{
public:
	FBlendCamera_ViewExtension();
	virtual ~FBlendCamera_ViewExtension();
	
	virtual void SetupViewFamily(FSceneViewFamily& InViewFamily)override;
	virtual void SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView)override {};
	virtual void BeginRenderViewFamily(FSceneViewFamily& InViewFamily)override {};
	virtual void PreRenderViewFamily_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneViewFamily& InViewFamily)override {};
	virtual void PreRenderView_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneView& InView)override {}
	virtual void PostRenderViewFamily_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneViewFamily& InViewFamily)override;
	virtual void PrePostProcessPass_RenderThread(FRDGBuilder& GraphBuilder, const FSceneView& View, const FPostProcessingInputs& Inputs)override;
	virtual void PostRenderBasePass_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneView& InView)override;
	virtual void PostRenderView_RenderThread(FRDGBuilder& GraphBuilder, FSceneView& InView)override;
	
protected:
	
	void ApplyRenderWrap_BlendCamera(FRHICommandListImmediate& RHICmdList, FSceneViewFamily& InViewFamily);
	void ApplyRenderWrap_BlendCamera(FRDGBuilder& GraphBuilder, const FSceneView& View, const FPostProcessingInputs& Inputs);
	void ApplyRenderWrap_BlendCamera(FRDGBuilder& GraphBuilder, const FSceneView& View);
	void OnResolvedSceneColorCallback(FRHICommandListImmediate& RHICmdList, class FSceneRenderTargets& SceneContext);
};