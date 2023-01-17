
template<typename T, int dimensions>
void Subdivide(std::vector<T>& triangle_coords) {
    std::vector<T> new_triangle_coords;
    glm::vec<dimensions, T> a, b, c;
    for (int i = 0; i < triangle_coords.size(); i += dimensions * 3) {
        for (int dim = 0; dim < dimensions; ++dim) {
            a[dim] = (triangle_coords[i + dim] + triangle_coords[i + dimensions + dim]) / 2;
            b[dim] = (triangle_coords[i + dim] + triangle_coords[i + dimensions * 2 + dim]) / 2;
            c[dim] = (triangle_coords[i + dimensions + dim] + triangle_coords[i + dimensions * 2 + dim]) / 2;
        }

        //triangle 1
        for (int dim = 0; dim < dimensions; ++dim) new_triangle_coords.push_back(a[dim]);
        for (int dim = 0; dim < dimensions; ++dim) new_triangle_coords.push_back(b[dim]);
        for (int dim = 0; dim < dimensions; ++dim) new_triangle_coords.push_back(triangle_coords[i + dim]);

        //triangle 2
        for (int dim = 0; dim < dimensions; ++dim) new_triangle_coords.push_back(a[dim]);
        for (int dim = 0; dim < dimensions; ++dim) new_triangle_coords.push_back(triangle_coords[i + dimensions + dim]);
        for (int dim = 0; dim < dimensions; ++dim) new_triangle_coords.push_back(c[dim]);

        //triangle 3
        for (int dim = 0; dim < dimensions; ++dim) new_triangle_coords.push_back(b[dim]);
        for (int dim = 0; dim < dimensions; ++dim) new_triangle_coords.push_back(c[dim]);
        for (int dim = 0; dim < dimensions; ++dim) new_triangle_coords.push_back(triangle_coords[i + dimensions * 2 + dim]);

        //triangle 4
        for (int dim = 0; dim < dimensions; ++dim) new_triangle_coords.push_back(a[dim]);
        for (int dim = 0; dim < dimensions; ++dim) new_triangle_coords.push_back(c[dim]);
        for (int dim = 0; dim < dimensions; ++dim) new_triangle_coords.push_back(b[dim]);
    }
    triangle_coords = new_triangle_coords;
}