#include "ImageWidget.h"

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

ImageWidget::ImageWidget()
{
  setTemplateText(tr("main-view"));
  this->bindWidget("main-image", std::make_unique<Wt::WImage>("css/image.jpg"));
}

void ImageWidget::renderTemplate(std::ostream &result)
{
    WTemplate::renderTemplate(result);
}