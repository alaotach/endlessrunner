// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "HAL/CriticalSection.h"
#include "Containers/Ticker.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "UObject/ObjectSaveContext.h"

struct FAssetData;
class UBlueprint;
class UObject;
class UPackage;
class UWakatimeSettings;


class FWakatimeIntegrationModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
private:
	bool OnTimerTick(float DeltaTime);
	void OnAssetAdded(const FAssetData& AssetData);
	void OnAssetRemoved(const FAssetData& AssetData);
	void OnAssetRenamed(const FAssetData& AssetData, const FString& OldPath);
	void OnPackageSaved(const FString& PackageFileName, UPackage* Package, FObjectPostSaveContext ObjectSaveContext);
	void OnMapChanged(uint32 MapChangeFlags);
	void OnActorSelectionChanged(const TArray<UObject*>& NewSelection, bool bForceRefresh);
	void OnBeginPIE(bool bIsSimulating);
	void OnEndPIE(bool bIsSimulating);
	void OnBlueprintCompiled();
	void OnObjectPropertyChanged(UObject* Object, struct FPropertyChangedEvent& PropertyChangedEvent);
	void OnAssetOpened(UObject* Asset, class IAssetEditorInstance* Instance);
	void OnActorMoved(AActor* Actor);
	void OnActorSpawned(AActor* Actor);
	void OnActorDeleted(AActor* Actor);
	void OnEditorModeChanged(FEdMode* Mode, bool bEntering);
	void OnLightingBuildStarted();
	void OnApplicationActivated(bool bIsActive);
	void MarkActivity();
	void SendHeartbeat();
	int64 GetCurrentTime();
	void OnHttpResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	FCriticalSection DataLock;

	bool Dirty = false;
	int32 DeleteOperations = 0;
	int32 SaveOperations = 0;
	int32 RenameOperations = 0;
	int32 AddOperations = 0;
	int64 LastAssetPushTime = -1;
	int64 SaveDebounce = 2;
	int64 LastActivityTime = 0;
	bool bIsEditorActive = true;
	FName LastSavedName = FName(TEXT("None"));

	FTSTicker::FDelegateHandle TimerHandle;
};
