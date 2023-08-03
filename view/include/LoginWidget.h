#pragma once

#include <Wt/Auth/AuthWidget.h>
#include <Wt/Auth/RegistrationModel.h>

class Session;

/*
 * Displays login, logout and registration options
 */
class LoginWidget : public Wt::Auth::AuthWidget
{
public:
  LoginWidget(Session& session, const std::string& basePath);

  virtual void createLoginView() override;
  virtual void createLoggedInView() override;
};