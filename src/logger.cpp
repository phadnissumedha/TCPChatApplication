#include <iostream>
#include <fstream>
#include <ctime>
#include "logger.h"
#include "config.h"

using namespace std;

/* Function to generate timestamp */
string getLogTimestamp()
{
    time_t now = time(0);
    struct tm *ltm = localtime(&now);

    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", ltm);

    return string(buffer);
}

void logMessage(string level, string message)
{
    string logFilePath = getConfigValue("LOG_FILE");

    ofstream logFile(logFilePath, ios::app);

    if(!logFile)
    {
        cout << "Error opening log file" << endl;
        return;
    }

    logFile << getLogTimestamp() << " | "
            << level << " | "
            << message << endl;

    logFile.close();
}