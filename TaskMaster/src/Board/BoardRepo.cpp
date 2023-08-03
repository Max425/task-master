#include "BoardRepo.hpp"
#include "Serialization.hpp"
#include <exception>
#include <iostream>

Board BoardRepo::CreateBoard(int projectId, int creatorId, const std::string &name, const std::string &text)
{
    auto answer = _dbDriver->Exec("INSERT INTO board (project_id, creator_id, name, text) VALUES ("
                     + std::to_string(projectId) + ", " + std::to_string(creatorId) + ", \'"
                     + name + "\', \'" + text + "\') RETURNING *;");
    return serializationBoard(answer[0]);
}

Board BoardRepo::GetBoard(int boardId)
{
    auto answer = _dbDriver->Exec("SELECT * FROM board WHERE id=" + std::to_string(boardId) + ";");
    if (answer.size() == 0)
        throw std::runtime_error("Board with id=" + std::to_string(boardId) + " not found");
    return serializationBoard(answer[0]);
}

void BoardRepo::EditBoard(const Board &board)
{
    if (!_dbDriver->Connected())
        throw std::runtime_error("Database is unavailable");
    _dbDriver->Exec("UPDATE board SET text = \'" + board.Text + "\', name = \'" + board.Name + "\' WHERE id = " + std::to_string(board.Id));
}

bool BoardRepo::DeleteBoard(int boardId)
{
    _dbDriver->Exec("DELETE FROM board WHERE id=" + std::to_string(boardId) + ";");
    return true;
}

std::vector<Board> BoardRepo::GetAllBoardsForProject(int projectId)
{
    auto answer = _dbDriver->Exec("SELECT * FROM board WHERE project_id=" + std::to_string(projectId) + ";");
    if (answer.size() == 0)
        return std::vector<Board>();
    std::vector<Board> res;
    for (const auto &data : answer)
    {
        res.push_back(serializationBoard(data));
    }
    return res;
}
