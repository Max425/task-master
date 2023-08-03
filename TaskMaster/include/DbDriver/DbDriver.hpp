#include "DbDriverInterface.hpp"
#include <pqxx/pqxx>
#include <vector>
#include <string>

class DbDriver : public DbDriverInterface
{
public:
    DbDriver(const std::string& connect);
    ~DbDriver();
    std::vector<std::vector<std::string>> Exec(const std::string& query) override;
    bool Connected() const override;
private:
    pqxx::connection conn_;
};