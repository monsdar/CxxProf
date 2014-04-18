
#include "brofiler_static/BrofilerStatic.h"

#include <boost/date_time/posix_time/posix_time.hpp>

#include <boost/thread/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>

boost::shared_ptr<BrofilerStatic> mainBro_;

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
    
    //Adding PlotValues helps us to see how the application is handling the change of values over time
    mainBro_->addPlotValue("SomeValue", someValue);

    if (someValue == 3)
    {
        //Let's add some complexity by sleeping a bit when someValue reaches 3
        boost::shared_ptr<IActivity> act;
        act = mainBro_->createActivity("recursiveOperation::middle");
        boost::this_thread::sleep(boost::posix_time::milliseconds(200));
    }
    if(someValue < 5)
    {
        someValue = recursiveOperation(++someValue);
    }

    return someValue;
}

int main()
{
    std::cout << "Creating BrofilerStatic" << std::endl;
    mainBro_.reset( new BrofilerStatic() );
    
    std::cout << "Loading DynBrofiler" << std::endl;
    mainBro_->loadDynBrofiler();

    //Measure how long the rest of this main() takes
    boost::shared_ptr<IActivity> act;
    act = mainBro_->createActivity("main");
    
    //Add a mark for a better overview of which state the application is currently in
    mainBro_->addMark("LongOperation start");
    longOperation();
    longOperation();

    //And another mark
    mainBro_->addMark("RecursiveOperation start");
    recursiveOperation(1);
}
