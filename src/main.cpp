#include <iostream>
#include <core/caster.hpp>
#include <core/packeter.hpp>
#include <core/snipper.hpp>
#include <utils/vector.hpp>
#include <help.hpp>

using std::cout;

_GX_CASTER _caster_;
_GX_SNIPPER _snipper_;
_GX_PACKETER _packeter_;
_GX_SNIPPER_config_t _snipper_conf_;
_GX_CASTER_format_t _caster_format_from_;
_GX_CASTER_format_t *_caster_format_to_ = 0;
uint32_t packeter_size = 0, multi_casting = 1;

void arg_error(string argname, string reason)
{
    cout << "Error within [" << argname << "] argument: " << reason;
}

void caster_format_processing(string arg, string val, _GX_CASTER_format_t &format)
{
    const string f = arg.substr(arg.find_first_not_of("-"));

    if (f == "f" || f == "format-logic")
        format.format = value;
    else if (f == "b" || f == "binary")
        format.binary = true;
    else if (f == "B" || f == "binary-big-endian")
        format.endianness = true;
    else if (f == "c" || f == "seperator-char")
        format.seperator = value[0];
}

void args_processing(vector<string> &values, string &output, char **argv, int argc)
{
    vector<string> valids = {"-o", "--output"};
    vector<string> valuars = {"-o", "--output"};
    _GX_CASTER_format_t temp_f;
    bool multicasting = false;

    if (argv[1] == "snipper")
    {
        vector<string> valid_args = {"-hf", "--header-format", "-ff", "--footer-format", "-hc", "--header-seperator-char", "-Ahc", "--header-seperator-char-ascii", "-fc", "--footer-seperator-char", "-Afc", "--footer-seperator-char-ascii", "-A", "--alignment", "-p", "--padding-filler-char", "-Ap", "--padding-filler-char-ascii", "-B", "--full-binary", "-hb", "--header-binary", "-fb", "--footer-binary", "-b", "--binary-big-endian", "-c", "--snippets-seperator-char", "-Ac", "--snippets-seperator-char-ascii"};
        vector<string> valuar_args = {"-hf", "--header-format", "-ff", "--footer-format", "-hc", "--header-seperator-char", "-Ahc", "--header-seperator-char-ascii", "-fc", "--footer-seperator-char", "-Afc", "--footer-seperator-char-ascii", "-A", "--alignment", "-p", "--padding-filler-char", "-Ap", "--padding-filler-char-ascii", "-s", "--snippets-seperator-char", "-As", "--snippets-seperator-char-ascii"};
        valids.insert(valids.end(), valid_args.begin(), valid_args.end());
        valuars.insert(valuars.end(), valuar_args.begin(), valuar_args.end());
    }
    else if (argv[1] == "caster")
    {
        vector<string> valid_args = {"-m", "--multi-cast", "-f", "--format-logic", "-b", "--binary", "-B", "--binary-big-endian", "-c", "--seperator-char", "-Ac", "--seperator-char-ascii", "-tf", "--target-format-logic", "-tb", "--target-binary", "-tB", "--target-binary-big-endian", "-tc", "--target-seperator-char", "-Atc", "--target-seperator-char-ascii"};
        vector<string> valuar_args = {"-m", "--multi-cast", "-f", "--format-logic", "-c", "--seperator-char", "-Ac", "--seperator-char-ascii", "-Ac", "--seperator-char-ascii", "-tf", "--target-format-logic", "-tb", "--target-binary", "-tB", "--target-binary-big-endian", "-tc", "--target-seperator-char", "-Atc", "--target-seperator-char-ascii"};
        valids.insert(valids.end(), valid_args.begin(), valid_args.end());
        valuars.insert(valuars.end(), valuar_args.begin(), valuar_args.end());
    }
    else if (argv[1] == "packeter")
    {
        valids.push_back("-s");
        valids.push_back("--packet-size");
        valuars.push_back("-s");
        valuars.push_back("--packet-size");
    }
    for (uint32_t i = 2; i < argc; i++)
    {
        const string argument = argv[i];
        if (argument[0] != '-')
        {
            values.push_back(argument);
            continue;
        }
        const string value = i == argc - 1 ? "" : argv[i + 1];

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
        if (argv[1] == "snipper")
        {
            const string value = argv[i + 1];

            if (argument == "-hf" || argument == "--header-format")
                _snipper_conf_.header_format = value;
            else if (argument == "-ff" || argument == "--footer-format")
                _snipper_conf_.footer_format = value;

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

            else if (argument == "-hb" || argument == "--header-binary")
                _snipper_conf_.header_binary = true;

            else if (argument == "-fb" || argument == "--footer-binary")
                _snipper_conf_.footer_binary = true;

            else if (argument == "-b" || argument == "--binary-big-endian")
                _snipper_conf_.binary_endianness = true;

            else if (argument == "-B" || argument == "--full-binary")
            {
                _snipper_conf_.footer_binary = true;
                _snipper_conf_.header_binary = true;
            }

            else if (argument == "-A" || argument == "--alignment")
                _snipper_conf_.alignment = to_uint32(value);
        }
        else if (argv[1] == "packeter")
        {

            if (argument == "-s" || argument == "--packet-size")
                packeter_size = to_uint32(value);
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
                _caster_format_from_.endianness = true;
            else if (argument == "-c" || argument == "--seperator-char")
                _caster_format_from_.seperator = value[0];
            else if (argument == "-Ac" || argument == "--seperator-char-ascii")
                _caster_format_from_.seperator = (char)to_uint32(value);
        }
    }
}

int main(char **argv, int argc)
{
    vector<string> params;
    string output;
    if (argc < 2)
    {
        cout << "Invalid usage, verb required!\n"
             << HELP_TXT << "\n";
        exit(1);
    }
    else if (argv[1] != "caster" && argv[1] != "snipper" && argv[1] != "packeter")
    {
        cout << "Unkown verb '" << argv[1] << "'!\n"
             << HELP_TXT << "\n";
        exit(1);
    }
    args_processing(&params, argv, argc); // stage 1 processing.

    if (argv[1] == "caster")
    {
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
    }

    else if (argv[1] == "snipper")
    {
        if (!_snipper_conf_.header_format)
        {
            arg_error("-hf, --header-format", "it's required!");
            exit(1);
        }
        if (_snipper_conf_.footer_binary && !_snipper_conf_.footer_format.length())
        {
            arg_error("-ff, --footer-format", "it's required when [-fb, --footer-binary] flag!");
            exit(1);
        }
    }

    else if (argv[1] == "packeter" && !packeter_size)
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

    // ...

    return 0;
}