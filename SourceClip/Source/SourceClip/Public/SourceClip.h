// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

class FName;
class UToolMenus;


class FSourceClip
{
public:
	FSourceClip(void);
	virtual ~FSourceClip(void);

public:
	void AddDelegate();

private:
	void Clipping(void);

	/*-> Main Menu*/
	void ClipMainMenu(void);

	/*-> File SubMenu*/
	void ClipFileMenu(void);

	/*-> Window SubMenu*/
	void ClipWindowMenu(void);

	/*-> Developer Tools*/
	void ClipDeveloperTools(void);

	/*-> ToolBar Button*/
	void ClipToolBar(void);

	/*-> Editor Mode*/
	void ClipEditorMode(void);

	/*-> Context Menu*/
	void ClipContextMenu(void);

private:
	/*-> ToolMenus*/
	UToolMenus* ToolMenus;
	
	/*-> LevelEditorModule*/
	TSharedPtr<FTabManager> LevelEditorTabManager;

};

/** Global engine pointer. Can be 0 so don't use without checking. */
extern ENGINE_API class UEngine* GEngine;

