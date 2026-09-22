#include "./caster.hpp"

_GX_CASTER::_GX_CASTER() {};
void _GX_CASTER::config(string content, _GX_CASTER_format_t format)
{
    this->format = format;
    this->content = content;
};
string _GX_CASTER::cast_format(_GX_CASTER_format_t format)
{
    vector<vector<uint64_t>> snippets;

    // first, read snippets.

    if (this->format.binary)
    {
        vector<string> units = split(this->format.format, ',');
        uint32_t s = 0, n = this->content.size(), o = 0; // by bytes.
        for (string u : units)
        {
            s += to_uint32(u.substr(1)) / 8;
        }

        for (uint32_t i = 0; n >= s; i++)
        {
            vector<uint64_t> snipp;
            string snipp_raw = this->content.substr(i, s);
            for (string u : units)
            {
                const uint32_t s_i = to_uint32(u.substr(1)) / 8;
                snipp.push_back(to_uint64(snipp_raw.substr(o, s_i)));
                o += s_i;
            }
            snippets.push_back(snipp);
            o = 0;
            n -= s;
        }
    }
}