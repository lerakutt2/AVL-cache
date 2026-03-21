cmake_minimum_required(VERSION 3.22)
project(diplom VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Добавляем поддержку extended numeric literals
add_compile_options(-fext-numeric-literals)

# Директория с нашими заголовками
include_directories(include)

if(DEFINED ENV{QUADMATH_ROOT})
    set(QUADMATH_BASE_PATH $ENV{QUADMATH_ROOT})
    message(STATUS "Using QUADMATH_ROOT: ${QUADMATH_BASE_PATH}")
    
    # Ищем в указанной директории
    find_path(QUADMATH_INCLUDE_DIR
        NAMES quadmath.h
        PATHS ${QUADMATH_BASE_PATH}/include
        NO_DEFAULT_PATH
    )
    
    find_library(QUADMATH_LIBRARY
        NAMES quadmath libquadmath
        PATHS ${QUADMATH_BASE_PATH}/lib
        NO_DEFAULT_PATH
    )
endif()

# Если не нашли через переменную, ищем в стандартных местах
if(NOT QUADMATH_INCLUDE_DIR OR NOT QUADMATH_LIBRARY)
    # Для Linux/WSL
    if(UNIX)
        find_path(QUADMATH_INCLUDE_DIR
            NAMES quadmath.h
            PATHS
                /usr/include
                /usr/local/include
                /usr/lib/gcc/x86_64-linux-gnu/*/include
        )
        
        find_library(QUADMATH_LIBRARY
            NAMES quadmath
            PATHS
                /usr/lib
                /usr/lib/x86_64-linux-gnu
                /usr/local/lib
                /usr/lib/gcc/x86_64-linux-gnu/*/
        )
    endif()
endif()
# Проверяем, что нашли заголовки
if(NOT QUADMATH_INCLUDE_DIR)
    message(FATAL_ERROR "quadmath.h not found! Please install libquadmath-dev")
else()
    message(STATUS "Found quadmath.h: ${QUADMATH_INCLUDE_DIR}")
endif()

# Проверяем, что нашли библиотеку
if(NOT QUADMATH_LIBRARY)
    message(FATAL_ERROR "libquadmath not found in:\n"
            "  /usr/lib/x86_64-linux-gnu\n"
            "  /usr/lib/gcc/x86_64-linux-gnu/11\n"
            "Please install: sudo apt install libquadmath0")
else()
    message(STATUS "Found libquadmath: ${QUADMATH_LIBRARY}")
endif()

# Добавляем путь к заголовкам
include_directories(${QUADMATH_INCLUDE_DIR})

# Находим все исходники
file(GLOB SOURCES "src/diplom/*.cpp")

# Создаем исполняемый файл
add_executable(diplom_new ${SOURCES})

# Линкуем с quadmath
target_link_libraries(diplom_new ${QUADMATH_LIBRARY})

# Для отладки покажем, что мы нашли
message(STATUS "QUADMATH_INCLUDE_DIR: ${QUADMATH_INCLUDE_DIR}")
message(STATUS "QUADMATH_LIBRARY: ${QUADMATH_LIBRARY}")