#include "./riff.hpp"

void write_initial_chunk(string filepath, bool append, bool &stat)
{
    fstream f;

    if (!fs::exists(filepath) && !append)
    {
        stat = false;
        return;
    }

    f.open(filepath, ios::out | ios::binary | ios::trunc);
    if (!f.is_open())
    {
        stat = false;
        return;
    }
    _GXPM_chunk_header_t header = {{'I', 'N', 'I', 'T'}, 0};
    f.write((char *)&header, sizeof(_GXPM_chunk_header_t));
    f.close();
    stat = true;
}

void write_repos_chunks(vector<_GXPM_repository_chunk_t> repos, string filepath, bool &stat)
{
    fstream f;
    _GXPM_chunk_header_t *headers;
    headers = new _GXPM_chunk_header_t[repos.size()];

    if (!fs::exists(filepath))
    {
        delete[] headers;
        stat = false;
        return;
    }

    f.open(filepath, ios::out | ios::binary | ios::trunc);
    if (!f.is_open())
    {
        delete[] headers;
        stat = false;
        return;
    }

    for (uint32_t i = 0; i < repos.size(); i++)
    {
        _GXPM_chunk_header_t header = {{'R', 'E', 'P', 'O'}, sizeof(_GXPM_repository_chunk_t)};
        headers[i] = header;
    }
    for (uint32_t i = 0; i < repos.size(); i++)
    {
        f.write((char *)&headers[i], sizeof(_GXPM_chunk_header_t));
        f.write((char *)&repos[i], sizeof(_GXPM_repository_chunk_t));
    }

    f.close();
    delete[] headers;
    stat = true;
}

void write_libs_chunks(vector<_GXPM_metadata_chunk_t> metadatas, vector<_GXPM_dependecies_t> dependecies, string filepath, bool &stat)
{
    fstream f;
    _GXPM_chunk_header_t *headers;
    headers = new _GXPM_chunk_header_t[metadatas.size()];

    if (!fs::exists(filepath))
    {
        delete[] headers;
        stat = false;
        return;
    }

    f.open(filepath, ios::out | ios::binary | ios::trunc);
    if (!f.is_open())
    {
        delete[] headers;
        stat = false;
        return;
    }

    for (uint32_t i = 0; i < metadatas.size(); i++)
    {
        _GXPM_chunk_header_t header = {{'M', 'E', 'T', 'A'}, sizeof(_GXPM_metadata_chunk_t)};
        headers[i] = header;
    }
    for (uint32_t i = 0; i < metadatas.size(); i++)
    {
        f.write((char *)&headers[i], sizeof(_GXPM_chunk_header_t));
        f.write((char *)&metadatas[i], sizeof(_GXPM_metadata_chunk_t));
    }

    delete[] headers;
    headers = new _GXPM_chunk_header_t[dependecies.size()];

    for (uint32_t i = 0; i < dependecies.size(); i++)
    {
        uint32_t size = dependecies[i].dependecies.size() + 8;
        for (string s : dependecies[i].dependecies)
        {
            size += s.length();
        }

        _GXPM_chunk_header_t header = {{'D', 'E', 'P', 'S'}, size};
        headers[i] = header;
    }
    for (uint32_t i = 0; i < dependecies.size(); i++)
    {
        char id[8]{};
        for (uint32_t j = 0; j < min(dependecies[i].id.length(), 8); j++)
        {
            id[j] = dependecies[i].id[j];
        }

        f.write((char *)&headers[i], sizeof(_GXPM_chunk_header_t));
        f.write((char *)&id, 8); // 8 bytes as id.

        for (uint32_t j = 0; j < dependecies[i].dependecies.size(); j++)
        {
            f.write((char *)dependecies[i].dependecies[j].c_str(), dependecies[i].dependecies[j].length());
            if (j != dependecies[i].dependecies.size() - 1)
                f.put(',');
        }

        f.put('\0');
    }
    f.close();
    delete[] headers;
    stat = true;
}

vector<_GXPM_repository_t> read_repositories_chunk(string filepath, bool &stat)
{
    vector<_GXPM_repository_t> output;
    fstream f;

    if (!fs::exists(filepath))
    {
        stat = false;
        return output;
    }
    f.open(filepath, ios::in | ios::binary | ios::ate);
    if (!f.is_open())
    {
        stat = false;
        return output;
    }

    _GXPM_chunk_header_t temp_header;
    _GXPM_repository_chunk_t temp_chunk;
    const uint32_t file_size = f.tellg();
    f.seekg(0);

    while (f.read((char *)&temp_header, sizeof(_GXPM_chunk_header_t)))
    {
        string id;
        for (uint32_t i = 0; i < 4; i++)
        {
            id += temp_header.id[i];
        }

        if (id == "INIT")
        {
            f.close();
            stat = true;
            return output;
        }
        if (id == "REPO")
        {
            f.read((char *)&temp_chunk, sizeof(_GXPM_repository_chunk_t));
            _GXPM_repository_t repo = {temp_chunk.description, temp_chunk.pathname, temp_chunk.name, temp_chunk.libraries_count};
            output.push_back(repo);
        }
        else
        {
            f.seekg(temp_header.size_b + f.tellg());
            continue;
        }
    }
    f.close();
    stat = true;

    return output;
}

vector<_GXPM_metadata_t> read_metadata_chunks(string filepath, bool &stat)
{
    vector<_GXPM_metadata_t> output;
    fstream f;

    if (!fs::exists(filepath))
    {
        stat = false;
        return output;
    }
    f.open(filepath, ios::in | ios::binary | ios::ate);
    if (!f.is_open())
    {
        stat = false;
        return output;
    }

    _GXPM_chunk_header_t temp_header;
    _GXPM_metadata_chunk_t temp_chunk;
    const uint32_t file_size = f.tellg();
    f.seekg(0);

    while (f.read((char *)&temp_header, sizeof(_GXPM_chunk_header_t)))
    {
        string id;
        for (uint32_t i = 0; i < 4; i++)
        {
            id += temp_header.id[i];
        }
        if (id == "INIT")
        {
            f.close();
            stat = true;
            return output;
        }
        if (id == "META")
        {
            f.read((char *)&temp_chunk, sizeof(_GXPM_metadata_chunk_t));

            _GXPM_metadata_t metadata = {temp_chunk.description, temp_chunk.mainpoint_relative, temp_chunk.pathname, temp_chunk.libname, temp_chunk.version, temp_chunk.dep_id};
            output.push_back(metadata);
        }
        else
        {
            f.seekg(temp_header.size_b + f.tellg());
            continue;
        }
    }
    f.close();
    stat = true;

    return output;
}

vector<_GXPM_dependecies_t> read_dependecies_chunks(string filepath, bool &stat)
{
    vector<_GXPM_dependecies_t> output;
    fstream f;

    if (!fs::exists(filepath))
    {
        stat = false;
        return output;
    }
    f.open(filepath, ios::in | ios::binary | ios::ate);
    if (!f.is_open())
    {
        stat = false;
        return output;
    }

    _GXPM_chunk_header_t temp_header;
    const uint32_t file_size = f.tellg();
    f.seekg(0);

    while (f.read((char *)&temp_header, sizeof(_GXPM_chunk_header_t)))
    {
        string id;
        for (uint32_t i = 0; i < 4; i++)
        {
            id += temp_header.id[i];
        }

        if (id == "INIT")
        {
            f.close();
            stat = true;
            return output;
        }
        if (id == "DEPS")
        {
            _GXPM_dependecies_t dependecies;
            string dependecies_txt;
            char id[8];
            char ch;

            for (uint32_t i = 0; i < 8; i++)
            {
                id[i] = dependecies.id[i];
            }

            f.read((char *)&id, 8); // read 8 bytes as id of dependecies.
            while (f.read(&ch, 1))
            {
                dependecies_txt += ch;
            }

            if (dependecies_txt.length())
                dependecies_txt = dependecies_txt.substr(0, dependecies_txt.length() - 1);
            dependecies.dependecies = split(dependecies_txt, ',');
        }
        else
        {
            f.seekg(temp_header.size_b + f.tellg());
            continue;
        }
    }
    f.close();
    stat = true;

    return output;
}

bool validate_chunks(string filepath, bool &stat)
{
    fstream f;
    char empties[7] = {'\r', '\n', ' ', '\t', '\a'};

    if (!fs::exists(filepath))
    {
        stat = false;
        return false;
    }
    f.open(filepath, ios::in | ios::binary | ios::ate);
    if (!f.is_open())
    {
        stat = false;
        return false;
    }

    _GXPM_chunk_header_t temp_header;
    const uint32_t file_size = f.tellg();
    f.seekg(0);

    while (f.read((char *)&temp_header, sizeof(_GXPM_chunk_header_t)))
    {
        if (string(temp_header.id) == "META")
        {
            _GXPM_metadata_chunk_t temp_chunk;
            if (!f.read((char *)&temp_chunk, sizeof(_GXPM_metadata_chunk_t)))
            {
                f.close();
                stat = true;
                return false;
            }
            if (!filter(temp_chunk.description, empties, 7).size() || !fs::exists(temp_chunk.mainpoint_relative) || !fs::exists(temp_chunk.pathname) || !fs::is_directory(temp_chunk.pathname) || !filter(temp_chunk.libname, empties, 7).size() || !is_valid_version(temp_chunk.version))
            {
                f.close();
                stat = true;
                return false;
            }
        }
        else if (string(temp_header.id) == "DEPS")
        {
            vector<string> deps;
            string text;
            string id;
            char dep_id[8];
            char ch;

            for (uint32_t i = 0; i < 8; i++)
            {
                dep_id[i] = id[i];
            }

            if (!f.read((char *)&dep_id, 8))
            {
                f.close();
                stat = true;
                return false;
            }
            if (!filter(string(id), empties, 7).length())
            {
                f.close();
                stat = true;
                return false;
            }
            while (f.read(&ch, 1))
            {
                if (!ch)
                    break;
                text += ch;
            }
            deps = split(text, ',');
            for (string d : deps)
            {
                if (!filter(d, empties, 7).length())
                {
                    f.close();
                    stat = true;
                    return false;
                }
            }
        }
        else if (string(temp_header.id) == "REPO")
        {
            _GXPM_repository_chunk_t temp_chunk;
            f.read((char *)&temp_chunk, sizeof(_GXPM_repository_chunk_t));
            stat = true;
            if (!valid_name(string(temp_chunk.name)))
                return false;
            if (!fs::exists(string(temp_chunk.pathname)))
                return false;
        }
        else if (string(temp_header.id) == "INIT")
        {
            if (temp_header.size_b)
            {
                stat = true;
                return false;
            }
            return true;
        }
        else
        {
            stat = true;
            return false;
        }
    }
    f.close();
    stat = true;

    return true;
}

_GXPM_metadata_chunk_t to_metadata_chunk(_GXPM_metadata_t metadata)
{
    _GXPM_metadata_chunk_t output{};

    for (uint32_t i = 0; i < min(metadata.description.length(), 152); i++)
    {
        output.description[i] = metadata.description[i];
    }
    for (uint32_t i = 0; i < 152 - min(metadata.description.length(), 151); i++)
    {
        output.description[metadata.description.length() + i] = 0;
    }

    for (uint32_t i = 0; i < min(metadata.mainpoint_relative.length(), 48); i++)
    {
        output.mainpoint_relative[i] = metadata.mainpoint_relative[i];
    }
    for (uint32_t i = 0; i < 49 - min(metadata.mainpoint_relative.length(), 48); i++)
    {
        output.mainpoint_relative[metadata.mainpoint_relative.length() + i] = 0;
    }

    for (uint32_t i = 0; i < min(metadata.libname.length(), 32); i++)
    {
        output.libname[i] = metadata.libname[i];
    }
    for (uint32_t i = 0; i < 33 - min(metadata.libname.length(), 32); i++)
    {
        output.libname[metadata.libname.length() + i] = 0;
    }

    for (uint32_t i = 0; i < min(metadata.version.length(), 12); i++)
    {
        output.version[i] = metadata.version[i];
    }
    for (uint32_t i = 0; i < 13 - min(metadata.version.length(), 12); i++)
    {
        output.version[metadata.version.length() + i] = 0;
    }

    for (uint32_t i = 0; i < min(metadata.pathname.length(), 1024); i++)
    {
        output.pathname[i] = metadata.pathname[i];
    }
    for (uint32_t i = 0; i < 1025 - min(metadata.pathname.length(), 1024); i++)
    {
        output.pathname[metadata.pathname.length() + i] = 0;
    }
    for (uint32_t i = 0; i < 8; i++)
    {
        output.dep_id[i] = metadata.dep_id[i];
    }

    return output;
}

_GXPM_repository_chunk_t to_repository_chunk(_GXPM_repository_t repo)
{
    _GXPM_repository_chunk_t output;

    for (uint32_t i = 0; i < min(repo.description.length(), 151); i++)
    {
        output.description[i] = repo.description[i];
    }
    for (uint32_t i = 0; i < 152 - min(repo.description.length(), 151); i++)
    {
        output.description[repo.description.length() + i] = 0;
    }

    for (uint32_t i = 0; i < min(repo.path.length(), 1024); i++)
    {
        output.pathname[i] = repo.path[i];
    }
    for (uint32_t i = 0; i < 1025 - min(repo.path.length(), 1024); i++)
    {
        output.pathname[repo.path.length() + i] = 0;
    }
    for (uint32_t i = 0; i < min(repo.name.length(), 31); i++)
    {
        output.name[i] = repo.name[i];
    }
    for (uint32_t i = 0; i < 32 - min(repo.name.length(), 31); i++)
    {
        output.name[repo.name.length() + i] = 0;
    }
    output.libraries_count = repo.libraries_count;

    return output;
}