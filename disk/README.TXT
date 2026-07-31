The game data lives in disk/extract/ (created by tools/extract.ps1) and is NOT committed.

To get it, run the extraction script with YOUR LSD: Dream Emulator disc image
(a .cue or .bin you dump). It will create disk/extract/ automatically:

    pwsh tools/extract.ps1 -Image C:\path\to\your\LSD.cue

Or u can drop the .cue/.bin into this folder and run:

    pwsh tools/extract.ps1

Either way, you need the game disc - I can not distribute it, so it's not included here.
Do NOT commit the files in disk/extract/ (already gitignored).
