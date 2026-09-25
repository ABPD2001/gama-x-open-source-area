#ifndef PACKETER_HPP
#define PACKETER_HPP
#include <string>
#include <vector>
#include <cmath>
#include <cinttypes>

using std::ceil;
using std::min;
using std::string;
using std::uint16_t;
using std::uint32_t;
using std::uint64_t;
using std::uint8_t;
using std::vector;

class _GX_PACKETER
{
private:
    uint32_t packet_size;

public:
    _GX_PACKETER();
    void config(uint32_t packet_size);
    void generate(vector<string> &output_packets, string content);
};
#endif