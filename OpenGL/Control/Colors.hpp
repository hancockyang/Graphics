#pragma once

#include <vector>
#include "glm/glm.hpp"

namespace Colors {

   std::vector<glm::vec3> colors = {
       glm::vec3(1.0, 1.0, 1.0),	// White
       glm::vec3(1.0, 1.0, 0.0),	// Yellow
       glm::vec3(0.0, 1.0, 0.0),	// Green
       glm::vec3(0.0, 1.0, 1.0),	// Cyan
       glm::vec3(1.0, 0.0, 1.0),	// Magenta
       glm::vec3(1.0, 0.0, 0.0),	// Red
       glm::vec3(0.5, 0.5, 0.5),	// Gray
       glm::vec3(0.0, 0.0, 1.0)	// Blue
   };

}
