#ifndef LOGSTORAGE_H
#define LOGSTORAGE_H

#include "SdCardService.h"

class LogStorage {
public:
    LogStorage(SdCardService &sdCardService);
    void WriteLog(String log);

private:
    SdCardService &sdCardService;
};

#endif //LOGSTORAGE_H
