#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "User.hpp"
#include "Task.hpp"
#include "Board.hpp"
#include "Project.hpp"
#include "TaskRepoInterface.hpp"
#include "TaskController.hpp"

class TaskControllerTest : public ::testing::Test {
protected:
    void SetUp() override {}

    void TearDown() override {}
};

class MockTaskHandler : public TaskRepoInterface
{
public:
    MOCK_METHOD(bool, EditTask, (const Task& newTask), (override));
    MOCK_METHOD(bool, CreateTask, (const Task& Task), (override));
    MOCK_METHOD(std::vector<Task>, GetAllTasksForBoard, (int boardId), (override));
    MOCK_METHOD(bool, ChangeTaskStatus, (TaskStatus status, int taskId), (override));
    MOCK_METHOD(bool, DeleteTask, (int taskId), (override));
};


TEST_F(TaskHandlerTest, EditTaskCorrect)
{
    auto mockTaskHandler = std::make_unique<MockTaskHandler>();
    auto mockTaskHandlerAdr = mockTaskHandler.get();
    TaskController taskContr(std::move(mockTaskHandler));
    Task task = Task(1, 1, "Wash dishes", "need to wash");

    EXPECT_CALL(*mockTaskHandlerAdr, EditTask(task))
        .Times(1);

    taskContr.EditTask(task);
}

TEST_F(TaskHandlerTest, CreateTaskCorrect)
{
    auto mockTaskHandler = std::make_unique<MockTaskHandler>();
    auto mockTaskHandlerAdr = mockTaskHandler.get();
    TaskController taskContr(std::move(mockTaskHandler));
    Task task(1, 1, "Wash dishes", "Need to wash dishes");

    EXPECT_CALL(*mockTaskHandlerAdr, CreateTask(task))
        .Times(1);

    taskContr.CreateTask(task);
}