#include "Project.hpp"

Project::Project(int creatorId, const std::string &name) :
    CreatorId(creatorId), Name(name) {}

Project::Project(int creatorId, const std::string &name, const std::string &description) :
CreatorId(creatorId), Name(name), Description(description) {}

Project::Project(){}