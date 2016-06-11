#pragma once

#include "imgui.h"

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <vector>
#include <algorithm>

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
	ConnectionType_ModuleOutput,
	ConnectionType_Color,
	ConnectionType_ColorGradient,
	ConnectionType_Float,
	ConnectionType_Bool,
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct ConnectionDesc
{
	const char* name;
	ConnectionType type;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct NodeType
{
	const char* name;
	ConnectionDesc inputConnections[MAX_CONNECTION_COUNT];
	ConnectionDesc outputConnections[MAX_CONNECTION_COUNT];
	ImColor color;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

static struct NodeType s_nodeTypes[] =
{
	/*
	Node creation template:
		{
			Node Name
			{
				{ Input Connection Name, Connection type },
			},
			{
				{ Output Connection Name, Connection type },
			},
			Node Color
		}
	*/
	{}
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum DragState
{
	DragState_Default,
	DragState_Hover,
	DragState_BeginDrag,
	DragState_Draging,
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

void ShowExampleAppCustomNodeGraph(bool* opened);