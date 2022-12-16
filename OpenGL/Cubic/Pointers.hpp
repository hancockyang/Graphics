#pragma once

#include <vector>
#include <glm/glm.hpp>

namespace Points {
   std::vector<glm::vec3> points = {
       glm::vec3(-0.5, -0.5, -0.5),
       glm::vec3(0.5, -0.5, -0.5),
       glm::vec3(-0.5,  0.5, -0.5),
       glm::vec3(0.5,  0.5, -0.5),
       glm::vec3(-0.5, -0.5,  0.5),
       glm::vec3(0.5, -0.5,  0.5),
       glm::vec3(-0.5,  0.5,  0.5),
       glm::vec3(0.5,  0.5,  0.5)
   };

}
