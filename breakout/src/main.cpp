#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <cmath>

using namespace sf;
using namespace std;

const int WIDTH = 800;
const int HEIGHT = 600;

const int BRICK_ROW=4;
const int BRICK_COL=10;
const int BRICK_HEIGHT=28;
const int BRICK_WIDTH=76;
const int BRICK_PADDING=2; 
const int BRICK_OFFSET_TOP=60;
const int BRICK_OFFSET_LEFT=11;       

int main(){

    RenderWindow window (VideoMode(WIDTH, HEIGHT), "Breakout");
    window.setFramerateLimit(60);

    Font font;
    font.loadFromFile("assets/fonts/LiberationSans-Regular.ttf");

    Texture backgroundtexture;
    backgroundtexture.loadFromFile("assets/images/background.png");
    Sprite backgroundsprite;
    backgroundsprite.setTexture(backgroundtexture);

    Texture paddletexture;
    paddletexture.loadFromFile("assets/images/paddle.png");
    Sprite paddlesprite;
    paddlesprite.setTexture(paddletexture);
    paddlesprite.setPosition(350,550);

    Texture balltexture;
    balltexture.loadFromFile("assets/images/ball.png");
    Sprite ballsprite;
    ballsprite.setTexture(balltexture);
    ballsprite.setPosition(392,530);

    Texture brickt[4];
    brickt[0].loadFromFile("assets/images/brick_blue.png");
    brickt[1].loadFromFile("assets/images/brick_green.png");
    brickt[2].loadFromFile("assets/images/brick_red.png");
    brickt[3].loadFromFile("assets/images/brick_yellow.png");
    Sprite bricksprite;

    SoundBuffer hitbuffer;
    hitbuffer.loadFromFile("assets/audio/hit.wav");
    Sound hitsound;
    hitsound.setBuffer(hitbuffer);

    SoundBuffer brickbuffer;
    brickbuffer.loadFromFile("assets/audio/brick.wav");
    Sound bricksound;
    bricksound.setBuffer(brickbuffer);

    SoundBuffer winbuffer;
    winbuffer.loadFromFile("assets/audio/winfretless.wav");
    Sound winsound;
    winsound.setBuffer(winbuffer);

    SoundBuffer overbuffer;
    overbuffer.loadFromFile("assets/audio/gameover.wav");
    Sound oversound;
    oversound.setBuffer(overbuffer);

    Music backgroundmusic;
    backgroundmusic.openFromFile("assets/audio/Red Curtain.ogg");
    backgroundmusic.setLoop(true);
    backgroundmusic.setVolume(50);
    backgroundmusic.play();

    float ballSpeed = 10.0f;
    int dirX = 1;
    int dirY = -1;

    float paddlespeed= 12.0f;

    int score=0;
    int lives=3;
    int highscore=0;
    int brickdestroyed= 0;

    bool flashing;
    int flashcount;
    Clock flashclock;

    bool gameOver=false;
    bool gameWon=false;
    bool ballLaunched=false;
    bool bricks [BRICK_ROW] [BRICK_COL];

    for(int r = 0; r < BRICK_ROW ; r++){
        for(int c = 0; c < BRICK_COL ; c++){
            bricks [r][c] = true;
        }
    }

    Text scoretext;
    scoretext.setFont(font);
    scoretext.setCharacterSize(20);
    scoretext.setPosition(10,10);
    scoretext.setFillColor(Color::White);
    scoretext.setString("score: 0");

    Text livestext;
    livestext.setCharacterSize(20);
    livestext.setFont(font);
    livestext.setPosition(350,10);
    livestext.setFillColor(Color::Red);
    livestext.setString("lives: 3");

    Text messagetext;
    messagetext.setFont(font);
    messagetext.setFillColor(Color::White);
    messagetext.setCharacterSize(40);
    messagetext.setPosition(200,250);
    messagetext.setString("Press space to launch");

    Text gameOvertext;
    gameOvertext.setFont(font);
    gameOvertext.setCharacterSize(60);
    gameOvertext.setFillColor(Color::Red);
    gameOvertext.setPosition(150,250);
    gameOvertext.setString("GAME OVER!");

    Text gameWontext;
    gameWontext.setFont(font);
    gameWontext.setCharacterSize(60);
    gameWontext.setFillColor(Color::Green);
    gameWontext.setPosition(150,250);
    gameWontext.setString("YOU WON!!");

    Text restarttext;
    restarttext.setFont(font);
    restarttext.setCharacterSize(25);
    restarttext.setFillColor(Color::White);
    restarttext.setPosition(220,330);
    restarttext.setString("Press 'R' to restart");

    Text highscoretext;
    highscoretext.setFont(font);
    highscoretext.setFillColor(Color::Yellow);
    highscoretext.setCharacterSize(20);
    highscoretext.setPosition(600,10);
    highscoretext.setString("Highscore: ");

    while(window.isOpen()){

        Event event;

        while(window.pollEvent(event)){
            if(event.type==Event::Closed){
                window.close();
            }

            if(event.type==Event::KeyPressed){
                
                if(event.key.code==Keyboard::Space && !ballLaunched && !gameOver && !gameWon){
                     ballLaunched=true;
                     messagetext.setString("");
                     hitsound.play();
                }
                if(event.key.code==Keyboard::R && (gameOver || gameWon)){
                    ballsprite.setPosition(392,530);
                    ballLaunched=false;
                    dirX = 1;
                    dirY = -1;

                    paddlesprite.setPosition(350,550);

                    lives = 3;
                    score = 0;
                    flashcount = 0;
                    ballSpeed = 10.0f;
                    brickdestroyed = 0;

                    scoretext.setString("Score: 0");
                    livestext.setString("Lives: 3");

                    for(int r = 0; r < BRICK_ROW ; r++){
                        for(int c= 0 ; c < BRICK_COL ; c++){
                            bricks[r][c]= true;
                        }
                    }

                    gameOver=false;
                    gameWon=false;
                    backgroundmusic.play();
                
                    messagetext.setString("Press Space to launch");

                }
                
            }
        }
       
        if(!gameOver && !gameWon){

            if(Keyboard::isKeyPressed(Keyboard::Left)){
                if(paddlesprite.getPosition().x > 0){
                    paddlesprite.move(-paddlespeed, 0);
                }
            }

            if(Keyboard::isKeyPressed(Keyboard::Right)){
                if(paddlesprite.getPosition().x +100 < WIDTH){
                    paddlesprite.move(paddlespeed , 0);
                }
            }

        if(!ballLaunched){
            ballsprite.setPosition(paddlesprite.getPosition().x + 42 , 530);
        }

        if(ballLaunched){
            float len = (dirX == 0) ? 1.0f : sqrt(2.0f);
            ballsprite.move(ballSpeed * dirX / len, ballSpeed * dirY / len);
        

         Vector2f pos = ballsprite.getPosition();

         if(ballsprite.getPosition().x < 0){
            dirX = 1;
            ballsprite.setPosition(0,ballsprite.getPosition().y);
         }

         if(pos.x + 15 > WIDTH){
            dirX = -1;
            ballsprite.setPosition(WIDTH - 15 , pos.y);
         }

         if(pos.y < 0){
            dirY = 1;
            ballsprite.setPosition(pos.x , 0);
         }

         if(pos.y + 15 > HEIGHT){
            lives --;
            livestext.setString("Lives: " + to_string(lives));

            if(lives<= 0){
                gameOver=true;
                backgroundmusic.stop();
                oversound.play();
                flashing=true;
                flashcount=0;
            }

            else{
            ballsprite.setPosition(392,530);
            paddlesprite.setPosition(350,550);
            ballLaunched=false;
            messagetext.setString("Press Space to launch");
            dirX = 1;
            dirY = -1;
            }
         }

         if(ballsprite.getGlobalBounds().intersects(paddlesprite.getGlobalBounds())){
            dirY = -1;
            hitsound.play();
    
            float ballCenter = ballsprite.getPosition().x + 7;
            float paddleCenter = paddlesprite.getPosition().x + 50;
            float diff = ballCenter - paddleCenter;

            if(diff > 10) dirX = 1;
            else if(diff < -10) dirX = -1;
            else dirX=0;
         }
            
         for(int row=0 ; row < BRICK_ROW ; row++){
            for(int col= 0 ; col < BRICK_COL ; col++){
               
                if(bricks[row][col]){

                    float brickX = BRICK_OFFSET_LEFT + col*(BRICK_WIDTH + BRICK_PADDING);
                    float brickY = BRICK_OFFSET_TOP + row*(BRICK_HEIGHT + BRICK_PADDING);

                    FloatRect brickRect(brickX, brickY, BRICK_WIDTH , BRICK_HEIGHT);

                    if(ballsprite.getGlobalBounds().intersects(brickRect)){
                        score += 10;
                        brickdestroyed ++ ;
                        bricksound.play();
                        bricks [row] [col] = false;
                        scoretext.setString("Score: " + to_string(score));
                        dirY = -dirY;

                        if(score>highscore){
                            highscore=score;
                            highscoretext.setString("Highscore: " + to_string(score));
                        }

                        if(brickdestroyed % 10 == 0){
                            ballSpeed += 0.5f;
                            if(ballSpeed >= 12.0f){
                                ballSpeed=12.0f;
                            }

                        }
                    }
                }
            }
         }

         bool alldestroyed=true;
         
         for(int row= 0; row < BRICK_ROW; row++){
            for(int col= 0 ; col < BRICK_COL ; col++){
                 if(bricks[row][col]){
                    alldestroyed= false;
                 }
            } 
         }

         if(alldestroyed){
            gameWon=true;
            backgroundmusic.stop();
            winsound.play();
         }

       }
    }

       window.clear(Color::Black);
       window.draw(backgroundsprite);

       for(int row= 0; row < BRICK_ROW ; row++){
           for(int col= 0; col <BRICK_COL ; col++){
               if(bricks[row][col]){

                float brickX= BRICK_OFFSET_LEFT + col*(BRICK_WIDTH + BRICK_PADDING);
                float brickY= BRICK_OFFSET_TOP + row*(BRICK_HEIGHT + BRICK_PADDING);

                bricksprite.setTexture(brickt[row % 4]);
                bricksprite.setPosition(brickX , brickY);
                window.draw(bricksprite);
               }
           }
       }

       window.draw(paddlesprite);
       window.draw(ballsprite);
       window.draw(scoretext);
       window.draw(livestext);
       window.draw(messagetext);
       window.draw(highscoretext);

       if(flashing){
        if(flashclock.getElapsedTime().asSeconds() > 0.1f){
            flashcount++;
            flashclock.restart();

            if(flashcount <= 6){
                if(flashcount % 2 == 0){
                    window.clear(Color::Red);
                }
            }

        }
       }

       if(gameOver){
        window.draw(gameOvertext);
        window.draw(restarttext);
       }

       if(gameWon){
        window.draw(gameWontext);
        window.draw(restarttext);
       }

       window.display();

    }

     return 0;
}