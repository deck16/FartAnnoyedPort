cmake_minimum_required (VERSION 3.0)

project(ChiliPy)

set(SOURCES 
"Game.cpp"
"Sound.cpp"
"Mouse.cpp"
"MainWindow.cpp"
"Main.cpp"
"Keyboard.cpp"
"Graphics.cpp"
"DXErr.cpp"
"COMInitializer.cpp"
"Resource.rc")

set(HEADERS
"ChiliException.h"
"ChiliWin.h"
"Colors.h"
"COMInitializer.h"
"DXErr.h"
"Game.h"
"Graphics.h"
"Keyboard.h"
"MainWindow.h"
"Mouse.h"
"Resource.h"
"Sound.h"
"SoundEffect.h")

add_executable(Engine ${SOURCES} ${HEADERS})
add_definitions(-DUNICODE -D_UNICODE)
set_target_properties(Engine PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")