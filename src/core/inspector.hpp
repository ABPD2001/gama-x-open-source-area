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
    string mainpoint_label();
    vector<_GX_label_t> conflicts();
    vector<_GX_register_t> registers();
    vector<_GX_linter_ignored_t> linter_ignored_lines();
};
#endif