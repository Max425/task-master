#include "UserRepo.hpp"
#include "Serialization.hpp"
#include <iostream>

bool UserRepo::EditUser(size_t userId, const std::string &userName)
{
    _dr->Exec("UPDATE \"user\" SET identity=\'" + userName + "\' WHERE id=" + std::to_string(userId) + ";");
    return true;
}
bool UserRepo::CreateUser(const std::string &userName)
{
    return true;
}

User UserRepo::GetUserByName(const std::string &userName)
{
    if (!_dr->Connected())
        throw std::runtime_error("Database is unavailable");
    auto answer = _dr->Exec("SELECT id, identity FROM \"user\" WHERE identity=\'" + userName + "\';");
    return serializationUser(answer[0]);
}

bool UserRepo::AddTaskToUser(int userId, int taskId)
{
    _dr->Exec("INSERT INTO task_users (user_id, task_id) VALUES (\'" + std::to_string(userId) + "\', \'"
              + std::to_string(taskId) + "\');");
    return true;
}
bool UserRepo::DeleteTaskFromUser(int userId, int taskId)
{
    _dr->Exec("DELETE FROM task_users WHERE user_id=" + std::to_string(userId)
              + " AND task_id=" + std::to_string(taskId) + ";");
    return true;
}
bool UserRepo::AddProjectToUser(int userId, int projectId)
{
    _dr->Exec("INSERT INTO project_users (user_id, project_id) VALUES (\'" + std::to_string(userId) + "\', \'"
              + std::to_string(projectId) + "\');");
    return true;
}
bool UserRepo::DeleteProjectFromUser(int userId, int projectId)
{
    _dr->Exec("DELETE FROM project_users WHERE user_id=" + std::to_string(userId)
              + " AND project_id=" + std::to_string(projectId) + ";");
    return true;
}
std::vector<User> UserRepo::GetUsersForTask(int taskId)
{
    auto answer = _dr->Exec("SELECT \"user\".id, \"user\".identity FROM \"user\" INNER JOIN task_users ON task_users.user_id=public.user.id WHERE task_users.task_id=" + std::to_string(taskId) + ";");
    std::vector<User> res;
    for (const auto &data : answer)
    {
        res.push_back(serializationUser(data));
    }
    return res;
}

std::vector<User> UserRepo::GetUsersForProject(int projectId)
{
    auto answer = _dr->Exec("SELECT \"user\".id, \"user\".identity FROM \"user\" INNER JOIN project_users ON project_users.user_id=public.user.id WHERE project_users.project_id="+ std::to_string(projectId) + ";");
    std::vector<User> res;
    for (const auto &data : answer)
    {
        res.push_back(serializationUser(data));
    }
    return res;
}
std::vector<User> UserRepo::GetUsersNotInTask(int projectId, int taskId)
{
    auto answer = _dr->Exec("SELECT public.user.id, public.user.identity FROM public.user INNER JOIN project_users ON project_users.user_id=public.user.id WHERE project_users.project_id="
                             + std::to_string(projectId) + " AND public.user.id NOT IN (SELECT user_id FROM task_users WHERE task_users.task_id=" + std::to_string(taskId) + ");");
    if (answer.size() == 0)
        return std::vector<User>();
    std::vector<User> res;
    for (const auto &data : answer)
    {
        res.push_back(serializationUser(data));
    }
    return res;
}
std::vector<User> UserRepo::GetUsersNotInProject(int projectId)
{
    auto answer = _dr->Exec("SELECT public.user.id, public.user.identity FROM public.user WHERE public.user.id NOT IN (SELECT user_id FROM project_users WHERE project_users.project_id=" + std::to_string(projectId) + ");");
    if (answer.size() == 0)
        return std::vector<User>();
    std::vector<User> res;
    for (const auto &data : answer)
    {
        res.push_back(serializationUser(data));
    }
    return res;
}
std::vector<User> UserRepo::GetUsers()
{
    auto answer = _dr->Exec("SELECT public.user.id, public.user.identity FROM public.user;");
    if (answer.size() == 0)
        throw std::runtime_error("Database has no \"user\"");
    std::vector<User> res;
    for (const auto &data : answer)
    {
        res.push_back(serializationUser(data));
    }
    return res;
}

User UserRepo::GetUser(int userId)
{
    if (!_dr->Connected())
        throw std::runtime_error("Database is unavailable");
    auto answer = _dr->Exec("SELECT public.user.id, public.user.identity FROM \"user\" WHERE id=" + std::to_string(userId) + ";");
    if (answer.size() == 0)
        throw std::runtime_error("Users with userId=" + std::to_string(userId) + " not found");
    return serializationUser(answer[0]);
}

User UserRepo::GetUserByIdentity(int userIdentityId)
{
    if (!_dr->Connected())
        throw std::runtime_error("Database is unavailable");
    auto answer = _dr->Exec("SELECT id, identity FROM \"user\" WHERE auth_info_id = " + std::to_string(userIdentityId) + ";");
    if (answer.size() == 0)
        throw std::runtime_error("Users with auth_info_id=" + std::to_string(userIdentityId) + " not found");
    return serializationUser(answer[0]);
}

bool UserRepo::CreateUserWithIdentity(int userIdentityId)
{
    return true;
}

