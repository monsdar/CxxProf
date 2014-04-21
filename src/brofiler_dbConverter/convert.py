
import fnmatch
import json
import os
import sqlite3
import sys

def convertDb(givenDb):
    connection = sqlite3.connect(givenDb)
    sqliteCursor = connection.cursor()

    traceEvents = []
    
    #TODO:  When using X-phases we get problems if multiple activities have the same starttime. They'll be visualized stacked though they've
    #       been originally executed serially.
    #       Better use B/E phases here, but I can't think of a good way to get the hierarchy displayed without using much extra memory currently.
    previousDuration = 0.0
    previousTimestamp = 0.0
    
    getAllActivities = "SELECT * FROM activities ORDER BY ActivityId;"
    for row in sqliteCursor.execute(getAllActivities):
        starttime = int(row[3])
        endtime = int(row[4])
        duration = endtime - starttime
        
        #HACK:  The following hinders similar elements from stacking up by not drawing them
        #       This is not a good solution as it removes profiling data that the user would expect...
        #       See above TODO
        if(duration == 0.0 and previousDuration == 0.0 and previousTimestamp == starttime):
            continue
        previousDuration = duration
        previousTimestamp = starttime
        
        actObject = {"cat":"Brofiler", "pid":"Process #1", "tid":int(row[1]), "ts":int(row[3]), "ph":"X", "name":row[5], "dur": duration}
        traceEvents.append(actObject)
        
    getAllMarks = "SELECT * FROM marks ORDER BY MarkId;"
    for row in sqliteCursor.execute(getAllMarks):
        markObject = {"cat":"Brofiler", "pid":"Process #1", "tid":0, "ts":int(row[2]), "ph":"I", "name":row[1], "s":"g"}
        traceEvents.append(markObject)
        
    getAllPlots = "SELECT * FROM plots ORDER BY PlotId;"
    for row in sqliteCursor.execute(getAllPlots):
        plotObject = {"cat":"Brofiler", "pid":"Process #1", "tid":0, "ts":int(row[3]), "ph":"C", "name":row[1], "args":{}}
        countObject = {row[1]:int(row[2])}
        plotObject["args"] = countObject
        traceEvents.append(plotObject)
        
    jsonData = json.dumps(traceEvents)
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


