# XDRVTools for Notepad++
XDRVTools is a plugin for Notepad++ that makes directly editing .xdrv files as text easier and more efficient.
Current build is guaranteed to work on Windows systems, not yet tested for Linux or Mac.


## Installation:
1. Locate the Notepad++ plugins folder. This can be done by selecting "Plugins" → "Open Plugins Folder..." in Notepad++ or by going to your program files and finding the Notepad++ folder.
2. Paste the file `XDRVTools.dll` to Notepad++ `plugins\XDRVTools` folder.
3. Restart Notepad++. The plugin should appear under the "Plugins" menu.
4. Open an XDRV file and start using the suite of commands.


## Planned Changes:
* Rework functions to perform text changes in one action (in order to work better with Undo/Redo commands.)
* Make **Beat / Measure Navigator Dock**'s "Go to measure" function account for time signature changes, rather than assuming 4/4.


## Important Info:
* For ease of use, XDRVTools will expand your selections to cover all of the contained lines or measures as relevant.
* XDRVTools is only meant to be used with .xdrv files. The behavior of XDRVTools commands on other video chart files is unpredictable.


## Command List:
1. **Write Empty Note:** Creates an empty note (000-000|00|0) directly at the selection.
2. **Double Subdivisions:** Doubles the subdivisions of all selected beats.
3. **Triple Subdivisions:** Triples the subdivisions of all selected beats.
4. **Quadrouple Subdivisions:** Quadrouples the subdivisions of all selected beats.
---
5. **Mirror Notes:** Mirrors notes horizontally in all selected lines. (Excludes drifts.)
6. **Mirror Events:** Mirrors events in all selected line. (Left → Right, Right → Left)
7. **Mirror Both:** Mirrors both notes and events in the selected lines.
---
8. **Beat / Measure Navigator Dock:** Creates a dock that allows you to easily find a specific beat/measure in the file. (Currently assumes that your song is in 4/4.)
9. **Select Lines**: Expands your current selection to cover the entirety of all contained lines.
10. **Select Beats**: Expands your current selection to cover the entirety of all contained beats.
