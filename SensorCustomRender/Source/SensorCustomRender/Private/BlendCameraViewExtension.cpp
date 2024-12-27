#include "BlendCameraViewExtension.h"

#include "EngineModule.h"

#include "SceneRendering.h"
#include "ScreenRendering.h"
#include "RenderGraphParameter.h"
#include "PostProcess/PostProcessing.h"
#include "PostProcess/SceneFilterRendering.h"

#define USE_RENDER_RDG 1
BEGIN_SHADER_PARAMETER_STRUCT(FBlendCameraShaderParameters, )
SHADER_PARAMETER_STRUCT_REF(FViewUniformShaderParameters, View)
#if USE_RENDER_RDG
SHADER_PARAMETER_RDG_UNIFORM_BUFFER(FSceneTextureUniformParameters, SceneTextures)
#else
SHADER_PARAMETER_STRUCT_REF(FSceneTextureUniformParameters, SceneTextures)
#endif
SHADER_PARAMETER(int32, LightEffectIntensity)//光照是否影响强度数据
SHADER_PARAMETER(int32, CoordSystemType)//坐标系类型：0为世界坐标系，1为局部坐标系
//SHADER_PARAMETER(FVector2D, CameraFOV)//视场角（弧度）
RENDER_TARGET_BINDING_SLOTS()
END_SHADER_PARAMETER_STRUCT()

#pragma optimize("", off)
// A pixel shader for capturing a component of the rendered scene for a scene capture.
class FBlendCameraRenderPS : public FGlobalShader
{
public:
	DECLARE_GLOBAL_SHADER(FBlendCameraRenderPS);
	SHADER_USE_PARAMETER_STRUCT(FBlendCameraRenderPS, FGlobalShader);

	using FParameters = FBlendCameraShaderParameters;

	class FSourceModeDimension : SHADER_PERMUTATION_ENUM_CLASS("SOURCE_MODE", EBlendCamera_SourceMode);
	using FPermutationDomain = TShaderPermutationDomain<FSourceModeDimension>;

	static FPermutationDomain GetPermutationVector(EBlendCamera_SourceMode SourceMode)
	{
		FPermutationDomain PermutationVector;
		PermutationVector.Set<FSourceModeDimension>(SourceMode);
		return PermutationVector;
	}

	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
	{
		return /*IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5)*/true;
	}

	static void ModifyCompilationEnvironment(const FGlobalShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment)
	{
		static const TCHAR* ShaderSourceModeDefineName[] =
		{
			TEXT("SOURCE_MODE_LIDAR_AND_INTENSITY"),
			TEXT("SOURCE_MODE_ANGLE_DEPTH_INTENSITY"),
			TEXT("SOURCE_MODE_DEPTH_AND_INTENSITY"),
		};
		static_assert(UE_ARRAY_COUNT(ShaderSourceModeDefineName) == (uint32)EBlendCamera_SourceMode::MAX, "EBlendCamera_SourceMode doesn't match define table.");

		const FPermutationDomain PermutationVector(Parameters.PermutationId);
		const uint32 SourceModeIndex = static_cast<uint32>(PermutationVector.Get<FSourceModeDimension>());
		OutEnvironment.SetDefine(ShaderSourceModeDefineName[SourceModeIndex], 1u);
	}
};

IMPLEMENT_GLOBAL_SHADER(FBlendCameraRenderPS, "/SensorCustomRender/Private/BlendCameraRender.usf", "BlendCamera_MainPS", SF_Pixel);


FBlendCamera_ViewExtension::FBlendCamera_ViewExtension()
{
	//GetRendererModule().GetResolvedSceneColorCallbacks().AddRaw(this, &FBlendCamera_ViewExtension::OnResolvedSceneColorCallback);
}

FBlendCamera_ViewExtension::~FBlendCamera_ViewExtension()
{
	GetRendererModule().GetResolvedSceneColorCallbacks().RemoveAll(this);
}

void FBlendCamera_ViewExtension::SetupViewFamily(FSceneViewFamily& InViewFamily)
{
	{
		InViewFamily.bResolveScene = false;
#if !USE_RENDER_RDG
		InViewFamily.SceneCaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;
#endif
	}
}

void FBlendCamera_ViewExtension::PostRenderViewFamily_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneViewFamily& InViewFamily)
{
	ApplyRenderWrap_BlendCamera(RHICmdList, InViewFamily);
}

void FBlendCamera_ViewExtension::PrePostProcessPass_RenderThread(FRDGBuilder& GraphBuilder, const FSceneView& View, const FPostProcessingInputs& Inputs)
{
	ApplyRenderWrap_BlendCamera(GraphBuilder, View, Inputs);
}

void FBlendCamera_ViewExtension::PostRenderBasePass_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneView& InView)
{
}

void FBlendCamera_ViewExtension::PostRenderView_RenderThread(FRDGBuilder& GraphBuilder, FSceneView& InView)
{
	ApplyRenderWrap_BlendCamera(GraphBuilder, InView);
}

void FBlendCamera_ViewExtension::ApplyRenderWrap_BlendCamera(FRHICommandListImmediate& RHICmdList, FSceneViewFamily& InViewFamily)
{
#if !USE_RENDER_RDG
	if (CameraSourceMode != EBlendCamera_SourceMode::MAX)
	{
		QUICK_SCOPE_CYCLE_COUNTER(STAT_BlendCameraRender_PostRender);
		SCOPED_DRAW_EVENT(RHICmdList, BlendCameraRender_Pixel);


		RHICmdList.ImmediateFlush(EImmediateFlushType::FlushRHIThreadFlushResources);
		TArray<const FSceneView*> SceneViews = InViewFamily.Views;
		const FBlendCameraRenderPS::FPermutationDomain PixelPermutationVector = FBlendCameraRenderPS::GetPermutationVector(CameraSourceMode);
		for (int32 ViewIndex = 0; ViewIndex < SceneViews.Num(); ++ViewIndex)
		{
			if (SceneViews[ViewIndex]->bIsViewInfo)
			{
				const FViewInfo& View = *((const FViewInfo*)SceneViews[ViewIndex]);

				FRHIRenderPassInfo RenderPassInfo(InViewFamily.RenderTarget->GetRenderTargetTexture(), ERenderTargetActions::Clear_Store);
				RHICmdList.BeginRenderPass(RenderPassInfo, TEXT("ShaderPlugin_OutputToRenderTarget"));

				auto ShaderMap = GetGlobalShaderMap(GMaxRHIFeatureLevel);
				TShaderMapRef<FScreenVS> VertexShader(View.ShaderMap);
				TShaderMapRef<FBlendCameraRenderPS> PixelShader(View.ShaderMap, PixelPermutationVector);

				FBlendCameraRenderPS::FParameters PassParameters;
				PassParameters.View = View.ViewUniformBuffer;
				PassParameters.SceneTextures = CreateSceneTextureUniformBuffer(RHICmdList, nullptr,  GMaxRHIFeatureLevel, ESceneTextureSetupMode::All);
				PassParameters.CoordSystemType = (int32)CoordSystemType;
				//PassParameters.CameraFOV = CameraFOV_Radian;
				PassParameters.LightEffectIntensity = bLightEffectIntensity ? 1 : 0;
			//	PassParameters.RenderTargets[0] = FRenderTargetBinding(ProcessInputs->ViewFamilyTexture, ERenderTargetLoadAction::ENoAction);

				FUniformBufferStaticBindings GlobalUniformBuffers(PassParameters.SceneTextures);
				SCOPED_UNIFORM_BUFFER_GLOBAL_BINDINGS(RHICmdList, GlobalUniformBuffers);

				SetShaderParameters(RHICmdList, PixelShader, PixelShader.GetPixelShader(), PassParameters);

				// Set the graphic pipeline state.
				FGraphicsPipelineStateInitializer GraphicsPSOInit;
				RHICmdList.ApplyCachedRenderTargets(GraphicsPSOInit);
				GraphicsPSOInit.BlendState = TStaticBlendState<>::GetRHI();
				GraphicsPSOInit.RasterizerState = TStaticRasterizerState<>::GetRHI();
				GraphicsPSOInit.DepthStencilState = TStaticDepthStencilState<false, CF_Always>::GetRHI();
				GraphicsPSOInit.BoundShaderState.VertexDeclarationRHI = GFilterVertexDeclaration.VertexDeclarationRHI;
				GraphicsPSOInit.BoundShaderState.VertexShaderRHI = VertexShader.GetVertexShader();
				GraphicsPSOInit.BoundShaderState.PixelShaderRHI = PixelShader.GetPixelShader();
				GraphicsPSOInit.PrimitiveType = PT_TriangleStrip;

				SetGraphicsPipelineState(RHICmdList, GraphicsPSOInit, EApplyRendertargetOption::DoNothing);

				DrawRectangle(
					RHICmdList,
					View.ViewRect.Min.X, View.ViewRect.Min.Y,
					View.ViewRect.Width(), View.ViewRect.Height(),
					View.ViewRect.Min.X, View.ViewRect.Min.Y,
					View.ViewRect.Width(), View.ViewRect.Height(),
					View.UnconstrainedViewRect.Size(),
					FSceneRenderTargets::Get(RHICmdList).GetBufferSizeXY(),
					VertexShader,
					EDRF_UseTriangleOptimization);

				RHICmdList.EndRenderPass();
			}
		}
	}
#endif
}

void FBlendCamera_ViewExtension::ApplyRenderWrap_BlendCamera(FRDGBuilder& GraphBuilder, const FSceneView& View, const FPostProcessingInputs& Inputs)
{
#if USE_RENDER_RDG
	if (CameraSourceMode != EBlendCamera_SourceMode::MAX)
	{
		if (View.bIsViewInfo)
		{
			const FViewInfo& ViewInfo = *(static_cast<const FViewInfo*>(&View));

			QUICK_SCOPE_CYCLE_COUNTER(STAT_BlendCameraRender_PostRender);
			RDG_EVENT_SCOPE(GraphBuilder, "BlendCameraRender_[%d]", CameraSourceMode);

			const FIntRect Viewport = static_cast<const FViewInfo&>(View).ViewRect;
			FScreenPassTexture SceneColor((*Inputs.SceneTextures)->SceneColorTexture, Viewport);

			const auto& ViewFamily = View.Family;
			FGraphicsPipelineStateInitializer GraphicsPSOInit;
			GraphicsPSOInit.RasterizerState = TStaticRasterizerState<FM_Solid, CM_None>::GetRHI();
			GraphicsPSOInit.DepthStencilState = TStaticDepthStencilState<false, CF_Always>::GetRHI();
			GraphicsPSOInit.BlendState = TStaticBlendState<>::GetRHI();
			const FBlendCameraRenderPS::FPermutationDomain PixelPermutationVector = FBlendCameraRenderPS::GetPermutationVector(CameraSourceMode);

			TRDGUniformBufferRef<FSceneTextureUniformParameters> SceneTextureUniformBuffer = CreateSceneTextureUniformBuffer(GraphBuilder, nullptr, ViewFamily->GetFeatureLevel(), ESceneTextureSetupMode::GBuffers | ESceneTextureSetupMode::SceneColor | ESceneTextureSetupMode::SceneDepth);

			FBlendCameraRenderPS::FParameters* PassParameters = GraphBuilder.AllocParameters<FBlendCameraRenderPS::FParameters>();
			PassParameters->View = View.ViewUniformBuffer;
			PassParameters->SceneTextures = Inputs.SceneTextures;
			PassParameters->RenderTargets[0] = FRenderTargetBinding(SceneColor.Texture, ERenderTargetLoadAction::ENoAction);/*FRenderTargetBinding(Inputs.ViewFamilyTexture, ERenderTargetLoadAction::ELoad);*/
			//const FSceneRenderTargets& SceneContext = FSceneRenderTargets::Get(GraphBuilder.RHICmdList);
			TShaderMapRef<FScreenVS> VertexShader(ViewInfo.ShaderMap);
			TShaderMapRef<FBlendCameraRenderPS> PixelShader(ViewInfo.ShaderMap, PixelPermutationVector);

			GraphicsPSOInit.BoundShaderState.VertexDeclarationRHI = GFilterVertexDeclaration.VertexDeclarationRHI;
			GraphicsPSOInit.BoundShaderState.VertexShaderRHI = VertexShader.GetVertexShader();
			GraphicsPSOInit.BoundShaderState.PixelShaderRHI = PixelShader.GetPixelShader();
			GraphicsPSOInit.PrimitiveType = PT_TriangleList;

			GraphBuilder.AddPass(
				RDG_EVENT_NAME("View_Blend"),
				PassParameters,
				ERDGPassFlags::Raster,
				[PassParameters, GraphicsPSOInit, VertexShader, PixelShader, &ViewInfo](FRHICommandListImmediate& RHICmdList)
				{
					FGraphicsPipelineStateInitializer LocalGraphicsPSOInit = GraphicsPSOInit;
					RHICmdList.ApplyCachedRenderTargets(LocalGraphicsPSOInit);
					SetGraphicsPipelineState(RHICmdList, LocalGraphicsPSOInit);
					SetShaderParameters(RHICmdList, PixelShader, PixelShader.GetPixelShader(), *PassParameters);

					
					DrawRectangle(
						RHICmdList,
						ViewInfo.ViewRect.Min.X, ViewInfo.ViewRect.Min.Y,
						ViewInfo.ViewRect.Width() / 2, ViewInfo.ViewRect.Height() / 2,
						ViewInfo.ViewRect.Min.X, ViewInfo.ViewRect.Min.Y,
						ViewInfo.ViewRect.Width() / 2, ViewInfo.ViewRect.Height() / 2,
						ViewInfo.UnconstrainedViewRect.Size(),
						ViewInfo.GetSceneTexturesConfig().Extent,
						VertexShader,
						EDRF_UseTriangleOptimization);
				});
		}
	}
#endif
}

void FBlendCamera_ViewExtension::ApplyRenderWrap_BlendCamera(FRDGBuilder& GraphBuilder, const FSceneView& View)
{
#if USE_RENDER_RDG
	if (CameraSourceMode != EBlendCamera_SourceMode::MAX)
	{
		if (View.bIsViewInfo)
		{
			const FViewInfo& ViewInfo = *(static_cast<const FViewInfo*>(&View));

			QUICK_SCOPE_CYCLE_COUNTER(STAT_BlendCameraRender_PostRender);
			RDG_EVENT_SCOPE(GraphBuilder, "BlendCameraRender_[%d]", CameraSourceMode);

			auto& ViewFamily = View.Family;


			FGraphicsPipelineStateInitializer GraphicsPSOInit;
			GraphicsPSOInit.BlendState = TStaticBlendState<>::GetRHI();
			GraphicsPSOInit.RasterizerState = TStaticRasterizerState<>::GetRHI();
			GraphicsPSOInit.DepthStencilState = TStaticDepthStencilState<false, CF_Always>::GetRHI();
			const FBlendCameraRenderPS::FPermutationDomain PixelPermutationVector = FBlendCameraRenderPS::GetPermutationVector(CameraSourceMode);

			FRDGTextureRef ViewFamilyTexture = TryCreateViewFamilyTexture(GraphBuilder, *ViewFamily);

			FBlendCameraRenderPS::FParameters* PassParameters = GraphBuilder.AllocParameters<FBlendCameraRenderPS::FParameters>();
			PassParameters->View = View.ViewUniformBuffer;
			PassParameters->SceneTextures = GetSceneTextureShaderParameters(View).SceneTextures;
			PassParameters->RenderTargets[0] = FRenderTargetBinding(ViewFamilyTexture, ERenderTargetLoadAction::ENoAction);
			PassParameters->CoordSystemType = (int32)CoordSystemType;
			//PassParameters->CameraFOV = CameraFOV_Radian;
			PassParameters->LightEffectIntensity = bLightEffectIntensity ? 1 : 0;
			TShaderMapRef<FScreenVS> VertexShader(ViewInfo.ShaderMap);
			TShaderMapRef<FBlendCameraRenderPS> PixelShader(ViewInfo.ShaderMap, PixelPermutationVector);

			GraphicsPSOInit.BoundShaderState.VertexDeclarationRHI = GFilterVertexDeclaration.VertexDeclarationRHI;
			GraphicsPSOInit.BoundShaderState.VertexShaderRHI = VertexShader.GetVertexShader();
			GraphicsPSOInit.BoundShaderState.PixelShaderRHI = PixelShader.GetPixelShader();
			GraphicsPSOInit.PrimitiveType = PT_TriangleStrip;

			GraphBuilder.AddPass(
				RDG_EVENT_NAME("View_Blend"),
				PassParameters,
				ERDGPassFlags::Raster,
				[PassParameters, GraphicsPSOInit, VertexShader, PixelShader, &ViewInfo](FRHICommandListImmediate& RHICmdList)
				{
					IRendererModule* RendererModule = &FModuleManager::GetModuleChecked<IRendererModule>("Renderer");

					FGraphicsPipelineStateInitializer LocalGraphicsPSOInit = GraphicsPSOInit;
					RHICmdList.ApplyCachedRenderTargets(LocalGraphicsPSOInit);
					SetGraphicsPipelineState(RHICmdList, LocalGraphicsPSOInit, 0);
					SetShaderParameters(RHICmdList, PixelShader, PixelShader.GetPixelShader(), *PassParameters);

					RendererModule->DrawRectangle(
						RHICmdList,
						ViewInfo.ViewRect.Min.X, ViewInfo.ViewRect.Min.Y,
						ViewInfo.ViewRect.Width(), ViewInfo.ViewRect.Height(),
						ViewInfo.ViewRect.Min.X, ViewInfo.ViewRect.Min.Y,
						ViewInfo.ViewRect.Width(), ViewInfo.ViewRect.Height(),
						ViewInfo.UnconstrainedViewRect.Size(),
						ViewInfo.GetSceneTexturesConfig().Extent,
						VertexShader,
						EDRF_UseTriangleOptimization);

				});

		}
	}
#endif
}

void FBlendCamera_ViewExtension::OnResolvedSceneColorCallback(FRHICommandListImmediate& RHICmdList, class FSceneRenderTargets& SceneContext)
{
	
}

#pragma optimize("", on)