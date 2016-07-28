#pragma once

template <typename DataTable, typename Functor>
void forEach(DataTable& table, Functor f)
{
    for(size_t i = 0; i < table.ids.size(); ++i)
    {
        f(table.ids[i], table.data[i]);
    }
}
