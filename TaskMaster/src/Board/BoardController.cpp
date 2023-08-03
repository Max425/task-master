#include "BoardController.hpp"

Task BoardController::AddTask(int boardId, const std::string& taskName, const std::string &text)
{
    auto task = Task(boardId, taskName, text);
    return _taskRepo->CreateTask(task);
}

bool BoardController::DeleteTask(int taskId)
{
    return _taskRepo->DeleteTask(taskId);
}

bool BoardController::ChangeTaskStatus(int taskId, TaskStatus status)
{
    return _taskRepo->ChangeTaskStatus(status, taskId);
}

std::vector<Task> BoardController::GetAllTasksForBoard(int boardId)
{
    return _taskRepo->GetAllTasksForBoard(boardId);
}
