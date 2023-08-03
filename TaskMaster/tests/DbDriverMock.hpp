#pragma once
#include "DbDriverInterface.hpp"
#include <string>
#include <vector>


class DbDriverMock : public DbDriverInterface {
 public:
  MOCK_METHOD(std::vector<std::vector<std::string>>, Exec, (const std::string& query), (override));
  MOCK_METHOD(bool, Connected, (), (const, override));
};