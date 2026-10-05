#ifndef TYPES_HPP
#define TYPES_HPP
#include <string>
#include <cstdint>
#include <vector>

using std::string;
using std::uint64_t;
using std::uintmax_t;
using std::vector;

struct _GX_label_t
{
    string filename;
    string text;
    string name;
    uintmax_t line_idx;
};

struct _GX_register_t
{
    string filename;
    string name;
    string type;
};

struct _GX_linter_ignored_t
{
    string filename;
    uintmax_t line_idx;
};

struct _GX_limit_t
{
    string filename;
    string special_register_name;
    uint64_t min;
    uint64_t max;
};

struct _GX_marco_t
{
    string filename;
    string from;
    string to;
};

struct _GX_attachment_t
{
    string type;
    string filename;
    string callername;
};

struct _GX_define_argument_t
{
    string value;
    string name;
    string type;
};

struct _GX_mainpoint_t
{
    string filename;
    string name;
};

struct _GX_define_t
{
    string filename;
    string text;
    vector<_GX_define_argument_t> arguments;
    string name;
};

struct _GX_file_t
{
    string content;
    string name;
};
#endif