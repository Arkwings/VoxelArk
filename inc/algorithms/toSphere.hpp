
template<typename T, int dimensions>
void ToSphere(std::vector<T>& triangle_coords) {
    std::vector<T> new_triangle_coords;
    glm::vec<dimensions, T> a;
    for (int i = 0; i < triangle_coords.size(); i += dimensions) {
        for (int dim = 0; dim < dimensions; ++dim)   a[dim] = triangle_coords[i + dim];
        a = glm::normalize(a);
        for (int dim = 0; dim < dimensions; ++dim)   new_triangle_coords.push_back(a[dim]);
    }
    triangle_coords = new_triangle_coords;
}