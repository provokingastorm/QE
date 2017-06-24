#ifndef E_CONFIG_H
#define E_CONFIG_H

/////////////////////////////
// PROGRAMS (Main functions)
/////////////////////////////
#define BUILD_DIAG          0
#define BUILD_SCANTAGS      0
#define BUILD_ANIMTWO       0
#define BUILD_LIGHTJOINT    0
#define BUILD_FOG           0
#define BUILD_POLYCOL       0
#define BUILD_PARSE         0
#define BUILD_POWERBAR      0
#define BUILD_WAYPOINTS     0
#define BUILD_PERSONTEST    0
#define BUILD_COMMANDS      0
#define BUILD_SHOULDERPROJ	1

#define BUILD_GAME          0


/////////////////////////////
// PROGRAM CODE
/////////////////////////////

#define BUILD_PROVOKING     0  // Builds the codebase for Provoking without the main() function

/////////////////////////////
// BUILD OPTIONS
/////////////////////////////
#define BUILD_HUD           1   // Builds with the Hud displayed
#define BUILD_SOUNDS        1   // Builds with Sounds that play
#define BUILD_ANIMATIONS    1
#define LOSE_CONDITIONS     1   // Builds with win / lose conditions
#define FULLSCREEN          0   // Builds the game in fullscreen mode
#define BUILD_MENUS         0   // Builds the menu-ed version of Provoking

/////////////////////////////
// DEBUG SETTINGS
/////////////////////////////
#define DEBUG_WEATHER       0   // Displays debug output (print statements) related to the weather
#define DEBUG_PERSON_COLORS 0   // Renders the people in colors instead of textures

#define DEBUG_SYSTEM        0
#define DEBUG_RESOURCES     0   // Displays debug output (print statements) related to the ResourceManager
#define DEBUG_MESSAGING     0   // Displays debug output (print statements) related to the MessagingSystem
#define DEBUG_ENTITIES      0   // Displays debug output (print statements) related to the EntityManager
#define DEBUG_TYPEHANDLER   0   // Displays debug output (print statements) related to the TypeHandler

#define BUILD_NEWBUTTONS    0

/////////////////////////////
// PROVOKING TOOLS
/////////////////////////////
#define BUILD_FACETEST      0
#define BUILD_RAINPROTOTYPE 0

#endif
