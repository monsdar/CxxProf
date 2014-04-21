
import json
import sqlite3
import sys

def main():
    if(len(sys.argv) < 2):
        print "Please give a session db as input"
        exit()
    
    connection = sqlite3.connect(sys.argv[1])
    sqliteCursor = connection.cursor()

    traceEvents = []
    
    #TODO:  When using X-phases we get problems if multiple activities have the same starttime. They'll be visualized stacked though they've
    #       been originally executed serially.
    #       Better use B/E phases here, but I can't think of a good way to get the hierarchy displayed without using much extra memory currently.
    
    getAllActivities = "SELECT * FROM activities ORDER BY ActivityId;"
    for row in sqliteCursor.execute(getAllActivities):
        duration = int(row[4]) - int(row[3])
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
    outfile = open(sys.argv[1] + ".json", "w")
    outfile.write(jsonData)
    outfile.close()
    
if __name__ == "__main__":
    main()


