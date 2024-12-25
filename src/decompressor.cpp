#include<bits/stdc++.h>
#include <fstream>
#define C '\xFF'
using namespace std;

void readBinaryFile(const string &filePath , const string &textFilePath) {

    map<string,char> Map ;

    ifstream file(filePath, ios::binary); // Open file in binary mode
    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return;
    }

    char byte;

    file.seekg(-1, ios::end); 
    streampos fileSize = file.tellg();
    file.read(&byte, sizeof(char));
    int padding = byte ;
    file.seekg(0,ios::beg);

    int numberOfCharacters ;
    file.read(reinterpret_cast<char*>(&numberOfCharacters), sizeof(int)) ;
    for(int i = 0 ; i < numberOfCharacters ; i++){

        file.read(&byte, sizeof(char)) ;
        char c = byte ;

        file.read(&byte, sizeof(char)) ;
        int numberOfBits = byte ;

        string bitsString = "" ;

        while (numberOfBits >= 8)
        {
            file.read(&byte, sizeof(char)) ;
            numberOfBits -= 8 ;

            for(int i = 7 ; i >= 0 ; i--){

                if(byte & (1<<i)){
                    bitsString +="1" ;
                }
                else bitsString +="0" ;
            }
        }

        if(numberOfBits > 0){
            file.read(&byte, sizeof(char)) ;
            for(int i = 7 ; i >= 8 - numberOfBits ; i--){

                if(byte & (1<<i)){
                    bitsString +="1" ;
                }
                else bitsString +="0" ;
            }
        }
        
        Map[bitsString] = c ;
    }//map is fetched 

    file.seekg(0, ios::cur); 
    streampos cur = file.tellg() ;

    ofstream rebuiltFile(textFilePath);
    if (!rebuiltFile.is_open()) {
        cout << "File could not be created" << endl;
        return;
    }

    string main = "" ;

    unsigned long long validBits = fileSize - cur ;


   if(file.is_open())
   { 
    while (file.peek() != EOF && file.good() && !file.eof() && validBits) { // Read one byte (8 bits) at a time

            file.get(byte) ;
            validBits-- ;
           if(validBits == 0){
                for(int i = 7 ; i >= padding ; i--){

                main += (byte & (1<<i)) ? "1" : "0" ;

                if(Map.find(main) != Map.end()){
                    char c = Map[main] ;
                    rebuiltFile.write(reinterpret_cast<const char *>(&c), sizeof(char)) ;
             
                    main = "" ;
                }
            }

           }else{
                for(int i = 7 ; i >= 0 ; i--){

                main += (byte & (1<<i)) ? "1" : "0" ;

                if(Map.find(main) != Map.end()){
                    char c = Map[main] ;
                    rebuiltFile.write(reinterpret_cast<const char *>(&c), sizeof(char)) ;
                    main = "" ;
                }
            } 
           }
        }

    }

    if (!file.eof() && file.fail()) {
        cerr << "Error reading file!" << endl;
    }
    rebuiltFile.close() ;
    file.close();
}
int main(int argc , char *argv[]){

    if(argc != 3){
        cerr << "Usage: " << argv[0] << "input_file_path output_file_path" << endl;
        return 1;
    }
    
    string binaryFilePath = argv[1] ;
    string textFilePath = argv[2] ;

    
    readBinaryFile(binaryFilePath,textFilePath);
    
}
