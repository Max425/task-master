#include "TasksContainer.h"

#include "TaskListItem.h"

TasksContainer::TasksContainer(TaskStatus type) : containerType(type)
{
    acceptDrops("TaskListItem");
    addStyleClass("cont");
}


void TasksContainer::dropEvent(Wt::WDropEvent event)
{
    if(event.mimeType() == "TaskListItem")
    {
        TaskListItem *child = static_cast<TaskListItem*>(event.source());
        child->SetStatus(containerType);
        auto parent = static_cast<TasksContainer*>(child->parent());
        auto childPtr = parent->removeWidget(child);
        addWidget(std::move(childPtr));
    }
}