#pragma once
#include <string>
#include <memory>
#include <vector>
#include "User.hpp"
#include "Task.hpp"
#include "Task.hpp"
#include "TaskRepoInterface.hpp"
#include "DbDriverInterface.hpp"

using DbDriverUPtr = std::shared_ptr<DbDriverInterface>;

class TaskRepo: public TaskRepoInterface
{
public:
    TaskRepo(DbDriverUPtr dr): _dr(std::move(dr)) {}

    bool EditTask(const Task& newTask) override;
    Task CreateTask(const Task& Task) override;
    std::vector<Task> GetAllTasksForBoard(int boardId) override;
    bool ChangeTaskStatus(TaskStatus status, int taskId) override;
    bool DeleteTask(int taskId) override;
    std::vector<Task> GetTasksForUser(int userId) override;
    
    ~TaskRepo() {}
private:
    DbDriverUPtr _dr;
};