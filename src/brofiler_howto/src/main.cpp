
#include "brofiler/Brofiler.h"

#include <boost/thread/thread.hpp>

int main()
{
    Brofiler bro;
    
    while(true)
    {
        bro.printHello();
        boost::this_thread::sleep( boost::posix_time::milliseconds(1000) );
    }
    
}
