#ifndef STUDENTUNDO_H_
#define STUDENTUNDO_H_

#include "Undo.h"
#include <stack>

class StudentUndo : public Undo {
public:

    void submit(Action action, int row, int col, char ch = 0);
    Action get(int& row, int& col, int& count, std::string& text);
    void clear();

private:
    // To store changes
    struct Change
    {
        // Constructor
        Change(Action action, int row, int col, std::string str)
        {
            act = action;
            r = row;
            c = col;
            letters = str;
        }
        
        Action act;
        int r;
        int c;
        std::string letters;
    };
    
    // To record undo changes
    std::stack<Change> m_changes;
};

#endif // STUDENTUNDO_H_
