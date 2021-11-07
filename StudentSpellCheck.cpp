#include "StudentSpellCheck.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cctype>

using namespace std;

SpellCheck* createSpellCheck()
{
    return new StudentSpellCheck;
}

StudentSpellCheck::StudentSpellCheck()
{
    // Initialize root
    m_root = getNode();
}

StudentSpellCheck::~StudentSpellCheck()
{
    // Free dynamically allocated memory
    deleteNodes(m_root);
}

bool StudentSpellCheck::load(std::string dictionaryFile)
{
    // Check if dictionary can be opened
    ifstream infile(dictionaryFile);
    if (!infile)
    {
        // cerr << "Error: " << file << " cannot be opened";
        return false;
    }
    
    // Clear old dictionary
    deleteNodes(m_root);
    
    // Reinitialize root
    m_root = getNode();
    
    // Insert dictionary into trie
    string word;
    while (getline(infile, word))
    {
        insert(word);
    }
    
    return true;
}

bool StudentSpellCheck::spellCheck(std::string word, int max_suggestions, std::vector<std::string>& suggestions)
{
    // Check if word is in dictionary
    if (isWord(word))
    {
        return true;
    }
    // Word does not exist
    else
    {
        // Find suggestions for word
        findSuggestions(word, max_suggestions, suggestions);
        return false;
    }
}

void StudentSpellCheck::spellCheckLine(const std::string& line, std::vector<SpellCheck::Position>& problems)
{
    // Clear out old problems data
    problems.clear();
    
    string word = "";
    int start = 0;
    
    // Iterate through line
    for (int i = 0; i < line.length(); i++)
    {
        // Current character is a letter
        if (isalpha(line[i]))
        {
            word += tolower(line[i]);
        }
        // Current character is an apostrophe
        else if (line[i] == '\'')
        {
            word += "'";
        }
        // Current character is a space-like seperator
        else
        {
            // Check if word is formed
            if (word != "")
            {
                // Check if word is misspelled
                if (!isWord(word))
                {
                    // Add corresponding position
                    SpellCheck::Position newPos;
                    newPos.start = start;
                    newPos.end = i-1;
                    problems.push_back(newPos);
                }
                
                // Reset for new word
                word = "";
            }
            start = i+1;
        }
    }
    
    // Add final word if needed
    if (!isWord(word))
    {
        // Add corresponding position
        SpellCheck::Position newPos;
        newPos.start = start;
        newPos.end = line.length()-1;
        problems.push_back(newPos);
    }
}

// Private Member Functions
void StudentSpellCheck::insert(string str)
{
    // Start at the root
    Node* curNode = m_root;

    // Iterate through string's letters
    for (int i = 0; i < str.length(); i++)
    {
        int charIndex;
        // Check if current character is an apostrophe
        if (str[i] == '\'')
        {
            charIndex = 26;
        }
        // Current character is a letter
        else if (isalpha(str[i]))
        {
            // Find corresonding array index
            charIndex = tolower(str[i]) - 'a';
        }
        else
        {
            continue;
        }

        // Add new if needed
        if (curNode->letters[charIndex] == nullptr)
        {
            curNode->letters[charIndex] = getNode();
        }

        // Move to next letter
        curNode = curNode->letters[charIndex];
    }

    // Mark final node as end of a word
    if (curNode != m_root)
    {
        curNode->endOfWord = true;
    }
}

bool StudentSpellCheck::isWord(string word)
{
    // Start at the root
    Node* curNode = m_root;

    for (int i = 0; i < word.length(); i++)
    {
        int charIndex;
        // Check if current character is an apostrophe
        if (word[i] == '\'')
        {
            charIndex = 26;
        }
        // Current character is a letter
        else
        {
            // Find corresonding array index
            charIndex = tolower(word[i]) - 'a';
        }
        
        // Check if word's current letter is in dictionary
        if (curNode->letters[charIndex] == nullptr)
        {
            return false;
        }
        // Move to next letter
        else
        {
            curNode = curNode->letters[charIndex];
        }
    }
    
    // Return if word ends
    return curNode->endOfWord;
}

void StudentSpellCheck::findSuggestions(string word, int max_suggestions, vector<string>& suggestions)
{
    // Clear suggestions
    suggestions.clear();
 
    // Add nothing if no suggestions needed
    if (max_suggestions == 0)
    {
        return;
    }
    
    // Iterate through word
    for (int i = 0; i < word.length(); i++)
    {
        // Form all possible letter combinations
        for (int j = 0; j < 27; j++)
        {
            string possSuggestion;
            
            // Use apostrophe
            if (j == 26)
            {
                possSuggestion = word.substr(0, i) + "'"+ word.substr(i+1);
            }
            // Use letter
            else
            {
                possSuggestion = word.substr(0, i) + (char)(j+'a') + word.substr(i+1);
            }
            
            // Check if formed word is in dictionary
            if (isWord(possSuggestion))
            {
                suggestions.push_back(possSuggestion);
                max_suggestions--;
                
                // Check if no more suggestions needed
                if (max_suggestions == 0)
                {
                    return;
                }
            }
        }
    }
}

void StudentSpellCheck::deleteNodes(Node* curNode)
{
    // Iterate over children nodes
    for (int i = 0; i < NUM_LETTERS; i++)
    {
        // Delete children if needed
        if (curNode->letters[i] != nullptr)
        {
            deleteNodes(curNode->letters[i]);
        }
    }
    
    // Delete current node
    delete curNode;
}
