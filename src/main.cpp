// STL
#include <iostream>
#include <vector>
#include <chrono>
#include <random>
// SFML
#include <SFML/Graphics.hpp>
// My includes
#include "FortuneAlgorithm.h"

constexpr double WINDOW_WIDTH = 600.0f;
constexpr double WINDOW_HEIGHT = 600.0f;
constexpr double POINT_RADIUS = 0.005f;

std::vector<Vector2> generatePoints(int nbPoints)
{
    uint64_t seed = std::chrono::system_clock::now().time_since_epoch().count();
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
                Vector2 origin = (halfEdge->origin->point - center) * 0.9f + center;
                Vector2 destination = (halfEdge->destination->point - center) * 0.9f + center;
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
    for (const auto& point : points)
        std::cout << point << std::endl;
    FortuneAlgorithm algorithm(points);
    algorithm.construct();
    VoronoiDiagram diagram = algorithm.getDiagram();

    // Print vertices
    const std::list<VoronoiDiagram::Vertex>& vertices = diagram.getVertices();
    for (const VoronoiDiagram::Vertex& vertex : vertices)
        std::cout << vertex.point << std::endl;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Voronoi", sf::Style::Default, settings);

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
