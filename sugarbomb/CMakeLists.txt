include_directories(${CMAKE_CURRENT_SOURCE_DIR}/SDK/Include)

#******************************************************************************

option(SBE_SYSTEM_HARD_LINKED "Compile the system module into the app code" OFF)

option(SBE_SOUND_HARD_LINKED "Compile the sound module into the app code" OFF)
option(SBE_INPUT_HARD_LINKED "Compile the input module into the app code" OFF)
option(SBE_RENDERER_HARD_LINKED "Compile the renderer module into the app code" OFF)

option(SBE_NETWORK_HARD_LINKED "Compile the network module into the app code" OFF)

option(SBE_GAMEFRAMEWORK_HARD_LINKED "Compile the game framework module into the app code" OFF)
option(SBE_GAME_HARD_LINKED "Compile the game module into the app code" OFF)

option(SBE_SINGLE_BINARY "Compile all the modules into the app code (which results in a single executable app)" OFF)

#******************************************************************************

add_subdirectory(AppFrameworks)
add_subdirectory(CoreLibs)

add_subdirectory(SbGameFramework)