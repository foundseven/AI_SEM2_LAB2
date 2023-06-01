/*
#include <cstdio>
#include <cmath>

struct Agent
{
    float x;
    float y;
    float rotation;
};

struct Enemy
{
    float x;
    float y;
    float rotation;
    bool visible;
    bool audible;
};

// Common data and interface for all nodes
class Node
{
public:
    Node(Agent& self, Enemy& enemy) : mSelf(self), mEnemy(enemy) {}
    virtual Node* Evaluate() = 0;

protected:
    Enemy& mEnemy;
    Agent& mSelf;
};

///////////////////////////////////////////
// Stores a yes decision and a no decision
///////////////////////////////////////////
class DecisionNode : public Node
{
public:
    DecisionNode(Agent& self, Enemy& enemy) : Node(self, enemy) {}
    Node* yes = nullptr;
    Node* no = nullptr;
};

class VisibleNode : public DecisionNode
{
public:
    VisibleNode(Agent& self, Enemy& enemy) : DecisionNode(self, enemy) {}
    Node* Evaluate() final
    {
        //if (mEnemy.visible)
        //    return yes;
        //else
        //    return no;
        return mEnemy.visible ? yes : no;
    }
};

class AudibleNode : public DecisionNode
{
public:
    AudibleNode(Agent& self, Enemy& enemy) : DecisionNode(self, enemy) {}
    Node* Evaluate() final
    {
        return mEnemy.audible ? yes : no;
    }
};

class ProximityNode : public DecisionNode
{
public:
    ProximityNode(Agent& self, Enemy& enemy) : DecisionNode(self, enemy) {}
    Node* Evaluate() final
    {
        float dx = mEnemy.x - mSelf.x;
        float dy = mEnemy.y - mSelf.y;
        return sqrtf(dx * dx + dy * dy) <= 10.0f ? yes : no;
    }
};

class FlankNode : public DecisionNode
{
public:
    FlankNode(Agent& self, Enemy& enemy) : DecisionNode(self, enemy) {}
    Node* Evaluate() final
    {
        return fabsf(mEnemy.rotation - mSelf.rotation) > 90.0f ? yes : no;
    }
};

//////////////////////
// Performs an action
//////////////////////
class ActionNode : public Node
{
public:
    ActionNode(Agent& self, Enemy& enemy) : Node(self, enemy) {}
    Node* Evaluate() override { return nullptr; }
    // nullptr because an action node is a leaf!

protected:
    void Attack() { printf("Agent attacking enemy\n"); }
    void Move() { printf("Agent moving to enemy\n"); }
    void Creep() { printf("Agent creeping to enemy\n"); }
};

class AttackNode : public ActionNode
{
public:
    AttackNode(Agent& self, Enemy& enemy) : ActionNode(self, enemy) {}
    Node* Evaluate() final
    {
        Attack();
        return ActionNode::Evaluate();
    }
};

class MoveNode : public ActionNode
{
public:
    MoveNode(Agent& self, Enemy& enemy) : ActionNode(self, enemy) {}
    Node* Evaluate() final
    {
        Move();
        return ActionNode::Evaluate();
    }
};

class CreepNode : public ActionNode
{
public:
    CreepNode(Agent& self, Enemy& enemy) : ActionNode(self, enemy) {}
    Node* Evaluate() final
    {
        Creep();
        return ActionNode::Evaluate();
    }
};

void Traverse(Node* node)
{
    if (node != nullptr)
    {
        Traverse(node->Evaluate());
    }
}

int main()
{
    // Actors
    Agent agent;
    agent.x = 10.0f;
    agent.y = 10.0f;
    agent.rotation = 0.0f; // 50.0f
    Enemy enemy;
    enemy.x = 100.0f; // 0.0f
    enemy.y = 100.0f; // 0.0f
    enemy.rotation = 100.0f;
    enemy.visible = false; // false
    enemy.audible = true;

    // Decision nodes
    VisibleNode visible(agent, enemy);
    AudibleNode audible(agent, enemy);
    ProximityNode proximity(agent, enemy);
    FlankNode flank(agent, enemy);

    // Action nodes
    AttackNode attack(agent, enemy);
    MoveNode move(agent, enemy);
    CreepNode creep(agent, enemy);

    // Decision tree creation
    visible.no = &audible;
    audible.yes = &creep;
    visible.yes = &proximity;
    proximity.no = &flank;
    flank.no = &attack;
    flank.yes = &move;
    proximity.yes = &attack;

    Traverse(&visible);

    return 0;
}

//*/