#include "LogStorage.h"
#include "Logger.h"
/**
 * @brief Bind to the card.
 */
LogStorage::LogStorage(SdCardService &sdCardService) : sdCardService(sdCardService)
{
}

/**
 * @brief Append one line to the log file, if the card is ready.
 * @param log The line to write.
 * @warning Blocking; costs a frame when called from the UI thread.
 */
void LogStorage::WriteLog(String log)
{
    if (!sdCardService.IsReady())
        return;

    File file = sdCardService.GetFileSystem().open("/LOG.txt", FILE_APPEND);
    if (file)
    {
        LOG_DEBUG("Writing log to file:", log);
        file.println(log);

        file.close();
    }
}
