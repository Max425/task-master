#include "MainPadgeController.hpp"

MainPadgeController::MainPadgeController(IProjectRepoUptr projectRepo) : projectRepo_(std::move(projectRepo)) {}

std::vector<Project> MainPadgeController::GetUserProjects(size_t userId)
{
    return projectRepo_->GetUserProjects(userId);
}

Project MainPadgeController::GetProjectById(size_t projectId)
{
    return projectRepo_->GetProjectById(projectId);
}

Project MainPadgeController::CreateProject(size_t userId, const std::string &projectName, const std::string &text)
{
    return projectRepo_->CreateProject(userId, projectName, text);
}

void MainPadgeController::DeleteProject(const Project &project)
{
    projectRepo_->RemoveProjectById(project.Id);
}

void MainPadgeController::DeleteUserFromProject(const Project &project, size_t userId)
{
    projectRepo_->DeleteUserFromProject(project.Id, userId);
}

void MainPadgeController::EditProject(const Project &project)
{
    projectRepo_->EditProject(project);
}
