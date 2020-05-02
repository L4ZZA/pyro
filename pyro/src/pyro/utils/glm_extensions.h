#pragma once
#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"

namespace pyro {
namespace math {

    static constexpr glm::vec3 axis_x    = glm::vec3( 1.f, 0.f, 0.f );
    static constexpr glm::vec3 axis_y    = glm::vec3( 0.f, 1.f, 0.f );
    static constexpr glm::vec3 axis_z    = glm::vec3( 0.f, 0.f, 1.f );
    static constexpr glm::vec3 zero_vec3 = glm::vec3(0.0f, 0.0f, 0.0f);
    static constexpr glm::vec3 origin    = zero_vec3;

}
}