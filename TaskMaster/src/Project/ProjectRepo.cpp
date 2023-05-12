#include <iostream>
#include "string"
#include "ProjectRepo.hpp"
#include "DbDriver.hpp"
#include "Serialization.hpp"

ProjectRepo::ProjectRepo(DbDriverSPtr dr) : dr_(dr) {}

std::vector<Project> ProjectRepo::GetUserProjects(int userId)
{
    auto answer = dr_->Exec("SELECT * FROM db_project WHERE creator_id=" + std::to_string(userId));
    std::vector<Project> res;
    for (const auto &data : answer)
    {
        res.push_back(serializationProject(data));
    }
    return res;
}

Project ProjectRepo::GetProjectById(int projectId)
{
    auto answer = dr_->Exec("SELECT * FROM db_project WHERE id=" + std::to_string(projectId));
    return serializationProject(answer[0]);
}

bool ProjectRepo::CreateProject(int creatorId, const std::string &projectName)
{
    dr_->Exec("INSERT INTO db_project (name, creator_Id) VALUES (\'" + projectName + "\'," + std::to_string(creatorId) + ")");
    return true;
}

bool ProjectRepo::RemoveProjectById(int projectId)
{
    dr_->Exec("DELETE FROM db_project WHERE id = " + std::to_string(projectId));
    return true;
}

bool ProjectRepo::AddUserToProject(int projectId, const std::string &userName)
{
    return true;
}

bool ProjectRepo::UpdateProject(const Project &project)
{
    return true;
}
