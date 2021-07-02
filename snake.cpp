#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <fstream>
#include <sstream>

using namespace std;

int main();

//implementing own kbhit (NOT WRITTEN BY ME, c++ forum: https://cboard.cprogramming.com/c-programming/63166-kbhit-linux.html (by: Thantos))
int kbhit (void)
{
  struct timeval tv;
  fd_set rdfs;
 
  tv.tv_sec = 0;
  tv.tv_usec = 0;
 
  FD_ZERO(&rdfs);
  FD_SET (STDIN_FILENO, &rdfs);
 
  select(STDIN_FILENO+1, &rdfs, NULL, NULL, &tv);
  return FD_ISSET(STDIN_FILENO, &rdfs);
 
}

//NOT WRITTEN BY ME, c++ forum: https://cboard.cprogramming.com/c-programming/63166-kbhit-linux.html (by: Thantos)
void changemode(int dir)
{
  static struct termios oldt, newt;
 
  if ( dir == 1 )
  {
    tcgetattr( STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newt);
  }
  else
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
}

//converting from milliseconds to microseconds
int msToUs(int ms){
    return ms * 1000;
}


class Snake {
    bool gameOver;


    const int height = 40;
    const int width = 80;

    int x, y, fX, fY, score;

    int tailX[40*80], tailY[40*80]; 
    int nTail;
    enum eDirection {STOP = 0, LEFT, RIGHT, UP, DOWN};  
    eDirection dir;
    public:
        
        bool getGameState();
        int getHighScore();
        void setHighScore(int);

        string getASCII(string);

        void gameMenu();

        void startGame();
        
        void displayHighScore();
        void Setup();
        void Draw();
        void Input();
        void Logic();
        

};


bool Snake::getGameState(){
    return gameOver;
}

void Snake::gameMenu(){

    int choice;

    string welcomeAscii = Snake::getASCII("snakewelcome.txt");
    system("clear");
    cout << welcomeAscii;
    cout << endl;
    cout << "1. Start Game" << endl;
    cout << "2. Settings" << endl;

    cout << "choice: ";
    cin >> choice;
    cout << endl;

    system("clear");

    if (choice == 1){
        Snake::startGame();
    }else if (choice == 2){
        int settingChoice;
        cout << "Settings" << endl;
        cout << endl;
        cout << "==============================================================" << endl;
        cout << "1. Reset Highscore" << endl;
        cout << "2. Set Highscore" << endl;

        cout << "choice: ";
        cin >> settingChoice;
        cout << endl;

        if (settingChoice == 1){
            int resetChoice;
            Snake::setHighScore(0);
            system("clear");
            cout << "Updated Highscore: " << Snake::getHighScore() << endl;
            cout << "Return to menu? " << endl;
            cout << "1. Yes" << endl;
            cout << "2. Quit Game" << endl;

            cout << "choice: ";
            cin >> resetChoice;
            cout << endl;

            if (resetChoice == 1){
                Snake::gameMenu();
            }
            
        }else if (settingChoice == 2){
            int newHS;
            system("clear");
            cout << "Enter highscore value: ";
            cin >> newHS;
            cout << endl;

            Snake::setHighScore(newHS);

            cout << "Updated Highscore: " << Snake::getHighScore() << endl;
            int updateChoice;
            cout << "Return to menu? " << endl;
            cout << "1. Yes" << endl;
            cout << "2. Quit Game" << endl;

            cout << "choice: ";
            cin >> updateChoice;
            cout << endl;

            if (updateChoice == 1){
                Snake::gameMenu();
            }
        }
    }

    
}

string Snake::getASCII(string path){

    string folderPath = "./ascii-art/";

    ifstream asciiFile (folderPath + path);

    string line;

    string fullAscii = "";

    while (getline(asciiFile, line)){
        fullAscii += line + "\n";
    }

    return fullAscii;

}

void Snake::startGame(){
    Snake::Setup();
    while(!Snake::getGameState()){
        Snake::Draw();
        Snake::Input();
        Snake::Logic();
        usleep(msToUs(80));
    }

    cout << "Game Over!" << endl;

    Snake::displayHighScore();
}

int Snake::getHighScore(){
    string path = "./highscore/highscore.txt";

    ifstream highscoreFile(path);

    string highscore;
    getline(highscoreFile, highscore);

    int intHS = atoi(highscore.c_str());

    return intHS;
}

void Snake::setHighScore(int newHS){
    string path = "./highscore/highscore.txt";

    ofstream highscoreFile(path);

    highscoreFile << newHS;
}

void Snake::displayHighScore(){
    
        
        if (Snake::getHighScore() < score){
            //update highscore
            Snake::setHighScore(score);

            
            int hs = Snake::getHighScore();
            //display highscore
            cout << "Congratulations! New Highscore: " << hs << endl;
        }
        
    

}

void Snake::Setup(){
    gameOver = false;
    score = 0;
    nTail = 0;
    dir = STOP;
    x = width / 2;//starts the game in the middle
    y = height / 2;

    fX = rand() % width; //random placement for fruit
    fY = rand() % height;
}

void Snake::Draw(){

    system("clear");//clears the console
    cout << endl;
    //top wall
    for (int i = 0; i < width+2; i++){
        cout << "#";
    }
    cout << endl;


    //side wall(right side)
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            if (j == 0){
                cout << "#";
            }
            
            if (i == y && j == x){
                cout << "O";
            }else if (i == fY && j == fX){
                cout << "F";
            }else{//Drawing the tail
                bool print = false; //not printing empty space if tail is there
                for (int k = 0; k < nTail; k++){
                    if (tailX[k] == j && tailY[k] == i){
                        cout << "o";
                        print = true;
                    }
                }

                if (!print){
                    cout << " ";
                }
                
            }
           
            

            //side wall (left side)
            if (j == width - 1){
                cout << "#";
            }
        }

        cout << endl;
        
        
    }

    //bottom wall
    for (int i = 0; i < width+2; i++){
        cout << "#";
    }
    cout << endl;
    cout << endl;
    cout << "Score: " << score << " | Current Highscore: " << Snake::getHighScore() << endl;

}

void Snake::Input(){

    changemode(1);
    if (kbhit()){
        switch(getchar()){
            case 97:
                dir = LEFT;
                break;
            case 115:
                dir = DOWN;
                break;
            case 100:
                dir = RIGHT;
                break;
            case 119:
                dir = UP;
                break;
            case 120:
                gameOver = true;
                break;
        }
    }
    changemode(0);
}

void Snake::Logic(){

    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y; //tail pos before previous (previous twice)
    tailX[0] = x;
    tailY[0] = y;
    //creating the tail
    for (int i = 1; i < nTail; i++){
        prev2X = tailX[i];
        prev2Y = tailY[i];

        tailX[i] = prevX;
        tailY[i] = prevY;

        prevX = prev2X;
        prevY = prev2Y;
    }
    switch(dir){
        case LEFT:
            x--; 
            break;
        case RIGHT:
            x++;
            break;
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
        default:
            break;
    }

    if (x > width || x < 0 || y > height || y < 0){
        gameOver = true;
    }

    for (int i = 0; i < nTail; i++){
        if (tailX[i] == x && tailY[i] == y){
            gameOver = true;
        }
    }

    if (x == fX && y == fY){
        score += 10;
        fX = rand() % width;
        fY = rand() % height;
        nTail++;
    }
}



int main(){

    Snake snake;
    snake.gameMenu();
    //snake.startGame();
    // snake.Setup();
    // while (!snake.getGameState()){
        
    //     snake.Draw();
    //     snake.Input();
    //     snake.Logic();
    //     usleep(msToUs(80));
    // }

    // cout << "Game Over :(" << endl;

    // changemode(1);
    // while(!kbhit()){
    //     cout << "waiting" << endl;
    // }
    // int ch = getchar();
    // cout << "You pressed: " << ch << endl;
    
    // changemode(0);
    return 0;
}