
#include "brofiler/Brofiler.h"
#include "brofiler_dyn/NetworkBrofiler.h"

#include <boost/thread/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>

boost::shared_ptr<Brofiler> mainBro_;

void longOperation()
{
    boost::shared_ptr<IActivity> act;
    act = mainBro_->createActivity("longOperation");

    boost::this_thread::sleep(boost::posix_time::seconds(1));
}

int recursiveOperation(int someValue)
{
    boost::shared_ptr<IActivity> act;
    act = mainBro_->createActivity("recursiveOperation");
    
    if(someValue < 5)
    {
        someValue = recursiveOperation(++someValue);
    }
    
    return someValue;
}

int main()
{
    std::cout << "Creating Brofiler" << std::endl;
    mainBro_.reset( new Brofiler() );
    
    std::cout << "Loading DynBrofiler" << std::endl;
    mainBro_->loadDynBrofiler();
    
    //let's call some of the above mentioned functions
    //of course we're measuring how long this test takes altogether
    boost::shared_ptr<IActivity> act;
    act = mainBro_->createActivity("main");
    
    longOperation();
    longOperation();
    recursiveOperation(1);    
}
