#ifndef STUDENTTEXTEDITOR_H_
#define STUDENTTEXTEDITOR_H_

#include "TextEditor.h"
#include <list>

class Undo;

class StudentTextEditor : public TextEditor {
public:

    StudentTextEditor(Undo* undo);
    ~StudentTextEditor();
    bool load(std::string file);
    bool save(std::string file);
    void reset();
    void move(Dir dir);
    void del();
    void backspace();
    void insert(char ch);
    void enter();
    void getPos(int& row, int& col) const;
    int getLines(int startRow, int numRows, std::vector<std::string>& lines) const;
    void undo();

private:
    std::list<std::string> m_lines;
    std::list<std::string>::iterator m_curLine;
    int m_row;
    int m_col;
    
    // Helper functions to prevent undoing an undo
    void del(bool undo);
    void backspace(bool undo);
    void insert(char ch, bool undo);
    void enter(bool undo);
};

#endif // STUDENTTEXTEDITOR_H_
