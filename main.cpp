#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");

    sf::Font font;
    if (!font.loadFromFile("data/assets/fonts/Ubuntu-C.ttf"))
        return EXIT_FAILURE;

    sf::Text text("Hello SFML", font, 50);

    // Start the game loop
    while (window.isOpen()) {
        // Process events
        sf::Event event;
        while (window.pollEvent(event)) {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(text);
        window.display();
    }

    return EXIT_SUCCESS;
}
