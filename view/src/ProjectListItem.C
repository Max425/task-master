#include "ProjectListItem.h"

#include <Wt/WAnchor.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>
#include <Wt/WTextArea.h>

ProjectListItem::ProjectListItem(Session& session, const Project& proj) : session_(session), project_(proj), editStatus_(false)
{
    userId_ = session_.userController().GetUserByIdentity(session_.user().Id).Id;
    setTemplateText(tr("project"));

    titleLabel_ = this->bindWidget("title" ,std::make_unique<Wt::WAnchor>(Wt::WLink(Wt::LinkType::InternalPath, "/project/" + std::to_string(proj.Id)), proj.Name));
    descriptionLabel_ = this->bindWidget("description", std::make_unique<Wt::WText>(proj.Description));
    
    titleEdit_ = this->bindWidget("nameInput", std::make_unique<WLineEdit>(proj.Name));
    descriptionEdit_ = this->bindWidget("textInput", std::make_unique<WLineEdit>(proj.Description));

    cancelButton_ = this->bindWidget("cancelButton", std::make_unique<WPushButton>("cancel"));
    cancelButton_->addStyleClass("btn");
    cancelButton_->addStyleClass("btn-success");
    cancelButton_->clicked().connect(this, &ProjectListItem::CancelBtnClicked);
    
    deleteButton_ = this->bindWidget("deleteButton", std::make_unique<WPushButton>(project_.CreatorId == userId_ ? "delete project" : "leave project"));
    deleteButton_->addStyleClass("btn");
    deleteButton_->addStyleClass("btn-danger");
    deleteButton_->clicked().connect(this, &ProjectListItem::DeleteBtnClicked);
    
    editButton_ = this->bindWidget("editButton", std::make_unique<WPushButton>("edit"));
    editButton_->addStyleClass("btn");
    editButton_->addStyleClass("btn-success");
    editButton_->clicked().connect(this, &ProjectListItem::EditBtnClicked);
    
    titleEdit_->hide();
    descriptionEdit_->hide();
    cancelButton_->hide();
    deleteButton_->hide();
}

void ProjectListItem::DeleteBtnClicked()
{
    if (project_.CreatorId == userId_) {
        session_.mainPadgeController().DeleteProject(project_);
    } else {
        session_.mainPadgeController().DeleteUserFromProject(project_, userId_);
    }
    this->removeFromParent();
}

void ProjectListItem::EditBtnClicked()
{
    if (editStatus_) {
        project_.Name = titleEdit_->text().toUTF8();
        project_.Description = descriptionEdit_->text().toUTF8();
        titleLabel_->setText(titleEdit_->text());
        descriptionLabel_->setText(descriptionEdit_->text());
        session_.mainPadgeController().EditProject(project_);
        CancelBtnClicked();
    } else if (userId_ == project_.CreatorId) {
        editButton_->setText("save");
        titleLabel_->hide();
        descriptionLabel_->hide();
        titleEdit_->show();
        deleteButton_->show();
        descriptionEdit_->show();
        cancelButton_->show();
        editStatus_ = true;
    } else {
        editButton_->setText("cancel");
        deleteButton_->show();
        editStatus_ = true;
    }
}

void ProjectListItem::CancelBtnClicked()
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


void ProjectListItem::renderTemplate(std::ostream& result)
{
    WTemplate::renderTemplate(result);
}
