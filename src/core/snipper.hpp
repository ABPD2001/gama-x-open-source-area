#include <string>
#include <vector>
#include <utils/string.hpp>
#include <utils/number.hpp>

using std::string;
using std::to_string;
using std::vector;

struct _GX_SNIPPER_config_t
{
    string header_format;
    string footer_format;
    string header_content_seperator = "\n";
    string footer_content_seperator = "\n";
    bool header_binary = false;
    bool footer_binary = false;
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
    string content;
    string head = "";
    string footer = "";
    struct _GX_SNIPPER_config_t config_st;

public:
    _GX_SNIPPER();
    void config(struct _GX_SNIPPER_config_t config);
    string snippet_output(string content);
    string txtout(vector<string> &contents, vector<vector<_GX_SNIPPER_field_t>> f_fields, vector<vector<_GX_SNIPPER_field_t>> h_fields);
    string binout(vector<string> &contents, vector<void *> f_fields, vector<void *> h_fields);

    string txtout_single(vector<string> &contents, vector<_GX_SNIPPER_field_t> f_fields, vector<_GX_SNIPPER_field_t> h_fields);
    string binout_single(vector<string> &contents, vector<_GX_SNIPPER_field_t> f_fields, vector<_GX_SNIPPER_field_t> h_fields);

    void hinclude_bin(vector<void *> fields);
    void hinclude(vector<_GX_SNIPPER_field_t> fields);

    void finclude_bin(vector<void *> fields);
    void finclude(vector<_GX_SNIPPER_field_t> fields);
};