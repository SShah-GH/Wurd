#ifndef STUDENTSPELLCHECK_H_
#define STUDENTSPELLCHECK_H_

#include "SpellCheck.h"

#include <string>
#include <vector>

const int NUM_LETTERS = 27;

class StudentSpellCheck : public SpellCheck {
public:
    StudentSpellCheck();
    virtual ~StudentSpellCheck();
    bool load(std::string dictionaryFile);
    bool spellCheck(std::string word, int maxSuggestions, std::vector<std::string>& suggestions);
    void spellCheckLine(const std::string& line, std::vector<Position>& problems);

private:
    // Nodes for the trie
    struct Node
    {
        bool endOfWord;
        Node* letters[NUM_LETTERS]; // 27th letter is an apostrophe
    };
    // Root for trie
    Node* m_root;

    // Get new initialized node
    Node* getNode()
    {
        Node* newNode = new Node;

        // Initialize node data members
        newNode->endOfWord = false;
        for (int i = 0; i < NUM_LETTERS; i++)
        {
            newNode->letters[i] = nullptr;
        }

        return newNode;
    }

    // To insert a specific string into the trie
    void insert(std::string str);

    // To check if word exists in dictionary
    bool isWord(std::string word);
    
    // Helper function for spellCheck function
    void findSuggestions(std::string word, int max_suggestions, std::vector<std::string>& suggestions);

    // Free all node memory
    void deleteNodes(Node* curNode);
};

#endif  // STUDENTSPELLCHECK_H_
