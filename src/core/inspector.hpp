#ifndef INSPECTOR_HPP
#define INSPECTOR_HPP
#include "types.hpp"

class _GX_INSPECTOR_
{

public:
    vector<_GX_file_t> files;
    vector<_GX_label_t> total_labels;
    vector<_GX_attachment_t> attachments;
    vector<_GX_limit_t> protection_limits;

    _GX_INSPECTOR_();
    _GX_INSPECTOR_(vector<_GX_file_t> files);
    void init(vector<_GX_file_t> files);
    void analyze();

    // conflicts.

    vector<_GX_file_t> mainpoint_conflicts();
    vector<_GX_label_t> label_conflicts();
    vector<_GX_limit_t> protection_limits_conflicts();
    vector<_GX_linter_ignored_t> linter_ignored_lines();
    vector<vector<string>> circular_includes();

    // infos.

    vector<_GX_register_t> registers();
    vector<_GX_file_t> module_files();
    _GX_label_t mainpoint_label();
};
#endif