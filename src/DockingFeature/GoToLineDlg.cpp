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

//If this line is here, then the Github repo move worked

#include "GoToLineDlg.h"
#include "../PluginDefinition.h"

//Hi
extern NppData nppData;

void goToLineOfMeasure(HWND curScint, int measureGoal)
{
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

		if (i == lineCount)
		{
			::SendMessage(curScint, SCI_ENSUREVISIBLE, lineCount, 0);
			::SendMessage(curScint, SCI_GOTOLINE, lineCount, 0);
		}
		else if (currentLine[0] == '-')
		{
			currentBeat += 1;
			if (currentBeat == beatOfNextMeasure)
			{
				currentMeasure += measureIncrement;
				beatOfNextMeasure = currentBeat + beatsPerMeasure;
				noteReadSinceMeasure = false;

				if (currentMeasure >= measureGoal)
				{
					int lineToGo = i + 1;
					::SendMessage(curScint, SCI_ENSUREVISIBLE, lineToGo, 0);
					::SendMessage(curScint, SCI_GOTOLINE, lineToGo, 0);
					delete[] currentLine;
					break;
				}
			}
		}
		else if (lineLength > 11 && currentLine[3] == '-' && currentLine[7] == '|' && currentLine[10] == '|')
		{
			noteReadSinceMeasure = true;
		}
		else if (lineLength > 7 && currentLine[0] == '#' && currentLine[1] == 'T' && currentLine[6] == 'S')
		{
			string str = currentLine;
			size_t equalPos = str.find("=");
			size_t commaPos = str.find(",");
			size_t endPos = str.find("\n");
			if (commaPos == string::npos || equalPos == string::npos || endPos == string::npos)
			{
				::MessageBox(nppData._nppHandle, TEXT("missing pos."), TEXT("debug"), MB_OK);
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

void goToLineOfBeat(HWND curScint, int beatGoal)
{
	int lineCount = (int)::SendMessage(curScint, SCI_GETLINECOUNT, 0, 0);
	int currentBeat = -1;
	for (int i = 1; i <= lineCount; i++)
	{
		int lineLength = (int)::SendMessage(curScint, SCI_LINELENGTH, i, 0);
		char* currentLine = new char[lineLength + 1];
		::SendMessage(curScint, SCI_GETLINE, i, reinterpret_cast<LPARAM>(currentLine));

		if (i == lineCount)
		{
			::SendMessage(curScint, SCI_ENSUREVISIBLE, lineCount, 0);
			::SendMessage(curScint, SCI_GOTOLINE, lineCount, 0);
		}
		else if (currentLine[0] == '-')
		{
			currentBeat += 1;
			if (currentBeat == beatGoal)
			{
				int lineToGo = i + 1;
				::SendMessage(curScint, SCI_ENSUREVISIBLE, lineToGo, 0);
				::SendMessage(curScint, SCI_GOTOLINE, lineToGo, 0);
				delete[] currentLine;
				break;
			}
		}

		delete[] currentLine;
	}
}

INT_PTR CALLBACK DemoDlg::run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
		case WM_COMMAND : 
		{
			switch (wParam)
			{
				case IDOK :
				{
					int beat = getBeat();
					if (beat != -1)
					{
						// Get the current scintilla
						HWND curScint = getScintilla();

						goToLineOfBeat(curScint, beat);
					}
					return TRUE;
				}
				case IDOK2:
				{
					int measure = getMeasure();
					if (measure != -1)
					{
						// Get the current scintilla
						HWND curScint = getScintilla();

						goToLineOfMeasure(curScint, measure);
					}
					return TRUE;
				}
				case IDOK3:
				{
					createMeasureMarkers();
					return TRUE;
				}
				case IDOK4:
				{
					deleteMeasureMarkers();
					return TRUE;
				}
			}
				return FALSE;
		}

		default :
			return DockingDlgInterface::run_dlgProc(message, wParam, lParam);
	}
}

