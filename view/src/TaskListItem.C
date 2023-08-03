#include "TaskListItem.h"

#include <Wt/WAnchor.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>
#include <Wt/WTextArea.h>

TaskListItem::TaskListItem(Session& session, const Task& task, bool canAssign): 
    session_(session), 
    task_(task),
    editStatus_(false),
    userStringModel_(nullptr)
{
    users_ = session_.userController().GetUsersForTask(task_.Id);
    if(canAssign){
        setTemplateText(tr("task-list-item-assign"));
        userSelectionBox_=this->bindWidget("ToAssign", std::make_unique<Wt::WComboBox>());
        userSelectionBox_->clicked().connect(this, &TaskListItem::LoadUsersToAssign);
        auto assignButton = this->bindWidget("AssignButton", std::make_unique<WPushButton>("Assign user"));
        assignButton->addStyleClass("btn");
        assignButton->addStyleClass("btn-success");
        assignButton->clicked().connect(this, &TaskListItem::AssignButtonClicked);
    }else{
        setTemplateText(tr("task-list-item"));
        User user = session.user();
        bool asigned = false;
        for(auto u : users_)
            if(u.Id == user.Id){
                asigned=true;
                break;
            }
        if(asigned)
            this->bindEmpty("AssignYourself");
        else{
            auto assignYourself = this->bindWidget("AssignYourself", std::make_unique<Wt::WText>("AssignYourself") );
            assignYourself->setStyleClass("link");
            assignYourself->clicked().connect(this, &TaskListItem::AssignYourself);
        }
    }

    titleLabel_ = this->bindWidget("title", std::make_unique<Wt::WText>(task.Name));
    titleEdit_ = this->bindWidget("nameInput", std::make_unique<WLineEdit>(task.Name));
    descriptionLabel_ = this->bindWidget("text", std::make_unique<Wt::WText>(task.Text));
    descriptionEdit_ = this->bindWidget("textInput", std::make_unique<WLineEdit>(task.Text));
    usersListWidget_ = this->bindWidget("AssignedUsers", std::make_unique<WContainerWidget>());
    ShowUsers();

    cancelButton_ = this->bindWidget("cancelButton", std::make_unique<WPushButton>("cancel"));
    cancelButton_->addStyleClass("btn");
    cancelButton_->addStyleClass("btn-success");
    cancelButton_->clicked().connect(this, &TaskListItem::CancelBtnClicked);

    deleteButton_ = this->bindWidget("deleteButton", std::make_unique<WPushButton>("delete"));
    deleteButton_->addStyleClass("btn");
    deleteButton_->addStyleClass("btn-danger");
    deleteButton_->clicked().connect(this, &TaskListItem::DeleteBtnClicked);

    editButton_ = this->bindWidget("editButton", std::make_unique<WPushButton>("edit"));
    editButton_->addStyleClass("btn");
    editButton_->addStyleClass("btn-success");
    editButton_->clicked().connect(this, &TaskListItem::EditBtnClicked);

    titleEdit_->hide();
    descriptionEdit_->hide();
    cancelButton_->hide();
    deleteButton_->hide();
}


void TaskListItem::DeleteBtnClicked()
{
    session_.taskController().DeleteTask(task_.Id);
    this->removeFromParent();
}

void TaskListItem::CancelBtnClicked()
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

void TaskListItem::LoadUsersToAssign()
{
    if(userStringModel_) return;
    userStringModel_ = std::make_shared<Wt::WStringListModel>();
    auto users = session_.userController().GetUsersNotInTask(session_.projectController().GetBoard(task_.BoardId).ProjectId, task_.Id);
    for (size_t i = 0; i < users.size(); i++)
    {
        userStringModel_->addString(users[i].UserName);
        userStringModel_->setData(i, 0, users[i].Id, Wt::ItemDataRole::User);
    }
    userSelectionBox_->setModel(userStringModel_);
    userSelectionBox_->setNoSelectionEnabled(true);
}

void TaskListItem::AssignButtonClicked(){
    if(userSelectionBox_->currentIndex() == -1) return;
    int cur = userSelectionBox_->currentIndex();
    AddUser( session_.userController().GetUser( Wt::asNumber( userStringModel_->data( userStringModel_->index(cur, 0), Wt::ItemDataRole::User ) ) ) ); 
    userStringModel_->removeRow(cur);
}

void TaskListItem::AssignYourself()
{
    AddUser(session_.user());
    this->bindEmpty("AssignYourself");
}

void TaskListItem::AddUser(const User& user){
    session_.userController().AddTaskToUser(user.Id, task_.Id);
    users_.push_back(user);
    auto widget = usersListWidget_->addWidget(std::make_unique<Wt::WTemplate>(tr("user-task-list-item")));
    widget->bindString("username", user.UserName);
}


void TaskListItem::EditBtnClicked()
{
    if(editStatus_){
        task_.Name = titleEdit_->text().toUTF8();
        task_.Text = descriptionEdit_->text().toUTF8();
        titleLabel_->setText(titleEdit_->text());
        descriptionLabel_->setText(descriptionEdit_->text());
        session_.taskController().EditTask(task_);
        CancelBtnClicked();
    }else{
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

void TaskListItem::ShowUsers()
{
    usersListWidget_->clear();
    for(auto user : users_){
        auto widget = usersListWidget_->addWidget(std::make_unique<Wt::WTemplate>(tr("user-task-list-item")));
        widget->bindString("username", user.UserName);
    }
}

void TaskListItem::SetStatus(TaskStatus status)
{
    task_.Status = status;
    auto tmp = session_.boardController().ChangeTaskStatus(task_.Id, status);
}


void TaskListItem::renderTemplate(std::ostream& result)
{
    WTemplate::renderTemplate(result);
}