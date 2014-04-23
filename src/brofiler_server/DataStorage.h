
#ifndef _DATA_STORAGE_H_
#define _DATA_STORAGE_H_

#include "brofiler_dyn_network/NetworkObjects.h"

#include <boost/shared_ptr.hpp>
#include <sqlite3cc.h>

class DataStorage
{
public:
    DataStorage();
    virtual ~DataStorage();

    void storeResult(const Brofiler::NetworkObjects& objects);

private:
    void initDatabase(const std::string& filename);
    std::string getCurrentDateStr();

    boost::shared_ptr<sqlite::connection> connection_;

};

#endif //_DATA_STORAGE_H_
