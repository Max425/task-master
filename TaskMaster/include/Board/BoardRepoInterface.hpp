#pragma once
#include <vector>
#include "Board.hpp"
#include "Task.hpp"

class BoardRepoInterface
{
public:
    virtual ~BoardRepoInterface() { }
    virtual std::vector<Board> GetAllBoardsForProject(int projectId) = 0;
    virtual Board CreateBoard(int projectId, int creatorId, const std::string &name, const std::string &text) = 0;
    virtual bool DeleteBoard(int boardId) = 0;
    virtual Board GetBoard(int boardId) = 0;
    virtual void EditBoard(const Board &board) = 0;
};