#include "Errors.h"

Page404::Page404()
{
    setTemplateText(tr("error"));
    this->bindString("ErrorText", "Ошибка 404: Страница не найдена");
}

void Page404::renderTemplate(std::ostream &result)
{
    WTemplate::renderTemplate(result);
}

Page403::Page403()
{
    setTemplateText(tr("error"));
    this->bindString("ErrorText", "Ошибка 403: Нет доступа");
}

void Page403::renderTemplate(std::ostream &result)
{
    WTemplate::renderTemplate(result);
}
