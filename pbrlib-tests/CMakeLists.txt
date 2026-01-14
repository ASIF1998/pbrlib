#####################################################
#   Create PBRLibTests executable
#####################################################
file(GLOB_RECURSE PBRLIB_TESTS_SOURCES CONFIGURE_DEPENDS *.cpp)
file(GLOB_RECURSE PBRLIB_TESTS_HEADERS CONFIGURE_DEPENDS *.hpp)

add_executable(pbrlib-tests ${PBRLIB_TESTS_SOURCES} ${PBRLIB_TESTS_HEADERS})

target_include_directories(pbrlib-tests PRIVATE ../backend/include)

#####################################################
#   link gtest
#####################################################
if (WIN32)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/googletest third_party/googletest)
enable_testing()
include_directories(../third_party/googletest/googletest/include ${gtest_SOURCE_DIR})

target_link_libraries(pbrlib-tests PRIVATE pbrlib gtest)
