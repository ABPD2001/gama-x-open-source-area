#include <string>
#include <vector>
#include <utils/string.hpp>
#include <utils/number.hpp>

using std::string;
using std::vector;

struct _GX_CASTER_format_t
{
    string format; // or binary logic.
    bool binary = false;
    bool endianness = false; // small
};

struct _GX_CASTER_snippet_txt_t
{
    string regname;
    string value;
};
class _GX_CASTER
{
private:
    string content;
    _GX_CASTER_format_t format;

public:
    _GX_CASTER();
    void config(string content, _GX_CASTER_format_t format);
    string cast_format(_GX_CASTER_format_t format);
};