#pragma once

#include <Wt/WTemplate.h>
#include <Wt/WComboBox.h>
#include <Wt/WStringListModel.h>

#include "Session.h"
using namespace Wt;

namespace Wt {
  class WText;
}

class ProjectListItem : public Wt::WTemplate
{
public:
  ProjectListItem(Session& session, const Project& proj);

protected:
  virtual void renderTemplate(std::ostream& result);

private:
  Session& session_;
  Project project_;
  Wt::WPushButton* editButton_;
  Wt::WPushButton* cancelButton_;
  Wt::WPushButton* deleteButton_;
  Wt::WAnchor* titleLabel_;
  WText* descriptionLabel_;
  WLineEdit* titleEdit_;
  WLineEdit* descriptionEdit_;
  bool editStatus_;
  int userId_;

  void DeleteBtnClicked();
  void EditBtnClicked();
  void CancelBtnClicked();

  using WWebWidget::render;
};