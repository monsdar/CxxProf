
#include <iostream>
#include <cxxprof_static/CxxProf.h>

/**
 * This function prints a message `numberOfPrints` times
 */
void printSomething(unsigned int numberOfPrints)
{
    //We're plotting the numberOfPrints to see later why this function takes
    //longer in certain cases
    CXXPROF_PLOT("numberOfPrints", numberOfPrints);
    
    //Measure how long the function took
    CXXPROF_ACTIVITY(__FUNCTION__);
    
    //Just print the message a number of times
    for(unsigned int index = 0; index < numberOfPrints; ++index)
    {
        std::cout << "Hello World #" << index << std::endl;
    }
}

/**
 * Simply prints something, this should simulate a longer function...
 */
void someLongFunction()
{
    std::cout << "Some long function, what a ressource hogger!" << std::endl;
}

/**
 * The main-function. Need to say more?
 */
int main()
{
    //Init CxxProf and set aliases for this process and thread
    CXXPROF_INIT();
    CXXPROF_PROCESS_ALIAS("TutorialApp");
    CXXPROF_THREAD_ALIAS("MainThread");
    
    //measure how long it takes to run the entire main function
    CXXPROF_ACTIVITY("This is the main function");
    
    //this is our first batch of tests
    CXXPROF_MARK("PrintSomething tests");
    printSomething(1);
    printSomething(5);
    
    //in our second batch of tests we're just calling the long function
    CXXPROF_MARK("someLongFunction tests");
    { //let's create our own scope here
        CXXPROF_ACTIVITY("someLongFunction");
        someLongFunction();
    }
    
    //shutdown cleanly, this ends all activities that are still running and sends
    //outstanding data to the network
    CXXPROF_SHUTDOWN();
    
    return 0; //aaaand we're done
}
