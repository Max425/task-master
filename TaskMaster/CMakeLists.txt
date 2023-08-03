cmake_minimum_required(VERSION 3.10)
project(TaskMasterDB)
add_library(TaskMasterDB src/Project/ProjectController.cpp
                        src/Project/ProjectRepo.cpp
                        src/Board/BoardController.cpp
                        src/Board/BoardRepo.cpp
                        src/Task/TaskController.cpp
                        src/Task/TaskRepo.cpp
                        src/User/UserController.cpp
                        src/User/UserRepo.cpp
                        src/models/User.cpp
                        src/models/Task.cpp
                        src/models/Board.cpp
                        src/models/Project.cpp
                        src/DbDriver/DbDriver.cpp
                        src/MainPadge/MainPadgeController.cpp
                        src/Serialization/Serialization.cpp
                        )

target_include_directories(TaskMasterDB PUBLIC include/Project include/Board include/Task include/User include/models include/DbDriver include/MainPadge include/Serialization)
target_link_libraries(TaskMasterDB pqxx pq)

if(BUILD_TESTS)
  add_subdirectory(tests)
endif()