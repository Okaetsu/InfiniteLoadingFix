# Infinite Loading Screen Fix

Hello! This is a workaround fix for the Infinite Loading Screen bug that's present on many bigger community servers. There may be some side effects, however from my testing there didn't seem to be any.

It manually manages when to replicate things rather than replicating everything at once. There's definitely a lot of improvements that can be made, but it seems to be doing the job really well currently.

This doesn't work on Linux since UE4SS doesn't support Linux yet!

Make sure to backup your world before applying the mod. It should only causes visual glitches for clients at worst, but just in case.

## Installation

1. Download the latest experimental version of UE4SS [here](https://github.com/UE4SS-RE/RE-UE4SS/releases/download/experimental/UE4SS_v3.0.1-137-g6867a08.zip) or go to the experimental releases and find the .zip with `UE4SS_v3.0.1-137-g6867a08` in the name.

2. Extract UE4SS into the Win64 folder of your server

3. Download the [latest release of the mod](https://github.com/Okaetsu/InfiniteLoadingFix/releases/tag/v0.2.0)

4. Extract the mod folder that's inside the zip into Pal\Binaries\Win64\ue4ss\Mods folder of your server

5. Done! This is purely a server-side mod, so clients won't need it.
