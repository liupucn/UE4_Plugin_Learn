// Copyright (C) 2018 JackMyth. All Rights Reserved.

#pragma once

#include "StyleCustomizeModule.h"
#include <IDetailCustomization.h>

/**
 * 
 */

typedef TSharedPtr<FString>  FComboItemType; //重定义TSharedPtr<FString> 名字为 FComboItemType

class UE4ECSettingsCustomization :public IDetailCustomization
{
public:
	UE4ECSettingsCustomization();
	~UE4ECSettingsCustomization();
	
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailLyoutBuilder) override;

	//SComboBox的绑定函数
	TSharedRef<SWidget> MakeWidgetForOption(FComboItemType InOption);
	void OnSelectionChanged(FComboItemType NewValue, ESelectInfo::Type);
	void OnComboBoxOpening();
	FText GetCurrentItemLabel() const;
	void UpdateOptions();

private:
	TSharedPtr<SComboBox<FComboItemType>> ComBox;

	//下拉框的参数
	FComboItemType PreItem;
	FComboItemType CurrentItem;
	TArray<FComboItemType> Options;
	//TArray<FString>Options;
};

/*class UE4ECCustomStyleEditorCustomization :public IDetailCustomization
{
	// Inherited via IDetailCustomization
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
};*/