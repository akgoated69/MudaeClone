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

const std::string gems = "gems";
const std::string users = "users";
const std::string user_id = "user_id";
const std::string id = "id";
const std::string imgurLink = "imgurLink";
const std::string last_claim = "last_claim";

bool checkIfValueExists(sql::Connection* con,
                        const std::string& tableName,
                        const std::string& columnName,
                        const std::string& value);

void changeSpecificValueSQL(sql::Connection* con,
                            const std::string& table,
                            const std::string& valueName,
                            const std::string& value,
                            const std::string& critereaName,
                            const std::string& critereaValue);

bool checkIfValueisNullSQL(sql::Connection *con,
                            const std::string& table, 
                            const std::string& primaryKey, 
                            const std::string& primaryKeyValue, 
                            const std::string& column);

std::string getSpecificValueWithPrimaryKey(sql::Connection *con,
                            const std::string& table, 
                            const std::string& primaryKey, 
                            const std::string& primaryKeyValue, 
                            const std::string& column);

sql::ResultSet* getMultipleValuesWithPrimaryKey(sql::Connection *con,
                            const std::string& table, 
                            const std::string& primaryKey, 
                            const std::string& primaryKeyValue, 
                            const std::string& column);

sql::ResultSet* getLargestValueFromTableAndColumnSQL(sql::Connection* con, const std::string& table, const std::string& column);

void insertOneValueIntoDb(sql::Connection* con, const std::string& table, const std::string& column, const std::string& value);

sql::ResultSet* createRandomSQLQuery(sql::Connection* con, const std::string& table);

sql::ResultSet* createRandomSQLQueryGems(sql::Connection* con);

Mudae::Card gemSQLResultToCard(sql::ResultSet* res);

sql::ResultSet* makeSQLRequestForSpecificId(sql::Connection *con,const std::string& cardId);

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

sql::ResultSet* makeRequestForDateTime(sql::Connection* con);

std::string resultSetToDateTime(sql::ResultSet* res);

std::string getCurrentDateTime(sql::Connection *con);

std::string getCurrentDateTime(sql::Connection *con);

std::string resultSetToDateTime(sql::ResultSet* res);

sql::ResultSet* makeRequestForDateTime(sql::Connection* con);

void setSpecificValueStringSQL(sql::Connection* con, const std::string& table, const std::string& column, const std::string& value, const std::string& primaryKey, const std::string& primaryKeyValue);

void setClaimTimeForUser(sql::Connection *con, const std::string& userId);

#endif