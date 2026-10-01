cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME code)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/ssd1306/ssd1306.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/ssd1306/ssd1306_tests.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/ssd1306/ssd1306_fonts.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/ssd1306/ssd1306.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/ssd1306/ssd1306_tests.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/ssd1306/ssd1306_fonts.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/ssd1306/ssd1306_conf.h

    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/LED/led.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/LED/led.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/BUTTON/button.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/BUTTON/button.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/NEO_6M/gps.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/NEO_6M/gps.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/LMT01/lmt01.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/LMT01/lmt01.h




    ${CMAKE_CURRENT_SOURCE_DIR}/App/Src/analytics.c
    ${CMAKE_CURRENT_SOURCE_DIR}/App/Src/diagnostics.c
    ${CMAKE_CURRENT_SOURCE_DIR}/App/Src/logging.c
    ${CMAKE_CURRENT_SOURCE_DIR}/App/Src/system.c
    ${CMAKE_CURRENT_SOURCE_DIR}/App/Src/telemetry.c
    ${CMAKE_CURRENT_SOURCE_DIR}/App/Src/ui.c

     
    ${CMAKE_CURRENT_SOURCE_DIR}/App/Inc/analytics.h
    ${CMAKE_CURRENT_SOURCE_DIR}/App/Inc/diagnostics.h
    ${CMAKE_CURRENT_SOURCE_DIR}/App/Inc/logging.h
    ${CMAKE_CURRENT_SOURCE_DIR}/App/Inc/system.h
    ${CMAKE_CURRENT_SOURCE_DIR}/App/Inc/system_types.h
    ${CMAKE_CURRENT_SOURCE_DIR}/App/Inc/telemetry.h
    ${CMAKE_CURRENT_SOURCE_DIR}/App/Inc/ui.h

    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/MPU6050/mpu6050.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/MPU6050/mpu6050.h

)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
    ${CMAKE_CURRENT_SOURCE_DIR}/App/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/BUTTON
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/HC_SR04
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/KEYPAD
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/LED
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/LMT01
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/MPU6050
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/NEO_6M
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/SFH203
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/ssd1306
    


)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols

)


target_link_options(${CMAKE_PROJECT_NAME} PRIVATE "-u _printf_float" "-u _scanf_float")

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)
