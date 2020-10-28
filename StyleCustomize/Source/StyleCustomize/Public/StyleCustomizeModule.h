// Copyright 2018 Jack Myth. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ISettingsModule.h"
#include "Modules/ModuleManager.h"

struct FUThemeInfo_v0
{
	FString Name;
	float Size;
	int Version;
	int RequestEngineVersion;
	FString Author;
	FString Intro;
	FString IconImagePath;
	TArray<FString> tmpImagePath;
};

class FStyleCustomizeModule : public IModuleInterface
{
	enum class AssetType : uint8
	{
		ConfigIni,
		Texture2D,
		Font,
		FontFace,		//Reserved

		UAsset = 255
	};

	TSharedPtr<ISettingsSection> SettingS;
	TMap<FName, const FSlateBrush*> CachedOriginalBrushes;
	TArray<UObject*> FindReferencedAsset();
	FString GetAssetAbsolutePath(FString PackageName);
	bool _Internal_ImportUTheme_v0(TArray<uint8>& FileData, FText* ErrorMsg=nullptr);
	bool _Internal_ExportUAsset(UObject* UAssetObj, TArray<uint8>& UThemeData);
	bool _Internal_ExportConfigIni(TArray<uint8>& UThemeData);
	bool _Internal_ExportTexture2D(UObject* TextureObj, TArray<uint8>& UThemeData);
	bool _Internal_ExportFont(UObject* FontObj, TArray<uint8>& UThemeData, TArray<UObject*>& NeededObject);
	bool _Internal_ExportFontFace(const UObject* FontFaceObj, TArray<uint8>& UThemeData);
	bool _Internal_ImportUAsset(TArray<uint8>& UThemeData, int& Offset, FText* ErrorMsg=nullptr);
	bool _Internal_ImportConfigIni(TArray<uint8>& UThemeData, int& Offset);
	bool _Internal_ImportTexture2D(TArray<uint8>& UThemeData, int& Offset);
	bool _Internal_ImportFont(TArray<uint8>& UThemeData, int& Offset);
	class UFontFace* _Internal_ImportFontFace(TArray<uint8>& UThemeData, int& Offset);
	bool _Internal_LoadUThemeInfo(TArray<uint8>& UThemeData,int& Offset, int RequestVersion, int UThemeVersion, float Size);
	bool _Internal_AskUThemeInfo(FUThemeInfo_v0* OutInfo);
	uint8 ExtensionToUint8(FString FileExtension);
	FString Uint8ToExtension(uint8 Extension);
	void CacheOriginalBrushes();
	void ReCacheCustomBrushes();
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	bool OnSettingModified();
	bool ImportSettingFromIni(FString FilePath);
	bool ImportUTheme(FString FilePath, FText* ErrorMsg = nullptr);
	void ShowDialogForImport();
	void ImportBuiltInTheme();
	bool PackageTheme(FString FilePat, FUThemeInfo_v0 UThemeInfo);
	void ShowDialogForPackageTheme();
	void RestoreCachedBrush();

	void SaveConfig(FString ConfigFilePath);
	bool AutoSaveDirtyPackages();

public:
	void ApplyEditorStyle(class  UEditorCustomizeSetting* StyleSettings);
	void ApplyCoreStyle(class UEditorCustomizeSetting* StyleSettings);
	/*-> Apply icon style*/
	void ApplyEditorToolbarIcon(class UEditorCustomizeSetting* StyleSettings);
	void ApplyEditorModesToolbarIcon(class UEditorCustomizeSetting* StyleSettings);
	void ApplyEidtorMenuIcon(class UEditorCustomizeSetting* StyleSettings);
	void ApplyEditorContentBrowerIcon(class UEditorCustomizeSetting* StyleSettings);

private:
	void SetModuleStyleSlateBrush(FName ModuleStyleName, FName SlateBrushName, FSlateBrush* InSlateBrush);

public:
	void OnPreExit();

public:
	FSlateBoxBrush SlateBrushToBoxBrush(FSlateBrush InSlateBrush);
};

