#pragma once
#include "BoardHandlerInterface.hpp"
#include "BoardRepoInterface.hpp"
#include "TaskRepoInterface.hpp"
#include <memory>

using ITaskRepoUptr = std::unique_ptr<TaskRepoInterface>;

class BoardController : public BoardHandlerInterface
{
public:
    ~BoardController() { }
    BoardController(ITaskRepoUptr taskRepo): _taskRepo(std::move(taskRepo)) { }
    Task AddTask(int boardId, const std::string& taskName, const std::string &text) override;
    bool DeleteTask(int taskId) override;
    bool ChangeTaskStatus(int taskId, TaskStatus status) override;
    std::vector<Task> GetAllTasksForBoard(int boardId) override;
private:
    ITaskRepoUptr _taskRepo;
};