#pragma once

#include <Wt/WTemplate.h>


#include "Session.h"
using namespace Wt;

namespace Wt {
  class WText;
}

class TaskListItem : public Wt::WTemplate
{
public:
  TaskListItem(Session& session, const Task& task);
  TaskListItem(Session& Session, int boardId);  //для создания новой таски
  void SetStatus(TaskStatus status);

protected:
  virtual void renderTemplate(std::ostream& result);

private:
  Session& session_;
  Task task_;
  std::vector<User> users_;
  Wt::WContainerWidget* usersListWidget_;
  bool editStatus_;
  bool new_;
  Wt::WPushButton* editButton_;
  Wt::WPushButton* cancelButton_;
  WText* titleLabel_;
  WText* descriptionLabel_;
  WLineEdit* titleEdit_;
  WLineEdit* descriptionEdit_;

  void ShowUsers();
  void EditBtnClicked();
  void CancelBtnClicked();

  using WWebWidget::render;
};