#include "LoginWidget.h"

#include <Wt/WLineEdit.h>
#include <Wt/WTemplate.h>
#include <Wt/WText.h>
#include <Wt/Auth/PasswordService.h>
#include <Wt/Auth/RegistrationWidget.h>

#include "Session.h"

LoginWidget::LoginWidget(Session& session,
                                 const std::string& basePath)
  : AuthWidget(session.login())
{
  setInline(true);

  auto model
    = std::make_unique<Wt::Auth::AuthModel>(session.passwordAuth().baseAuth(),
                          session.users());
  model->addPasswordAuth(&session.passwordAuth());

  setModel(std::move(model));

  setInternalBasePath(basePath + "login");
}

void LoginWidget::createLoginView()
{
  AuthWidget::createLoginView();

  setTemplateText(tr("blog-login"));

  Wt::WLineEdit *userName = resolve<Wt::WLineEdit *>("user-name");
  userName->setPlaceholderText("login");
  userName->setToolTip("login");

  Wt::WLineEdit *password = resolve<Wt::WLineEdit *>("password");
  password->setPlaceholderText("password");
  password->setToolTip("password");
}

void LoginWidget::createLoggedInView()
{
  AuthWidget::createLoggedInView();

  auto logout = std::make_unique<Wt::WText>(tr("logout"));
  logout->setStyleClass("link");
  logout->clicked().connect(&login(), &Wt::Auth::Login::logout);
  bindWidget("logout", std::move(logout));
}
