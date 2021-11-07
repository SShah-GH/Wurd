#include "StudentUndo.h"
#include <string>

using namespace std;

Undo* createUndo()
{
    return new StudentUndo;
}

void StudentUndo::submit(const Action action, int row, int col, char ch)
{
    string letters = "";
    letters += ch;
    
    // Check if stack is empty
    if (!m_changes.empty())
    {
        // Get previous change for future use
        Change prev = m_changes.top();
        
        // Check if actions and rows match
        if (prev.act == action && prev.r == row)
        {
            // Deleted character
            if (action == DELETE)
            {
                // Batching backspaces
                if (prev.c - col == 1)
                {
                    // Remove previous entry
                    m_changes.pop();
                    
                    // Merge previous and new entry
                    letters += prev.letters;
                }
                // Batching dels
                else if (prev.c - col == 0)
                {
                    // Remove previous entry
                    m_changes.pop();
                    
                    // Merge previous and new entry
                    letters = prev.letters + letters;
                }
            }
            // Inserted character
            else if (action == INSERT)
            {
                // Batching inserts
                if (prev.c - col == -1)
                {
                    // Remove previous entry
                    m_changes.pop();
                    
                    // Merge previous and new entry
                    letters += prev.letters;
                }
            }
        }
    }
        
    // Push new action onto stack
    m_changes.push(Change(action, row, col, letters));
}

StudentUndo::Action StudentUndo::get(int& row, int& col, int& count, std::string& text)
{
    // Check if stack is empty
    if (m_changes.empty())
    {
        return Action::ERROR;
    }
    
    // Get the most recent operation
    Change operation = m_changes.top();
    m_changes.pop();
    
    // Set row and col locations
    row = operation.r;
    col = operation.c;
    
    // Undoing a insertion
    if (operation.act == INSERT)
    {
        count = operation.letters.length();
        text = "";
        return DELETE;
    }
    // Undoing a deletion
    else if (operation.act == DELETE)
    {
        count = 1;
        text = operation.letters;
        return INSERT;
    }
    // Undoing a join
    else if (operation.act == JOIN)
    {
        count = 1;
        text = "";
        return SPLIT;
    }
    // Undoing a split
    else
    {
        count = 1;
        text = "";
        return JOIN;
    }
}

void StudentUndo::clear()
{
    // Empty the stack
    while (!m_changes.empty())
    {
        m_changes.pop();
    }
}
