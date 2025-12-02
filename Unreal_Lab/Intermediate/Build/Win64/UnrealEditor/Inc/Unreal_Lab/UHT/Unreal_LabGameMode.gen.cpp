// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Unreal_Lab/Unreal_LabGameMode.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeUnreal_LabGameMode() {}

// Begin Cross Module References
ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
UNREAL_LAB_API UClass* Z_Construct_UClass_AUnreal_LabGameMode();
UNREAL_LAB_API UClass* Z_Construct_UClass_AUnreal_LabGameMode_NoRegister();
UPackage* Z_Construct_UPackage__Script_Unreal_Lab();
// End Cross Module References

// Begin Class AUnreal_LabGameMode
void AUnreal_LabGameMode::StaticRegisterNativesAUnreal_LabGameMode()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(AUnreal_LabGameMode);
UClass* Z_Construct_UClass_AUnreal_LabGameMode_NoRegister()
{
	return AUnreal_LabGameMode::StaticClass();
}
struct Z_Construct_UClass_AUnreal_LabGameMode_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering HLOD WorldPartition DataLayers Transformation" },
		{ "IncludePath", "Unreal_LabGameMode.h" },
		{ "ModuleRelativePath", "Unreal_LabGameMode.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AUnreal_LabGameMode>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_AUnreal_LabGameMode_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_AGameModeBase,
	(UObject* (*)())Z_Construct_UPackage__Script_Unreal_Lab,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AUnreal_LabGameMode_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_AUnreal_LabGameMode_Statics::ClassParams = {
	&AUnreal_LabGameMode::StaticClass,
	"Game",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	0,
	0,
	0x008802ACu,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AUnreal_LabGameMode_Statics::Class_MetaDataParams), Z_Construct_UClass_AUnreal_LabGameMode_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_AUnreal_LabGameMode()
{
	if (!Z_Registration_Info_UClass_AUnreal_LabGameMode.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AUnreal_LabGameMode.OuterSingleton, Z_Construct_UClass_AUnreal_LabGameMode_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_AUnreal_LabGameMode.OuterSingleton;
}
template<> UNREAL_LAB_API UClass* StaticClass<AUnreal_LabGameMode>()
{
	return AUnreal_LabGameMode::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(AUnreal_LabGameMode);
AUnreal_LabGameMode::~AUnreal_LabGameMode() {}
// End Class AUnreal_LabGameMode

// Begin Registration
struct Z_CompiledInDeferFile_FID_Unreal_Lab_Source_Unreal_Lab_Unreal_LabGameMode_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_AUnreal_LabGameMode, AUnreal_LabGameMode::StaticClass, TEXT("AUnreal_LabGameMode"), &Z_Registration_Info_UClass_AUnreal_LabGameMode, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AUnreal_LabGameMode), 1175144594U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Unreal_Lab_Source_Unreal_Lab_Unreal_LabGameMode_h_933779945(TEXT("/Script/Unreal_Lab"),
	Z_CompiledInDeferFile_FID_Unreal_Lab_Source_Unreal_Lab_Unreal_LabGameMode_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Unreal_Lab_Source_Unreal_Lab_Unreal_LabGameMode_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
