#pragma once

#ifdef DEBUG

inline std::ofstream debug_ofs("debug.out", std::ofstream::out);

#define DEBUG_PRINT(value) std::cerr << value << std::endl; debug_ofs << value << std::endl;
#define DEBUG_NAME_PRINT(name, value) std::cerr << name << "[" << value << "]" << std::endl;    debug_ofs << name << "[" << value << "]" << std::endl;

#define TIME_CHECK_START()    double debug_timer_start = omp_get_wtime();
#define TIME_CHECK_END(functionName)    std::cout << "Time taken by function " << functionName << ": " << omp_get_wtime() - debug_timer_start << " seconds" << std::endl;   debug_ofs << "Time taken by function " << functionName << ": " << omp_get_wtime() - debug_timer_start << " seconds" << std::endl;

inline void GLAPIENTRY
MessageCallback(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam) {

    if (severity == GL_DEBUG_SEVERITY_HIGH) {
        std::cerr << "GL CALLBACK: " << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "") << " type = 0x" << type << ", severity = 0x" << severity << ", message = " << message << ", " << userParam << " " << length << " " << id << " " << source << std::endl;
        debug_ofs << "GL CALLBACK: " << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "") << " type = 0x" << type << ", severity = 0x" << severity << ", message = " << message << ", " << userParam << " " << length << " " << id << " " << source << std::endl;
    }
}

#else

#define DEBUG_PRINT(value)
#define DEBUG_NAME_PRINT(name, value)

// #define TIME_CHECK_START()
// #define TIME_CHECK_END(functionName) 
#define TIME_CHECK_START()    double debug_timer_start = omp_get_wtime();
#define TIME_CHECK_END(functionName)    std::cout << "Time taken by function " << functionName << ": " << omp_get_wtime() - debug_timer_start << " seconds" << std::endl;

#endif