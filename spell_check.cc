// Danny You
#include "quadratic_probing.h"
//#include "double_hashing.h"
//#include "linear_probing.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

void SpellChecker(HashTable<string> &hash_table, const string &document_filename) {
    // used to insert missing alphabet
    char alphabet[] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
    // explicit ifstream call
    ifstream document(document_filename);
    string word;
        // while reading document word by word
        while(document >> word) {
            word[0] = tolower(word[0]); //set to lowecase
            string punc = word.substr(word.length()-1,word.length()-1);
            // remove punctuations at the end
            if(punc == "." || punc == "," || punc == "?") {
                word.pop_back();
            }
            // hash contains the word, print correct
            if (hash_table.Contains(word)) {
                std::cout << word << " is CORRECT" << endl;
            } 
            string modifiedWord = word;
            // hash does not contain the word, print incorrect and find alternatives
            if (!hash_table.Contains(word)) {
                std::cout << word << " is INCORRECT" << endl;
                // case a
                for(size_t i = 0; i < word.length()+1; i++) {  // length() + 1, just in case we need to add at the end of word
                    for( size_t j = 0; j < 26; j++) {
                        modifiedWord.insert(i,1,alphabet[j]);   // insert into modifiedWord the alphabets a-z to find match
                        if(hash_table.Contains(modifiedWord)) {
                            cout << "*** " << word << " -> " << modifiedWord << " ***" << " case a" << endl;
                        }
                        modifiedWord = word; // reset modifiedWord for the loop
                    }
                }
                // case b
                size_t i = 0;
                while (i < word.length()) {
                    modifiedWord.erase(i,1);
                    if(hash_table.Contains(modifiedWord)) {
                        cout << "*** " << word << " -> " << modifiedWord << " ***" << " case b" << endl;
                    }
                    modifiedWord = word;    // reset modifiedWord for the loop
                    i++;    // increment loop
                } 
                //case c
                size_t j = 0;
                while(j < word.length()-1) {
                    swap(modifiedWord[j], modifiedWord[j+1]);
                    if(hash_table.Contains(modifiedWord)) {
                        cout << "*** " << word << " -> " << modifiedWord << " ***" << " case c" << endl;     
                    }
                    modifiedWord = word;    // reset modifiedWord for the loop
                    j++; // increment loop
                }
            }
        }
}   // end of spell_check


HashTable<string> MakeDictionary(const string &dictionary_filename) {
       // make dictionary
    ifstream dictionary_file(dictionary_filename);
    string line;
    HashTable<string>hash_table;
    hash_table.MakeEmpty();
    // get dictionary words
    while(getline(dictionary_file,line)) {
        stringstream word(line);    // copy into stream
        string input;
        while(word >> input)    // while reading input insert into hash table
            hash_table.Insert(input);  
    }
    return hash_table;
}           

int testSpellingWrapper(int argument_count, char** argument_list) {
    const string document_filename(argument_list[1]);
    const string dictionary_filename(argument_list[2]);
    
    // Call functions implementing the assignment requirements.
    HashTable<string> dictionary = MakeDictionary(dictionary_filename);
    SpellChecker(dictionary,document_filename); 
    return 0;
}

// Sample main for program spell_check.
// WE WILL NOT USE YOUR MAIN IN TESTING. DO NOT CODE FUNCTIONALITY INTO THE
// MAIN. WE WILL DIRECTLY CALL testSpellingWrapper. ALL FUNCTIONALITY SHOULD BE
// THERE. This main is only here for your own testing purposes.
int main(int argc, char** argv) {
    if (argc != 3) {
        cout << "Usage: " << argv[0] << " <document-file> <dictionary-file>" << endl;
        return 0;
    }

    testSpellingWrapper(argc, argv);

    return 0;
}
