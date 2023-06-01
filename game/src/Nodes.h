#pragma once

// Placeholder class for AI data. Might pass an "AI world" (collection of all AI info) instead.
struct Agent;

class Node
{
public:
	Node(const Agent& self, const Agent& other) : mSelf(self), mOther(other) {}
	virtual Node* Evaluate() = 0;

private:
	const Agent& mSelf;
	const Agent& mOther;
};

class DecisionNode : public Node
{
public:
	DecisionNode(Agent& self, Agent& other) : Node(self, other) {}
	Node* yes = nullptr;
	Node* no = nullptr;
};

class ActionNode : public Node
{
public:
	ActionNode(Agent& self, Agent& other) : Node(self, other) {}
	Node* Evaluate() override { return nullptr; }
	// nullptr because an action node is a leaf!
};
