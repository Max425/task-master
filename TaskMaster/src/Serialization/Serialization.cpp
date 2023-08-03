#include "Serialization.hpp"
#include <iostream>

Project serializationProject(const std::vector<std::string> &answer)
{
    Project project;
    project.Id = std::stoi(answer[0]);
    project.Name = answer[1];
    project.CreatorId = std::stoi(answer[2]);
    project.Description = answer[3];

    return project;
}

Board serializationBoard(const std::vector<std::string> &answer)
{
    Board board;
    board.Id = std::stoi(answer[0]);
    board.Name = answer[1];
    board.Text = answer[2];
    board.CreatorId = std::stoi(answer[3]);
    board.ProjectId = std::stoi(answer[4]);
    return board;
}

User serializationUser(const std::vector<std::string> &answer)
{
    User user;
    user.Id = std::stoi(answer[0]);
    user.UserName = answer[1];
    return user;
}

Task serializationTask(const std::vector<std::string> &answer)
{
    Task task;
    task.Id = std::stoi(answer[0]);
    task.Name = answer[1];
    task.Text = answer[2];
    task.Status = static_cast<TaskStatus>(std::stoi(answer[3]));
    task.BoardId = std::stoi(answer[4]);
    task.DueDate = answer[5];
    return task;
}
