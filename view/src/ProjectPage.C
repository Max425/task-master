#include "ProjectPage.h"
#include "BoardListItem.h"
#include <algorithm>
#include <Wt/WDialog.h>
#include <Wt/WApplication.h>
#include <Wt/WBreak.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WDialog.h>
#include <Wt/WEnvironment.h>
#include <Wt/WLabel.h>
#include <Wt/WLineEdit.h>
#include <Wt/WTextArea.h>
#include <Wt/WPushButton.h>
#include <Wt/WRegExpValidator.h>
#include <Wt/WText.h>
#include <string>

ProjectPage::ProjectPage(Session& session, const Project& proj):session_(session), proj_(proj)
{
    if( session_.user().Id == proj.CreatorId ){
        setTemplateText(tr("project-page-admin"));
        users_ = session_.userController().GetUsersNotInProject(proj_.Id);
        bindAdmin();
    }else
        setTemplateText(tr("project-page"));
    this->bindString("projectName", proj.Name);
    this->bindString("description", proj.Description);
    createButton_ = this->bindWidget("createBoardButton", std::make_unique<Wt::WPushButton>("Create board"));
    createButton_->clicked().connect(this, &ProjectPage::createBoard);
    createButton_->addStyleClass("btn btn-success");
    boardsListWidget_ = this->bindWidget("boards", std::make_unique<Wt::WContainerWidget>());
    boards_ = session_.projectController().GetAllBoards(proj.Id);
    user_ = session.user();
    showBoards();
}

void ProjectPage::bindAdmin()
{
    userNameEdit_ = this->bindWidget("userNameEdit", std::make_unique<Wt::WLineEdit>());
    usersDropDown_ = this->bindWidget("usersList", std::make_unique<Wt::WComboBox>());
    usersDropDown_->setNoSelectionEnabled(true);
    userNameEdit_->changed().connect([this]() {
        usersDropDown_->clear();    
        std::string text = userNameEdit_->text().toUTF8();
        int k = 0;
        for (size_t i = 0; i < users_.size(); i++) {
            if (users_[i].UserName.find(text) != std::string::npos) {
                usersDropDown_->insertItem(k++, users_[i].UserName);
            }
        }
    });
    auto inviteButton = this->bindWidget("inviveButton", std::make_unique<Wt::WPushButton>("Invite to project"));
    inviteButton->addStyleClass("btn");
    inviteButton->addStyleClass("btn-success");
    inviteButton->clicked().connect(this, &ProjectPage::addUser);
}

void ProjectPage::showBoards()
{
    for(auto board : boards_)
        showBoard(board);
}

void ProjectPage::createBoard()
{
    Wt::WDialog *dialog = this->addChild(std::make_unique<Wt::WDialog>("Create new boatd"));

    Wt::WLabel *titleLabel = dialog->contents()->addNew<Wt::WLabel>("Board title");
    Wt::WLineEdit *titleEdit = dialog->contents()->addNew<Wt::WLineEdit>();
    Wt::WLabel *descriptionLabel = dialog->contents()->addNew<Wt::WLabel>("Description");
    Wt::WTextArea *descriptionEdit = dialog->contents()->addNew<Wt::WTextArea>();

    titleLabel->setBuddy(titleEdit);
    descriptionLabel->setBuddy(descriptionEdit);

    dialog->contents()->addStyleClass("form-group");
    descriptionEdit->addStyleClass("form-control");
    titleEdit->addStyleClass("form-control");

    auto validator = std::make_shared<Wt::WRegExpValidator>("^(?!\\s*$).+");
    validator->setMandatory(true);
    titleEdit->setValidator(validator);
    descriptionEdit->setValidator(validator);

    Wt::WPushButton *ok =
        dialog->footer()->addNew<Wt::WPushButton>("Create");
    ok->addStyleClass("btn");
    ok->addStyleClass("btn-secondary");
    ok->setDefault(true);
    if (wApp->environment().ajax())
        ok->disable();

    Wt::WPushButton *cancel =
        dialog->footer()->addNew<Wt::WPushButton>("Cancel");
    cancel->addStyleClass("btn");
    cancel->addStyleClass("btn");
    dialog->rejectWhenEscapePressed();

    titleEdit->keyWentUp().connect([=] {
        ok->setDisabled(titleEdit->validate() != Wt::ValidationState::Valid);
    });

    descriptionEdit->keyWentUp().connect([=] {
        ok->setDisabled(descriptionEdit->validate() != Wt::ValidationState::Valid);
    });

    ok->clicked().connect([=] {
        if (titleEdit->validate() == Wt::ValidationState::Valid && descriptionEdit->validate() == Wt::ValidationState::Valid)
            dialog->accept();
    });

    cancel->clicked().connect(dialog, &Wt::WDialog::reject);

    dialog->finished().connect([=] {
        if (dialog->result() == Wt::DialogCode::Accepted) {
            if (titleEdit->validate() == Wt::ValidationState::Valid && descriptionEdit->validate() == Wt::ValidationState::Valid) {
                int id = session_.user().Id;
                auto name = titleEdit->text().toUTF8();
                auto text = descriptionEdit->text().toUTF8();
                Board newBoard = session_.projectController().CreateBoard(proj_.Id, id, name, text);
                showBoard(newBoard);
            }
        }
        this->removeChild(dialog);
    });

    dialog->show();
}

void ProjectPage::addUser()
{
    auto name = usersDropDown_->currentText().toUTF8();
    if(name.empty()) return;
    User user = session_.userController().GetUserByName(name);
    auto criteria = [&name](const User& user) {
        return user.UserName == name;
    };
    users_.erase(std::remove_if(users_.begin(), users_.end(), criteria), users_.end());
    
    session_.userController().AddProjectToUser(user.Id, proj_.Id);
    usersDropDown_->removeItem(usersDropDown_->currentIndex());
}

void ProjectPage::showBoard(const Board& board)
{
    boardsListWidget_->addWidget(std::make_unique<BoardListItem>(session_, board));
}


void ProjectPage::renderTemplate(std::ostream& result)
{
    WTemplate::renderTemplate(result);
}