#pragma once

#include "FileException.hpp"

#include <filesystem>
#include <fstream>
#include <glm/glm.hpp>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>


std::string readShaderFile(std::string const &filepath);
