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

#include "Session.h"
#include "ProjectListItem.h"


class MainPage : public Wt::WTemplate
{
public:

  MainPage(Session& session);

protected:
  virtual void renderTemplate(std::ostream& result);

private:
  Session& session_;

  Wt::WContainerWidget *projects_;

  User user_;

  void showProjects();
  void showProject(const Project& proj);

  using WWebWidget::render;
};