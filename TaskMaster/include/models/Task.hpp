#pragma once
#include <string>
#include "Board.hpp"
#include "Project.hpp"

enum TaskStatus
{
    Open,
    Closed,
    InProgress
};

class Task
{
public:
    Task();
    Task(int boardId, const std::string &name, const std::string &text);
    size_t BoardId;
    size_t Id; 
    std::string Name;
    std::string Text;
    TaskStatus Status;
    std::string DueDate;
};
