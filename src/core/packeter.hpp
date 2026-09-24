#ifndef PACKETER_HPP
#define PACKETER_HPP
#include <string>
#include <vector>
#include <cmath>

using std::ceil;
using std::min;
using std::string;
using std::vector;

class _GX_PACKETER
{
private:
    string output_name;
    uint32_t packet_size;

public:
    _GX_PACKETER();
    void config(uint32_t packet_size, string output_name = "");
    void generate(vector<string> &output_packets, string content);
};
#endif