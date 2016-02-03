#ifndef SFML_Raycast_Engine_BSPGenerator_h
#define SFML_Raycast_Engine_BSPGenerator_h

#include <iostream>
#include <vector>
#include "MapUtils.h"

#define BSP_MIN_SPLIT_SIZE 3
#define BSP_MIN_ROOM_SIZE 5

namespace sfray {

    enum BSPSplitDirection {
        BSP_None,
        BSP_Horizontal,
        BSP_Vertical,
        };

    class BSPNode {
    public:
        BSPSplitDirection split_direction=BSP_None;
        int x;
        int y;
        int width;
        int height;
        bool is_root=false;
        BSPNode *child_a=NULL;
        BSPNode *child_b=NULL;
        
        void connectChildren(std::vector<std::vector<int> >& mapArray){
            
            // traverse children
            if (child_a != NULL){
                child_a->connectChildren(mapArray);
            }
            if (child_b != NULL){
                child_b->connectChildren(mapArray);
            }
            
            // connect children
            if (child_a->child_a == NULL && child_b->child_a == NULL){
                
            }
        }
        
        void bake(std::vector<std::vector<int> >& mapArray,std::vector<RoomDim>& rooms){
            if (child_a == NULL || child_b == NULL){
                
                RoomDim thisroom;
                thisroom.x = x;
                thisroom.y = y;
                if (width > BSP_MIN_ROOM_SIZE){
                    thisroom.width =  BSP_MIN_ROOM_SIZE + rand() % abs(width - BSP_MIN_ROOM_SIZE);
                }
                else{
                    thisroom.width = width;
                }
                if (height > BSP_MIN_ROOM_SIZE){
                    thisroom.height = BSP_MIN_ROOM_SIZE + rand() % abs(height - BSP_MIN_ROOM_SIZE);
                }
                else{
                    thisroom.height = height;
                }
                rooms.push_back(thisroom);
                
                // bake
                int chance = (rand() % 3) + 1; // 1-3
                for (int xx = x-1; xx <= x + thisroom.width; ++xx){
                    for (int yy = y-1; yy <= y + thisroom.height; ++yy){
                        if (xx >= x && xx < x + thisroom.width && yy >= y && yy < y + thisroom.height){
                            mapArray[xx][yy] = MAP_TILE_FLOOR;
                        }
                        else{
                            // custom rooms
                            mapArray[xx][yy] = chance;
                        }
                    }
                }
            }
            else{
                // step into children
                child_a->bake(mapArray,rooms);
                child_b->bake(mapArray,rooms);
            }
        };
        
        bool split(int depth){
            
            if (depth <= 0){
                return false;
            }
            
            split_direction = (BSPSplitDirection)((rand() % 2) + 1);
            
            if (split_direction == BSP_Horizontal){
                if (height < BSP_MIN_ROOM_SIZE*3){
                    return false;
                }
                
                // create two child nodes
                //int splity = height / 3 + (rand() % (height / 3));
                int splity = std::max(BSP_MIN_ROOM_SIZE,height / 3) + rand() % (std::max(BSP_MIN_ROOM_SIZE,height / 3));
                
                child_a = new BSPNode();
                child_a->x = x;
                child_a->y = y;
                child_a->width = width;
                child_a->height = splity;
                
                child_b = new BSPNode();
                child_b->x = x;
                child_b->y = y + splity + 1;
                child_b->width = width;
                child_b->height = abs(height - splity - 1);
                
            }
            else if (split_direction == BSP_Vertical){
                if (width < BSP_MIN_ROOM_SIZE*3){
                    return false;
                }
                
                int splitx = width / 3 + (rand() % (width / 3));
                
                child_a = new BSPNode();
                child_a->x = x;
                child_a->y = y;
                child_a->width = splitx;
                child_a->height = height;
                
                child_b = new BSPNode();
                child_b->x = x + splitx + 1;
                child_b->y = y;
                child_b->width =  abs(width - splitx - 1);
                child_b->height = height;
                
            }
            
            child_a->split(depth-1);
            child_b->split(depth-1);
            
            return true;
        };
    };

    static GenerationResults makeBSP(const int width, const int height){
        
        GenerationResults results;
        
        for (int build_stress = 0; build_stress < 1; ++build_stress){
            
            while(true){
                bool room_size_fail = false;
                
                std::vector<std::vector<int> > mapArray;
                std::vector<RoomDim> rooms;
                
                // create empty map
                for (int x = 0; x < width; ++x){
                    mapArray.push_back(std::vector<int>());
                    for (int y = 0; y < height; ++y){
                        mapArray[x].push_back(MAP_TILE_WALL);
                    }
                }
                
                // create root BSPNode
                BSPNode *root = new BSPNode();
                root->is_root = true;
                root->x = 1;
                root->y = 1;
                root->width = width-2;
                root->height = height-2;
                
                // split root
                root->split(width / BSP_MIN_ROOM_SIZE);
                
                
                // bake
                root->bake(mapArray,rooms);
                
                for (unsigned int i = 0; i < rooms.size(); ++i){
                    RoomDim r = rooms[i];
                    if (r.width < BSP_MIN_ROOM_SIZE || r.height < BSP_MIN_ROOM_SIZE){
                        room_size_fail = true;
                    }
                }
                
                if (!room_size_fail){
                    // connect rooms
                    ConnectRoomsInOrder(mapArray, rooms);
                    
                    // make doors
                    //CreateDoors(mapArray, rooms);
                    
                    results.mapArray = mapArray;
                    results.rooms = rooms;
                    
                    break;
                }
            
                // else, start over
            }
        }
        
        
        
        return results;

    }
    
} /* end of namespace */

#endif /* defined(__SevenDRL__MapDungeon__) */
