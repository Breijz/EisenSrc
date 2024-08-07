# EisenSrc 
EisenSrc is a simple project aiming to improve the Quake Engine where possible. The eventual goal is for it to be compatible with the Goldsource Engine.

## Current Improvements
- Made it possible to compile on modern Linux systems (Fixed various compiling errors that prevented this and removed obselete code)
- Updated the build system to use Meson instead of GNU Make
- Added GLFW for the primary window and input system.

## Planned Improvements
- Fix/Change the audio system to one based off of Miniaudio
    - Add DSP
- ~~Convert x86 ASM code to C99~~
    - ~~Remove Lookup tables (these were used for optimizing the game on old CPUs, which are no longer needed)~~
    - It appears this is not needed as Id shipped the code in C for AMD processors of the era.
- Update OpenGL code to OpenGL 4.1
    - Add RGB Lighting
    - Add Translucent Materials
- Increase Engine Limits to be the same as GoldSource Engine Limits OR QSB Engine Limits; whichever is higher.
- Add WAD3, VTF, and VPK file support
- Change out Q3 programming for DLL programming (potentially both?)
- Update Networking Code
    - Cross Platform Networking Library
- Updating Code
    - Convert to C++
    - Tackle getting rid of the abundance of globals
    - Get rid of legacy code (3DFX, SVGA, WGL, GLX, Win32, software rendering, etc
    - Similar code structure to GoldSource / Source

## Support
If you find any issues in this project or wish to contribute (if so then thank you!) then you can DM me on discord via my tag 'breijz' or you can use Github's Issues feature.
