struct PVector
{
    float x;
    float y;

    PVector(float x=0, float y=0)
    {
        this->x = x;
        this->y = y;
    }

    void sub(sf::Vector2f v)
    {
        x -= v.x;
        y -= v.y;
    }

    void mult(float n)
    {
        x *= n;
        y *= n;
    }

    void normalize(float n)
    {
        if(abs(x) > n)
        {
            x = n * (x/abs(x));
        }
        if(abs(y) > n)
        {
            y = n * (y/abs(y));
        }
    }
};

class AdjustableView: public sf::View
{
    float adjust;
    sf::Vector2f magnification;
public:
    AdjustableView()
    {
        adjust = 0.2;
        magnification = sf::Vector2f(1, 1);
    }
    void handleEvent(sf::Event& event)
    {
        if(event.type == sf::Event::KeyPressed)
        {
            if(event.key.code == sf::Keyboard::S){
                setSize(getSize().x * 1.25, getSize().y * 1.25);
            }
            if(event.key.code == sf::Keyboard::W){
                setSize(getSize().x / 1.25, getSize().y / 1.25);
            }
        }
    }

    void update()
    {
        setSize(getSize().x + magnification.x, getSize().y + magnification.y);
    }
};

class Button: public sf::Text
{
    bool active;
public:
    Button()
    {
        active = false;
    }

    void handleEvent(sf::Event& event, sf::Vector2f mousePos)
    {
        if(event.type == sf::Event::MouseButtonPressed)
        {
            if(getGlobalBounds().contains(mousePos))
            {
                active = true;
            }
        }
    }

    bool selected()
    {
        return active;
    }

    void update()
    {
        active = false;
    }
};
