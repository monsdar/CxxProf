#!/usr/bin/env python

import fnmatch
import os
import subprocess
import sys


#set pathes
DEV_PATH = os.path.dirname(os.path.realpath(__file__)) + '/../'
BUILD_PATH = DEV_PATH + "/build/"
INSTALL_PATH = DEV_PATH + "/install/"
THIRDPARTY_PATH = DEV_PATH + "/thirdparty/"

CTEST_EXE = DEV_PATH + "/thirdparty/cmake/ctest"
#NOTE: append the .exe suffix if we're on Windows
if(os.name == "nt" ):
    CTEST_EXE += ".exe"
    
if("--native-ctest" in sys.argv):
    CTEST_EXE = "ctest"
    sys.argv.remove("--native-ctest")

def main():
    #set the PATH to find Thirdparty on Windows systems
    if(os.name == "nt" ):
        os.environ['PATH'] = THIRDPARTY_PATH + '/boost/bin/'
        os.environ['PATH'] += ';' + THIRDPARTY_PATH + '/pluma/bin/'
        os.environ['PATH'] += ';' + THIRDPARTY_PATH + '/cmake/'
    
        #find our own components, set the PATH for them
        matches = []
        for root, dirnames, filenames in os.walk( INSTALL_PATH ):
            for filename in fnmatch.filter(dirnames, 'bin'):
                matches.append(os.path.join(root, filename))
        for path in matches:
            os.environ['PATH'] = os.environ['PATH'] + ';' + path
    
    #search for projects which need to be tested
    for root, dirnames, filenames in os.walk( BUILD_PATH ):
        for filename in fnmatch.filter(filenames, 'CTestTestfile.cmake'):            
            os.chdir( root )
            
            #run the tests
            testCmd = []
            testCmd.append(CTEST_EXE)
            testCmd.append("--no-compress-output")
            testCmd.append("-T")
            testCmd.append("Test")
            testCmd.append(".")
            process = subprocess.Popen(testCmd)
            process.wait()
            print "Tests executed with errorcode: " + str(process.returncode)

if __name__=="__main__":
    main()
