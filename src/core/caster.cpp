#include "./caster.hpp"

_GX_CASTER::_GX_CASTER() {};
void _GX_CASTER::config(_GX_CASTER_format_t format)
{
    this->format = format;
};
void _GX_CASTER::update(string content)
{
    if (this->format.binary)
    {
        vector<string> units = split(this->format.format, ',');
        uint32_t s = 0, n = content.size(), o = 0; // by bytes.
        for (string u : units)
        {
            s += to_uint32(u.substr(1)) / 8;
        }

        for (uint32_t i = 0; n >= s; i++)
        {
            vector<long long int> snipp;
            string snipp_raw = content.substr(i, s);
            for (string u : units)
            {
                const uint32_t s_i = to_uint32(u.substr(1)) / 8;
                snipp.push_back(to_uint32(snipp_raw.substr(o, s_i)));
                o += s_i;
            }
            this->snippets.push_back(snipp);
            o = 0;
            n -= s;
        }
    }
    else
    {
        const vector<string> lines = split(content, '\n');
        vector<long long int> snipp;
        if (this->format.seperator)
        {
            for (string l : lines)
            {
                const vector<string> parts = split(l, this->format.seperator);
                for (string p : parts)
                {
                    snipp.push_back(stoll(p));
                }
                this->snippets.push_back(snipp);
                snipp.clear();
            }
        }
        else
        {
            string sep;
            uint32_t n = 0;
            bool a = false;
            for (string l : lines)
            {
                sep = "";
                n = 0;
                a = false;
                for (uint32_t i = 0; i < l.length(); i++)
                {

                    if (l[i] == '>')
                    {
                        a = true;
                        continue;
                    }
                    if (l[i] == '<')
                    {
                        a = false;
                        continue;
                    }
                    if (a)
                    {
                        sep += l[i];
                        continue;
                    }
                    if (sep && !a)
                    {
                        const uint32_t idx = l.substr(n).find(sep);
                        snipp.push_back(stoll(l.substr(n, idx)));
                        n += idx + sep.length();
                        sep = "";
                    }
                }
                this->snippets.push_back(snipp);
                snipp.clear();
            }
        }
    }
}

string _GX_CASTER::cast_format(_GX_CASTER_format_t format)
{
}