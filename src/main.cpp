#include <iostream>
#include <fstream>
#include "./core/caster.hpp"
#include "./core/packeter.hpp"
#include "./core/snipper.hpp"
#include "./utils/vector.hpp"
#include "./help.hpp"
#define VERSION "v1.0.0"

using std::cout;
using std::fstream;
using std::ios;

_GX_CASTER _caster_;
_GX_SNIPPER _snipper_;
_GX_PACKETER _packeter_;
_GX_SNIPPER_config_t _snipper_conf_;
_GX_CASTER_format_t _caster_format_from_;
_GX_CASTER_format_t *_caster_format_to_ = 0;
string config;
uint32_t packeter_size = 0, multi_casting = 1;
char merge_sepc = '\0';
bool binary = false, single_meta = false;

void arg_error(string argname, string reason)
{
    cout << "Error within [" << argname << "] argument: " << reason;
}

inline string digiter(string num, uint32_t count)
{
    string output = "";
    for (uint32_t i = 0; i < count - num.length(); i++)
    {
        output += '0';
    }
    output += num;
    return output;
}

void output_filename_parser(vector<string> &output_names, string output, uint32_t counts)
{
    uint32_t dcounts = 0, idx = 0;
    string replace_str = "";
    for (char c : output)
    {
        if (c == '$')
        {
            dcounts++;
            replace_str += '$';
        }
        else if (dcounts && c != '$')
            break;
    }
    for (uint32_t i = 0; i < counts; i++)
    {
        output_names.push_back(replaceAll(output, replace_str, digiter(to_string(i), dcounts)));
    }
};

void args_processing(vector<string> &values, string &output, char **argv, int argc)
{
    vector<string> valids = {"-o", "--output"};
    vector<string> valuars = {"-o", "--output"};
    string arg = argv[1];
    _GX_CASTER_format_t temp_f;
    bool multicasting = false;

    if (arg == "snipper")
    {
        vector<string> valid_args = {"-C", "--config", "-hc", "--header-seperator-char", "-Ahc", "--header-seperator-char-ascii", "-fc", "--footer-seperator-char", "-Afc", "--footer-seperator-char-ascii", "-A", "--alignment", "-p", "--padding-filler-char", "-Ap", "--padding-filler-char-ascii", "-B", "--binary", "-b", "--binary-big-endian", "-c", "--snippets-seperator-char", "-Ac", "--snippets-seperator-char-ascii", "-S", "--meta-snipping"};
        vector<string> valuar_args = {"-C", "--config", "-hc", "--header-seperator-char", "-Ahc", "--header-seperator-char-ascii", "-fc", "--footer-seperator-char", "-Afc", "--footer-seperator-char-ascii", "-A", "--alignment", "-p", "--padding-filler-char", "-Ap", "--padding-filler-char-ascii", "-s", "--snippets-seperator-char", "-As", "--snippets-seperator-char-ascii"};
        valids.insert(valids.end(), valid_args.begin(), valid_args.end());
        valuars.insert(valuars.end(), valuar_args.begin(), valuar_args.end());
    }
    else if (arg == "caster")
    {
        vector<string> valid_args = {"-m", "--multi-cast", "-f", "--format-logic", "-b", "--binary", "-B", "--binary-big-endian", "-c", "--seperator-char", "-Ac", "--seperator-char-ascii", "-tf", "--target-format-logic", "-tb", "--target-binary", "-tB", "--target-binary-big-endian", "-tc", "--target-seperator-char", "-Atc", "--target-seperator-char-ascii"};
        vector<string> valuar_args = {"-m", "--multi-cast", "-f", "--format-logic", "-c", "--seperator-char", "-Ac", "--seperator-char-ascii", "-Ac", "--seperator-char-ascii", "-tf", "--target-format-logic", "-tb", "--target-binary", "-tB", "--target-binary-big-endian", "-tc", "--target-seperator-char", "-Atc", "--target-seperator-char-ascii"};
        valids.insert(valids.end(), valid_args.begin(), valid_args.end());
        valuars.insert(valuars.end(), valuar_args.begin(), valuar_args.end());
    }
    else if (arg == "packeter")
    {
        vector<string> valid_args = {"-s", "--packet-size", "-b", "--binary"};
        vector<string> valuar_args = {"-s", "--packet-size"};
        valids.insert(valids.end(), valid_args.begin(), valid_args.end());
        valuars.insert(valuars.end(), valuar_args.begin(), valuar_args.end());
    }
    else if (arg == "merge")
    {
        vector<string> valid_args = {"-b", "--binary", "-c", "--seperator-char", "-Ac", "--seperator-char-ascii"};
        vector<string> valuar_args = {"-c", "--seperator-char", "-Ac", "--seperator-char-ascii"};
        valids.insert(valids.end(), valid_args.begin(), valid_args.end());
        valuars.insert(valuars.end(), valuar_args.begin(), valuar_args.end());
    }
    for (uint32_t i = 2; i < argc; i++)
    {
        const string argument = argv[i], value = (i == argc - 1 ? "" : argv[i + 1]);

        if (argument[0] != '-')
        {
            values.push_back(argument);
            continue;
        }

        if (!argument[0] == '-' && !includes<string>(valids, argument))
        {
            arg_error(argument, "undefined argument!");
            exit(1);
        }
        if (includes(valuars, argument) && i == argc - 1)
        {
            arg_error(argument, "value missed!");
            exit(1);
        }
        if (argument == "-o" || argument == "--output")
        {
            output = value;
            continue;
        }
        if (arg == "snipper")
        {

            if (argument == "-C" || argument == "--config")
                config = value;

            if (argument == "-S" || argument == "--meta-snipping")
                single_meta = true;

            else if (argument == "-fc" || argument == "--footer-seperator-char")
                _snipper_conf_.footer_content_seperator = value[0];
            else if (argument == "-Afc" || argument == "--footer-seperator-char-ascii")
                _snipper_conf_.footer_content_seperator = (char)to_uint32(value);

            else if (argument == "-hc" || argument == "--header-seperator-char")
                _snipper_conf_.header_content_seperator = value[0];
            else if (argument == "-Ahc" || argument == "--header-seperator-char-ascii")
                _snipper_conf_.header_content_seperator = (char)to_uint32(value);

            else if (argument == "-p" || argument == "--padding-filler-char")
                _snipper_conf_.padding_filler = value[0];
            else if (argument == "-Ap" || argument == "--padding-filler-char-ascii")
                _snipper_conf_.padding_filler = (char)to_uint32(value);

            else if (argument == "-c" || argument == "--snippets-seperator-char")
                _snipper_conf_.snippet_seperator = value[0];
            else if (argument == "-Ac" || argument == "--snippets-seperator-char-ascii")
                _snipper_conf_.snippet_seperator = (char)to_uint32(value);

            else if (argument == "-B" || argument == "--binary-big-endian")
            {
                binary = true;
                _snipper_conf_.binary_endianness = true;
            }

            else if (argument == "-b" || argument == "--binary")
                binary = true;

            else if (argument == "-A" || argument == "--alignment")
                _snipper_conf_.alignment = to_uint32(value);
        }
        else if (arg == "packeter")
        {
            if (argument == "-s" || argument == "--packet-size")
                packeter_size = to_uint32(value);
            else if (argument == "-b" || argument == "--binary")
                binary = true;
        }
        else if (arg == "merge")
        {
            if (argument == "-b" || argument == "--binary")
                binary = true;
            else if (argument == "-c" || argument == "--seperator-char")
                merge_sepc = value[0];
            else if (argument == "-Ac" || argument == "--seperator-char-ascii")
                merge_sepc = (char)to_uint32(value);
        }
        else
        {
            if (argument == "-m" || argument == "--multi-cast")
                multi_casting = to_uint32(value);
            else if (argument == "-f" || argument == "--format-logic")
                _caster_format_from_.format = value;
            else if (argument == "-b" || argument == "--binary")
                _caster_format_from_.binary = true;
            else if (argument == "-B" || argument == "--binary-big-endian")
            {
                _caster_format_from_.binary = true;
                _caster_format_from_.endianness = true;
            }
            else if (argument == "-c" || argument == "--seperator-char")
                _caster_format_from_.seperator = value[0];
            else if (argument == "-Ac" || argument == "--seperator-char-ascii")
                _caster_format_from_.seperator = (char)to_uint32(value);
        }
    }
}

bool parse_fields_txt(vector<vector<_GX_SNIPPER_field_t>> &ffields, vector<vector<_GX_SNIPPER_field_t>> &hfields, string content)
{
    vector<string> lines = split(content, '\n');
    uint32_t hf_brack = 0;

    for (string l : lines)
    {
        l = trim(l);
        if (l == "[HEADER]")
        {
            hf_brack = 1;
            continue;
        }
        else if (l == "[FOOTER]")
        {
            hf_brack = 2;
            continue;
        }
        else if (hf_brack == 1)
        {
            const vector<string> parts = split(l, ';');
            vector<_GX_SNIPPER_field_t> fields;
            for (string p : parts)
            {
                const vector<string> kv = split(p, '=');
                if (kv.size() != 2)
                    return false;
                _GX_SNIPPER_field_t f = {kv[0], kv[1]};
                fields.push_back(f);
            }
            hfields.push_back(fields);
        }
        else if (hf_brack == 2)
        {
            const vector<string> parts = split(l, ';');
            vector<_GX_SNIPPER_field_t> fields;
            for (string p : parts)
            {
                const vector<string> kv = split(p, '=');
                if (kv.size() != 2)
                    return false;
                _GX_SNIPPER_field_t f = {kv[0], kv[1]};
                fields.push_back(f);
            }
            ffields.push_back(fields);
        }
    }
}
bool parse_fields_bin(vector<vector<string>> &ffields, vector<vector<string>> &hfields, string &content)
{
    vector<string> lines = split(content, '\n');
    uint32_t hf_brack = 0;

    for (string l : lines)
    {
        l = trim(l);
        if (l == "[HEADER]")
        {
            hf_brack = 1;
            continue;
        }
        else if (l == "[FOOTER]")
        {
            hf_brack = 2;
            continue;
        }
        else if (hf_brack == 1)
        {
            const vector<string> parts = split(l, ',');
            ffields.push_back(parts);
        }
        else if (hf_brack == 2)
        {
            const vector<string> parts = split(l, ',');
            ffields.push_back(parts);
        }
    }
}

void read_formats(string &footer_format, string &header_format, string &content)
{
    vector<string> lines = split(content, '\n');
    uint32_t hf_brack = 0;

    for (string l : lines)
    {
        l = split(l, '#')[0];
        l = trim(l);
        if (l.empty())
            continue;

        if (!footer_format.size())
        {
            const uint32_t foot_idx = l.find("[FOOTER FORMAT]");

            if (foot_idx != string::npos)
            {
                hf_brack = 2;
                const string temp = content.substr(foot_idx + 15);
                footer_format = temp.substr(temp.find_first_of('('), temp.find_first_of(')'));
            }
        }
        else if (header_format.size())
        {
            const uint32_t head_idx = l.find("[HEADER FORMAT]");

            if (head_idx != string::npos)
            {
                const string temp = content.substr(head_idx + 15);
                header_format = temp.substr(temp.find_first_of('('), temp.find_first_of(')'));
            }
        }
        else
            break;
    }
}

int main(int argc, char **argv)
{
    vector<string> params;
    string output, arg;
    if (argc < 2)
    {
        cout << "Invalid usage, verb required!\n"
             << HELP_TXT << "\n";
        exit(1);
    }
    arg = argv[1];
    if (arg != "help" && arg != "version" && arg != "merge" && arg != "caster" && arg != "snipper" && arg != "packeter")
    {
        cout << "Unkown verb '" << arg << "'!\n"
             << HELP_TXT << "\n";
        exit(1);
    }
    if (arg == "help")
    {
        cout << HELP_TXT << "\n";
        exit(0);
    }
    else if (arg == "version")
    {
        cout << VERSION << "\n";
        exit(0);
    }
    args_processing(params, output, argv, argc); // stage 1 processing.

    if (arg == "caster")
    {
        const vector<string> outputs = split(output, ',');
        if (outputs.size() != multi_casting)
        {
            arg_error("-o, --output", "it's incomplete!");
            exit(1);
        }
        _caster_format_to_ = new _GX_CASTER_format_t[multi_casting];
        for (uint32_t i = 2; i < argc; i++)
        {
            const string argument = argv[i];
            const vector<string> values = split(argv[i + 1], ',');

            if (argument == "-tf" || argument == "--target-format-logic")
                for (uint32_t j = 0; j < values.size(); j++)
                {
                    _caster_format_to_[j].format = values[j];
                }

            else if (argument == "-tb" || argument == "--target-binary")
                for (uint32_t j = 0; j < values.size(); j++)
                {
                    _caster_format_to_[j].binary = values[j][0] == 't';
                }
            else if (argument == "-tB" || argument == "--target-binary-big-endian")
                for (uint32_t j = 0; j < values.size(); j++)
                {
                    _caster_format_to_[j].endianness = values[j][0] == 't';
                }
            else if (argument == "-tc" || argument == "--target-seperator-char")
                for (uint32_t j = 0; j < values.size(); j++)
                {
                    _caster_format_to_[j].seperator = values[j][0];
                }

            else if (argument == "-Atc" || argument == "--target-seperator-char-ascii")
                for (uint32_t j = 0; j < values.size(); j++)
                {
                    _caster_format_to_[j].seperator = (char)to_uint32(values[j]);
                }
        }
        if (!_caster_format_from_.format.length())
        {
            arg_error("-f, --format-logic", "it's required!");
            exit(1);
        }
        for (uint32_t i = 0; i < multi_casting; i++)
        {
            if (!_caster_format_to_[i].format.length())
            {
                arg_error("-t, --target-format-logic", "it's incomplete!");
                exit(1);
            }
        }
        if (params.size() > 1)
        {
            cout << "Multi-Input is not acceptable within 'caster' verb, use 'merge' verb, then try again!\n";
            exit(1);
        }
    }

    else if (arg == "snipper")
    {
        if (_snipper_conf_.header_format.empty())
        {
            arg_error("-hf, --header-format", "it's required!");
            exit(1);
        }
        if (binary && _snipper_conf_.footer_format.empty())
        {
            arg_error("-ff, --footer-format", "it's required when [-fb, --footer-binary] flag!");
            exit(1);
        }
    }

    else if (arg == "packeter" && !packeter_size)
    {
        arg_error("-s, --packet-size", "should be valued as more than zero!");
        exit(1);
    }

    if (!output.size())
    {
        arg_error("-o, --output", "it's required!");
        exit(1);
    }
    if (!params.size())
    {
        cout << "Invalid usage, input files required!\n"
             << HELP_TXT << "\n";
        exit(1);
    }

    fstream f_inp;
    fstream f_out;
    if (arg == "merge")
    {
        char ch;

        f_out.open(output, (binary ? ios::out | ios::binary : ios::out));
        if (!f_out.is_open())
        {
            cout << "Failed to open (as output destiniation) '" << output << "'!\n";
            exit(1);
        }
        for (string file : params)
        {
            f_inp.open(file, (binary ? ios::in | ios::binary : ios::in));
            if (!f_inp.is_open())
            {
                cout << "Failed to open '" << file << "'!\n";
                exit(1);
            }

            string temp_content;
            while (f_inp.get(ch))
            {
                temp_content += ch;
            }

            if (f_inp.bad())
            {
                cout << "Failed to read from '" << file << "'!\n";
                goto file_action_fail;
            }
            f_out << temp_content;
            if (f_out.bad())
            {
                cout << "Failed to write into (as output) '" << output << "'!\n";
                goto file_action_fail;
            }
            f_inp.close();

            f_out.close(); // close the output.
        }
        exit(0);

    file_action_fail:
        f_out.close();
        f_inp.close();
        exit(1);
    }
    else if (arg == "packeter")
    {
        vector<string> output_packets;
        vector<string> output_filenames;
        uint32_t fidx = 0;

        _packeter_.config(packeter_size);

        for (string file : params)
        {
            f_inp.open(params[0], (binary ? ios::in | ios::binary : ios::in));
            if (!f_inp.is_open())
            {
                cout << "Failed to open '" << file << "'!\n";
                f_inp.close();
                exit(1);
            }

            string temp;
            char ch;

            while (f_inp.get(ch))
            {
                temp += ch;
            }
            if (f_inp.bad())
            {
                cout << "Failed to read from '" << file << "'!\n";
                f_inp.close();
                exit(1);
            }
            f_inp.close();
            _packeter_.generate(output_packets, temp);
        }
        output_filename_parser(output_filenames, output, output_packets.size());
        for (uint32_t i = 0; i < output_filenames.size(); i++)
        {
            f_out.open(output_filenames[i], (binary ? ios::out | ios::binary : ios::out));
            if (!f_out.is_open())
            {
                cout << "Failed to open (binary as output) '" << output_filenames[i] << "'!\n";
                exit(1);
            }

            f_out.write((char *)output_packets[i].data(), output_packets[i].size());
            f_out.close();
            if (f_out.bad())
            {
                cout << "Failed to write into (" << (binary ? "binary as " : "") << "output) '" << output_filenames[i] << "'!\n";
                f_out.close();
                exit(1);
            }
            f_out.close();
        }
    }
    else if (arg == "caster")
    {
        _caster_.config(_caster_format_from_);

        f_inp.open(params[0], (_caster_format_from_.binary ? ios::out | ios::binary : ios::out));
        if (!f_inp.is_open())
        {
            cout << "Failed to open '" << params[0] << "'!\n";
            exit(1);
        }

        const vector<string> outputs = split(output, ',');
        string temp;
        char ch;

        for (uint32_t i = 0; i < outputs.size(); i++)
        {
            f_inp.open(_caster_format_to_[i].format, ios::in);
            if (!f_inp.is_open())
            {
                cout << "Failed to open format/logic config file '" << _caster_format_to_[i].format << "'!\n";
                exit(1);
            }
            while (f_inp.get(ch))
            {
                temp += ch;
            }
            if (f_inp.bad())
            {
                cout << "Failed to read format/logic config file '" << _caster_format_to_[i].format << "'!\n";
                f_inp.close();
                exit(1);
            }
            f_inp.close();
            _caster_format_to_[i].format = temp;
        }
        temp.clear();

        while (f_inp.get(ch))
        {
            temp += ch;
        }
        if (f_inp.bad())
        {
            cout << "Failed to read from '" << params[0] << "'!\n";
            f_inp.close();
            exit(1);
        }

        _caster_.update(temp);
        for (uint32_t i = 0; i < multi_casting; i++)
        {
            f_out.open(outputs[i], (_caster_format_to_[i].binary ? ios::out | ios::binary : ios::out));
            if (!f_out.is_open())
            {
                cout << "Failed to open (output" << (_caster_format_to_[i].binary ? " as binary" : "") << ") '" << outputs[i] << "'!\n";
                f_out.close();
                exit(1);
            }
            const string output_str = _caster_.cast_format(_caster_format_to_[i]);
            f_out.write((char *)output_str.data(), output_str.size());
            if (f_out.bad())
            {
                cout << "Failed to write into (output" << (_caster_format_to_[i].binary ? " as binary" : "") << ") '" << outputs[i] << "'!\n";
                f_out.close();
                exit(1);
            }
            f_out.close();
        }
    }
    else if (arg == "snipper")
    {
        vector<string> content;
        string header_format, footer_format, temp, output;
        char ch;

        for (string file : params)
        {
            f_inp.open(file, (binary ? ios::out | ios::binary : ios::out));
            if (!f_inp.is_open())
            {
                cout << "Failed to open at '" << file << "'!\n";
                f_inp.close();
                exit(1);
            }
            while (f_inp.get(ch))
            {
                temp += ch;
            }
            if (f_inp.bad())
            {
                cout << "Failed to read at '" << file << "'!\n";
                f_inp.close();
                exit(1);
            }
            f_inp.close();
            content.push_back(temp);
        }

        if (config.length())
        {
            f_inp.open(config, ios::in);
            if (!f_inp.is_open())
            {
                cout << "Failed to open config at '" << config << "'!\n";
                f_inp.close();
                exit(1);
            }
            while (f_inp.get(ch))
            {
                temp += ch;
            }
            if (f_inp.bad())
            {
                cout << "Failed to read config at '" << config << "'!\n";
                f_inp.close();
                exit(1);
            }
            f_inp.close();
        }
        config = temp;
        read_formats(_snipper_conf_.footer_format, _snipper_conf_.header_format, config);
        _snipper_.config(_snipper_conf_);

        if (binary)
        {
            vector<vector<string>> hfields, ffields;
            parse_fields_bin(ffields, hfields, config);
            if (single_meta)
                temp = _snipper_.binout_single(content[0], ffields[0], hfields[0]);
            else
                temp = _snipper_.binout(content, ffields, hfields);
        }
        else
        {
            vector<vector<_GX_SNIPPER_field_t>> hfields, ffields;
            parse_fields_txt(ffields, hfields, config);
            if (single_meta)
                temp = _snipper_.txtout_single(content[0], ffields[0], hfields[0]);
            else
                temp = _snipper_.txtout(content, ffields, hfields);
        }

        f_out.open(output, (binary ? ios::out | ios::binary : ios::out));
        if (!f_out.is_open())
        {
            cout << "Failed to open (output) '" << output << "'!\n";
            exit(1);
        }
        f_out.write((char *)temp.data(), temp.size());
        if (f_out.bad())
        {
            cout << "Failed to write into (output) '" << output << "'!\n";
            f_out.close();
            exit(1);
        }
        f_out.close();
    }

    return 0;
}