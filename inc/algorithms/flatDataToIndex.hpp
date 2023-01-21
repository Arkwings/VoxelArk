#pragma once

template<typename T, bool flattenNormals>
std::vector<unsigned int> FlatDataToIndex(std::vector<Vertex3D>& vertices) {
    std::vector <unsigned int> indexes;
    bool found;
    for (unsigned int i = 0; i < vertices.size();) {
        found = false;
        for (unsigned int index = 0; index < indexes.size(); ++index) {
            if ((vertices[i].pos_x == vertices[indexes[index]].pos_x && vertices[i].pos_y == vertices[indexes[index]].pos_y && vertices[i].pos_z == vertices[indexes[index]].pos_z)
                && (vertices[i].tex_x == vertices[indexes[index]].tex_x && vertices[i].tex_y == vertices[indexes[index]].tex_y)) {
                if (flattenNormals || (vertices[i].norm_z == vertices[indexes[index]].norm_z && vertices[i].norm_y == vertices[indexes[index]].norm_y && vertices[i].norm_x == vertices[indexes[index]].norm_x)) {
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