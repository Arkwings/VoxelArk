#include "common.hpp"
#include "algorithms/diamondSquare.hpp"
#include "algorithms/perlin.hpp"
#include "algorithms/arkGen.hpp"
#include "texture.hpp"

Texture::Texture(const TEXTURE_TARGET t)
	: t_(t) {}

Texture::~Texture() {
	glDeleteTextures(1, &texID_);
}

void Texture::Bind(const unsigned int& channel, const unsigned int& location) {
	if (t_ == TEXTURE_2D_ARRAY) glUniform1i(location, array_2D_layers_);
	if (texID_ != std::numeric_limits<GLuint>::max()) {
		glActiveTexture(GL_TEXTURE0 + channel);
		glBindTexture(t_, texID_);
	}
}

void Texture::Unbind() {
	glBindTexture(t_, 0);
}

//IMAGE//

ImageTexture::ImageTexture(const std::vector<std::string>& tex_names, const TEXTURE_TARGET t, const std::unordered_map<TEXTURE_PARAM_NAME, TEXTURE_PARAM_VALUE>& params, const TEXTURE_CHANNELS c)
	: Texture(t)
	, texture_path_("")
	, texture_names_(tex_names) {
#ifdef DEBUG
	texture_path_ = __FILE__;
	texture_path_ = texture_path_.substr(0, texture_path_.rfind("\\") + 1) + "..\\extern\\tex\\";
	DEBUG_NAME_PRINT("texture_path", texture_path_);
	DEBUG_NAME_PRINT("texture_names", tex_names);
#endif
	//loading 
	std::vector<FIBITMAP*> dibs;
	std::vector<GLubyte*> texs;
	unsigned int max_width(0), max_height(0);

	GLint max_tex_size, max_tex_max_anisotropy;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_tex_size);
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_tex_max_anisotropy);

	std::vector<std::string> tex_names_cp(tex_names);
	if (tex_names_cp.empty())	tex_names_cp.push_back("test.jpg");

	for (unsigned int i = 0; i < tex_names_cp.size(); ++i) {
		std::string full_path = texture_path_ + tex_names_cp[i];
		FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(full_path.c_str());
		if (fif == FIF_UNKNOWN) { fif = FreeImage_GetFIFFromFilename(full_path.c_str()); }
		if (fif == FIF_UNKNOWN) { std::cerr << "Texture: unknown fif" << std::endl; return; }

		FIBITMAP* dib = nullptr;
		if (FreeImage_FIFSupportsReading(fif)) { dib = FreeImage_Load(fif, full_path.c_str()); }
		if (dib == nullptr) { std::cerr << "Texture: empty dib" << std::endl; return; }

		unsigned int width = FreeImage_GetWidth(dib);
		unsigned int height = FreeImage_GetHeight(dib);
		if (width > max_tex_size) {
			width = max_tex_size;
			FIBITMAP* rdib = FreeImage_Rescale(dib, width, height, FILTER_BICUBIC);
			FreeImage_Unload(dib);
			dib = rdib;
		}
		if (height > max_tex_size) {
			height = max_tex_size;
			FIBITMAP* rdib = FreeImage_Rescale(dib, width, height, FILTER_BICUBIC);
			FreeImage_Unload(dib);
			dib = rdib;
		}
		if (width == 0 || height == 0) { std::cerr << "Texture: width / height == 0" << std::endl; return; }
		if (width > max_width) { if (max_width == 0) { max_width = width; } else { std::cerr << "Texture: width mismatch" << std::endl; return; } }
		if (height > max_height) { if (max_height == 0) { max_height = height; } else { std::cerr << "Texture: height mismatch" << std::endl; return; } }

		unsigned int BPP = FreeImage_GetBPP(dib);
		if (BPP == 0) { std::cerr << "Texture: BPP extraction failed" << std::endl; return; }
		if ((c == BGRA || c == RGBA) && BPP != 32) {
			FIBITMAP* rdib = FreeImage_ConvertTo32Bits(dib);
			FreeImage_Unload(dib);
			dib = rdib;
			BPP = FreeImage_GetBPP(dib);
			DEBUG_NAME_PRINT("format modified", BPP);
		}
		else if ((c == BGR || c == RGB) && BPP != 24) {
			FIBITMAP* rdib = FreeImage_ConvertTo24Bits(dib);
			FreeImage_Unload(dib);
			dib = rdib;
			BPP = FreeImage_GetBPP(dib);
			DEBUG_NAME_PRINT("format modified", BPP);
		}
		DEBUG_NAME_PRINT("format: ", BPP);
		if (!FreeImage_FlipVertical(dib)) { std::cerr << "Texture: Image can't be flipped" << std::endl; return; }

		GLubyte* tex = FreeImage_GetBits(dib);
		if (tex == nullptr) { std::cerr << "Texture: empty tex" << std::endl; return; }

		texs.push_back(tex);

		if (t == TEXTURE_2D) { Attach2DTexture(params, c, texs[0], width, height); break; }
	}

	if (t == TEXTURE_2D_ARRAY) {
		array_2D_layers_ = texs.size();
		Attach2DArrayTexture(params, c, texs, max_width, max_height);
	}
}

ImageTexture::~ImageTexture() {}

//PROCEDURAL//

ProceduralTexture::ProceduralTexture(const TEXTURE_TARGET t, const std::unordered_map<TEXTURE_PARAM_NAME, TEXTURE_PARAM_VALUE>& params, const TEXTURE_CHANNELS c, const TEXTURE_ALGORITHMS algo, const float linear_factor, const float quadratic_factor, const int width, const int height, const unsigned int seed, const bool seamless_x, const bool seamless_y, const std::vector<float>& chan_mins, const std::vector<float>& chan_maxs, const std::vector<bool>& range_attenuation)
	: Texture(t) {
	GLfloat* tex = nullptr;
	int size = (width + height) / 2;

	glBindTexture(t_, texID_);

	//quicker to load, longer to draw
	if (c == RGB || c == BGR) {
		if (algo == DIAMOND_SQUARE)	tex = Generation::DiamondSquare<GLfloat>(size, chan_mins, chan_maxs, range_attenuation, linear_factor, seed, 3, seamless_x, seamless_y);
		if (algo == PERLIN_NOISE)	tex = Generation::NoiseMap<GLfloat, 2, Generation::PERLIN_OCTAVE_NOISE_CLAMP_REMAP>(glm::vec2(size, size), seed, 8, 0.6f, 4.0f);
		if (algo == ARK_NOISE) {
			Generation::ArkNoise<GLfloat>* testNoise = new Generation::ArkNoise<GLfloat>(width, height, 4.0f);
			testNoise->GenerateGradients(glm::vec<4, unsigned int>(1U, 1U, 1U, 1U), glm::vec<4, unsigned int>(5U, 2U, 3U, 4U), 7U);
			testNoise->GeneratePoints(true, 1, 1, 1);
			tex = testNoise->GetPoints();
			delete testNoise;
		}
	}
	else if (c == RGBA || c == BGRA) {
		//longer to load, quicker to draw
		if (algo == DIAMOND_SQUARE)	tex = Generation::DiamondSquare<GLfloat>(size, chan_mins, chan_maxs, range_attenuation, linear_factor, seed, 4, seamless_x, seamless_y);
	}

	Attach2DTexture(params, c, tex, width, height);

	if (tex)	delete[] tex;
}

ProceduralTexture::~ProceduralTexture() {}

ArkNoiseProceduralTexture::ArkNoiseProceduralTexture(const TEXTURE_TARGET t, const std::unordered_map<TEXTURE_PARAM_NAME, TEXTURE_PARAM_VALUE>& params, const TEXTURE_CHANNELS c, const unsigned int& width, const unsigned int& height, const GLfloat& frequency, const glm::vec<4, unsigned int>& verticesSeeds, const glm::vec<4, unsigned int>& edgesSeeds, const unsigned int& innerSeed)
	: Texture(t) {
	GLfloat* tex = nullptr;

	Generation::ArkNoise<GLfloat>* testNoise = new Generation::ArkNoise<GLfloat>(width, height, frequency);
	testNoise->GenerateGradients(verticesSeeds, edgesSeeds, innerSeed);
	testNoise->GeneratePoints(true, 1, 1, 1);
	tex = testNoise->GetPoints();

	Attach2DTexture(params, c, tex, width, height);

	delete testNoise;
	if (tex)	delete[] tex;
}

ArkNoiseProceduralTexture::~ArkNoiseProceduralTexture() {}


void Texture::Attach2DTexture(const std::unordered_map<TEXTURE_PARAM_NAME, TEXTURE_PARAM_VALUE>& params, const TEXTURE_CHANNELS c, GLfloat* tex, const unsigned int& width, const unsigned int& height) {
	glGenTextures(1, &texID_);
	glBindTexture(t_, texID_);

	glPixelStorei(GL_UNPACK_ROW_LENGTH, width);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D(t_, 0, (c == RGBA || c == BGRA) ? RGBA : RGB, width, height, 0, c, GL_FLOAT, tex);

	for (auto param : params)	glTexParameteri(t_, param.first, param.second);
	if (params.find(TEXTURE_MAX_LEVEL) != params.end())	glGenerateMipmap(t_);

	glBindTexture(t_, 0);
}
void Texture::Attach2DTexture(const std::unordered_map<TEXTURE_PARAM_NAME, TEXTURE_PARAM_VALUE>& params, const TEXTURE_CHANNELS c, unsigned char* tex, const unsigned int& width, const unsigned int& height) {
	glGenTextures(1, &texID_);
	glBindTexture(t_, texID_);

	glPixelStorei(GL_UNPACK_ROW_LENGTH, width);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D(t_, 0, (c == RGBA || c == BGRA) ? GL_RGBA32F : GL_RGB32F, width, height, 0, c, GL_UNSIGNED_BYTE, tex);

	for (auto param : params)	glTexParameteri(t_, param.first, param.second);
	if (params.find(TEXTURE_MAX_LEVEL) != params.end())	glGenerateMipmap(t_);

	glBindTexture(t_, 0);
}

void Texture::Attach2DArrayTexture(const std::unordered_map<TEXTURE_PARAM_NAME, TEXTURE_PARAM_VALUE>& params, const TEXTURE_CHANNELS c, const std::vector<GLubyte*>& tex, const unsigned int& width, const unsigned int& height) {

	glGenTextures(1, &texID_);
	glBindTexture(GL_TEXTURE_2D_ARRAY, texID_);

	bool mipmap(params.find(TEXTURE_MAX_LEVEL) != params.end());

	glPixelStorei(GL_UNPACK_ROW_LENGTH, width);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexStorage3D(
		GL_TEXTURE_2D_ARRAY,									//type of texture
		mipmap ? params.find(TEXTURE_MAX_LEVEL)->second : 1, 	//mipmap level
		(c == RGBA || c == BGRA) ? GL_RGBA8 : GL_RGB8, 			//internal format
		width, 													//image width
		height, 												//image height
		tex.size());														//number of layers

	for (unsigned int i = 0U; i < tex.size(); ++i) {
		glTexSubImage3D(
			GL_TEXTURE_2D_ARRAY, 	//type
			0, 						//mimap number
			0, 0, i,				//xoffset, yoffset, zoffset
			width, height,  		//width, height, 
			1,						//layer
			c, 						//format
			GL_UNSIGNED_BYTE, 		//type
			tex[i]);				//data pointer
	}

	for (auto param : params)	glTexParameteri(t_, param.first, param.second);
	if (mipmap)	    glGenerateMipmap(t_);

	glBindTexture(t_, 0);
}

BlockTexture::BlockTexture(const std::vector<std::string>& tex_names, const std::unordered_map<TEXTURE_PARAM_NAME, TEXTURE_PARAM_VALUE>& params, const TEXTURE_CHANNELS& c)
	: Texture(TEXTURE_2D_ARRAY) {
	std::string tex_path;
#ifdef DEBUG
	tex_path = __FILE__;
	tex_path = tex_path.substr(0, tex_path.rfind("\\") + 1) + "..\\extern\\tex\\";
	DEBUG_NAME_PRINT("texture_path", tex_path);
	DEBUG_NAME_PRINT("texture_names", tex_names);
#endif
	//loading 
	std::vector<FIBITMAP*> dibs;
	std::vector<GLubyte*> texs;
	unsigned int max_width(0), max_height(0);

	GLint max_tex_size, max_tex_max_anisotropy;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_tex_size);
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_tex_max_anisotropy);

	std::vector<std::string> tex_names_cp(tex_names);
	if (tex_names_cp.empty())	tex_names_cp.push_back("test.jpg");

	for (unsigned int i = 0; i < tex_names_cp.size(); ++i) {
		std::string full_path = tex_path + tex_names_cp[i];
		FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(full_path.c_str());
		if (fif == FIF_UNKNOWN) { fif = FreeImage_GetFIFFromFilename(full_path.c_str()); }
		if (fif == FIF_UNKNOWN) { std::cerr << "BlockTexture: unknown fif" << std::endl; return; }

		FIBITMAP* dib = nullptr;
		if (FreeImage_FIFSupportsReading(fif)) { dib = FreeImage_Load(fif, full_path.c_str()); }
		if (dib == nullptr) { std::cerr << "BlockTexture: empty dib" << std::endl; return; }

		unsigned int width = FreeImage_GetWidth(dib);
		unsigned int height = FreeImage_GetHeight(dib);
		if (width > max_tex_size) {
			width = max_tex_size;
			FIBITMAP* rdib = FreeImage_Rescale(dib, width, height, FILTER_BICUBIC);
			FreeImage_Unload(dib);
			dib = rdib;
		}
		if (height > max_tex_size) {
			height = max_tex_size;
			FIBITMAP* rdib = FreeImage_Rescale(dib, width, height, FILTER_BICUBIC);
			FreeImage_Unload(dib);
			dib = rdib;
		}
		if (width == 0 || height == 0) { std::cerr << "BlockTexture: width / height == 0" << std::endl; return; }
		if (width > max_width) { if (max_width == 0) { max_width = width; } else { std::cerr << "BlockTexture: width mismatch" << std::endl; return; } }
		if (height > max_height) { if (max_height == 0) { max_height = height; } else { std::cerr << "BlockTexture: height mismatch" << std::endl; return; } }

		unsigned int BPP = FreeImage_GetBPP(dib);
		if (BPP == 0) { std::cerr << "BlockTexture: BPP extraction failed" << std::endl; return; }
		if ((c == BGRA || c == RGBA) && BPP != 32) {
			FIBITMAP* rdib = FreeImage_ConvertTo32Bits(dib);
			FreeImage_Unload(dib);
			dib = rdib;
			BPP = FreeImage_GetBPP(dib);
			DEBUG_NAME_PRINT("format modified", BPP);
		}
		else if ((c == BGR || c == RGB) && BPP != 24) {
			FIBITMAP* rdib = FreeImage_ConvertTo24Bits(dib);
			FreeImage_Unload(dib);
			dib = rdib;
			BPP = FreeImage_GetBPP(dib);
			DEBUG_NAME_PRINT("format modified", BPP);
		}
		DEBUG_NAME_PRINT("format: ", BPP);
		if (!FreeImage_FlipVertical(dib)) { std::cerr << "BlockTexture: Image can't be flipped" << std::endl; return; }

		GLubyte* tex = FreeImage_GetBits(dib);
		if (tex == nullptr) { std::cerr << "BlockTexture: empty tex" << std::endl; return; }

		texs.push_back(tex);
	}

	array_2D_layers_ = texs.size();
	Attach2DArrayTexture(params, c, texs, max_width, max_height);
}

BlockTexture::~BlockTexture() {}

void BlockTexture::Bind(const unsigned int& channel, const unsigned int& location) {
	glUniform1i(location, array_2D_layers_);
	if (texID_ != std::numeric_limits<GLuint>::max()) {
		glActiveTexture(GL_TEXTURE0 + channel);
		glBindTexture(TEXTURE_2D_ARRAY, texID_);
	}
}

void BlockTexture::Unbind() {
	glBindTexture(TEXTURE_2D_ARRAY, 0);
}