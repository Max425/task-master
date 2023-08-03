CMAKE_MINIMUM_REQUIRED(VERSION 3.1)
set(CMAKE_CXX_STANDARD 20)
Project(TaskMaster)

FIND_PACKAGE(Wt REQUIRED Wt HTTP)
ADD_DEFINITIONS(-DHPDF_DLL)

find_package(PkgConfig)
pkg_check_modules(LIBCONFIG REQUIRED IMPORTED_TARGET libconfig++)

option(BUILD_TESTS "build tests")

if(BUILD_TESTS)
  enable_testing()
endif()

add_executable(taskmaster.wt
  taskmaster.C
  model/src/AuthUser.C
  model/src/Session.C
  view/src/MainView.C
  view/src/LoginWidget.C
  view/src/MainPage.C
  view/src/ProjectListItem.C
  view/src/ProjectPage.C
  view/src/BoardListItem.C
  view/src/BoardPage.C
  view/src/TaskListItem.C
  view/src/TasksContainer.C
  view/src/MyTasksPage.C
  view/src/Errors.C
  view/src/MainView.C
  view/src/ImageWidget.C
)

target_include_directories(taskmaster.wt PUBLIC model/include view/include)
target_link_libraries(taskmaster.wt Wt::Wt Wt::HTTP Wt::Dbo TaskMasterDB wtdbopostgres config++)

add_subdirectory(TaskMaster)
include_directories(TaskMaster/include)