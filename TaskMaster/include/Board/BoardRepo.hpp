#include "BoardRepoInterface.hpp"
#include "DbDriverInterface.hpp"
#include <memory>

using IDbDriverSptr = std::shared_ptr<DbDriverInterface>;

class BoardRepo : public BoardRepoInterface
{
public:
    ~BoardRepo() { }
    BoardRepo(IDbDriverSptr dbBriver) : _dbDriver(std::move(dbBriver)) { }
    std::vector<Board> GetAllBoardsForProject(int projectId) override;
    bool CreateBoard(int projectId, int creatorId, const std::string &name, const std::string &text) override;
    bool DeleteBoard(int boardId) override;
    Board GetBoard(int boardId) override;
private:
    IDbDriverSptr _dbDriver;
};