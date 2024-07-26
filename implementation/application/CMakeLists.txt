add_executable(App test.cpp)

target_link_libraries(App PRIVATE CustomNumber) #CustomNumber from custom-number/
#target_link_libraries(App PRIVATE CustomNumberManager) #CustomNumberManager from custom-number/
#target_link_libraries(App PRIVATE CustomNumberOperations) #CustomNumberOperations from custom-number/