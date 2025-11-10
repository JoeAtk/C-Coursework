Purpose
This file gives concise, actionable instructions for AI coding agents working in this repo. Focus on the small C coursework app that emits a simple text graphics protocol and is displayed by the provided launcher binary.

Big picture
The project is plain C sources that emit a textual drawing protocol to stdout. The display renderer is the launcher binary (in the repo root). Typical runtime is: run the program and pipe its stdout into launcher so the textual commands are rendered.
Key files:
graphics.h / graphics.c — the graphics API used by student code. Instead of drawing directly, functions print protocol commands (e.g. DL, DR, MS, SC, SW).
myProgram.c (example student program) — shows how the API is used (grid drawing, setWindowSize, fillRect, etc.).
launcher — binary consumer of protocol (reads textual commands and displays UI). It is not editable as text in the repo.
Runtime / build workflow (PowerShell)
Quick compile (gcc) for development and debugging (PowerShell):
gcc -g -O0 myProgram.c graphics.c -o myProgram.exe
.\myProgram.exe | .\launcher
Release build (strip optimizations):
gcc -O2 myProgram.c graphics.c -o myProgram.exe
.\myProgram.exe | .\launcher
Notes: graphics.c prints protocol tokens to stdout. launcher reads stdin — so piping (|) connects them.

Project-specific patterns and gotchas
Protocol tokens (examples — keep these stable if you change graphics.c):

drawLine -> prints: DL x1 x2 x3 x4
drawRect -> DR x y w h
fillRect -> FR x y w h
drawString -> DS x y @text — strings are prefixed with @ and newlines are replaced with the HTML newline entity (ampersand-hash-ten-semicolon) by replaceNewlineWithEntity.
setColour -> SC <name> and setRGBColour -> RG r g b
setWindowSize -> SW width height
The textual protocol is the contract between student code and launcher. If you change command names or formats in graphics.c, update launcher or the renderer that consumes these commands.

graphics.h contains the API prototypes that student code uses — prefer editing behavior there and keep signatures stable.

Examples to copy/paste
Use the API exactly as in myProgram.c:

set a window size: setWindowSize(windowSize+10, windowSize+10);
draw a grid: loop with drawLine(...) as in myProgram.c.
fill a cell: fillRect(x, y, w, h); (see fillGrid helper in myProgram.c).
Debugging hints
Add -g -O0 when compiling to enable source-level debugging with gdb (on platforms that support it). Use printf logging — the program already prints graphics commands so interleave debug prints carefully (they will appear on stdout unless redirected).
To inspect the exact textual protocol output without launcher, run and redirect to a file: .\myProgram.exe > out.txt then open out.txt.
When to ask maintainer / human
If you change the text protocol (any printf format in graphics.c), ask for a test update or provide a compatibility layer — launcher expects the current tokens.
If you need to modify launcher but only the binary is present, request the source from the repo owner.
If any part of this is unclear or you'd like examples for adding automated tests or a Makefile, tell me which area to expand and I will update this doc.
