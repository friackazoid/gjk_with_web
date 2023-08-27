find_package(Seasocks REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(jsoncpp REQUIRED)

add_executable(${PROJECT_NAME} main.cpp gjk.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${EIGEN3_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE 
    Seasocks::seasocks
    jsoncpp_lib
    )
