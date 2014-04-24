
#include "cxxprof_server/DataStorage.h"

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

    //NOTE: Init is alway needed, because we're creating a new database at each startup
    //      Perhaps we should treat it like an error if there is no init needed...
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
        //TODO: How could we react better on errors? What errors are possible?
        std::cout << error.what() << std::endl;
    }
}

void DataStorage::storeResult(const CxxProf::NetworkObjects& objects)
{
    if (!connection_)
    {
        std::cout << "ERROR: No database loaded..." << std::endl;
        return;
    }

    //create a deferred transaction for the following commands
    //Simple explanation: If we do not wrap everything into a transaction here the performance would be VERY bad
    sqlite::transaction_guard< > transactionGuard(*connection_);

    //Add the Marks
    std::vector<CxxProf::NetworkMark>::const_iterator markIter = objects.Marks.begin();
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
                ");");

            insertCommand.bind(1, markIter->Name);
            insertCommand.bind(2, markIter->Timestamp);

            //We do not use the error-code here, but it is good to know that there is one if we need it
            int error = insertCommand.exec();
        }
        catch (sqlite::sqlite_error& error)
        {
            //TODO: How could we react better on errors? What errors are possible?
            std::cout << error.what() << std::endl;
        }
    }

    //Add the Plots
    std::vector<CxxProf::NetworkPlot>::const_iterator plotIter = objects.Plots.begin();
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
            ");");

        insertCommand.bind(1, plotIter->Name);
        insertCommand.bind(2, plotIter->Value);
        insertCommand.bind(3, plotIter->Timestamp);

        try
        {
            //We do not use the error-code here, but it is good to know that there is one if we need it
            int error = insertCommand.exec();
        }
        catch (sqlite::sqlite_error& error)
        {
            //TODO: How could we react better on errors? What errors are possible?
            std::cout << error.what() << std::endl;
        }
    }

    //Add the ActivityResults to the Database
    std::vector<CxxProf::ActivityResult>::const_iterator actIter = objects.ActivityResults.begin();
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
            ");");

        insertCommand.bind(1, actIter->ActId);
        insertCommand.bind(2, actIter->ThreadId);
        insertCommand.bind(3, actIter->ParentId);
        insertCommand.bind(4, actIter->StartTime);
        insertCommand.bind(5, actIter->StopTime);
        insertCommand.bind(6, actIter->Name);

        try
        {
            //We do not use the error-code here, but it is good to know that there is one if we need it
            int error = insertCommand.exec();
        }
        catch (sqlite::sqlite_error& error)
        {
            //TODO: How could we react better on errors? What errors are possible?
            std::cout << error.what() << std::endl;
        }
    }

    //commit the transaction (or it will rollback)
    transactionGuard.commit();
}
