#include "Session.h"

#include <Wt/Auth/AuthService.h>
#include <Wt/Auth/HashFunction.h>
#include <Wt/Auth/PasswordService.h>
#include <Wt/Auth/PasswordStrengthValidator.h>
#include <Wt/Auth/PasswordVerifier.h>
#include <Wt/Auth/Dbo/AuthInfo.h>

#include <Wt/Dbo/backend/Postgres.h>
#include <Wt/Dbo/FixedSqlConnectionPool.h>

using namespace Wt;

namespace
{

  Auth::AuthService AuthService;
  Auth::PasswordService PasswordService(AuthService);

}

void Session::configureAuth()
{
  AuthService.setAuthTokensEnabled(true, "logincookie");

  std::unique_ptr<Auth::PasswordVerifier> verifier = std::make_unique<Auth::PasswordVerifier>();
  verifier->addHashFunction(std::make_unique<Auth::BCryptHashFunction>(7));
  PasswordService.setVerifier(std::move(verifier));
  PasswordService.setAttemptThrottlingEnabled(true);
  PasswordService.setStrengthValidator(std::make_unique<Auth::PasswordStrengthValidator>());
}

std::unique_ptr<dbo::SqlConnectionPool> Session::createConnectionPool(const std::string &pgsql)
{
  auto connection = std::make_unique<dbo::backend::Postgres>(pgsql);

  connection->setProperty("show-queries", "true");

  return std::make_unique<dbo::FixedSqlConnectionPool>(std::move(connection), 10);
}

Session::Session(dbo::SqlConnectionPool &connectionPool, std::shared_ptr<DbDriverInterface> mainDb) : connectionPool_(connectionPool),
                                                                                                      users_(*this),
                                                                                                      mainPage_(std::make_unique<MainPadgeController>(std::make_unique<ProjectRepo>(mainDb))),
                                                                                                      userCont_(std::make_unique<UserController>(std::make_unique<UserRepo>(mainDb))),
                                                                                                      boardCont_(std::make_unique<BoardController>(std::make_unique<TaskRepo>(mainDb))),
                                                                                                      taskCont_(std::make_unique<TaskController>(std::make_unique<TaskRepo>(mainDb))),
                                                                                                      projCont_(std::make_unique<ProjectController>(std::make_unique<ProjectRepo>(mainDb), std::make_unique<BoardRepo>(mainDb), std::make_unique<UserRepo>(mainDb)))
{
  setConnectionPool(connectionPool_);
  mapClass<AuthUser>("auth_user");
  mapClass<AuthInfo>("auth_info");
  mapClass<AuthInfo::AuthIdentityType>("user");
  mapClass<AuthInfo::AuthTokenType>("auth_token");

  try
  {
    createTables();
    std::cerr << "Created database." << std::endl;
  }
  catch (std::exception &e)
  {
    std::cerr << e.what() << std::endl;
    std::cerr << "Using existing database";
  }
}

const User Session::user()
{
  if (login_.loggedIn())
  {
    auto id = stoi(login_.user().id());
    try
    {
      return userController().GetUserByIdentity(id);
    }
    catch (std::runtime_error &e)
    {
      try
      {
        return userController().CreateUserWithIdentity(id);
      }
      catch (std::runtime_error &e)
      {
        User rez;
        rez.Id = 0;
        return rez;
      }
    }
  }
  User rez;
  rez.Id = 0;
  return rez;
}

const Auth::AuthService &Session::auth()
{
  return AuthService;
}

const Auth::PasswordService &Session::passwordAuth()
{
  return PasswordService;
}