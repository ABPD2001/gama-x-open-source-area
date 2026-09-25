#ifndef CASTER_HPP
#define CASTER_HPP
#include <string>
#include <vector>
#include <cinttypes>
#include "../utils/string.hpp"
#include "../utils/number.hpp"

using std::stoll;
using std::string;
using std::to_string;
using std::uint16_t;
using std::uint32_t;
using std::uint64_t;
using std::uint8_t;
using std::vector;

struct _GX_CASTER_format_t
{
    string format; // or binary logic.
    char seperator;
    bool binary = false;
    bool endianness = false; // small
};

class _GX_CASTER
{
private:
    vector<vector<uint64_t>> snippets;
    string content;
    _GX_CASTER_format_t format;

public:
    _GX_CASTER();
    void config(_GX_CASTER_format_t format);
    void update(string content);
    string cast_format(_GX_CASTER_format_t format);
};
#endif