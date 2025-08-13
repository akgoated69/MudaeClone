#ifndef SQLLOGIC
#define SQLLOGIC

#include "cardClass.h"
#include <dpp/dpp.h>
#include <iostream>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>
#include <cstdlib>
#include <string>
#include <vector>
#include "randomlogic.h"

bool checkIfValueExists(sql::Connection* con,
                        const std::string& tableName,
                        const std::string& columnName,
                        const std::string& value);

void changeSpecificValueSQL(sql::Connection* con,
                            std::string& table,
                            std::string& valueName,
                            std::string& value,
                            std::string& critereaName,
                            std::string& critereaValue);

bool checkIfValueisNullSQL(sql::Connection *con,
                            std::string& table, 
                            std::string& primaryKey, 
                            std::string& primaryKeyValue, 
                            std::string& column);

std::string getSpecificValueWithPrimaryKey(sql::Connection *con,
                            std::string& table, 
                            std::string& primaryKey, 
                            std::string& primaryKeyValue, 
                            std::string& column);

sql::ResultSet* getMultipleValuesWithPrimaryKey(sql::Connection *con,
                            std::string& table, 
                            std::string& primaryKey, 
                            std::string& primaryKeyValue, 
                            std::string& column);

sql::ResultSet* createRandomSQLQueryGems(sql::Connection* con);

Mudae::Card gemSQLResultToCard(sql::ResultSet* res);

sql::ResultSet* makeSQLRequestForSpecificId(sql::Connection *con,std::string& id);

sql::ResultSet* makeSQLRequestForLargestId(sql::Connection *con);

int sqlResultToInt(sql::ResultSet* res);

int getMaxId(sql::Connection *con);

void uploadCardToDb(sql::Connection* con, std::string link);

bool checkIfUserInDB(sql::Connection* con, std::string userID);

void uploadUserID(sql::Connection* con, std::string userID);

bool checkIfCardClaimed(sql::Connection* con,std::string cardId);

void assignCardOwner(sql::Connection* con, std::string imageId, std::string owner);

std::string getCardOwner(sql::Connection* con, std::string id);

std::vector<std::string> resultSetToVectorString(sql::ResultSet* res, std::string column);

std::vector<int> getAllCardBelongingToUser(sql::Connection *con, std::string user_id);

bool checkIfUserHasCards(sql::Connection *con, std::string user_id);


#endif