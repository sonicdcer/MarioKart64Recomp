# MarioKart 64: Recompiled
MarioKart 64: Recompiled is a project that uses [N64: Recompiled](https://github.com/Mr-Wiseguy/N64Recomp) to **statically recompile** MarioKart 64 into a native port with many new features and enhancements. This project uses [RT64](https://github.com/rt64/rt64) as the rendering engine to provide some of these enhancements.

### [Check out the latest release here](https://github.com/sonicdcer/MarioKart64Recomp/releases/latest).

Join the [N64: Recompiled Community Discord](https://discord.gg/AWZThJ4dPf) to discuss this and other N64: Recompiled projects!

[![Discord Invitation](https://discordapp.com/api/guilds/1374083583739826328/widget.png?style=banner2 'N64 Recomp')](https://discord.gg/AWZThJ4dPf)

### **This repository and its releases do not contain game assets. The original game is required to build or run this project.**

## Table of Contents
* [System Requirements](#system-requirements)
* [Features](#features)
  * [Plug and Play](#plug-and-play)
  * [Fully Intact N64 Effects](#fully-intact-n64-effects)
  * [Easy-to-Use Menus](#easy-to-use-menus)
  * [High Framerate Support](#high-framerate-support)
  * [Widescreen and Ultrawide Support](#widescreen-and-ultrawide-support)
  * [Low Input Lag](#low-input-lag)
  * [Linux and Steam Deck Support](#linux-and-steam-deck-support)
* [Planned Features](#planned-features)
* [FAQ](#faq)
* [Known Issues](#known-issues)
* [Building](#building)
* [Libraries Used and Projects Referenced](#libraries-used-and-projects-referenced)

## System Requirements
A GPU supporting Direct3D 12.0 (Shader Model 6), Vulkan 1.2, or Metal Argument Buffers Tier 2 support is required to run this project. The oldest GPUs that should be supported for each vendor are:
* GeForce GT 630
* Radeon HD 7750 (the one from 2012, not to be confused with the RX 7000 series) and newer
* Intel HD 510 (Skylake)
* A Mac with Apple Silicon or an Intel 7th Gen CPU with MacOS 13.0+

On x86-64 PCs, a CPU supporting the SSE4.1 instruction set is also required (Intel Core 2 Penryn series or AMD Bulldozer and newer). ARM64 builds will work on any ARM64 CPU.

If you have issues with crashes on startup, make sure your graphics drivers are fully up to date. 

## Features

#### Plug and Play
Simply provide your copy of the North American version of the game in the main menu and start playing! This project will automatically load assets from the provided copy, so there is no need to go through a separate extraction step or build the game yourself.

#### Fully Intact N64 Effects
A lot of care was put into RT64 to make sure all graphical effects were rendered exactly as they did originally on the N64. No workarounds or "hacks" were made to replicate these effects, with the only modifications to them being made for enhancement purposes such as widescreen support.

#### Easy-to-Use Menus
Gameplay settings, graphics settings, input mappings, and audio settings can all be configured with the in-game config menu. The menus can all be used with mouse, controller, or keyboard for maximum convenience.

#### High Framerate Support
Play at any framerate you want thanks to functionality provided by RT64! Game objects and terrain, texture scrolling, screen effects, and most HUD elements are all rendered at high framerates. By default, this project is configured to run at your monitor's refresh rate. You can also play at the original framerate of the game if you prefer. **Changing framerate has no effect on gameplay.**

**Note**: External framerate limiters (such as the NVIDIA Control Panel) are known to potentially cause problems, so if you notice any stuttering then turn them off and use the manual framerate slider in the in-game graphics menu instead.

#### Widescreen and Ultrawide Support
Any aspect ratio is supported, with most effects modded to work correctly in widescreen. The HUD can also be positioned at 16:9 when using ultrawide aspect ratios if preferred.

**Note**: Some animation quirks can be seen at the edges of the screen in certain cutscenes when using very wide aspect ratios.

#### Mod Support
Install community made mods and texture packs! Mods can change any part of the game, including adding completely new features and content. You can install mods by simply dragging the mod files onto the game window before starting the game or by clicking the **Install Mods** button in the mod menu. Mods can be toggled in the mod menu, and some mods can be configured there as well.

If you're interested in making mods for this project, check out [the mod template](https://github.com/sonicdcer/MK64RecompModTemplate) and [the modding documentation](https://hackmd.io/fMDiGEJ9TBSjomuZZOgzNg). If you're interested in making texture packs, check out [the RT64 documentation](https://github.com/rt64/rt64/blob/main/TEXTURE-PACKS.md).

#### Additional Control Options
Customize your experience by setting your stick deadzone to your liking, as well as adjusting the X and Y axis inversion for aiming.

#### Low Input Lag
This project has been optimized to have as little input lag as possible, making the game feel more responsive than ever!

#### Linux and Steam Deck Support
A Linux binary as well as a Flatpak is available for playing on most up-to-date distros, including on the Steam Deck.

To play on Steam Deck, extract the Linux build onto your deck. Then, in desktop mode, right click the MarioKart64Recompiled executable file and select "Add to Steam". From there, you can return to Gaming mode and configure the controls as needed.

## Planned Features
* Model Replacements
* Ray Tracing (via RT64)
* Multi-language support with support for loading custom translations

## FAQ

#### What is static recompilation?
Static recompilation is the process of automatically translating an application from one platform to another. For more details, check out the full description of how this project's recompilation works here: [N64: Recompiled](https://github.com/Mr-Wiseguy/N64Recomp).

#### How is this related to the decompilation project?
Unlike N64 ports in the past, this project is not based on the source code provided by a decompilation of the game. This is because static recompilation bypasses the need for decompiled source code when making a port, allowing ports to be made **without source code**. However, the reverse engineering work done by the decompilation team was invaluable for providing some of the enhancements featured in this project. For this reason, the project uses headers and some functions from the decompilation project in order to make modifications to the game. Many thanks to the decompilation team for all of the hard work they've done.

#### Where is the savefile stored?
- Windows: `%LOCALAPPDATA%\MarioKart64Recompiled\saves`
- Linux: `~/.config/MarioKart64Recompiled/saves`
- macOS: `~/Library/Application Support/MarioKart64Recompiled/saves`

#### How do I choose a different ROM?
**You don't.** This project is **only** a port of MarioKart 64, and it will only accept one specific ROM: the US version of the N64 release of MarioKart 64. ROMs in formats other than .z64 will be automatically converted, as long as it is the correct ROM. **It is not an emulator and it cannot run any arbitrary ROM.** 

Instead, you can change the game by installing mods. See the [mod support](#mod-support) section for details.

#### Can you run this project as a portable application?
Yes, if you place a file named `portable.txt` in the same folder as the executable then this project will run in portable mode. In portable mode, the save files, config files, and mods are placed in the same folder as the executable.

If you want to play a modded ROM or in another language, note that support for modding and other languages will be added to the project itself in the future and will not rely on you supplying a different ROM. 

## Known Issues
* Intel GPUs on Linux may not currently work. If you have experience with Vulkan development on Linux, help here would be greatly appreciated!
* The prebuilt Linux binary may not work correctly on some distributions of Linux. If you encounter such an issue, building the project locally yourself is recommended. A Flatpak or AppImage may be provided in the future to solve this issue. Adding the Linux version to Steam and setting "Steam Linux Runtime" as the compatibility tool or launching it via Gamescope may work around the issue. Alternatively, running the Windows version with Proton is known to work well and may also work around this issue.
* Overlays such as MSI Afterburner and other software such as Wallpaper Engine can cause performance issues with this project that prevent the game from rendering correctly. Disabling such software is recommended.

## Building
Building is not required to play this project, as prebuilt binaries (which do not contain game assets) can be found in the [Releases](https://github.com/sonicdcer/MarioKart64Recomp/releases/latest) section. Instructions on how to build this project can be found in the [BUILDING.md](BUILDING.md) file.

## Libraries Used and Projects Referenced
* [RT64](https://github.com/rt64/rt64) for the project's rendering engine
* [RmlUi](https://github.com/mikke89/RmlUi) for building the menus and launcher
* [lunasvg](https://github.com/sammycage/lunasvg) for SVG rendering, used by RmlUi
* [FreeType](https://freetype.org/) for font rendering, used by RmlUi  
* [moodycamel::ConcurrentQueue](https://github.com/cameron314/concurrentqueue) for semaphores and fast, lock-free MPMC queues
* [Gamepad Motion Helpers](https://github.com/JibbSmart/GamepadMotionHelpers) for sensor fusion and calibration algorithms to implement gyro aiming
* [MarioKart 64 Decompilation](https://github.com/n64decomp/mk64) for headers and some function definitions, used for making patches or some enhancements
* [Ares emulator](https://github.com/ares-emulator/ares) for RSP vector instruction reference implementations, used in RSP recompilation

Special thanks to [thecozies](https://github.com/thecozies) for designing and helping implement the launcher and config menus!
