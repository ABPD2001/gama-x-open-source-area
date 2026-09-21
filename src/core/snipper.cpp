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
    char *buffer = 0;
    const vector<string> bfields_format = split(this->config_st.header_format, ',');

    for (uint32_t i = 0; i < fields.size(); i++)
    {

        if (bfields_format[i][0] == 's')
        {                                                                  // if it was string.
            string *val = (string *)fields[i];                             // cast pointer.
            this->head.insert(this->head.end(), val->begin(), val->end()); // merge at the end.
        }
        else
        {
            const uint32_t s = to_uint32(bfields_format[i].substr(1));
            buffer = new char[s];
            for (uint32_t i = 0; i < s; i++)
            {
                buffer[i] = ((char *)fields[i])[i];
            }
            this->head += buffer;
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
    char *buffer = 0;
    const vector<string> bfields_format = split(this->config_st.header_format, ',');

    for (uint32_t i = 0; i < fields.size(); i++)
    {

        if (bfields_format[i][0] == 's')
        {                                                                      // if it was string.
            string *val = (string *)fields[i];                                 // cast pointer.
            this->footer.insert(this->footer.end(), val->begin(), val->end()); // merge at the end.
        }
        else
        {
            const uint32_t s = to_uint32(bfields_format[i].substr(1));
            buffer = new char[s];
            for (uint32_t i = 0; i < s; i++)
            {
                buffer[i] = ((char *)fields[i])[i];
            }
            this->footer += buffer;
        }
    }
}

string _GX_SNIPPER::txtout(vector<string> &contents, vector<vector<_GX_SNIPPER_field_t>> f_fields, vector<vector<_GX_SNIPPER_field_t>> h_fields)
{
    string output = "";
    for (uint32_t i = 0; i < contents.size(); i++)
    {
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
            this->finclude_bin(f_pointers[i]);
        if (i < h_fields.size())
            this->hinclude_bin(h_pointers[i]);
        output += this->snippet_output(contents[i]);
    }
    return output;
}