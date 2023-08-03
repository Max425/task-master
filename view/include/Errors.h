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


class Page404 : public Wt::WTemplate
{
public:
  Page404();

protected:
  virtual void renderTemplate(std::ostream& result);
  
  using WWebWidget::render;
};


class Page403 : public Wt::WTemplate
{
public:
  Page403();

protected:
  virtual void renderTemplate(std::ostream& result);
  
  using WWebWidget::render;
};