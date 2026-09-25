#include "./snipper.hpp"

_GX_SNIPPER::_GX_SNIPPER() {};
void _GX_SNIPPER::config(struct _GX_SNIPPER_config_t config)
{
    this->config_st = config;
}
string _GX_SNIPPER::snippet_output(string content)
{
    string output = this->head + this->config_st.header_content_seperator + this->content + (this->config_st.footer_format.length() ? this->config_st.footer_content_seperator + this->footer : "");
    if (this->config_st.alignment && output.length() % this->config_st.alignment)
    {
        for (uint32_t i = 0; i < output.length() % this->config_st.alignment; i++)
        {
            output += this->config_st.padding_filler;
        }
    }
    return output;
}

void _GX_SNIPPER::hinclude(vector<_GX_SNIPPER_field_t> fields)
{
    this->head = this->config_st.header_format;
    for (_GX_SNIPPER_field_t f : fields)
    {
        this->head = replaceAll(this->head, string("<") + f.id + '>', f.value);
    }
}

void _GX_SNIPPER::hinclude_bin(vector<string> fields, uint32_t size)
{
    this->head = "";
    string buffer = "";
    const vector<string> bfields_format = split(this->config_st.header_format, ',');

    for (uint32_t i = 0; i < fields.size(); i++)
    {
        if (bfields_format[i] == "string")
        // if it was string (with null).
        {
            this->head += fields[i];
            this->head += 0;
        }

        else if (bfields_format[i] == "text")
            // if it was string.
            this->head += fields[i];

        else
        {
            const uint32_t s = to_uint32(bfields_format[i].substr(1)) / 8;
            if (fields[i] == "_size_")
                this->head += toBinary(size, s, this->config_st.binary_endianness);

            if (bfields_format[i][0] == 's')
                this->head += toBinary(stoll(fields[i]), s, this->config_st.binary_endianness);
            if (bfields_format[i][0] == 'u')
                this->head += toBinary(to_uint64(fields[i]), s, this->config_st.binary_endianness);
        }
    }
}

void _GX_SNIPPER::finclude(vector<_GX_SNIPPER_field_t> fields)
{

    if (!this->config_st.footer_format)
    {
        this->footer = "";
        return;
    }
    this->footer = this->config_st.footer_format;
    for (_GX_SNIPPER_field_t f : fields)
    {
        this->footer = replaceAll(this->footer, string("<") + f.id + '>', f.value);
    }
}

void _GX_SNIPPER::finclude_bin(vector<string> fields, uint32_t size)
{
    this->footer = "";
    if (!this->config_st.footer_format)
        return;
    string buffer = "";
    const vector<string> bfields_format = split(this->config_st.footerr_format, ',');

    for (uint32_t i = 0; i < fields.size(); i++)
    {
        if (bfields_format[i] == "string")
        // if it was string (with null).
        {
            this->footer += fields[i];
            this->footer += 0;
        }

        else if (bfields_format[i] == "text")
            // if it was string.
            this->footer += fields[i];

        else
        {
            const uint32_t s = to_uint32(bfields_format[i].substr(1)) / 8;
            if (fields[i] == "_size_")
                this->footer += toBinary(size, s, this->config_st.binary_endianness);
            if (bfields_format[i] == "u64")
                this->footer += toBinaryu64(to_uint64(fields[i]), s, this->config_st.binary_endianness);
            else
                this->footer += toBinary(stoll(fields[i]), s, this->config_st.binary_endianness);
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
string _GX_SNIPPER::binout(vector<string> &contents, vector<vector<string>> f_pointers, vector<vector<string>> h_pointers)
{
    string output = "";
    for (uint32_t i = 0; i < contents.size(); i++)
    {
        if (i < f_fields.size())
        {
            this->finclude_bin(f_pointers[i], contents.size());
        }
        if (i < h_fields.size())
            this->hinclude_bin(h_pointers[i], contents.size());
        output += this->snippet_output(contents[i]);
    }
    return output;
}

string _GX_SNIPPER::txtout_single(string content, vector<_GX_SNIPPER_field_t> f_fields, vector<_GX_SNIPPER_field_t> h_fields)
{
    chrono::system_clock::time_point chrono_now = chrono::system_clock::now();
    chrono::system_clock::duration chrono_duration = chrono_now.time_since_epoch();
    const chrono::system_clock::duration unix_ms = chrono::duration_cast<chrono::milliseconds>(chrono_duration).count();
    vector<_GX_SNIPPER_field_t> default_fields = {{"IDX", "0"}, {"SIZE", to_string(content.size())}, {"UNIX_MS", to_string(unix_ms)}};

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

    return this->snippet_output(content);
}

string _GX_SNIPPER::binout_single(string content, vector<string> f_pointers, vector<string> h_pointers)
{
    if (f_pointers.size())
        this->finclude(f_pointers, content.size());

    if (h_pointers.size())
        this->hinclude(h_pointers, content.size());

    return this->snippet_output(content);
}