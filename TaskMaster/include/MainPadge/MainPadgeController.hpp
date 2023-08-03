#pragma once
#include <vector>
#include <string>
#include <memory>
#include "ProjectRepoInterface.hpp"
#include "MainPadgeHandlerInterface.hpp"
#include "Project.hpp"

using IProjectRepoUptr = std::unique_ptr<ProjectRepoInterface>;

class MainPadgeController : public MainPadgeHandlerInterface
{
public:
    MainPadgeController(IProjectRepoUptr projectRepo);
    std::vector<Project> GetUserProjects(size_t userId) override;
    virtual Project GetProjectById(size_t projectId) override;
    virtual Project CreateProject(size_t userId, const std::string &projectName, const std::string &text) override;
    virtual void DeleteProject(const Project &project) override;
    virtual void DeleteUserFromProject(const Project &project, size_t userId) override;
    virtual void EditProject(const Project &project) override;

private:
    IProjectRepoUptr projectRepo_;
};