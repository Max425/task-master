#include <iostream>
#include <memory>
#include "DbDriver.hpp"
#include "BoardRepo.hpp"
#include "BoardController.hpp"
#include "UserController.hpp"
#include "UserRepo.hpp"
#include "ProjectRepo.hpp"
#include "MainPadgeController.hpp"

int main()
{

    auto db = std::make_shared<DbDriver>("hostaddr=95.165.158.58 port=28009 dbname=TaskMaster user=umlaut-super password=");
    
    // auto rep = std::make_unique<ProjectRepo>(db);
    // MainPadgeController m(std::move(rep));
    // std::vector<Project> re = m.GetUserProjects(1);
    // for (const auto &project : re)
    // {
    //     std::cout << project.Name << std::endl;
    // }

    auto repo = std::make_unique<UserRepo>(db);
    UserController mp(std::move(repo));
    std::vector<User> res = mp.GetUsersNotInProject(1);
    for (const auto &user : res)
    {
        std::cout << user.UserName << std::endl;
    }
    return 0;
}