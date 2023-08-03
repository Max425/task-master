#pragma once

#include <Wt/WAnchor.h>
#include <Wt/WApplication.h>
#include <Wt/WCheckBox.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WEnvironment.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WStackedWidget.h>
#include <Wt/WTemplate.h>
#include <Wt/WText.h>
#include <Wt/Auth/PasswordService.h>
#include <Wt/Auth/PasswordVerifier.h>
#include <Wt/Dbo/backend/Postgres.h>

#include "Session.h"
#include "LoginWidget.h"
#include "ProjectPage.h"
#include "BoardPage.h"
#include "MyTasksPage.h"
#include "Errors.h"
#include <Wt/WCompositeWidget.h>
#include "DbDriverInterface.hpp"
#include "AuthUser.h"
#include "User.hpp"
#include "MainPage.h"

namespace Wt {
  class WContainerWidget;
}

class MainView;

class ViewImpl : public Wt::WContainerWidget
{
public:
  ViewImpl(const std::string &basePath, dbo::SqlConnectionPool &connectionPool,
           std::shared_ptr<DbDriverInterface> mainDb, MainView *blogView);

  void onUserChanged();
  Session &session();
  void setInternalBasePath(const std::string &basePath);
  ~ViewImpl();

private:
  std::string basePath_;
  Session session_;
  LoginWidget *loginWidget_;

  Wt::WStackedWidget *panel_;
  Wt::WTemplate *mustLoginWarning_;
  Wt::WTemplate *invalidUser_;
  Wt::WTemplate *loginStatus_;
  Wt::WContainerWidget *page_;

  void logout();
  void loggedOut();
  void loggedIn();
  void refresh();
  void handlePathChange(const std::string &path_);
  void showProject(Wt::WApplication *app, int userId);
  void showBoard(Wt::WApplication *app, int userId);
  bool checkLoggedIn();
};

class MainView : public Wt::WCompositeWidget
{
public:
  MainView(const std::string& basePath, Wt::Dbo::SqlConnectionPool& db, std::shared_ptr<DbDriverInterface> mainDb);

  void setInternalBasePath(const std::string& basePath);

  const User user();
  void login(const std::string& user);
  void logout();

  Wt::Signal<Wt::WString>& userChanged() { return userChanged_; }

private:
  ViewImpl *impl_;
  Wt::Signal<Wt::WString> userChanged_;
};
