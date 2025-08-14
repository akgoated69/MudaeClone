
#include "sqllogic.h"


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
        pstmt.release();
        return res->next();
    }
    catch (sql::SQLException& e) {
        std::cerr << "[MySQL] Error: " << e.what() << std::endl;
        return false;
    }
}

// changes the value at a specified table and column using a key pair
void changeSpecificValueSQL(sql::Connection* con,
                            const std::string& table,
                            const std::string& valueName,
                            const std::string& value,
                            const std::string& critereaName,
                            const std::string& critereaValue)  
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
                            const std::string& table, 
                            const std::string& primaryKey, 
                            const std::string& primaryKeyValue, 
                            const std::string& column)
{
    try {
        std::cout<<"[SQL request]:"<<std::string("SELECT * FROM ") + table + std::string(" WHERE ") + primaryKey + std::string(" = ") + primaryKeyValue + std::string(" AND ") + column + std::string(" IS NULL;")<<std::endl;
        std::unique_ptr<sql::PreparedStatement> pstmt(
            con->prepareStatement(std::string("SELECT * FROM ") + table + std::string(" WHERE ") + primaryKey + std::string(" = ") + primaryKeyValue + std::string(" AND ") + column + std::string(" IS NULL;"))
        );

        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        pstmt.release();
        return res->next();
    }
    catch (sql::SQLException& e) {
        std::cerr << "[MySQL] Error: " << e.what() << std::endl;
        return false;
    }
}   

// returns a string value using a primary key ONLY WORKS FOR ONE VALUE
std::string getSpecificValueWithPrimaryKey(sql::Connection *con,
                            const std::string& table, 
                            const std::string& primaryKey, 
                            const std::string& primaryKeyValue, 
                            const std::string& column)
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
        pstmt.release();

        return value;
    }
    catch (sql::SQLException& e) {
        std::cerr << "[MySQL] Error: " << e.what() << std::endl;
        return "";
    }
}  

// returns multiple values from a primary key using a resultset
sql::ResultSet* getMultipleValuesWithPrimaryKey(sql::Connection *con,
                            const std::string& table, 
                            const std::string& primaryKey, 
                            const std::string& primaryKeyValue, 
                            const std::string& column)
{
    try
    {
        std::cout<<"[SQL request]: "<<std::string("SELECT ") + column + std::string(" FROM ") + table + std::string(" WHERE ") + primaryKey + std::string(" = ") + primaryKeyValue<<std::endl;
        std::unique_ptr<sql::PreparedStatement> pstmt(
            con->prepareStatement(std::string("SELECT ") + column + std::string(" FROM ") + table + std::string(" WHERE ") + primaryKey + std::string(" = ") + primaryKeyValue)
        );

        sql::ResultSet* res(pstmt->executeQuery());
        pstmt.release();
        return res;
    }
    catch(const std::exception& e)
    {
        std::cerr << "[MySQL] Error: " << e.what() << std::endl;
        return nullptr;
    }
}

sql::ResultSet* getLargestValueFromTableAndColumnSQL(sql::Connection* con, const std::string& table, const std::string& column)
{
    try
    {
        std::cout<<"[SQL request] SELECT MAX(" + column + ") AS max_id FROM gems;"<<std::endl;
        std::unique_ptr <sql::PreparedStatement> pstmt(con -> prepareStatement("SELECT MAX(" + column + ") AS max_id FROM " + table + ";"));
        sql::ResultSet* res(pstmt -> executeQuery());
        pstmt.release();
        return res;
    }
    catch(const std::exception& e)
    {
        std::cerr << "[MySQL] Error: " << e.what() << std::endl;
        return nullptr;
    }
    
}

void insertOneValueIntoDb(sql::Connection* con, const std::string& table, const std::string& column, const std::string& value)
{
    try
    {
        std::cout<<"[SQL request]: "<<std::string("INSERT INTO " + table + " (" + column + ") VALUES(" + value +");")<<std::endl;
        std::unique_ptr<sql::PreparedStatement> pstmt(con -> prepareStatement("INSERT INTO " + table + " (" + column + ") VALUES(" + value +");"));
        pstmt -> execute();
        pstmt.release();
    }
    catch(const std::exception& e)
    {
        std::cerr << "[MySQL] Error: " << e.what() << std::endl;
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
        std::cout<<"Is the error in resultSetToVectorInt?"<<std::endl;
        values.push_back(res->getInt(column));
    }

    return values;
}

sql::ResultSet* createRandomSQLQuery(sql::Connection* con, const std::string& table)
{
    sql::Statement *stmt = con->createStatement();
    std::cout<<std::string("[SQL request]: SELECT * FROM ") + table + std::string(" ORDER BY RAND( ) LIMIT 1;")<<std::endl;
    sql::ResultSet *res = stmt->executeQuery(std::string("SELECT * FROM ") + table + std::string(" ORDER BY RAND( ) LIMIT 1;"));
    delete stmt;
    return res;
}


// queries a random card from database
sql::ResultSet* createRandomSQLQueryGems(sql::Connection* con)
{
    return createRandomSQLQuery(con,gems);
}

// converts a sql query result into a card object
Mudae::Card gemSQLResultToCard(sql::ResultSet* res)
{
    Mudae::Card tempCard;
    while (res->next())
    {
        std::cout<<"Is the error in gemSQLResultToCard?"<<std::endl;
        tempCard.setCard(res->getInt("id"),res->getString("imgurLink"));
    } 
    return tempCard;
}

// makes an sql query for a specific card based on id number
sql::ResultSet* makeSQLRequestForSpecificId(sql::Connection *con,const std::string& cardId)
{
    const std::string all = "*";
    return getMultipleValuesWithPrimaryKey(con,gems,id,cardId,all);
}

// makes an sql query to find the card with the largest id card (important for error handling)
sql::ResultSet* makeSQLRequestForLargestId(sql::Connection *con)
{
    return getLargestValueFromTableAndColumnSQL(con,gems,id);
}


// converst the a sql result (for id) into an integer
int sqlResultToInt(sql::ResultSet* res)
{
    int value{};
    while(res->next())
    {
        value = res->getInt(1);
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
    insertOneValueIntoDb(con,gems,imgurLink,link);
}

// checks if a user is in the database
bool checkIfUserInDB(sql::Connection* con, std::string userID)
{
    return checkIfValueExists(con,"users","user_id",userID);
}

// uploads user ID into database
void uploadUserID(sql::Connection* con, std::string userID)
{
    insertOneValueIntoDb(con,users,user_id,userID);
}

// checks if the card is claimed returns true if its not 
bool checkIfCardClaimed(sql::Connection* con,std::string cardId)
{
    return checkIfValueisNullSQL(con,gems,id, cardId, user_id);
}

// assigns a card object a user_id
void assignCardOwner(sql::Connection* con, std::string imageId, std::string owner)
{
    changeSpecificValueSQL(con, gems, user_id, owner, id, imageId);
}

// returns the user_id of a card
std::string getCardOwner(sql::Connection* con, std::string cardID)
{
    if(checkIfCardClaimed(con,id))
    {
        return "No owner";
    }
    return getSpecificValueWithPrimaryKey(con,gems,id,cardID,user_id);
}

// returns the id of each card belonging to a user in a vector
std::vector<int> getAllCardBelongingToUser(sql::Connection *con, std::string userID)
{
    std::string value = std::string("'") + userID + std::string("'");
    return resultSetToVectorInt(getMultipleValuesWithPrimaryKey(con,gems,user_id,value,id),id);
}

// checks if the user has any cards belonging to him 
bool checkIfUserHasCards(sql::Connection *con, std::string userId)
{
    std::string value = std::string("'") + userId +  std::string("'");
    return checkIfValueisNullSQL(con,gems,user_id,value,id);
}

sql::ResultSet* makeRequestForDateTime(sql::Connection* con)
{
    std::unique_ptr<sql::PreparedStatement> pstmt(con -> prepareStatement("SELECT NOW();"));
    sql::ResultSet* res(pstmt->executeQuery());
    pstmt.release();
    return res;
}

std::string resultSetToDateTime(sql::ResultSet* res)
{
    std::string dateTime;
    while(res -> next())
    {
        dateTime = res -> getString("NOW()");
    }
    return dateTime;
}

std::string getCurrentDateTime(sql::Connection *con)
{
    return resultSetToDateTime(makeRequestForDateTime(con));
}

void setSpecificValueStringSQL(sql::Connection* con, const std::string& table, const std::string& column, const std::string& value, const std::string& primaryKey, const std::string& primaryKeyValue)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> pstmt(con -> prepareStatement("UPDATE " + table + " SET " + column + " = '" + value +"' WHERE " + primaryKey + " = '" + primaryKeyValue + "';"));
        pstmt -> execute();
        pstmt.release();
    }
    catch(const std::exception& e)
    {
        std::cerr << "[MySQL] Error: " << e.what() << std::endl;
    }
}

void setClaimTimeForUser(sql::Connection *con, const std::string& userId)
{
    setSpecificValueStringSQL(con,users,last_claim,getCurrentDateTime(con),user_id,userId);
}

