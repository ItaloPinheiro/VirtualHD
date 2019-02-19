#include <bitset>
#include <map>
#include <cmath>
#include <string>

#include "VirtualHD.h"

#define BLOCKSIZE_A 7
#define BLOCKSIZE_C 5
#define BLOCKSIZE_P 5
#define END_OF_CONTENT 65535

using namespace std;

void VirtualHD::actions(fstream& fs) {
    string option;
    loadBlocks(fs);
    do{
        cout << Manager::getPath();
        cin >> option;
        cin.get();
        if(option == "create"){
            createFile();
        }else if(option == "createdir") {
            createFolder();
        }else if(option == "cd"){
            openFolder();
        }else if(option == "cd.."){
            exitFolder();
        }else if(option == "cd\\") {
            goBackToRoot();
        }else if(option == "type"){
            viewFile();
        }else if(option == "typehd") {
            viewHD();
        }else if(option == "dir"){
            viewFolder();
        }else if(option == "remove"){
            resetBlock();
        }else
            cout << "Comando nao existente!" << endl;
    }while(option != "exit");

    Manager::resetPath();
}

//region HDFunctions

void VirtualHD::openHD(){
    fstream in,
            out;
    Manager::addPath(HDname);
    currentPathAdr = binaryConvertITS(0);
    in.open(HDname + ".txt", fstream::in);
    actions(in);
    in.close();
    out.open(HDname + ".txt", fstream::out);
    saveHD(out);
    out.close();
}

void VirtualHD::loadBlocks(fstream& fs){
    unsigned char flag;
    string blockAdrActual,
            blockAdrDad,
            blockAdrContent,
            blockInfo;

    while (fs >> flag){
        switch(flag){
            case 'V': {
                fs >> blockAdrActual >> blockAdrDad >> blockInfo >> blockAdrContent;
                Block block(flag, blockAdrActual, blockAdrDad, blockInfo, blockAdrContent);
                blocks[blockAdrActual] = block;
                break;
            }
            case 'P':{
                fs >> blockAdrActual >> blockAdrDad >> blockInfo;
                blockInfo = Manager::regexFunction(replace, " ", blockInfo);
                Block block(flag, blockAdrActual, blockAdrDad, blockInfo);
                blocks[blockAdrActual] = block;
                break;
            }
            case 'A':{
                fs >> blockAdrActual >> blockAdrDad >> blockInfo >> blockAdrContent;
                blockInfo = Manager::regexFunction(replace, " ", blockInfo);
                Block block(flag, blockAdrActual, blockAdrDad, blockInfo, blockAdrContent);
                blocks[blockAdrActual] = block;
                break;
            }
            case 'C':{
                fs >> blockAdrActual >> blockAdrDad >> blockInfo;
                blockInfo = Manager::regexFunction(replace, " ", blockInfo);
                Block block(flag, blockAdrActual, blockAdrDad, blockInfo);
                blocks[blockAdrActual] = block;
            }
        }
    }
}

void VirtualHD::viewHD() {
    for(it = blocks.begin(); it != blocks.end(); ++it){
        if(it->second.flag != 'V'){
            if(it->second.flag == 'P'){
                cout << binaryConvertSTI(it->second.blockAdrActual) << " " << (char)it->second.flag << " " << binaryConvertSTI(it->second.blockAdrDad) << " " << it->second.blockName << endl
                    << stringToHex(it->second.blockAdrActual) << " " << 50 << " " << stringToHex(it->second.blockAdrDad) << " " << stringToHex(it->second.blockName) << endl;
            }
            else if(it->second.flag == 'C'){
                cout << binaryConvertSTI(it->second.blockAdrActual) << " " << (char)it->second.flag << " " << binaryConvertSTI(it->second.blockAdrDad) << " " << it->second.blockName << endl
                    << stringToHex(it->second.blockAdrActual) << " " << 43 << " " << stringToHex(it->second.blockAdrDad) << " " << stringToHex(it->second.blockName) << endl;
            }
            else if(it->second.flag == 'A'){
                cout << binaryConvertSTI(it->second.blockAdrActual) << " " << (char)it->second.flag  << " " << binaryConvertSTI(it->second.blockAdrDad) << " " << it->second.blockName << " " << binaryConvertSTI(it->second.blockAdrContent) << endl
                    << stringToHex(it->second.blockAdrActual) << " " << 41 << " " << stringToHex(it->second.blockAdrDad) << " " << stringToHex(it->second.blockName) << " " << stringToHex(it->second.blockAdrContent) << endl;
            }
        }
    }
}

void VirtualHD::saveHD(fstream& fs){
    for(it = blocks.begin(); it != blocks.end(); ++it)
        if(it->second.flag == 'P' || it->second.flag == 'C')
            fs << it->second.flag << " " << it->second.blockAdrActual << " " << it->second.blockAdrDad << " " << Manager::regexFunction(" ", replace, it->second.blockName) << endl;
        else if(it->second.flag == 'V' || it->second.flag == 'A')
            fs << it->second.flag << " " << it->second.blockAdrActual << " " << it->second.blockAdrDad << " " << Manager::regexFunction(" ", replace, it->second.blockName) << " " << Manager::regexFunction(" ", replace, it->second.blockAdrContent) << endl;
}

void VirtualHD::createHD(){
    fstream fs;
    fs.open(HDname + ".txt", fstream::out);
    initialize(fs);
    fs.close();
}

void VirtualHD::setParameters(){
    cin >> HDname >> sizeB >> qtdB;
}

void VirtualHD::initialize(fstream& fs) {
    int i;
    string addressIni = binaryConvertITS(0);
    currentPathAdr = addressIni;
    fs << "P" << " " << addressIni << " " << addressIni << " " << "ROOT" << "\n";
    for(i = 1; i < qtdB; i++){
        string address = binaryConvertITS(i);
        fs << "V" << " " << address << " " << addressIni << " " << "#" << " " << addressIni << "\n";
    }
}

void VirtualHD::goBackToRoot() {
    currentPathAdr = binaryConvertITS(0);
    Manager::resetPath();
    Manager::addPath(HDname);
}

const string& VirtualHD::getHDname(){
    return HDname;
}

const int VirtualHD::getQtdB(){
    return qtdB;
}

const int VirtualHD::getSizeB(){
    return sizeB;
}

void VirtualHD::setHDname(const string &HDname) {
    this->HDname = HDname;
}

void VirtualHD::setQtdB(int qtdB) {
    this->qtdB = qtdB;
}

void VirtualHD::setSizeB(int sizeB) {
    this->sizeB = sizeB;
}

//endregion

//region BinaryFunctions

string VirtualHD::binaryConvertITS(int number){
    return bitset<16>(number).to_string();
}

unsigned int VirtualHD::binaryConvertSTI(string str){
    return bitset<16>(str).to_ulong();
}

string VirtualHD::stringToHex(string& input)
{
    char lut[] = "0123456789ABCDEF";
    size_t len = input.length();
    string output;
    output.reserve(2 * len);
    for (size_t i = 0; i < len; ++i)
    {
        const unsigned char c = input[i];
        output.push_back(lut[c >> 4]);
        output.push_back(lut[c & 15]);
    }
    return output;
}

//endregion

//region FolderFunctions

void VirtualHD::openFolder() {
    string folderName,
            pos;
    getline(cin, folderName);
    pos = findFolder(folderName);
    if(pos.empty())
        cout << "Pasta nao existe!" << endl;
    else{
        currentPathAdr = pos;
        Manager::addPath(folderName);
    }
}

void VirtualHD::viewFolder() {
    for(it = blocks.begin(); it != blocks.end(); ++it)
        if(it->second.flag != 'V' && it->second.blockAdrDad == currentPathAdr)
            cout << it->second.flag << " " << it->second.blockName << endl;
}

void VirtualHD::createFolder(){
    string folderName, nextBlock, exists;
    getline(cin, folderName);
    exists = findFolder(folderName);
    if(exists.empty()){
        nextBlock = findEmptyBlock();
        if(!nextBlock.empty()){
            blocks[nextBlock].flag = 'P';
            blocks[nextBlock].blockAdrDad = currentPathAdr;

            //validate if the folderName has a acceptable size
            if(folderName.size() > sizeB - BLOCKSIZE_P)
                folderName = folderName.substr(0, sizeB - BLOCKSIZE_P);

            blocks[nextBlock].blockName = folderName;
        }else
            cout << "HD cheio !!!" << endl;
    }else
        cout << "Pasta ja existente" << endl;
}

void VirtualHD::exitFolder() {
    Manager::removePath();
    currentPathAdr = blocks[currentPathAdr].blockAdrDad;
}

//endregion

//region FileFunctions

void VirtualHD::viewFile(){
    string fileName,
            pos;
    getline(cin, fileName);
    pos = findFile(fileName);
    if(pos.empty())
        cout << "Arquivo nao existe, verifique os parametros" << endl;
    else{
        while(pos != binaryConvertITS(END_OF_CONTENT)){
            cout << blocks[pos].blockName;
            pos = blocks[pos].blockAdrDad;
        }
        cout << endl;
    }
}

void VirtualHD::createFile(){
    string fileName,
            exists,
            nextBlock,
            nextBlockContent;
    getline(cin, fileName);
    exists = findFile(fileName);
    if(exists.empty()){
        nextBlock = findEmptyBlock();
        if(!nextBlock.empty()){
            blocks[nextBlock].flag = 'A';
            blocks[nextBlock].blockAdrDad = currentPathAdr;

            //validate if fileName has a acceptable size
            if(fileName.size() > sizeB - BLOCKSIZE_A)
                fileName = fileName.substr(0, sizeB - BLOCKSIZE_A);

            blocks[nextBlock].blockName = fileName;
            nextBlockContent = findEmptyBlock();

            if(!nextBlockContent.empty()){
                cin.get();
                blocks[nextBlock].blockAdrContent = nextBlockContent;
                string content;
                getline(cin, content);
                createContent(content, nextBlockContent);
            }else
                cout << "HD cheio !!!" << endl;
        }else
            cout << "HD cheio !!!" << endl;
    }else
        cout << "Arquivo ja existente" << endl;
}

void VirtualHD::createContent(string content, string nextBlockContent){
    string prevBlockContent = nextBlockContent;
    int size = sizeB - BLOCKSIZE_C;
    int qtdBlocks = ceil(content.size() / (double)size);
    int start = 0,
            end = size;
    blocks[nextBlockContent].flag = 'C';
    string temp = content.substr(start, size);
    blocks[nextBlockContent].blockName = temp;

    for(int i = 1; i < qtdBlocks; i++){
        nextBlockContent = findEmptyBlock();
        if(!nextBlockContent.empty()) {
            blocks[nextBlockContent].flag = 'C';
            blocks[prevBlockContent].blockAdrDad = nextBlockContent;
            start += size;
            end += size;
            temp = content.substr(start, size);
            blocks[nextBlockContent].blockName = temp;
        }else
            cout << "HD cheio - atencao, o arquivo foi parcialmente gravado" << endl;

        prevBlockContent = nextBlockContent;
    }
    blocks[prevBlockContent].blockAdrDad = binaryConvertITS(END_OF_CONTENT);
}

void VirtualHD::resetBlock() {
    string name,
            pos;
    cin >> name;
    pos = findFileToReset(name);
    if(pos.empty())
        cout << "Arquivo nao existe, verifique os parametros" << endl;
    else{
        while(pos != binaryConvertITS(END_OF_CONTENT)){
            string dad, child;
            blocks[pos].blockName = "#";
            if(blocks[pos].flag == 'A'){
                blocks[pos].blockAdrDad = binaryConvertITS(0);
                dad = blocks[pos].blockAdrContent;
                blocks[pos].blockAdrContent = binaryConvertITS(0);
                blocks[pos].flag = 'V';
                pos = dad;
            }else{
                child = blocks[pos].blockAdrDad;
                blocks[pos].blockAdrDad = binaryConvertITS(0);
                blocks[pos].blockAdrContent = binaryConvertITS(0);
                blocks[pos].flag = 'V';
                pos = child;
            }
        }

        cout << "Arquivo " << name << " removido com sucesso" << endl;
    }
}

//endregion

//region SearchFunctions

string VirtualHD::findEmptyBlock(){
    for(it = blocks.begin(); it != blocks.end(); ++it)
        if(it->second.flag == 'V')
            return it->second.blockAdrActual;

    return "";
}

string VirtualHD::findFile(string name){
    for(it = blocks.begin(); it != blocks.end(); ++it)
        if(it->second.flag == 'A' && it->second.blockName == name && it->second.blockAdrDad == currentPathAdr)
            return it->second.blockAdrContent;

     return "";
}

string VirtualHD::findFolder(string name){
    for(it = blocks.begin(); it != blocks.end(); ++it)
        if(it->second.flag == 'P' && it->second.blockName == name && it->second.blockAdrDad == currentPathAdr)
            return it->second.blockAdrActual;

    return "";
}

string VirtualHD::findFileToReset(string name){
    for(it = blocks.begin(); it != blocks.end(); ++it)
        if(it->second.flag == 'A' && it->second.blockName == name && it->second.blockAdrDad == currentPathAdr)
            return it->second.blockAdrActual;

    return "";
}
//endregion
