
#include "brofiler_server/DataStorage.h"

#include <boost/filesystem.hpp>

DataStorage::DataStorage()
{
    //TODO: Create a new database at each startup? Is there a case where we need to reuse the database?

    std::string dbName = "data.db";
    bool isInitNeeded = !boost::filesystem::exists(dbName);
    connection_.reset(new sqlite::connection(dbName));

    if (isInitNeeded)
    {
        initDatabase(dbName);
    }
}

DataStorage::~DataStorage()
{
    if (connection_)
    {
        connection_->close();
    }
}

void DataStorage::initDatabase(const std::string& filename)
{
    //TODO: We need ActivityId and ThreadId to form a composite Primary Key -> Is this possible with SQLite?

    sqlite::command initCommand(*connection_,
        "CREATE TABLE activities ( "
        "   ActivityId INTEGER PRIMARY KEY, "
        "   ThreadId INTEGER, "
        "   ParentId INTEGER, "
        "   Starttime INTEGER, "
        "   Stoptime INTEGER, "
        "   Name TEXT "
        ")");

    try
    {
        int error = initCommand.exec();
    }
    catch (sqlite::sqlite_error& error)
    {
        std::cout << error.what() << std::endl;
    }
}

void DataStorage::storeResult(const ActivityResult& result)
{
    if (!connection_)
    {
        std::cout << "ERROR: No database loaded..." << std::endl;
        return;
    }

    //TODO: Check if an ActivityReult with the given ActId is already stored in the database, then react appropriately

    sqlite::command insertCommand(*connection_,
        "INSERT INTO activities VALUES ( "
        "?, " //ActivityId
        "?, " //ThreadId
        "?, " //ParentId
        "?, " //Starttime
        "?, " //Stoptime
        "? " //Name 
        ")");

    insertCommand.bind(1, result.ActId);
    insertCommand.bind(2, result.ThreadId);
    insertCommand.bind(3, result.ParentId);
    insertCommand.bind(4, result.StartTime);
    insertCommand.bind(5, result.StopTime);
    insertCommand.bind(6, result.Name);

    try
    {
        int error = insertCommand.exec();
    }
    catch (sqlite::sqlite_error& error)
    {
        std::cout << error.what() << std::endl;
    }
}

/*
void checkSqliteCode()
{
sqlite::connection conn("data.s3db");
sqlite::query queryAllActivities(conn, "SELECT * FROM activities");
std::cout << "Results: " << queryAllActivities.num_results() << std::endl;

for (sqlite::query::iterator iter = queryAllActivities.begin(); iter != queryAllActivities.end(); iter++)
{
unsigned int actId = iter->column< unsigned int >(0);
unsigned int threadId = iter->column< unsigned int >(1);
unsigned int parentId = iter->column< unsigned int >(2);
unsigned int startTime = iter->column< unsigned int >(3);
unsigned int stopTime = iter->column< unsigned int >(4);
std::string name = iter->column< std::string >(5);

std::cout << std::endl;
std::cout << "Row Number: " << iter->row_number() << std::endl;
std::cout << "Activity " << name << " (" << actId << ")" << std::endl;
std::cout << "Thread: " << threadId << std::endl;
std::cout << "Parent: " << parentId << std::endl;
std::cout << "Start: " << startTime << std::endl;
std::cout << "Stop: " << stopTime << std::endl;
}
}
*/

