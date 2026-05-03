========================================================================
    APPLICATION : TeknoR6Vegas2 Project Overview (V3 / nice-rice fork)
========================================================================
 
This file contains a summary of what you will find in each of the files that
make up the TeknoR6Vegas2 application.
 
========================================================================
    SOURCE FILES
========================================================================
 
TeknoR6Vegas2.vcxproj
    Main project file for Visual C++. Contains platform, configuration,
    and build settings.
 
TeknoR6Vegas2.cpp
    Application entry point. Initialises the ModManager and launches the
    main form.
 
Form1.h
    UI form class and InitializeComponent(). Handles all user input and
    passes settings to ModManager. Does not contain any game logic.
 
Manager.h / Manager.cpp
    Core logic class (ModManager). Responsible for launching the game
    process, applying memory patches at startup, and coordinating
    settings from the UI. Supports both the standard client executable
    and the dedicated server (SADS).
 
IniConfig.h / IniConfig.cpp
    Handles reading and writing of game config (.ini) files. Creates
    backups of original files on first run and restores them as needed.
    Manages server config, map selection, weapon/gadget loadouts,
    graphics, sound, and network settings.
 
PE.h / PE.cpp
    Parses the PE header of the game executable to locate the entry
    point and code section for memory patching.
 
memory.h / memory.cpp
    Utilities for reading a region of a remote process into a local
    buffer and scanning it for byte patterns.
 
AssemblyInfo.cpp
    Contains custom attributes for modifying assembly metadata.
 
/////////////////////////////////////////////////////////////////////////////
Other standard files:
 
StdAfx.h, StdAfx.cpp
    Used to build a precompiled header (PCH) file named
    TeknoR6Vegas2.pch and a precompiled types file named StdAfx.obj.
 
resource.h / app.rc
    Defines resource IDs and embeds the application icon and
    background image.
 
/////////////////////////////////////////////////////////////////////////////
- Source: https://github.com/nice-rice/Rainbow-Six-Vegas-2-Server-Mod
