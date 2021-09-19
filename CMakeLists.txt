cmake_minimum_required(VERSION 3.16.3)
project(OS_Assignment-2 C)

set(CMAKE_C_STANDARD 11)
set (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address -fsanitize=undefined")
set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address -fsanitize=undefined")

set( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR} )
#add_subdirectory(./dist)
add_executable(OS_Assignment-2 main.c
        ./utils/init.c 
        utils/init.h 
        global.h 
        src/prompt.c 
        utils/clear_variables.c 
        utils/clear_variables.h 
        src/prompt.h 
        src/cd.c 
        src/cd.h 
        src/echo.c 
        src/echo.h 
        src/args_vector.c 
        src/args_vector.h 
        src/run_bg.c 
        src/run_bg.h 
        src/run_fg.c 
        src/run_fg.h 
        src/pinfo.c 
        src/pinfo.h 
        src/track_bg.c 
        src/track_bg.h 
        utils/pid_name_pair.c 
        utils/pid_name_pair.h 
        utils/stack.c 
        utils/stack.h src/ls.c src/ls.h src/history.c src/history.h)

target_link_libraries(OS_Assignment-2 m)