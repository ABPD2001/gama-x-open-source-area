#ifndef INSPECTOR_HPP
#define INSPECTOR_HPP
#include "types.hpp"
#include "../utils/string.hpp"
#include "../utils/vector.hpp"
#include "../utils/number.hpp"

class _GX_INSPECTOR_
{
public:
    vector<_GX_file_t> files;
    vector<_GX_label_t> total_labels;
    vector<_GX_attachment_t> attachments;
    vector<_GX_limit_t> protection_limits;
    vector<_GX_define_t> marco_instructions;
    vector<_GX_marco_t> macros;
    vector<_GX_mainpoint_t> mainpoints;

    _GX_INSPECTOR_();
    _GX_INSPECTOR_(vector<_GX_file_t> files);
    void init(vector<_GX_file_t> files);
    void analyze();

    // conflicts.

    vector<_GX_label_t> mainpoint_conflicts();
    uint32_t mainpoint_verified();
    vector<vector<_GX_label_t>> label_conflicts();
    vector<_GX_linter_ignored_t> linter_ignored_lines();
    vector<vector<_GX_limit_t>> protection_limits_conflicts();
    vector<string> circular_includes(uint32_t idx = 0);
    vector<vector<_GX_marco_t>> macro_conflicts();
    vector<vector<_GX_define_t>> macro_instruction_conflicts();

    // infos.

    vector<_GX_register_t> registers();
    vector<string> module_files();
    _GX_label_t mainpoint_label();
};
#endif