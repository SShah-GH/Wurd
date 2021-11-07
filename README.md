## Project Description

Wurd is a C++ text editor. Users can modify files, undo changes, and spell check. Features are as follows:

| Keybinding | Functionality                         |
| ---------- | ------------------------------------- |
| Arrow keys | Move cursor                           |
| Backspace  | Delete character prior to cursor      |
| Delete     | Delete character after cursor         |
| Home       | Move cursor to start of line          |
| End        | Move cursor to end of line            |
| PgUp       | Move up one page                      |
| PgDown     | Move down one page                    |
| Ctrl-Z     | Undo last change                      |
| Ctrl-S     | Save file                             |
| Ctrl-L     | Load new file                         |
| Ctrl-D     | Load new dictionary (for spell check) |
| Ctrl-X     | Exit editor                           |

## Usage

To build Wurd

1. Change into the Wurd directory and type `make`
2. To run the program, type `./wurd`

## Project Files

The Wurd Test Editor was made through UCLA's CS32 course. A skeleton repo including the GUI and basic structure was provided through the class, in addition to a 20+ page spec detailing requirements.

From there, all files named `Student*` were written by me. Specifically, StudentTestEditor creates, holds, and modifies the file's text using a C++ list. StudentUndo enables the undo (Ctrl-Z) functionality using a stack. StudentSpellCheck performs the spell checking with the loaded dictionary using a trie. Note, dictionary.txt is loaded by default.

A compiled executable version of Wurd for MacOS named `wurd` has also been included in this repo. Feel free to try it out!
