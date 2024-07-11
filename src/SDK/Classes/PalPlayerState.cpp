#include "SDK/Classes/PalPlayerState.h"

namespace Palworld {
	Palworld::FGuid APalPlayerState::GetLoginTryingPlayerUId_InServer()
	{
		auto PlayerUId = *this->GetValuePtrByPropertyNameInChain<FGuid>(STR("LoginTryingPlayerUId_InServer"));
		return PlayerUId;
	}
}