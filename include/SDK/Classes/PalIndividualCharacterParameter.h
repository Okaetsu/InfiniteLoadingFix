#pragma once

#include <UObject.hpp>
#include <SDK/Structs/Pal_structs.h>

namespace Palworld {
    class UPalIndividualCharacterParameter : public RC::Unreal::UObject {
    public:
        FPalInstanceID GetIndividualId();

        FPalIndividualCharacterSaveParameter GetSaveParameter();
    };
}
