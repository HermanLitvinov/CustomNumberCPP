add_library(CustomNumber source/CustomNumber.cpp include/CustomNumber.hpp)
target_include_directories(CustomNumber PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)