#pragma once

#pragma warning(push, 0) 

#include <iostream>
#include <iomanip>
#include <fstream>
#include <ostream>
#include <filesystem>
#include <optional>
#include <type_traits>

#include <chrono>
#include <thread>
#include <memory>
#include <future>
#include <functional>
#include <mutex>
#include <execution>
#include <coroutine>

#include <cstring>
#include <string>

#include <set>
#include <array>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <numeric>
#include <random>
#include <cmath>

// #ifdef _MSC_VER                       
// #pragma comment(lib, "OpenGL32.Lib")
// #include <windows.h>
// #include <gl/GL.h>
// #endif

#include "omp.h"
#include "json.hpp"
#include "exprtk.hpp"
#include "FreeImage.h"
#include "glad/gl.h"
#include "GLFW/glfw3.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/type_precision.hpp"
#include "glm/ext/matrix_float4x4_precision.hpp"
#include "glm/ext/vector_float4_precision.hpp"

#pragma warning(pop)

#include "macros/mathDefinitions.hpp"
#include "macros/objectDefinitions.hpp"
#include "macros/shaderDefinitions.hpp"
#include "macros/cameraDefinitions.hpp"
#include "macros/textureDefinitions.hpp"
#include "macros/openGLDefinitions.hpp"
#include "macros/userDefinitions.hpp"

#include "utils/print.hpp"
#include "utils/convert.hpp"
#include "utils/debug.hpp"
#include "utils/interpolation.hpp"
#include "utils/singleton.hpp"
#include "utils/bulkData.hpp"
#include "utils/hash.hpp"
#include "utils/jsonLoader.hpp"
#include "utils/threadpool.hpp"
#include "utils/scheduler.hpp"
#include "utils/typeTest.hpp"

#include "algorithms/equation.hpp"
#include "algorithms/arkGen.hpp"

#include "window.hpp"
#include "event.hpp"
#include "handlers/shaderHandler.hpp"
#include "handlers/textureHandler.hpp"
#include "handlers/objectHandler.hpp"
#include "handlers/cameraHandler.hpp"

#include "terrain/noiseMap.hpp"
#include "terrain/sky.hpp"

#include "application.hpp"