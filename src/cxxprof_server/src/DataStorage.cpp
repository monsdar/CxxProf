
#include "cxxprof_server/DataStorage.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>

#include <boost/filesystem.hpp>

#include <stdlib.h> // needed for exit()

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
    //create a deferred transaction for the following commands
    //Simple explanation: If we do not wrap everything into a transaction here the performance would be VERY bad
    sqlite::transaction_guard< > transactionGuard(*connection_);

    try
    {
        //Create table for the AppInfo
        //We're making Name UNIQUE here because there shouldn't be more than 1 application with the same name
        //This makes it easier when we're adding the applicationInfo to the DB later
        sqlite::command initAppInfoCommand(*connection_,
            "CREATE TABLE appinfo ( "
            "   AppId INTEGER PRIMARY KEY AUTOINCREMENT, "
            "   Name TEXT UNIQUE, "
            "   Starttime INTEGER"
            ")");

        //Create table for the ThreadInfo
        //We do not need to autoincrement here as the Id is unique on userside
        sqlite::command initThreadInfoCommand(*connection_,
            "CREATE TABLE threadinfo ( "
            "   ThreadId INTEGER, "
            "   AppId INTEGER, "
            "   Name TEXT, "
            "   PRIMARY KEY(ThreadId, AppId)"
            ")");

        //Create table for the Marks
        sqlite::command initMarkCommand(*connection_,
            "CREATE TABLE marks ( "
            "   MarkId INTEGER PRIMARY KEY AUTOINCREMENT, "
            "   AppId INTEGER NOT NULL, "
            "   Name TEXT, "
            "   Timestamp INTEGER "
            ")");


        //Create table for the Plots
        sqlite::command initPlotCommand(*connection_,
            "CREATE TABLE plots ( "
            "   PlotId INTEGER PRIMARY KEY AUTOINCREMENT, "
            "   AppId INTEGER NOT NULL, "
            "   Name TEXT, "
            "   Value REAL, "
            "   Timestamp INTEGER "
            ")");

        //Create table for the activities
        //We do not need to use AUTO_INCREMENT for these activities because their IDs are defined previously on user side
        //Same goes for ThreadId and AppId
        sqlite::command initActCommand(*connection_,
            "CREATE TABLE activities ( "
            "   ActivityId INTEGER NOT NULL, "
            "   ThreadId INTEGER NOT NULL, "
            "   AppId INTEGER NOT NULL, "
            "   ParentId INTEGER, "
            "   Starttime INTEGER, "
            "   Stoptime INTEGER, "
            "   Name TEXT, "
            "   PRIMARY KEY(ActivityId, ThreadId, AppId)"
            ")");

        initAppInfoCommand.exec();
        initThreadInfoCommand.exec();
        initActCommand.exec();
        initMarkCommand.exec();
        initPlotCommand.exec();
    }
    catch (sqlite::sqlite_error& error)
    {
        //If something went wrong during initialization of the database we should fail hard and early
        std::cout << __FUNCTION__ << ": " << error.what() << std::endl;
        exit(1);
    }

    //commit the transaction (or it will rollback)
    transactionGuard.commit();
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

    //Add/Update the AppInfo
    unsigned int appId = updateAppInfo(objects.Info);
    updateThreadInfo(objects.Info, appId);

    //Add the Marks
    updateMarks(objects.Marks, appId);

    //Add the Plots
    updatePlots(objects.Plots, appId);

    //Add the ActivityResults to the Database
    updateActivities(objects.ActivityResults, appId);

    //commit the transaction (or it will rollback)
    transactionGuard.commit();
}

unsigned int DataStorage::updateAppInfo(const CxxProf::AppInfo& info)
{
    try
    {
        sqlite::command insertCommand(*connection_,
            "INSERT INTO appinfo ( "
            "Name, "
            "Starttime) "
            "VALUES ( "
            "?, " //Name
            "? " //Timestamp
            ");");

        insertCommand.bind(1, info.Name);
        insertCommand.bind(2, info.Starttime);

        //We do not use the error-code here, but it is good to know that there is one if we need it
        int errorNum = insertCommand.exec();
    }
    catch (sqlite::sqlite_error&)
        //catch (sqlite::sqlite_error& error)
    {
        //NOTE: Unfortunately we're getting contraint_errors by design here. This is
        //      because of the UNIQUEness of the 'Name' attribute.
        //      sqlite3cc does not return the expected error code, nor is there a
        //      way to distinguish sqlite_error exceptions from each other.
        //      So we have to ignore everything on purpose to not confuse the user...

        //std::cout << __FUNCTION__ << ": " << error.what() << std::endl;
    }

    unsigned int appId = 0;
    sqlite::query idQuery(*connection_, "SELECT AppId FROM appinfo WHERE Name='" + info.Name + "';");
    for (sqlite::query::iterator queryIter = idQuery.begin(); queryIter != idQuery.end(); ++queryIter)
    {
        appId = queryIter->column< unsigned int >(0);
    }

    return appId;
}
void DataStorage::updateMarks(const std::vector<CxxProf::NetworkMark>& marks, unsigned int appId)
{
    std::vector<CxxProf::NetworkMark>::const_iterator markIter = marks.begin();
    for (; markIter != marks.end(); ++markIter)
    {
        try
        {
            sqlite::command insertCommand(*connection_,
                "INSERT INTO marks ( "
                "AppId, "
                "Name, "
                "Timestamp) "
                "VALUES ( "
                "?, " //AppId
                "?, " //Name
                "? " //Timestamp
                ");");

            insertCommand.bind(1, appId);
            insertCommand.bind(2, markIter->Name);
            insertCommand.bind(3, markIter->Timestamp);

            //We do not use the error-code here, but it is good to know that there is one if we need it
            int error = insertCommand.exec();
        }
        catch (sqlite::sqlite_error& error)
        {
            //TODO: How could we react better on errors? What errors are possible?
            std::cout << __FUNCTION__ << ": " << error.what() << std::endl;
        }
    }
}
void DataStorage::updatePlots(const std::vector<CxxProf::NetworkPlot>& plots, unsigned int appId)
{
    std::vector<CxxProf::NetworkPlot>::const_iterator plotIter = plots.begin();
    for (; plotIter != plots.end(); ++plotIter)
    {
        sqlite::command insertCommand(*connection_,
            "INSERT INTO plots ("
            "AppId, "
            "Name, "
            "Value, "
            "Timestamp) "
            "VALUES ( "
            "?, " //AppId
            "?, " //Name
            "?, " //Value
            "? " //Timestamp
            ");");

        insertCommand.bind(1, appId);
        insertCommand.bind(2, plotIter->Name);
        insertCommand.bind(3, plotIter->Value);
        insertCommand.bind(4, plotIter->Timestamp);

        try
        {
            //We do not use the error-code here, but it is good to know that there is one if we need it
            int error = insertCommand.exec();
        }
        catch (sqlite::sqlite_error& error)
        {
            //TODO: How could we react better on errors? What errors are possible?
            std::cout << __FUNCTION__ << ": " << error.what() << std::endl;
        }
    }
}
void DataStorage::updateActivities(const std::vector<CxxProf::ActivityResult>& activities, unsigned int appId)
{
    std::vector<CxxProf::ActivityResult>::const_iterator actIter = activities.begin();
    for (; actIter != activities.end(); ++actIter)
    {
        //TODO: Check if an ActivityResult with the given ActId is already stored in the database, then react appropriately
        sqlite::command insertCommand(*connection_,
            "INSERT INTO activities ("
            "ActivityId,"
            "ThreadId,"
            "AppId, "
            "ParentId,"
            "Starttime,"
            "Stoptime,"
            "Name) "
            "VALUES ( "
            "?, " //ActivityId
            "?, " //ThreadId
            "?, " //AppId
            "?, " //ParentId
            "?, " //Starttime
            "?, " //Stoptime
            "? " //Name 
            ");");

        insertCommand.bind(1, actIter->ActId);
        insertCommand.bind(2, actIter->ThreadId);
        insertCommand.bind(3, appId);
        insertCommand.bind(4, actIter->ParentId);
        insertCommand.bind(5, actIter->Starttime);
        insertCommand.bind(6, actIter->Stoptime);
        insertCommand.bind(7, actIter->Name);

        try
        {
            //We do not use the error-code here, but it is good to know that there is one if we need it
            int error = insertCommand.exec();
        }
        catch (sqlite::sqlite_error& error)
        {
            //TODO: How could we react better on errors? What errors are possible?
            std::cout << __FUNCTION__ << ": " << error.what() << std::endl;
        }
    }
}
void DataStorage::updateThreadInfo(const CxxProf::AppInfo& info, unsigned int appId)
{
    std::map<unsigned int, std::string>::const_iterator threadIter = info.ThreadAliases.begin();
    for (; threadIter != info.ThreadAliases.end(); ++threadIter)
    {
        try
        {
            sqlite::command insertCommand(*connection_,
                "INSERT INTO threadinfo ( "
                "ThreadId, "
                "AppId, "
                "Name) "
                "VALUES ( "
                "?, " //ThreadId
                "?, " //AppId
                "? " //Name
                ");");

            insertCommand.bind(1, threadIter->first);
            insertCommand.bind(2, appId);
            insertCommand.bind(3, threadIter->second);

            //We do not use the error-code here, but it is good to know that there is one if we need it
            int error = insertCommand.exec();
        }
        catch (sqlite::sqlite_error&)
        //catch (sqlite::sqlite_error& error)
        {
            //NOTE: Unfortunately we're getting contraint_errors by design here. This is
            //      because of the UNIQUEness of the 'Name' attribute.
            //      sqlite3cc does not return the expected error code, nor is there a
            //      way to distinguish sqlite_error exceptions from each other.
            //      So we have to ignore everything on purpose to not confuse the user...

            //std::cout << __FUNCTION__ << ": " << error.what() << std::endl;
        }
    }
}
