#include "SDK/Classes/PalIndividualCharacterParameter.h"

namespace Palworld {
    FPalInstanceID UPalIndividualCharacterParameter::GetIndividualId()
    {
        auto IndividualId = *this->GetValuePtrByPropertyNameInChain<FPalInstanceID>(STR("IndividualId"));
        return IndividualId;
    }

    FPalIndividualCharacterSaveParameter UPalIndividualCharacterParameter::GetSaveParameter()
    {
        auto SaveParameter = *this->GetValuePtrByPropertyNameInChain<FPalIndividualCharacterSaveParameter>(STR("SaveParameter"));
        return SaveParameter;
    }
}
