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

namespace {

  Auth::AuthService AuthService;
  Auth::PasswordService PasswordService(AuthService);

}

void Session::configureAuth()
{
  AuthService.setAuthTokensEnabled(true, "logincookie");

  std::unique_ptr<Auth::PasswordVerifier> verifier
      = std::make_unique<Auth::PasswordVerifier>();
  verifier->addHashFunction(std::make_unique<Auth::BCryptHashFunction>(7));
  PasswordService.setVerifier(std::move(verifier));
  PasswordService.setAttemptThrottlingEnabled(true);
  PasswordService.setStrengthValidator(std::make_unique<Auth::PasswordStrengthValidator>());
}

std::unique_ptr<dbo::SqlConnectionPool> Session::createConnectionPool(const std::string& pgsql)
{
  auto connection = std::make_unique<dbo::backend::Postgres>(pgsql);

  connection->setProperty("show-queries", "true");

  return std::make_unique<dbo::FixedSqlConnectionPool>(std::move(connection), 10);
}

Session::Session(dbo::SqlConnectionPool& connectionPool, std::shared_ptr<DbDriverInterface> mainDb):
    connectionPool_(connectionPool),
    users_(*this)
{
  setConnectionPool(connectionPool_);

  mapClass<AuthUser>("auth_user");
  mapClass<AuthInfo>("auth_info");
  mapClass<AuthInfo::AuthIdentityType>("auth_identity");
  mapClass<AuthInfo::AuthTokenType>("auth_token");

  try {
    createTables();
    std::cerr << "Created database." << std::endl;
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
    std::cerr << "Using existing database";
  }
}

dbo::ptr<AuthUser> Session::user() const
{
  if (login_.loggedIn()) {
    dbo::ptr<AuthInfo> authInfo = users_.find(login_.user());
    return authInfo->user();
  } else
    return dbo::ptr<AuthUser>();
}

const Auth::AuthService& Session::auth()
{
  return AuthService;
}

const Auth::PasswordService& Session::passwordAuth()
{
  return PasswordService;
}