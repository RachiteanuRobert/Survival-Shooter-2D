#pragma once

#include "components/simple_scene.h"
#include "entity.h"

using namespace std;

namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        struct ViewportSpace
        {
            ViewportSpace() : x(0), y(0), width(1), height(1) {}
            ViewportSpace(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

        struct LogicSpace
        {
            LogicSpace() : x(0), y(0), width(1), height(1) {}
            LogicSpace(float x, float y, float width, float height)
                : x(x), y(y), width(width), height(height) {}
            float x;
            float y;
            float width;
            float height;
        };

     public:
         Tema1();
        ~Tema1();

        void Init() override;

        int squareSide;
        bool moved_x = true, moved_y = true, h_pack_spawned = true;
        float player_health = 10, player_exp = 0, h_pack_timer = 0 ;
        float time_counter = 0, projectile_time_counter = 0, prev_x = 0, prev_y = 0;
        Entity player, map_base, health_pack;
        vector<Entity> enemies, obstacles, projectiles;
        Mesh* projectile_mesh;
        Mesh* enemy_mesh;
        Entity new_obstacle, new_border, health_bar, exp_bar, health_border, exp_border;

     private:
        void FrameStart() override;
        glm::mat3 ComputeEntityMatrix(Entity curr_entity);
        glm::vec2 CheckCollision_Circle_AABB(Entity& one, Entity& two);
        glm::vec2 CheckCollision_Circle_Circle(Entity one, Entity two);
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        // Sets the logic space and view space
        // logicSpace: { x, y, width, height }
        // viewSpace: { x, y, width, height }
        glm::mat3 VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);
        glm::mat3 VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);

        void SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor = glm::vec3(0), bool clear = true);


     protected:
        float cx, cy;
        float translateX = 0;
        float translateY = 0;
        float scaleX = 1;
        float scaleY = 1;
        float angularStep = 0;

        // TODO(student): If you need any other class variables, define them here.
        float scaleSpeed = 0.5;

        float length;
        ViewportSpace viewSpace;
        LogicSpace logicSpace;
        glm::mat3 modelMatrix, visMatrix;
    };
}   // namespace m1
