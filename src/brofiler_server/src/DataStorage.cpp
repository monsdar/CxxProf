
#include "brofiler_server/DataStorage.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>

#include <boost/filesystem.hpp>
#include <ctime>

DataStorage::DataStorage()
{
    //create a new database for each session
    std::string dbName = "session_" + getCurrentDateStr() + ".db";
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

std::string DataStorage::getCurrentDateStr()
{
    boost::posix_time::ptime currentTime = boost::posix_time::second_clock::local_time();
    static std::locale loc(std::cout.getloc(), new boost::posix_time::time_facet("%d%b%Y_%H%M%S"));
    std::ostringstream dtStream;
    dtStream.imbue(loc);
    dtStream << currentTime;

    return dtStream.str();
}

void DataStorage::initDatabase(const std::string& filename)
{
    //Create table for the Marks
    sqlite::command initMarkCommand(*connection_,
        "CREATE TABLE marks ( "
        "   MarkId INTEGER PRIMARY KEY AUTOINCREMENT, "
        "   Name TEXT, "
        "   Timestamp INTEGER "
        ")");


    //Create table for the Plots
    sqlite::command initPlotCommand(*connection_,
        "CREATE TABLE plots ( "
        "   PlotId INTEGER PRIMARY KEY AUTOINCREMENT, "
        "   Name TEXT, "
        "   Value REAL, "
        "   Timestamp INTEGER "
        ")");

    //Create table for the activities
    sqlite::command initActCommand(*connection_,
        "CREATE TABLE activities ( "
        "   ActivityId INTEGER NOT NULL, "
        "   ThreadId INTEGER NOT NULL, "
        "   ParentId INTEGER, "
        "   Starttime INTEGER, "
        "   Stoptime INTEGER, "
        "   Name TEXT, "
        "   PRIMARY KEY(ActivityId, ThreadId)"
        ")");

    try
    {
        initActCommand.exec();
        initMarkCommand.exec();
        initPlotCommand.exec();
    }
    catch (sqlite::sqlite_error& error)
    {
        std::cout << error.what() << std::endl;
    }
}

void DataStorage::storeResult(const NetworkObjects& objects)
{
    if (!connection_)
    {
        std::cout << "ERROR: No database loaded..." << std::endl;
        return;
    }

    //Add the Marks
    std::vector<NetworkMark>::const_iterator markIter = objects.Marks.begin();
    for (; markIter != objects.Marks.end(); ++markIter)
    {
        try
        {
            sqlite::command insertCommand(*connection_,
                "INSERT INTO marks ( "
                    "Name, "
                    "Timestamp) "
                "VALUES ( "
                    "?, " //Name
                    "? " //Timestamp
                ")");

            insertCommand.bind(1, markIter->Name);
            insertCommand.bind(2, markIter->Timestamp);

            int error = insertCommand.exec();
        }
        catch (sqlite::sqlite_error& error)
        {
            std::cout << error.what() << std::endl;
        }
    }

    //Add the Plots
    std::vector<NetworkPlot>::const_iterator plotIter = objects.Plots.begin();
    for (; plotIter != objects.Plots.end(); ++plotIter)
    {
        sqlite::command insertCommand(*connection_,
            "INSERT INTO plots ("
                "Name, "
                "Value, "
                "Timestamp) "
            "VALUES ( "
                "?, " //Name
                "?, " //Value
                "? " //Timestamp
            ")");

        insertCommand.bind(1, plotIter->Name);
        insertCommand.bind(2, plotIter->Value);
        insertCommand.bind(3, plotIter->Timestamp);

        try
        {
            int error = insertCommand.exec();
        }
        catch (sqlite::sqlite_error& error)
        {
            std::cout << error.what() << std::endl;
        }
    }

    //Add the ActivityResults to the Database
    std::vector<ActivityResult>::const_iterator actIter = objects.ActivityResults.begin();
    for (; actIter != objects.ActivityResults.end(); ++actIter)
    {
        //TODO: Check if an ActivityResult with the given ActId is already stored in the database, then react appropriately
        sqlite::command insertCommand(*connection_,
            "INSERT INTO activities ("
                "ActivityId,"
                "ThreadId,"
                "ParentId,"
                "Starttime,"
                "Stoptime,"
                "Name) "
            "VALUES ( "
                "?, " //ActivityId
                "?, " //ThreadId
                "?, " //ParentId
                "?, " //Starttime
                "?, " //Stoptime
                "? " //Name 
            ")");

        insertCommand.bind(1, actIter->ActId);
        insertCommand.bind(2, actIter->ThreadId);
        insertCommand.bind(3, actIter->ParentId);
        insertCommand.bind(4, actIter->StartTime);
        insertCommand.bind(5, actIter->StopTime);
        insertCommand.bind(6, actIter->Name);

        try
        {
            int error = insertCommand.exec();
        }
        catch (sqlite::sqlite_error& error)
        {
            std::cout << error.what() << std::endl;
        }
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

