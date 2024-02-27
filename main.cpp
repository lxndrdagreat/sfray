/* SFML Raycast Engine
 *
 * Example Program
 *
 * by Dan Alexander
 */
#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>

// Here is a small helper for you ! Have a look.
#include "ResourcePath.hpp"
#include "Entity.hpp"
#include "Raycaster.hpp"
#include "Map.hpp"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

int main(int, char const**)
{
    // This example map is a large room with the different features
    // showcased inside of it.
    // It's dimensions are 20x20.
    unsigned int mapWidth = 20;
    unsigned int mapHeight = 20;
    std::vector<int> basicLargeRoom{
             1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
             1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
             1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 1,
             1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
             1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
             1, 0, 0, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
             1, 0, 0, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
             1, 0, 0, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
             1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
             1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
             1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
             1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
             1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
             1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
             1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 1,
             1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 1,
             1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 1,
             1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 1,
             1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
             1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    };

    // This map is for testing the drawing distance
    std::vector<std::vector<int>> hugeSpace;
    for (int x = 0; x < 50; ++x){
        hugeSpace.push_back(std::vector<int>());
        for (int y = 0; y < 50; ++y){
            if (x == 0 || x == 49){
                hugeSpace[x].push_back(1);
            }
            else if (y == 0 || y == 49){
                hugeSpace[x].push_back(2);
            }
            else{
                hugeSpace[x].push_back(0);
            }
        }
    }

    // Create the actual map object that the renderer uses
    sfray::Map worldMap;

    // Load textures into the map object
    worldMap.loadTexture(1, resourcePath() + "wall1.png");  // 128x128 version
    worldMap.loadTexture(2, resourcePath() + "wall2.png");  // 128x128 version
    worldMap.loadTexture(3, resourcePath() + "wall3.png");  // 128x128 version
    worldMap.loadTexture(0, resourcePath() + "floor.png");  // 128x128 version
    worldMap.loadTexture(-1, resourcePath() + "floor2.png");  // 128x128 version
    worldMap.loadTexture(4, resourcePath() + "pillar.png");
    worldMap.loadTexture(5, resourcePath() + "box.png");

    // load in the map data from above
    worldMap.setDataFromIntArray(basicLargeRoom, mapWidth, mapHeight);

    // Some example entities for testing
    for (unsigned int i = 0; i < 10; ++i){
        sfray::Entity* ent = new sfray::Entity();
        ent->setTextureUID((rand() % 2 == 0 ? 4 : 5));
        int x = 1 + rand() % (worldMap.getWidth() - 2);
        int y = 1 + rand() % (worldMap.getHeight() - 2);
        while(!worldMap.getTile(x, y).isFloor()){
            x = 1 + rand() % (worldMap.getWidth() - 2);
            y = 1 + rand() % (worldMap.getHeight() - 2);
        }
        ent->setPosition(x, y);
        worldMap.addEntity(ent);
    }

    // Create the raycaster instance
    sfray::Raycaster raycaster;

    // Set the map for the raycaster
    raycaster.setMap(worldMap);

    // Let's make the floor and ceiling slightly different colors
    raycaster.setFloorRenderColor(sf::Color(50, 50, 50));
    raycaster.setCeilingRenderColor(sf::Color(80, 80, 80));

    // Max Object Render Distance
    // Change this to change how close entities have to be
    // to the camera in order to be drawn to the screen.
    // The default is an absurdly high number, since there
    // is not a major performance hit drawing the sprites.
    // However, they do not really look good at a far distance,
    // so you will probably want to change this to meet your needs.
    raycaster.setMaxObjectRenderDistance(100);

    // Set the raycaster engine's drawing size
    raycaster.setSize(640, 480);

    // Set up the camera
    sfray::Camera camera;
    camera.setPosition(2, 2);
    camera.setDirectionInDegrees(0);

    // Create the main window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML Raycast Engine Example");

    // GAME TIME CLOCK
    sf::Clock clock;

    float fps = 0.0f;
    float fps_timer = 0.0f;

    // Framerate counter TEXT OBJECT
    sf::Font font;
    if (!font.loadFromFile(resourcePath() + "sansation.ttf")){
        throw "Could not load font";
    }
    sf::Text fps_text;
    fps_text.setFont(font);
    fps_text.setCharacterSize(32);
    fps_text.setFillColor(sf::Color::Yellow);
    fps_text.setPosition(10, 10);

    sf::Texture overlayTexture;
    overlayTexture.loadFromFile(resourcePath() + "overlay.png");
    sf::Sprite overlaySprite(overlayTexture);
    overlaySprite.setOrigin(0.0f, overlayTexture.getSize().y);
    overlaySprite.setScale(2.0f, 2.0f);
    overlaySprite.setPosition(WINDOW_WIDTH * 0.65f, WINDOW_HEIGHT + 100);
    bool drawOverlay = false;

    // Start the game loop. This is the normal basics used in SFML.
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Escape pressed : exit
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close();
            }

            // render settings for debug
            // Z: Change floor render method
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Z){
                int current = raycaster.getFloorRenderMethod();
                current += 1;
                if (current > 2){
                    current = 0;
                }
                raycaster.setFloorRenderMethod((sfray::FloorRenderMethod)current);
            }
            // X: Change ceiling render method
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::X){
                int current = raycaster.getCeilingRenderMethod();
                current += 1;
                if (current > 2){
                    current = 0;
                }
                raycaster.setCeilingRenderMethod((sfray::CeilingRenderMethod)current);
            }
            // C: Change wall render method
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::C){
                int current = raycaster.getWallRenderMethod();
                current += 1;
                if (current > 2){
                    current = 0;
                }
                raycaster.setWallRenderMethod((sfray::WallRenderMethod)current);
            }
            // V: change entity render method
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::V){
                int current = raycaster.getEntityRenderMethod();
                current += 1;
                if (current > 1){
                    current = 0;
                }
                raycaster.setEntityRenderMethod((sfray::EntityRenderMethod)current);
            }
            // B: toggle overlay render on/off
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::B){
                drawOverlay = !drawOverlay;
            }
        }

        double delta = clock.restart().asSeconds();
        fps = 1.0f / delta;

        double rotSpeed = 3.0 * delta;
        float degreesToRotate = 128.0f * delta;
        double moveSpeed = 5.0 * delta;
        double moveCheck = 0.5;

        camera.moved = true;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
            camera.moved = true;
            // rotate camera left
            camera.rotateByDegrees(degreesToRotate);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
            camera.moved = true;
            // rotate camera right
            camera.rotateByDegrees(-degreesToRotate);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
            // move camera forwards
            camera.moved = true;
            sf::Vector2f pos = camera.getPosition();
            if (worldMap.getTile(int(camera.getPosition().x + camera.getDirection().x * moveCheck),
                                 int(camera.getPosition().y)).isWall() == false){
                pos.x += camera.getDirection().x * moveSpeed;
            }
            if (worldMap.getTile(int(camera.getPosition().x),
                                 int(camera.getPosition().y + camera.getDirection().y * moveCheck)).isWall() == false){
                pos.y += camera.getDirection().y * moveSpeed;
            }
            camera.setPosition(pos);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
            // move camera backwards
            camera.moved = true;
            sf::Vector2f pos = camera.getPosition();
            if (worldMap.getTile(int(camera.getPosition().x - camera.getDirection().x * moveCheck),
                                 int(camera.getPosition().y)).isWall() == false){
                pos.x -= camera.getDirection().x * moveSpeed;
            }
            if (worldMap.getTile(int(camera.getPosition().x),
                                 int(camera.getPosition().y - camera.getDirection().y * moveCheck)).isWall() == false){
                pos.y -= camera.getDirection().y * moveSpeed;
            }
            camera.setPosition(pos);
        }

        // DRAWING BEGINS HERE

        // Clear the screen
        window.clear();

        // Have the raycasting engine draw for our camera
        raycaster.drawForCamera(window, camera);

        // RESET THE VIEW
        window.setView(window.getDefaultView());

        // DRAW THE OVERLAY
        if (drawOverlay)
        {
            window.draw(overlaySprite);
        }


        // Draw the framerate counter
        fps_timer -= delta;
        if (fps_timer <= 0.0f){
            fps_timer = 1.0f;
            fps_text.setString(std::to_string((int)fps));
        }
        window.draw(fps_text);

        // Update the window
        window.display();
    }

    return EXIT_SUCCESS;
}


