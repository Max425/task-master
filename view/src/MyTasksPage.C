#include "MyTasksPage.h"
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

#include "TasksContainer.h"
#include "TaskListItem.h"

MyTasksPage::MyTasksPage(Session& session):session_(session), dragStatus(false)
{
    setTemplateText(tr("mytasks-page"));
    this->bindString("boardName", "Your tasks");
    this->bindString("description", "Tasks in which you are in the list of performers");
    int userId = session_.user().Id;
    tasks_ = session_.taskController().GetTasksForUser(userId);
    dragChangeButton_ = this->bindWidget("unlockDrag", std::make_unique<Wt::WPushButton>("Enable Drag"));
    dragChangeButton_->addStyleClass("btn");
    dragChangeButton_->addStyleClass("btn-success");
    dragChangeButton_->clicked().connect(this, &MyTasksPage::ChangeDrag);

    openListWidget_ = this->bindWidget("openTasks", std::make_unique<TasksContainer>(TaskStatus::Open));
    progressListWidget_ = this->bindWidget("inProgressTasks", std::make_unique<TasksContainer>(TaskStatus::InProgress));
    closedListWidget_ = this->bindWidget("closedTasks", std::make_unique<TasksContainer>(TaskStatus::Closed));
    showTasks();
}

void MyTasksPage::ChangeDrag()
{
    dragStatus = dragStatus ? false : true;
    if(dragStatus)
        dragChangeButton_->setText("Disable Drag");
    else
        dragChangeButton_->setText("Enable Drag");
    SetDrag(dragStatus);
}

void MyTasksPage::SetDrag(bool drag)
{
    for(auto c : openListWidget_->children())
    {
        auto child = static_cast<TaskListItem*>(c);
        if(drag)
            child->setDraggable("TaskListItem");
        else
            child->unsetDraggable();
    }
    for(auto c : progressListWidget_->children())
    {
        auto child = static_cast<TaskListItem*>(c);
        if(drag)
            child->setDraggable("TaskListItem");
        else
            child->unsetDraggable();
    }
    for(auto c : closedListWidget_->children())
    {
        auto child = static_cast<TaskListItem*>(c);
        if(drag)
            child->setDraggable("TaskListItem");
        else
            child->unsetDraggable();
    }
}

void MyTasksPage::showTasks()
{
    for(auto task : tasks_)
        showTask(task);
}

void MyTasksPage::showTask(const Task& task)
{
    switch (task.Status)
    {
    case TaskStatus::Open:
        openListWidget_->addWidget(std::make_unique<TaskListItem>(session_, task, false));
        break;
    case TaskStatus::InProgress:
        progressListWidget_->addWidget(std::make_unique<TaskListItem>(session_, task, false));
        break;
    case TaskStatus::Closed:
        closedListWidget_->addWidget(std::make_unique<TaskListItem>(session_, task, false));
        break;
    }
}


void MyTasksPage::renderTemplate(std::ostream& result)
{
    WTemplate::renderTemplate(result);
}

void MyTasksPage::showDialogAddTask()
{
    Wt::WDialog *dialog = this->addChild(std::make_unique<Wt::WDialog>("Create new task"));

    Wt::WLabel *nameLabel = dialog->contents()->addNew<Wt::WLabel>("Task name");
    Wt::WLineEdit *nameEdit = dialog->contents()->addNew<Wt::WLineEdit>();
    Wt::WLabel *descriptionLabel = dialog->contents()->addNew<Wt::WLabel>("Description");
    Wt::WTextArea *descriptionEdit = dialog->contents()->addNew<Wt::WTextArea>();

    nameLabel->setBuddy(nameEdit);
    descriptionLabel->setBuddy(descriptionEdit);

    dialog->contents()->addStyleClass("form-group");

    auto validator = std::make_shared<Wt::WRegExpValidator>("^(?!\\s*$).+");
    validator->setMandatory(true);
    nameEdit->setValidator(validator);
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

    nameEdit->keyWentUp().connect([=] {
        ok->setDisabled(nameEdit->validate() != Wt::ValidationState::Valid);
    });

    descriptionEdit->keyWentUp().connect([=] {
        ok->setDisabled(descriptionEdit->validate() != Wt::ValidationState::Valid);
    });


    ok->clicked().connect([=] {
        if (nameEdit->validate() == Wt::ValidationState::Valid && descriptionEdit->validate() == Wt::ValidationState::Valid)
            dialog->accept();
    });

    cancel->clicked().connect(dialog, &Wt::WDialog::reject);


    dialog->finished().connect([=] {
        if (dialog->result() == Wt::DialogCode::Accepted){
            Task task;
            // task.BoardId = board_.Id;
            task.Name = nameEdit->text().toUTF8();
            task.Text = descriptionEdit->text().toUTF8();
            task.Status = TaskStatus::Open;
            task = session_.taskController().CreateTask(task);
            openListWidget_->addWidget(std::make_unique<TaskListItem>(session_, task, false));
        }
        this->removeChild(dialog);
    });

    dialog->show();
}