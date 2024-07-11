#pragma once

#include <cstdint>
#include <FString.hpp>
#include <NameTypes.hpp>
#include <UClass.hpp>
#include <UnrealCoreStructs.hpp>
#include <SDK/Structs/Guid.h>
#include <SDK/Enums/Pal_enums.h>

namespace Palworld {
    struct FDateTime {
        int64_t Ticks;
    };

    struct FFixedPoint64 {
        int64_t Value;
    };

    struct FPalInstanceID {
        Palworld::FGuid PlayerUId;
        Palworld::FGuid InstanceId;
        RC::Unreal::FString DebugName;

        bool operator==(const FPalInstanceID& Other) const
        {
            if ((((Other.PlayerUId).D ^ (this->PlayerUId).D |
                (Other.PlayerUId).C ^ (this->PlayerUId).C |
                (Other.PlayerUId).B ^ (this->PlayerUId).B |
                (Other.PlayerUId).A ^ (this->PlayerUId).A) == 0) &&
                (((Other.InstanceId).D ^ (this->InstanceId).D |
                    (Other.InstanceId).C ^ (this->InstanceId).C |
                    (Other.InstanceId).B ^ (this->InstanceId).B |
                    (Other.InstanceId).A ^ (this->InstanceId).A) == 0)) {
                return true;
            }
            return false;
        }
    };

    struct FPalWorkSuitabilityInfo {
        EPalWorkSuitability WorkSuitability;
        int Rank;
    };

    struct FPalContainerId {
        Palworld::FGuid ID;
    };

    struct FPalCharacterSlotId {
        FPalContainerId ContainerId;
        int SlotIndex;
    };

    struct FPalGotStatusPoint {
        RC::Unreal::FName StatusName;
        int StatusPoint;
    };

    struct FloatContainer_FloatPair {
        RC::Unreal::FName Key;
        float Value;
    };

    struct FFloatContainer {
        RC::Unreal::TArray<FloatContainer_FloatPair> Values;
    };

    struct FPalFoodRegeneEffectParameter {
        EPalFoodStatusEffectType EffectType;
        int Value;
        int Interval;
    };

    struct FPalFoodRegeneInfo {
        RC::Unreal::FName ItemId;
        int EffectTime;
        int RemainingTime;
        RC::Unreal::TArray<FPalFoodRegeneEffectParameter> RegeneEfectParameters;
    };

    struct FPalWorkSuitabilityPreferenceInfo {
        RC::Unreal::TArray<EPalWorkSuitability> OffWorkSuitabilityList;
        bool bAllowBaseCampBattle;
    };

    struct FPalArenaCharacterRestoreParameter {
        bool bValid;
        FFixedPoint64 HP;
        float FullStomach;
        float SanityValue;
        EPalBaseCampWorkerSickType WorkerSick;
    };

    struct FPalIndividualCharacterCacheParameter {
        Palworld::FGuid GroupId;
        bool bSleeping;
        Palworld::FGuid InvaderTargetBaseCampId;
        EPalInvaderType InvaderType;
        RC::Unreal::FString FilteredNickName;
        bool bIsInArena;
    };

    struct FPalIndividualCharacterSaveParameter {
        RC::Unreal::FName CharacterID;
        RC::Unreal::FName UniqueNPCID;
        EPalGenderType Gender;
        RC::Unreal::UClass* CharacterClass;
        int Level;
        int Rank;
        int RankUpExp;
        int Rank_HP;
        int Rank_Attack;
        int Rank_Defence;
        int Rank_CraftSpeed;
        int Exp;
        RC::Unreal::FString NickName;
        bool IsRarePal;
        RC::Unreal::TArray<EPalWazaID> EquipWaza;
        RC::Unreal::TArray<EPalWazaID> MasteredWaza;
        FFixedPoint64 HP;
        int Talent_HP;
        int Talent_Melee;
        int Talent_Shot;
        int Talent_Defense;
        float FullStomach;
        EPalStatusPhysicalHealthType PhysicalHealth;
        EPalBaseCampWorkerSickType WorkerSick;
        RC::Unreal::TArray<RC::Unreal::FName> PassiveSkillList;
        int DyingTimer;
        FFixedPoint64 MP;
        bool IsPlayer;
        FDateTime OwnedTime;
        Palworld::FGuid OwnerPlayerUId;
        RC::Unreal::TArray<Palworld::FGuid> OldOwnerPlayerUIds;
        FFixedPoint64 MaxHP;
        int Support;
        int CraftSpeed;
        RC::Unreal::TArray<FPalWorkSuitabilityInfo> CraftSpeeds;
        FFixedPoint64 ShieldHP;
        FFixedPoint64 ShieldMaxHP;
        FFixedPoint64 MaxMP;
        FFixedPoint64 MaxSP;
        EPalStatusHungerType HungerType;
        float SanityValue;
        EPalBaseCampWorkerEventType BaseCampWorkerEventType;
        float BaseCampWorkerEventProgressTime;
        FPalContainerId ItemContainerId;
        FPalContainerId EquipItemContainerId;
        FPalCharacterSlotId SlotID;
        float MaxFullStomach;
        float FullStomachDecreaseRate_Tribe;
        int UnusedStatusPoint;
        RC::Unreal::TArray<FPalGotStatusPoint> GotStatusPointList;
        RC::Unreal::TArray<FPalGotStatusPoint> GotExStatusPointList;
        FFloatContainer DecreaseFullStomachRates;
        FFloatContainer AffectSanityRates;
        FFloatContainer CraftSpeedRates;
        RC::Unreal::FVector LastJumpedLocation;
        RC::Unreal::FName FoodWithStatusEffect;
        int Timer_FoodWithStatusEffect;
        FPalFoodRegeneInfo FoodRegeneEffectInfo;
        EPalWorkSuitability CurrentWorkSuitability;
        FPalWorkSuitabilityPreferenceInfo WorkSuitabilityOptionInfo;
        bool bAppliedDeathPenalty;
        float PalReviveTimer;
        int VoiceID;
        Palworld::FGuid SkinAppliedCharacterId;
        RC::Unreal::FName SkinName;
        bool bSkinApplied;
        bool IsFavoritePal;
        FPalArenaCharacterRestoreParameter ArenaRestoreParameter;
        FPalIndividualCharacterCacheParameter Dynamic;
    };
}