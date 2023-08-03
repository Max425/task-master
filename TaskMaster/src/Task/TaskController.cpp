#include "TaskController.hpp"

bool TaskController::EditTask(const Task& newTask) 
{
    return _repo->EditTask(newTask);
}

Task TaskController::CreateTask(const Task& Task) 
{
    return _repo->CreateTask(Task);
}

bool TaskController::DeleteTask(int taskId)
{
    return _repo->DeleteTask(taskId);
}

std::vector<Task> TaskController::GetTasksForUser(int userId)
{
    return _repo->GetTasksForUser(userId);
}
