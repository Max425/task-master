#include <vector>
#include <string>
#include "Project.hpp"
#include "Board.hpp"
#include "User.hpp"
#include "Task.hpp"

Project serializationProject(const std::vector<std::string> &answer);
Board serializationBoard(const std::vector<std::string> &answer);
User serializationUser(const std::vector<std::string> &answer);
Task serializationTask(const std::vector<std::string> &answer);
