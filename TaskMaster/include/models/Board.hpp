#pragma once
#include <string>

class Board
{
public:
    Board();
    Board(int projectId, int creatorId, const std::string &name, const std::string &text);
    size_t Id;
    size_t ProjectId;
    size_t CreatorId;
    std::string Name;
    std::string Text;
};
