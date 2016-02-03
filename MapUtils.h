//
//  MapUtils.h
//  SevenDRL
//
//  Created by Daniel Alexander on 9/27/13.
//  Copyright (c) 2013 danalexander. All rights reserved.
//

#ifndef SFML_Raycast_Engine_MapUtils_h
#define SFML_Raycast_Engine_MapUtils_h

#include <vector>
#include <queue>
#include <array>

#define GRIDS 3
#define MAP_SIZE 50
#define DOOR_CHANCE 35

#define MAP_TILE_FLOOR 0
#define MAP_TILE_WALL 1

namespace sfray {

    struct RoomDim {
        int x;
        int y;
        int width;
        int height;
    };

    struct GenerationResults {
        std::vector<std::vector<int> > mapArray;
        std::vector<RoomDim> rooms;
    };

    static void CreateDoors(std::vector<std::vector<int> >& mapArray, std::vector<RoomDim>& rooms){
        int width = mapArray.size();
        int height = mapArray[0].size();

        // make doors
        for (unsigned int ri = 0; ri < rooms.size(); ++ri){
            RoomDim room = rooms[ri];
            for (int x = room.x-1; x < room.x-1+room.width+1; ++x){
                int y = room.y-1;

                if (mapArray[x][y] == 1
                    && (x-1 >= 0 && mapArray[x-1][y] != 1)
                    && (x+1 < width && mapArray[x+1][y] != 1)){

                    int chance = rand() % 100;

                    if (chance <= DOOR_CHANCE){
                        mapArray[x][y] = 2;
                    }

                }

                y = room.y + room.height;
                if (mapArray[x][y] == 1
                    && (x-1 >= 0 && mapArray[x-1][y] != 1)
                    && (x+1 < width && mapArray[x+1][y] != 1)){

                    int chance = rand() % 100;

                    if (chance <= DOOR_CHANCE){
                        mapArray[x][y] = 2;
                    }

                }
            }

            for (int y = room.y-1; y < room.y + room.height+1; ++y){
                int x = room.x - 1;

                if (mapArray[x][y] == 1
                    && (y-1 >= 0 && mapArray[x][y-1] != 1)
                    && (y+1 < height && mapArray[x][y+1] != 1)){

                    int chance = rand() % 100;

                    if (chance <= DOOR_CHANCE){
                        mapArray[x][y] = 2;
                    }
                }

                x = room.x + room.width;
                if (mapArray[x][y] == 1
                    && (y-1 >= 0 && mapArray[x][y-1] != 1)
                    && (y+1 < height && mapArray[x][y+1] != 1)){

                    int chance = rand() % 100;

                    if (chance <= DOOR_CHANCE){
                        mapArray[x][y] = 2;
                    }

                }
            }
        }

    }

    static void ConnectRoomsInOrder(std::vector<std::vector<int> >& mapArray, std::vector<RoomDim>& rooms)
    {
        for (unsigned int i = 0; i < rooms.size()-1; ++i){
            // pick a random room from the queue

            RoomDim room =  rooms[i];
            RoomDim other_room = rooms[i+1];

            // connect the two rooms
            int x = room.x + room.width / 2;
            int y = room.y + room.height / 2;
            int x2 = other_room.x + other_room.width / 2;
            int y2 = other_room.y + other_room.height / 2;

            while (x != x2){
                if (x < x2){
                    ++x;
                }
                else{
                    --x;
                }
                mapArray[x][y] = MAP_TILE_FLOOR;

            }
            while (y != y2){
                if (y < y2){
                    ++y;
                }else{
                    --y;
                }
                mapArray[x][y] = MAP_TILE_FLOOR;
            }
        }
    }

} /* end of namespace */

#endif
