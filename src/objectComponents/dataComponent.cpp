#include "common.hpp"
#include "algorithms/flatDataToIndex.hpp"
#include "algorithms/subdivision.hpp"
#include "algorithms/toSphere.hpp"
#include "objectComponents/dataComponent.hpp"

SquareDataComponent::SquareDataComponent(const int& subdivisions) {
	ID_ = OBJ3D::SQUARE;
	indexed_ = true;

	std::vector<GLfloat> vertices({
		-1, 0, -1,   -1, 0, 1,   1, 0, -1,
		1, 0, -1,   -1, 0, 1,   1, 0, 1
		});

	std::vector<GLfloat> tex_coords({
		0.0f, 0.0f,   0.0f, 1.0f,   1.0f, 0.0f,
		1.0f, 0.0f,   0.0f, 1.0f,   1.0f, 1.0f
		});

	std::vector<GLfloat> normals({
		0, 1, 0,   0, 1, 0,   0, 1, 0,
		0, 1, 0,   0, 1, 0,   0, 1, 0
		});


	const unsigned int size = vertices.size() / 3;
	datas_.resize(size);
	for (unsigned int i = 0; i < size; ++i) {
		datas_[i].pos = glm::vec3(vertices[i * 3], vertices[i * 3 + 1], vertices[i * 3 + 2]);
		datas_[i].tex = glm::vec2(tex_coords[i * 2], tex_coords[i * 2 + 1]);
		datas_[i].norm = glm::vec3(normals[i * 3], normals[i * 3 + 1], normals[i * 3 + 2]);
	}

	indexes_ = FlatDataToIndex<float, 3, true>(datas_);
}

CubeDataComponent::CubeDataComponent(const int& subdivisions, const bool& sphere) {
	ID_ = OBJ3D::CUBE;
	indexed_ = true;

	std::vector<GLfloat> vertices({
		//haut
		-0.5, 0.5, -0.5,   -0.5, 0.5, 0.5,   0.5, 0.5, -0.5,
		0.5, 0.5, -0.5,   -0.5, 0.5, 0.5,   0.5, 0.5, 0.5,
		//bas
		-0.5, -0.5, -0.5,   0.5, -0.5, -0.5,   -0.5, -0.5, 0.5,
		0.5, -0.5, -0.5,   0.5, -0.5, 0.5,   -0.5, -0.5, 0.5,
		//arriere
		-0.5, -0.5, 0.5,   0.5, -0.5, 0.5,   -0.5, 0.5, 0.5,
		0.5, -0.5, 0.5,   0.5, 0.5, 0.5,   -0.5, 0.5, 0.5,
		//avant
		-0.5, -0.5, -0.5,   -0.5, 0.5, -0.5,   0.5, -0.5, -0.5,
		0.5, -0.5, -0.5,   -0.5, 0.5, -0.5,   0.5, 0.5, -0.5,
		//droite
		-0.5, -0.5, -0.5,   -0.5, -0.5, 0.5,   -0.5, 0.5, -0.5,
		-0.5, 0.5, -0.5,   -0.5, -0.5, 0.5,   -0.5, 0.5, 0.5,
		//gauche
		0.5, -0.5, -0.5,   0.5, 0.5, -0.5,   0.5, -0.5, 0.5,
		0.5, 0.5, -0.5,   0.5, 0.5, 0.5,   0.5, -0.5, 0.5
		});

	std::vector<GLfloat> tex_coords({
			//haut
			0.0f, 0.0f,   0.0f, 1.0f,   1.0f, 0.0f,
			1.0f, 0.0f,   0.0f, 1.0f,   1.0f, 1.0f,
			//bas
			0.0f, 0.0f,   1.0f, 0.0f,   0.0f, 1.0f,
			1.0f, 0.0f,   1.0f, 1.0f,   0.0f, 1.0f,
			//arriere
			0.0f, 1.0f,   1.0f, 1.0f,   0.0f, 0.0f,
			1.0f, 1.0f,   1.0f, 0.0f,   0.0f, 0.0f,
			//avant
			1.0f, 1.0f,   1.0f, 0.0f,   0.0f, 1.0f,
			0.0f, 1.0f,   1.0f, 0.0f,   0.0f, 0.0f,
			//droite
			0.0f, 1.0f,   1.0f, 1.0f,   0.0f, 0.0f,
			0.0f, 0.0f,   1.0f, 1.0f,   1.0f, 0.0f,
			//gauche
			1.0f, 1.0f,   1.0f, 0.0f,   0.0f, 1.0f,
			1.0f, 0.0f,   0.0f, 0.0f,   0.0f, 1.0f
		});

	std::vector<GLfloat> normals({
			//haut
			0, 1, 0,   0, 1, 0,   0, 1, 0,
			0, 1, 0,   0, 1, 0,   0, 1, 0,
			//bas
			0, -1, 0,   0, -1, 0,   0, -1, 0,
			0, -1, 0,   0, -1, 0,   0, -1, 0,
			//arriere
			0, 0, 1,   0, 0, 1,   0, 0, 1,
			0, 0, 1,   0, 0, 1,   0, 0, 1,
			//avant
			0, 0, -1,   0, 0, -1,   0, 0, -1,
			0, 0, -1,   0, 0, -1,   0, 0, -1,
			//droite
			-1, 0, 0,   -1, 0, 0,   -1, 0, 0,
			-1, 0, 0,   -1, 0, 0,   -1, 0, 0,
			//gauche
			1, 0, 0,   1, 0, 0,   1, 0, 0,
			1, 0, 0,   1, 0, 0,   1, 0, 0
		});

	for (int i = 0; i < subdivisions; ++i) {
		Subdivide<GLfloat, 3>(vertices);
		Subdivide<GLfloat, 2>(tex_coords);
		Subdivide<GLfloat, 3>(normals);
	}

	if (sphere)	ToSphere<GLfloat, 3>(vertices);

	const unsigned int size = vertices.size() / 3;
	datas_.resize(size);
	for (unsigned int i = 0; i < size; ++i) {
		datas_[i].pos = glm::vec3(vertices[i * 3], vertices[i * 3 + 1], vertices[i * 3 + 2]);
		datas_[i].tex = glm::vec2(tex_coords[i * 2], tex_coords[i * 2 + 1]);
		datas_[i].norm = glm::vec3(normals[i * 3], normals[i * 3 + 1], normals[i * 3 + 2]);
	}

	indexes_ = FlatDataToIndex<float, 3, false>(datas_);
}

CircleDataComponent::CircleDataComponent(const unsigned int divisions, const int& subdivisions) {
	ID_ = OBJ3D::CIRCLE;
	indexed_ = true;

	if (divisions < 3) std::cerr << "circle with " << divisions << "divisions can't be made" << std::endl;
	else {
		float x = cos(0);
		float y = sin(0);

		std::vector<GLfloat> vertices;
		std::vector<GLfloat> tex_coords;
		std::vector<GLfloat> normals;

		float old_x, old_y;
		float angle = glm::radians(360 / float(divisions));

		for (unsigned int division = 1; division < divisions + 1; ++division) {
			old_x = x;
			old_y = y;

			x = cos(angle * division);
			y = sin(angle * division);

			vertices.push_back(old_x);
			vertices.push_back(0);
			vertices.push_back(old_y);

			vertices.push_back(0);
			vertices.push_back(0);
			vertices.push_back(0);

			vertices.push_back(x);
			vertices.push_back(0);
			vertices.push_back(y);

			tex_coords.push_back(old_x);
			tex_coords.push_back(old_y);

			tex_coords.push_back(0);
			tex_coords.push_back(0);

			tex_coords.push_back(x);
			tex_coords.push_back(y);

			normals.push_back(0);
			normals.push_back(1);
			normals.push_back(0);

			normals.push_back(0);
			normals.push_back(1);
			normals.push_back(0);

			normals.push_back(0);
			normals.push_back(1);
			normals.push_back(0);
		}

		const unsigned int size = vertices.size() / 3;
		datas_.resize(size);
		for (unsigned int i = 0; i < size; ++i) {
			datas_[i].pos = glm::vec3(vertices[i * 3], vertices[i * 3 + 1], vertices[i * 3 + 2]);
			datas_[i].tex = glm::vec2(tex_coords[i * 2], tex_coords[i * 2 + 1]);
			datas_[i].norm = glm::vec3(normals[i * 3], normals[i * 3 + 1], normals[i * 3 + 2]);
		}
	}

	indexes_ = FlatDataToIndex<float, 3, true>(datas_);
}

SphereDataComponent::SphereDataComponent(const unsigned int divisions_width, const unsigned int divisions_height, const int& subdivisions, const bool displacement) {
	ID_ = OBJ3D::SPHERE;
	indexed_ = true;

	if (divisions_width < 3) std::cerr << "sphere with " << divisions_width << "width divisions can't be made" << std::endl;
	else if (divisions_height < 2) std::cerr << "sphere with " << divisions_height << "height divisions can't be made" << std::endl;
	else {
		std::vector<GLfloat> points;
		std::vector<GLfloat> tex_points;

		std::vector<GLfloat> vertices;
		std::vector<GLfloat> tex_coords;
		std::vector<GLfloat> normals;

		float xpos, ypos, zpos;

		float angle_h = MATH::M_PI / float(divisions_height);
		for (int y = 1; y < divisions_height; ++y) {
			ypos = cos(angle_h);

			float angle_w = MATH::M_PI * 2 / float(divisions_width);
			for (int x = 0; x < divisions_width + 1; ++x) {  //redrawing first raw for simplicity
				zpos = sin(angle_h) * cos(angle_w);
				xpos = sin(angle_w) * sin(angle_h);

				points.push_back(xpos);
				points.push_back(ypos);
				points.push_back(zpos);

				// tex_points.push_back(sin(angle_w) / (M_PI * 2));
				// tex_points.push_back(ypos / M_PI);
				tex_points.push_back(static_cast<GLfloat>(x) / divisions_width);
				tex_points.push_back(static_cast<GLfloat>(y) / divisions_height);

				angle_w += MATH::M_PI_X_2 / float(divisions_width);
			}

			angle_h += MATH::M_PI / float(divisions_height);
		}

		for (int x = 0; x < divisions_width; ++x) {
			//triangle 1 top
			vertices.push_back(points[(x + 1) * 3]);
			vertices.push_back(points[(x + 1) * 3 + 1]);
			vertices.push_back(points[(x + 1) * 3 + 2]);

			vertices.push_back(0);
			vertices.push_back(1);
			vertices.push_back(0);

			vertices.push_back(points[x * 3]);
			vertices.push_back(points[x * 3 + 1]);
			vertices.push_back(points[x * 3 + 2]);

			tex_coords.push_back(tex_points[(x + 1) * 2]);
			tex_coords.push_back(tex_points[(x + 1) * 2 + 1]);

			tex_coords.push_back((tex_points[(x + 1) * 2] + tex_points[x * 2]) / 2);
			tex_coords.push_back(0);

			tex_coords.push_back(tex_points[x * 2]);
			tex_coords.push_back(tex_points[x * 2 + 1]);

			normals.push_back(points[(x + 1) * 3]);
			normals.push_back(points[(x + 1) * 3 + 1]);
			normals.push_back(points[(x + 1) * 3 + 2]);

			normals.push_back(0);
			normals.push_back(1);
			normals.push_back(0);

			normals.push_back(points[x * 3]);
			normals.push_back(points[x * 3 + 1]);
			normals.push_back(points[x * 3 + 2]);
		}

		for (int y = 0; y < divisions_height - 2; ++y) {
			for (int x = 0; x < divisions_width; ++x) {
				//triangle 1
				vertices.push_back(points[(y * (divisions_width + 1) + x + 1) * 3]);
				vertices.push_back(points[(y * (divisions_width + 1) + x + 1) * 3 + 1]);
				vertices.push_back(points[(y * (divisions_width + 1) + x + 1) * 3 + 2]);

				vertices.push_back(points[(y * (divisions_width + 1) + x) * 3]);
				vertices.push_back(points[(y * (divisions_width + 1) + x) * 3 + 1]);
				vertices.push_back(points[(y * (divisions_width + 1) + x) * 3 + 2]);

				vertices.push_back(points[((y + 1) * (divisions_width + 1) + x) * 3]);
				vertices.push_back(points[((y + 1) * (divisions_width + 1) + x) * 3 + 1]);
				vertices.push_back(points[((y + 1) * (divisions_width + 1) + x) * 3 + 2]);

				tex_coords.push_back(tex_points[(y * (divisions_width + 1) + x + 1) * 2]);
				tex_coords.push_back(tex_points[(y * (divisions_width + 1) + x + 1) * 2 + 1]);

				tex_coords.push_back(tex_points[(y * (divisions_width + 1) + x) * 2]);
				tex_coords.push_back(tex_points[(y * (divisions_width + 1) + x) * 2 + 1]);

				tex_coords.push_back(tex_points[((y + 1) * (divisions_width + 1) + x) * 2]);
				tex_coords.push_back(tex_points[((y + 1) * (divisions_width + 1) + x) * 2 + 1]);

				normals.push_back(points[(y * (divisions_width + 1) + x + 1) * 3]);
				normals.push_back(points[(y * (divisions_width + 1) + x + 1) * 3 + 1]);
				normals.push_back(points[(y * (divisions_width + 1) + x + 1) * 3 + 2]);

				normals.push_back(points[(y * (divisions_width + 1) + x) * 3]);
				normals.push_back(points[(y * (divisions_width + 1) + x) * 3 + 1]);
				normals.push_back(points[(y * (divisions_width + 1) + x) * 3 + 2]);

				normals.push_back(points[((y + 1) * (divisions_width + 1) + x) * 3]);
				normals.push_back(points[((y + 1) * (divisions_width + 1) + x) * 3 + 1]);
				normals.push_back(points[((y + 1) * (divisions_width + 1) + x) * 3 + 2]);

				//triangle 2
				vertices.push_back(points[(y * (divisions_width + 1) + x + 1) * 3]);
				vertices.push_back(points[(y * (divisions_width + 1) + x + 1) * 3 + 1]);
				vertices.push_back(points[(y * (divisions_width + 1) + x + 1) * 3 + 2]);

				vertices.push_back(points[((y + 1) * (divisions_width + 1) + x) * 3]);
				vertices.push_back(points[((y + 1) * (divisions_width + 1) + x) * 3 + 1]);
				vertices.push_back(points[((y + 1) * (divisions_width + 1) + x) * 3 + 2]);

				vertices.push_back(points[((y + 1) * (divisions_width + 1) + x + 1) * 3]);
				vertices.push_back(points[((y + 1) * (divisions_width + 1) + x + 1) * 3 + 1]);
				vertices.push_back(points[((y + 1) * (divisions_width + 1) + x + 1) * 3 + 2]);

				tex_coords.push_back(tex_points[(y * (divisions_width + 1) + x + 1) * 2]);
				tex_coords.push_back(tex_points[(y * (divisions_width + 1) + x + 1) * 2 + 1]);

				tex_coords.push_back(tex_points[((y + 1) * (divisions_width + 1) + x) * 2]);
				tex_coords.push_back(tex_points[((y + 1) * (divisions_width + 1) + x) * 2 + 1]);

				tex_coords.push_back(tex_points[((y + 1) * (divisions_width + 1) + x + 1) * 2]);
				tex_coords.push_back(tex_points[((y + 1) * (divisions_width + 1) + x + 1) * 2 + 1]);

				normals.push_back(points[(y * (divisions_width + 1) + x + 1) * 3]);
				normals.push_back(points[(y * (divisions_width + 1) + x + 1) * 3 + 1]);
				normals.push_back(points[(y * (divisions_width + 1) + x + 1) * 3 + 2]);

				normals.push_back(points[((y + 1) * (divisions_width + 1) + x) * 3]);
				normals.push_back(points[((y + 1) * (divisions_width + 1) + x) * 3 + 1]);
				normals.push_back(points[((y + 1) * (divisions_width + 1) + x) * 3 + 2]);

				normals.push_back(points[((y + 1) * (divisions_width + 1) + x + 1) * 3]);
				normals.push_back(points[((y + 1) * (divisions_width + 1) + x + 1) * 3 + 1]);
				normals.push_back(points[((y + 1) * (divisions_width + 1) + x + 1) * 3 + 2]);
			}
		}

		for (int x = 0; x < divisions_width; ++x) {
			//triangle 2 bottom
			vertices.push_back(points[((divisions_height - 2) * (divisions_width + 1) + x) * 3]);
			vertices.push_back(points[((divisions_height - 2) * (divisions_width + 1) + x) * 3 + 1]);
			vertices.push_back(points[((divisions_height - 2) * (divisions_width + 1) + x) * 3 + 2]);

			vertices.push_back(0);
			vertices.push_back(-1);
			vertices.push_back(0);

			vertices.push_back(points[((divisions_height - 2) * (divisions_width + 1) + x + 1) * 3]);
			vertices.push_back(points[((divisions_height - 2) * (divisions_width + 1) + x + 1) * 3 + 1]);
			vertices.push_back(points[((divisions_height - 2) * (divisions_width + 1) + x + 1) * 3 + 2]);

			tex_coords.push_back(tex_points[((divisions_height - 2) * (divisions_width + 1) + x) * 2]);
			tex_coords.push_back(tex_points[((divisions_height - 2) * (divisions_width + 1) + x) * 2 + 1]);

			tex_coords.push_back((tex_points[((divisions_height - 2) * (divisions_width + 1) + x) * 2] + tex_points[((divisions_height - 2) * (divisions_width + 1) + x + 1) * 2]) / 2);
			tex_coords.push_back(1);

			tex_coords.push_back(tex_points[((divisions_height - 2) * (divisions_width + 1) + x + 1) * 2]);
			tex_coords.push_back(tex_points[((divisions_height - 2) * (divisions_width + 1) + x + 1) * 2 + 1]);

			normals.push_back(points[((divisions_height - 2) * (divisions_width + 1) + x) * 3]);
			normals.push_back(points[((divisions_height - 2) * (divisions_width + 1) + x) * 3 + 1]);
			normals.push_back(points[((divisions_height - 2) * (divisions_width + 1) + x) * 3 + 2]);

			normals.push_back(0);
			normals.push_back(0);
			normals.push_back(0);

			normals.push_back(points[((divisions_height - 2) * (divisions_width + 1) + x + 1) * 3]);
			normals.push_back(points[((divisions_height - 2) * (divisions_width + 1) + x + 1) * 3 + 1]);
			normals.push_back(points[((divisions_height - 2) * (divisions_width + 1) + x + 1) * 3 + 2]);
		}

		const unsigned int size = vertices.size() / 3;
		datas_.resize(size);
		for (unsigned int i = 0; i < size; ++i) {
			datas_[i].pos = glm::vec3(vertices[i * 3], vertices[i * 3 + 1], vertices[i * 3 + 2]);
			datas_[i].tex = glm::vec2(tex_coords[i * 2], tex_coords[i * 2 + 1]);
			datas_[i].norm = glm::vec3(normals[i * 3], normals[i * 3 + 1], normals[i * 3 + 2]);
		}
	}

	indexes_ = FlatDataToIndex<float, 3, true>(datas_);
}

Pointer2DDataComponent::Pointer2DDataComponent() {
	ID_ = OBJ2D::POINTER;
	indexed_ = true;

	std::vector<GLfloat> vertices({
			//gauche
			-0.03f, -0.002f,  -0.002f, -0.002f,  -0.03f, 0.002f,
			-0.03f, 0.002f,   -0.002f, -0.002f,  -0.002f, 0.002f,
			//droite
			0.03f, -0.002f,   0.03f, 0.002f,     0.002f, -0.002f,
			0.03f, 0.002f,    0.002f, 0.002f,    0.002f, -0.002f,
			//haut
			-0.002f, 0.03f,   -0.002f, 0.002f,   0.002f, 0.03f,
			0.002f, 0.03f,    -0.002f, 0.002f,   0.002f, 0.002f,
			//bas
			-0.002f, -0.03f,  0.002f, -0.03f,    -0.002f, -0.002f,
			0.002f, -0.03f,   0.002f, -0.002f,   -0.002f, -0.002f
		});

	std::vector<GLfloat> tex_coords({
			//gauche
			0.0f, 0.4f,   0.4f, 0.4f,   0.0f, 0.6f,
			0.0f, 0.6f,   0.4f, 0.4f,  	0.4f, 0.6f,
			//droite
			1.0f, 0.4f,   1.0f, 0.6f,   0.6f, 0.4f,
			1.0f, 0.6f,   0.6f, 0.6f,   0.6f, 0.4f,
			//haut
			0.4f, 1.0f,   0.4f, 0.6f,   0.6f, 1.0f,
			0.6f, 1.0f,   0.4f, 0.6f,  	0.6f, 0.6f,
			//bas
			0.4f, 0.0f,   0.6f, 0.0f,   0.4f, 0.4f,
			0.6f, 0.0f,   0.6f, 0.4f,   0.4f, 0.4f
		});

	const unsigned int size = vertices.size() / 2;
	datas_.resize(size);
	for (unsigned int i = 0; i < size; ++i) {
		datas_[i].pos = glm::vec2(vertices[i * 2], vertices[i * 2 + 1]);
		datas_[i].tex = glm::vec2(tex_coords[i * 2], tex_coords[i * 2 + 1]);
	}

	indexes_ = FlatDataToIndex<float, 2, true>(datas_);
}
