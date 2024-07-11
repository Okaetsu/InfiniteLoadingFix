#pragma once

#include <Function/Function.hpp>
#include <UObject.hpp>
#include <SDK/Classes/PalIndividualCharacterParameter.h>
#include <SDK/Structs/Pal_structs.h>

namespace Palworld {
    class UPalIndividualCharacterParameter;

    class UPalCharacterManagerReplicator : public RC::Unreal::UObject {
    public:
        typedef void(__stdcall* TYPE_AddRepCharacterParameter)(UPalCharacterManagerReplicator*, Palworld::FGuid*, Palworld::FPalInstanceID*,
            UPalIndividualCharacterParameter*);
        static inline TYPE_AddRepCharacterParameter AddRepCharacterParameter;
    };
}
