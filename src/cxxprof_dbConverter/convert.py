
import fnmatch
import json
import os
import sqlite3
import sys

'''
This script simply loads the SQLite DBs created by cxxprof_server and converts them to a JSON format which
Chromiums tracing-view could understand and display.

There are some issues where the activities are not fully compatible with the format of the needed JSON, but
for simplicities sake and better performance this is hacked to work in a useful way.

Usage:
    If the script is called with db-files as arguments it will convert these, else if will simply convert
    everything it finds in the current working directory
'''

class Thread():
    def __init__(self):
        self.ThreadId = 0
        self.Name = ""

class App():
    def __init__(self):
        self.AppId = 0
        self.Name = ""
        self.Starttime = 0
        self.Threads = {}

def convertDb(givenDb):
    connection = sqlite3.connect(givenDb)
    sqliteCursor = connection.cursor()
    
    #This is where we're storing our data. Read: the JSON object
    traceEvents = []

    #this is where we're storing the application and thread aliases
    applications = {}
    
    #this reads in the applications, we need these values later to get the right data to the right places
    getAllApps = "SELECT * FROM appinfo ORDER BY AppId;"
    for row in sqliteCursor.execute(getAllApps):
        newApp = App()
        newApp.AppId = int(row[0])
        newApp.Name = row[1]
        newApp.Starttime = int(row[2])
        
        applications[newApp.AppId] = newApp
        
    #same as above with the applications, just with threads
    getAllThreads = "SELECT * FROM threadinfo ORDER BY ThreadId;"
    for row in sqliteCursor.execute(getAllThreads):
        newThread = Thread()
        newThread.ThreadId = int(row[0])
        newThread.Name = row[2]
        appId = int(row[1])
        
        applications[appId].Threads[newThread.ThreadId] = newThread
    
    #TODO:  When using X-phases we get problems if multiple activities have the same starttime. They'll be visualized stacked though they've
    #       been originally executed serially.
    #       Better use B/E phases here, but I can't think of a good way to get the hierarchy displayed without using much extra memory currently.
    #TODO:  The below hack removes empty activities (duration == 0.0) when the previous activity duration was also 0.0... This is NOT a good solution.
    previousDuration = 0.0
    previousTimestamp = 0.0
    
    #It's best to acquire the activities by ActivityId, that way they're sorted after their creation date and nothing gets mixed up
    #Using the Starttime here causes problems when multiple activities have the same starttime due to bad timer resolution.
    getAllActivities = "SELECT * FROM activities ORDER BY ActivityId;"
    for row in sqliteCursor.execute(getAllActivities):
        starttime = int(row[4])
        endtime = int(row[5])
        duration = endtime - starttime
        
        #HACK:  The following hinders similar elements from stacking up by not drawing them
        #       This is not a good solution as it removes profiling data that the user would expect...
        #       See above TODO
        if(duration == 0.0 and previousDuration == 0.0 and previousTimestamp == starttime):
            continue
        previousDuration = duration
        previousTimestamp = starttime
        
        appName = applications[int(row[2])].Name
        threadName = applications[int(row[2])].Threads[int(row[1])].Name
        #TODO: As soon as we have support for different processes we need to alter the following line to support that
        actObject = {"cat":"CxxProf", "pid":appName, "tid":threadName, "ts":int(row[4]), "ph":"X", "name":row[6], "dur": duration}
        traceEvents.append(actObject)
        
    getAllMarks = "SELECT * FROM marks ORDER BY MarkId;"
    for row in sqliteCursor.execute(getAllMarks):
        appName = applications[int(row[1])].Name
        #TODO: As soon as we have support for different processes we need to alter the following line to support that
        markObject = {"cat":"CxxProf", "pid":appName, "tid":0, "ts":int(row[3]), "ph":"I", "name":row[2], "s":"p"}
        traceEvents.append(markObject)
        
    getAllPlots = "SELECT * FROM plots ORDER BY PlotId;"
    for row in sqliteCursor.execute(getAllPlots):
        appName = applications[int(row[1])].Name
        #TODO: As soon as we have support for different processes we need to alter the following line to support that
        plotObject = {"cat":"CxxProf", "pid":appName, "tid":0, "ts":int(row[4]), "ph":"C", "name":row[2], "args":{}}
        countObject = {row[2]:int(row[3])}
        plotObject["args"] = countObject
        traceEvents.append(plotObject)
        
    #JSON is so easy with Python... I won't talk about C++ here...
    jsonData = json.dumps(traceEvents)
    
    #TODO:  This results in problems if the given files are with their path. the resulting file then
    #       will be named with the path as part of their filename (is this really the case? Haven't tested yet...)
    outfile = open(givenDb + ".json", "w")
    outfile.write(jsonData)
    outfile.close()

def main():
    #if there are arguments given try to convert these
    for arg in sys.argv[1:]:
        convertDb(arg)
        exit()
    
    #in other cases just convert everything we find
    matches = []
    for root, dirnames, filenames in os.walk('.'):
        for filename in fnmatch.filter(filenames, '*.db'):
            convertDb(filename)
    

if __name__ == "__main__":
    main()


