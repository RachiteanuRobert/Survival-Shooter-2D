#pragma onced sa

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace m1
{

    // Create square with given bottom left corner, length and color
    Mesh* CreateSquare(const std::string& name, glm::vec3 leftBottomCorner, int length, glm::vec3 color, bool fill = true);

    Mesh* CreateRectangle(const std::string& name, glm::vec3 leftBottomCorner, int width, int height, glm::vec3 color, bool fill);
    
    Mesh* CreateUFO(const std::string& name, float xT, float yT, int length, glm::vec3 color, bool fill = true);

    Mesh* CreateTank(const std::string& name, float xT, float yT, int length, glm::vec3 color);
}
