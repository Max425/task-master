#include "DbDriver.hpp"
#include <iostream>

std::vector<std::vector<std::string>> result_to_vector(const pqxx::result &res)
{
    std::vector<std::vector<std::string>> result_vector;
    for (auto row = res.begin(); row != res.end(); ++row)
    {
        std::vector<std::string> row_vector;
        for (auto col = row.begin(); col != row.end(); ++col)
        {
            row_vector.push_back(col->c_str());
        }
        result_vector.push_back(row_vector);
    }
    return result_vector;
}

DbDriver::DbDriver(const std::string &connect) : conn_(connect) {}

DbDriver::~DbDriver() { conn_.disconnect(); }

std::vector<std::vector<std::string>> DbDriver::Exec(const std::string &query)
{
    std::vector<std::vector<std::string>> result_vector;
    try
    {
        pqxx::work txn(conn_);
        pqxx::result res = txn.exec(query);
        result_vector = result_to_vector(res);
        txn.commit();
    }
    catch (const std::exception &e)
    {
        std::cout << e.what();
    }
    return result_vector;
}

bool DbDriver::Connected() const
{
    return conn_.is_open();
}