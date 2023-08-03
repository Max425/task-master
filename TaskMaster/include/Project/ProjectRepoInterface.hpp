#pragma once
#include <vector>
#include "Project.hpp"

class ProjectRepoInterface
{
public:
    virtual ~ProjectRepoInterface() {}
    virtual std::vector<Project> GetUserProjects(int userId) = 0;
    virtual Project GetProjectById(int projectId) = 0;
    virtual Project CreateProject(int userId, const std::string &projectName, const std::string &text) = 0;
    virtual bool RemoveProjectById(int projectId) = 0;
    virtual bool AddUserToProject(int userId, int projectId) = 0;
    virtual bool EditProject(const Project &project) = 0;
    virtual void DeleteUserFromProject(int projectId, size_t userId) = 0;
};
