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


class BoardPage : public Wt::WTemplate
{
public:

  BoardPage(Session& session, const Board& board);
  void ChangeDrag();

protected:
  virtual void renderTemplate(std::ostream& result);

private:
  Session& session_;
  Board board_;
  std::vector<Task> tasks_;
  bool dragStatus;
  bool isCreator;

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