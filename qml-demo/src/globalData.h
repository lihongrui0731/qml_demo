#ifndef GLOBALDATA_H
#define GLOBALDATA_H

#include<QString>
class GlobalData
{
public:
    static QString CONFIG_DIR;
    static QString CONFIG_FILE;

    GlobalData(){
        this->CONFIG_DIR = "123";
        this->CONFIG_FILE = "123";
    };
    ~GlobalData(){

    };
};
#endif // GLOBALDATA_H
