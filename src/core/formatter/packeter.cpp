#include "./packetr.hpp"

_GX_PACKETER::_GX_PACKETER() {}

void _GX_PACKETER::config(uint32_t packet_size, string output_name = "")
{
    this->packet_size = packet_size;
    this->output_name = output_name;
}

void _GX_PACKETER::generate(vector<string> &output_packets, string content)
{
    output_packets.clear();
    for (uint32_t i = 0; i < ceil(content.length() / this->packets_size); i++)
    {
        const uint32_t available_n = min(content.length() - i * this->packets_size, this->packets_size);
        output_packets.push(string(""));
        for (uint32_t j = 0; j < available_n; j++)
        {
            output_packets[i] = this->content.substr(i * this->packets_size - 1, available_n);
        }
    }
}