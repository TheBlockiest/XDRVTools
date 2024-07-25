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

extern NppData nppData;

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
						int which = -1;
						::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
						if (which == -1)
							return FALSE;
						HWND curScint = (which == 0)?nppData._scintillaMainHandle:nppData._scintillaSecondHandle;

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
						int which = -1;
						::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
						if (which == -1)
							return FALSE;
						HWND curScint = (which == 0) ? nppData._scintillaMainHandle : nppData._scintillaSecondHandle;

						goToLineOfBeat(curScint, measure * 4);
					}
					return TRUE;
				}
			}
				return FALSE;
		}

		default :
			return DockingDlgInterface::run_dlgProc(message, wParam, lParam);
	}
}

