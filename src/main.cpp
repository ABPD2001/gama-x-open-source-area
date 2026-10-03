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
        const fs::path path = fs::absolute(f).string();
        if (fs::is_directory(path))
        {
            vector<string> childs;

            for (const auto &entry : fs::directory_iterator(path))
            {
                const string filename = entry.path().filename();
                if (filename.substr(filename.length() - 3) == ".s" || filename.substr(filename.length() - 3) == ".S")
                    childs.push_back(entry.path().string());
            }
            read_files(files, childs);
        }
        else
        {
            const string filename = path.filename().string();
            if (filename.substr(filename.length() - 3) == ".s" || filename.substr(filename.length() - 3) == ".S")
            {
                stream.open(path, ios::in);
                if (!stream.is_open())
                {
                    cout << "Failed to open '" << path << "'!\n";
                    return 1;
                }
                while (stream.get(ch))
                {
                    temp += ch;
                }
                if (stream.bad())
                {
                    cout << "Failed to read '" << path << "'!\n";
                    stream.close();
                    return 1;
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
    const bool &bool_flags[] = {registers, labels, macro, definitions, plimits, attach, linter_ign, circular_inclusion, mainpoint};
    const bool &bool_cflags[] = {
        labelsC,
        macrosC,
        definitionsC,
        plimitsC,
        mainpointC,
    };

    for (uint32_t i = 0; i < argc; i++)
    {
        const string arg = string(argv[i]);
        if (arg[0] != '-')
            filenames.push_back(arg);
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
                flags[i] = true;
            }
        else if (arg == "-C" || arg == "--list-all-conflicts")
            for (uint32_t i = 0; i < 10; i++)
            {
                cflags[i] = true;
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

        const int fidx = find_arr<string, 18>(flags, arg);
        const int cidx = find_arr<string, 10>(cflags, arg);
        if (fidx != -1)
            bool_flags[floor(fidx / 2)] = true;
        else if (cidx != -1)
            bool_cflags[floor(cidx / 2)] = true;
        else
        {
            cout << "Error within [" << arg << "] argument: invalid argument!\n";
            return 1;
        }
    }

    vector<_GX_file_t> files;
    read_files(files, filenames);

    return 0;
}