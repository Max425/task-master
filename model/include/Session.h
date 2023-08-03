#pragma once

#include <Wt/Auth/Login.h>
#include <Wt/Auth/Dbo/UserDatabase.h>

#include <Wt/Dbo/Session.h>
#include <Wt/Dbo/ptr.h>
#include "DbDriverInterface.hpp"
#include "BoardController.hpp"
#include "MainPadgeController.hpp"
#include "ProjectController.hpp"
#include "TaskController.hpp"
#include "UserController.hpp"
#include "ProjectRepo.hpp"
#include "UserRepo.hpp"
#include "TaskRepo.hpp"
#include "BoardRepo.hpp"


#include "AuthUser.h"
#include "User.hpp"

namespace dbo = Wt::Dbo;

using UserDatabase = Wt::Auth::Dbo::UserDatabase<AuthInfo>;

/*
добавить сигналы смены 
*/

class Session : public dbo::Session
{
public:
  static void configureAuth();

  Session(dbo::SqlConnectionPool& connectionPool, std::shared_ptr<DbDriverInterface> mainDb);

  const User user();

  UserDatabase& users() { return users_; }
  Wt::Auth::Login& login() { return login_; }

  static const Wt::Auth::AuthService& auth();
  static const Wt::Auth::PasswordService& passwordAuth();

  static std::unique_ptr<dbo::SqlConnectionPool> createConnectionPool(const std::string& pgsql);

  MainPadgeHandlerInterface& mainPadgeController() { return *mainPage_; }
  UserHandlerInterface& userController() { return *userCont_; }
  BoardHandlerInterface& boardController() { return *boardCont_; }
  TaskHandlerInterface& taskController() { return *taskCont_; }
  ProjectHandlerInterface& projectController() { return *projCont_; }

private:
  dbo::SqlConnectionPool&     connectionPool_;
  UserDatabase users_;
  Wt::Auth::Login login_;
  std::unique_ptr<MainPadgeHandlerInterface> mainPage_;
  std::unique_ptr<UserHandlerInterface> userCont_;
  std::unique_ptr<BoardHandlerInterface> boardCont_;
  std::unique_ptr<TaskHandlerInterface> taskCont_;
  std::unique_ptr<ProjectHandlerInterface> projCont_;
};