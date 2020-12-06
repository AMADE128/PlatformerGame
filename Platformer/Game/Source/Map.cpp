
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "Collisions.h"
#include "Scene.h"

#include "Point.h"
#include "Defs.h"
#include "Log.h"
#include "Collider.h"
#include "Player.h"
#include "ModuleEnemies.h"

#include <math.h>

Map::Map() : Module(), mapLoaded(false)
{
    name.Create("map");
}

// Destructor
Map::~Map()
{}

int Properties::GetProperty(const char* value, int defaultValue) const
{
	ListItem<Property*>* item = list.start;

	while (item)
	{
		if (item->data->name == value)
			return item->data->value;
		item = item->next;
	}

	return defaultValue;
}

// Called before render is available
bool Map::Awake(pugi::xml_node& config)
{
    LOG("Loading Map Parser");
    bool ret = true;

    folder.Create(config.child("folder").child_value());

    return ret;
}

bool Map::Start()
{

	return true;
}

void Map::ResetPath(iPoint start)
{
	start = WorldToMap(start.x, start.y);
	frontier.Clear();
	visited.Clear();
	breadcrumbs.Clear();

	frontier.Push(start, 0);
	visited.Add(start);
	breadcrumbs.Add(start);

	memset(costSoFar, 0, sizeof(uint) * COST_MAP_SIZE * COST_MAP_SIZE);
}

void Map::DrawPath()
{
	iPoint point;

	// Draw visited
	ListItem<iPoint>* item = visited.start;

	while (item)
	{
		point = item->data;
		TileSet* tileset = GetTilesetFromTileId(26);

		SDL_Rect rec = tileset->GetTileRect(26);
		iPoint pos = MapToWorld(point.x, point.y);

		app->render->DrawTexture(tileset->texture, pos.x, pos.y, &rec);

		item = item->next;
	}

	// Draw frontier
	for (uint i = 0; i < frontier.Count(); ++i)
	{
		point = *(frontier.Peek(i));
		TileSet* tileset = GetTilesetFromTileId(25);

		SDL_Rect rec = tileset->GetTileRect(25);
		iPoint pos = MapToWorld(point.x, point.y);

		app->render->DrawTexture(tileset->texture, pos.x, pos.y, &rec);
	}

	// Draw path
	for (uint i = 0; i < path.Count(); ++i)
	{
		iPoint pos = MapToWorld(path[i].x, path[i].y);
	}
}

int Map::MovementCost(int x, int y) const
{
	int ret = -1;

 	if ((x >= 0) && (x < data.width) && (y >= 0) && (y < data.height))
	{
		int tileId = data.layers.end->data->Get(x, y); 

		if (tileId == 0) ret = 3;
		else ret = 0;
	}

	return ret;
}


void Map::ComputePath(int x, int y)
{
	path.Clear();
	iPoint goal = WorldToMap(x, y);

	uint i = breadcrumbs.Count() - (breadcrumbs.Count() - visited.Count() + 1);
	uint j = visited.Find(goal);

	path.PushBack(goal);

	while (j > 0)
	{
		if (visited[i] != breadcrumbs[j])
		{
			i--;
		}
		else if (visited[i] == breadcrumbs[j])
		{
			j = i;
			path.PushBack(visited[j]);
		}
	}

	/*while (breadcrumbs.At(i - 1)->data != visited.start->data)
	{
		if (visited.At(i) != breadcrumbs.At(j))
		{
			i--;
		}
		else if (visited.At(i) == breadcrumbs.At(j))
		{
			j = i;
			path.PushBack(visited[j]);
		}
	}*/
	path.Flip();

}


bool Map::IsWalkable(int x, int y) const
{

	MapLayer* layer = data.layers.start->data;
	int tileId = layer->Get(x, y);

	if (x > 0 && x < 1000 && y > 0 && y < 1000 && tileId == 0) 	return true;

	else return false;

}


void Map::PropagateDijkstra()
{

	TileDestiny.x = app->player->position.x;
	TileDestiny.y = app->player->position.y;
 	TileDestiny = WorldToMap(TileDestiny.x, TileDestiny.y);

	iPoint curr;
	curr = frontier.GetLast()->data;
	if (frontier.Pop(curr) && curr != TileDestiny)
	{
		iPoint neighbors[4];
		neighbors[0].Create(curr.x + 1, curr.y + 0);
		neighbors[1].Create(curr.x + 0, curr.y + 1);
		neighbors[2].Create(curr.x - 1, curr.y + 0);
		neighbors[3].Create(curr.x + 0, curr.y - 1);

		for (uint i = 0; i < 4; i++)
		{
			if (MovementCost(neighbors[i].x, neighbors[i].y) > 0)
			{
				if (visited.Find(neighbors[i]) == -1)
				{
					frontier.Push(neighbors[i], MovementCost(neighbors[i].x, neighbors[i].y));
					visited.Add(neighbors[i]);
					costSoFar[i][0] = MovementCost(neighbors[i].x, neighbors[i].y);
					breadcrumbs.Add(curr);

				}
			}
		}
	}
	else
	{
		breadcrumbs.Add(curr);
		ComputePath(app->player->position.x, app->player->position.y);
	}
	
	// L11: TODO 3: Taking BFS as a reference, implement the Dijkstra algorithm
	// use the 2 dimensional array "costSoFar" to track the accumulated costs
	// on each cell (is already reset to 0 automatically)
}

// Draw the map (all requried layers)
void Map::Draw()
{
	if (mapLoaded == false) return;

	// L04: DONE 5: Prepare the loop to draw all tilesets + DrawTexture()
	
	MapLayer* layer;
	TileSet* tileset;
	iPoint cord;

	for (ListItem<MapLayer*>* item = data.layers.start; item; item = item->next)
	{
		layer = item->data;
		for (int y = 0; y < data.height; ++y)
		{
			for (int x = 0; x < data.width; ++x)
			{
				int tileId = layer->Get(x, y);
				cord = MapToWorld(x, y);
				tileset = GetTilesetFromTileId(tileId);
				if (tileId > 0 && tileId < 260)
				{
					// L04: TODO 9: Complete the draw function
					app->render->DrawTexture(tileset->texture, cord.x, cord.y, &tileset->GetTileRect(tileId));
				}
			}
		}
	}
}

// L04: DONE 8: Create a method that translates x,y coordinates from map positions to world positions
iPoint Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	if (data.orientation == "orthogonal")
	{
		ret.x = x * data.tileWidth;
		ret.y = y * data.tileHeight;
	}
	else if (data.orientation == "isometric")
	{
		// L05: TODO 1: Add isometric map to world coordinates
		ret.x = (x - y) * (data.tileWidth * 0.5f);
		ret.y = (x + y) * (data.tileHeight * 0.5f);
	}

	return ret;
}

// L05: TODO 2: Add orthographic world to map coordinates
iPoint Map::WorldToMap(int x, int y) const
{
	iPoint ret(0, 0);

	// L05: TODO 3: Add the case for isometric maps to WorldToMap
	if (data.orientation == "orthogonal")
	{
		ret.x = x / data.tileWidth;
		ret.y = y / data.tileHeight;
	}
	else if (data.orientation == "isometric")
	{

		float half_width = data.tileWidth * 0.5f;
		float half_height = data.tileHeight * 0.5f;
		ret.x = int((x / half_width + y / half_height) / 2);
		ret.y = int((y / half_height - (x / half_width)) / 2);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

TileSet* Map::GetTilesetFromTileId(int id) const
{
	ListItem<TileSet*>* item = data.tilesets.start;
	TileSet* set = item->data;

	if (id < 17)
	{
		set = item->data;
	}
	else if (id > 16 && id < 259)
	{
		item = item->next;
		set = item->data;
	}
	else if (id == 259)
	{
		item = item->next;
		item = item->next;
		set = item->data;
	}
	else if (id > 259 && id < 269)
	{
		item = item->next;
		item = item->next;
		item = item->next;
		set = item->data;
	}
	else if (id > 268 && id < 278)
	{
		item = item->next;
		item = item->next;
		item = item->next;
		item = item->next;
		set = item->data;
	}
	else if (id > 277 && id < 287)
	{
		item = item->next;
		item = item->next;
		item = item->next;
		item = item->next;
		item = item->next;
		set = item->data;
	}
	else if (id > 286)
	{
		item = item->next;
		item = item->next;
		item = item->next;
		item = item->next;
		item = item->next;
		item = item->next;
		set = item->data;
	}

	return set;
}

// Get relative Tile rectangle
SDL_Rect TileSet::GetTileRect(int id) const
{
	SDL_Rect rect = { 0 };

	// L04: DONE 7: Get relative Tile rectangle
	int relativeId = id - firstgid;
	rect.w = tileWidth;
	rect.h = tileHeight;
	rect.x = margin + ((rect.w + spacing) * (relativeId % numTilesWidth));
	rect.y = margin + ((rect.h + spacing) * (relativeId / numTilesWidth));
	
	return rect;
}

// Called before quitting
bool Map::CleanUp()
{
	if (!active)
		return true;
    LOG("Unloading map");

    // L03: DONE 2: Make sure you clean up any memory allocated from tilesets/map
    // Remove all tilesets
	ListItem<TileSet*>* item;
	item = data.tilesets.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.Clear();

	// L04: DONE 2: clean up all layer data
	// Remove all layers
	ListItem<MapLayer*>* item2;
	item2 = data.layers.start;

	while (item2 != NULL)
	{
		RELEASE(item2->data);
		item2 = item2->next;
	}
	data.layers.Clear();

	// Clean up the pugui tree
	mapFile.reset();

	active = false;

    return true;
}

// Load new map
bool Map::Load(const char* filename)
{
    bool ret = true;
    SString tmp("%s%s", folder.GetString(), filename);

    pugi::xml_parse_result result = mapFile.load_file(tmp.GetString());

    if(result == NULL)
    {
        LOG("Could not load map xml file %s. pugi error: %s", filename, result.description());
        ret = false;
    }

	// Load general info
    if(ret == true)
    {
        // L03: DONE 3: Create and call a private function to load and fill all your map data
		ret = LoadMap();
	}

    // L03: DONE 4: Create and call a private function to load a tileset
    // remember to support more any number of tilesets!
	pugi::xml_node tileset;
	for (tileset = mapFile.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if (ret == true) ret = LoadTilesetDetails(tileset, set);

		if (ret == true) ret = LoadTilesetImage(tileset, set);

		data.tilesets.Add(set);
	}

	// L04: DONE 4: Iterate all layers and load each of them
	// Load layer info
	pugi::xml_node layer;
	for (layer = mapFile.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* lay = new MapLayer();

		ret = LoadLayer(layer, lay);

		if (ret == true)
			data.layers.Add(lay);
	}
    
    if(ret == true)
    {
        // L03: TODO 5: LOG all the data loaded iterate all tilesets and LOG everything

		LOG("Successfully parsed map XML file: %s", filename);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tileWidth, data.tileHeight);
		LOG("Tileset ----");
		LOG("name: %s firstgid: %d", mapFile.child("map").child("tileset").attribute("name").as_string(), mapFile.child("map").child("tileset").attribute("firstgid").as_int());
		LOG("tile width: %d tile height: %d", mapFile.child("map").child("tileset").attribute("tilewidth").as_int(), mapFile.child("map").child("tileset").attribute("tileheight").as_int());
		LOG("spacing: %d margin: %d", mapFile.child("map").child("tileset").attribute("spacing").as_int(), mapFile.child("map").child("tileset").attribute("margin").as_int());

		// L04: TODO 4: LOG the info for each loaded layer

    }

    mapLoaded = ret;

    return ret;
}

// L03: TODO: Load map general properties
bool Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = mapFile.child("map");

	if (map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		// L03: TODO: Load map general properties
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tileWidth = map.attribute("tilewidth").as_int();
		data.tileHeight = map.attribute("tileheight").as_int();
		data.orientation = map.attribute("orientation").as_string();
	}

	return ret;
}

// L03: TODO: Load Tileset attributes
bool Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	
	// L03: TODO: Load Tileset attributes
	set->name = tileset_node.attribute("name").as_string();
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	set->tileWidth = tileset_node.attribute("tilewidth").as_int();
	set->tileHeight = tileset_node.attribute("tileheight").as_int();

	return ret;
}

// L03: TODO: Load Tileset image
bool Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if (image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		// L03: TODO: Load Tileset image
		set->texture = app->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		set->texWidth = image.attribute("width").as_int();
		set->texHeight = image.attribute("height").as_int();
		set->numTilesWidth = set->texWidth / set->tileWidth;
		set->numTilesHeight = set->texHeight / set->tileHeight;
		set->offsetX = 0;
		set->offsetY = 0;
	}

	return ret;
}

// L04: TODO 3: Create the definition for a function that loads a single layer
bool Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;
		
	
	// L04: TODO 3: Load a single layer
	layer->name = node.attribute("name").as_string("");
	layer->width = node.attribute("width").as_int(0);
	layer->height = node.attribute("height").as_int(0);
	layer->visible = node.attribute("visible").as_int(0);

	int size = layer->width * layer->height;

	uint* pointer = new uint[size];
	memset(pointer, 0, size);
	layer->data = pointer;
	if (layer->data == NULL) {
		LOG("Error al crear el array de layer");
		ret = false;
	}
	else
	{
		pugi::xml_node tileNode = node.child("data").child("tile");
		int i = 0;
		for (pugi::xml_node tile = tileNode; tile && ret; tile = tile.next_sibling("tile"))
		{
			layer->data[i] = tile.attribute("gid").as_uint(0);
			i++;
		}
		i = 0;

	}

	return ret;
}
