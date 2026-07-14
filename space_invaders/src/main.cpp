#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;
using namespace sf;

const int WIDTH = 800;
const int HEIGHT = 600;

//Grid plan
const int ENEMY_WIDTH = 40;
const int ENEMY_HEIGHT = 32;
const int ENEMY_PADDING = 15;
const int ENEMY_OFFSET_TOP = 60;
const int ENEMY_OFFSET_LEFT = 60;
const int ENEMY_ROW = 4;
const int ENEMY_COL = 8;

int main(){

    RenderWindow window(VideoMode(WIDTH,HEIGHT) , "Space invaders");
    window.setFramerateLimit(60);
    
    Font font;
    font.loadFromFile("assets/fonts/LiberationSans-Regular.ttf");

    //background
    Texture backgroundtex;
    backgroundtex.loadFromFile("assets/images/background.png");
    Sprite backgroundsprite;
    backgroundsprite.setTexture(backgroundtex);

    //player texture
    Texture playertex;
    playertex.loadFromFile("assets/images/player.png");
    Sprite playersprite;
    playersprite.setTexture(playertex);
    playersprite.setPosition(385,520);

    //player's bullet
    Texture bullettex;
    bullettex.loadFromFile("assets/images/bullet.png");

    //enemy's bullet
    Texture enemybullettex;
    enemybullettex.loadFromFile("assets/images/enemybullet.png");

    //first enemy body
    Texture enemy1tex;
    enemy1tex.loadFromFile("assets/images/alien.png");

    //second enemy body
    Texture enemy2tex;
    enemy2tex.loadFromFile("assets/images/alien1.png");

    //player speed and shoot mechanic
    float playerspeed = 5.0f;
    bool canshoot = true;
    Clock shootclock;
    float shootcooldown = 0.5f;

    //player's and enemy's bullet with shoot parameters
    vector <Sprite> bullets;
    vector <Sprite> enemybullets;
    float bulletspeed = 8.0f;
    float enemybulletspeed = 5.0f;

    //enemy grid
    bool enemies [ENEMY_ROW] [ENEMY_COL];

    for(int row = 0 ; row < ENEMY_ROW ; row++){
        for(int col = 0 ; col < ENEMY_COL ; col++){

            enemies [row] [col] = true;
        }
    }

    //enmey movement and explosion parameters
    float enemydirx = 1.0f;
    float enemyOffsetX = 0;
    float enemyOffsetY = 0;
    float enemyspeed = 0.3f;
    float explosiontime= 0.1f;

    float explosionx = 0;
    float explosiony = 0;
    Clock explosionclock;
    
    //UI
    int score= 0;
    int lives= 3;
    int highscore = 0;
    int enemydead = 0;

    //clock mechanincs
    Clock enemyshootclock;
    float enemyshoottime = 1.5f;

    Clock enemymoveclock;
    float enemymovetime = 0.5f;

    Clock flashclock;
    int flashcount = 0;

    //SCore text
    Text scoretext;
    scoretext.setFont(font);
    scoretext.setCharacterSize(20);
    scoretext.setFillColor(Color::White);
    scoretext.setPosition(10,10);
    scoretext.setString("Score: 0");

    //Lives text
    Text livestext;
    livestext.setFont(font);
    livestext.setCharacterSize(20);
    livestext.setFillColor(Color::Red);
    livestext.setPosition(350,10);
    livestext.setString("Lives: 3");

    //Highscore text
    Text highscoretext;
    highscoretext.setFont(font);
    highscoretext.setCharacterSize(20);
    highscoretext.setFillColor(Color::Yellow);
    highscoretext.setPosition(600,10);
    highscoretext.setString("Highscore: 0");

    //Gameover text
    Text gameovertext;
    gameovertext.setFont(font);
    gameovertext.setFillColor(Color::Red);
    gameovertext.setCharacterSize(60);
    gameovertext.setPosition(200,250);
    gameovertext.setString("GAME OVER!!");

    //Gamewin text
    Text gamewintext;
    gamewintext.setFont(font);
    gamewintext.setCharacterSize(60);
    gamewintext.setFillColor(Color::Red);
    gamewintext.setPosition(200,250);
    gamewintext.setString("YOU WON!!");

    //Restart text
    Text restarttext;
    restarttext.setFont(font);
    restarttext.setCharacterSize(20);
    restarttext.setFillColor(Color::White);
    restarttext.setPosition(300,250);
    restarttext.setString("Press 'R' to restart");

    //Enemy explosion sound
    SoundBuffer explosionbuffer;
    explosionbuffer.loadFromFile("assets/audio/explosion.wav");
    Sound explosionsound;
    explosionsound.setBuffer(explosionbuffer);

    //Gameover sound
    SoundBuffer gameoverbuffer;
    gameoverbuffer.loadFromFile("assets/audio/gameover.wav");
    Sound gameoversound;
    gameoversound.setBuffer(gameoverbuffer);

    //Player hit sound
    SoundBuffer playerdiebuffer;
    playerdiebuffer.loadFromFile("assets/audio/playerdie.wav");
    Sound playerdiesound;
    playerdiesound.setBuffer(playerdiebuffer);

    //Player shooting sound
    SoundBuffer shootbuffer;
    shootbuffer.loadFromFile("assets/audio/shoot.wav");
    Sound shootsound;
    shootsound.setBuffer(shootbuffer);

    //Win sound
    SoundBuffer winbuffer;
    winbuffer.loadFromFile("assets/audio/win.wav");
    Sound winsound;
    winsound.setBuffer(winbuffer);

    //background sound
    Music backgroundmusic;
    backgroundmusic.openFromFile("assets/audio/Red Curtain.ogg");
    backgroundmusic.setVolume(30);
    backgroundmusic.setLoop(true);
    backgroundmusic.play();

    //Flags
    bool gamewon = false;
    bool gameover = false;
    bool flashing = false;
    bool exploding = false;

    while(window.isOpen()){

        Event event;
        
        while(window.pollEvent(event)){

            if(event.type==Event::Closed){
                window.close();
            }

            if(event.type==Event::KeyPressed){
                
                //Restart event
                if(event.key.code == Keyboard::R && (gamewon || gameover)){

                    playersprite.setPosition(385,520);
                    
                    bullets.clear();
                    enemybullets.clear();

                    for(int row = 0 ; row < ENEMY_ROW ; row++){
                        for(int col = 0 ; col <ENEMY_COL ; col++){
                            enemies [row] [col] = true;
                        }
                    }

                    enemydirx = 1.0f;
                    enemyOffsetX= 0;
                    enemyOffsetY= 0;
                    enemyspeed= 0.6f;
                    enemyshoottime = 2.0f;

                    score = 0;
                    lives = 3;
                    flashcount = 0;

                    scoretext.setString("Score: 0");
                    livestext.setString("Lives: 3");
                    
                    gamewon = false;
                    gameover = false;
                    exploding= false;
                }
            } 
        }  

        if(!gameover && !gamewon){

            if(shootclock.getElapsedTime().asSeconds() > shootcooldown){
                    canshoot= true;
            }

            //left movement
            if(Keyboard::isKeyPressed(Keyboard::Left) && playersprite.getPosition().x > 0 ){
              playersprite.move( -playerspeed , 0 );
            }

            //right movement
            if(Keyboard::isKeyPressed(Keyboard::Right) && playersprite.getPosition().x + 30 < WIDTH){
                playersprite.move( playerspeed , 0);
            }

            //Player shooting mechanic
            if(Keyboard::isKeyPressed(Keyboard::Space) && !gameover && !gamewon && canshoot){
                canshoot=false;
                shootsound.play();
                    
                Sprite bulletsprite;
                bulletsprite.setTexture(bullettex);
                bulletsprite.setPosition(playersprite.getPosition().x + 13 , playersprite.getPosition().y);
                bullets.push_back(bulletsprite);//Bullet sprites made bullets
                
                shootclock.restart();
            }
            

            //Player's bullets movement and deadlines
            for(int i = 0; i < bullets.size() ; i++){
             bullets [i].move(0, -bulletspeed);

                if(bullets [i].getPosition().y < 0){
                  bullets.erase(bullets.begin() + i );
                   i-- ;
                }
            }

            //Enemy's bullets and deadlines
            for(int i = 0 ; i < enemybullets.size() ; i++ ){
                enemybullets[i].move( 0, enemybulletspeed);

                if(enemybullets[i].getPosition().y > HEIGHT){
                    enemybullets.erase(enemybullets.begin() + i);
                    i--;
                }
            }

            //Enemy grid movement
            if(enemymoveclock.getElapsedTime().asSeconds() > enemymovetime){
                enemyOffsetX += enemyspeed * enemydirx;
                 
                bool hitwall=false;

                for(int row=0 ; row < ENEMY_ROW ; row++){
                    for(int col=0; col < ENEMY_COL ; col++){
                        if(enemies[row][col]){
                           
                            float X = ENEMY_OFFSET_LEFT + col*(ENEMY_WIDTH + ENEMY_PADDING) + enemyOffsetX;

                            if( X < 0 || X + ENEMY_WIDTH > WIDTH){
                                hitwall = true;     
                            }
                        }
                    }
                }

                if(hitwall){
                  enemydirx = -enemydirx ;
                  enemyOffsetY += 20;
                }
            
            }

            //Enemy shooting mechanics
            if(enemyshootclock.getElapsedTime().asSeconds() > enemyshoottime){
                vector <pair<int,int>> aliveenemies;

                for(int row = 0 ; row < ENEMY_ROW ; row++){
                    for(int col = 0 ; col < ENEMY_COL ; col++){
                        if (enemies [row] [col]){
                            aliveenemies.push_back({row,col});
                        }
                    }
                }

                //Only alive enemies will shoot
                if(aliveenemies.size() > 0){
                    int indx = rand() % aliveenemies.size();
                    int r = aliveenemies[indx].first;
                    int c = aliveenemies[indx].second;

                    float X = ENEMY_OFFSET_LEFT + c * (ENEMY_WIDTH + ENEMY_PADDING) + enemyOffsetX + 18;
                    float Y = ENEMY_OFFSET_TOP +  r * (ENEMY_HEIGHT + ENEMY_PADDING) + enemyOffsetY + ENEMY_HEIGHT ;

                    Sprite enemybulletsprite;
                    enemybulletsprite.setTexture(enemybullettex);
                    enemybulletsprite.setPosition(X,Y);
                    enemybullets.push_back(enemybulletsprite);
                }
                enemyshootclock.restart();
            }

            //Player's bullet hits enemy
            for(int i = 0 ; i < bullets.size() ; i++){
                bool hit = false;
                for(int row = 0 ; row < ENEMY_ROW && !hit ; row++){
                    for(int col = 0 ; col< ENEMY_COL && !hit ; col++){
                        if(enemies[row][col]){
                           
                            float X = ENEMY_OFFSET_LEFT + col * (ENEMY_WIDTH + ENEMY_PADDING) + enemyOffsetX;
                            float Y = ENEMY_OFFSET_TOP + row * (ENEMY_HEIGHT + ENEMY_PADDING) + enemyOffsetY;

                            FloatRect enmeyrect(X , Y , ENEMY_WIDTH , ENEMY_HEIGHT);

                            if(bullets[i].getGlobalBounds().intersects(enmeyrect)){
                                enemies[row][col] = false;

                                exploding = true;
                                explosionclock.restart();
                                explosionx= X;
                                explosiony= Y;

                                explosionsound.play();

                                score += 10;
                                scoretext.setString("Score: " + to_string(score));

                                if(score > highscore){
                                    highscore = score;
                                    highscoretext.setString("Highscore: " + to_string(highscore));
                                }

                                enemydead ++;

                                if(enemydead % 5 == 0){
                                    enemyshoottime -= 0.2f;
                                    enemyspeed += 0.4f;
                                    if(enemyshoottime <=0.3f){
                                        enemyshoottime=0.3f;
                                    }
                                }

                                bullets.erase(bullets.begin() + i );
                                i--;
                                hit = true;
                            }
                        }
                    }
                }
            }

            //Enemy's bulllet hits player
            for(int i = 0 ; i < enemybullets.size() ; i ++){
                if(enemybullets[i].getGlobalBounds().intersects(playersprite.getGlobalBounds())){

                    playerdiesound.play();

                    lives --;
                    livestext.setString("Lives: " + to_string(lives));

                    enemybullets.erase(enemybullets.begin() + i);
                    i--;
                    
                    //Player dies when live is 0
                    if(lives <= 0){
                        gameover = true;
                        gameoversound.play();
                        backgroundmusic.stop();
                        flashing = true;
                        flashcount = 0;
                    }
                    else{
                        playersprite.setPosition(385,520);
                    }
                    
                }
            }

            //Gameover condition when enemy grid comes closer at down
            for(int row = 0 ; row < ENEMY_ROW ; row++){
                for(int col = 0 ; col < ENEMY_COL ; col++){
                    
                    if(enemies[row][col]){

                        float ey = ENEMY_OFFSET_TOP + row * (ENEMY_HEIGHT + ENEMY_PADDING) + enemyOffsetY;

                        if(ey + ENEMY_HEIGHT > HEIGHT-60){
                            gameover = true;
                        }
                    }
                }
            }                       

            //Win condition of all enemy dead flag 
            bool alldead = true;

            //win condition for all dead check
            for(int row = 0 ; row < ENEMY_ROW ; row++){
                for(int col = 0 ; col < ENEMY_COL ; col++){
                    if(enemies[row][col]){
                        alldead = false;
                    }
                }
            }

            //if all dead game won
            if(alldead){
                gamewon = true;
                winsound.play();
                backgroundmusic.stop();
            }
                
        }

        //Drawing section
        window.clear(Color::Black);
        window.draw(backgroundsprite);

        //enemy grid drawing
        for(int row = 0 ; row < ENEMY_ROW ; row++){
            for(int col = 0 ; col < ENEMY_COL ; col++){
                if( enemies[row][col]){

                    float ex = ENEMY_OFFSET_LEFT + col * (ENEMY_WIDTH + ENEMY_PADDING) + enemyOffsetX;
                    float ey = ENEMY_OFFSET_TOP + row * (ENEMY_HEIGHT + ENEMY_PADDING) + enemyOffsetY;

                    Sprite enemysprite;
                    if(row % 2 == 0){
                        enemysprite.setTexture(enemy1tex);
                    }
                    else{
                        enemysprite.setTexture(enemy2tex);
                    }
                    enemysprite.setPosition(ex,ey);
                    window.draw(enemysprite);
                }
            }
        }

        //Player drawing
        window.draw(playersprite);
            
        //Player's bullet drawing
        for(int i = 0 ; i < bullets.size() ; i++){
            window.draw(bullets[i]);
        }

        //Enemy's bullet drawing
        for(int i = 0 ; i < enemybullets.size() ; i ++){
            window.draw(enemybullets[i]);
        }

        //UI drawing
        window.draw(scoretext);
        window.draw(livestext);
        window.draw(highscoretext);

        //Flash effect when gameover drawing
        if(flashing){
            if(flashclock.getElapsedTime().asSeconds() > 0.1f){
                flashcount++;
                flashclock.restart();
      
                if(flashcount % 2 == 0){
                    window.clear(Color::Red);
                }

                if(flashcount >= 6){
                    flashing = false;
                }
                
            }
        }

        //Explosion effect when enemy dies
        if(exploding){
            if(explosionclock.getElapsedTime().asSeconds() < explosiontime){
                
                CircleShape explosion(12);
                explosion.setFillColor(Color(255,165,0,180));
                explosion.setPosition(explosionx , explosiony);
                window.draw(explosion);

                CircleShape inner(8);
                inner.setFillColor(Color(225,225,0,220));
                inner.setPosition(explosionx + 4 , explosiony + 4);
                window.draw(inner);
            }
            else{
                exploding= false;
            }
        }

        //Gameover UI
        if(gameover){
            window.draw(gameovertext);
            window.draw(restarttext);
        }

        //Gamewon UI
        if(gamewon){
            window.draw(gamewintext);
            window.draw(restarttext);
        }

        //Display  everything
        window.display();
    }
    return 0;
}    