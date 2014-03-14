
import fnmatch
import os
import shutil
import subprocess
import sys
import multiprocessing
from xml.etree import ElementTree;

#set pathes
DEV_PATH = os.path.dirname(os.path.realpath(__file__)) + '/../'
BUILD_PATH = DEV_PATH + "/build/"
CMAKE_PATH = DEV_PATH + "/cmake/"
INSTALL_PATH = DEV_PATH + "/install/"
SRC_PATH = DEV_PATH + "/src/"
CMAKE_EXE = DEV_PATH + "/thirdparty/cmake/cmake.exe"

#NOTE:  This switches the parallel build of components on/off
#       It's currently turned off because we had problems with gotest-release and its log-files
MULTITHREADED = True

#NOTE:  This performs the build with an alternative Platform Toolset instead of using the installed version
#       Example: It allows to use Visual Studio 2013 with a v100 toolset
#USE_ALTERNATIVE_TOOLSET = 'v90'
USE_ALTERNATIVE_TOOLSET = ''


class XmlComponent():
    def __init__(self):
        self.name = ""
        self.dependencies = []
      
def xmlStrip(givenString):
    if(givenString == None):
        givenString = ''
    givenString = givenString.strip('\n')
    givenString = givenString.strip(' ')
    return givenString
    
def readComponents():
    components = []
    
    #check if there is a cfg
    depConfig = os.path.dirname(os.path.realpath(__file__)) + '/dependencies.xml'
    if not( os.path.exists(depConfig) ):
        print "No dependencies.xml exists"
        return components
    
    tree = ElementTree.parse(depConfig)
    xmlComponents = tree.findall('component')
    for xmlComponent in list(xmlComponents):
        newComp = XmlComponent()
        newComp.name = xmlStrip( xmlComponent.find('name').text )
        
        xmlDependencies = xmlComponent.find('dependencies').findall('dependency')
        for xmlDependency in list(xmlDependencies):
            newDep = xmlStrip( xmlDependency.text )
            newComp.dependencies.append( newDep )            
        components.append( newComp )
            
    return components

class Component():
    def __init__(self, cmakePath):
        self.fullpath = os.path.dirname(os.path.realpath( cmakePath ))
        self.name = os.path.basename( self.fullpath )
        self.buildpath = BUILD_PATH + "/" + self.name
        self.dependencies = []

def createBuildFiles(component):    
    #create the build environment with cmake
    try:
        os.mkdir( BUILD_PATH )
    except:
        pass
    try:
        os.mkdir( component.buildpath )
    except:
        pass
    
    os.chdir( component.buildpath )
    outLog = open('cmake_' + component.name + '_out.log', 'wb')
    errLog = open('cmake_' + component.name + '_err.log', 'wb')
    
    cmakeCreate = []
    cmakeCreate.append( CMAKE_EXE )
    cmakeCreate.append( component.fullpath )
    cmakeCreate.append( "-DCMAKE_INSTALL_PREFIX=" + INSTALL_PATH )
    
    if( USE_ALTERNATIVE_TOOLSET ):
        cmakeCreate.append( "-T" )
        cmakeCreate.append( USE_ALTERNATIVE_TOOLSET )
    
    process = subprocess.Popen( cmakeCreate, stdout=outLog, stderr=errLog )
    process.wait()
    if not( process.returncode == 0 ):
        print "Creating Build-files for " + component.name + " exited with ErrorCode: " + str(process.returncode)
        exit(process.returncode)
    else:
        print "|    ..." + component.name.ljust(20, ' ') + " - " + "cmake".ljust(20, ' ') + " - SUCCESSFUL"
        
def build(component, type):        
    os.chdir( component.buildpath )
    outLog = open('build_' + component.name + '_' + type + '_out.log', 'wb')
    errLog = open('build_' + component.name + '_' + type + '_err.log', 'wb')
    
    cmakeBuild = []
    cmakeBuild.append( CMAKE_EXE )
    cmakeBuild.append( "--build" )
    cmakeBuild.append( "." )
    cmakeBuild.append( "--target" )
    cmakeBuild.append( "install" )
    cmakeBuild.append( "--config" )
    cmakeBuild.append( type )
    process = subprocess.Popen( cmakeBuild, stdout=outLog, stderr=errLog )
    process.wait()
    if not( process.returncode == 0 ):
        print "Building " + type + " for " + component.name + " exited with ErrorCode: " + str(process.returncode)
        exit(process.returncode)
    else:
        print "|    ..." + component.name.ljust(20, ' ') + " - " + type.ljust(20, ' ') + " - SUCCESSFUL"
        
class ExecWorker(multiprocessing.Process):
    def __init__(self, component):
        self.component = component
        multiprocessing.Process.__init__(self)
        
    def run(self):
        #build the component
        createBuildFiles(self.component)
        build(self.component, "release")
        build(self.component, "debug")
    
def cleanup():
    #clean build directory
    try:
        shutil.rmtree( BUILD_PATH )
    except:
        pass
        
    #clean install directory
    try:
        shutil.rmtree( INSTALL_PATH )
    except:
        pass
        
    #remove the Find-Scripts
    for root, dirnames, filenames in os.walk( CMAKE_PATH ):
        for filename in fnmatch.filter(filenames, 'Find*.cmake'):
            try:
                os.remove( os.path.join(root, filename) )
            except:
                pass
    
def main():

    #clean the development if parameter clean is given
    if( sys.argv[-1].lower() == "clean"):
        print "Cleaning up..."
        cleanup();
    
    #first read the dependency config
    dependencies = readComponents()
    
    #search for all components by their CMakeLists.txt
    components = []
    for root, dirnames, filenames in os.walk( SRC_PATH ):
      for filename in fnmatch.filter(filenames, 'CMakeLists.txt'):
            newComp = Component( os.path.join(root, filename) )
            for depComp in dependencies:
                if(depComp.name == newComp.name):
                    newComp.dependencies = depComp.dependencies
            components.append(newComp)
          
    #let's keep a list of components we already built,
    #this is needed to see if all dependencies have been built
    doneComponents = []
    while( components ): #check if the components-list is empty
    
        #this contains the components we will build this iteration
        buildComponents = []
        
        #check which components we can build, according to their dependencies
        for comp in components:
            buildComp = True
            for depName in comp.dependencies:
                if depName in doneComponents:
                    pass #the above if does not work with 'not'... so let's just put the actual code in the else-branch and pass here
                else:
                    buildComp = False  
            
            if(buildComp):
                buildComponents.append(comp)
                
        #now remove the soon to be built components from our components list
        for comp in buildComponents:
            components.remove(comp)

        #check if there are components to build, if not and there are still some left we have a dependency problem
        if( (not buildComponents) and components):
            print "=========== ERROR =============="
            print "There seems to be a dependency problem, cannot resolve all dependencies!!"
            print "Compiled Components:"
            for comp in doneComponents:
                print " -- " + comp
            print "Components left:"
            for comp in components:
                print " -- " + comp.name
            exit(-1)

        print ""
        print " ---------------------------------------------------------------------------"
        print "| We're building the following components in this iteration:"
        for comp in buildComponents:
            print "| -- " + comp.name
        
        #build the solutions in threads, toggle MULTITHREADED-flag to switch between serial/parallel build
        buildWorkers = []
        for comp in buildComponents:
            newWorker = ExecWorker(comp)
            buildWorkers.append(newWorker)
            
            #add the component to the list of finished components (it's easier to do it here)
            doneComponents.append(comp.name)
                
        for worker in buildWorkers:
            worker.start()
            if( not MULTITHREADED ):
                #wait for the worker immediately if multithreading is turned off
                worker.join()
        
        #wait for the threads after all have been started if multithreading is turned on
        if( MULTITHREADED ):
            for worker in buildWorkers:
                worker.join()
                
        for worker in buildWorkers:
            if( not worker.exitcode == 0 ):
                print "Errors occured, stopping the build..."
                exit(-1)
                
    print "================================="
    print "====== BUILD SUCCESSFUL ========="
    print "================================="
        
if __name__=="__main__":
    main()

