#ifndef PARAMETERUPDATER_H
#define PARAMETERUPDATER_H

#include "DatabaseTool.h"
#include <string>

class ParameterUpdater {
public:
    void init(DatabaseTool* db);
    void updateParameter(const std::string& data);

private:
    DatabaseTool* database;
};

#endif  // PARAMETERUPDATER_H
