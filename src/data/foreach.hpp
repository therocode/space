#pragma once

template <typename Functor, typename DataTable>
void forEach(Functor f, DataTable& table)
{
    ++table.meta.metrics[AccessType::Iteration];
    for(size_t i = 0; i < table.ids.size(); ++i)
    {
        f(table.ids[i], table.data[i]);
    }
}
