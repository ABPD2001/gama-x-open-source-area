#include "inspector.hpp"

_GX_INSPECTOR_::_GX_INSPECTOR_() {};
_GX_INSPECTOR_::_GX_INSPECTOR_(vector<_GX_file_t> files)
{
    this->files = files;
}
void _GX_INSPECTOR_::init(vector<_GX_file_t> files)
{
    this->files = files;
}

void _GX_INSPECTOR_::analyze()
{
    for (_GX_file_t f : this->files)
    {
        vector<string> lines = split(f.content, '\n');
        for (uint32_t i = 0; i < lines.size(); i++)
        {
            string l = lines[i];
            l = split(l, '@')[0];
            l = trim(l);
            if (l.empty())
                continue;

            const vector<string> space_parts = split(l, ' ');
            if (space_parts[0][0] == '.')
            {
                string attachments[] = {".include", ".import", ".argular", ".extern"};
                if (includes_arr<string, 4>(attachments, space_parts[0]))
                {
                    this->attachments.push_back({space_parts[0], space_parts[1], f.name});
                    continue;
                }
                else if (space_parts[0] == ".replace" && space_parts.size() == 3)
                {
                    this->macros.push_back({f.name, space_parts[1], space_parts[2]});
                    continue;
                }
                else if (space_parts[0] == ".define")
                {
                    _GX_define_t def;
                    _GX_define_argument_t arg;
                    def.name = space_parts[1];
                    def.filename = f.name;
                    const vector<string> params = split(space_parts[2], ',');
                    for (string p : params)
                    {
                        uint32_t lvl = 0;
                        for (char c : p)
                        {
                            if (c == ' ' || c == '\t')
                                continue;
                            if (c == '(')
                            {
                                lvl = 1;
                                continue;
                            }
                            else if (c == ')')
                            {
                                lvl = 2;
                                continue;
                            }
                            else if (c == '=')
                            {
                                lvl = 3;
                                continue;
                            }

                            switch (lvl)
                            {
                            case 0:
                                arg.name += c;
                                break;

                            case 1:
                                arg.type += c;
                                break;
                            case 3:
                                arg.value += c;
                                break;
                            default:
                                break;
                            }
                        }
                        def.arguments.push_back(arg);
                        arg.name.clear();
                        arg.type.clear();
                        arg.value.clear();
                    }
                    i++;
                    while (1)
                    {
                        def.text += l + '\n';
                        if (trim(l) == ".enddef")
                            break;
                    }
                    this->marco_instructions.push_back(def);
                }
                else if (space_parts[0] == ".limit")
                {
                    _GX_limit_t limit;
                    limit.filename = f.name;
                    limit.special_register_name = space_parts[1];
                    const vector<string> range = split(space_parts[2], '~');
                    limit.min = stoll(range[0]);
                    limit.max = stoll(range[1]);
                    this->protection_limits.push_back(limit);
                }
                else if (space_parts[0] == ".main" && space_parts.size() >= 2)
                {
                    _GX_mainpoint_t mp = {f.name, space_parts[1]};
                    this->mainpoints.push_back(mp);
                }
            }
            else
            {
                if (space_parts[0].find(":") != string::npos)
                {
                    _GX_label_t lbl;
                    lbl.name = space_parts[0].substr(0, space_parts[0].find(":"));
                    lbl.filename = f.name;
                    lbl.line_idx = i;
                    i++;
                    while (1)
                    {
                        if (trim(l) == "end" || l.find(':') != string::npos)
                            break;
                        lbl.text += l;
                        lbl.text += '\n';
                    }
                    lbl.text = lbl.text.substr(0, lbl.text.length() - 1);
                    this->total_labels.push_back(lbl);
                }
            }
        }
    }
}

vector<_GX_label_t> _GX_INSPECTOR_::mainpoint_conflicts()
{
    vector<_GX_label_t> output;
    if (this->mainpoints.size() >= 1)
    {
        if (this->mainpoints.size() > 1)
        {
            for (_GX_mainpoint_t mp : this->mainpoints)
            {
                bool found = false;
                for (_GX_label_t lbl : this->total_labels)
                {
                    if (lbl.name == mp.name)
                    {
                        output.push_back(lbl);
                        found = true;
                        break;
                    }
                }
                if (!found)
                    output.push_back({mp.filename, "", mp.name, 0});
            }
        }
        else
        {
            for (_GX_label_t lbl : this->total_labels)
            {
                if (lbl.name == this->mainpoints[0].name)
                {
                    output.push_back(lbl);
                    break;
                }
            }
        }
    }
    if (output.size() == 1)
        output.clear();
    return output;
}

uint32_t _GX_INSPECTOR_::mainpoint_verified()
{
    if (this->mainpoint_conflicts().size())
        return 1;
    if (!this->mainpoints.size())
        return 2;
    return 0;
}

vector<vector<_GX_label_t>> _GX_INSPECTOR_::label_conflicts()
{
    vector<vector<_GX_label_t>> output;

    for (_GX_label_t lbl : this->total_labels)
    {
        vector<_GX_label_t> temp;
        for (uint32_t i = 0; i < this->total_labels.size(); i++)
        {
            if (this->total_labels[i].name == lbl.name)
                temp.push_back(this->total_labels[i]);
        }
        if (temp.size() > 1)
        {
            bool push = true;
            for (vector<_GX_label_t> &c : output)
            {
                if (c[0].name == lbl.name)
                {
                    push = false;
                    break;
                }
            }
            if (push)
                output.push_back(temp);
        }
    }
    return output;
}

vector<_GX_linter_ignored_t> _GX_INSPECTOR_::linter_ignored_lines()
{
    vector<_GX_linter_ignored_t> output;
    for (_GX_file_t f : this->files)
    {
        vector<string> lines = split(f.content, '\n');
        for (uintmax_t i = 0; i < lines.size(); i++)
        {
            lines[i] = trim(lines[i]);
            if (lines[i][lines[i].length() - 1] == '$')
                output.push_back({f.name, i + 1});
        }
    }
    return output;
}

vector<vector<_GX_limit_t>> _GX_INSPECTOR_::protection_limits_conflicts()
{
    vector<vector<_GX_limit_t>> output;
    for (_GX_limit_t lim : this->protection_limits)
    {
        vector<_GX_limit_t> temp;
        for (uint32_t i = 0; i < this->protection_limits.size(); i++)
        {
            if (this->protection_limits[i].special_register_name == lim.special_register_name)
                temp.push_back(this->protection_limits[i]);
        }
        output.push_back(temp);
    }

    return output;
}

vector<_GX_register_t> _GX_INSPECTOR_::registers()
{
    vector<_GX_register_t> regs;
    vector<string> reg_names;
    const string no_reg_instructions[] = {"reset", "transpile", "call", "jmp", "cmptxt", "debug"};

    for (_GX_file_t f : this->files)
    {
        const vector<string> lines = split(f.content, '\n');
        for (string l : lines)
        {
            l = trim(l);
            if (l.find(":") != string::npos || l[0] == '.' || l == "end")
                continue;
            const vector<string> parts = split(l, ' ');
            if (parts.size() < 2)
                continue;
            const vector<string> commas = split(parts[1], ',');

            if (includes_arr<const string, 6>(no_reg_instructions, parts[0]))
                continue;
            if (parts[0] == "mvfr")
            {
                if (!includes(reg_names, commas[0]))
                    regs.push_back({f.name, commas[1], "float"});
                if (!includes(reg_names, commas[1]))
                    regs.push_back({f.name, commas[1], "numeric"});
            }
            else if (parts[0] == "mvrf")
            {
                if (!includes(reg_names, commas[1]))
                    regs.push_back({f.name, commas[1], "float"});
                if (!includes(reg_names, commas[0]))
                    regs.push_back({f.name, commas[0], "numeric"});
            }
            else
            {
                if (commas.size())
                    for (uint32_t i = 0; i < commas.size(); i++)
                    {
                        if (commas[i][0] == '#' || commas[i][0] == '"')
                            continue;
                        if (!includes(reg_names, commas[i]))
                        {
                            regs.push_back({f.name, commas[i], parts[0][0] == 'F' ? "float" : "numeric"});
                            reg_names.push_back(commas[i]);
                        }
                    }
                else if (!includes(reg_names, parts[1]) && parts[1][0] != '#' && parts[1][0] != '"')
                {
                    regs.push_back({f.name, parts[1], parts[0][0] == 'F' ? "float" : "numeric"});
                    reg_names.push_back(parts[1]);
                }
            }
        }
    }

    return regs;
}

vector<string> _GX_INSPECTOR_::module_files()
{
    vector<string> output;
    for (_GX_file_t f : this->files)
    {
        const vector<string> lines = split(f.content, '\n');
        for (string l : lines)
        {
            if (trim(l) == ".module")
            {
                output.push_back(f.name);
                break;
            }
        }
    }

    return output;
}

_GX_label_t _GX_INSPECTOR_::mainpoint_label()
{
    _GX_label_t output;
    if (!this->mainpoints.size())
        return output;
    for (_GX_label_t lbl : this->total_labels)
    {

        if (lbl.name == this->mainpoints[0].name)
            return lbl;
    }
    return output;
}

vector<string> _GX_INSPECTOR_::circular_includes(uint32_t idx)
{
    vector<string> trace;
    for (; idx < this->attachments.size(); idx++)
    {
        if (this->attachments[idx].type == "includes")
        {
            trace.push_back(this->attachments[idx].callername);
            for (uint32_t i = idx; i < this->attachments.size(); i++)
            {
                if (includes(trace, this->attachments[idx].callername))
                    break;
                if (this->attachments[idx].callername == this->attachments[i].filename)
                {
                    idx = i;
                    trace.push_back(this->attachments[idx].filename);
                }
            }
        }
    }
    return trace;
}

vector<vector<_GX_marco_t>> _GX_INSPECTOR_::macro_conflicts()
{
    vector<vector<_GX_marco_t>> output;

    for (_GX_marco_t macro : this->macros)
    {
        vector<_GX_marco_t> temp;
        for (uint32_t i = 0; i < this->macros.size(); i++)
        {
            if (this->macros[i].from == macro.from)
                temp.push_back(this->macros[i]);
        }
        bool push = true;
        for (vector<_GX_marco_t> &c : output)
        {
            if (c[0].from == macro.from)
            {
                push = false;
                break;
            }
        }
        if (push)
            output.push_back(temp);
    }
    return output;
}

vector<vector<_GX_define_t>> _GX_INSPECTOR_::macro_instruction_conflicts()
{
    vector<vector<_GX_define_t>> output;
    for (_GX_define_t def : this->marco_instructions)
    {
        vector<_GX_define_t> temp;
        for (uint32_t i = 0; i < this->total_labels.size(); i++)
        {
            if (this->marco_instructions[i].name == def.name)
                temp.push_back(this->marco_instructions[i]);
        }
        output.push_back(temp);
    }
    return output;
}