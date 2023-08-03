#pragma once
#include <string>
#include <memory>
#include "User.hpp"
#include "Task.hpp"
#include "TaskHandlerInterface.hpp"
#include "TaskRepoInterface.hpp"

using TRepoUPtr = std::unique_ptr<TaskRepoInterface>;

class TaskController : public TaskHandlerInterface
{
public:
    TaskController(TRepoUPtr repo): _repo(std::move(repo)) {}
    ~TaskController() {}
    bool EditTask(const Task& newTask) override;
    Task CreateTask(const Task& Task) override;
    bool DeleteTask(int taskId) override;
    std::vector<Task> GetTasksForUser(int userId) override;

private:
    TRepoUPtr _repo;
};