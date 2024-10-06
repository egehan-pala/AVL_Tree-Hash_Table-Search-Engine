
#include <iostream>
#include "AVLTree.h"
#include "hash.h"
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>
#include <cctype>

using namespace std;

struct DocumentItem {
    string documentName;
    int count;
    DocumentItem(){}
    DocumentItem(string documentName, int count=0): documentName(documentName), count(count)
    {}
};


struct WordItem {
    string word;
    vector<DocumentItem> documents;
    WordItem() {}
    WordItem(string wrd): word(wrd)
    {}
};

// Function to convert a string to lowercase
string toLower(string& str) {
    string temp;
    for(int i=0; i<str.length(); i++){
        temp += tolower(str[i]);
    }
    return temp;
}

//Function for to check if given document name exists in the listoffiles vector
bool checkVector(vector<string> &listOfFiles, string &docName){
    for(int i=0; i<listOfFiles.size(); i++){
        if(docName == listOfFiles[i]){
            return true;
        }
    }
    return false;
}


stringstream &operator>>(stringstream &ss, string &rhs) {
    string word;
    char nextChar;
    while (ss.get(nextChar)) {
        if (isalpha(nextChar)) {
            word += nextChar;
        } 
        else {
            if (!word.empty()) {
                rhs = word;  // Assign the accumulated word to rhs
                break;
            }
        }
    }
    return ss;
}


// Function to split a line of strings into single strings
vector<string> split(const string& str) {
    vector<string> queryWords;
    stringstream ss(str);
    string word;
    
    while (ss >> word) { // if there are empty spaces between strings split them and add to the vector
        queryWords.push_back(word);
    }
    
    return queryWords;
}

int main(){

    vector<string> fileNames;
    int num =0; int size =101;
    cout << "Enter number of input files: "; cin >> num;

    for(int i=1; i<=num; i++){
        string name;
        cout << "Enter " << i << ". file name: "; cin >> name;
        fileNames.push_back(name);
    }
    WordItem* notFound;
    HashTable<string, WordItem*> hashTable(notFound);
    AVLSearchTree<string, WordItem*> tree;

    for (int i= 0; i< fileNames.size(); i++){
        string line;
        string filename = fileNames[i];
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error opening file " << filename << endl; // if cannot open give error message and return 1
            return 1;
        }
        
        while (getline(file, line)){
            stringstream ss(line);
            string word;

            while (ss >> word) {
                
                // Convert word to lowercase
                word = toLower(word);

                // --------------------------- AVL Tree Operations --------------------------------

                treeNode<string, WordItem*>* temp = tree.traversal(word);
                WordItem* wordItem;

                // Check if the word is already in the tree
                if (temp == nullptr) {
                    // If not, create a new WordItem and insert it into the tree
                    wordItem = new WordItem(word);
                    tree.insert(word, wordItem);
                }

                temp = tree.traversal(word);
                wordItem = temp->value;
                // Increment count for the document
                bool documentFound = false;
                for (int i=0; i<wordItem->documents.size(); i++){
                    if (wordItem->documents[i].documentName == filename) { //if documentName vector contains the file name that we took the word from, increment the count of it
                        wordItem->documents[i].count++;
                        documentFound = true; // change the bool for found
                        break;
                    }
                }

                if (!documentFound) { //if bool is false (meaning the document does not exists in the documentName vector of the tree node) 
                    DocumentItem docItem(filename, 1); // insert the document name to the vector and set the count for one (as it is newly added)
                    wordItem->documents.push_back(docItem);
                }
                

                //--------------------------- Hash Table Operations -------------------------------


                // Assuming your hash table implementation uses functions like insert and find

                // 1. Search for the word in the hash table

                WordItem* wItem = hashTable.find(word);

                
                // 2. If word not found, create a new WordItem and insert it
                if (wItem == nullptr) {
                    wItem = new WordItem(word);
                    hashTable.insert(word, wItem);
                }  
                      
                


                // 3. Increment count for the document
                documentFound = false;
                for (int i= 0; i < wItem->documents.size(); i++) {
                    if (wItem->documents[i].documentName == filename) {
                        wItem->documents[i].count++;
                        documentFound = true;
                        break;
                    }
                }
                
                
                
                // 4. If document not found in wordItem->documents, add it
                if (!documentFound) {
                DocumentItem docItem(filename, 1);
                wItem->documents.push_back(docItem);
                }
                

            }
            
        }
        
        file.close();
    }


    cout << "\nAfter preprocessing, the unique word count is " << hashTable.getSize() << ". Current load ratio is " << hashTable.loadFactor() << endl;

    string query;

    cout << "Enter queried words in one line: "; 
    cin.ignore();
    getline(cin, query);
    if (query.rfind(" ") == query.length())
    {
        query = query.substr(0, query.length()-1);
    }


    vector<string> words_query;
    stringstream ss(query);
    string dummy;

    if(query.find(" ") != string::npos){
        while(ss >> dummy){// split the line of strings in to a vector
            words_query.push_back(dummy);
        } 
    }
    else {
        bool check = true;
        for(int i=0; i<query.length(); i++){
            if(!isalpha(query[i]))
            {
                check = false;
                break;
            }
        }
        if(check)
            words_query.push_back(query);
        else
        {
            while(ss >> dummy){// split the line of strings in to a vector
                words_query.push_back(dummy);
            } 
        }
    }

    

    for(int j=0; j< words_query.size(); j++){
        
        words_query[j] = toLower(words_query[j]); // lower case all the words
    }
    cout << endl;

    int k = 20;
    auto start = std::chrono::high_resolution_clock::now();
    for (int j = 0; j < k; j++)
    {
        if (j == 0) {

            bool firstOutput = true;
            for (int i=0; i<words_query.size(); i++) {
                treeNode<string, WordItem*>* temp = tree.traversal(words_query[i]); // search for node
                
                WordItem* wordItem= nullptr;
                if(temp != nullptr) // if exists continue
                {
                    wordItem = temp->value;
                }
                if (wordItem != nullptr) {// if exists continue
                    for (int j=0; j<wordItem->documents.size(); j++){ 
                        DocumentItem doc = wordItem->documents[j]; 
                        if(checkVector(fileNames, doc.documentName)){ //check whether the given document name exists in the first given document list
                            cout << "in Document " << doc.documentName << ", "; // if so cout
                        }
                        cout << wordItem->word << " found " << doc.count << " times." << endl;
                        /*
                        if(j != wordItem->documents.size()-1){
                            cout << ", ";
                        }
                        else{
                            cout<< ".";
                        }
                        */
                        firstOutput = false; 
                    }
                    cout << endl;
                }
            }
            if (firstOutput) { // If query word does not exist in the tree cout error message
                cout << "No document contains the given query" << endl;;
            }
        }
    }
    auto BSTTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);
   
    start = std::chrono::high_resolution_clock::now();
    for (int j = 0; j < k; j++)
    {
         // QueryDocuments (with hashtable);
        if (j == 0) {


            bool firstOutput = true;
            for (int i=0; i<words_query.size(); i++) {
                WordItem* wordItem = hashTable.find(words_query[i]); // search for node

                if (wordItem != nullptr) {// if exists continue
                    for (int j=0; j<wordItem->documents.size(); j++){ 
                        DocumentItem doc = wordItem->documents[j]; 
                        if(checkVector(fileNames, doc.documentName)){ //check whether the given document name exists in the first given document list
                            cout << "in Document " << doc.documentName << ", "; // if so cout
                        }
                        cout << wordItem->word << " found " << doc.count << " times." << endl;

                        /*
                        if(j != wordItem->documents.size()-1){
                            cout << ", ";
                        }
                        else{
                            cout<< ".";
                        }
                        */
                        firstOutput = false; 

                    }
                    cout << endl;
                }
            }
            if (firstOutput) { // If query word does not exist in the tree cout error message
                cout << "No document contains the given query" << endl;;
            }
        }
    }
    auto HTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
                    (std::chrono::high_resolution_clock::now() - start);

    cout << "\nTime: " << BSTTime.count() / k << "\n";
    cout << "\nTime: " << HTTime.count() / k << "\n";
    cout << "\nSpeed up: " << float((float(BSTTime.count()))/ float(HTTime.count())) << endl;


    return 0; 
}




