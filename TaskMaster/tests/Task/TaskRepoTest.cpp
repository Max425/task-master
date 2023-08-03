#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "TaskRepoInterface.hpp"
#include "TaskRepo.hpp"
#include "User.hpp"
#include "Task.hpp"
#include "DbDriverMock.hpp"

using std::vector;
using std::string;



//create task tests

TEST_F(TaskRepoTest, CreateTaskCorrect){
    auto mockDbDriver = std::make_unique<DbDriverMock>();
    auto mockDbDriverAdr = mockDbDriver.get();
    TaskRepo taskRepo(std::move(mockDbDriver));
    Task task(1, 1, "Wash dishes", "Need to wash dishes");
    EXPECT_CALL(*mockDbDriverAdr, Connected())
        .Times(testing::AtLeast(1))
        .WillRepeatedly(testing::Return(true));
    EXPECT_CALL(*mockDbDriverAdr, 
                Exec("INSERT INTO task (ProjectId, BoardId, TaskName, Text) VALUES (1), (1), ('Wash dishes'), ('Need to wash dishes');"))
                .Times(testing::AtLeast(1));
    taskRepo.CreateTask(task);
}

TEST_F(TaskRepoTest, CreateTaskWithNotExistingProjectId){
    auto mockDbDriver = std::make_unique<DbDriverMock>();
    auto mockDbDriverAdr = mockDbDriver.get();
    TaskRepo taskRepo(std::move(mockDbDriver));
    Task task(-1, 1, "Wash dishes", "Need to wash dishes");
    EXPECT_CALL(*mockDbDriverAdr, Connected())
        .Times(testing::AtLeast(1))
        .WillRepeatedly(testing::Return(true));
    
    EXPECT_THROW(taskRepo.CreateTask(task), std::invalid_argument);
}

TEST_F(TaskRepoTest, CreateTaskWithNotExistingBoardId){
    auto mockDbDriver = std::make_unique<DbDriverMock>();
    auto mockDbDriverAdr = mockDbDriver.get();
    TaskRepo taskRepo(std::move(mockDbDriver));
    Task task(1, -1, "Wash dishes", "Need to wash dishes");
    EXPECT_CALL(*mockDbDriverAdr, Connected())
        .Times(testing::AtLeast(1))
        .WillRepeatedly(testing::Return(true));
    
    EXPECT_THROW(taskRepo.CreateTask(task), std::invalid_argument);
}

TEST_F(TaskRepoTest, CreateTaskWithNoName){
    auto mockDbDriver = std::make_unique<DbDriverMock>();
    auto mockDbDriverAdr = mockDbDriver.get();
    TaskRepo taskRepo(std::move(mockDbDriver));
    Task task(1, 1, "", "Need to wash dishes");
    EXPECT_CALL(*mockDbDriverAdr, Connected())
        .Times(testing::AtLeast(1))
        .WillRepeatedly(testing::Return(true));
    
    EXPECT_THROW(taskRepo.CreateTask(task), std::invalid_argument);
}

TEST_F(TaskRepoTest, CreateTaskNoDescription){
    auto mockDbDriver = std::make_unique<DbDriverMock>();
    auto mockDbDriverAdr = mockDbDriver.get();
    TaskRepo taskRepo(std::move(mockDbDriver));
    Task task(1, 1, "Wash dishes", "");
    EXPECT_CALL(*mockDbDriverAdr, Connected())
        .Times(testing::AtLeast(1))
        .WillRepeatedly(testing::Return(true));
    EXPECT_CALL(*mockDbDriverAdr, 
                Exec("INSERT INTO task (" + std::to_string(ProjectId) + "," + std::to_string(BoardId) + "," + TaskName +"," + Text +") VALUES (1), (1), ('Wash dishes'), ('');"))
                .Times(testing::AtLeast(1));
    taskRepo.CreateTask(task);
}

//edit task tests

TEST_F(TaskRepoTest, EditTaskCorrect){
    auto mockDbDriver = std::make_unique<DbDriverMock>();
    auto mockDbDriverAdr = mockDbDriver.get();
    TaskRepo taskRepo(std::move(mockDbDriver));
    Task task(1, 1, "Wash dishes", "");
    EXPECT_CALL(*mockDbDriverAdr, Connected())
        .Times(testing::AtLeast(1))
        .WillRepeatedly(testing::Return(true));
    std::string expectedSqlQuery = "UPDATE tasks SET name = " + task.TaskName + "WHERE name = " + task.TaskName + ";";
    EXPECT_CALL(*mockDbDriverAdr, Exec(expectedSqlQuery))
        .WillRepeatedly(testing::Return(true));
    taskRepo.EditTask(task);
}

//get all tasks for board tests

TEST_F(TaskRepoTest, GetAllTasksForBoardCorrect){
    auto mockDbDriver = std::make_unique<DbDriverMock>();
    auto mockDbDriverAdr = mockDbDriver.get();
    TaskRepo taskRepo(std::move(mockDbDriver));
    int Id = 1;
    EXPECT_CALL(*mockDbDriverAdr, Connected())
        .Times(testing::AtLeast(1))
        .WillRepeatedly(testing::Return(true));
    EXPECT_CALL(*mockDbDriverAdr, Exec("SELECT TaskName, Text FROM board INNER JOIN task ON BoardId =" + std::to_string(Id) +";"))
                .Times(testing::AtLeast(1));
    taskRepo.GetAllTasksForBoard(Id);
}

TEST_F(TaskRepoTest, GetAllTasksForBoardNoTaskWithSuchId){
    auto mockDbDriver = std::make_unique<DbDriverMock>();
    auto mockDbDriverAdr = mockDbDriver.get();
    TaskRepo taskRepo(std::move(mockDbDriver));
    int Id = -1;
    EXPECT_CALL(*mockDbDriverAdr, Connected())
        .Times(testing::AtLeast(1))
        .WillRepeatedly(testing::Return(true));
        
    EXPECT_THROW(taskRepo.GetAllTasksForBoard(Id);, std::invalid_argument);
}

//change task status tests

TEST_F(TaskRepoTest, ChangeTaskStatusCorrect){
    int taskId = 1;
    TaskStatus taskStat = Open;
    auto mockDbDriver = std::make_unique<DbDriverMock>();
    auto mockDbDriverAdr = mockDbDriver.get();
    TaskRepo taskRepo(std::move(mockDbDriver));
    EXPECT_CALL(*mockDbDriverAdr, Connected())
        .Times(testing::AtLeast(1))
        .WillRepeatedly(testing::Return(true));
    std::string expectedSqlQuery = "UPDATE tasks SET status = " + std::to_string(taskStat) + "WHERE Id = " + std::to_string(taskId) + ";";
    EXPECT_CALL(*mockDbDriverAdr, Exec(expectedSqlQuery))
        .WillRepeatedly(testing::Return(true));
    taskRepo.ChangeTaskStatus(taskStat, taskId);
}

TEST_F(TaskRepoTest, ChangeTaskStatusNoSuchId){
    int taskId = -1;
    TaskStatus taskStat = Open;
    auto mockDbDriver = std::make_unique<DbDriverMock>();
    auto mockDbDriverAdr = mockDbDriver.get();
    TaskRepo taskRepo(std::move(mockDbDriver));
    EXPECT_CALL(*mockDbDriverAdr, Connected())
        .Times(testing::AtLeast(1))
        .WillRepeatedly(testing::Return(true));

    EXPECT_THROW(taskRepo.ChangeTaskStatus(taskStat, taskId), std::invalid_argument);
}

//delete task tests

TEST_F(TaskRepoTest, DeleteTaskCorrect){
    auto mockDbDriver = std::make_unique<DbDriverMock>();
    auto mockDbDriverAdr = mockDbDriver.get();
    TaskRepo taskRepo(std::move(mockDbDriver));
    int Id = 1;
    EXPECT_CALL(*mockDbDriverAdr, Connected())
        .Times(testing::AtLeast(1))
        .WillRepeatedly(testing::Return(true));
    EXPECT_CALL(*mockDbDriverAdr, Exec("DELETE FROM tasks WHERE id =" + std::to_string(Id) + ";"))
                .Times(testing::AtLeast(1));
    taskRepo.DeleteTask(Id);
}


TEST_F(TaskRepoTest, DeleteTaskWithNoSuchId){
    auto mockDbDriver = std::make_unique<DbDriverMock>();
    auto mockDbDriverAdr = mockDbDriver.get();
    TaskRepo taskRepo(std::move(mockDbDriver));
    int Id = -1;
    EXPECT_CALL(*mockDbDriverAdr, Connected())
        .Times(testing::AtLeast(1))
        .WillRepeatedly(testing::Return(true));
    
    EXPECT_THROW(taskRepo.DeleteTask(Id), std::invalid_argument);
}
