#pragma once

template<typename T, int Dim, bool flattenNormals>
std::vector<unsigned int> FlatDataToIndex(std::vector<Vertex<Dim>>& vertices) {
    std::vector <unsigned int> indexes;
    bool found;
    for (unsigned int i = 0; i < vertices.size();) {
        found = false;
        for (unsigned int index = 0; index < indexes.size(); ++index) {
            if (vertices[i].pos == vertices[indexes[index]].pos && vertices[i].tex == vertices[indexes[index]].tex) {
                if (flattenNormals || vertices[i].norm == vertices[indexes[index]].norm) {
                    found = true;
                    indexes.push_back(indexes[index]);
                    vertices.erase(vertices.begin() + i);
                    break;
                }
            }
            if (found) break;
        }
        if (!found) {
            indexes.push_back(i);
            ++i;
        }
    }

    return indexes;
}