#include "cardClass.h"
#include <dpp/dpp.h>
#include <iostream>

sql::ResultSet* createRandomSQLQueryGems(sql::Connection* con){
    sql::Statement *stmt = con->createStatement();
    sql::ResultSet *res = stmt->executeQuery("SELECT * FROM gems ORDER BY RAND( ) LIMIT 1;");
    free(stmt);
    return res;
}

Mudae::Card gemSQLResultToCard(sql::ResultSet* res)
{
    Mudae::Card tempCard;
    while (res->next())
    {
        tempCard.setCard(res->getInt("id"),res->getString("imgurLink"));
    } 
    return tempCard;
}

sql::ResultSet* makeSQLRequestForSpecificId(sql::Connection *con,std::string& id)
{
    sql::Statement *stmt = con->createStatement();
    sql::ResultSet *res = stmt->executeQuery(std::string("SELECT * FROM gems WHERE id = ") + id + std::string(";"));
    free(stmt);
    return res;
}

sql::ResultSet* makeSQLRequestForLargestId(sql::Connection *con)
{
    sql::Statement *stmt = con->createStatement();
    sql::ResultSet *res = stmt->executeQuery("SELECT MAX(id) AS lim FROM gems;");
    free(stmt);
    return res;
}

int sqlResultToInt(sql::ResultSet* res)
{
    int value{};
    while(res->next())
    {
        value = res->getInt("lim");
    }
    return value;
}

int getMaxId(sql::Connection *con)
{
    return sqlResultToInt(makeSQLRequestForLargestId(con));
}