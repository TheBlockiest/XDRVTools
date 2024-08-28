//this file is part of notepad++
//Copyright (C)2022 Don HO <don.h@free.fr>
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#include "PluginDefinition.h"
#include "menuCmdID.h"
#include <string>
using namespace std;

#include "DockingFeature/GoToLineDlg.h"

#ifdef UNICODE 
#define generic_itoa _itow
#else
#define generic_itoa itoa
#endif

DemoDlg _navDock;
#define DOCKABLE_DEMO_INDEX 9

struct metadataElement {
    string name;
    string value;
};

//
// The plugin data that Notepad++ needs
//
FuncItem funcItem[nbFunc];

//
// The data of Notepad++ that you can use in your plugin commands
//
NppData nppData;

//
// Initialize your plugin data here
// It will be called while plugin loading   
void pluginInit(HANDLE hModule)
{
    _navDock.init((HINSTANCE)hModule, NULL);
}

//
// Here you can do the clean up, save the parameters (if any) for the next session
//
void pluginCleanUp()
{
}

//
// Initialization of your plugin commands
// You should fill your plugins commands here
void commandMenuInit()
{

    //--------------------------------------------//
    //-- STEP 3. CUSTOMIZE YOUR PLUGIN COMMANDS --//
    //--------------------------------------------//
    // with function :
    // setCommand(int index,                      // zero based number to indicate the order of command
    //            TCHAR *commandName,             // the command name that you want to see in plugin menu
    //            PFUNCPLUGINCMD functionPointer, // the symbol of function (function pointer) associated with this command. The body should be defined below. See Step 4.
    //            ShortcutKey *shortcut,          // optional. Define a shortcut to trigger this command
    //            bool check0nInit                // optional. Make this menu item be checked visually
    //            );
	ShortcutKey *emptyNoteKey = new ShortcutKey; //Ctrl alt 0
    emptyNoteKey->_isAlt = true;
    emptyNoteKey->_isCtrl = true;
    emptyNoteKey->_isShift = false;
    emptyNoteKey->_key = 0x30;

    ShortcutKey* doubleSubdivKey = new ShortcutKey; //Ctrl alt 8
    doubleSubdivKey->_isAlt = true;
    doubleSubdivKey->_isCtrl = true;
    doubleSubdivKey->_isShift = false;
    doubleSubdivKey->_key = 0x38;

    ShortcutKey* tripleSubdivKey = new ShortcutKey; //Ctrl alt 9
    tripleSubdivKey->_isAlt = true;
    tripleSubdivKey->_isCtrl = true;
    tripleSubdivKey->_isShift = false;
    tripleSubdivKey->_key = 0x39;

    ShortcutKey* mirrorNoteKey = new ShortcutKey; //Ctrl alt N
    mirrorNoteKey->_isAlt = true;
    mirrorNoteKey->_isCtrl = true;
    mirrorNoteKey->_isShift = false;
    mirrorNoteKey->_key = 0x4E;
    mirrorNoteKey->_key = 0x4E;

    ShortcutKey* mirrorEventKey = new ShortcutKey; //Ctrl alt E
    mirrorEventKey->_isAlt = true;
    mirrorEventKey->_isCtrl = true;
    mirrorEventKey->_isShift = false;
    mirrorEventKey->_key = 0x45;

    ShortcutKey* navKey = new ShortcutKey; //Ctrl alt D
    navKey->_isAlt = true;
    navKey->_isCtrl = true;
    navKey->_isShift = false;
    navKey->_key = 0x44;

    ShortcutKey* docKey = new ShortcutKey; //Ctrl alt /
    docKey->_isAlt = true;
    docKey->_isCtrl = true;
    docKey->_isShift = false;
    docKey->_key = 0xBF;

    setCommand(0, TEXT("Write Empty Note"), writeEmptyNote, emptyNoteKey, false);
    setCommand(1, TEXT("Double Subdivisions"), doubleSubdivisions, doubleSubdivKey, false);
    setCommand(2, TEXT("Triple Subdivisions"), tripleSubdivisions, tripleSubdivKey, false);
    setCommand(3, TEXT("Quadrouple Subdivisions"), quadroupleSubdivisions, NULL, false);

    setCommand(4, TEXT("---"), NULL, NULL, false);

    setCommand(5, TEXT("Mirror Notes"), mirrorNotes, mirrorNoteKey, false);
    setCommand(6, TEXT("Mirror Events"), mirrorEvents, mirrorEventKey, false);
    setCommand(7, TEXT("Mirror Both"), mirrorBoth, NULL, false);

    setCommand(8, TEXT("---"), NULL, NULL, false);

    setCommand(DOCKABLE_DEMO_INDEX, TEXT("Beat / Measure Navigator Dock"), dockableNav, navKey, false);
    setCommand(10, TEXT("Create Measure Markers"), createMeasureMarkers, NULL, false);
    setCommand(11, TEXT("Select Lines"), selectLines, NULL, false);
    setCommand(12, TEXT("Select Beats"), selectBeats, NULL, false);

    setCommand(13, TEXT("---"), NULL, NULL, false);

    setCommand(14, TEXT("Generate Metadata"), generateMetadata, NULL, false);

    setCommand(15, TEXT("---"), NULL, NULL, false);

    setCommand(16, TEXT("Go To Chart-Documentation"), goToDocs, docKey, false);
}

//
// Here you can do the clean up (especially for the shortcut)
//
void commandMenuCleanUp()
{
    delete funcItem[0]._pShKey;
    delete funcItem[1]._pShKey;
    delete funcItem[2]._pShKey;
    delete funcItem[5]._pShKey;
    delete funcItem[6]._pShKey;
    delete funcItem[9]._pShKey;
    delete funcItem[16]._pShKey;
}

//
// This function help you to initialize your plugin commands
//
bool setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk, bool check0nInit) 
{
    if (index >= nbFunc)
        return false;

    if (!pFunc)
        return false;

    lstrcpy(funcItem[index]._itemName, cmdName);
    funcItem[index]._pFunc = pFunc;
    funcItem[index]._init2Check = check0nInit;
    funcItem[index]._pShKey = sk;

    return true;
}

//----------------------------------------------//
//-- STEP 4. DEFINE YOUR ASSOCIATED FUNCTIONS --//
//----------------------------------------------//

HWND getScintilla()
{
    int which;
    // Get the Scintilla
    ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
    return (which == 0) ? nppData._scintillaMainHandle : nppData._scintillaSecondHandle;
}

// Write an empty note where the mouse should go.
void writeEmptyNote()
{
    HWND curScint = getScintilla();
    ::SendMessage(curScint, SCI_REPLACESEL, 0, (LPARAM)"000-000|00|0");
}

// Gets bounds of lines from start and end of selection.
void getBoundsFromSelection(int* startLine, int* endLine)
{
    HWND curScint = getScintilla();

    int selectionStart = (int)::SendMessage(curScint, SCI_GETSELECTIONSTART, 0, 0);
    int selectionEnd = (int)::SendMessage(curScint, SCI_GETSELECTIONEND, 0, 0);
    
    *startLine = (int)::SendMessage(curScint, SCI_LINEFROMPOSITION, selectionStart, 0);
    *endLine = (int)::SendMessage(curScint, SCI_LINEFROMPOSITION, selectionEnd, 0);
}

// Gets bounds of beats from start and end of selection
void getBeatsFromSelection(int* beatStartLine, int* beatEndLine)
{
    HWND curScint = getScintilla();

    int selectionStart = (int)::SendMessage(curScint, SCI_GETSELECTIONSTART, 0, 0);
    int selectionEnd = (int)::SendMessage(curScint, SCI_GETSELECTIONEND, 0, 0);
    int startLine = (int)::SendMessage(curScint, SCI_LINEFROMPOSITION, selectionStart, 0);
    int endLine = (int)::SendMessage(curScint, SCI_LINEFROMPOSITION, selectionEnd, 0);
    int lineCount = (int)::SendMessage(curScint, SCI_GETLINECOUNT, 0, 0);

    for (int i = startLine; i >= 0; i--)
    {
        int lineLength = (int)::SendMessage(curScint, SCI_LINELENGTH, i, 0);
        char* currentLine = new char[lineLength + 1];
        ::SendMessage(curScint, SCI_GETLINE, i, reinterpret_cast<LPARAM>(currentLine));
        
        if (currentLine[0] == '-')
        {
            *beatStartLine = i + 1;
            break;
        }
        else if (i == 0)
        {
            *beatStartLine = 0;
        }

        delete[] currentLine;
    }

    for (int i = endLine; i <= lineCount; i++)
    {
        int lineLength = (int)::SendMessage(curScint, SCI_LINELENGTH, i, 0);
        char* currentLine = new char[lineLength + 1];
        ::SendMessage(curScint, SCI_GETLINE, i, reinterpret_cast<LPARAM>(currentLine));

        if (currentLine[0] == '-')
        {
            *beatEndLine = i - 1;
            break;
        }
        else if (i == lineCount)
        {
            *beatEndLine = lineCount;
        }

        delete[] currentLine;
    }
}

void selectLines()
{
    HWND curScint = getScintilla();

    // Get the start & end of selection, and convert the returned positions to line value
    int startLine, endLine;
    getBoundsFromSelection(&startLine, &endLine);
    int selectStart = (int)::SendMessage(curScint, SCI_POSITIONFROMLINE, startLine, 0);
    int selectEnd = (int)::SendMessage(curScint, SCI_GETLINEENDPOSITION, endLine, 0);

    ::SendMessage(curScint, SCI_SETSEL, selectStart, selectEnd);
}

void selectLine(int lineNumber)
{
    HWND curScint = getScintilla();

    int selectStart = (int)::SendMessage(curScint, SCI_POSITIONFROMLINE, lineNumber, 0);
    int selectEnd = (int)::SendMessage(curScint, SCI_GETLINEENDPOSITION, lineNumber, 0);

    ::SendMessage(curScint, SCI_SETSEL, selectStart, selectEnd);
}

void selectBeats()
{
    HWND curScint = getScintilla();

    // Get the start & end of selection, and convert the returned positions to line value
    int startLine, endLine;
    getBeatsFromSelection(&startLine, &endLine);
    int selectStart = (int)::SendMessage(curScint, SCI_POSITIONFROMLINE, startLine, 0);
    int selectEnd = (int)::SendMessage(curScint, SCI_GETLINEENDPOSITION, endLine, 0);

    ::SendMessage(curScint, SCI_SETSEL, selectStart, selectEnd);
}

void multiplySubdivisions(int mult)
{
    HWND curScint = getScintilla();

    int startLine, endLine;
    getBeatsFromSelection(&startLine, &endLine);

    for (int i = endLine; i >= startLine; i--)
    {
        int lineLength = (int)::SendMessage(curScint, SCI_LINELENGTH, i, 0);
        char* currentLine = new char[lineLength + 1];
        ::SendMessage(curScint, SCI_GETLINE, i, reinterpret_cast<LPARAM>(currentLine));

        if (currentLine[0] == 0)
        {
            continue;
        }

        if (lineLength > 10 && currentLine[3] == '-' && currentLine[7] == '|' && currentLine[10] == '|')
        {
            // Add (mult - 1) empty notes after the corresponding note
            int pos = (int)::SendMessage(curScint, SCI_GETLINEENDPOSITION, i, 0);

            for (int j = 0; j < mult - 1; j++)
            {
                ::SendMessage(curScint, SCI_INSERTTEXT, pos, (LPARAM)"\n\
000-000|00|0");
            }
        }

        delete[] currentLine;
    }
}

// Double the subdivisions in all selected measures.
void doubleSubdivisions()
{
    multiplySubdivisions(2);
}

// Triple the subdivisions in all selected measures.
void tripleSubdivisions()
{
    multiplySubdivisions(3);
}

// Quadrouple subdivisions in all selected measures.
void quadroupleSubdivisions()
{
    multiplySubdivisions(4);
}

string mirrorNoteString(string str)
{
    // Bring into string format, reverse sections, concatenate, create reference
    string noteSubstr = str.substr(0, 7);
    string gearSubstr = str.substr(8, 2);
    reverse(noteSubstr.begin(), noteSubstr.end());
    reverse(gearSubstr.begin(), gearSubstr.end());
    string newLine = noteSubstr + "|" + gearSubstr + "|" + str[11];
    return newLine;
}

string mirrorEventString(string str)
{
    // Bring into string format, reverse sections, concatenate, create reference
    // This code uses the grave accent character (`), so if your events use grave accent characters, random lefts and rights MAY appear
    // However, ` is ugly so this should not be a problem for most everyone
    string newLine = str;
    while (true)
    {
        size_t found = 0;
        found = newLine.find("Left", found + 1);
        if (found != string::npos && found < newLine.length())
        {
            newLine = newLine.substr(0, found) + "`R" + newLine.substr(found + 4, newLine.length());
        }
        else
        {
            break;
        }
    }
    while (true)
    {
        size_t found = 0;
        found = newLine.find("Right", found + 1);
        if (found != string::npos && found < newLine.length())
        {
            newLine = newLine.substr(0, found) + "`L" + newLine.substr(found + 5, newLine.length());
        }
        else
        {
            break;
        }
    }
    while (true)
    {
        size_t found = 0;
        found = newLine.find("`", found + 1);
        if (found != string::npos && found < newLine.length())
        {
            if (newLine[found + 1] == 'L')
            {
                newLine = newLine.substr(0, found) + "Left" + newLine.substr(found + 2, newLine.length());
            }
            else if(newLine[found + 1] == 'R')
            {
                newLine = newLine.substr(0, found) + "Right" + newLine.substr(found + 2, newLine.length());
            }
        }
        else
        {
            break;
        }
    }

    return newLine;
}

void mirrorElements(string mirrorType)
{
    HWND curScint = getScintilla();

    int startLine, endLine;
    getBoundsFromSelection(&startLine, &endLine);

    for (int i = startLine; i <= endLine; i++)
    {
        selectLine(i);

        int lineLength = (int)::SendMessage(curScint, SCI_LINELENGTH, i, 0);
        char* currentLine = new char[lineLength + 1];
        ::SendMessage(curScint, SCI_GETSELTEXT, 0, reinterpret_cast<LPARAM>(currentLine));

        if (currentLine[0] == 0)
        {
            continue;
        }

        if (mirrorType == "notes")
        {
            // Check for three characters that indicate a line to be a note
            if (lineLength > 10 && currentLine[3] == '-' && currentLine[7] == '|' && currentLine[10] == '|')
            {
                // Bring into string format, reverse sections, concatenate, create reference
                string newLine = mirrorNoteString(currentLine);
                const char* str = newLine.c_str();
                ::SendMessage(curScint, SCI_REPLACESEL, 0, (LPARAM)str);
            }
        }
        else if (mirrorType == "events")
        {
            // Check for hash that indicates a line to be an event
            if (currentLine[0] == '#')
            {
                string newLine = mirrorEventString(currentLine);
                const char* str = newLine.c_str();
                ::SendMessage(curScint, SCI_REPLACESEL, 0, (LPARAM)str);
            }
        }
        else
        {
            if (lineLength > 10 && currentLine[3] == '-' && currentLine[7] == '|' && currentLine[10] == '|')
            {
                string newLine = mirrorNoteString(currentLine);
                const char* str = newLine.c_str();
                ::SendMessage(curScint, SCI_REPLACESEL, 0, (LPARAM)str);
            }
            else if (currentLine[0] == '#')
            {
                string newLine = mirrorEventString(currentLine);
                const char* str = newLine.c_str();
                ::SendMessage(curScint, SCI_REPLACESEL, 0, (LPARAM)str);
            }
        }

        delete[] currentLine;
    }
}

// Mirror all selected notes
void mirrorNotes()
{
    mirrorElements("notes");
}

// Multiply all selected events
void mirrorEvents()
{
    mirrorElements("events");
}

void mirrorBoth()
{
    mirrorElements("both");
}

void dockableNav()
{
    _navDock.setParent(nppData._nppHandle);
    tTbData	data = { 0 };

    if (!_navDock.isCreated())
    {
        _navDock.create(&data);

        // define the default docking behaviour
        data.uMask = DWS_DF_CONT_RIGHT;

        data.pszModuleName = _navDock.getPluginFileName();

        // the dlgDlg should be the index of funcItem where the current function pointer is
        // in this case is DOCKABLE_DEMO_INDEX
        data.dlgID = DOCKABLE_DEMO_INDEX;
        ::SendMessage(nppData._nppHandle, NPPM_DMMREGASDCKDLG, 0, (LPARAM)&data);
    }
    _navDock.display();
}

void deleteMeasureMarkers()
{
    HWND curScint = getScintilla();

    int lineCount = (int)::SendMessage(curScint, SCI_GETLINECOUNT, 0, 0);

    for (int i = 1; i <= lineCount; i++)
    {
        int lineLength = (int)::SendMessage(curScint, SCI_LINELENGTH, i, 0);
        char* currentLine = new char[lineLength + 1];
        ::SendMessage(curScint, SCI_GETLINE, i, reinterpret_cast<LPARAM>(currentLine));

        if (currentLine[0] == 0)
        {
            continue;
        }

        if (lineLength > 3 && currentLine[0] == '/' && currentLine[2] == '`' && currentLine[3] == 'B')
        {
            // Current line is a pre-existing beat marker, please delete
            int nextLine = i + 1;
            int start = (int)::SendMessage(curScint, SCI_POSITIONFROMLINE, i, 0);
            int end = (int)::SendMessage(curScint, SCI_POSITIONFROMLINE, nextLine, 0);
            int length = end - start;
            ::SendMessage(curScint, SCI_DELETERANGE, start, length);

            // Line count has changed, recalculate it!
            lineCount -= 1;
        }

        delete[] currentLine;
    }

}

void createMeasureMarkers()
{
    deleteMeasureMarkers();

    HWND curScint = getScintilla();

    int lineCount = (int)::SendMessage(curScint, SCI_GETLINECOUNT, 0, 0);

    int currentBeat = -1;
    int currentMeasure = 0;
    int beatOfNextMeasure = 0;
    int beatsPerMeasure = 4;
    int measureIncrement = 1;
    bool noteReadSinceMeasure = true;
    for (int i = 1; i <= lineCount; i++)
    {
        int lineLength = (int)::SendMessage(curScint, SCI_LINELENGTH, i, 0);
        char* currentLine = new char[lineLength + 1];
        ::SendMessage(curScint, SCI_GETLINE, i, reinterpret_cast<LPARAM>(currentLine));

        if (currentLine[0] == 0)
        {
            continue;
        }

        if (currentLine[0] == '-')
        {
            currentBeat += 1;

            if (currentBeat == beatOfNextMeasure)
            {
                currentMeasure += measureIncrement;
                beatOfNextMeasure = currentBeat + beatsPerMeasure;
                noteReadSinceMeasure = false;

                string str = "\n\
//`B" + to_string(currentBeat) + ",M" + to_string(currentMeasure);
                const char* newMarker = str.c_str();
                int pos = (int)::SendMessage(curScint, SCI_GETLINEENDPOSITION, i, 0);
                ::SendMessage(curScint, SCI_INSERTTEXT, pos, (LPARAM)newMarker);

                lineCount += 1;
                i += 1;
            }
        }
        else if (lineLength > 10 && currentLine[3] == '-' && currentLine[7] == '|' && currentLine[10] == '|')
        {
            noteReadSinceMeasure = true;
        }
        else if (lineLength > 6 && currentLine[0] == '#' && currentLine[1] == 'T' && currentLine[6] == 'S')
        {
            string str = currentLine;
            size_t equalPos = str.find("=");
            size_t commaPos = str.find(",");
            size_t endPos = str.find("\n");
            if (commaPos == string::npos || equalPos == string::npos || endPos == string::npos)
            {
                continue;
            }

            try
            {
                beatsPerMeasure = stoi(str.substr(equalPos + 1, commaPos - equalPos - 1));
                measureIncrement = stoi(str.substr(commaPos + 1, endPos - commaPos - 1)) / 4;
            }
            catch (...)
            {
                ::SendMessage(curScint, SCI_ENSUREVISIBLE, i, 0);
                ::SendMessage(curScint, SCI_GOTOLINE, i, 0);
                ::MessageBox(nppData._nppHandle, TEXT("Malformed time signature event."), TEXT("Error"), MB_OK);
                break;
            }

            if (!noteReadSinceMeasure)
            {
                beatOfNextMeasure = currentBeat + beatsPerMeasure;
            }
        }

        delete[] currentLine;
    }
}

void metadata(bool smart)
{
    HWND curScint = getScintilla();

    metadataElement metadataValues[] = {
        {"MUSIC_TITLE","Song Title"},
        {"ALTERNATE_TITLE",""},
        {"SUBTITLE",""},
        {"MUSIC_ARTIST","Song Artist"},
        {"MUSIC_CREDIT",""},
        {"MUSIC_AUDIO","audio.ogg"},
        {"MUSIC_PREVIEW_START","0"},
        {"MUSIC_PREVIEW_LENGTH","10"},
        {"MUSIC_VOLUME","1"},
        {"MUSIC_OFFSET","0"},
        {"JACKET_IMAGE","jacket.jpg"},
        {"JACKET_ILLUSTRATOR","Jacket Illustrator"},
        {"CHART_AUTHOR","Me"},
        {"CHART_TAGS","0,0,0,0"},
        {"CHART_BOSS","FALSE"},
        {"CHART_DIFFICULTY","EXTREME"},
        {"CHART_LEVEL","15"},
        {"CHART_UNLOCK",""},
        {"CHART_DISPLAY_BPM","120"},
        {"CHART_BPM","120"},
        {"FLASH_TRACK","FALSE"},
        {"KEYBOARD_ONLY","FALSE"},
        {"ORIGINAL","FALSE"},
        {"MODFILE_PATH",""},
        {"RPC_HIDDEN","FALSE"},
        {"DISABLE_LEADERBOARD_UPLOADING","FALSE"},
        {"STAGE_BACKGROUND","default"}
    };
    int len = sizeof metadataValues / sizeof metadataValues[0] - 1;

    if (smart)
    {
        ::MessageBox(nppData._nppHandle, TEXT("Nothing here yet :("), TEXT("Sorry"), MB_OK);
        // We will put some stuff in here, handling metadata gen first
    }

    // Find the first line, then delete all previous metadata lines
    int lineCount = (int)::SendMessage(curScint, SCI_GETLINECOUNT, 0, 0);
    int length = 0;
    for (int i = 0; i <= lineCount; i++)
    {
        int lineLength = (int)::SendMessage(curScint, SCI_LINELENGTH, i, 0);
        char* currentLine = new char[lineLength + 1];
        ::SendMessage(curScint, SCI_GETLINE, i, reinterpret_cast<LPARAM>(currentLine));

        if (currentLine[0] == '-')
        {
            length = (int)::SendMessage(curScint, SCI_POSITIONFROMLINE, i, 0);
            delete[] currentLine;
            break;
        }
        else
        {
            string lineAsStr = currentLine;
            size_t equalPos = lineAsStr.find("=");
            size_t endLinePos = lineAsStr.find("\n");

            if (equalPos != string::npos && endLinePos != string::npos)
            {
                // If equal sign is found, save the value of the metadata to the table
                // Prevents unnecessary deletion / replacement
                string tag = lineAsStr.substr(0, equalPos);
                for (int j = len; j >= 0; j--)
                {
                    if (metadataValues[j].name == tag)
                    {
                        metadataValues[j].value = lineAsStr.substr(equalPos + 1, endLinePos - equalPos - 1);
                    }
                }
            }
        }

        delete[] currentLine;
    }

    if (length > 0)
    {
        ::SendMessage(curScint, SCI_DELETERANGE, 0, length);
    }
    // Generate (sequentially) the metadata lines at the beginning of the code

    for (int i = len; i >= 0; i--)
    {
        string str = metadataValues[i].name + "=" + metadataValues[i].value + "\n\
";
        const char* newMetadata = str.c_str();
        ::SendMessage(curScint, SCI_INSERTTEXT, 0, (LPARAM)newMetadata);
    }
}

void generateMetadata()
{
    metadata(false);
}

void goToDocs()
{
    ::ShellExecute(NULL, TEXT("open"), TEXT("https://github.com/EX-XDRiVER/Chart-Documentation"), NULL, NULL, SW_SHOWNORMAL);
}