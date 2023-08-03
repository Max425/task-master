#include "ProjectController.hpp"

ProjectController::ProjectController(IProjectRepoUptr projectRepo, IBoardRepoUptr boardRepo, IUserRepoUptr userRepo) : 
    projectRepo_(std::move(projectRepo)), boardRepo_(std::move(boardRepo)), userRepo_(std::move(userRepo)) {}
ProjectController::~ProjectController() {}

Board ProjectController::CreateBoard(int projectId, int creatorId, const std::string &boardName, const std::string &text)
{
    return boardRepo_->CreateBoard(projectId, creatorId, boardName, text);
}
void ProjectController::AddUser(int userId, int projectId)
{
    userRepo_->AddProjectToUser(userId, projectId);
}
std::vector<User> ProjectController::GetAllUsers(int projectId)
{
    return userRepo_->GetUsersForProject(projectRepo_->GetProjectById(projectId).Id);
}
std::vector<Board> ProjectController::GetAllBoards(int projectId)
{
    return boardRepo_->GetAllBoardsForProject(projectId);
}

Board ProjectController::GetBoard(int boardId)
{
    return boardRepo_->GetBoard(boardId);
}

void ProjectController::DeleteBoard(int boardId)
{
    boardRepo_->DeleteBoard(boardId);
}

void ProjectController::EditBoard(const Board &board)
{
    boardRepo_->EditBoard(board);
}
