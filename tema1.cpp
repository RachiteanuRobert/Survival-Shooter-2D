#include "lab_m1/tema1/tema1.h"

#include <vector>
#include <iostream>

#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/obj2D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}

void Tema1::Init()
{   
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    int map_length = 2000;
    int squareSide = map_length / 20;

    logicSpace.x = 0;       // logic x
    logicSpace.y = 0;       // logic y
    logicSpace.width = resolution.x;   // logic width
    logicSpace.height = resolution.y;  // logic height

    glm::vec3 corner = glm::vec3(0.001, 0.001, 0);
    glm::vec2 left_corner = glm::vec2(-map_length / 2, -map_length / 2);

    //Player Mesh
    Mesh* player_mesh = CreateUFO("player_model", 0, 0, squareSide, glm::vec3(0, 0, 1));
    player.mesh = player_mesh;
    player.radius = 50;

    //Map Base Mesh
    Mesh* map_base_mesh = CreateSquare("map_base_model",
        glm::vec3(left_corner.x, left_corner.y, 0), map_length, glm::vec3(0.403, 0.074, 0.403));
    map_base.mesh = map_base_mesh;

    //Map Border Meshes
    //Botttom Map Border
    Mesh* new_border_mesh = CreateRectangle("map_base_model",
        glm::vec3(left_corner.x, 0, 0), map_length, 10, glm::vec3(0.650, 0.803, 0.886), true);
    new_border.mesh = new_border_mesh;
    new_border.xpos = 0;
    new_border.ypos = left_corner.y;
    new_border.width = map_length;
    new_border.height = 10;
    new_border.ld_corner.x = left_corner.x;
    new_border.ld_corner.y = left_corner.y;
    obstacles.push_back(new_border);

    //Upper Map Border
    new_border_mesh = CreateRectangle("upper_border_model",
        glm::vec3(left_corner.x, 0, 0), map_length, 10, glm::vec3(0.650, 0.803, 0.886), true);
    new_border.mesh = new_border_mesh;
    new_border.xpos = 0;
    new_border.ypos = -left_corner.y;
    new_border.width = map_length;
    new_border.height = 10;
    new_border.ld_corner.x = left_corner.x;
    new_border.ld_corner.y = -left_corner.y;
    obstacles.push_back(new_border);

    //Left Map Border
    new_border_mesh = CreateRectangle("left_border_model",
        glm::vec3(0, left_corner.y, 0), 10, map_length, glm::vec3(0.650, 0.803, 0.886), true);
    new_border.mesh = new_border_mesh;
    new_border.xpos = -left_corner.x;
    new_border.ypos = 0;
    new_border.width = 10;
    new_border.height = map_length;
    new_border.ld_corner.x = -left_corner.x;
    new_border.ld_corner.y = left_corner.y;
    obstacles.push_back(new_border);

    //Right Map Border
    new_border_mesh = CreateRectangle("right_border_model",
        glm::vec3(0, left_corner.y, 0), 10, map_length + 10, glm::vec3(0.650, 0.803, 0.886), true);
    new_border.mesh = new_border_mesh;
    new_border.xpos = left_corner.x;
    new_border.ypos = 0;
    new_border.width = 10;
    new_border.height = map_length + 10;
    new_border.ld_corner.x = left_corner.x;
    new_border.ld_corner.y = left_corner.y;
    obstacles.push_back(new_border);

    //Projectile Mesh
    projectile_mesh = CreateSquare("projectile_model",
        glm::vec3(player.xpos, player.ypos, 0), 20, glm::vec3(1, 1, 0));

    //Obstacle Mesh
    Mesh* obstacle_mesh = CreateRectangle("lu_obstacle_model",
        glm::vec3(-300, -250, 0), 600, 500, glm::vec3(0.521, 0.980, 0), true); //left up
    new_obstacle.mesh = obstacle_mesh;
    new_obstacle.xpos = -400;
    new_obstacle.ypos = 400;
    new_obstacle.width = 600/2;
    new_obstacle.height = 500/2;
    new_obstacle.ld_corner.x = -700;
    new_obstacle.ld_corner.y = 150;
    obstacles.push_back(new_obstacle);

    
    obstacle_mesh = CreateRectangle("ld_obstacle_model",
        glm::vec3(-300, -250, 0), 600, 500, glm::vec3(0.956, 0.956, 0.4), true); //left down
    new_obstacle.mesh = obstacle_mesh;
    new_obstacle.xpos = -400;
    new_obstacle.ypos = -400;
    new_obstacle.width = 600/2;
    new_obstacle.height = 500/2;
    new_obstacle.ld_corner.x = -700;
    new_obstacle.ld_corner.y = -650;
    obstacles.push_back(new_obstacle);

    obstacle_mesh = CreateRectangle("ru_obstacle_model",
        glm::vec3(-300, -250, 0), 600, 500, glm::vec3(0.956, 0.956, 0.41), true); //right up
    new_obstacle.mesh = obstacle_mesh;
    new_obstacle.xpos = 400;
    new_obstacle.ypos = 400;
    new_obstacle.width = 600/2;
    new_obstacle.height = 500/2;
    new_obstacle.ld_corner.x = 100;
    new_obstacle.ld_corner.y = 150;
    obstacles.push_back(new_obstacle);

    obstacle_mesh = CreateRectangle("rd_obstacle_model",
        glm::vec3(-300, -250, 0), 600, 500, glm::vec3(0.521, 0.980, 0), true); //right down
    new_obstacle.mesh = obstacle_mesh;
    new_obstacle.xpos = 400;
    new_obstacle.ypos = -400;
    new_obstacle.width = 600/2;
    new_obstacle.height = 500/2;
    new_obstacle.ld_corner.x = 100;
    new_obstacle.ld_corner.y = -650;
    obstacles.push_back(new_obstacle); 

    //Health Border Mesh
    obstacle_mesh = CreateRectangle("health_bar_outline_model",
        glm::vec3(-225, -35, 0), 450, 70, glm::vec3(1, 1, 1), false); 
    health_border.mesh = obstacle_mesh;
    health_border.xpos = 300;
    health_border.ypos = 200;

    //Health Bar Mesh
    obstacle_mesh = CreateRectangle("health_bar_outline_model",
        glm::vec3(-224, -34, 0), 448, 68, glm::vec3(1, 0, 0), true);
    health_bar.mesh = obstacle_mesh;
    health_bar.xpos = 300;
    health_bar.ypos = 200;
    health_bar.width = 448;

    //Health Pack Mesh
    obstacle_mesh = CreateSquare("map_base_model",
        glm::vec3(-35, -35, 0), 70, glm::vec3(1, 0, 0));
    health_pack.mesh = obstacle_mesh;
    health_pack.xpos = 0;
    health_pack.ypos = 850;
    health_pack.width = 70 / 2;
    health_pack.height = 70 / 2;
    health_pack.ld_corner.x = -35;
    health_pack.ld_corner.y = 815;
    //health_packs.push_back(health_pack);


    //Experience Border Mesh
    obstacle_mesh = CreateRectangle("experience_bar_outline_model",
      glm::vec3(-225, -35, 0), 450, 70, glm::vec3(1, 1, 1), false);
    exp_border.mesh = obstacle_mesh;
    exp_border.xpos = 300;
    exp_border.ypos = 100;

    //Experience Bar Mesh
    obstacle_mesh = CreateRectangle("experience_bar_outline_model",
        glm::vec3(-224, -34, 0), 448, 68, glm::vec3(1, 1, 0), true);
    exp_bar.mesh = obstacle_mesh;
    exp_bar.xpos = 300;
    exp_bar.ypos = 100;
    exp_bar.width = 448;

    //Enemy Mesh
    enemy_mesh = CreateTank("tank_model", -squareSide / 2, -squareSide / 2,
        squareSide, glm::vec3(0, 0.3f, 0));
}

// 2D visualization matrix
glm::mat3 Tema1::VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    tx = viewSpace.x - sx * logicSpace.x;
    ty = viewSpace.y - sy * logicSpace.y;

    return glm::transpose(glm::mat3(
        sx, 0.0f, tx,
        0.0f, sy, ty,
        0.0f, 0.0f, 1.0f));
}


// Uniform 2D visualization matrix (same scale factor on x and y axes)
glm::mat3 Tema1::VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty, smin;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    if (sx < sy)
        smin = sx;
    else
        smin = sy;
    tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
    ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

    return glm::transpose(glm::mat3(
        smin, 0.0f, tx,
        0.0f, smin, ty,
        0.0f, 0.0f, 1.0f));
}


void Tema1::SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear)
{
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
    GetSceneCamera()->Update();
}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


}

//Create Model Matrix 
glm::mat3 Tema1::ComputeEntityMatrix(Entity curr_entity) {
    curr_entity.matrix = glm::mat3(1);
    curr_entity.matrix *= transform2D::Translate(curr_entity.xpos, curr_entity.ypos);
    curr_entity.matrix *= transform2D::Scale(curr_entity.xscale, curr_entity.yscale);
    curr_entity.matrix *= transform2D::Rotate(curr_entity.rotation);

    return curr_entity.matrix;
}

//Checks the collision between a circle and a rectangle
glm::vec2 Tema1::CheckCollision_Circle_AABB(Entity & one, Entity & two){
    glm::vec2 c_position(one.xpos, one.ypos);
    // get center point circle first 
    glm::vec2 center(c_position);
    // calculate AABB info (center, half-extents)
    glm::vec2 aabb_half_extents(two.width, two.height);
    glm::vec2 aabb_center(
        two.ld_corner.x + aabb_half_extents.x,
        two.ld_corner.y + aabb_half_extents.y
    );
    // get difference vector between both centers
    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    // add clamped value to AABB_center and we get the value of box closest to circle
    glm::vec2 closest = aabb_center + clamped;
    // retrieve vector between center circle and closest point AABB and check if length <= radius
    difference = closest - center;

    if (glm::length(difference) < one.radius)
        return glm::normalize(difference);
    else
        return glm::vec2(0);
}

//Checks the collision between 2 circles
glm::vec2 Tema1::CheckCollision_Circle_Circle(Entity one, Entity two) {

    glm::vec2 difference = glm::vec2(one.xpos, one.ypos) - glm::vec2(two.xpos, two.ypos);

    if(glm::length(difference) < one.radius + two.radius)
        return glm::normalize(difference);
    else
        return glm::vec2(0);
}

void Tema1::Update(float deltaTimeSeconds)
{
    //Logic Space initialize
    glm::ivec2 resolution = window->GetResolution();
    logicSpace.x = player.xpos - resolution.x / 2;
    logicSpace.y = player.ypos - resolution.y / 2;
    logicSpace.width = resolution.x;
    logicSpace.height = resolution.y;
    viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
    SetViewportArea(viewSpace, glm::vec3(0), true);

    modelMatrix = glm::mat3(1);

    glm::mat3 visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);
    
   
    for (auto& curr_proj = projectiles.begin(); curr_proj != projectiles.end();) {
        bool hit = false;
        //Obstacle - Projectile Collision
        for (auto& curr_obstacle = obstacles.begin(); curr_obstacle != obstacles.end();) {
            glm::vec2 response = CheckCollision_Circle_AABB(*curr_proj, *curr_obstacle);

            if (response != glm::vec2(0)) {
                curr_proj = projectiles.erase(curr_proj);
                hit = true;
                break;
            }
            else
                curr_obstacle++;
        }

        if (hit) continue;

        //Enemy - Projectile Collision
        for (auto& curr_enemy = enemies.begin(); curr_enemy != enemies.end();) {
            glm::vec2 response = CheckCollision_Circle_Circle(*curr_proj, *curr_enemy);

            if (response != glm::vec2(0)) {
                curr_enemy = enemies.erase(curr_enemy);
                curr_proj = projectiles.erase(curr_proj);
                hit = true;
                player_exp++;
                if (player_exp > 10)
                    player_exp = 0;
                break;
            }
            else
                curr_enemy++;   
        }

        if (!hit)  curr_proj++;
    }

    //Enemy - Player Collision
    moved_x = true;
    moved_y = true;
    for (auto& curr_enemy = enemies.begin(); curr_enemy != enemies.end();) {
        glm::vec2 response = CheckCollision_Circle_Circle(player, *curr_enemy);

        if (response.x != 0)
            moved_x = false;
        if (response.y != 0)
            moved_y = false;
        if (response != glm::vec2(0)) {
            curr_enemy = enemies.erase(curr_enemy);
            player_health--;
            if (player_health == 0)
                exit(0);
        }
        else
            curr_enemy++;
    }

    //Obstacle - Player Collision
    for (auto& curr_obstacle = obstacles.begin(); curr_obstacle != obstacles.end();) {
        glm::vec2 response = CheckCollision_Circle_AABB(player, *curr_obstacle);

        if (response != glm::vec2(0)) {
            if (response.x != 0)        
                player.xpos = prev_x;
            
            if (response.y != 0) 
                player.ypos = prev_y;

            break;
        }
        else
            curr_obstacle++;
    }

    //Health Pack - Player Collision
    glm::vec2 resp = CheckCollision_Circle_AABB(player, health_pack);
    if (resp != glm::vec2(0) && h_pack_spawned)  {
        h_pack_spawned = false;

        player_health += 5;
        if (player_health > 10)
            player_health = 10;
    }

    if (!h_pack_spawned)
        h_pack_timer += deltaTimeSeconds;

    if (h_pack_timer > 10) {
        h_pack_spawned = true;
        h_pack_timer = 0;
    }

    //Health Bar Render
    float aux_health_xpos = health_bar.xpos;
    health_bar.xscale = player_health / 10;
    health_bar.xpos -= health_bar.width / 2 * (1 - health_bar.xscale);
    health_bar.matrix = ComputeEntityMatrix(health_bar);
    RenderMesh2D(health_bar.mesh, shaders["VertexColor"], health_bar.matrix);
    health_bar.xpos = aux_health_xpos;

    //Experience Bar Render
    float aux_exp_xpos = exp_bar.xpos;
    exp_bar.xscale = player_exp / 10;
    exp_bar.xpos -= exp_bar.width / 2 * (1 - exp_bar.xscale);
    exp_bar.matrix = ComputeEntityMatrix(exp_bar);
    RenderMesh2D(exp_bar.mesh, shaders["VertexColor"], exp_bar.matrix);
    exp_bar.xpos = aux_exp_xpos;

    //Health Border Render
    health_border.matrix = ComputeEntityMatrix(health_border);
    RenderMesh2D(health_border.mesh, shaders["VertexColor"], health_border.matrix);

    //Experience Border Render
    exp_border.matrix = ComputeEntityMatrix(exp_border);
    RenderMesh2D(exp_border.mesh, shaders["VertexColor"], exp_border.matrix);

    //Player Render
    player.matrix = ComputeEntityMatrix(player);
    RenderMesh2D(player.mesh, shaders["VertexColor"], visMatrix * player.matrix);

    //Health Pack Render
    if (h_pack_spawned) {
        health_pack.matrix = ComputeEntityMatrix(health_pack);
        RenderMesh2D(health_pack.mesh, shaders["VertexColor"], visMatrix * health_pack.matrix);
    }

    //Enemies Spawn
    time_counter += deltaTimeSeconds; // counts how many seconds have passed
    projectile_time_counter += deltaTimeSeconds;
    if ((int) time_counter >= 5) {
        Entity new_enemy;
        time_counter = 0;

        new_enemy.mesh = enemy_mesh;
        new_enemy.xvel = 500;
        new_enemy.yvel = 500;
        new_enemy.radius = 50;
        new_enemy.xpos = 0;
        new_enemy.ypos = 0;
        new_enemy.ld_corner.x = -squareSide / 2;
        new_enemy.ld_corner.y = -squareSide / 2;
        new_enemy.width = squareSide / 2;
        new_enemy.height = squareSide / 2;

       

        enemies.push_back(new_enemy);
    }

    //Enemy Render
    for (Entity& curr_enemy : enemies) {
        float xgap = curr_enemy.xpos - player.xpos;
        float ygap = curr_enemy.ypos - player.ypos;

        glm::vec2 dir = glm::normalize(glm::vec2(xgap, ygap));

        curr_enemy.xpos -= 300 * dir.x * deltaTimeSeconds;
        curr_enemy.ypos -= 300 * dir.y * deltaTimeSeconds;

        //Enemy Rotation
        double currDeltaX = player.xpos - curr_enemy.xpos / 2;
        double currDeltaY = player.ypos - curr_enemy.ypos / 2;
        double dist = sqrt(currDeltaX * currDeltaX + currDeltaY * currDeltaY);
        double angle = acos(currDeltaX / dist);

        if (currDeltaY < 0) angle *= -1;
        curr_enemy.rotation = angle;

        curr_enemy.matrix = ComputeEntityMatrix(curr_enemy);

        RenderMesh2D(curr_enemy.mesh, shaders["VertexColor"], visMatrix * curr_enemy.matrix);

    }

    //Projectiles Render
    for (Entity &curr_projectile : projectiles) {
        curr_projectile.xpos += cos(curr_projectile.rotation - M_PI / 2) * deltaTimeSeconds * curr_projectile.xvel;
        curr_projectile.ypos += sin(curr_projectile.rotation - M_PI / 2) * deltaTimeSeconds * curr_projectile.yvel;

        curr_projectile.matrix = ComputeEntityMatrix(curr_projectile);

        RenderMesh2D(curr_projectile.mesh, shaders["VertexColor"], visMatrix * curr_projectile.matrix);
    }

    //Obstacles Render
    for (Entity& curr_obstacle : obstacles) {
        curr_obstacle.matrix = ComputeEntityMatrix(curr_obstacle);
        RenderMesh2D(curr_obstacle.mesh, shaders["VertexColor"], visMatrix * curr_obstacle.matrix);
    }

    //Map Base Render
    RenderMesh2D(map_base.mesh, shaders["VertexColor"], visMatrix * map_base.matrix);


    //Minimap
    logicSpace.x = -995;
    logicSpace.y = -995;
    logicSpace.width = 2000;
    logicSpace.height = 2000;

    viewSpace = ViewportSpace(resolution.x * 0.7, resolution.y * 0.7,
        resolution.y * 0.3, resolution.y * 0.3);
    SetViewportArea(viewSpace, glm::vec3(0), true);

    visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);

    RenderMesh2D(health_bar.mesh, shaders["VertexColor"], health_bar.matrix);

    RenderMesh2D(exp_bar.mesh, shaders["VertexColor"], exp_bar.matrix);

    RenderMesh2D(health_border.mesh, shaders["VertexColor"], health_border.matrix);

    RenderMesh2D(exp_border.mesh, shaders["VertexColor"], exp_border.matrix);

    RenderMesh2D(player.mesh, shaders["VertexColor"], visMatrix * player.matrix);

    if (h_pack_spawned) 
        RenderMesh2D(health_pack.mesh, shaders["VertexColor"], visMatrix * health_pack.matrix);

    for (Entity& curr_enemy : enemies)  
        RenderMesh2D(curr_enemy.mesh, shaders["VertexColor"], visMatrix * curr_enemy.matrix);

    for (Entity& curr_projectile : projectiles) 
        RenderMesh2D(curr_projectile.mesh, shaders["VertexColor"], visMatrix * curr_projectile.matrix);

    for (Entity& curr_obstacle : obstacles) 
        RenderMesh2D(curr_obstacle.mesh, shaders["VertexColor"], visMatrix * curr_obstacle.matrix);

    RenderMesh2D(map_base.mesh, shaders["VertexColor"], visMatrix* map_base.matrix);
}


void Tema1::FrameEnd() {}


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    glm::ivec2 resolution = window->GetResolution();

    prev_x = player.xpos;
    prev_y = player.ypos;

    if (window->KeyHold(GLFW_KEY_A)) {    
        if (moved_x)
            player.xpos -= 500 * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_W)) {
        if (moved_y)
            player.ypos += 500 * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        if (moved_y)
            player.ypos -= 500 * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_D)) {
        if (moved_x)
            player.xpos += 500 * deltaTime;
    }
    
}


void Tema1::OnKeyPress(int key, int mods)
{
  
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    //identi
    glm::ivec2 resolution = window->GetResolution();
    mouseY = resolution.y - mouseY;
   
    double currDeltaX = mouseX - resolution.x / 2;
    double currDeltaY = mouseY - resolution.y / 2;
    double dist = sqrt(currDeltaX * currDeltaX + currDeltaY * currDeltaY);
    double angle = acos(currDeltaX / dist);

    if (currDeltaY < 0)
        angle *= -1;
    
    player.rotation = angle + M_PI / 2;
}
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    if (window->MouseHold(GLFW_MOUSE_BUTTON_1)) {
        if (projectile_time_counter >= 0.5) {
            Entity projectile;

            projectile.mesh = projectile_mesh;
            projectile.xpos = player.xpos;
            projectile.ypos = player.ypos;
            projectile.xvel = 1000;
            projectile.yvel = 1000;
            projectile.radius = 8;
            projectile.rotation = player.rotation;
            projectiles.push_back(projectile);
            
            projectile_time_counter = 0;
        }
    }
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
