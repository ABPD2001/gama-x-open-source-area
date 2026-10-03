#ifndef INSPECTOR_HPP
#define INSPECTOR_HPP
#include "types.hpp"
#include "../utils/string.hpp"
#include "../utils/vector.hpp"
#include "../utils/number.hpp"

class _GX_INSPECTOR_
{
private:
public:
    vector<_GX_file_t> files;
    vector<_GX_label_t> total_labels;
    vector<_GX_attachment_t> attachments;
    vector<_GX_limit_t> protection_limits;
    vector<_GX_define_t> marco_instructions;
    vector<_GX_marco_t> marcos;
    vector<string> mainpoints;

    _GX_INSPECTOR_();
    _GX_INSPECTOR_(vector<_GX_file_t> files);
    void init(vector<_GX_file_t> files);
    void analyze();

    // conflicts.

    vector<_GX_file_t> mainpoint_conflicts();
    uint32_t mainpoint_verified();
    vector<_GX_label_t> label_conflicts();
    vector<_GX_linter_ignored_t> linter_ignored_lines();
    vector<_GX_limit_t> protection_limits_conflicts();
    vector<string> circular_includes(uint32_t idx = 0);
    vector<_GX_marco_t> macro_conflicts();

    // infos.

    vector<_GX_register_t> registers();
    vector<string> module_files();
    _GX_label_t mainpoint_label();
};

inline uint32_t counts(vector<_GX_label_t> vec, _GX_label_t element)
{
    uint32_t output = 0;
    for (uint32_t i = 0; i < vec.size(); i++)
    {
        if (element.name == vec[i].name)
            output++;
    }
    return output;
}
inline uint32_t counts(vector<_GX_limit_t> vec, _GX_limit_t element)
{
    uint32_t output = 0;
    for (uint32_t i = 0; i < vec.size(); i++)
    {
        if (element.special_register_name == vec[i].special_register_name)
            output++;
    }
    return output;
}
inline uint32_t counts(vector<_GX_register_t> vec, string element)
{
    uint32_t output = 0;
    for (uint32_t i = 0; i < vec.size(); i++)
    {
        if (element == vec[i].name)
            output++;
    }
    return output;
}
inline uint32_t counts(vector<_GX_marco_t> vec, string element)
{
    uint32_t output = 0;
    for (uint32_t i = 0; i < vec.size(); i++)
    {
        if (element == vec[i].from)
            output++;
    }
    return output;
}
#endif