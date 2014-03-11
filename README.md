#Brofiler

Brofiler is a manual instrumented Profiling library for C++. It provides mechanisms to add measurement-points to the code. At runtime these are 

##Goals
These are the main goals for the project:
* Easy integration
* Easily remove the lib during compile time
* Easily remove the lib during runtime
* Support for multithreaded applications
* Support for distributed systems
 
With the primary goals in mind the project aims to also achieve the following:
* Best performance possible
* Little overhead as possible

##Overall Architecture
Brofiler consists of 3 components:
* Client library - This is what you integrate into your code. The client sends measurement data to the server
* Server application - This receives and stores the data
* UI application - This queries the server and displays the data in a timeline

###Client
The client consists of a static and a dynamic library. You're compiling to the static library, which lazy loads the dynamic library during runtime. If the dynamic library could not be found, Brofiler will not run and limit its overhead to the minimum. This allows for switching between profiling and not profiling by just removing the dynamic library from your execution directory.

###Server
The server runs as a separate process on the local host or even on a remote machine. This allows for the storage of data to be as performant as possible. It also enables the profiling of a distributed system where different components run as different processes or even on different hosts. That way it is easily possible to measure networking code and how it performs during certain situations.

###UI
The UI application connects to the server and shows the data which has been recorded previously. The format is some kind of a timeline where the execution of the code can be seen over time. That way it is easily possible to see how your application behaves during its lifecycle and how different states affect the performance.
