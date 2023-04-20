#pragma once

#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <fstream>
#include <vector>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../headers/shader.h"
#include "../headers/camera.h"
#include "../headers/light/light.h"
#include "../headers/light/light_mesh.h"
#include "../headers/bezier/curve/bezier_curve.h"
#include "../headers/bezier/curve/bezier_curve_mesh.h"
#include "../headers/bezier/curve/bezier_curve_control_polygon_mesh.h"
#include "../headers/bezier/surface/bezier_surface.h"
#include "../headers/bezier/surface/bezier_surface_mesh.h"
#include "../headers/bezier/surface/bezier_surface_control_polygon_mesh.h"
#include "../headers/bezier/surface/bezier_surface_normal_mesh.h"


// Screen settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

