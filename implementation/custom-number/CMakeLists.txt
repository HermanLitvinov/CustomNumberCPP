add_library(CustomNumber source/CustomNumber.cpp include/CustomNumber.hpp source/CustomNumberManager.cpp include/CustomNumberManager.hpp include/CustomNumberOperations.hpp)
#add_library(CustomNumberManager source/CustomNumberManager.cpp include/CustomNumberManager.hpp)
#add_library(CustomNumberOperations include/CustomNumberOperations.hpp)

target_include_directories(CustomNumber PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)