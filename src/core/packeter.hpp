#include <string>
#include <vector>
#include <cmath>

using std::ceil;
using std::min;
using std::string;
using std::vector;

class _GX_CASTER_PACKETER
{
private:
    string output_name;
    uint32_t packet_size;

public:
    _GX_CASTER_PACKETER();
    void config(uint32_t packet_size, string output_name = "");
    void generate(vector<string> &output_packets, string content);
};