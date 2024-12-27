#include "SensorCustomRenderAPI.h"
#include "BlendCameraViewExtension.h"

ISensorCustomRenderExtension::ISensorCustomRenderExtension()
	: CameraSourceMode(EBlendCamera_SourceMode::MAX)
	, CoordSystemType(FBlendCamera_ViewExtension::LOCAL_XYZ)
	, CameraFOV_Radian(PI * 0.5, PI * 0.5)
	, bLightEffectIntensity(true)
{
	
}

ISensorCustomRenderExtension::~ISensorCustomRenderExtension()
{

}

void ISensorCustomRenderExtension::SetCoordSystemType(ECoordSystemType CoordType)
{
	CoordSystemType = CoordType;
}

void ISensorCustomRenderExtension::SetCameraSourceMode(EBlendCamera_SourceMode InCameraSourceMode)
{
	CameraSourceMode = InCameraSourceMode;
}

void ISensorCustomRenderExtension::SetFOVAngle(const float InHorizonAngle)
{
	
}

void ISensorCustomRenderExtension::SetFOVAngle2D(const FVector2D InFOVAngle)
{
	CameraFOV_Radian = FVector2D(FMath::DegreesToRadians(InFOVAngle.X), FMath::DegreesToRadians(InFOVAngle.Y));
}

void ISensorCustomRenderExtension::SetLightEffectIntensity(bool bEffectIntensity)
{
	bLightEffectIntensity = bEffectIntensity;
}

TSharedPtr<ISensorCustomRenderExtension, ESPMode::ThreadSafe> ISensorCustomRenderExtension::CreateBlendCameraExtension()
{
	return MakeShared<FBlendCamera_ViewExtension, ESPMode::ThreadSafe>();
}
