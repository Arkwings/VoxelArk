#pragma once

enum TEXTURE_TARGET {
	TEXTURE_1D = GL_TEXTURE_1D,
	TEXTURE_1D_ARRAY = GL_TEXTURE_1D_ARRAY,
	TEXTURE_2D = GL_TEXTURE_2D,
	TEXTURE_2D_ARRAY = GL_TEXTURE_2D_ARRAY,
	TEXTURE_2D_MULTISAMPLE = GL_TEXTURE_2D_MULTISAMPLE,
	TEXTURE_2D_MULTISAMPLE_ARRAY = GL_TEXTURE_2D_MULTISAMPLE_ARRAY,
	TEXTURE_3D = GL_TEXTURE_3D,
	TEXTURE_CUBE_MAP = GL_TEXTURE_CUBE_MAP,
	TEXTURE_CUBE_MAP_ARRAY = GL_TEXTURE_CUBE_MAP_ARRAY,
	TEXTURE_RECTANGLE = GL_TEXTURE_RECTANGLE
};

enum TEXTURE_PARAM_NAME {
	DEPTH_STENCIL_TEXTURE_MODE = GL_DEPTH_STENCIL_TEXTURE_MODE,
	TEXTURE_BASE_LEVEL = GL_TEXTURE_BASE_LEVEL,
	TEXTURE_COMPARE_FUNC = GL_TEXTURE_COMPARE_FUNC,
	TEXTURE_COMPARE_MODE = GL_TEXTURE_COMPARE_MODE,
	TEXTURE_LOD_BIAS = GL_TEXTURE_LOD_BIAS,
	TEXTURE_MIN_FILTER = GL_TEXTURE_MIN_FILTER,
	TEXTURE_MAG_FILTER = GL_TEXTURE_MAG_FILTER,
	TEXTURE_MIN_LOD = GL_TEXTURE_MIN_LOD,
	TEXTURE_MAX_LOD = GL_TEXTURE_MAX_LOD,
	TEXTURE_MAX_LEVEL = GL_TEXTURE_MAX_LEVEL,
	TEXTURE_SWIZZLE_R = GL_TEXTURE_SWIZZLE_R,
	TEXTURE_SWIZZLE_G = GL_TEXTURE_SWIZZLE_G,
	TEXTURE_SWIZZLE_B = GL_TEXTURE_SWIZZLE_B,
	TEXTURE_SWIZZLE_A = GL_TEXTURE_SWIZZLE_A,
	TEXTURE_WRAP_S = GL_TEXTURE_WRAP_S,
	TEXTURE_WRAP_T = GL_TEXTURE_WRAP_T,
	TEXTURE_WRAP_R = GL_TEXTURE_WRAP_R
};

enum TEXTURE_PARAM_VALUE {
	//DEPTH_STENCIL_TEXTURE_MODE
	DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
	//STENCIL_COMPONENT = GL_STENCIL_COMPONENT, //fails

	//TEXTURE_BASE_LEVEL
	BASE_LEVEL_0 = 0,	//no mipmap
	BASE_LEVEL_1 = 1,	//1 pixel for 2*2
	BASE_LEVEL_2 = 2,	//1 pixel for 4*4
	BASE_LEVEL_3 = 3,	//1 pixel for 8*8
	BASE_LEVEL_4 = 4,	//1 pixel for 16*16
	BASE_LEVEL_5 = 5,	//1 pixel for 32*32
	BASE_LEVEL_6 = 6,	//1 pixel for 64*64
	BASE_LEVEL_7 = 7,	//1 pixel for 128*128
	BASE_LEVEL_8 = 8,	//1 pixel for 256*256
	BASE_LEVEL_9 = 9,	//1 pixel for 512*512
	BASE_LEVEL_10 = 10,	//1 pixel for 1024*1024
	BASE_LEVEL_11 = 11,	//1 pixel for 2048*2048

	//TEXTURE_COMPARE_FUNC
	EQUAL = GL_EQUAL,
	NOTEQUAL = GL_NOTEQUAL,
	LEQUAL = GL_LEQUAL,
	GEQUAL = GL_GEQUAL,
	LESS = GL_LESS,
	GREATER = GL_GREATER,
	ALWAYS = GL_ALWAYS,
	NEVER = GL_NEVER,

	//TEXTURE_COMPARE_MODE
	COMPARE_REF_TO_TEXTURE = GL_COMPARE_REF_TO_TEXTURE,
	NONE = GL_NONE,

	//TEXTURE_LOD_BIAS
	BIAS_LEVEL_0 = 0, //add more if needed

	//TEXTURE_MIN_FILTER/MAG_FILTER
	NEAREST = GL_NEAREST,
	LINEAR = GL_LINEAR,
	NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
	LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR,
	NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
	LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,

	//TEXTURE_MIN_LOD/MAX_LOD
	MIN_LOD_0 = -1000,
	MAX_LOD_0 = 1000, //add more if needed

	//TEXTURE_MAX_LEVEL
	MAX_LEVEL_0 = 1000, //add more if needed

	//TEXTURE_SWIZZLE_R/G/B/A
	RED = GL_RED,
	GREEN = GL_GREEN,
	BLUE = GL_BLUE,
	ALPHA = GL_ALPHA,
	ZERO = GL_ZERO,
	ONE = GL_ONE,

	//TEXTURE_WRAP_S/T/R
	CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
	REPEAT = GL_REPEAT,
	MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
	CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
	MIRRORED_CLAMP_TO_EDGE = GL_MIRROR_CLAMP_TO_EDGE
};

enum TEXTURE_CHANNELS {
	RGB = GL_RGB,
	BGR = GL_BGR,
	RGBA = GL_RGBA,
	BGRA = GL_BGRA,
};

enum TEXTURE_ALGORITHMS {
	DIAMOND_SQUARE = 0,
	PERLIN_NOISE = 1,
	ARK_NOISE = 2
};

class Texture {
	public:
	template<typename T> void Attach2DTexture(const std::unordered_map<TEXTURE_PARAM_NAME, TEXTURE_PARAM_VALUE>& params, const TEXTURE_CHANNELS c, T* tex, const unsigned int& width, const unsigned int& height);
	template<typename T> void Attach2DArrayTexture(const std::unordered_map<TEXTURE_PARAM_NAME, TEXTURE_PARAM_VALUE>& params, const TEXTURE_CHANNELS c, const std::vector<T*>& tex, const unsigned int& width, const unsigned int& height);
	virtual void Bind(const unsigned int& channel, const unsigned int& location = 0);
	virtual void Unbind();
	virtual ~Texture();
	const unsigned int& GetLayers() const { return array_2D_layers_; }

	protected:
	Texture(const TEXTURE_TARGET t);

	GLuint texID_ = std::numeric_limits<GLuint>::max();
	TEXTURE_TARGET t_ = TEXTURE_2D;
	unsigned int array_2D_layers_ = 0;

	private:
	Texture() = delete;
};

class ImageTexture: public Texture {
	public:
	ImageTexture(const std::vector<std::string>& tex_names, const TEXTURE_TARGET t, const std::unordered_map<TEXTURE_PARAM_NAME, TEXTURE_PARAM_VALUE>& params, const TEXTURE_CHANNELS c);
	virtual ~ImageTexture();
	const std::vector<std::string>& getNames() const { return texture_names_; };

	private:
	std::string texture_path_ = "";
	std::vector<std::string> texture_names_;
};

class ProceduralTexture: public Texture {
	public:
	ProceduralTexture(const TEXTURE_ALGORITHMS algo, const std::vector<std::any>& algo_params, const TEXTURE_TARGET t, const std::unordered_map<TEXTURE_PARAM_NAME, TEXTURE_PARAM_VALUE>& params, const TEXTURE_CHANNELS c);
	virtual ~ProceduralTexture();

	private:
	std::vector<std::any> procedural_parameters_;
};

