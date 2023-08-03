#pragma once
#include <memory>
#include "DbDriverInterface.hpp"
#include "ProjectRepoInterface.hpp"
#include "Project.hpp"

using DbDriverSPtr = std::shared_ptr<DbDriverInterface>;

class ProjectRepo : public ProjectRepoInterface
{
public:
    ProjectRepo(DbDriverSPtr dr);
    std::vector<Project> GetUserProjects(int userId) override;
    Project GetProjectById(int projectId) override;
    Project CreateProject(int userId, const std::string &projectName, const std::string &text) override;
    bool RemoveProjectById(int projectId) override;
    bool AddUserToProject(int userId, int projectId) override;
    bool EditProject(const Project &project) override;
    void DeleteUserFromProject(int projectId, size_t userId) override;

private:
    DbDriverSPtr dr_;
};
