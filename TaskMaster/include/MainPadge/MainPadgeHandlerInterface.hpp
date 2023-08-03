#pragma once
#include <vector>
#include <string>
#include "Project.hpp"

class MainPadgeHandlerInterface
{
public:
    virtual ~MainPadgeHandlerInterface() {}
    virtual std::vector<Project> GetUserProjects(size_t userId) = 0;
    virtual Project GetProjectById(size_t projectId) = 0;
    virtual Project CreateProject(size_t userId, const std::string &projectName, const std::string &text) = 0;
    virtual void DeleteProject(const Project &project) = 0;
    virtual void DeleteUserFromProject(const Project &project, size_t userId) = 0;
    virtual void EditProject(const Project &project) = 0;
};

