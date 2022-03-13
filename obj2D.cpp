#include "obj2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

#include<math.h>

using namespace std;

Mesh* obj2D::CreateTank(
    const std::string& name,
    float xT, float yT,
    int length,
    glm::vec3 color
)
{
    int res = length / 2;
    int offset = 0;

    glm::vec3 body_corner = glm::vec3(xT, yT, 0);
    glm::vec3 turret_corner = glm::vec3(xT + length / 4, yT + length / 4, 0);
    glm::vec3 hatch_corner = glm::vec3(xT, yT, 0);
    glm::vec3 gun_corner = glm::vec3(xT + length * 3 / 4, yT + length / 2, 0);

    glm::vec3 turret_color = glm::vec3(0.2f, 0.443f, 0.12f);
    glm::vec3 gun_color = glm::vec3(0.02f, 0.1f, 0.05f);

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(gun_corner, gun_color),
        VertexFormat(gun_corner + glm::vec3(length * 3 / 2, 0, 0), gun_color),
        VertexFormat(gun_corner + glm::vec3(length * 3 / 2, 6, 0), gun_color),
        VertexFormat(gun_corner + glm::vec3(0, 6, 0), gun_color),

        VertexFormat(turret_corner, turret_color),
        VertexFormat(turret_corner + glm::vec3(length, 0, 0), turret_color),
        VertexFormat(turret_corner + glm::vec3(length, length / 2 , 0), turret_color),
        VertexFormat(turret_corner + glm::vec3(0, length / 2, 0), turret_color),

        VertexFormat(body_corner, color),
        VertexFormat(body_corner + glm::vec3(length * 3 / 2, 0, 0), color),
        VertexFormat(body_corner + glm::vec3(length * 3 / 2, length , 0), color),
        VertexFormat(body_corner + glm::vec3(0, length, 0), color)

    };

    std::vector<unsigned int> indices = { 0, 1, 2,  0, 2, 3,  4, 5, 6,  4, 6, 7,  8, 9, 10,  8, 10, 11 };
    Mesh* tank = new Mesh(name);
    tank->InitFromData(vertices, indices);
    return tank;
}


Mesh* obj2D::CreateUFO(
    const std::string& name,
    float xT, float yT,
    int length,
    glm::vec3 color,
    bool fill)
{
    int res = length / 2;
    int offset = 0;

    vector<VertexFormat> circle_vertices;
    vector<unsigned int> circle_indices;

    //hub
    float lh_xT = xT;
    float lh_yT = yT;

    circle_vertices.push_back(VertexFormat(glm::vec3(lh_xT, lh_yT, 0),
        glm::vec3(0, 0, 1),
        glm::vec3(0.2, 0.8, 0.6)));

    for (double fi = 0; fi < 2 * M_PI; fi += 2 * M_PI / res) {
        double x = lh_xT + (length / 4) * cos(fi);
        double y = lh_yT + (length / 4) * sin(fi);

        circle_vertices.push_back(VertexFormat(glm::vec3(x, y, 0),
            glm::vec3(0.4f, 0.6f, 0.8f),
            glm::vec3(0.23f, 0.5f, 0.872f)));
    }

    for (unsigned int x = 1; x < res; x++) {
        circle_indices.push_back(offset);
        circle_indices.push_back(x + offset);
        circle_indices.push_back(x + offset + 1);
    }

    circle_indices.push_back(offset);
    circle_indices.push_back(offset + res);
    circle_indices.push_back(offset + 1);

    offset += res + 1;


    //body
    circle_vertices.push_back(VertexFormat(glm::vec3(xT, yT, 0),
        glm::vec3(0.75f, 0.75f, 0.75f),
        glm::vec3(0.2, 0.8, 0.6)));

    for (double fi = 0; fi < 2 * M_PI; fi += 2 * M_PI / res) {
        double x = xT + (length / 2) * cos(fi);
        double y = yT + (length / 2) * sin(fi);

        circle_vertices.push_back(VertexFormat(glm::vec3(x, y, 0),
            glm::vec3(0.5f, 0.5f, 0.5f),
            glm::vec3(0.2, 0.8, 0.6)));
    }

    for (unsigned int x = 1; x < res; x++) {
        circle_indices.push_back(offset);
        circle_indices.push_back(x + offset);
        circle_indices.push_back(x + offset + 1);
    }

    circle_indices.push_back(offset);
    circle_indices.push_back(offset + res);
    circle_indices.push_back(offset + 1);

    offset += res + 1;


    //turret
    circle_vertices.push_back(VertexFormat(glm::vec3(xT, yT - 4.5 * length / 10, 0),
        glm::vec3(0.2f, 0.2f, 0.8f),
        glm::vec3(0.2, 0.8, 0.6)));

    for (double fi = 0; fi < 2 * M_PI; fi += 2 * M_PI / res) {
        double x = xT + (length / 6) * cos(fi);
        double y = yT - (4.5 * length / 10) + (length / 5) * sin(fi);

        circle_vertices.push_back(VertexFormat(glm::vec3(x, y, 0),
            glm::vec3(0.5f, 0.5f, 0.5f),
            glm::vec3(0.2, 0.8, 0.6)));
    }

    for (unsigned int x = 1; x < res; x++) {
        circle_indices.push_back(offset);
        circle_indices.push_back(x + offset);
        circle_indices.push_back(x + offset + 1);
    }

    circle_indices.push_back(offset);
    circle_indices.push_back(offset + res);
    circle_indices.push_back(offset + 1);


    Mesh* player = new Mesh(name);
    player->InitFromData(circle_vertices, circle_indices);

    return player;
}


Mesh* obj2D::CreateSquare(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}