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
#include "Board.hpp"
#include "Task.hpp"


class MyTasksPage : public Wt::WTemplate
{
public:

  MyTasksPage(Session& session);
  void ChangeDrag();

protected:
  virtual void renderTemplate(std::ostream& result);

private:
  Session& session_;
  std::vector<Task> tasks_;
  bool dragStatus;

  User user_;

  Wt::WContainerWidget *openListWidget_;
  Wt::WContainerWidget *closedListWidget_;
  Wt::WContainerWidget *progressListWidget_;
  Wt::WPushButton* dragChangeButton_;

  void showDialogAddTask();
  void showTasks();
  void showTask(const Task& task);
  void SetDrag(bool drag);

  using WWebWidget::render;
};