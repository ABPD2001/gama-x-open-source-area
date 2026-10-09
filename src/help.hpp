#ifndef HELP_HPP
#define HELP_HPP

constexpr char HELP_TXT[] = R"(Usage: gxinsp [FILES...] [FLAGS...]
Gama-X Program Inspector, analyzes a program and determines its structure from the perspectives of execution, dependencies on other files, and more. It can also detect conflicts within the program. 

Flags:
    [-h, --help]:                                   Print this text.
    [-V, --version]:                                Print version.

    [-T, --trace-file]:                             Use linker trace as input files.
    [-r, --registers]:                              List registers that are used inside of program.
    [-l, --labels]:                                 List defined labels.
    [-L, --labels-conflicts]:                       List conflicts of labels.
    [-m, --macros]:                                 List defined macros ('.replace' pre-processors).
    [-M, --macros-conflicts]:                       List conflicts of defined macros ('.replace' pre-processors).
    [-d, --defined-instructions]:                   List macro-defined instructions ('.define' pre-processors).
    [-D, --defined-instructions-conflicts]:         List macro-defined instructions conflicts ('.define' pre-processors).
    [-p, --defined-protection-limits]:              List defined protection limits of program ('.limit' pre-processors). 
    [-P, --defined-protection-limits-conflicts]:    List defined protection limits conflicts of program ('.limit' pre-processors). 
    [-a, --attachments]:                            List attachments of program, that means, external inputs, included files and imported libraries.
    [-I, --linter-ignored-lines]:                   List lines that are gonna ignored by linter because of linter-bypassing mechanisem ('$' sign at end of line).
    [-mo, --module-files]:                          List module (no mainpoint) files.
    [-mp, --mainpoint]:                             Check for mainpoint of program.
    [-MP, --mainpoint-conflicts]:                   Check for mainpoint conflicts by label-name or multi mainpoint definition.

    [-A, --list-all]:                               List all listable things.
    [-C, --list-all-conflicts]:                     List all conflicted things.

Note: linker trace is generatable by Gama-X Compiler (gx) within -T flag.
Note: passing directory as file means all children of that directory.

v1.0.0
<--- Gama-X Program Inspector --->)";
#endif