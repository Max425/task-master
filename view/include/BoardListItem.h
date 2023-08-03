#pragma once

#include <Wt/WTemplate.h>

#include "Board.hpp"
#include "Session.h"

using namespace Wt;

namespace Wt {
  class WText;
}

class BoardListItem : public Wt::WTemplate
{
public:
  BoardListItem(Session& session, const Board& board);

protected:
  virtual void renderTemplate(std::ostream& result);

private:
  Session& session_;
  Board board_;
  Wt::WPushButton* editButton_;
  Wt::WPushButton* cancelButton_;
  Wt::WPushButton* deleteButton_;
  Wt::WAnchor* titleLabel_;
  WText* descriptionLabel_;
  WLineEdit* titleEdit_;
  WLineEdit* descriptionEdit_;
  bool editStatus_;
  int userId_;

  void bindAdmin();
  void DeleteBtnClicked();
  void EditBtnClicked();
  void CancelBtnClicked();

  using WWebWidget::render;
};