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
            cout << l << "\n";
            if (l.empty())
                continue;

            const vector<string> space_parts = split(l, ' ');
            if (space_parts[0][0] == '.')
            {
                l = l.substr(1);
                string attachments[] = {"include", "import", "argular", "extern"};
                if (includes_arr<string, 4>(attachments, space_parts[0]))
                {
                    this->attachments.push_back({space_parts[0], space_parts[1], f.name});
                    continue;
                }
                else if (space_parts[0] == "replace")
                {
                    this->macros.push_back({space_parts[0], space_parts[1], f.name});
                    continue;
                }
                else if (space_parts[0] == "define")
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
                else if (space_parts[0] == "limit")
                {
                    _GX_limit_t limit;
                    limit.filename = f.name;
                    limit.special_register_name = space_parts[1];
                    const vector<string> range = split(space_parts[2], '~');
                    limit.min = stoll(range[0]);
                    limit.max = stoll(range[1]);
                    this->protection_limits.push_back(limit);
                }
                else if (space_parts[0] == "main")
                {
                    _GX_mainpoint_t mp = {f.name, filter(space_parts[0], ':')};
                    this->mainpoints.push_back(mp);
                }
            }
            else
            {
                cout << space_parts[0] << "\n";
                if (space_parts[0].find(":") != string::npos)
                {
                    cout << "label like line found\n";
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
        output.push_back(temp);
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

    for (_GX_file_t f : this->files)
    {
        const vector<string> lines = split(f.content, '\n');
        for (string l : lines)
        {
            l = trim(l);
            const vector<string> parts = split(l, ' ');
            const vector<string> commas = split(parts[1], ',');

            if (parts.size() < 2)
                continue;
            if (parts[0] == "mvfr")
            {
                if (!includes(reg_names, commas[0]))
                    regs.push_back({commas[1], "float"});
                if (!includes(reg_names, commas[1]))
                    regs.push_back({commas[1], "numeric"});
            }
            else if (parts[0] == "mvrf")
            {
                if (!includes(reg_names, commas[1]))
                    regs.push_back({commas[1], "float"});
                if (!includes(reg_names, commas[0]))
                    regs.push_back({commas[0], "numeric"});
            }
            else if (includes(reg_names, parts[1]))
                continue;
            else
            {
                reg_names.push_back(parts[1]);
                regs.push_back({parts[1], parts[0][0] == 'F' ? "float" : "numeric"});
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