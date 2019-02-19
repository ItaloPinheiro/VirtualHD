#ifndef SO_FINAL_VIRTUALHD_H
#define SO_FINAL_VIRTUALHD_H

#include <fstream>
#include <iostream>
#include <iomanip>
#include <map>

#include "Manager.h"
#include "Block.h"

using namespace std;

class VirtualHD : public Manager {
public:
    map <string, Block> blocks;
    map <string, Block>::iterator it;
    string HDname,
            currentPathAdr,
            replace = {17};
    int qtdB,
        sizeB;

    void actions(fstream &);
    void openHD();
    void loadBlocks(fstream&);
    void saveHD(fstream& fs);
    void createHD();
    void viewHD();
    void setParameters();
    void initialize(fstream &);
    const string& getHDname();
    const int getQtdB();
    const int getSizeB();
    void setHDname(const string&) ;
    void setQtdB(int) ;
    void setSizeB(int);
    string binaryConvertITS(int);
    unsigned int binaryConvertSTI(string);
    string stringToHex(string& input);
    void goBackToRoot();
    void openFolder();
    void viewFolder();
    void createFolder();
    void exitFolder();
    void viewFile();
    void createFile();
    void createContent(string, string);
    void resetBlock();
    string findEmptyBlock();
    string findFile(string name);
    string findFileToReset(string name);
    string findFolder(string name);
};

#endif //SO_FINAL_VIRTUALHD_H


