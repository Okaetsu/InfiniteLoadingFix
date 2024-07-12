#include <Mod/CppUserModBase.hpp>
#include <safetyhook.hpp>
#include <UE4SSProgram.hpp>
#include <Signatures.hpp>
#include <SigScanner/SinglePassSigScanner.hpp>
#include <UObject.hpp>
#include <UObjectGlobals.hpp>
#include <UFunction.hpp>
#include <SDK/Classes/PalPlayerState.h>
#include <SDK/Classes/PalCharacterManagerReplicator.h>
#include <SDK/Classes/PalIndividualCharacterParameter.h>
#include <SDK/Structs/Pal_structs.h>

using namespace RC;
using namespace RC::Unreal;
using namespace Palworld;

std::vector<RC::SignatureContainer> SigContainer;
RC::SinglePassScanner::SignatureContainerMap SigContainerMap;

UPalCharacterManagerReplicator* PalCharacterManagerReplicator = nullptr;

struct CharacterRepData {
    Palworld::FGuid RequestGUID;
    FPalInstanceID ID;
    UPalIndividualCharacterParameter* Parameter;
};

// Keep track of players that have logged in during this session
std::unordered_set<Palworld::FGuid> Players;

// Keep track of Palbox Pals
std::unordered_set<Palworld::FGuid> PalboxPals;

// Store pal data that should be replicated later
std::unordered_map<Palworld::FGuid, std::vector<CharacterRepData>> LateReplicationMap;

// Store player data that should be replicated later
std::unordered_map<Palworld::FGuid, CharacterRepData> LatePlayerReplicationMap;

// Store character data that has already been replicated, TODO: Actually use this to remove stuff from the Array when it's not relevant?
std::unordered_set<Palworld::FGuid> ReplicatedMap;

// Set to true when the first Player connects.
bool HasInitialized = false;

// Signature stuff, expect them to break with updates
void BeginScan()
{
    SignatureContainer PlayerState_Signature = [=]() -> SignatureContainer {
        return {
            {{ "48 89 5C 24 10 48 89 6C 24 18 48 89 74 24 20 57 41 56 41 57 48 81 EC 80 00 00 00 48 8B F9 48 8D"}},
            [=](SignatureContainer& self) {
                void* FunctionPointer = static_cast<void*>(self.get_match_address());

                APalPlayerState::ProcessJoinedPlayer_InServer = reinterpret_cast<APalPlayerState::TYPE_ProcessJoinedPlayer_InServer>(FunctionPointer);

                self.get_did_succeed() = true;

                return true;
            },
            [](const SignatureContainer& self) {
                if (!self.get_did_succeed())
                {
                    Output::send<LogLevel::Error>(STR("Failed to find signature for APalPlayerState::ProcessJoinedPlayer_InServer\n"));
                }
            }
        };
    }();

    SignatureContainer PalCharacterManagerReplicator_Signature = [=]() -> SignatureContainer {
        return {
            {{ "48 89 5C 24 10 48 89 6C 24 18 48 89 74 24 20 48 89 4C 24 08 57 41 54 41 55 41 56 41 57 48 83 EC 20 48 8D B1 30 01 00 00"}},
            [=](SignatureContainer& self) {
                void* FunctionPointer = static_cast<void*>(self.get_match_address());

                UPalCharacterManagerReplicator::AddRepCharacterParameter =
                    reinterpret_cast<UPalCharacterManagerReplicator::TYPE_AddRepCharacterParameter>(FunctionPointer);

                self.get_did_succeed() = true;

                return true;
            },
            [](const SignatureContainer& self) {
                if (!self.get_did_succeed())
                {
                    Output::send<LogLevel::Error>(STR("Failed to find signature for UPalCharacterManagerReplicator::AddRepCharacterParameter\n"));
                }
            }
        };
    }();

    SigContainer.emplace_back(PlayerState_Signature);
    SigContainer.emplace_back(PalCharacterManagerReplicator_Signature);
    SigContainerMap.emplace(ScanTarget::MainExe, SigContainer);
    SinglePassScanner::start_scan(SigContainerMap);
}

SafetyHookInline AddRepCharacterParameter_Hook{};
void __stdcall AddRepCharacterParameter(UPalCharacterManagerReplicator* This, Palworld::FGuid* RequestGUID, FPalInstanceID* ID,
    UPalIndividualCharacterParameter* Parameter) {
    auto SaveParameter = Parameter->GetSaveParameter();

    if (!PalCharacterManagerReplicator)
    {
        // Store the actual PalCharacterManagerReplicator pointer for later use. We want to make sure it's the right object we feed into AddRepCharacterParameter
        PalCharacterManagerReplicator = This;
    }

    if (SaveParameter.IsPlayer)
    {
        // If it's a player, replicate it later when the player connects to the server
        auto RepData = CharacterRepData{};
        RepData.RequestGUID = *RequestGUID;
        RepData.ID = *ID;
        RepData.Parameter = Parameter;

        auto UId = Parameter->GetIndividualId().PlayerUId;
        LatePlayerReplicationMap[UId] = RepData;
    }
    else
    {
        if ((SaveParameter.OwnerPlayerUId.A | SaveParameter.OwnerPlayerUId.B | SaveParameter.OwnerPlayerUId.C | SaveParameter.OwnerPlayerUId.D) == 0)
        {
            // If it's a wild NPC, replicate it immediately
            AddRepCharacterParameter_Hook.call(This, RequestGUID, ID, Parameter);
        }
        else
        {
            auto UId = SaveParameter.OwnerPlayerUId;

            if (HasInitialized)
            {
                // Here we assume the Pal was collected from the ground.
                AddRepCharacterParameter_Hook.call(This, RequestGUID, ID, Parameter);
            }
            else
            {
                // If it's a player owned NPC, replicate it once the owning player actually connects to the server
                auto RepData = CharacterRepData{};
                RepData.RequestGUID = *RequestGUID;
                RepData.ID = *ID;
                RepData.Parameter = Parameter;

                LateReplicationMap[UId].push_back(RepData);
                PalboxPals.insert(ID->InstanceId);
            }
        }
    }
}

SafetyHookInline JoinedPlayer_Hook{};
void __stdcall ProcessJoinedPlayer_InServer(APalPlayerState* PlayerState)
{
    auto PlayerUId = PlayerState->GetLoginTryingPlayerUId_InServer();

    HasInitialized = true;

    if (!Players.count(PlayerUId))
    {
        Players.insert(PlayerUId);

        auto PlayerDataIterator = LatePlayerReplicationMap.find(PlayerUId);
        if (PlayerDataIterator != LatePlayerReplicationMap.end())
        {
            // Intentionally making a copy here so it doesn't explode when passed into the params
            auto PlayerData = PlayerDataIterator->second;
            AddRepCharacterParameter_Hook.call(PalCharacterManagerReplicator, &PlayerData.RequestGUID, &PlayerData.ID, PlayerData.Parameter);
        }

        LatePlayerReplicationMap.erase(PlayerUId);

        // Process the list of things to replicate related to this player which is the player itself and their owned Pals
        for (auto& ReplData : LateReplicationMap[PlayerUId])
        {
            AddRepCharacterParameter_Hook.call(PalCharacterManagerReplicator, &ReplData.RequestGUID, &ReplData.ID, ReplData.Parameter);
        }

        // Finally remove the data we just processed so we don't replicate them again and create duplicates
        LateReplicationMap.erase(PlayerUId);
    }

    // Jump back to the original function
    JoinedPlayer_Hook.call(PlayerState);
}

class InfiniteLoadingFix : public RC::CppUserModBase
{
public:
    InfiniteLoadingFix() : CppUserModBase()
    {
        ModName = STR("InfiniteLoadingFix");
        ModVersion = STR("0.2.0");
        ModDescription = STR("Fixes the Infinite Loading Screen that happens when the server reaches roughly 40k+ total Pals");
        ModAuthors = STR("Okaetsu");
    }

    ~InfiniteLoadingFix() override
    {
    }

    auto on_unreal_init() -> void override
    {
        wprintf(STR("%s v%s by %s\n"), ModName.c_str(), ModVersion.c_str(), ModAuthors.c_str());

        BeginScan();

        AddRepCharacterParameter_Hook = safetyhook::create_inline(reinterpret_cast<void*>(UPalCharacterManagerReplicator::AddRepCharacterParameter),
            reinterpret_cast<void*>(AddRepCharacterParameter));

        JoinedPlayer_Hook = safetyhook::create_inline(reinterpret_cast<void*>(APalPlayerState::ProcessJoinedPlayer_InServer),
            reinterpret_cast<void*>(ProcessJoinedPlayer_InServer));
    }
};

#define INFINITELOADINGFIX_API __declspec(dllexport)
extern "C"
{
    INFINITELOADINGFIX_API RC::CppUserModBase* start_mod()
    {
        return new InfiniteLoadingFix();
    }

    INFINITELOADINGFIX_API void uninstall_mod(RC::CppUserModBase* mod)
    {
        delete mod;
    }
}
