#pragma once

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
		: datas_(std::vector<Vertex3D>())
		, ID_(-1)
		, indexed_(false) {}
	virtual ~DataComponent() {}

	const std::vector<Vertex3D>& GetData() const {
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
	std::vector<Vertex3D> datas_;
	std::vector<unsigned int> indexes_;
	int ID_;
	bool indexed_;
};

class SquareDataComponent: public DataComponent {
	public:
	SquareDataComponent() = delete;
	SquareDataComponent(const int& subdivisions = 0);
};

class CubeDataComponent: public DataComponent {
	public:
	CubeDataComponent() = delete;
	CubeDataComponent(const int& subdivisions = 0, const bool& sphere = false);
};

class CircleDataComponent: public DataComponent {
	public:
	CircleDataComponent() = delete;
	CircleDataComponent(const unsigned int divisions, const int& subdivisions = 0);
};

class SphereDataComponent: public DataComponent {
	public:
	SphereDataComponent() = delete;
	SphereDataComponent(const unsigned int divisions_width, const unsigned int divisions_height, const int& subdivisions = 0, const bool displacement = false);
};

class Pointer2DDataComponent: public DataComponent {
	public:
	Pointer2DDataComponent();
};