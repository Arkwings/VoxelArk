#include "common.hpp"
#include "shader.hpp"

Shader::Shader(const std::string debug_file, const std::string vs_path, const std::string fs_path, const std::string gs_path)
	: shaderID_(0)
	, errors_(false)
	, debug_file_path_(debug_file)
	, debug_file_(new std::ofstream(debug_file, std::ofstream::out))
	, shader_path_("")
	, vs_path_(vs_path)
	, gs_path_(gs_path)
	, fs_path_(fs_path) {
#ifdef DEBUG
	shader_path_ = __FILE__;
	shader_path_ = shader_path_.substr(0, shader_path_.rfind("\\") + 1);
	DEBUG_NAME_PRINT("shader_path", shader_path_);
#endif

	//check vs
	std::filesystem::file_time_type temp_time1 = std::filesystem::last_write_time(shader_path_ + vs_path_);
	if (modif_time_ < temp_time1)
		modif_time_ = temp_time1;

	//check gs
	if (!gs_path_.empty()) {
		std::filesystem::file_time_type temp_time2 = std::filesystem::last_write_time(shader_path_ + gs_path_);
		if (modif_time_ < temp_time2)
			modif_time_ = temp_time2;
	}

	//check fs
	std::filesystem::file_time_type temp_time3 = std::filesystem::last_write_time(shader_path_ + fs_path_);
	if (modif_time_ < temp_time3)
		modif_time_ = temp_time3;

	compile();
}

Shader::~Shader() {
	glDeleteProgram(shaderID_);
	delete debug_file_;
}

void Shader::Recompile() {
	bool recompile = false;

	//check vs
	std::filesystem::file_time_type temp_time1 = std::filesystem::last_write_time(shader_path_ + vs_path_);
	if (modif_time_ < temp_time1) {
		modif_time_ = temp_time1;
		recompile = true;
	}

	//check gs
	if (!gs_path_.empty()) {
		std::filesystem::file_time_type temp_time2 = std::filesystem::last_write_time(shader_path_ + gs_path_);
		if (modif_time_ < temp_time2) {
			modif_time_ = temp_time2;
			recompile = true;
		}
	}

	//check fs
	std::filesystem::file_time_type temp_time3 = std::filesystem::last_write_time(shader_path_ + fs_path_);
	if (modif_time_ < temp_time3) {
		modif_time_ = temp_time3;
		recompile = true;
	}

	if (recompile) {
		delete debug_file_;
		debug_file_ = new std::ofstream(shader_path_ + debug_file_path_, std::ofstream::out);

		glDeleteProgram(shaderID_);
		compile();
	}
}

std::string Shader::readFile(const std::string shader_path) {
	std::ifstream shader_stream(shader_path, std::ios::in);

	if (shader_stream.is_open()) {
		std::string shader_content;
		std::string tmp("");
		while (!shader_stream.eof()) {
			std::getline(shader_stream, tmp);
			shader_content.append(tmp + '\n');
		}
		shader_stream.close();
		return shader_content;
	}
	else {
		std::cerr << "shaders ---- could not read file (" << shader_path << ")" << std::endl;
		exit(-1);
	}
}

void Shader::compile() {
	GLint result;
	int logLength;

	(*debug_file_) << "shaders ---- CREATING PROGRAM ..." << std::endl;

	shaderID_ = glCreateProgram();

	compileShader(GL_VERTEX_SHADER, shader_path_ + vs_path_);
	if (!gs_path_.empty()) compileShader(GL_GEOMETRY_SHADER, shader_path_ + gs_path_);
	compileShader(GL_FRAGMENT_SHADER, shader_path_ + fs_path_);

	glLinkProgram(shaderID_);
	glGetProgramiv(shaderID_, GL_LINK_STATUS, &result);
	glGetProgramiv(shaderID_, GL_INFO_LOG_LENGTH, &logLength);

	std::vector<char> pError((logLength > 1) ? static_cast<long unsigned int>(logLength) : 1);
	glGetProgramInfoLog(shaderID_, logLength, NULL, &pError[0]);

	(*debug_file_) << "shaders ---- ERRORS: " << &pError[0] << std::endl << std::endl;
}

void Shader::compileShader(const int type, std::string shader_path) {
	GLint result;
	int logLength;

	(*debug_file_) << "shaders ---- PATH: " << shader_path << std::endl;
	GLuint fileID = glCreateShader(type);
	std::string vs_str = readFile(shader_path.c_str());
	const char* vs_src = vs_str.c_str();

	(*debug_file_) << "        ---- compile and check vs ..." << std::endl;
	glShaderSource(fileID, 1, &vs_src, NULL);
	glCompileShader(fileID);
	glGetShaderiv(fileID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fileID, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<char> shaderError((logLength > 1) ? static_cast<long unsigned int>(logLength) : 1);
	glGetShaderInfoLog(fileID, logLength, NULL, &shaderError[0]);
	(*debug_file_) << "        ---- errors: " << &shaderError[0] << std::endl;
	if (shaderError[0] != '\0') errors_ = true;

	glAttachShader(shaderID_, fileID);
	glDeleteShader(fileID);
}
