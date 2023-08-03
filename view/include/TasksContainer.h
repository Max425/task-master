#pragma once



#include <Wt/WContainerWidget.h>
#include "Task.hpp"






class TasksContainer : public Wt::WContainerWidget
{
private:
    TaskStatus containerType;
public:
    TasksContainer(TaskStatus type);
    void dropEvent(Wt::WDropEvent event);
};

