#include <SFML/Graphics.hpp>
#include <sstream>
#include <fstream>
#include <string>

using namespace sf;

const int TILE_SIZE = 48;
const int COLS = 32;
const int VIEW_COLS = 16;
const int ROWS = 12;
const int WIDTH = VIEW_COLS * TILE_SIZE;
const int HEIGHT = ROWS * TILE_SIZE;

const int PLAYER_W = 30;
const int PLAYER_H = 42;
const int COIN_W = TILE_SIZE / 2;
const int COIN_H = TILE_SIZE / 2;

const float GRAVITY = 0.5f;
const float JUMP_FORCE = -11.0f;
const float MOVE_SPEED = 4.0f;

const int COIN_COUNT = 8;
const int ENEMY_COUNT = 3;

const int SPRITE_W = 110;
const int SPRITE_H = 110;

const int ENEMYS_W = 65;
const int ENEMYS_H = 65;

const int COINS_W = 50;
const int COINS_H = 50;

struct Enemy{
    float x,y;
    float vx;
    bool active = true;
    bool idle = false;
    Clock idleclock;
};

float idletime = 1.0f;

Enemy enemy[ENEMY_COUNT] {
    {23*TILE_SIZE, 5*TILE_SIZE , 2.0f , true},
    {12*TILE_SIZE, 4*TILE_SIZE , 2.0f , true},
    {11*TILE_SIZE, 8*TILE_SIZE , 2.0f , true},
};

struct Coin{
  float x,y;
  bool active = true;
};

Coin coin[COIN_COUNT] {
    {24*TILE_SIZE,  5*TILE_SIZE, true},
    {12*TILE_SIZE,  4*TILE_SIZE, true},
    {11*TILE_SIZE,  8*TILE_SIZE, true},
    {27*TILE_SIZE, 2*TILE_SIZE, true},
    {20*TILE_SIZE, 8*TILE_SIZE, true},
    {18*TILE_SIZE, 2*TILE_SIZE, true},
    {0*TILE_SIZE, 5*TILE_SIZE, true},
    {16*TILE_SIZE, 7*TILE_SIZE, true},
};

bool overlaps(int ax , int ay , int ap , int aq, 
int bx , int by , int bp , int bq){

    return ax + ap >  bx && 
           ax <  bx + bp &&
           ay <  by + bq &&
           ay + aq >  by ;
}

int map[ROWS][COLS];

void loadMap(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    int row = 0;
    while (std::getline(file, line) && row < ROWS) {
        std::stringstream ss(line);
        std::string val;
        int col = 0;
        while (std::getline(ss, val, ',') && col < COLS) {
            map[row][col] = std::stoi(val);
            col++;
        }
        row++;
    }
}

struct Player{
    float x,y;
    float vy;
    bool onground;
};

struct anim{
    int frame;
    int tframe;
    int fwidth;
    int fheight;
    float fspeed = 0.2f;
    Clock animclock;
    Texture texture;
    Sprite sprite;
    float x,y,vx,vy;
};  

bool issolid(int row , int col){
    if(row < 0 || row >= ROWS) return false;
    if (col < 0 || col >= COLS) return false;
    return map[row][col]==1 || map[row][col]==2 || map[row][col]==0;
}

void horizontaltile(Player& player){ 
    float left = player.x + 1;
    float right = player.x + 29;  
    float bottom = player.y + 41;
    float top = player.y + 1;

    int TOP_ROW = (int) top / TILE_SIZE;
    int BOTTOM_ROW = (int) bottom / TILE_SIZE;
    int LEFT_COL = (int) left / TILE_SIZE;
    int RIGHT_COL = (int) right / TILE_SIZE;

    //left wall collision
    if(issolid(TOP_ROW , LEFT_COL) || issolid(BOTTOM_ROW , LEFT_COL)){
        player.x = LEFT_COL * TILE_SIZE + TILE_SIZE;
    }

    //Right wall collision
    if(issolid(TOP_ROW , RIGHT_COL) || issolid(BOTTOM_ROW , RIGHT_COL)){   
        player.x = RIGHT_COL * TILE_SIZE - PLAYER_W ;
    }

    //Screen horizontal edges collision
    if(player.x < 0) player.x = 0;
    if(player.x + 30 > TILE_SIZE * COLS) player.x = TILE_SIZE * COLS - 30;
}

void verticaltile(Player& player){

    float left = player.x + 1;
    float right = player.x + 29;
    float bottom = player.y + 41;
    float top = player.y + 1;

    int TOP_ROW = (int) top / TILE_SIZE;
    int BOTTOM_ROW = (int) bottom / TILE_SIZE;
    int LEFT_COL = (int) left / TILE_SIZE;
    int RIGHT_COL = (int) right / TILE_SIZE;

    //bottom wall collision
    if(issolid(BOTTOM_ROW , LEFT_COL) || issolid(BOTTOM_ROW , RIGHT_COL)){
        player.vy = 0;
        player.y = (BOTTOM_ROW * TILE_SIZE) - PLAYER_H ;
        player.onground = true;  
    }

    //top wall collision
    if(issolid(TOP_ROW , LEFT_COL) || issolid(TOP_ROW , RIGHT_COL)){
        player.y = TOP_ROW * TILE_SIZE + TILE_SIZE;
        player.vy = 0;
    }
    
    //Screen vertical edges collision
    if(player.y < 0) player.y = 0;
    if(player.y + 42 > TILE_SIZE * ROWS) player.y = TILE_SIZE * ROWS - 42;
}

void enemyupdate(Enemy& enemy , Player& player){

    if(!(enemy.active)) return ;

    // expire idle first — if still idle, skip all movement logic
    if(enemy.idle){
        if(enemy.idleclock.getElapsedTime().asSeconds() >= idletime)
            enemy.idle = false;
        return;
    }

    float enemytop = enemy.y;
    float enemybottom = enemy.y + PLAYER_H;
    float enemyright = enemy.x + PLAYER_W;
    float enemyleft = enemy.x;

    int TOP_ROW = (int) enemytop / TILE_SIZE;
    int BOTTOM_ROW = (int) enemybottom / TILE_SIZE;
    int RIGHT_COL = (int) enemyright / TILE_SIZE;
    int LEFT_COL = (int) enemyleft / TILE_SIZE;
    int FEET_ROW = BOTTOM_ROW + 1;

    //left collision
    if(issolid(TOP_ROW , LEFT_COL) || issolid(BOTTOM_ROW , LEFT_COL)){
        enemy.x = LEFT_COL * TILE_SIZE + TILE_SIZE + 25;
        enemy.vx = -enemy.vx;
        enemy.idle = true; enemy.idleclock.restart();
        return;
    }

    //right collision
    if(issolid(TOP_ROW , RIGHT_COL) || issolid(BOTTOM_ROW , RIGHT_COL)){
        enemy.x = RIGHT_COL * TILE_SIZE - PLAYER_W - 25;
        enemy.vx = -enemy.vx;
        enemy.idle = true; enemy.idleclock.restart();
        return;
    }

    //ground check — only one can fire per frame due to returns above
    if(!issolid(FEET_ROW , RIGHT_COL) && enemy.vx > 0){
        enemy.x = RIGHT_COL * TILE_SIZE  - PLAYER_W - 25;
        enemy.vx = -enemy.vx;
        enemy.idle = true; enemy.idleclock.restart();
        return;
    }

    if(!issolid(FEET_ROW , LEFT_COL) && enemy.vx < 0){
        enemy.x = LEFT_COL * TILE_SIZE + TILE_SIZE + 25;
        enemy.vx = -enemy.vx;
        enemy.idle = true; enemy.idleclock.restart();
    }
}


void enemycollision(Enemy& enemy , Player& player , int& score , Text& scoretext){

    if(!enemy.active) return;

    if(overlaps(player.x , player.y , PLAYER_W , PLAYER_H , enemy.x + 10 , enemy.y+10 , PLAYER_W - 10 , PLAYER_H-10)){
        player.x = 100;
        player.y = 400;
        player.vy = 0;
        score = 0;
        scoretext.setString("Score: " + std::to_string(score));
    } 

    if(overlaps(player.x , player.y , PLAYER_W , PLAYER_H , enemy.x + 12 , enemy.y , PLAYER_W - 12 , PLAYER_H - 32)){
        enemy.active = false;
        score+=20;
        player.vy = JUMP_FORCE / 2 ;
        scoretext.setString("Score: " +std::to_string(score));
    }
}

void animation(anim& a , float& fspeed){
    if(a.animclock.getElapsedTime().asSeconds() > a.fspeed){
        a.frame++;
        if(a.frame >= a.tframe){
            a.frame = 0;
        }
        a.sprite.setTextureRect(IntRect(a.frame * a.fwidth , 0 , a.fwidth , a.fheight));
        a.animclock.restart();
    }
}

//face direction handling for player animations
bool facingright = true;

int main() {

    loadMap("assets/data/map.csv");
    
       

    //coin animation setup
    anim coinanim;
    coinanim.texture.loadFromFile("assets/images/coin.png");
    coinanim.sprite.setTexture(coinanim.texture);
    coinanim.frame = 0;
    coinanim.tframe = 8;
    coinanim.fwidth = 32;
    coinanim.fheight = 32;
    coinanim.sprite.setScale( (float)COIN_W / coinanim.fwidth , (float)COIN_H / coinanim.fheight );

    //knight walk animation setup
    anim kwalk;
    kwalk.texture.loadFromFile("assets/images/knight_walk.png");
    kwalk.sprite.setTexture(kwalk.texture);
    kwalk.frame = 0;
    kwalk.tframe = 8;
    kwalk.fwidth = 96;
    kwalk.fheight = 84;
    kwalk.sprite.setScale( (float)PLAYER_W / kwalk.fwidth , (float)PLAYER_H / kwalk.fheight );
    kwalk.fspeed = 0.1f;

    //knight jump animation setup
    anim kjump;
    kjump.texture.loadFromFile("assets/images/knight_jump.png");
    kjump.sprite.setTexture(kjump.texture);
    kjump.frame = 0;
    kjump.tframe = 5;
    kjump.fwidth = 96;
    kjump.fheight = 84;
    kjump.sprite.setScale( (float)PLAYER_W / kjump.fwidth , (float)PLAYER_H / kjump.fheight );

    //knight idle animation setup
    anim kidle;
    kidle.texture.loadFromFile("assets/images/knight_idle.png");
    kidle.sprite.setTexture(kidle.texture);
    kidle.frame = 0;
    kidle.tframe = 7;
    kidle.fwidth = 96;
    kidle.fheight = 84;
    kidle.sprite.setScale( (float)PLAYER_W / kidle.fwidth , (float)PLAYER_H / kidle.fheight );

    //mummy walk animation setup
    anim mwalk;
    mwalk.texture.loadFromFile("assets/images/mummy_walk.png");
    mwalk.sprite.setTexture(mwalk.texture);
    mwalk.frame = 0;
    mwalk.tframe = 6;
    mwalk.fwidth = 48;
    mwalk.fheight = 48;
    mwalk.sprite.setScale( (float)PLAYER_W / mwalk.fwidth , (float)PLAYER_H / mwalk.fheight );

    //mummy idle animation setup
    anim midle;
    midle.texture.loadFromFile("assets/images/mummy_idle.png");
    midle.sprite.setTexture(midle.texture);
    midle.frame = 0;
    midle.tframe = 4;
    midle.fwidth = 48;
    midle.fheight = 48;
    midle.sprite.setScale( (float)PLAYER_W / midle.fwidth , (float)PLAYER_H / midle.fheight );

    //player setup
    Player player;
    player.x = 100;
    player.y = 400;
    player.vy = 0;
    player.onground = false;

    int score = 0;

    Font font;
    font.loadFromFile("assets/fonts/LiberationSans-Regular.ttf");

    Text scoretext;
    scoretext.setFont(font);
    scoretext.setPosition(10,10);
    scoretext.setFillColor(Color::Green);
    scoretext.setString("Score: 0");
   
    RenderWindow window(VideoMode(VIEW_COLS * TILE_SIZE , ROWS * TILE_SIZE),"Platformer game");
    window.setFramerateLimit(60);

    RectangleShape tile(Vector2f(TILE_SIZE - 1 , TILE_SIZE - 1));
    tile.setFillColor(Color::Red);

    RectangleShape playershape(Vector2f(30,42));
    playershape.setFillColor(Color::Green);

    RectangleShape coinshape(Vector2f(COIN_W , COIN_H));
    coinshape.setTexture(&coinanim.texture);

    RectangleShape enemyshape(Vector2f(PLAYER_W ,PLAYER_H));
    enemyshape.setFillColor(Color::Red);

    while(window.isOpen()){
 
        Event event;
        while(window.pollEvent(event)){
            if(event.type==Event::Closed){
                window.close();
            } 
        }

        if(Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A)){
            player.x -= MOVE_SPEED;
        }

        if(Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D)){
            player.x += MOVE_SPEED;
        }

        horizontaltile(player);

        if(Keyboard::isKeyPressed(Keyboard::Space) && player.onground){
            player.vy = JUMP_FORCE;
            player.onground = false;
        }

        //physics
        player.vy += GRAVITY;
        player.y += player.vy;

        verticaltile(player);
        
        //enemy movement
        for(int i = 0 ; i < ENEMY_COUNT ; ++i){
            if(!enemy[i].idle) enemy[i].x += enemy[i].vx;
            enemyupdate(enemy[i],player);
            enemycollision(enemy[i] , player , score , scoretext);
        }

        //camera mechanics 
        float camx = player.x - WIDTH/2 - PLAYER_W/2;
        if(camx < 0) camx = 0;
        if(camx > COLS * TILE_SIZE - WIDTH) camx =  COLS * TILE_SIZE - WIDTH;

        window.clear(Color::Black);

        for(int row = 0; row < ROWS ; row++){
            for(int col = 0; col < COLS ; col++){

                if(map[row][col] == -1) continue;   
                
                if(map [row] [col] == 1){
                    tile.setFillColor(Color::Red);
                }
                if(map [row] [col] == 0){
                    tile.setFillColor(Color::Green);
                }
                if(map [row] [col] == 2){
                    tile.setFillColor(Color::Yellow);
                }
                tile.setPosition(col * TILE_SIZE - camx , row * TILE_SIZE);
                window.draw(tile);
            }
        }

        for(int i = 0 ; i < COIN_COUNT ; ++i){
            if(coin[i].active && overlaps(player.x , player.y , PLAYER_W , PLAYER_H ,
               coin[i].x , coin[i].y , COIN_W , COIN_H)){
                
                score += 10;
                coin[i].active = false;
                scoretext.setString("Score: " + std::to_string(score));
            }
        }

        for(int i = 0 ; i < COIN_COUNT ; i++){
            if(coin[i].active){
                animation(coinanim, coinanim.fspeed);
                coinanim.sprite.setScale( (float)COINS_W / coinanim.fwidth , (float)COINS_H / coinanim.fheight );
                coinanim.sprite.setPosition(coin[i].x - camx  - (COINS_W - COIN_W)/2 + 10, coin[i].y - (COINS_H - COIN_H)/2 + 13);
                window.draw(coinanim.sprite);
            }
        }

        for(int i = 0; i < ENEMY_COUNT ; ++i){
            if(enemy[i].active){
                if(enemy[i].idle){
                    animation(midle , midle.fspeed);
                    if(enemy[i].vx > 0){
                        midle.sprite.setScale( -(float)ENEMYS_W / midle.fwidth , (float)ENEMYS_H / midle.fheight );
                        midle.sprite.setPosition(enemy[i].x - camx + PLAYER_W +(ENEMYS_W - PLAYER_W)/2, enemy[i].y - (ENEMYS_H - PLAYER_H)/2 - 7);
                    } else {
                        midle.sprite.setScale( (float)ENEMYS_W / midle.fwidth , (float)ENEMYS_H / midle.fheight );
                        midle.sprite.setPosition(enemy[i].x - camx - (ENEMYS_W - PLAYER_W)/2 , enemy[i].y - (ENEMYS_H - PLAYER_H)/2 -7);
                    }
                    window.draw(midle.sprite);
                } else {
                    animation(mwalk , mwalk.fspeed);
                    if(enemy[i].vx > 0){
                        mwalk.sprite.setScale( -(float)ENEMYS_W / mwalk.fwidth , (float)ENEMYS_H / mwalk.fheight );
                        mwalk.sprite.setPosition(enemy[i].x - camx + PLAYER_W +(ENEMYS_W - PLAYER_W)/2, enemy[i].y - (ENEMYS_H - PLAYER_H)/2 -7);
                    } else {
                        mwalk.sprite.setScale( (float)ENEMYS_W / mwalk.fwidth , (float)ENEMYS_H / mwalk.fheight );
                        mwalk.sprite.setPosition(enemy[i].x - camx - (ENEMYS_W - PLAYER_W)/2 , enemy[i].y - (ENEMYS_H - PLAYER_H)/2 -7);
                    }
                    window.draw(mwalk.sprite);
                }
            }
        }
        
        //face direction handling for player animations
        if(Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right)){
            facingright = true;
        }
        if(Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left)){
            facingright = false;
        }
        
        //player in ground and moving animation handling
        if(player.onground){
            if(Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::D) || 
               Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::Right)){
                animation(kwalk , kwalk.fspeed);
                if(facingright){
                    kwalk.sprite.setScale((float)SPRITE_W / kwalk.fwidth, (float)SPRITE_H / kwalk.fheight);
                    kwalk.sprite.setPosition(player.x - camx - (SPRITE_W - PLAYER_W)/2, player.y - (SPRITE_H - PLAYER_H)/2 - 5);
                    window.draw(kwalk.sprite);
                }
                else {
                    kwalk.sprite.setScale(-(float)SPRITE_W / kwalk.fwidth, (float)SPRITE_H / kwalk.fheight);
                    kwalk.sprite.setPosition(player.x - camx + PLAYER_W +(SPRITE_W - PLAYER_W)/2, player.y - (SPRITE_H - PLAYER_H)/2 - 5);
                    window.draw(kwalk.sprite);
                }
            } 
            //player in ground and idle animation handling
            else{
                animation(kidle , kidle.fspeed);
                if(facingright){
                    kidle.sprite.setScale((float)SPRITE_W / kidle.fwidth, (float)SPRITE_H / kidle.fheight);
                    kidle.sprite.setPosition(player.x - camx - (SPRITE_W - PLAYER_W)/2, player.y - (SPRITE_H - PLAYER_H)/2 - 5);
                    window.draw(kidle.sprite);
                } else {
                    kidle.sprite.setScale(-(float)SPRITE_W / kidle.fwidth, (float)SPRITE_H / kidle.fheight);
                    kidle.sprite.setPosition(player.x - camx + PLAYER_W +(SPRITE_W - PLAYER_W)/2, player.y - (SPRITE_H - PLAYER_H)/2 - 5);
                    window.draw(kidle.sprite);
                }
            }
        } 

        //player in air animation handling
        if((!player.onground)){
            animation(kjump , kjump.fspeed);
            if(facingright){
                kjump.sprite.setScale((float)SPRITE_W / kjump.fwidth, (float)SPRITE_H / kjump.fheight);
                kjump.sprite.setPosition(player.x - camx - (SPRITE_W - PLAYER_W)/2, player.y - (SPRITE_H - PLAYER_H)/2 - 5);
                window.draw(kjump.sprite);
            } else {
                kjump.sprite.setScale(-(float)SPRITE_W / kjump.fwidth, (float)SPRITE_H / kjump.fheight);
                kjump.sprite.setPosition(player.x - camx + PLAYER_W +(SPRITE_W - PLAYER_W)/2, player.y - (SPRITE_H - PLAYER_H)/2 - 5);
                window.draw(kjump.sprite);
            }
        }

        window.draw(scoretext);
        window.display();
    }
    return 0;
}