
#include <boost/thread/thread.hpp>
#include <iostream>

int main()
{    
    while(true)
    {
        std::cout << "Hello World!" << std::endl;
        boost::this_thread::sleep( boost::posix_time::milliseconds(1000) );
    }
    
}
