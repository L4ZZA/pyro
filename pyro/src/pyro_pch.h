#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "pyro/core/logger.h"
#include "pyro/debug/profiler.h"

#ifdef PYRO_PLATFORM_WIN
    #include <Windows.h>
#endif 

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "pyro/utils/glm_extensions.h"
