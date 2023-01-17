#pragma once

template <unsigned int Dim>
class DataComponent {
	public:
	enum CHANNEL {
		POSITIONS = 0,
		TEX_COORDS = 1,
		NORMALS = 2,
		TANGENTS = 3,
		BITANGENTS = 4
	};

	DataComponent()
		: datas_(std::vector<Vertex<Dim>>())
		, ID_(-1)
		, indexed_(false) {}
	virtual ~DataComponent() {}

	const bool HasData(const CHANNEL c) const {
		if (datas_.find(c) == datas_.end())	return false;
		return true;
	}
	void SetData(const CHANNEL c, std::vector<GLfloat>& data, GLuint nb_pack) {
		datas_.insert(std::pair<CHANNEL, std::pair<GLuint, std::vector<GLfloat>>>(c, { nb_pack, data }));
	}

	const std::vector<Vertex<Dim>>& GetData() const {
		return datas_;
	}

	const std::vector<unsigned int>& GetIndexes() const {
		return indexes_;
	}

	const bool& GetIndexed() const {
		return indexed_;
	}

	const int& GetID() const { return ID_; };

	protected:
	std::vector<Vertex<Dim>> datas_;
	std::vector<unsigned int> indexes_;
	int ID_;
	bool indexed_;
};

class SquareDataComponent: public DataComponent<3> {
	public:
	SquareDataComponent() = delete;
	SquareDataComponent(const int& subdivisions = 0);
};

class CubeDataComponent: public DataComponent<3> {
	public:
	CubeDataComponent() = delete;
	CubeDataComponent(const int& subdivisions = 0, const bool& sphere = false);
};

class CircleDataComponent: public DataComponent<3> {
	public:
	CircleDataComponent() = delete;
	CircleDataComponent(const unsigned int divisions, const int& subdivisions = 0);
};

class SphereDataComponent: public DataComponent<3> {
	public:
	SphereDataComponent() = delete;
	SphereDataComponent(const unsigned int divisions_width, const unsigned int divisions_height, const int& subdivisions = 0, const bool displacement = false);
};

class Pointer2DDataComponent: public DataComponent<2> {
	public:
	Pointer2DDataComponent();
};