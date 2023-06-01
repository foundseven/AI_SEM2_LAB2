#include "rlImGui.h"
#include "imgui.h"
#include "raylib.h"
#include "math.h"
#include <vector>
#include <raylib.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

class Rigidbody
{
public:
    Vector2 position;
    Vector2 velocity;
    Vector2 mousePosition;

    Rigidbody(float x = 0.0f, float y = 0.0f, float vx = 0.0f, float vy = 0.0f)
        : position({ x, y }), velocity({ vx, vy }) {}
};

// Sprite class
class Sprite
{
public:
    Texture2D texture;
    Vector2 position;
    Rectangle bounds;

    void Draw()
    {
        DrawTextureEx(texture, position, 0.0f, 1.0f, WHITE);
    }
};

// Agent class
class Agent
{
public:
    Rigidbody rigidbody;
    Sprite sprite;
    float maxSpeed;
    float maxAcceleration;

    Agent(float x = 0.0f, float y = 0.0f, float vx = 0.0f, float vy = 0.0f)
        : rigidbody(x, y, vx, vy), maxSpeed(0.0f), maxAcceleration(0.0f) {}

};

Vector2 WraparoundScreen(Vector2 position)
{
    Vector2 outPosition =
    {
        fmodf(position.x + SCREEN_WIDTH , SCREEN_WIDTH),
        fmodf(position.y + SCREEN_HEIGHT, SCREEN_HEIGHT)
    };

    return outPosition;
}

void UpdateRigidbody(Rigidbody& rigidbody, float deltaTime)
{
    rigidbody.position.x += rigidbody.velocity.x * deltaTime;
    rigidbody.position.y += rigidbody.velocity.y * deltaTime;

}

std::vector<Agent> agents;

void UpdateAgents(float deltaTime)
{
    for (Agent& agent : agents)
    {
        UpdateRigidbody(agent.rigidbody, deltaTime);
    }
}

/////////////////////////////////////////////////////////////////////////

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sunshine");//drawing the screen
    SetTargetFPS(60); //target frame rate
    rlImGuiSetup(true); //sets up imgui
    const float dt = GetFrameTime(); // time between frmes

    bool collision = false;         // Collision detection
    bool pause = false;             // Movement pause
    bool useGUI = false;


    Vector2 position = { 100 , 100 }; //px
    Vector2 velocity = { 10 , 0 }; //px/s
    Vector2 acceleration = { 0 , 50 }; //px/s/s

    float speed = 500;
    float maxSpeed = 100;
    float maxAccel = 100;

    //calling in classes
    std::vector<Agent> agents;
    Agent agent;
    Rigidbody rigidbody; //for mine

    //calling on pos, vels and what-not
    agent.rigidbody.position = { 400, 200 };
    agent.rigidbody.velocity = { 10, 0 };
    agent.maxSpeed = 400.0f;
    agent.maxAcceleration = 800.0f;
    Rigidbody circleA = { SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2 }; //for stationary


    //background
    Texture2D background = LoadTexture("../game/assets/textures/background.png");
    background.width = 1280;
    background.height = 720;

    float scrollingBack = 0.0f;
        scrollingBack -= 0.1f;


    ///////////////////////////////////////////////////////////////////

    while (!WindowShouldClose())
    {
        const float deltaTime = GetFrameTime();
        BeginDrawing();
        ClearBackground(RAYWHITE);

        //to follow the mouse
        rigidbody.mousePosition = GetMousePosition(); //for mine


        if (IsKeyDown(KEY_GRAVE)) useGUI = !useGUI;
        if (useGUI)
        {
            rlImGuiBegin();

            ImGui::DragFloat2("Position", &(rigidbody.position.x), 0, SCREEN_WIDTH);
            ImGui::DragFloat2("Velocity", &(agent.rigidbody.velocity.x), -maxSpeed, maxSpeed);
            ImGui::DragFloat2("Acceleration", &(acceleration.x), 1, -maxAccel, maxAccel);

            rlImGuiEnd();
        }
       
        
       HideCursor();


        rigidbody.position = rigidbody.position + agent.rigidbody.velocity * deltaTime + acceleration * deltaTime * deltaTime * 0.5f;
        agent.rigidbody.velocity = agent.rigidbody.velocity + acceleration * deltaTime;

        if (IsMouseButtonUp(MOUSE_LEFT_BUTTON))
        {
            acceleration = Normalize(rigidbody.mousePosition - rigidbody.position) * speed - agent.rigidbody.velocity;
        }
        else
        {
            acceleration = Normalize(rigidbody.position - rigidbody.mousePosition) * speed - agent.rigidbody.velocity;
        }

        if (CheckCollisionCircles(circleA.position, 65, rigidbody.position, 65))
        {
            acceleration = Normalize(rigidbody.position - circleA.position) * speed - agent.rigidbody.velocity;
        }


        rigidbody.position = WraparoundScreen(rigidbody.position);

        //////////////////////////////////////////////////////////////////////////

        //for a background
        DrawTextureEx(background, (Vector2{ scrollingBack, 20 }), 0.0f, 2.0f, WHITE);
        DrawTextureEx(background, (Vector2{ background.width * 2 + scrollingBack, 20 }), 0.0f, 2.0f, WHITE);

        //drawing text onto the screen
        DrawText("LAB 2 + 3", 596, 30, 22, GREEN);
        DrawText(TextFormat("Mouse Position: %.2f, %.2f", rigidbody.mousePosition.x, rigidbody.mousePosition.y), 900, 30, 25, LIGHTGRAY);
        DrawText(TextFormat("Blue Circle Position: %.2f, %.2f", rigidbody.position.x, rigidbody.position.y), 900, 60, 25, LIGHTGRAY);

        DrawText(TextFormat("Blue Circle Velocity: %.2f, %.2f", agent.rigidbody.velocity.x, agent.rigidbody.velocity.y), 900, 90, 25, LIGHTGRAY);

        //drawing circles
        DrawCircleV(rigidbody.position, 50, DARKBLUE); //following that seeks and flees
        DrawCircleV(circleA.position, 50, DARKPURPLE); //stationary
        DrawCircleGradient(rigidbody.mousePosition.x, rigidbody.mousePosition.y, 50, GREEN, DARKGREEN); //my circle


        //for pointing lines between the moving and mine
        DrawLineV(rigidbody.position, rigidbody.position + agent.rigidbody.velocity, RED); //vel
        DrawLineV(rigidbody.position, rigidbody.position + acceleration, GREEN); //accel
        DrawLineV(rigidbody.position, rigidbody.position + (rigidbody.mousePosition - rigidbody.position) * 150, ORANGE); //the pos between the two


        //for pointing lines between the stationary and mine
        DrawLineV(circleA.position, circleA.position + (rigidbody.mousePosition - circleA.position) * 150, DARKPURPLE); //the pos between the stationary and mine

        DrawFPS(10, 30);

        EndDrawing();
    }
    rlImGuiShutdown();
    CloseWindow();
    return 0;
}