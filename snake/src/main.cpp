#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;
using namespace sf;

const int CELL_SIZE=40;
const int GRID_WIDTH=20;
const int GRID_HEIGHT=15;

int main(){

    srand(time(0));

    RenderWindow window(VideoMode(GRID_WIDTH * CELL_SIZE , GRID_HEIGHT * CELL_SIZE), "Snake Game!!");
    window.setFramerateLimit(60);
    
    //snake body
    vector<Vector2i> snake;
    snake.push_back(Vector2i(10,7));
    snake.push_back(Vector2i(9,7));
    snake.push_back(Vector2i(8,7));

    //direction
    Vector2i direction(1,0);

    //food
    Vector2i food(5,5);

    //movement timer
    Clock moveClock;
    float moveSpeed=0.2f;

    //score
    int score = 0;
    int highscore= 0;
    
    Font font;
    font.loadFromFile("assets/fonts/LiberationSans-Regular.ttf");
     
    //score text
    Text scoretext;
    scoretext.setFont(font);
    scoretext.setCharacterSize(20);
    scoretext.setFillColor(Color::White);
    scoretext.setPosition(10,10);
    scoretext.setString("Score: 0");

    //gameover text
    Text gameovertext;
    gameovertext.setFont(font);
    gameovertext.setCharacterSize(50);
    gameovertext.setFillColor(Color::Red);
    gameovertext.setPosition(200,250);
    gameovertext.setString("GAME OVER!!");

    //game restart text
    Text restart;
    restart.setFont(font);
    restart.setFillColor(Color::White);
    restart.setPosition(200,320);
    restart.setCharacterSize(20);
    restart.setString("Press 'R' to restart the game");

    //head of snake
    Texture headtexture;
    headtexture.loadFromFile("assets/images/head.png");
    Sprite headsprite;
    headsprite.setOrigin(19,19);
    headsprite.setTexture(headtexture);

    //tounge out head of snake for animation
    Texture tonguetexture;
    tonguetexture.loadFromFile("assets/images/head_tongue.png");
    Sprite tonguesprite;
    tonguesprite.setTexture(tonguetexture);

    //body of snake
    Texture bodytexture;
    bodytexture.loadFromFile("assets/images/body.png");
    Sprite bodysprite;
    bodysprite.setTexture(bodytexture);

    //food
    Texture foodtexture;
    foodtexture.loadFromFile("assets/images/apple.png");
    Sprite foodsprite;
    foodsprite.setTexture(foodtexture);
  
    //grid line
    Texture backgroundtexture;
    backgroundtexture.loadFromFile("assets/images/background.png");
    Sprite backgroundsprite;
    backgroundsprite.setTexture(backgroundtexture);

    //sound when food is eaten
    SoundBuffer eatbuffer;
    eatbuffer.loadFromFile("assets/audio/eat.wav");
    Sound eatsound;
    eatsound.setBuffer(eatbuffer);

    //sound when game is over
    SoundBuffer gameoverbuffer;
    gameoverbuffer.loadFromFile("assets/audio/gameover.wav");
    Sound gameoversound;
    gameoversound.setBuffer(gameoverbuffer);

    //background music
    Music backgroundmusic;
    backgroundmusic.openFromFile("assets/audio/Intense.ogg");
    backgroundmusic.setVolume(30);
    backgroundmusic.setLoop(true);
    backgroundmusic.play();
    
    //highscore text
    Text highscoretext;
    highscoretext.setFont(font);
    highscoretext.setCharacterSize(20);
    highscoretext.setPosition(600,10);
    highscoretext.setFillColor(Color::Yellow);
    highscoretext.setString("Best: ");

    //finalscore text
    Text finalscoretext;
    finalscoretext.setFont(font);
    finalscoretext.setPosition(250,370);
    finalscoretext.setCharacterSize(30);
    finalscoretext.setFillColor(Color::Green);
    finalscoretext.setString("score :" + to_string(score));

    //pause screen text
    Text pausetext;
    pausetext.setFont(font);
    pausetext.setFillColor(Color::Yellow);
    pausetext.setCharacterSize(60);
    pausetext.setPosition(250,220);
    pausetext.setString("PAUSED!");

    //Pause subtext
    Text pausesub;
    pausesub.setFont(font);
    pausesub.setFillColor(Color::White);
    pausesub.setCharacterSize(25);
    pausesub.setPosition(220,310);
    pausesub.setString("Press ESC. to continue");

    //boolean flag to check condition of game
    bool gameOver=false;
    bool directionchange=false;

    //boolean flag to flash red at gameover
    bool flashing=false;
    Clock flashclock;
    int flashcount= 0;

    //boolean flag to pause the game
    bool paused=false;

    //head animation
    bool tongue=false;
    Clock tongueclock;
    float tonguespeed=0.3f;

    //wiggling
    Clock wiggleclock;

    while(window.isOpen()){

    Event event;

    while(window.pollEvent(event)){

       if(event.type==Event::Closed){
           window.close();
        }
      
      //movements
      if(event.type==Event::KeyPressed){

      if(event.key.code==Keyboard::Up && direction.y != 1 && !directionchange){
        direction= Vector2i(0,-1);
        directionchange=true;
        headsprite.setRotation(180);
      }

      if(event.key.code==Keyboard::Down && direction.y !=-1 && !directionchange){
          direction=Vector2i(0,1);
          directionchange=true;
          headsprite.setRotation(0);
      }
      
      if(event.key.code==Keyboard::Left && direction.x !=1 && !directionchange){
          direction=Vector2i(-1,0);
          directionchange=true;
          headsprite.setRotation(90);
      } 

      if(event.key.code==Keyboard::Right && direction.x !=-1 && !directionchange){
         direction=Vector2i(1,0);
         directionchange=true;
         headsprite.setRotation(270);
      }
                 
  //gameover display
     if(event.key.code==Keyboard::R && gameOver){
          snake.clear();
          snake.push_back(Vector2i(10,7));
          snake.push_back(Vector2i(9,7));
          snake.push_back(Vector2i(8,7));
          direction=Vector2i(1,0);
          score=0;
          moveSpeed = 0.2f;
          gameOver=false;
          scoretext.setString("Score: 0");

         }  

  //for screen pause
   if(event.key.code==Keyboard::Escape && !gameOver){
   paused=!paused;
   if(paused){
   backgroundmusic.pause();
     }
   else{
  backgroundmusic.play();
       }
    }

   }

}

              
if(!gameOver && !paused){//GAME UPDATES

  if(moveClock.getElapsedTime().asSeconds() > moveSpeed){

    directionchange=false;

   //making new head
   Vector2i newhead = snake[0] + direction;
            
   // wall collision
   if(newhead.x < 0 || newhead.x >= GRID_WIDTH ||
    newhead.y < 0 || newhead.y >= GRID_HEIGHT){
    gameOver=true;
    gameoversound.play();
    flashing=true;
    flashcount=0;
    finalscoretext.setString("Score: " + to_string(score));
       }
                 
// self collision
for(int i= 0 ; i < snake.size() ; i++){
  if(newhead == snake[i]){
     gameOver=true;   
     gameoversound.play();
     flashing=true;
     flashcount=0;
     finalscoretext.setString("Score: " + to_string(score));
    }
  }

  //adding new head
 if(!gameOver){
  snake.insert(snake.begin(),newhead);
              
 //if food eaten keep new head
  if(newhead==food){
     score++;
    scoretext.setString("Score: " + to_string(score));
  if(score > highscore){
     highscore = score;
     highscoretext.setString("Best: " + to_string(highscore));
     }

    moveSpeed *= 0.95;
    food = Vector2i(rand() % 20,rand() % 15);
    eatsound.play();
  }

 //if food is not eaten popback
//:delete the tail to keep the size same
 else{
     snake.pop_back();
    }
  moveClock.restart();
  }
                 
 }

 //head tongue out animation
 if(tongueclock.getElapsedTime().asSeconds() > tonguespeed){
    tongue=!tongue;
    tongueclock.restart();
    if(tongue){
      headsprite.setTexture(tonguetexture);
    }
    else{
      headsprite.setTexture(headtexture);
    }
 }
}

window.clear(Color(20,20,20));

//drwaing background
window.draw(backgroundsprite);
           
//foodspritel
foodsprite.setPosition(food.x * CELL_SIZE + 1, food.y * CELL_SIZE +1);
window.draw(foodsprite);
          

  window.draw(scoretext);
  window.draw(highscoretext);

//for flash
if(flashing){
  if(flashclock.getElapsedTime().asSeconds() > 0.1f ){
      flashcount++;
      flashclock.restart();
 if(flashcount >= 6){
 flashing =false;
    }
   }
if(flashcount % 2 == 0){
   window.clear(Color::Red);
    }
}

 //wigglespeed value from clock
   float wigglespeed=wiggleclock.getElapsedTime().asSeconds();

//wiggle animation and snake setup
 for(int i = 0 ; i < snake.size() ; i++){
    
  if(i==0){
    headsprite.setPosition(snake[i].x * CELL_SIZE + 20 , snake[i].y * CELL_SIZE + 20);
    window.draw(headsprite);
  }

  else{
  float wiggle=sin(wigglespeed * 5 + i * 0.8f) * 3;
   
  float offSetX=0;
  float offSetY=0;
   
  if(direction.x !=0){
    offSetY=wiggle;
  }
  
  else{
    offSetX=wiggle;
  }

  bodysprite.setPosition(snake[i].x * CELL_SIZE + offSetX , snake[i].y * CELL_SIZE + offSetY);
  window.draw(bodysprite);
  }
 }

//when game is paused
if(paused){
  RectangleShape overlay(Vector2f(800,600));
  overlay.setFillColor(Color(0,0,0,150));
  window.draw(overlay);
  window.draw(pausetext);
  window.draw(pausesub);
}

//if flag is true which means if game is over
if(gameOver){
  window.draw(gameovertext);
  window.draw(restart);
  window.draw(finalscoretext);
}

window.display();

  }    
}
