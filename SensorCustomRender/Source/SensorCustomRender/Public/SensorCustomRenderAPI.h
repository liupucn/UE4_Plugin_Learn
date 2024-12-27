#pragma once
#include "SceneViewExtension.h"

enum class EBlendCamera_SourceMode : uint8
{
	LIDAR_AND_Intensity,
	ANGLE_DEPTH_Intensity,
	DEPTH_AND_Intensity,
	MAX
};

class SENSORCUSTOMRENDER_API ISensorCustomRenderExtension : public ISceneViewExtension
{
public:
	ISensorCustomRenderExtension();
	virtual ~ISensorCustomRenderExtension();

	static TSharedPtr<ISensorCustomRenderExtension, ESPMode::ThreadSafe> CreateBlendCameraExtension();

	enum ECoordSystemType
	{
		WORLD_XYZ = 0,
		LOCAL_XYZ,
	};
	
	void SetCoordSystemType(ECoordSystemType CoordType);
	void SetCameraSourceMode(EBlendCamera_SourceMode InCameraSourceMode);
	void SetFOVAngle(const float InHorizonAngle);
	void SetFOVAngle2D(const FVector2D InFOVAngle);
	void SetLightEffectIntensity(bool bEffectIntensity);
protected:
	EBlendCamera_SourceMode CameraSourceMode;
	ECoordSystemType CoordSystemType;
	FVector2D CameraFOV_Radian;
	bool bLightEffectIntensity;
};