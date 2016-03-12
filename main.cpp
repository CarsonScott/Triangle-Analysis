#include <iostream>
#include <SFML/Graphics.hpp>
#include <ctime>
#include <cstdlib>
#include <cmath>

#define PI 3.14159265359

sf::Vector2f getOffset(sf::Vector2f a, sf::Vector2f b)
{
    sf::Vector2f offset(b.x - a.x, b.y - a.y);
    return offset;
}

float getDistance(sf::Vector2f a, sf::Vector2f b)
{
    sf::Vector2f offset = getOffset(a, b);
    float distance = sqrt(pow(offset.x, 2) + pow(offset.y, 2));
    return distance;
}

float inDegrees(float radian)
{
    return radian * 180/PI;
}

class Triangle
{
    float area;
    std::vector<float> distances;
    std::vector<float> angles;

    void setDistances(std::vector<sf::Vector2f> points)
    {
        for(int i = 0; i < points.size(); i++)
        {
            for(int j = i; j < points.size(); j++)
            {
                if(j != i)
                {
                    float d = getDistance(points[i], points[j]);
                    distances.push_back(d);
                }
            }
        }
    }

    void setAngles()
    {
        float a = distances[0];
        float b = distances[1];
        float c = distances[2];

        float A = acos( (pow(b, 2) + pow(c, 2) - pow(a, 2)) / (2*b*c) );
        float B = acos( (pow(c, 2) + pow(a, 2) - pow(b, 2)) / (2*a*c) );
        float C = acos( (pow(b, 2) + pow(a, 2) - pow(c, 2)) / (2*a*b) );

        angles.push_back(A);
        angles.push_back(B);
        angles.push_back(C);
    }

public:
    Triangle(sf::Vector2f& a, sf::Vector2f& b, sf::Vector2f& c)
    {
        std::vector<sf::Vector2f> points;
        points.push_back(a);
        points.push_back(b);
        points.push_back(c);

        setDistances(points);
        setAngles();
    }

    Triangle(std::vector<sf::Vector2f>& points)
    {
        setDistances(points);
        setAngles();
    }

    float getPerimeter()
    {
        float perimeter = 0;
        for(int i = 0; i < distances.size(); i++)
        {
            perimeter += distances[i];
        }
        return perimeter;
    }

    float getArea()
    {
        float p = getPerimeter() / 2;

        float a = distances[0];
        float b = distances[1];
        float c = distances[2];

        return sqrt(p*(p-a)*(p-b)*(p-c));
    }

    float getAngle(int a)
    {
        if(a < 3)
        {
            return angles[a];
        }
    }
};

class PointSelecter
{
    std::vector<sf::Vector2f> points;
    bool plotting;
public:
    PointSelecter()
    {
        plotting = true;
    }

    void handleEvent(sf::Event& event, sf::Vector2f mousePos)
    {
        if(event.type == sf::Event::MouseButtonPressed)
        {
            points.push_back(mousePos);
            if(points.size() == 3)
            {
                plotting = false;
            }
        }
    }

    bool isPlotting()
    {
        return plotting;
    }

    std::vector<sf::Vector2f>& getPoints()
    {
        return points;
    }

    void reset()
    {
        points.clear();
        plotting = true;
    }
};

class Game
{
public:
    void main(sf::RenderWindow& window)
    {
        PointSelecter ps;
        std::vector<Triangle> triangles;

        sf::Clock clock;
        while (window.isOpen())
        {
            float dt = clock.restart().asSeconds();

            sf::Event event;
            while (window.pollEvent(event))
            {
                ps.handleEvent(event, sf::Vector2f(sf::Mouse::getPosition(window)));
                if (event.type == sf::Event::Closed)
                {
                    window.close();
                }
            }
            window.clear();

            if(!ps.isPlotting())
            {
                std::vector<sf::Vector2f> points = ps.getPoints();
                Triangle tri(points);
                triangles.push_back(tri);

                std::cout << "Area:\n-  " << tri.getArea();
                std::cout << "\nPerimeter:\n-  " << tri.getPerimeter();

                std::cout << "\nAngles: \n-  " << inDegrees(tri.getAngle(0));
                std::cout << "\n-  " << inDegrees(tri.getAngle(1));
                std::cout << "\n-  " << inDegrees(tri.getAngle(2)) << "\n\n";

                ps.reset();
            }

            window.display();
        }
    }
};

int main()
{
    srand(time(NULL));
    sf::RenderWindow window(sf::VideoMode(1200, 900), "");
    Game().main(window);
    return 0;
}
