add_library(NeuralNet include/AbstractModule.hpp)

target_include_directories(NeuralNet PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)