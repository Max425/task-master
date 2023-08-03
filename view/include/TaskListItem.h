#pragma once

#include <Wt/WTemplate.h>
#include <Wt/WComboBox.h>
#include <Wt/WStringListModel.h>


#include "Session.h"
using namespace Wt;

namespace Wt {
  class WText;
}

class TaskListItem : public Wt::WTemplate
{
public:
  TaskListItem(Session& session, const Task& task, bool canAssign);
  void SetStatus(TaskStatus status);

protected:
  virtual void renderTemplate(std::ostream& result);

private:
  Session& session_;
  Task task_;
  std::vector<User> users_;
  Wt::WContainerWidget* usersListWidget_;
  bool editStatus_;
  Wt::WPushButton* editButton_;
  Wt::WPushButton* cancelButton_;
  Wt::WPushButton* deleteButton_;
  WText* titleLabel_;
  WText* descriptionLabel_;
  WLineEdit* titleEdit_;
  WLineEdit* descriptionEdit_;

  Wt::WComboBox* userSelectionBox_;
  std::shared_ptr<Wt::WStringListModel> userStringModel_;
  
  void DeleteBtnClicked();
  void AssignYourself();
  void AddUser(const User& user);
  void ShowUsers();
  void EditBtnClicked();
  void CancelBtnClicked();
  void LoadUsersToAssign();
  void AssignButtonClicked();

  using WWebWidget::render;
};