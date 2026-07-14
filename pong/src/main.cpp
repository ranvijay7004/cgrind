#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
using namespace sf;

int main()
{

    RenderWindow window(VideoMode(1024, 576), "MY FIRST GAME!");
    
    float ballspeed(0.8f);
    float paddlespeed(0.6f);
    
    CircleShape ball(20);
    ball.setFillColor(Color::Red);
    ball.setPosition(1024 / 2, 576 / 2);
    ball.setOutlineColor(Color::Green);
    ball.setOutlineThickness(1);
    ball.setScale(1.0f, 1.0f);
    float ballSpeed= ballspeed;
    float ballVelocityX = ballspeed;
    float ballVelocityY = ballspeed;

    RectangleShape paddle1(Vector2f(15, 80));
    paddle1.setFillColor(Color::White);
    paddle1.setPosition(20, 248);
    float Paddle1Speed= paddlespeed;

    RectangleShape paddle2(Vector2f(15, 80));
    paddle2.setFillColor(Color::White);
    paddle2.setPosition(984, 248);
    float Paddle2Speed= paddlespeed;

    Font font;
    font.loadFromFile("assets/fonts/LiberationSans-Regular.ttf");

    Text scoretext;
    scoretext.setFont(font);
    scoretext.setCharacterSize(40);
    scoretext.setFillColor(Color::White);
    scoretext.setPosition(462, 20);
    scoretext.setString("0:0");

    SoundBuffer paddlebuffer;
    paddlebuffer.loadFromFile("assets/audio/paddle.wav");
    Sound paddlesound;
    paddlesound.setBuffer(paddlebuffer);

    SoundBuffer scorebuffer;
    scorebuffer.loadFromFile("assets/audio/score.wav");
    Sound scoresound;
    scoresound.setBuffer(scorebuffer);


    int score1 = 0;                                                       
    int score2 = 0;

    while (window.isOpen())
    {

        Event event;

        while (window.pollEvent(event))
        {

            if (event.type == Event::Closed)
            {
                window.close();
            }
        }

        // paddles movement

        if (Keyboard::isKeyPressed(Keyboard::W))
        {
            paddle1.move(0, -Paddle1Speed);
        }

        if (Keyboard::isKeyPressed(Keyboard::S))
        {
            paddle1.move(0, Paddle1Speed);
        }

        if (Keyboard::isKeyPressed(Keyboard::Up))
        {
            paddle2.move(0, -Paddle2Speed);
        }

        if (Keyboard::isKeyPressed(Keyboard::Down))
        {
            paddle2.move(0, Paddle2Speed);
        }

        ball.move(ballVelocityX, ballVelocityY);
        Vector2f pos = ball.getPosition();

        Vector2f paddle1pos = paddle1.getPosition();
        Vector2f paddle2pos = paddle2.getPosition();

        // paddles collision with walls

        if (paddle1pos.y < 0)
        {
            paddle1.setPosition(paddle1pos.x, 0);
        }

        if (paddle1pos.y + 80 > 576)
        {
            paddle1.setPosition(paddle1pos.x, 576 - 80);
        }

        if (paddle2pos.y < 0)
        {
            paddle2.setPosition(paddle2pos.x, 0);
        }

        if (paddle2pos.y + 80 > 576)
        {
            paddle2.setPosition(paddle2pos.x, 576 - 80);
        }

        // "collision of balls with walls and paddles"

        if (pos.y <= 0)
        {
            ballVelocityY = -ballVelocityY;
            ball.setPosition(pos.x, 0);
        }

        if (pos.y + 40 >= 576)
        {
            ballVelocityY = -ballVelocityY;
            ball.setPosition(pos.x, 576 - 40);
        }

        if (pos.x <= 0)
        {
            score2++;
            scoretext.setString(std::to_string(score1) + ":" + std::to_string(score2));
            ball.setPosition(1024 / 2, 576 / 2);
            ballVelocityX = ballSpeed;
            ballVelocityY = ballSpeed;
            scoresound.play();
        }

        if (pos.x + 40 >= 1024)
        {
            score1++;
            scoretext.setString(std::to_string(score1) + ":" + std::to_string(score2));
            ball.setPosition(1024 / 2, 576 / 2);
            ballVelocityX = -ballSpeed;
            ballVelocityY = ballSpeed;
            scoresound.play();
        }

        if (score1 >= 10){
            scoretext.setString("Player 1 WINS!");
            scoretext.setFillColor(Color::Green);
            scoretext.setCharacterSize(60);
            scoretext.setPosition(250,250);
            window.clear(Color::Red);
            window.draw(scoretext);
            window.display();
            sleep(seconds(3));
            window.close();
        }

        if (score2 >= 10){
            scoretext.setString("Player 2 WINS!");
            scoretext.setFillColor(Color::Green);
            scoretext.setCharacterSize(60);
            scoretext.setPosition(250,250);
            window.clear(Color::Red);
            window.draw(scoretext);
            window.display();
            sleep(seconds(3));
            window.close();
        }

        if (ball.getGlobalBounds().intersects(paddle1.getGlobalBounds()))
        {
            ballVelocityX = -ballVelocityX;
            paddlesound.play();
        }

        if (ball.getGlobalBounds().intersects(paddle2.getGlobalBounds()))
        {
            ballVelocityX = -ballVelocityX;
            paddlesound.play();
        }
        
        window.clear(Color::Black);
        window.draw(scoretext);
        window.draw(paddle1);
        window.draw(paddle2);
        window.draw(ball);
        window.display();

        
    }
    return 0;
}
