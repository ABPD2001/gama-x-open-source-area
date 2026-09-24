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
    chrono::system_clock::time_point chrono_now = chrono::system_clock::now();
    chrono::system_clock::duration chrono_duration = chrono_now.time_since_epoch();
    const chrono::system_clock::duration unix_ms = chrono::duration_cast<chrono::milliseconds>(chrono_duration).count();

    string output = "";
    for (uint32_t i = 0; i < contents.size(); i++)
    {
        vector<_GX_SNIPPER_field_t> default_fields = {{"IDX", String(i)}, {"SIZE", to_string(contents[i].size())}, {"UNIX_MS", to_string(unix_ms)}};
        if (i < f_fields.size())
        {
            f_fields.insert(f_fields.end(), default_fields.begin(), default_fields.end());
            this->finclude(f_fields[i]);
        }
        if (i < h_fields.size())
        {
            h_fields.insert(h_fields.end(), default_fields.begin(), default_fields.end());
            this->hinclude(h_fields[i]);
        }
        output += this->snippet_output(contents[i]) + this->config_st.snippet_seperator;
    }
    return output.substr(0, output.length() - 1);
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
    uint32_t total_size = 0;

    for (string c : contents)
    {
        total_size += c.size();
    }

    chrono::system_clock::time_point chrono_now = chrono::system_clock::now();
    chrono::system_clock::duration chrono_duration = chrono_now.time_since_epoch();
    const chrono::system_clock::duration unix_ms = chrono::duration_cast<chrono::milliseconds>(chrono_duration).count();
    vector<_GX_SNIPPER_field_t> default_fields = {{"IDX", "0"}, {"SIZE", to_string(total_size)}, {"UNIX_MS", to_string(unix_ms)}};

    if (f_fields.size())
    {
        f_fields.insert(f_fields.end(), default_fields.begin(), default_fields.end());
        this->finclude(f_fields);
    }

    if (h_fields.size())
    {
        h_fields.insert(h_fields.end(), default_fields.begin(), default_fields.end());
        this->hinclude(h_fields);
    }

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