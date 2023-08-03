#include "BoardRepoInterface.hpp"
#include "DbDriverInterface.hpp"
#include <memory>

using DbDriverSPtr = std::shared_ptr<DbDriverInterface>;

class BoardRepo : public BoardRepoInterface
{
public:
    ~BoardRepo() { }
    BoardRepo(DbDriverSPtr dbBriver) : _dbDriver(std::move(dbBriver)) { }
    std::vector<Board> GetAllBoardsForProject(int projectId) override;
    Board CreateBoard(int projectId, int creatorId, const std::string &name, const std::string &text) override;
    bool DeleteBoard(int boardId) override;
    Board GetBoard(int boardId) override;
    void EditBoard(const Board &board)  override;
private:
    DbDriverSPtr _dbDriver;
};