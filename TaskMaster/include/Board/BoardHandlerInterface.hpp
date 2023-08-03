#pragma once
#include <string>
#include <vector>
#include "Task.hpp"
#include "User.hpp"
#include <vector>

class BoardHandlerInterface
{
public:
    virtual ~BoardHandlerInterface() { }
    virtual Task AddTask(int boardId, const std::string& taskName, const std::string &text) = 0;
    virtual bool DeleteTask(int taskId) = 0;
    virtual bool ChangeTaskStatus(int taskId, TaskStatus status) = 0;
    virtual std::vector<Task> GetAllTasksForBoard(int boardId) = 0;
};