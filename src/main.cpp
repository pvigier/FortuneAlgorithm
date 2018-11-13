// STL
#include <iostream>
#include <vector>
#include <chrono>
#include <random>
// SFML
#include <SFML/Graphics.hpp>
// My includes
#include "FortuneAlgorithm.h"

constexpr float WINDOW_WIDTH = 600.0f;
constexpr float WINDOW_HEIGHT = 600.0f;
constexpr float POINT_RADIUS = 0.005f;

std::vector<Vector2> generatePoints(int nbPoints)
{
    uint64_t seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::cout << seed << std::endl;
    std::default_random_engine generator(seed);
    std::uniform_real_distribution<double> distribution (0.0, 1.0);

    std::vector<Vector2> points;
    for (int i = 0; i < nbPoints; ++i)
        points.push_back(Vector2{distribution(generator), distribution(generator)});

    return points;
}

void drawPoint(sf::RenderWindow& window, Vector2 point, sf::Color color)
{
    sf::CircleShape shape(POINT_RADIUS);
    shape.setPosition(sf::Vector2f(point.x - POINT_RADIUS, 1 - point.y - POINT_RADIUS));
    shape.setFillColor(color);
    window.draw(shape);
}

void drawEdge(sf::RenderWindow& window, Vector2 origin, Vector2 destination, sf::Color color)
{
    sf::Vertex line[] =
    {
        sf::Vertex(sf::Vector2f(origin.x, 1.0f - origin.y), color),
        sf::Vertex(sf::Vector2f(destination.x, 1.0f - destination.y), color)
    };
    window.draw(line, 2, sf::Lines);
}

void drawPoints(sf::RenderWindow& window, std::vector<Vector2> points)
{
    for (Vector2& point : points)
        drawPoint(window, point, sf::Color(100, 250, 50));
}

void drawDiagram(sf::RenderWindow& window, VoronoiDiagram& diagram, int nbSites)
{
    for (int i = 0; i < nbSites; ++i)
    {
        const VoronoiDiagram::Site* site = diagram.getSite(i);
        Vector2 center = site->point;
        VoronoiDiagram::Face* face = site->face;
        VoronoiDiagram::HalfEdge* halfEdge = face->outerComponent;
        while (halfEdge->prev != nullptr)
        {
            halfEdge = halfEdge->prev;
            if (halfEdge == face->outerComponent)
                break;
        }
        VoronoiDiagram::HalfEdge* start = halfEdge;
        while (halfEdge != nullptr)
        {
            if (halfEdge->origin != nullptr && halfEdge->destination != nullptr)
            {
                Vector2 origin = (halfEdge->origin->point - center) * 0.9 + center;
                Vector2 destination = (halfEdge->destination->point - center) * 0.9 + center;
                drawEdge(window, origin, destination, sf::Color::Red);
            }
            halfEdge = halfEdge->next;
            if (halfEdge == start)
                break;
        }
    }
}

int main()
{
    std::vector<Vector2> points = generatePoints(100);
    /*std::cout << "Points" << std::endl;
    for (const auto& point : points)
        std::cout << point << std::endl;*/
    FortuneAlgorithm algorithm(points);
    auto start = std::chrono::system_clock::now();
    algorithm.construct();
    auto duration = std::chrono::system_clock::now() - start;
    std::cout << "construction: " << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << "ms" << std::endl;
    Box box = {-10000.0, -10000.0, 10000.0, 10000.0};
    start = std::chrono::system_clock::now();
    algorithm.bound(box);
    duration = std::chrono::system_clock::now() - start;
    std::cout << "bounding: " << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << "ms" << std::endl;
    VoronoiDiagram diagram = algorithm.getDiagram();

    // Print vertices
    /*std::cout << "Vertices" << std::endl;
    const std::list<VoronoiDiagram::Vertex>& vertices = diagram.getVertices();
    for (const VoronoiDiagram::Vertex& vertex : vertices)
        std::cout << vertex.point << std::endl;*/

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Voronoi", sf::Style::Default, settings);
    //window.setView(sf::View(sf::FloatRect(box.left, box.bottom, box.right - box.left, box.top - box.bottom)));
    window.setView(sf::View(sf::FloatRect(0.0f, 0.0f, 1.0f, 1.0f)));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);

        drawDiagram(window, diagram, points.size());
        drawPoints(window, points);

        window.display();
    }

    return 0;
}
