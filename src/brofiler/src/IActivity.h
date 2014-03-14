
#ifndef _I_ACTIVITY_H_
#define _I_ACTIVITY_H_

class IActivity
{
public:
    virtual IActivity() {};
    virtual std::string getName() const = 0;
};

#endif //_I_ACTIVITY_H_
