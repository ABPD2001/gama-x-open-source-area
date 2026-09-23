#include <string>
#include <vector>
#include <utils/string.hpp>
#include <utils/number.hpp>

using std::stoll;
using std::string;
using std::vector;

struct _GX_CASTER_format_t
{
    string format; // or binary logic.
    string seperator;
    bool binary = false;
    bool endianness = false; // small
};

class _GX_CASTER
{
private:
    vector<long long int> snippets;
    string content;
    _GX_CASTER_format_t format;

public:
    _GX_CASTER();
    void config(_GX_CASTER_format_t format);
    void update(string content);
    string cast_format(_GX_CASTER_format_t format);
};