#pragma once
#include <string>

class Project
{
public:
    Project();
    Project(int creatorId, const std::string &name);
    Project(int creatorId, const std::string &name,const std::string &description);
    size_t Id;
    size_t CreatorId;
    std::string Name;
    std::string Description;
};