My first college-level game project, Japanese Mahjong (or richii mahjong). 
Licensed under GNU GPL.
Utilizes SFML 2.1 (licensed udner zlib/libpng).
Developed in Visual Studio 2012, provided by Microsoft DreamSpark.

This project uses a client-server architecture, so the server is actually a separate project. They are deeply intertwined, however.

COMPILATION INSTRUCTIONS:
To implement SFML into Visual Studio 2012, for each project the following has to be done in VS:

Under Configuration Properties:
- Under C/C++:
-- Under General, add "<location of SFML-2.1 compilation>\SFML-2.1\include" to "Additional Include Directories"
- Under Linker:
-- Under General, add "<location of SFML-2.1 compilation>\SFML-2.1\lib" to "Additional Library Directories"
-- Under Input, add "sfml-main-d.lib;sfml-audio-d.lib;sfml-graphics-d.lib;sfml-network-d.lib;sfml-system-d.lib;sfml-window-d.lib;" to the beginning of "Additional Dependencies"

Once you have compiled both projects in one solution in VS2012, you will need to drop the following file in the solutions' Debug folder: 
libsnfile-1.dll
openal32.dll
sfml-audio-d-2.dll
sfml-graphics-d-2.dll
sfml-network-d-2.dll
sfml-system-d-2.dll
sfml-window-d-2.dll

These can be found in the /bin/ folder of the SFML distribution

It is probably possible to compile with say, gcc instead, but I have not tried and might not be able to help you if you wish to do so.