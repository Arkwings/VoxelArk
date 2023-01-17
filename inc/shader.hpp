#pragma once

class Shader {
	public:
	Shader() = delete;
	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;
	Shader(const std::string debug_file, const std::string vs_path, const std::string fs_path, const std::string gs_path = "");
	~Shader();

	void Recompile();
	GLuint GetID() { return shaderID_; }

	private:
	std::string readFile(std::string shader_path);
	void compile();
	void compileShader(const int type, std::string shader_path);

	GLuint shaderID_;
	bool errors_;
	std::string debug_file_path_;
	std::ofstream* debug_file_;
	std::string shader_path_;
	std::string vs_path_;
	std::string gs_path_;
	std::string fs_path_;
	std::filesystem::file_time_type modif_time_;
};
