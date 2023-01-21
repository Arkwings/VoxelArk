#include "common.hpp"
#include "algorithms/diamondSquare.hpp"
#include "algorithms/perlin.hpp"
#include "algorithms/arkGen.hpp"
#include "texture.hpp"

Texture::Texture(const TEXTURE_TARGET t)
	: t_(t) {

	G_OGLThreadPool->QueueJob([&]() {
		glGenTextures(1, &texID_);
		});
}

Texture::~Texture() {
	G_OGLThreadPool->QueueJob([&]() {
		glDeleteTextures(1, &texID_);
		});
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

template <typename T>
void Texture::Attach2DTexture(const std::unordered_map<TEXTURE_PARAM_NAME, TEXTURE_PARAM_VALUE>& params, const TEXTURE_CHANNELS c, T* tex, const unsigned int& width, const unsigned int& height) {

	G_OGLThreadPool->QueueJob([&]() {
		glBindTexture(t_, texID_);

	glPixelStorei(GL_UNPACK_ROW_LENGTH, width);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	if constexpr (eqTypes<T, GLfloat>())	glTexImage2D(t_, 0, (c == RGBA || c == BGRA) ? GL_RGBA32F : GL_RGB32F, width, height, 0, c, GL_FLOAT, tex);
	if constexpr (eqTypes<T, GLubyte>())	glTexImage2D(t_, 0, (c == RGBA || c == BGRA) ? GL_RGBA32F : GL_RGB32F, width, height, 0, c, GL_UNSIGNED_BYTE, tex);

	for (auto param : params)	glTexParameteri(t_, param.first, param.second);
	if (params.find(TEXTURE_MAX_LEVEL) != params.end())	glGenerateMipmap(t_);

	glBindTexture(t_, 0);
		});

	while (!G_OGLThreadPool->IsIdle()) {}
}

template <typename T>
void Texture::Attach2DArrayTexture(const std::unordered_map<TEXTURE_PARAM_NAME, TEXTURE_PARAM_VALUE>& params, const TEXTURE_CHANNELS c, const std::vector<T*>& tex, const unsigned int& width, const unsigned int& height) {

	G_OGLThreadPool->QueueJob([&]() {
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

	if constexpr (eqTypes<T, GLubyte>()) {
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
	}
	if constexpr (eqTypes<T, GLfloat>()) {
		for (unsigned int i = 0U; i < tex.size(); ++i) {
			glTexSubImage3D(
				GL_TEXTURE_2D_ARRAY, 	//type
				0, 						//mimap number
				0, 0, i,				//xoffset, yoffset, zoffset
				width, height,  		//width, height, 
				1,						//layer
				c, 						//format
				GL_FLOAT, 		//type
				tex[i]);				//data pointer
		}
	}

	for (auto param : params)	glTexParameteri(t_, param.first, param.second);
	if (mipmap)	    glGenerateMipmap(t_);

	glBindTexture(t_, 0);
		});

	while (!G_OGLThreadPool->IsIdle()) {}
}

/////////
//IMAGE//
/////////

ImageTexture::ImageTexture(const std::vector<std::string>& tex_names, const TEXTURE_TARGET t, const std::unordered_map<TEXTURE_PARAM_NAME, TEXTURE_PARAM_VALUE>& params, const TEXTURE_CHANNELS c)
	: Texture(t)
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
		if (width > TEX::MAX_TEX_SIZE) {
			width = TEX::MAX_TEX_SIZE;
			FIBITMAP* rdib = FreeImage_Rescale(dib, width, height, FILTER_BICUBIC);
			FreeImage_Unload(dib);
			dib = rdib;
		}
		if (height > TEX::MAX_TEX_SIZE) {
			height = TEX::MAX_TEX_SIZE;
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
		dibs.push_back(dib);
		if (tex == nullptr) { std::cerr << "Texture: empty tex" << std::endl; return; }

		texs.push_back(tex);

		if (t == TEXTURE_2D) { Attach2DTexture(params, c, texs[0], width, height); break; }
	}

	if (t == TEXTURE_2D_ARRAY) {
		array_2D_layers_ = texs.size();
		Attach2DArrayTexture(params, c, texs, max_width, max_height);
	}

	for (unsigned int i = 0; i < dibs.size(); ++i)	FreeImage_Unload(dibs[i]);
}

ImageTexture::~ImageTexture() {}

//PROCEDURAL//

ProceduralTexture::ProceduralTexture(const TEXTURE_ALGORITHMS algo, const std::vector<std::any>& algo_params, const TEXTURE_TARGET t, const std::unordered_map<TEXTURE_PARAM_NAME, TEXTURE_PARAM_VALUE>& params, const TEXTURE_CHANNELS c)
	: Texture(t) {
	GLfloat* tex = nullptr;

	Generation::ArkNoise<GLfloat>* testNoise = nullptr;
	int sizeX, sizeY;

	if (c == RGB || c == BGR) {
		if (algo == DIAMOND_SQUARE) {
			if (algo_params.size() != 14)	std::cerr << "for diamond square algorithm, algo_params must be [(int)size, (int)channels, (GLfloat)chan_min* 3, (GLfloat)chan_max * 3, (bool)range_attenuation * 3, (GLfloat)range_factor, (unsigned int)seed, (bool)seamless_x, (bool)seamless_y]" << std::endl;
			sizeX = std::any_cast<int>(algo_params[0]);
			tex = Generation::DiamondSquare<GLfloat>(
				sizeX,
				{ std::any_cast<GLfloat>(algo_params[1]),std::any_cast<GLfloat>(algo_params[2]) , std::any_cast<GLfloat>(algo_params[3]) },
				{ std::any_cast<GLfloat>(algo_params[4]),std::any_cast<GLfloat>(algo_params[5]) , std::any_cast<GLfloat>(algo_params[6]) },
				{ std::any_cast<bool>(algo_params[7]),std::any_cast<bool>(algo_params[8]) , std::any_cast<bool>(algo_params[9]) },
				std::any_cast<GLfloat>(algo_params[10]),
				std::any_cast<unsigned int>(algo_params[11]),
				3,
				std::any_cast<bool>(algo_params[12]),
				std::any_cast<bool>(algo_params[13]));
			sizeY = sizeX;
		}
		if (algo == PERLIN_NOISE) {
			if (algo_params.size() != 6)	std::cerr << "for perlin noise algorithm, algo_params must be [(int)sizeX, (int)sizeY, (unsigned int)seed, (unsigned int)octaves, (GLfloat)persistence, (GLfloat)frequency" << std::endl;
			sizeX = std::any_cast<int>(algo_params[0]);
			sizeY = std::any_cast<int>(algo_params[1]);
			tex = Generation::NoiseMap<GLfloat, 2, Generation::PERLIN_OCTAVE_NOISE_CLAMP_REMAP>(
				glm::vec2(sizeX, sizeY),
				std::any_cast<unsigned int>(algo_params[2]),
				std::any_cast<unsigned int>(algo_params[3]),
				std::any_cast<GLfloat>(algo_params[4]),
				std::any_cast<GLfloat>(algo_params[5]));
		}
		if (algo == ARK_NOISE) {
			if (algo_params.size() != 15)	std::cerr << "for ark noise algorithm, algo_params must be [(unsigned int)sizeX, (unsigned int)sizeY, (GLfloat)frequency, (unsigned int)vertices seed*4, (unsigned int)edges seed*4, (unsigned int)inner seed, (unsigned int)octaves, (GLfloat)persistence, (GLfloat)amplitude" << std::endl;
			sizeX = static_cast<int>(std::any_cast<unsigned int>(algo_params[0]));
			sizeY = static_cast<int>(std::any_cast<unsigned int>(algo_params[1]));
			testNoise = new Generation::ArkNoise<GLfloat>(sizeX, sizeY, std::any_cast<GLfloat>(algo_params[2]));
			testNoise->GenerateGradients(
				{ std::any_cast<unsigned int>(algo_params[3]), std::any_cast<unsigned int>(algo_params[4]), std::any_cast<unsigned int>(algo_params[5]), std::any_cast<unsigned int>(algo_params[6]) },
				{ std::any_cast<unsigned int>(algo_params[7]), std::any_cast<unsigned int>(algo_params[8]), std::any_cast<unsigned int>(algo_params[9]), std::any_cast<unsigned int>(algo_params[10]) },
				std::any_cast<unsigned int>(algo_params[11]));
			testNoise->GeneratePoints(true, std::any_cast<unsigned int>(algo_params[12]), std::any_cast<GLfloat>(algo_params[13]), std::any_cast<GLfloat>(algo_params[14]));
			tex = testNoise->GetPoints();
		}
	}
	else if (c == RGBA || c == BGRA) {
		if (algo == DIAMOND_SQUARE) {
			if (algo_params.size() != 17)	std::cerr << "for diamond square algorithm, algo_params must be [(int)size, (int)channels, (GLfloat)chan_min* 3, (GLfloat)chan_max * 3, (bool)range_attenuation * 3, (GLfloat)range_factor, (unsigned int)seed, (bool)seamless_x, (bool)seamless_y]" << std::endl;
			int size = std::any_cast<int>(algo_params[0]);
			tex = Generation::DiamondSquare<GLfloat>(
				size,
				{ std::any_cast<GLfloat>(algo_params[1]),std::any_cast<GLfloat>(algo_params[2]) , std::any_cast<GLfloat>(algo_params[3]), std::any_cast<GLfloat>(algo_params[4]) },
				{ std::any_cast<GLfloat>(algo_params[5]),std::any_cast<GLfloat>(algo_params[6]) , std::any_cast<GLfloat>(algo_params[7]), std::any_cast<GLfloat>(algo_params[8]) },
				{ std::any_cast<bool>(algo_params[9]),std::any_cast<bool>(algo_params[10]) , std::any_cast<bool>(algo_params[11]), std::any_cast<bool>(algo_params[12]) },
				std::any_cast<GLfloat>(algo_params[13]),
				std::any_cast<unsigned int>(algo_params[14]),
				4,
				std::any_cast<bool>(algo_params[15]),
				std::any_cast<bool>(algo_params[16]));
		}
	}

	Attach2DTexture(params, c, tex, sizeX, sizeY);

	if (tex)	delete[] tex;
	if (testNoise != nullptr)	delete testNoise;
}

ProceduralTexture::~ProceduralTexture() {}
