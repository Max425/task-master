add_executable(TaskMasterTest   Project/ProjectControllerTest.cpp Project/ProjectRepoTest.cpp 
                                Board/BoardControllerTest.cpp
                                Board/BoardRepoTest.cpp
                                Task/TaskControllerTest.cpp
                                Task/TaskRepoTest.cpp
                                User/UserControllerTest.cpp
                                User/UserRepoTest.cpp)
                              
find_package(GTest CONFIG REQUIRED)

target_link_libraries(TaskMasterTest TaskMaster
                                 GTest::gtest_main GTest::gmock)
                                 
target_include_directories(TaskMasterTest PUBLIC .)

target_link_libraries(TaskMasterTest TaskMaster
                                 GTest::gtest_main)

add_test(TaskMasterTest TaskMasterTest)