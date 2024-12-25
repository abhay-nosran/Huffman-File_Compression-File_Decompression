#include<bits/stdc++.h>
#include <fstream>
// #define C '!'
#define C '\xFF'
using namespace std;

class huffmantreeNode{

    public :
    huffmantreeNode * left ;
    huffmantreeNode * right ;
    char character ;
    int frequency ;

    huffmantreeNode(int frequency , char character){
        
        this -> frequency = frequency ;
        this -> character = character ;
        left = NULL ;
        right = NULL ;
    }
};
class compare{
    public :
      bool operator()(huffmantreeNode * below,huffmantreeNode * above){
        return below -> frequency > above -> frequency ;
      }
};
void compressionTable(huffmantreeNode * root , string s , map<char,string>& m){

    if(!root) return ;

    if ( root -> character != C)
    {
         m[root -> character] = s ;
    }

       
    compressionTable(root -> left , s + '0' , m) ;
    compressionTable(root -> right , s + '1' , m) ;
    
}
huffmantreeNode *  huffmanTree_generator(map<char,int>& m){
    
    priority_queue<huffmantreeNode *,vector<huffmantreeNode *>,compare> min_heap  ;
    for(auto i : m){
        huffmantreeNode * Node = new huffmantreeNode(i.second,i.first) ;
        min_heap.push(Node) ;
    }

    while(min_heap.size() > 1)
    {
       huffmantreeNode * left = min_heap.top(); min_heap.pop();
       huffmantreeNode * right = min_heap.top();  min_heap.pop();
       huffmantreeNode * Node = new huffmantreeNode(left ->frequency + right -> frequency , C) ;
       Node -> left = left ;
       Node -> right = right ;

       min_heap.push(Node) ;
    }

    return min_heap.top() ;
}
map<char,int> frequencyCalc(string file_path){
   
   map<char,int> m ;
   char c ;
   fstream input_file(file_path);

   if(input_file.is_open()){

         while (input_file.good() && !input_file.eof())
        {
           input_file.get(c) ;
           m[c]++ ;
        }   
   }

   if (!input_file.eof() && input_file.fail())
    {
        cout << "Error in Reading File " << endl ;
    }
    
    
    input_file.close() ;

    return m ;

}
void preorder(huffmantreeNode * root){
    if(!root) return ;

    cout << root ->character << "->" << root -> frequency << endl ;
    preorder(root -> left);
    preorder(root -> right);
}

void compression(string &file_path, map<char, string> &m,string outputFilePath) {

    ofstream file(outputFilePath, ios::binary | ios::app);
    if (!file.is_open()) {
        cout << "File could not be created" << endl;
        return;
    }

    // Step 1: Write the map to the file
    int mapSize = m.size();
    file.write(reinterpret_cast<const char *>(&mapSize), sizeof(int)); // Write map size

    for (auto it : m) {

        char key = it.first ;
        string value = it.second ;

        file.write(reinterpret_cast<const char *>(&key), sizeof(char)); // Write the key (char)

        char valueSize = value.size(); // Write the size of the value string
        file.write(reinterpret_cast<const char *>(&valueSize), sizeof(char));

        // Write the value string as binary
        char buffer = 0;
        int bitCount = 0;

        for (char bit : value) {
            buffer = (buffer << 1) | (bit - '0'); // Build the binary buffer
            bitCount++;

            if (bitCount == 8) { // Write when a full byte is formed
                file.write(reinterpret_cast<const char *>(&buffer), sizeof(char));
                buffer = 0;
                bitCount = 0;
            }
        }

        // Write remaining bits with padding
        if (bitCount > 0) {
            buffer <<= (8 - bitCount); // Pad the remaining bits with 0s
            file.write(reinterpret_cast<const char *>(&buffer), sizeof(char));
        }
    }
    
    ifstream input_file(file_path);
    if (!input_file.is_open()) {
        cout << "Input file could not be opened" << endl;
        return;
    }

    string bitBuffer = "";
    char c;
    while (input_file.get(c)) {
        bitBuffer += m[c]; // Add Huffman code to the buffer

        while (bitBuffer.size() >= 8) {
            // Write the first 8 bits
            char byte = 0;
            for (int i = 0; i < 8; i++) {
                byte = (byte << 1) | (bitBuffer[i] - '0');
            }

            file.write(reinterpret_cast<const char *>(&byte), sizeof(char));
            bitBuffer.erase(0, 8); // Remove the written bits
        }
    }

    // Write remaining bits with padding
    if (!bitBuffer.empty()) {
        char byte = 0;
        for (size_t i = 0; i < bitBuffer.size(); i++) {
            byte = (byte << 1) | (bitBuffer[i] - '0');
        }
        byte <<= (8 - bitBuffer.size()); // Pad the remaining bits
        file.write(reinterpret_cast<const char *>(&byte), sizeof(char));
    }
    char padding = (8 - bitBuffer.size())%8 ;
    file.write(reinterpret_cast<const char *>(&padding), sizeof(char));

    file.close();
    input_file.close();

}



int main(int argc , char *argv[]){

    if(argc != 3){
        cerr << "Usage: " << argv[0] << "input_file_path output_file_path" << endl;
        return 1;
    }
    
    string file_path = argv[1] ;
    string outputFilePath = argv[2] ;

    // cout << file_path << endl ;
    // cout << outputFilePath << endl ;
    map <char , int > frequency_map = frequencyCalc(file_path);
    huffmantreeNode * node = huffmanTree_generator(frequency_map);
    map<char , string> m ;
    compressionTable(node,"",m) ;
    compression(file_path,m,outputFilePath) ;

}
