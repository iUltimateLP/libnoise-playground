#pragma once

#include "libs/ImGui/imgui.h"
#include <math.h>
#include <string.h>
#include <vector>
#include <algorithm>
#include <stdint.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define sizeof_array(t) (sizeof(t) / sizeof(t[0]))
const float NODE_SLOT_RADIUS = 5.0f;
const ImVec2 NODE_WINDOW_PADDING(8.0f, 8.0f);
#define MAX_CONNECTION_COUNT 32

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
static inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static uint32_t s_id = 0;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum ConnectionType
{
	ConnectionType_Module,
	ConnectionType_ColorGradient,
	ConnectionType_Output,
	ConnectionType_Test,
};

struct ConnectionDesc
{
	const char* name;
	ConnectionType type;
};

struct NodeType
{
	const char* name;
	ConnectionDesc inputConnections[MAX_CONNECTION_COUNT];
	ConnectionDesc outputConnections[MAX_CONNECTION_COUNT];
};

struct Connection
{
	ImVec2 pos;
	ConnectionDesc desc;

	inline Connection()
	{
		pos.x = pos.y = 0.0f;
		input = 0;
	}

	union {
		float v3[3];
		float v;
		int i;
	};

	struct Connection* input;
	std::vector<Connection*> output;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Node types are defined here

static struct NodeType s_nodeTypes[] = 
{
	{
		"TestNode",
		{
			{"Input 1", ConnectionType_Test},
			{"Input 2", ConnectionType_Test},
		},
		{
			{"Output", ConnectionType_Test},
		},
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Node
{
	ImVec2 pos;
	ImVec2 size;
	int id;
	const char* name;
	std::vector<Connection*> inputConnections;
	std::vector<Connection*> outputConnections;
};

enum DragState
{
	DragState_Default,
	DragState_Hover,
	DragState_BeginDrag,
	DragState_Dragging,
	DragState_Connect,
};

struct DragNode
{
	ImVec2 pos;
	Connection* con;
};

static DragNode s_dragNode;
static DragState s_dragState = DragState_Default;

static std::vector<Node*> s_nodes;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void setupConnections(std::vector<Connection*>& connections, ConnectionDesc* connectionDescs);

static Node* createNodeFromType(ImVec2 position, NodeType* nodeType);

Node* createNodeFromName(ImVec2 position, const char* name);

void drawHermite(ImDrawList* drawList, ImVec2 p1, ImVec2 p2, int steps);

static bool isConnectorHovered(Connection* c, ImVec2 offset);

static Connection* getHoveredConnector(ImVec2 offset, ImVec2* pos);

void updateDragging(ImVec2 offset);

static void displayNode(ImDrawList* drawList, ImVec2 offset, Node* node, int& selectedNode);

Node* findNodeByConnector(Connection* findCon);

void renderLines(ImDrawList* drawList, ImVec2 offset);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////