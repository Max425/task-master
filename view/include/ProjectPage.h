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
#include <Wt/WComboBox.h>
#include <Wt/WStringListModel.h>

#include "Session.h"
#include "ProjectListItem.h"
#include "Project.hpp"
#include "Board.hpp"


class ProjectPage : public Wt::WTemplate
{
public:

  ProjectPage(Session& session, const Project& proj);

protected:
  virtual void renderTemplate(std::ostream& result);

private:
  Session& session_;
  Project proj_;
  std::vector<Board> boards_;

  User user_;
  std::vector<User> users_;
  Wt::WContainerWidget *boardsListWidget_;
  Wt::WPushButton* createButton_;
  Wt::WLineEdit* userNameEdit_;
  Wt::WComboBox *usersDropDown_;

  void bindAdmin();
  void addUser();
  void showBoards();
  void createBoard();
  void showBoard(const Board& board);

  using WWebWidget::render;
};