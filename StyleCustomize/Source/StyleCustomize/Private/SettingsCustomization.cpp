// Copyright (C) 2018 JackMyth. All Rights Reserved.

#include "SettingsCustomization.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "Widgets/Input/SButton.h"
#include "Modules/ModuleManager.h"
#include "DetailWidgetRow.h"
#include "Misc/MessageDialog.h"
#include <Widgets/Text/STextBlock.h>
#include <Widgets/Input/SEditableText.h>
#include <SNameComboBox.h>
#include "Interfaces\IPluginManager.h"
#include "HAL\FileManager.h"
#include <../Private/PropertyNode.h>

#define LOCTEXT_NAMESPACE "FUE4EditorCustomizeModule"

UE4ECSettingsCustomization::UE4ECSettingsCustomization()
{
	//初始化下拉框中的选项
	UpdateOptions();
	PreItem = MakeShareable(new FString("Not Selected"));
}	

UE4ECSettingsCustomization::~UE4ECSettingsCustomization()
{
}

void UE4ECSettingsCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailLyoutBuilder)
{
	IDetailCategoryBuilder& ResetToDefaultCategory= DetailLyoutBuilder.EditCategory(TEXT("StyleCustomize"));
	ResetToDefaultCategory.AddCustomRow(FText::FromString("StyleCustomize"))
		.WholeRowWidget
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			[
			SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				[
				SNew(SButton)
				.Text(LOCTEXT("ImportTheme", "Import Theme"))
				.HAlign(EHorizontalAlignment::HAlign_Center)
				.ToolTipText(LOCTEXT("ImportThemeButtonTip", "Import Theme From a file.\nBoth *.UTheme and *.Ini are Supported."))
				.OnClicked_Lambda([]()
							  {
								  FModuleManager::LoadModuleChecked<FStyleCustomizeModule>("StyleCustomize").ShowDialogForImport();
								  return FReply::Handled();
							  })
				]
				+ SHorizontalBox::Slot()
				[
				SNew(SButton)
				.Text(LOCTEXT("PackageTheme", "Package Current Theme"))
				.HAlign(EHorizontalAlignment::HAlign_Center)
				.ToolTipText(LOCTEXT("PackageThemeButtonTip","Package Current Theme as a *.UTheme file.\n"
									 "A UTheme file include all Contents referenced by the ConfigFile in UE4EditorCustomize content floder. You can share it to other people.\n"
									"You can also export only a ini file. But it will not contain any Content."))
				.OnClicked_Lambda([]()
							  {
								  FModuleManager::LoadModuleChecked<FStyleCustomizeModule>("StyleCustomize").ShowDialogForPackageTheme();
								  return FReply::Handled();
							  })
				]
			]
			+ SVerticalBox::Slot()
			[
				SNew(SButton)
				.Text(LOCTEXT("ImportBuiltInTheme", "Import Built-In Theme"))
				.HAlign(EHorizontalAlignment::HAlign_Center)
				.OnClicked_Lambda([]()
								  {
									  FModuleManager::LoadModuleChecked<FStyleCustomizeModule>("StyleCustomize").ImportBuiltInTheme();
									  return FReply::Handled();
								  })
			]
			+ SVerticalBox::Slot()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot().HAlign(HAlign_Center).VAlign(VAlign_Center).Padding(FMargin(50, 0, 0, 0)).FillWidth(1)
				[
					SNew(STextBlock).Text(NSLOCTEXT("StyleCustomize", "已存在的风格", "已存在的风格"))
				]
				+ SHorizontalBox::Slot().HAlign(HAlign_Fill)//.VAlign(VAlign_Fill).Padding(FMargin(50, 0, 50, 0)).FillWidth(2)
				[
					SNew(SComboBox<FComboItemType>)
					.OptionsSource(&Options)
					.OnGenerateWidget(this, &UE4ECSettingsCustomization::MakeWidgetForOption)
					.OnSelectionChanged(this, &UE4ECSettingsCustomization::OnSelectionChanged).InitiallySelectedItem(CurrentItem)
					.OnComboBoxOpening(this, &UE4ECSettingsCustomization::UpdateOptions)
					[
						SNew(STextBlock).Text(this, &UE4ECSettingsCustomization::GetCurrentItemLabel)
						//SNew(STextBlock).Text(CurrentItem)
					]
				]
			]
		];
}

TSharedRef<SWidget> UE4ECSettingsCustomization::MakeWidgetForOption(FComboItemType InOption)
{
	return SNew(STextBlock).Text(FText::FromString(*InOption));
}

void UE4ECSettingsCustomization::OnSelectionChanged(FComboItemType NewValue, ESelectInfo::Type)
{
	CurrentItem = NewValue;
	FString ConfigFilePath;
	if (CurrentItem)
	{
		ConfigFilePath = IPluginManager::Get().FindPlugin(TEXT("StyleCustomize"))->GetBaseDir() + FString("/Content/Content/") + *CurrentItem.Get() + ".ini";

	}
	if (!ConfigFilePath.IsEmpty())
	{
		FModuleManager::LoadModuleChecked<FStyleCustomizeModule>("StyleCustomize").ImportSettingFromIni(ConfigFilePath);
	}

}

FText UE4ECSettingsCustomization::GetCurrentItemLabel() const
{
	if (CurrentItem.IsValid() && *CurrentItem != *PreItem)
	{
		*PreItem = *CurrentItem;
		return FText::FromString(*CurrentItem);
	}

	return FText::FromString(*PreItem);
}

void UE4ECSettingsCustomization::UpdateOptions()
{
	Options.Empty();
	FString ConfigsFilePath = IPluginManager::Get().FindPlugin(TEXT("StyleCustomize"))->GetBaseDir() + FString("/Content/Content/");
	const TCHAR* FileExtension = TEXT(".ini");
	TArray<FString> AssetName;
	if (IFileManager::Get().DirectoryExists(*ConfigsFilePath))
	{
		IFileManager::Get().FindFiles(AssetName, *ConfigsFilePath, FileExtension);
	}
	for (auto Item:AssetName)
	{
		FString OptionItem = FPaths::GetBaseFilename(Item);
		Options.Add(MakeShareable( new FString(OptionItem)) );
	}
}

/*void UE4ECCustomStyleEditorCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	auto& UE4ECModule = FModuleManager::LoadModuleChecked<FStyleCustomizeModule>("StyleCustomize");
	auto& CustomStyleCategory = DetailBuilder.EditCategory(TEXT("CustomStyle"));
	CustomStyleCategory.AddCustomRow(FText::FromString("+"))[
		SNew(SButton)
			.Text(LOCTEXT("AddCustomStyle", "Add Custom Style"))
			.OnClicked_Lambda([&CustomStyleCategory]()
				{
					TSharedPtr<SEditableText> StyleNameWidget;
					TSharedPtr<SNameComboBox> StyleTypeWidget;
					const char* StyleTypeCollection[] =
					{
						"FSlateBrush"
					};
					TArray<TSharedPtr<FName>> StyleTypeSource;
					for (const char* mStyleType: StyleTypeCollection)
					{
						StyleTypeSource.Add(MakeShareable(new FName(mStyleType)));
					}

					//Open a dialog
					SNew(SWindow)
						.Title(LOCTEXT("AddCustomStyle", "Add Custom Style"))
						.Content()[
							SNew(SVerticalBox)
								+ SVerticalBox::Slot()[
									SNew(STextBlock).Text(LOCTEXT("StyleName:", "Style Name:"))
								]
								+ SVerticalBox::Slot()[
									SAssignNew(StyleNameWidget, SEditableText)
								]
								+ SVerticalBox::Slot()[
									SNew(STextBlock).Text(LOCTEXT("StyleType:", "Style Type:"))
								]
								+ SVerticalBox::Slot()[
									SAssignNew(StyleTypeWidget,SNameComboBox)
									.OptionsSource(&StyleTypeSource)
									.InitiallySelectedItem(StyleTypeSource[0])
								]
								+ SVerticalBox::Slot()[
									SNew(SButton)
										.Text(LOCTEXT("OK","OK"))
										.OnClicked_Lambda([]()
										{

										})
								].HAlign(HAlign_Right)
						]
				})
	];

}*/

#undef LOCTEXT_NAMESPACE
