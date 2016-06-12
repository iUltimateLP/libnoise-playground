#include "nodes.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Node types


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void setupConnections(std::vector<Connection*>& connections, ConnectionDesc* connectionDescs)
{
	for (int i = 0; i < MAX_CONNECTION_COUNT; ++i)
	{
		const ConnectionDesc& desc = connectionDescs[i];

		if (!desc.name)
			break;

		Connection* con = new Connection;
		con->desc = desc;

		connections.push_back(con);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static Node* createNodeFromType(ImVec2 pos, NodeType* nodeType)
{
	Node* node = new Node;
	node->id = s_id++;
	node->name = nodeType->name;

	ImVec2 titleSize = ImGui::CalcTextSize(node->name);

	titleSize.y *= 3;

	setupConnections(node->inputConnections, nodeType->inputConnections);
	setupConnections(node->outputConnections, nodeType->outputConnections);

	// Calculate the size needed for the whole box

	ImVec2 inputTextSize(0.0f, 0.0f);
	ImVec2 outputText(0.0f, 0.0f);

	for (Connection* c : node->inputConnections)
	{
		ImVec2 textSize = ImGui::CalcTextSize(c->desc.name);
		inputTextSize.x = std::max<float>(textSize.x, inputTextSize.x);

		c->pos = ImVec2(0.0f, titleSize.y + inputTextSize.y + textSize.y / 2.0f);

		inputTextSize.y += textSize.y;
		inputTextSize.y += 3.0f;		// size between text entries
	}

	inputTextSize.x += 40.0f;

	// max text size + 40 pixels in between

	float xStart = inputTextSize.x;

	// Calculate for the outputs

	for (Connection* c : node->outputConnections)
	{
		ImVec2 textSize = ImGui::CalcTextSize(c->desc.name);
		inputTextSize.x = std::max<float>(xStart + textSize.x, inputTextSize.x);
	}
	
	node->pos = pos;
	node->size.x = inputTextSize.x;
	node->size.y = inputTextSize.y + titleSize.y + std::max<float>(node->inputConnections.size(), node->outputConnections.size() * 10.0f);

	inputTextSize.y = 0.0f;

	// set the positions for the output nodes when we know where the place them

	for (Connection* c : node->outputConnections)
	{
		ImVec2 textSize = ImGui::CalcTextSize(c->desc.name);

		c->pos = ImVec2(node->size.x, titleSize.y + inputTextSize.y + textSize.y / 2.0f);

		inputTextSize.y += textSize.y;
		inputTextSize.y += 3.0f;		// size between text entries
	}

	// calculate the size of the node depending on nuber of connections

	return node;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Node* createNodeFromName(ImVec2 pos, const char* name)
{
	for (int i = 0; i < (int)sizeof_array(s_nodeTypes); ++i)
	{
		if (!strcmp(s_nodeTypes[i].name, name))
			return createNodeFromType(pos, &s_nodeTypes[i]);
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
static void saveNodes(const char* filename)
{
json_t* root = json_object();
json_t* nodes = json_array();
for (Node* node : s_nodes)
{
json_t* item = json_object();
json_object_set_new(item, "type", json_string(node->name));
json_object_set_new(item, "id", json_integer(node->id));
json_object_set_new(item, "pos", json_pack("{s:f, s:f}", "x",  node->pos.x, "y", node->pos.y));
json_array_append_new(nodes, item);
}
// save the nodes
json_object_set_new(root, "nodes", nodes);
if (json_dump_file(root, filename, JSON_INDENT(4) | JSON_PRESERVE_ORDER) != 0)
printf("JSON: Unable to open %s for write\n", filename);
}
*/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void drawHermite(ImDrawList* drawList, ImVec2 p1, ImVec2 p2, int STEPS, ImColor lineColor)
{
	ImVec2 t1 = ImVec2(+80.0f, 0.0f);
	ImVec2 t2 = ImVec2(+80.0f, 0.0f);

	for (int step = 0; step <= STEPS; step++)
	{
		float t = (float)step / (float)STEPS;
		float h1 = +2 * t*t*t - 3 * t*t + 1.0f;
		float h2 = -2 * t*t*t + 3 * t*t;
		float h3 = t*t*t - 2 * t*t + t;
		float h4 = t*t*t - t*t;
		drawList->PathLineTo(ImVec2(h1*p1.x + h2*p2.x + h3*t1.x + h4*t2.x, h1*p1.y + h2*p2.y + h3*t1.y + h4*t2.y));
	}

	drawList->PathStroke(lineColor, false, 3.0f);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static bool isConnectorHovered(Connection* c, ImVec2 offset)
{
	ImVec2 mousePos = ImGui::GetIO().MousePos;
	ImVec2 conPos = offset + c->pos;

	float xd = mousePos.x - conPos.x;
	float yd = mousePos.y - conPos.y;

	return ((xd * xd) + (yd *yd)) < (NODE_SLOT_RADIUS * NODE_SLOT_RADIUS);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static Connection* getHoveredConnector(ImVec2 offset, ImVec2* pos)
{
	for (Node* node : s_nodes)
	{
		ImVec2 nodePos = node->pos + offset;

		for (Connection* con : node->inputConnections)
		{
			if (isConnectorHovered(con, nodePos))
			{
				*pos = nodePos + con->pos;
				return con;
			}
		}

		for (Connection* con : node->outputConnections)
		{
			if (isConnectorHovered(con, nodePos))
			{
				*pos = nodePos + con->pos;
				return con;
			}
		}
	}

	s_dragNode.con = 0;
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void updateDragging(ImVec2 offset)
{
	switch (s_dragState)
	{
	case DragState_Default:
	{
		ImVec2 pos;
		Connection* con = getHoveredConnector(offset, &pos);

		if (con)
		{
			s_dragNode.con = con;
			s_dragNode.pos = pos;
			s_dragState = DragState_Hover;
			return;
		}

		break;
	}

	case DragState_Hover:
	{
		ImVec2 pos;
		Connection* con = getHoveredConnector(offset, &pos);

		// Make sure we are still hovering the same node

		if (con != s_dragNode.con)
		{
			s_dragNode.con = 0;
			s_dragState = DragState_Default;
			return;
		}

		if (ImGui::IsMouseClicked(0) && s_dragNode.con)
			s_dragState = DragState_Dragging;

		break;
	}

	case DragState_BeginDrag:
	{
		break;
	}

	case DragState_Dragging:
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();

		drawList->ChannelsSetCurrent(0); // Background

		drawHermite(drawList, s_dragNode.pos, ImGui::GetIO().MousePos, 12, VariableColors[s_dragNode.con->desc.type]);

		if (!ImGui::IsMouseDown(0))
		{
			ImVec2 pos;
			Connection* con = getHoveredConnector(offset, &pos);
			
			if (con == s_dragNode.con)
			{
				s_dragNode.con = 0;
				s_dragState = DragState_Default;
				return;
			}

			con->input = s_dragNode.con;
			s_dragNode.con = 0;
			s_dragState = DragState_Default;
		}

		break;
	}

	case DragState_Connect:
	{
		break;
	}
	}
}

static NodeType getNodeTypeByName(const char* name)
{
	for (int i = 0; i < (int)sizeof_array(s_nodeTypes); ++i)
	{
		if (!strcmp(s_nodeTypes[i].name, name))
			return s_nodeTypes[i];
	}

	return NodeType();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void displayNode(ImDrawList* drawList, ImVec2 offset, Node* node, int& node_selected)
{
	int node_hovered_in_scene = -1;
	bool open_context_menu = false;

	ImGui::PushID(node->id);
	ImVec2 node_rect_min = offset + node->pos;

	// Display node contents first
	drawList->ChannelsSetCurrent(1); // Foreground
	bool old_any_active = ImGui::IsAnyItemActive();

	// Draw title in center

	ImVec2 textSize = ImGui::CalcTextSize(node->name);

	ImVec2 pos = node_rect_min + NODE_WINDOW_PADDING;
	pos.x = node_rect_min.x + (node->size.x / 2) - textSize.x / 2;

	ImGui::SetCursorScreenPos(pos);
	//ImGui::BeginGroup(); // Lock horizontal position
	ImGui::Text("%s", node->name);
	//ImGui::SliderFloat("##value", &node->Value, 0.0f, 1.0f, "Alpha %.2f");
	//float dummy_color[3] = { node->Pos.x / ImGui::GetWindowWidth(), node->Pos.y / ImGui::GetWindowHeight(), fmodf((float)node->ID * 0.5f, 1.0f) };
	//ImGui::ColorEdit3("##color", &dummy_color[0]);

	// Save the size of what we have emitted and weither any of the widgets are being used
	bool node_widgets_active = (!old_any_active && ImGui::IsAnyItemActive());
	//node->size = ImGui::GetItemRectSize() + NODE_WINDOW_PADDING + NODE_WINDOW_PADDING;
	ImVec2 node_rect_max = node_rect_min + node->size;

	// Display node box
	drawList->ChannelsSetCurrent(0); // Background

	ImGui::SetCursorScreenPos(node_rect_min);
	ImGui::InvisibleButton("node", node->size);

	if (ImGui::IsItemHovered())
	{
		node_hovered_in_scene = node->id;
		open_context_menu |= ImGui::IsMouseClicked(1);
	}

	bool node_moving_active = false;

	if (ImGui::IsItemActive() && !s_dragNode.con)
		node_moving_active = true;

	ImU32 node_bg_color = node_hovered_in_scene == node->id ? ImColor(75, 75, 75) : ImColor(60, 60, 60);
	drawList->AddRectFilled(node_rect_min, node_rect_max, node_bg_color, 4.0f);

	ImVec2 titleArea = node_rect_max;
	titleArea.y = node_rect_min.y + 30.0f;

	// Draw text bg area
	drawList->AddRectFilled(node_rect_min + ImVec2(1, 1), titleArea, getNodeTypeByName(node->name).color, 4.0f);
	drawList->AddRect(node_rect_min, node_rect_max, ImColor(100, 100, 100), 4.0f);

	offset = node_rect_min;
	offset.y += 40.0f;

	for (Connection* con : node->inputConnections)
	{
		ImGui::SetCursorScreenPos(offset + ImVec2(10.0f, 0));
		ImGui::Text("%s", con->desc.name);

		ImColor conColor = ImColor(150, 150, 150);

		if (isConnectorHovered(con, node_rect_min))
			conColor = ImColor(200, 200, 200);

		drawList->AddCircleFilled(node_rect_min + con->pos, NODE_SLOT_RADIUS, conColor);

		offset.y += textSize.y + 2.0f;
	}

	offset = node_rect_min;
	offset.y += 40.0f;

	for (Connection* con : node->outputConnections)
	{
		textSize = ImGui::CalcTextSize(con->desc.name);

		ImGui::SetCursorScreenPos(offset + ImVec2(con->pos.x - (textSize.x + 10.0f), 0));
		ImGui::Text("%s", con->desc.name);

		ImColor conColor = ImColor(150, 150, 150);

		if (isConnectorHovered(con, node_rect_min))
			conColor = ImColor(200, 200, 200);

		drawList->AddCircleFilled(node_rect_min + con->pos, NODE_SLOT_RADIUS, conColor);

		offset.y += textSize.y + 2.0f;
	}


	//for (int i = 0; i < node->outputConnections.size(); ++i)
	//	drawList->AddCircleFilled(offset + node->outputSlotPos(i), NODE_SLOT_RADIUS, ImColor(150,150,150,150));

	if (node_widgets_active || node_moving_active)
		node_selected = node->id;

	if (node_moving_active && ImGui::IsMouseDragging(0))
		node->pos = node->pos + ImGui::GetIO().MouseDelta;

	//ImGui::EndGroup();

	ImGui::PopID();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TODO: Ugly fix: me

Node* findNodeByCon(Connection* findCon)
{
	for (Node* node : s_nodes)
	{
		for (Connection* con : node->inputConnections)
		{
			if (con == findCon)
				return node;
		}

		for (Connection* con : node->outputConnections)
		{
			if (con == findCon)
				return node;
		}
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void renderLines(ImDrawList* drawList, ImVec2 offset)
{
	for (Node* node : s_nodes)
	{
		for (Connection* con : node->inputConnections)
		{
			if (!con->input)
				continue;

			Node* targetNode = findNodeByCon(con->input);

			if (!targetNode)
				continue;

			drawHermite(drawList,
				offset + targetNode->pos + con->input->pos,
				offset + node->pos + con->pos,
				12, VariableColors[con->input->desc.type]);

			// offset + targetNode->pos + con->input->pos,
			// offset + node->pos + con->pos,
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ShowNodeGraph(bool* bOpen)
{
	ImGui::SetNextWindowSize(ImVec2(700, 600), ImGuiSetCond_FirstUseEver);
	if (!ImGui::Begin("Graph Editor", bOpen))
	{
		ImGui::End();
		return;
	}


	bool bContextMenuOpen = false;
	int hoveredNode_List = -1;
	int hoveredNode_Scene = -1;

	static int selectedNode = -1;
	static ImVec2 scrolling = ImVec2(0.0f, 0.0f);

	ImGui::SameLine();
	ImGui::BeginGroup();

	// Create our child canvas
	//ImGui::Text("Hold middle mouse button to scroll (%.2f,%.2f)", scrolling.x, scrolling.y);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1, 1));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImColor(40, 40, 40, 200));
	ImGui::BeginChild("scrolling_region", ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);
	ImGui::PushItemWidth(120.0f);

	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->ChannelsSplit(2);
	//ImVec2 offset = ImGui::GetCursorScreenPos() - scrolling;

	for (Node* node : s_nodes)
		displayNode(draw_list, scrolling, node, selectedNode);

	updateDragging(scrolling);
	renderLines(draw_list, scrolling);

	draw_list->ChannelsMerge();

	// Open context menu
	if (!ImGui::IsAnyItemHovered() && ImGui::IsMouseHoveringWindow() && ImGui::IsMouseClicked(1))
	{
		selectedNode = hoveredNode_List = hoveredNode_Scene = -1;
		bContextMenuOpen = true;
	}
	if (bContextMenuOpen)
	{
		ImGui::OpenPopup("context_menu");
		if (hoveredNode_List != -1)
			selectedNode = hoveredNode_List;
		if (hoveredNode_Scene != -1)
			selectedNode = hoveredNode_Scene;
	}

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
	if (ImGui::BeginPopup("context_menu"))
	{
		for (int i = 0; i < (int)sizeof_array(s_nodeTypes); ++i)
		{
			if (ImGui::MenuItem(s_nodeTypes[i].name))
			{
				Node* node = createNodeFromType(ImGui::GetIO().MousePos, &s_nodeTypes[i]);
				s_nodes.push_back(node);
			}
		}

		ImGui::EndPopup();
	}
	ImGui::PopStyleVar();

	// Scrolling
	if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemActive() && ImGui::IsMouseDragging(2, 0.0f))
	{
		scrolling = scrolling + ImGui::GetIO().MouseDelta;
	}

	ImGui::PopItemWidth();
	ImGui::EndChild();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar(2);
	ImGui::EndGroup();


	// Window Moving
	if (ImGui::IsMouseDragging(0, 0.0f) && ImGui::IsMouseHoveringWindow())
	{
		scrolling = scrolling + ImGui::GetIO().MouseDelta;
	}

	ImGui::End();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////