#include "MainPage.h"

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

MainPage::MainPage(Session &session) : session_(session)
{
    setTemplateText(tr("main-page"));
    projects_ = this->bindWidget("projects", std::make_unique<Wt::WContainerWidget>());
    user_ = session.user();
    
    createButton_ = this->bindWidget("createProjectButton", std::make_unique<Wt::WPushButton>("Create project"));
    createButton_->clicked().connect(this, &MainPage::createProject);
    createButton_->addStyleClass("btn btn-success");
    
    myTasksButton_ = this->bindWidget("myTasksButton", std::make_unique<Wt::WPushButton>("My tasks"));
    myTasksButton_->addStyleClass("btn btn-primary");
    myTasksButton_->clicked().connect(this, &MainPage::redirectToMyTasks);
    
    showProjects();
}

void MainPage::showProjects()
{
    auto projects = session_.mainPadgeController().GetUserProjects(user_.Id);
    for (auto proj : projects)
        showProject(proj);
}

void MainPage::createProject()
{
    Wt::WDialog *dialog = this->addChild(std::make_unique<Wt::WDialog>("Create new project"));

    Wt::WLabel *titleLabel = dialog->contents()->addNew<Wt::WLabel>("Project title");
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
                auto newProject = session_.mainPadgeController().CreateProject(id, name, text);
                showProject(newProject);
            }
        }
        this->removeChild(dialog);
    });

    dialog->show();
}


void MainPage::showProject(const Project &proj)
{
    projects_->addWidget(std::make_unique<ProjectListItem>(session_, proj));
}

void MainPage::redirectToMyTasks() {
    Wt::WApplication::instance()->setInternalPath("/mytasks", true);
}

void MainPage::renderTemplate(std::ostream &result)
{
    WTemplate::renderTemplate(result);
}