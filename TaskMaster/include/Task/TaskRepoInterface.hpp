#pragma once
#include <string>
#include <vector>
#include "User.hpp"
#include "Task.hpp"
#include "Task.hpp"

class TaskRepoInterface
{
public:
    virtual ~TaskRepoInterface() {}
    virtual bool EditTask(const Task& newTask) = 0;
    virtual Task CreateTask(const Task& Task) = 0;
    virtual std::vector<Task> GetAllTasksForBoard(int boardId) = 0;
    virtual bool ChangeTaskStatus(TaskStatus status, int taskId) = 0;
    virtual bool DeleteTask(int taskId) = 0;
    virtual std::vector<Task> GetTasksForUser(int userId) = 0;
};