#include "BoardPage.h"
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

BoardPage::BoardPage(Session& session, const Board& board):session_(session), board_(board), dragStatus(false)
{
    setTemplateText(tr("board-page"));
    this->bindString("boardName", board.Name);
    this->bindString("description", board.Text);
    this->bindWidget("creator", std::make_unique<Wt::WText>(session_.userController().GetUser(board.CreatorId).UserName));
    user_ = session_.user();
    isCreator = user_.Id == board_.CreatorId;
    tasks_ = session_.boardController().GetAllTasksForBoard(board_.Id);
    dragChangeButton_ = this->bindWidget("unlockDrag", std::make_unique<Wt::WPushButton>("Enable Drag"));
    dragChangeButton_->addStyleClass("btn");
    dragChangeButton_->addStyleClass("btn-success");
    dragChangeButton_->clicked().connect(this, &BoardPage::ChangeDrag);

    auto createTaskButton = this->bindWidget("createTask", std::make_unique<Wt::WPushButton>("Create Task"));
    createTaskButton->addStyleClass("btn");
    createTaskButton->addStyleClass("btn-success");
    createTaskButton->clicked().connect(this, &BoardPage::showDialogAddTask);

    openListWidget_ = this->bindWidget("openTasks", std::make_unique<TasksContainer>(TaskStatus::Open));
    progressListWidget_ = this->bindWidget("inProgressTasks", std::make_unique<TasksContainer>(TaskStatus::InProgress));
    closedListWidget_ = this->bindWidget("closedTasks", std::make_unique<TasksContainer>(TaskStatus::Closed));
    
    showTasks();
}

void BoardPage::ChangeDrag()
{
    dragStatus = dragStatus ? false : true;
    if(dragStatus)
        dragChangeButton_->setText("Disable Drag");
    else
        dragChangeButton_->setText("Enable Drag");
    SetDrag(dragStatus);
}

void BoardPage::SetDrag(bool drag)
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

void BoardPage::showTasks()
{
    for(auto task : tasks_)
        showTask(task);
}

void BoardPage::showTask(const Task& task)
{
    switch (task.Status)
    {
    case TaskStatus::Open:
        openListWidget_->addWidget(std::make_unique<TaskListItem>(session_, task, isCreator));
        break;
    case TaskStatus::InProgress:
        progressListWidget_->addWidget(std::make_unique<TaskListItem>(session_, task, isCreator));
        break;
    case TaskStatus::Closed:
        closedListWidget_->addWidget(std::make_unique<TaskListItem>(session_, task, isCreator));
        break;
    }
}


void BoardPage::renderTemplate(std::ostream& result)
{
    WTemplate::renderTemplate(result);
}

void BoardPage::showDialogAddTask()
{
    Wt::WDialog *dialog = this->addChild(std::make_unique<Wt::WDialog>("Create new task"));

    Wt::WLabel *nameLabel = dialog->contents()->addNew<Wt::WLabel>("Task name");
    Wt::WLineEdit *nameEdit = dialog->contents()->addNew<Wt::WLineEdit>();
    Wt::WLabel *descriptionLabel = dialog->contents()->addNew<Wt::WLabel>("Description");
    Wt::WTextArea *descriptionEdit = dialog->contents()->addNew<Wt::WTextArea>();

    nameLabel->setBuddy(nameEdit);
    descriptionLabel->setBuddy(descriptionEdit);

    dialog->contents()->addStyleClass("form-group");
    descriptionEdit->addStyleClass("form-control");
    nameEdit->addStyleClass("form-control");

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
            task.BoardId = board_.Id;
            task.Name = nameEdit->text().toUTF8();
            task.Text = descriptionEdit->text().toUTF8();
            task.Status = TaskStatus::Open;
            task = session_.taskController().CreateTask(task);
            openListWidget_->addWidget(std::make_unique<TaskListItem>(session_, task, isCreator));
        }
        this->removeChild(dialog);
    });

    dialog->show();
}