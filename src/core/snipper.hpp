#ifndef SNIPPER_HPP
#define SNIPPER_HPP
#include <string>
#include <vector>
#include <chrono>
#include <cinttypes>
#include "../utils/string.hpp"
#include "../utils/number.hpp"

namespace chrono = std::chrono;

using std::stoll;
using std::string;
using std::to_string;
using std::uint16_t;
using std::uint32_t;
using std::uint64_t;
using std::uint8_t;
using std::vector;

struct _GX_SNIPPER_config_t
{
    string header_format;
    string footer_format = "";
    string header_content_seperator = "\n";
    string footer_content_seperator = "\n";
    string snippet_seperator = "\r";
    uint32_t alignment = 0;
    char padding_filler = 0;
    bool binary_endianness = false; // Small-Endian
};

struct _GX_SNIPPER_field_t
{
    string id;
    string value;
};

class _GX_SNIPPER
{
private:
    string head = "";
    string footer = "";
    struct _GX_SNIPPER_config_t config_st;

public:
    _GX_SNIPPER();
    void config(struct _GX_SNIPPER_config_t config);
    string snippet_output(string content);
    string txtout(vector<string> &contents, vector<vector<_GX_SNIPPER_field_t>> f_fields, vector<vector<_GX_SNIPPER_field_t>> h_fields);
    string binout(vector<string> &contents, vector<vector<string>> f_fields, vector<vector<string>> h_fields);

    string txtout_single(string content, vector<_GX_SNIPPER_field_t> f_fields, vector<_GX_SNIPPER_field_t> h_fields);
    string binout_single(string content, vector<string> f_fields, vector<string> h_fields);

    void hinclude_bin(vector<string> fields, uint32_t size);
    void hinclude(vector<_GX_SNIPPER_field_t> fields);

    void finclude_bin(vector<string> fields, uint32_t size);
    void finclude(vector<_GX_SNIPPER_field_t> fields);
};
#endif