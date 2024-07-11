#pragma once

#include <AActor.hpp>
#include <SDK/Structs/Pal_structs.h>

namespace Palworld {
    class APalPlayerState : public RC::Unreal::AActor {
    public:
       Palworld::FGuid GetLoginTryingPlayerUId_InServer();
    public:
        typedef void(__thiscall* TYPE_ProcessJoinedPlayer_InServer)(APalPlayerState*);
        static inline TYPE_ProcessJoinedPlayer_InServer ProcessJoinedPlayer_InServer;
    };
}
