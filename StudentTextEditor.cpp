#include "StudentTextEditor.h"
#include "Undo.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

TextEditor* createTextEditor(Undo* un)
{
    return new StudentTextEditor(un);
}

StudentTextEditor::StudentTextEditor(Undo* undo)
 : TextEditor(undo)
{
    // Add first line if needed
    if (m_lines.empty())
    {
        m_lines.push_back("");
    }
    
    // Start at beginning of document
    m_row = 0;
    m_col = 0;
    m_curLine = m_lines.begin();
}

StudentTextEditor::~StudentTextEditor()
{}

bool StudentTextEditor::load(std::string file)
{
    // Check if file can be opened
    ifstream infile(file);
    if (!infile)
    {
        // cerr << "Error: " << file << " cannot be opened";
        return false;
    }
    
    // Reset document
    reset();
    
    
    // Load all lines into member list
    string curLine;
    while (getline(infile, curLine))
    {
        // Remove carriage return character if needed
        if (curLine.length() > 0 && curLine[curLine.length()-1] == '\r')
        {
            curLine = curLine.substr(0, curLine.length()-1);
        }
        m_lines.push_back(curLine);
    }
    
    // Reset start line back to first line
    m_curLine = m_lines.begin();
    
    return true;
}

bool StudentTextEditor::save(std::string file)
{
    // Start write to file
    ofstream outfile(file);
    if (!outfile)
    {
        // cerr << "Error: Cannot write to " << file
        return false;
    }
    
    // Write all lines to outfile
    for (list<string>::iterator iter = m_lines.begin(); iter != m_lines.end(); iter++)
    {
        outfile << *iter << endl;
    }

    return true;
}

void StudentTextEditor::reset()
{
    // Clear list
    m_lines.clear();
    
    // Move editing position
    m_row = 0;
    m_col = 0;
    m_curLine = m_lines.begin();

    // Clear undo history
    getUndo()->clear();
}

void StudentTextEditor::move(Dir dir)
{
    switch (dir) {
        case Dir::UP:
            // Move cursor up 1 line if possible
            if (m_row != 0)
            {
                m_row--;
                
                // Find previous line
                std::list<std::string>::iterator prevLine = m_curLine;
                prevLine--;
                
                // Move m_col if needed
                if ((*prevLine).length() < m_col)
                {
                    m_col = (*prevLine).length();
                }
                
                // Change current line
                m_curLine--;
            }
            break;
        case Dir::DOWN:
            // Move cursor down 1 line if possible
            if (m_row != m_lines.size()-1)
            {
                m_row++;
                
                // Find next line
                std::list<std::string>::iterator nextLine = m_curLine;
                nextLine++;
                
                // Move m_col if needed
                if ((*nextLine).length() < m_col)
                {
                    m_col = (*nextLine).length();
                }
                
                // Change current line
                m_curLine++;
            }
            break;
        case Dir::LEFT:
            // Check if at the beginning of a line
            if (m_col == 0)
            {
                // Only move if not at the top line
                if (m_row != 0)
                {
                    m_row--;
                    m_curLine--;
                    m_col = (*m_curLine).length();
                }
            }
            // Not at beginning of the line
            else
            {
                m_col--;
            }
            break;
        case Dir::RIGHT:
            // Check if at the end of a line
            if (m_col == (*m_curLine).length())
            {
                // Only move if not at the bottom line
                if (m_row != m_lines.size()-1)
                {
                    m_row++;
                    m_curLine++;
                    m_col = 0;
                }
            }
            // Not at end of the line
            else
            {
                m_col++;
            }
            break;
        case Dir::HOME:
            // Move to first character on current line
            m_col = 0;
            break;
        case Dir::END:
            // Move editing position to end of line
            m_col = (*m_curLine).length();
            break;
        default:
            break;
    }
}

void StudentTextEditor::del()
{
    del(true);
}

void StudentTextEditor::backspace()
{
    backspace(true);
}

void StudentTextEditor::insert(char ch)
{
    insert(ch, true);
}

void StudentTextEditor::enter()
{
    enter(true);
}

void StudentTextEditor::getPos(int& row, int& col) const
{
    row = m_row;
    col = m_col;
}

int StudentTextEditor::getLines(int startRow, int numRows, std::vector<std::string>& lines) const
{
    // Check if input rows are valid
    if (startRow < 0 || numRows < 0 || startRow > m_lines.size())
    {
        return -1;
    }
 
    // Clear all lines in vector
    lines.clear();
    
    // Move an iterator to startRow
    list<string>::const_iterator curLine = m_curLine;
    int curRow = m_row;
    while (curRow != startRow)
    {
        if (curRow > startRow)
        {
            curRow--;
            curLine--;
        }
        else if (curRow < startRow)
        {
            curRow++;
            curLine++;
        }
    }
    
    // Copy all rows into lines vector
    int numAdded = 0;
    for (int i = 0; i < numRows && curLine != m_lines.end(); i++)
    {
        lines.push_back(*curLine);
        curLine++;
        numAdded++;
    }
    
    // Return number of lines added in vector
    return numAdded;
}

void StudentTextEditor::undo()
{
    // Get last undo operation
    int row;
    int col;
    int count;
    string text;
    Undo::Action action = getUndo()->get(row, col, count, text);
    
    if (action == Undo::Action::ERROR)
    {
        return;
    }
    
    // Move to specified row and col
    while (m_row != row)
    {
        if (m_row > row)
        {
            m_row--;
            m_curLine--;
        }
        else if (m_row < row)
        {
            m_row++;
            m_curLine++;
        }
    }
    m_col = col;
    
    // Need to insert characters
    if (action == Undo::Action::INSERT)
    {
        for (int i = 0; i < text.length(); i++)
        {
            insert(text[i], false);
        }
        
        // Reset location back
        m_col = col;
    }
    // Need to delete characters
    else if (action == Undo::Action::DELETE)
    {
        for (int i = 0; i < count; i++)
        {
            backspace(false);
        }
    }
    // Need to split line
    else if (action == Undo::Action::SPLIT)
    {
        enter(false);
        move(LEFT);
    }
    // Need to join lines
    else
    {
        del(false);
    }
}

void StudentTextEditor::del(bool undo)
{
    // Check if at the end of a line
    if (m_col == (*m_curLine).length())
    {
        // Find next line
        std::list<std::string>::iterator nextLine = m_curLine;
        nextLine++;
        
        // Update current line and delete next line
        if (nextLine != m_lines.end())
        {
            *m_curLine = *m_curLine + *nextLine;
            m_lines.erase(nextLine);
            
            if (undo)
            {
                // Submit join to undo
                getUndo()->submit(Undo::Action::JOIN, m_row, m_col, '\0');
            }
        }
    }
    // In the middle of a line
    else
    {
        char charDeleted = (*m_curLine)[m_col];
        *m_curLine = (*m_curLine).substr(0, m_col) + (*m_curLine).substr(m_col+1);
        
        if (undo)
        {
            // Submit deletion to undo
            getUndo()->submit(Undo::Action::DELETE, m_row, m_col, charDeleted);
        }
    }
}

void StudentTextEditor::backspace(bool undo)
{
    // Check if at the beginning of a line
    if (m_col == 0)
    {
        // Find previous line
        std::list<std::string>::iterator prevLine = m_curLine;
        
        // Update previous line and delete current line
        if (prevLine != m_lines.begin())
        {
            prevLine--;

            // Store for future updating
            int newCol = (*prevLine).length();
            
            *prevLine = *prevLine + *m_curLine;
            m_curLine = m_lines.erase(m_curLine);
            
            // Update current location
            m_curLine--;
            m_row--;
            m_col = newCol;
            
            if (undo)
            {
                // Submit join to undo
                getUndo()->submit(Undo::Action::JOIN, m_row, m_col, '\0');
            }
        }
    }
    // In the middle of a line
    else
    {
        char charDeleted = (*m_curLine)[m_col-1];
        
        *m_curLine = (*m_curLine).substr(0, m_col-1) + (*m_curLine).substr(m_col);
        m_col--;
        
        if (undo)
        {
            // Submit deletion to undo
            getUndo()->submit(Undo::Action::DELETE, m_row, m_col, charDeleted);
        }
    }
}

void StudentTextEditor::insert(char ch, bool undo)
{
    // Add spaces if character is tab
    if (ch == '\t')
    {
        *m_curLine = (*m_curLine).substr(0, m_col) + "    " + (*m_curLine).substr(m_col);
        
        if (undo)
        {
            // Process as spaces to enable batching
            for (int i = 0; i < 4; i++)
            {
                m_col++;
                
                // Submit insertion to undo
                getUndo()->submit(Undo::Action::INSERT, m_row, m_col, ' ');
            }
        }
        else
        {
            m_col += 4;
        }
    }
    // Add singular character
    else
    {
        *m_curLine = (*m_curLine).substr(0, m_col) + ch + (*m_curLine).substr(m_col);
        m_col++;
    
        if (undo)
        {
            // Submit insertion to undo
            getUndo()->submit(Undo::Action::INSERT, m_row, m_col, ch);
        }
    }
}

void StudentTextEditor::enter(bool undo)
{
    if (undo)
    {
        // Submit split to undo
        getUndo()->submit(Undo::Action::SPLIT, m_row, m_col, '\0');
    }
    
    // Get text for next line
    string nextLine = (*m_curLine).substr(m_col);
    
    // Set new value for current line
    *m_curLine = (*m_curLine).substr(0, m_col);
    
    // Create and move to next line
    m_curLine++;
    m_curLine = m_lines.insert(m_curLine, nextLine);
    m_row++;
    m_col = 0;
}
