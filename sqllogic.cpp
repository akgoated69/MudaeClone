
#include "sqllogic.h"

const std::string gems = "gems";
const std::string users = "users";
const std::string user_id = "user_id";
const std::string id = "id";


// checks if a specific value for an unspecified table and column exist
bool checkIfValueExists(sql::Connection* con,
                        const std::string& tableName,
                        const std::string& columnName,
                        const std::string& value)
{
    std::cout<<"[SQL request]: "<<"SELECT 1 FROM " + tableName + " WHERE " + columnName + " =" + value + "LIMIT 1;"<<std::endl;
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(
            con->prepareStatement("SELECT 1 FROM " + tableName + " WHERE " + columnName + " = ? LIMIT 1;")
        );
        pstmt->setString(1, value);

        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        return res->next();
    }
    catch (sql::SQLException& e) {
        std::cerr << "[MySQL] Error: " << e.what() << std::endl;
        return false;
    }
}

// changes the value at a specified table and column using a key pair
void changeSpecificValueSQL(sql::Connection* con,
                            std::string& table,
                            std::string& valueName,
                            std::string& value,
                            std::string& critereaName,
                            std::string& critereaValue)  
{
    try
    {
        std::cout<<"[SQL request]: "<<"UPDATE " + table + " SET " + valueName + "= '" + value + "' WHERE " + critereaName + "=" + critereaValue + ";"<<std::endl;
        sql::PreparedStatement *pstmt;
        pstmt = con -> prepareStatement("UPDATE " + table + " SET " + valueName + "= '" + value + "' WHERE " + critereaName + "=" + critereaValue + ";");
        pstmt -> executeUpdate();
    delete pstmt;
    }
    catch(sql::SQLException &e)
    {
        std::cerr << e.what() << '\n';
    }
    
}

// checks if the value at a specific table and column are null using key pair to specify the row
bool checkIfValueisNullSQL(sql::Connection *con,
                            std::string& table, 
                            std::string& primaryKey, 
                            std::string& primaryKeyValue, 
                            std::string& column)
{
    try {
        std::cout<<"[SQL request]:"<<std::string("SELECT * FROM ") + table + std::string(" WHERE ") + primaryKey + std::string(" = ") + primaryKeyValue + std::string(" AND ") + column + std::string(" IS NULL;")<<std::endl;
        std::unique_ptr<sql::PreparedStatement> pstmt(
            con->prepareStatement(std::string("SELECT * FROM ") + table + std::string(" WHERE ") + primaryKey + std::string(" = ") + primaryKeyValue + std::string(" AND ") + column + std::string(" IS NULL;"))
        );

        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        return res->next();
    }
    catch (sql::SQLException& e) {
        std::cerr << "[MySQL] Error: " << e.what() << std::endl;
        return false;
    }
}   

// returns a string value using a primary key ONLY WORKS FOR ONE VALUE
std::string getSpecificValueWithPrimaryKey(sql::Connection *con,
                            std::string& table, 
                            std::string& primaryKey, 
                            std::string& primaryKeyValue, 
                            std::string& column)
{
    try {
        std::cout<<"[SQL request]"<<std::string("SELECT ") + column + std::string(" FROM ") + table + std::string(" WHERE ") + primaryKey + std::string(" = ") + primaryKeyValue<<std::endl;
        std::unique_ptr<sql::PreparedStatement> pstmt(
            con->prepareStatement(std::string("SELECT ") + column + std::string(" FROM ") + table + std::string(" WHERE ") + primaryKey + std::string(" = ") + primaryKeyValue)
        );

        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        std::string value;

        while(res-> next())
        {
            value = res->getString(column);
        }

        return value;
    }
    catch (sql::SQLException& e) {
        std::cerr << "[MySQL] Error: " << e.what() << std::endl;
        return "";
    }
}  

// returns multiple values from a primary key using a resultset
sql::ResultSet* getMultipleValuesWithPrimaryKey(sql::Connection *con,
                            std::string& table, 
                            std::string& primaryKey, 
                            std::string& primaryKeyValue, 
                            std::string& column)
{
    try
    {
        std::cout<<"[SQL request]: "<<std::string("SELECT ") + column + std::string(" FROM ") + table + std::string(" WHERE ") + primaryKey + std::string(" = ") + primaryKeyValue<<std::endl;
        std::unique_ptr<sql::PreparedStatement> pstmt(
            con->prepareStatement(std::string("SELECT ") + column + std::string(" FROM ") + table + std::string(" WHERE ") + primaryKey + std::string(" = ") + primaryKeyValue)
        );

        sql::ResultSet* res(pstmt->executeQuery());
        return res;
    }
    catch(const std::exception& e)
    {
        std::cerr << "[MySQL] Error: " << e.what() << std::endl;
        return nullptr;
    }
}

// turns a resultset of STRINGS into a string that contains all of them seperate by oxford commas
// and an and at the end
std::vector<std::string> resultSetToVectorString(sql::ResultSet* res, std::string column)
{
    std::vector<std::string> values{};

    while(res->next())
    {
        values.push_back(res->getString(column));
    }

    return values;
}

std::vector<int> resultSetToVectorInt(sql::ResultSet* res, std::string column)
{
    std::vector<int> values{};

    while(res->next())
    {
        values.push_back(res->getInt(column));
    }

    return values;
}



// queries a random card from database
sql::ResultSet* createRandomSQLQueryGems(sql::Connection* con){
    sql::Statement *stmt = con->createStatement();
    std::cout<<"[SQL request]: SELECT * FROM gems ORDER BY RAND( ) LIMIT 1;"<<std::endl;
    sql::ResultSet *res = stmt->executeQuery("SELECT * FROM gems ORDER BY RAND( ) LIMIT 1;");
    delete stmt;
    return res;
}

// converts a sql query result into a card object
Mudae::Card gemSQLResultToCard(sql::ResultSet* res)
{
    Mudae::Card tempCard;
    while (res->next())
    {
        tempCard.setCard(res->getInt("id"),res->getString("imgurLink"));
    } 
    return tempCard;
}

// makes an sql query for a specific card based on id number
sql::ResultSet* makeSQLRequestForSpecificId(sql::Connection *con,std::string& id)
{
    sql::Statement *stmt = con->createStatement();
    std::cout<<"[SQL request]: "<<std::string("SELECT * FROM gems WHERE id = ") + id + std::string(";")<<std::endl;
    sql::ResultSet *res = stmt->executeQuery(std::string("SELECT * FROM gems WHERE id = ") + id + std::string(";"));
    delete stmt;
    return res;
}

// makes an sql query to find the card with the largest id card (important for error handling)
sql::ResultSet* makeSQLRequestForLargestId(sql::Connection *con)
{
    sql::Statement *stmt = con->createStatement();
    std::cout<<"[SQL request]: SELECT MAX(id) AS lim FROM gems"<<std::endl;
    sql::ResultSet *res = stmt->executeQuery("SELECT MAX(id) AS lim FROM gems;");
    delete stmt;
    return res;
}


// converst the a sql result (for id) into an integer
int sqlResultToInt(sql::ResultSet* res)
{
    int value{};
    while(res->next())
    {
        value = res->getInt("lim");
    }
    return value;
}

// makes a query for the largest id number and returns it in int form
int getMaxId(sql::Connection *con)
{
    return sqlResultToInt(makeSQLRequestForLargestId(con));
}


// uploads a card to a database given a link
void uploadCardToDb(sql::Connection* con, std::string link)
{
    sql::Statement *stmt = con->createStatement();
    std::cout<<"[SQL request]: "<<std::string("INSERT INTO gems (imgurLink) VALUES('" + link + std::string("');"))<<std::endl;
    stmt->execute(std::string("INSERT INTO gems (imgurLink) VALUES('" + link + std::string("');")));
    delete stmt; 
}

// checks if a user is in the database
bool checkIfUserInDB(sql::Connection* con, std::string userID)
{
    return checkIfValueExists(con,"users","user_id",userID);
}

// uploads user ID into database
void uploadUserID(sql::Connection* con, std::string userID)
{
    sql::Statement *stmt = con->createStatement();
    std::cout<<"[SQL request]: "<<std::string("INSERT INTO users (user_id) VALUES('") + userID + std::string("');")<<std::endl;
    stmt->execute(std::string("INSERT INTO users (user_id) VALUES('") + userID + std::string("');"));
    delete stmt;
}

// checks if the card is claimed returns true if its not 
bool checkIfCardClaimed(sql::Connection* con,std::string cardId)
{
    std::string table = "gems";
    std::string primaryKey = "id";
    std::string column = "user_id";
    
    return checkIfValueisNullSQL(con,table,primaryKey, cardId, column);
}

// assigns a card object a user_id
void assignCardOwner(sql::Connection* con, std::string imageId, std::string owner)
{
    std::string userColumn = "user";
    std::string user_id = "user_id";
    std::string table = "gems";
    std::string id = "id";
    changeSpecificValueSQL(con, table, user_id, owner, id, imageId);
}

// returns the user_id of a card
std::string getCardOwner(sql::Connection* con, std::string id)
{
    std::string table = "gems";
    std::string column = "user_id";
    std::string primaryKey = "id";
    if(checkIfCardClaimed(con,id))
    {
        return "No owner";
    }
    return getSpecificValueWithPrimaryKey(con,table,primaryKey,id,column);
}

// returns the id of each card belonging to a user in a vector
std::vector<int> getAllCardBelongingToUser(sql::Connection *con, std::string user_id)
{
    std::string table = "gems";
    std::string primaryKey = "user_id"; 
    std::string column = "id";
    std::string value = std::string("'") + user_id + std::string("'");
    return resultSetToVectorInt(getMultipleValuesWithPrimaryKey(con,table,primaryKey,value,column),column);
}

// checks if the user has any cards belonging to him 
bool checkIfUserHasCards(sql::Connection *con, std::string user_id)
{
    std::string table = "gems";
    std::string primaryKey = "user_id";
    std::string id = "id";
    std::string value = std::string("'") + user_id +  std::string("'");
    return checkIfValueisNullSQL(con,table,primaryKey,value,id);
}

