cmake_minimum_required (VERSION 3.0)

set(SOURCES 
"Game.cpp"
"Mouse.cpp"
"MainWindow.cpp"
"Main.cpp"
"Keyboard.cpp"
"Graphics.cpp"
"Ball.cpp"
"Brick.cpp"
"FrameTimer.cpp"
"Paddle.cpp"
"RectF.cpp"
"SpriteCodex.cpp"
"Vec2.cpp"
"Walls.cpp"
"Beveler.cpp")

set(HEADERS
"ChiliException.h"
"Colors.h"
"DXErr.h"
"Game.h"
"Graphics.h"
"Keyboard.h"
"MainWindow.h"
"Mouse.h"
"Ball.h"
"Brick.h"
"FrameTimer.h"
"Paddle.h"
"RectF.h"
"SpriteCodex.h"
"Vec2.h"
"Walls.h"
"Beveler.h"
"LifeCounter.h")

set(CMAKE_CXX_STANDARD 17)
add_executable(Engine ${SOURCES} ${HEADERS})
add_definitions(-DUNICODE -D_UNICODE) # just for dev time
target_link_libraries(Engine glfw glad)
