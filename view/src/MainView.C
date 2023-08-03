#include "MainView.h"
#include "ImageWidget.h"
#include <Wt/WApplication.h>
#include <Wt/WMessageBox.h>

ViewImpl::ViewImpl(const std::string &basePath, dbo::SqlConnectionPool &connectionPool,
                   std::shared_ptr<DbDriverInterface> mainDb, MainView *blogView)
    : basePath_(basePath),
      session_(connectionPool, mainDb),
      loginWidget_(nullptr),
      panel_(nullptr),
      mustLoginWarning_(nullptr),
      invalidUser_(nullptr),
      page_(nullptr)
{
  Wt::WApplication *app = Wt::WApplication::instance();

  app->messageResourceBundle().use(Wt::WApplication::appRoot() + "templates");
  app->useStyleSheet("css/style.css");
  app->useStyleSheet("css/bootstrap.css");
  app->useStyleSheet("css/wt.css");
  app->useStyleSheet("css/form.css");
  app->useStyleSheet("css/webkit-transitions.css");
  app->internalPathChanged().connect(this, &ViewImpl::handlePathChange);

  loginStatus_ = this->addWidget(std::make_unique<Wt::WTemplate>(tr("blog-login-status")));
  panel_ = this->addWidget(std::make_unique<Wt::WStackedWidget>());
  page_ = this->addWidget(std::make_unique<Wt::WContainerWidget>());
  session_.login().changed().connect(this, &ViewImpl::onUserChanged);

  auto loginWidget = std::make_unique<LoginWidget>(session_, basePath);
  loginWidget_ = loginWidget.get();
  loginWidget_->hide();

  auto loginLink = std::make_unique<Wt::WText>(tr("login"));
  auto lPtr = loginLink.get();
  loginLink->setStyleClass("link");
  loginLink->clicked().connect(loginWidget_, &WWidget::show);
  loginLink->clicked().connect(lPtr, &WWidget::hide);

  auto registerLink = std::make_unique<Wt::WText>(tr("Wt.Auth.register"));
  registerLink->setStyleClass("link");
  registerLink->clicked().connect(loginWidget_,
                                  &LoginWidget::registerNewUser);

  loginStatus_->bindWidget("login", std::move(loginWidget));
  loginStatus_->bindWidget("login-link", std::move(loginLink));
  loginStatus_->bindWidget("register-link", std::move(registerLink));

  setInternalBasePath(basePath_);

  onUserChanged();

  loginWidget_->processEnvironment();
}

void ViewImpl::onUserChanged()
{
  session_.login().loggedIn() ? loggedIn() : loggedOut();
  session_.user();
}

Session &ViewImpl::session()
{
  return session_;
}

void ViewImpl::setInternalBasePath(const std::string &basePath)
{
  basePath_ = basePath;
  refresh();
}

ViewImpl::~ViewImpl()
{
  clear();
}

void ViewImpl::logout()
{
  session_.login().logout();
}

void ViewImpl::loggedOut()
{
  loginStatus_->bindEmpty("profile-link");
  loginStatus_->bindEmpty("author-panel-link");
  loginStatus_->bindEmpty("userlist-link");

  loginStatus_->resolveWidget("login")->hide();
  loginStatus_->resolveWidget("login-link")->show();
  loginStatus_->resolveWidget("register-link")->show();

  refresh();
  panel_->hide();
  page_->clear();
  page_->addWidget(std::make_unique<ImageWidget>());
}

void ViewImpl::loggedIn()
{
  if (int(session_.user().Id) == 0)
  {
    session_.login().logout();
    Wt::WMessageBox::show("Account Created", "Your account has been created. Please log in to authorize.",
                          Wt::StandardButton::Ok);
    return;
  }
  Wt::WApplication::instance()->changeSessionId();
  refresh();
  loginStatus_->resolveWidget("login")->show();
  loginStatus_->resolveWidget("login-link")->hide();
  loginStatus_->resolveWidget("register-link")->hide();

  auto profileLink = std::make_unique<Wt::WText>(tr("profile"));
  profileLink->setStyleClass("link");

  loginStatus_->bindWidget("profile-link", std::move(profileLink));
}

void ViewImpl::refresh()
{
  handlePathChange(Wt::WApplication::instance()->internalPath());
}

void ViewImpl::handlePathChange(const std::string &path_)
{
  Wt::WApplication *app = Wt::WApplication::instance();
  int userId = session_.user().Id;
  if (!app->internalPathMatches(basePath_) || userId == 0) {
    page_->addWidget(std::make_unique<ImageWidget>());
    return;
  }

  std::string path = app->internalPathNextPart(basePath_); 
  page_->clear();
  if (path.empty()) 
  {
    page_->addWidget(std::make_unique<MainPage>(session_));
  } else if (path == "project") 
  {
    showProject(app, userId);
  } else if (path == "board") 
  {
    showBoard(app, userId);
  } else if (path == "mytasks") 
  {
    page_->addWidget(std::make_unique<MyTasksPage>(session_));
  } else 
  {
    page_->addWidget(std::make_unique<Page404>());
  }
}

void ViewImpl::showProject(Wt::WApplication *app, int userId)
{
  int projId = std::stoi(app->internalPathNextPart("/project/"));
  std::vector<User> projectUsers;
  try
  {
    projectUsers = session_.userController().GetUsersForProject(projId);
  }
  catch (...)
  {
    page_->addWidget(std::make_unique<Page404>());
    return;
  }
  auto userWithId = std::find_if(projectUsers.begin(), projectUsers.end(), [userId](const User &user) {
    return user.Id == userId;
  });
  if (userWithId != projectUsers.end())
  {
    page_->clear();
    page_->addWidget(std::make_unique<ProjectPage>(session_, session_.mainPadgeController().GetProjectById(projId)));
  }
  else
  {
    page_->addWidget(std::make_unique<Page403>());
    return;
  }
}

void ViewImpl::showBoard(Wt::WApplication *app, int userId)
{
    int boardId = std::stoi(app->internalPathNextPart("/board/"));
    std::vector<User> projectUsers;
    try
    {
      int projId = session_.projectController().GetBoard(boardId).ProjectId;
      projectUsers = session_.userController().GetUsersForProject(projId);
    }
    catch (...)
    {
      page_->addWidget(std::make_unique<Page404>());
      return;
    }
    auto userWithId = std::find_if(projectUsers.begin(), projectUsers.end(), [userId](const User &user) {
      return user.Id == userId;
    });
    if (userWithId != projectUsers.end())
    {
      page_->clear();
      page_->addWidget(std::make_unique<BoardPage>(session_, session_.projectController().GetBoard(boardId)));
    }
    else
    {
      page_->addWidget(std::make_unique<Page403>());
      return;
    }
}

bool ViewImpl::checkLoggedIn() { return true; }


MainView::MainView(const std::string &basePath, Wt::Dbo::SqlConnectionPool &db, std::shared_ptr<DbDriverInterface> mainDb)
    : WCompositeWidget(),
      userChanged_()
{
  impl_ = setImplementation(std::make_unique<ViewImpl>(basePath, db, mainDb, this));
}

void MainView::setInternalBasePath(const std::string &basePath)
{
  impl_->setInternalBasePath(basePath);
}

const User MainView::user()
{
  return impl_->session().user();
}
