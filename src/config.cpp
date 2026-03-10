#include <fstream>
#include <string>
#include <sstream>
#include "config.h"

using namespace std;

string getConfigValue(const string& key) {

    ifstream file("config.ini");
    string line;

    while(getline(file, line)) {

        // Ignore section headers
        if(line[0] == '[')
            continue;

        size_t pos = line.find('=');

        if(pos != string::npos) {

            string k = line.substr(0,pos);
            string value = line.substr(pos+1);

            if(k == key)
                return value;
        }
    }

    return "";
}