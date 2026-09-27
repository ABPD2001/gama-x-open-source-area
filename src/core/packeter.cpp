#include "./packeter.hpp"

_GX_PACKETER::_GX_PACKETER() {}

void _GX_PACKETER::config(uint32_t packet_size)
{
    this->packet_size = packet_size;
}

void _GX_PACKETER::generate(vector<string> &output_packets, string content)
{
    output_packets.clear();
    for (uint32_t i = 0; i < ceil(content.length() / this->packet_size); i++)
    {
        const uint32_t available_n = min((uint32_t)(content.length() - i * this->packet_size), this->packet_size);
        output_packets.push_back(string(""));

        output_packets[i] = content.substr(max(0, (int)(i * this->packet_size - 1)), available_n);
    }
}