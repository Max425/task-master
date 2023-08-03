#include "BoardListItem.h"

#include <Wt/WAnchor.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>
#include <Wt/WTextArea.h>

BoardListItem::BoardListItem(Session& session, const Board& board) : session_(session), board_(board)
{
    int userId = session_.user().Id;
    Project project = session_.mainPadgeController().GetProjectById(board_.ProjectId);
    if (userId == board_.CreatorId || userId == project.CreatorId) {
        setTemplateText(tr("board-list-item-admin"));
        bindAdmin();
    } else {
        setTemplateText(tr("board-list-item"));
    }

    this->bindWidget("creator", std::make_unique<Wt::WText>(session_.userController().GetUser(board.CreatorId).UserName));
    titleLabel_ = this->bindWidget("title" ,std::make_unique<Wt::WAnchor>(Wt::WLink(Wt::LinkType::InternalPath, "/board/" + std::to_string(board.Id)), board.Name));
    descriptionLabel_ = this->bindWidget("description", std::make_unique<Wt::WText>(board.Text));
}

void BoardListItem::bindAdmin()
{
    titleEdit_ = this->bindWidget("nameInput", std::make_unique<WLineEdit>(board_.Name));
    descriptionEdit_ = this->bindWidget("textInput", std::make_unique<WLineEdit>(board_.Text));

    cancelButton_ = this->bindWidget("cancelButton", std::make_unique<WPushButton>("cancel"));
    cancelButton_->addStyleClass("btn");
    cancelButton_->addStyleClass("btn-success");
    cancelButton_->clicked().connect(this, &BoardListItem::CancelBtnClicked);
    
    deleteButton_ = this->bindWidget("deleteButton", std::make_unique<WPushButton>("delete board"));
    deleteButton_->addStyleClass("btn");
    deleteButton_->addStyleClass("btn-danger");
    deleteButton_->clicked().connect(this, &BoardListItem::DeleteBtnClicked);
    
    editButton_ = this->bindWidget("editButton", std::make_unique<WPushButton>("edit"));
    editButton_->addStyleClass("btn");
    editButton_->addStyleClass("btn-success");
    editButton_->clicked().connect(this, &BoardListItem::EditBtnClicked);
    
    titleEdit_->hide();
    descriptionEdit_->hide();
    cancelButton_->hide();
    deleteButton_->hide();
}

void BoardListItem::DeleteBtnClicked()
{
    session_.projectController().DeleteBoard(board_.Id);
    this->removeFromParent();
}

void BoardListItem::EditBtnClicked()
{
    if (editStatus_) {
        board_.Name = titleEdit_->text().toUTF8();
        board_.Text = descriptionEdit_->text().toUTF8();
        titleLabel_->setText(titleEdit_->text());
        descriptionLabel_->setText(descriptionEdit_->text());
        session_.projectController().EditBoard(board_);
        CancelBtnClicked();
    }  else {
        editButton_->setText("save");
        titleLabel_->hide();
        descriptionLabel_->hide();
        titleEdit_->show();
        deleteButton_->show();
        descriptionEdit_->show();
        cancelButton_->show();
        editStatus_ = true;
    }
}

void BoardListItem::CancelBtnClicked()
{
    descriptionLabel_->show();
    titleLabel_->show();
    titleEdit_->hide();
    descriptionEdit_->hide();
    cancelButton_->hide();
    deleteButton_->hide();
    editButton_->setText("edit");
    editStatus_ = false;
}

void BoardListItem::renderTemplate(std::ostream& result)
{
    WTemplate::renderTemplate(result);
}