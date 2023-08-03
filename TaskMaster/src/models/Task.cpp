#include "Task.hpp"

Task::Task( int boardId, const std::string &name, const std::string &text) :
     BoardId(boardId), Name(name),Text(text), Status(TaskStatus::Open) {}

Task::Task(){}