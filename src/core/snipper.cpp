#include "./snipper.hpp"

_GX_SNIPPER::_GX_SNIPPER() {};
void _GX_SNIPPER::config(struct _GX_SNIPPER_config_t config)
{
    this->config_st = config;
}
string _GX_SNIPPER::snippet_output(string content)
{
    this->content = content;
    return this->head + this->config_st.header_content_seperator + this->content + this->config_st.footer_content_seperator + this->footer;
}

void _GX_SNIPPER::hinclude(vector<_GX_SNIPPER_field_t> fields)
{
    this->head = this->config_st.header_format;
    for (_GX_SNIPPER_field_t f : fields)
    {
        this->head = replaceAll(this->head, string("<") + f.id + '>', f.value);
    }
}

void _GX_SNIPPER::hinclude_bin(vector<void *> fields)
{
    this->head = "";
    string buffer = "";
    const vector<string> bfields_format = split(this->config_st.header_format, ',');

    for (uint32_t i = 0; i < fields.size(); i++)
    {
        char *val = fields + i; // cast pointer.
        if (bfields_format[i][0] == 's')
        // if it was string.
        {
            for (uint32_t j = 0; val[j]; j++)
            {
                this->head += val[j];
            }
        }
        else
        {
            const uint32_t s = to_uint32(bfields_format[i].substr(1));
            this->head += toBinary(to_uint64(val), s, this->config_st.binary_endianness);
        }
    }
}

void _GX_SNIPPER::finclude(vector<_GX_SNIPPER_field_t> fields)
{
    this->footer = this->config_st.footer_format;
    for (_GX_SNIPPER_field_t f : fields)
    {
        this->footer = replaceAll(this->footer, string("<") + f.id + '>', f.value);
    }
}

void _GX_SNIPPER::finclude_bin(vector<void *> fields)
{
    this->footer = "";
    string buffer = "";
    const vector<string> bfields_format = split(this->config_st.footerr_format, ',');

    for (uint32_t i = 0; i < fields.size(); i++)
    {
        char *val = fields + i; // cast pointer.
        if (bfields_format[i][0] == 's')
        // if it was string.
        {
            for (uint32_t j = 0; val[j]; j++)
            {
                this->footer += val[j];
            }
        }
        else
        {
            const uint32_t s = to_uint32(bfields_format[i].substr(1));
            this->footer += toBinary(to_uint64(val), s, this->config_st.binary_endianness);
        }
    }
}

string _GX_SNIPPER::txtout(vector<string> &contents, vector<vector<_GX_SNIPPER_field_t>> f_fields, vector<vector<_GX_SNIPPER_field_t>> h_fields)
{
    string output = "";
    for (uint32_t i = 0; i < contents.size(); i++)
    {
        vector<_GX_SNIPPER_field_t> default_hfields = {{"IDX", String(i)}, {"SIZE", to_string(h_fields[i].size())}};
        vector<_GX_SNIPPER_field_t> default_ffields = {{"IDX", String(i)}, {"SIZE", to_string(f_fields[i].size())}};

        if (i < f_fields.size())
            this->finclude(f_fields[i]);
        if (i < h_fields.size())
            this->hinclude(h_fields[i]);

        output += this->snippet_output(contents[i]);
    }
    return output;
}
string _GX_SNIPPER::binout(vector<string> &contents, vector<vector<void *>> f_pointers, vector<vector<void *>> h_pointers)
{
    string output = "";
    for (uint32_t i = 0; i < contents.size(); i++)
    {
        if (i < f_fields.size())
        {
            this->finclude_bin(f_pointers[i]);
        }
        if (i < h_fields.size())
            this->hinclude_bin(h_pointers[i]);
        output += this->snippet_output(contents[i]);
    }
    return output;
}

string _GX_SNIPPER::txtout_single(vector<string> &contents, vector<_GX_SNIPPER_field_t> f_fields, vector<_GX_SNIPPER_field_t> h_fields)
{
    if (f_fields.size())
        this->finclude(f_fields);

    if (h_fields.size())
        this->hinclude(h_fields);

    return this->snippet_output(contents);
}

string _GX_SNIPPER::binout_single(vector<string> &contents, vector<void *> f_pointers, vector<void *> h_pointers)
{
    if (f_pointers.size())
        this->finclude(f_pointers);

    if (h_pointers.size())
        this->hinclude(h_pointers);

    return this->snippet_output(contents);
}