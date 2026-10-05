#include <iostream>
#include <cmath>
#include <filesystem>
#include <fstream>
#include "./core/inspector.hpp"
#include "help.hpp"
#define VERSION "v1.0.0"

using std::cout;
using std::floor;
using std::fstream;
using std::ios;
namespace fs = std::filesystem;

void read_files(vector<_GX_file_t> &files, vector<string> filenames)
{
    fstream stream;
    string temp;
    char ch;

    for (string f : filenames)
    {
        const fs::path path = fs::absolute(f);
        if (fs::is_directory(path))
        {
            vector<string> childs;

            for (const auto &entry : fs::directory_iterator(path))
            {
                const string filename = entry.path().filename().string();
                if (filename.substr(filename.length() - 2) == ".s" || filename.substr(filename.length() - 2) == ".S")
                    childs.push_back(fs::absolute(entry.path()).string());
            }
            read_files(files, childs);
        }
        else
        {
            const string filename = path.filename().string();
            if (filename.substr(filename.length() - 2) == ".s" || filename.substr(filename.length() - 2) == ".S")
            {
                stream.open(path, ios::in);
                if (!stream.is_open())
                {
                    cout << "Failed to open '" << path << "'!\n";
                    exit(1);
                }
                while (stream.get(ch))
                {
                    temp += ch;
                }
                if (stream.bad())
                {
                    cout << "Failed to read '" << path << "'!\n";
                    stream.close();
                    exit(1);
                }
                stream.close();
                files.push_back({temp, path.string()});
                temp.clear();
            }
        }
    }
};

int main(int argc, char *argv[])
{
    vector<string> filenames;

    const string flags[] = {"-r", "--registers", "-l", "--labels", "-m", "--macros", "-d", "--defined-instructions", "-p", "--defined-protection-limits", "-a", "--attachments", "-I", "--linter-ignored-lines", "-c", "--circular-inclusion", "-m", "--mainpoint"};
    const string cflags[] = {"-L", "--labels-conflicts", "-M", "--macros-conflicts", "-D", "--defined-instructions-conflicts", "-P", "--defined-protections-limit-conflicts", "-M", "--mainpoint-conflicts"};
    string trace;
    bool marcos = false, registers = false, macro = false, macrosC = false, definitions = false, definitionsC = false, attach = false, mainpoint = false, mainpointC = false, linter_ign = false, labels = false, labelsC = false, plimits = false, plimitsC = false, circular_inclusion = false;
    bool *const bool_flags[] = {&registers, &labels, &macro, &definitions, &plimits, &attach, &linter_ign, &circular_inclusion, &mainpoint};
    bool *const bool_cflags[] = {
        &labelsC,
        &macrosC,
        &definitionsC,
        &plimitsC,
        &mainpointC,
    };

    for (uint32_t i = 1; i < argc; i++)
    {
        const string arg = string(argv[i]);
        if (arg[0] != '-')
        {
            filenames.push_back(arg);
            continue;
        }
        else if (arg == "-V" || arg == "--version")
        {
            cout << VERSION << "\n";
            return 0;
        }
        else if (arg == "-h" || arg == "--help")
        {
            cout << HELP_TXT << "\n";
            return 0;
        }
        else if (arg == "-A" || arg == "--list-all")
            for (uint32_t i = 0; i < 18; i++)
            {
                *(bool_flags[i]) = true;
            }
        else if (arg == "-C" || arg == "--list-all-conflicts")
            for (uint32_t i = 0; i < 10; i++)
            {
                *(bool_cflags[i]) = true;
            }

        if (arg == "-T" || arg == "--trace-file")
        {
            if (i == argc - 1)
            {
                cout << "Error within [-T, --trace-file] argument: value missed!\n";
                return 1;
            }
            trace = argv[i + 1];
            continue;
        }

        int fidx = find_arr<const string, 18>(flags, arg);
        int cidx = find_arr<const string, 10>(cflags, arg);
        if (fidx != -1)
        {
            fidx = floor(fidx / 2);
            *(bool_flags[fidx]) = true;
        }
        else if (cidx != -1)
        {
            cidx = floor(cidx / 2);
            *(bool_cflags[cidx]) = true;
        }
        else
        {
            cout << "Error within [" << arg << "] argument: invalid argument!\n";
            return 1;
        }
    }

    vector<_GX_file_t> files;
    read_files(files, filenames);
    _GX_INSPECTOR_ _inspector_(files);
    _inspector_.analyze();
    if (macro)
    {
        cout << "<=== Macros ===>\n";
        for (_GX_marco_t m : _inspector_.macros)
        {
            cout << '[' << m.filename << "] " << m.from << " -> " << m.to << "\n";
        }
        cout << "\nNote: Conflicts have not been processed in this section.\n";
    }
    if (macrosC)
    {
        cout << "<=== Macros (Conflicts) ===>\n";
        const vector<vector<_GX_marco_t>> conflicts = _inspector_.macro_conflicts();
        for (uint32_t i = 0; i < conflicts.size(); i++)
        {
            cout << "-- " << conflicts[i][0].from << " --\n";
            for (uint32_t j = 0; j < conflicts[i].size(); j++)
            {
                cout << '[' << conflicts[i][j].filename << "] -> " << conflicts[i][j].to << "\n";
            }
        }
    }
    if (registers)
    {
        cout << "<=== Registers ===>\n";
        for (_GX_register_t r : _inspector_.registers())
        {
            cout << r.name << " (" << r.type << ")\n";
        }
    }
    if (labels)
    {
        cout << "<=== Labels ===>\n";
        for (_GX_label_t l : _inspector_.total_labels)
        {
            cout << '[' << l.filename << "] " << l.name << " at line " << l.line_idx + 1 << "\n";
        }
        cout << "\nNote: Conflicts have not been processed in this section.\n";
    }
    if (labelsC)
    {
        cout << "<=== Labels (Conflicts) ===>\n";
        vector<vector<_GX_label_t>> conflicts = _inspector_.label_conflicts();
        for (uint32_t i = 0; i < conflicts.size(); i++)
        {
            cout << "-- " << conflicts[i][0].name << " --\n";
            for (uint32_t j = 0; j < conflicts[i].size(); j++)
            {
                cout << '[' << conflicts[i][j].filename << "] at line " << conflicts[i][j].line_idx << "\n";
            }
        }
    }
    if (definitions)
    {
        cout << "<=== Definitions (Marco-Instructions) ===>\n";
        for (_GX_define_t d : _inspector_.marco_instructions)
        {
            cout << '[' << d.filename << "] " << d.name << ' ';
            for (_GX_define_argument_t a : d.arguments)
            {
                cout << a.name << '(' << a.type << ")";
                if (a.value.size())
                    cout << " = " << a.value << "\n";
            }
        }
        cout << "\nNote: Conflicts have not been processed in this section.\n";
    }
    if (definitionsC)
    {
        vector<vector<_GX_define_t>> defs = _inspector_.macro_instruction_conflicts();
        for (uint32_t i = 0; i < defs.size(); i++)
        {
            cout << "-- " << defs[i][0].name << " --\n";
            for (uint32_t j = 0; j < defs[i].size(); j++)
            {
                cout << '[' << defs[i][j].filename << "] " << defs[i][j].name << ' ';
                for (_GX_define_argument_t a : defs[i][j].arguments)
                {
                    cout << a.name << '(' << a.type << ")";
                    if (a.value.size())
                        cout << " = " << a.value;
                    cout << "\n";
                }
            }
        }
    }
    if (attach)
    {
        cout << "<=== Attachments ===>\n";
        for (_GX_attachment_t a : _inspector_.attachments)
        {
            cout << '[' << a.callername << "] " << a.filename << " (";
            if (a.type == "argular")
                cout << "external-input, argument-based";
            else if (a.type == "extern")
                cout << "external-input, text-based";
            else if (a.type == "include")
                cout << "file inclusion";
            else if (a.type == "import")
                cout << "library importation";
            cout << ")\n";
        }
    }
    if (mainpoint)
    {
        cout << "<=== Mainpoint ===>\n";
        const _GX_mainpoint_t mp = _inspector_.mainpoints[_inspector_.mainpoints.size() - 1];
        if (!mp.name.size())
            cout << "Mainpoint not declared!\n";
        else
        {
            cout << "Mainpoint (Final) declared in [" << mp.filename << "] as '" << mp.name << "'.\n";
            _GX_label_t mainpoint_lbl = {"", "", "", 0};
            for (_GX_label_t lbl : _inspector_.total_labels)
            {
                if (lbl.name == mp.name)
                {
                    mainpoint_lbl = lbl;
                    break;
                }
            }
            if (mainpoint_lbl.filename.empty())
                cout << "Mainpoint not found!\n";
            else
            {
                cout << "Mainpoint (Final) defined in [" << mainpoint_lbl.filename << "] at line " << mainpoint_lbl.line_idx;
                cout << "\n\nNote: Conflicts have not been processed in this section.\n";
            }
        }
    }
    if (mainpointC)
    {
        cout << "<=== Mainpoint (Conflicts) ===>\n";
        vector<_GX_label_t> conflicts = _inspector_.mainpoint_conflicts();
        const _GX_mainpoint_t mp = _inspector_.mainpoints[_inspector_.mainpoints.size() - 1];
        if (!mp.name.size())
            cout << "Mainpoint not declared!\n";
        else
        {
            cout << "Mainpoint (Final) declared in [" << mp.filename << "] as '" << mp.name << "'.\n";
            for (_GX_label_t m : conflicts)
            {
                cout << "A conflict found in [" << m.filename << "] that defined as '" << m.name << "': ";
                if (m.filename.empty())
                    cout << "mainpoint declared again.";
                else
                    cout << "label-related conflict.";
                cout << "\n";
            }
            if (!conflicts.size())
                cout << "No conflicts, fine.\n";
        }
    }
    if (linter_ign)
    {
        cout << "<=== Linter bypassed lines ===>\n";
        vector<_GX_linter_ignored_t> ignored_lines = _inspector_.linter_ignored_lines();
        for (uint32_t i = 0; i < ignored_lines.size(); i++)
        {
            cout << "[" << ignored_lines[i].filename << "]: line " << ignored_lines[i].line_idx << " ignored.\n";
        }
        cout << "\nWarning: linter bypassing would be dangerous often times, should be used carefully.\n";
    }
    if (circular_inclusion)
    {
        cout << "<=== Circular Inclusion Check ===>\n";

        bool conflicted = false;
        for (uint32_t i = 0; i < _inspector_.attachments.size(); i++)
        {
            vector<string> circular = _inspector_.circular_includes(i);
            for (uint32_t j = 0; j < circular.size(); j++)
            {
                if (j)
                    cout << " -> ";
                cout << circular[i][j];
                conflicted = true;
            }
            cout << "\n";
        }
        if (!conflicted)
            cout << "No circular inclusion, fine.\n";
    }

    return 0;
}