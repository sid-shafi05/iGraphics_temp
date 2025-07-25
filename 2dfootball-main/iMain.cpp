#include "iGraphics.h"
#include<string.h>
#include<math.h>
#include<stdlib.h>

Sprite players[6],opponents[6],gk[2],fball, pauseButtons;
Image ball[1],player[1],opponent[1],field, backButton, pauseButton; 
double length=120*6 , width = 90*6, gallary=40, goalbar=160;
long long int i,ii,gametime=0,tracktime=0;
double timer=1;
int sec,minit;
int level,scoreplayer=0,scoreopp=0;
double constSpeed=0.5, speed=0.6 ,gkspeed=0.5, passedballspeed=1, shootballspeed=1.3, acceleration=-0.002, varAcceleration[2];
double playerradius=8,ballradius=5,collidepos=5;
double newball[4]={width/2,length/2};
double gkpointer[2][2]={ {width/2,gallary+playerradius},{width/2,length-gallary-playerradius} };
double newgk[2][2]={ {width/2,gallary+playerradius},{width/2,length-gallary-playerradius} };
double playerposition[2][6][4]={{{width/2,length*8/10,width/2,length*8/10,}, {width/6,length*8/10,width/6,length*8/10}, {width*5/6,length*8/10,width*5/6,length*8/10},{width*4/6,length*7/10,width*4/6,length*7/10},{width*2/6,length*7/10,width*2/6,length*7/10} , {width/2,length*6/10,width/2,length*6/10} },{ {width/2,length*2/10,width/2,length*2/10},{width/6,length*2/10,width/6,length*2/10},{width*5/6,length*2/10,width*5/6,length*2/10} ,{width*4/6,length*7/20,width*4/6,length*7/20} ,{width*2/6,length*7/20,width*2/6,length*7/20}, {width/2,length/2,width/2,length/2} }};
double newplayerposition[2][6][4]={{{width/2,length*8/10,width/2,length*8/10,}, {width/6,length*8/10,width/6,length*8/10}, {width*5/6,length*8/10,width*5/6,length*8/10} ,{width*4/6,length*7/10,width*4/6,length*7/10} ,{width*2/6,length*7/10,width*2/6,length*7/10}, {width/2,length*6/10,width/2,length*6/10} },{ {width/2,length*2/10,width/2,length*2/10},{width/6,length*2/10,width/6,length*2/10},{width*5/6,length*2/10,width*5/6,length*2/10},{width*4/6,length*7/20,width*4/6,length*7/20} ,{width*2/6,length*7/20,width*2/6,length*7/20}, {width/2,length/2,width/2,length/2} }};
double ballpointer[4]={width/2,length/2};
int activeplayer=5,ballstate=2,ballholder=activeplayer,activeplayeropp=5,helpingplayer=4,helpingplayeropp=4,lastTouch=1;
int page_number=0,coverpagetime=100;
double dxy=30;
double positionField[2][6][4]={{{4*width/7, 3*width/7, length-gallary, length/2}, {3*width/7, gallary, length-gallary, 10*length/20}, {width-gallary, 4*width/7,  length-gallary, 10*length/20} , {width-gallary, 4*width/7, 10*length/20, gallary}, {3*width/7, gallary, 10*length/20, gallary}, {4*width/7, 3*width/7, length/2, gallary}}, {{4*width/7, 3*width/7, length/2, gallary}, {3*width/7, gallary, 10*length/20, gallary}, {width-gallary, 4*width/7, 10*length/20, gallary},{width-gallary, 4*width/7, length-gallary, 10*length/20}, {3*width/7, gallary, length-gallary, 10*length/20},  {4*width/7, 3*width/7, length-gallary, length/2}}};
int dX=60, dY=100;
int frameCount = 0;
int previousTime = 0, previousFpsTime = 0;
int fps = 0;
double varFriction = 0.95;

//Penalty
//Penalty
#define MAX_SHOTS 20
double  playerX[MAX_SHOTS];  
int     playerResult[MAX_SHOTS]={0};
int     playerCount = 0;

double  oppX[MAX_SHOTS];
int     oppResult[MAX_SHOTS]={0};
int     oppCount    = 0;


int penaltyActive = 0;             // Flag to indicate penalty mode  
double targetX = -1, targetY = -1; // Clicked target position
double gkMoveDirection = 1;        // 1 for right, -1 for left
long long int gkMoveTimer = 0;     // Timer for goalkeeper movement
int activeTeam = 0;     // 0 for player team, 1 for opponent team
int scoreMark = 0;      // Flag to indicate if a score has been marked
double ballSpeed = 3.0; // Speed of the ball during penalty
int penaltyTurn = 0;    // Track the current penalty turn
# define maxPenalties 5 // Maximum penalties per team
double playerMark[2*maxPenalties];
double opponentMark[2*maxPenalties];
int    playerMarkCount   = 0;
int    opponentMarkCount = 0;
//Penalty meter
double penaltyMeterX ; // X position of the penalty meter// Y position of the penalty meter
double penaltyMeterMaxY=gallary+100 ; // Maximum Y position of the meter
double penaltyMeterMinY = gallary + 20; // Minimum Y position of the meter
double penaltyMeterY = penaltyMeterMinY; 
int meterDirection = 1; // Direction of the meter movement (1 for right, -1 for left)
double midY = (penaltyMeterMinY +penaltyMeterMaxY) * 0.5;
const double meterSpeed = .05; // Speed of the meter movement

void iShowSpeed(double x, double y)
{
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    frameCount++;
    if (previousFpsTime == 0)
    {
        previousFpsTime = currentTime; // Initialize on first call
        frameCount = 0;
    }
    else
    {
        int elapsedFpsTime = currentTime - previousFpsTime;

        if (elapsedFpsTime > 1000)
        {
            fps = (frameCount * 1000.0f) / elapsedFpsTime;
            frameCount = 0;
            previousFpsTime = currentTime;
        }
    }

    char fpsText[20];
    sprintf(fpsText, "FPS: %d", fps);
    iText(x, y, fpsText);
}
bool checkcollision(double x1, double y1, double x2, double y2, double r)
{
    if ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) <= r * r)
        return true;
    else
        return false;
}
void meterUpdate(){
    if(meterDirection == 1) {
        penaltyMeterY += meterSpeed;
        if(penaltyMeterY >= penaltyMeterMaxY) {
            meterDirection = -1; // Change direction to left
        }
    } else {
        penaltyMeterY -= meterSpeed;
        if(penaltyMeterY <= penaltyMeterMinY) {
            meterDirection = 1; // Change direction to right
        }
    }
}
void drawMeter(){
     penaltyMeterX=width-100;
     iSetColor(200,200,200);
     iRectangle(penaltyMeterX, penaltyMeterMinY, 10, penaltyMeterMaxY - penaltyMeterMinY);
        iSetColor(255,255,255);
        iLine(penaltyMeterX,penaltyMeterY, penaltyMeterX + 10, penaltyMeterY);
    iSetColor(255,255,0);               // bright yellow tick
    iLine(penaltyMeterX, midY, penaltyMeterX + 10, midY);
    }
void spritepositionupdate()
{
    for (i = 0; i < 6; i++)
    {
        iSetSpritePosition(&players[i], playerposition[1][i][0] - playerradius, playerposition[1][i][1] - playerradius);
        iSetSpritePosition(&opponents[i], playerposition[0][i][0] - playerradius, playerposition[0][i][1] - playerradius);
        if (i == 0 || i == 1)
            iSetSpritePosition(&gk[i], gkpointer[i][0] - playerradius, gkpointer[i][1] - playerradius);
    }
    iSetSpritePosition(&fball, ballpointer[0] - ballradius, ballpointer[1] - ballradius);
}

void spriteshow()
{
    // iShowImage(0,0,"field.png");
    // iShowSprite(&fields);
    for (i = 0; i < 6; i++)
    {
        iShowSprite(&players[i]);
        iShowSprite(&opponents[i]);
    }
    iShowSprite(&gk[0]);
    iShowSprite(&gk[1]);
    iShowSprite(&fball);
    iShowImage(width - 15, length - 15, "pause.png");
}
void gameresult(){
    if (scoreplayer > scoreopp) {
        iSetColor(0, 255, 0);
        iTextAdvanced(width / 2 - 50, length / 2, "You Win!", .1, 3);
    } else if (scoreplayer < scoreopp) {
        iSetColor(255, 0, 0);
        iTextAdvanced(width / 2 - 50, length / 2, "You Lose!", .1, 3);
    } else {
        iSetColor(255, 255, 0);
        iTextAdvanced(width / 2 - 50, length / 2, "It's a Draw!", .1, 3);
    }
}
void Penalty()
{
    // player
    if (activeTeam == 1)
    {
        playerposition[activeTeam][activeplayer][0] = width / 2;
        playerposition[activeTeam][activeplayer][1] = length - gallary - (length * .15);
        gkpointer[0][0] = width / 2;
        gkpointer[0][1] = length - gallary - playerradius;
        gkpointer[1][0] = width / 6;
        gkpointer[1][1] = length - length / 8;
    }
    else
    {
        playerposition[activeTeam][activeplayer][0] = width / 2;
        playerposition[activeTeam][activeplayer][1] = length - gallary - (length * .15);
        gkpointer[1][0] = width / 2;
        gkpointer[1][1] = length - gallary - playerradius;
        gkpointer[0][0] = width / 6;
        gkpointer[0][1] = length - length / 8;
    }
    iSetSpritePosition(&players[activeplayer], playerposition[1][activeplayer][0] - playerradius, playerposition[1][activeplayer][1] - playerradius);
    iSetSpritePosition(&gk[activeTeam], gkpointer[activeTeam][0] - playerradius, gkpointer[activeTeam][1] - playerradius);
    // ball
    ballpointer[0] = playerposition[activeTeam][activeplayer][0];
    ballpointer[1] = playerposition[activeTeam][activeplayer][1];
    ballpointer[2] = 0;
    ballpointer[3] = 0;
    iSetSpritePosition(&fball, ballpointer[0] - ballradius, ballpointer[1] - ballradius);
    ballstate = 2;

    // others
    double centerY = length / 2;
    double spacing = width / 16;
    int playerIndex = 0;

    for (int i = 0; i < 6; i++)
    {
        if (i == activeplayer && activeTeam == 0)
            continue; // Skip the active player
        playerposition[0][i][0] = gallary + spacing * (playerIndex + 1);
        playerposition[0][i][1] = centerY;
        iSetSpritePosition(&players[i], playerposition[0][i][0] - playerradius, playerposition[0][i][1] - playerradius);
        playerIndex++;
    }

    for (int i = 0; i < 6; i++)
    {
        if (i == activeplayer && activeTeam == 1)
            continue; // Skip the active player
        playerposition[1][i][0] = gallary + spacing * (playerIndex + 1);
        playerposition[1][i][1] = centerY;
        iSetSpritePosition(&opponents[i], playerposition[1][i][0] - playerradius, playerposition[1][i][1] - playerradius);
        playerIndex++;
    }
}
void swapteams()
{
    if (ballstate == 2 && penaltyActive && scoreMark != 0)
    {
        activeTeam = 1 - activeTeam; // Swap teams
        penaltyTurn++;
        scoreMark = 0; // Reset score mark flag
        if (penaltyTurn >= 10)
        { // 5 shots each
            if (scoreplayer != scoreopp)
            {
                penaltyActive = 0;
            }
            // Else continue sudden death
        }
        else if (penaltyTurn >= 2 * maxPenalties)
        {
            if (abs(scoreplayer - scoreopp) > (10 - penaltyTurn) / 2)
            {
                penaltyActive = 0; // Can't catch up
            }
        }
    }
}
void penaltyGK(){
        static int firstFrame = 1;
        if (firstFrame)
        {
            gkpointer[1 - activeTeam][0] = width / 2;
            gkpointer[1 - activeTeam][1] = length - gallary - playerradius; // Bottom goalpost
            iSetSpritePosition(&gk[1 - activeTeam], gkpointer[1 - activeTeam][0] - playerradius, gkpointer[1 - activeTeam][1] - playerradius);
            Penalty(); // Set up initial penalty state
            firstFrame = 0;
            // gk2
            gkpointer[activeTeam][0] = width / 6;
            gkpointer[activeTeam][1] = length - length / 8;
            iSetSpritePosition(&gk[activeTeam], gkpointer[activeTeam][0] - playerradius, gkpointer[activeTeam][1] - playerradius);
        }
        printf("iDraw called | ballstate: %d | ballpointer: %f, %f\n", ballstate, ballpointer[0], ballpointer[1]);

        // Goalkeeper movement
        double leftpost = width / 2 - 80;
        double rightpost = width / 2 + 80;
        if (gkpointer[1 - activeTeam][0] < leftpost)
            gkMoveDirection = 1; // Right
        else if (gkpointer[1 - activeTeam][0] > rightpost)
            gkMoveDirection = -1; // Left
        gkpointer[1 - activeTeam][0] += gkMoveDirection * gkspeed / 2;
        iSetSpritePosition(&gk[1 - activeTeam], gkpointer[1 - activeTeam][0] - playerradius, gkpointer[1 - activeTeam][1] - playerradius);
    }
void drawPenaltyScoreboard()
{
    char player_text[50];
    char opponent_text[50];
    sprintf(player_text,"Pla:\n");
    sprintf(opponent_text,"Opp:\n");
    iSetColor(255, 255, 255);
    iTextAdvanced(2*width/3+gallary, length - gallary/3, player_text, .1, 2);
    iTextAdvanced(2*width/3+gallary, length - 2*gallary/3, opponent_text, .1, 2);
}
void penaltyCollisionCheck(){
        if (ballstate == 0) {
        // Apply velocity
        ballpointer[0] += ballpointer[2];
        ballpointer[1] += ballpointer[3];
        // Friction
        ballpointer[2] *= varFriction;
        ballpointer[3] *= varFriction;
        iSetSpritePosition(&fball,
            ballpointer[0] - ballradius,
            ballpointer[1] - ballradius
        );

        // Check for GK collision or out‑of‑bounds → miss
        if ( checkcollision(
                ballpointer[0], ballpointer[1],
                gkpointer[1 - activeTeam][0], gkpointer[1 - activeTeam][1],
                playerradius + ballradius
            )
            || ballpointer[0] <= width/2 - 80
            || ballpointer[0] >= width/2 + 80
        ) {
            ballstate = 2;
            scoreMark = -1;
        }
        // Check for crossing the goal‑line
        else if ( ballpointer[1] >= length - gallary - playerradius-ballradius) {
            ballstate = 2;
            if ( ballpointer[0] > width/2 - 80 && ballpointer[0] < width/2 + 80 ) {
                scoreMark = 1;  // goal
                if (activeTeam == 0) scoreplayer++;
                else                scoreopp++;
            }
            else {
                scoreMark = -1;  // miss
                if (activeTeam == 0) scoreplayer++;
                else                scoreopp++;
            }
        }

        // If it’s almost stopped → settle
        if ( fabs(ballpointer[2]) < 0.001 && fabs(ballpointer[3]) < 0.001 )
            ballstate = 2;
        if (ballstate == 2 && scoreMark == 0) {
        // never scored or missed yet, so this is a miss
        scoreMark = -1;
}
    }
}
void penaltyScore() {

    penaltyCollisionCheck();
    if ( ballstate == 2 && scoreMark != 0 ) {
        // Compute X for the new marker
        double x = 2*width/3+gallary +40+ (activeTeam == 0 ? playerCount : oppCount) * 15;

        // Record into correct team’s array
        if ( activeTeam == 0 && playerCount < MAX_SHOTS ) {
            playerX[playerCount]      = x;
            playerResult[playerCount] = scoreMark;
            playerCount++;
        }
        else if ( activeTeam == 1 && oppCount < MAX_SHOTS ) {
            oppX[oppCount]      = x;
            oppResult[oppCount] = scoreMark;
            oppCount++;
        }

        // Swap sides and reset for next kick
        swapteams();
        Penalty();

        // Clear the flag so we don’t double‑record
        scoreMark = 0;
    }

    // 3) Always update & draw sprites
    spritepositionupdate();
    spriteshow();
}
void drawPenaltyMarkers() {
    // Player’s row (top):
    for (int i = 0; i < playerCount; i++) {
        if (playerResult[i] ==  1) { iSetColor(0,255,0); }
        else if (playerResult[i] == -1) { iSetColor(255,0,0); }
        else continue;  // skip zeros
        iFilledCircle(playerX[i], length - gallary/3, 6);
    }
    // Opponent’s row (below):
    for (int i = 0; i < oppCount; i++) {
        if (oppResult[i] ==  1) { iSetColor(0,255,0); }
        else if (oppResult[i] == -1) { iSetColor(255,0,0); }
        else continue;
        iFilledCircle(oppX[i], length - gallary*2/3, 6);
    }
    // reset drawing color for the rest of your code
    iSetColor(255,255,255);
}
/*int igetspeed()
{
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    frameCount++;
    if (previousFpsTime == 0)
    {
        previousFpsTime = currentTime; // Initialize on first call
        frameCount = 0;
    }
    else
    {
        int elapsedFpsTime = currentTime - previousFpsTime;

        if (elapsedFpsTime > 1000)
        {
            fps = (frameCount * 1000.0f) / elapsedFpsTime;
            frameCount = 0;
            previousFpsTime = currentTime;
        }
    }

    return fps;
}*/

void loadresources()
{
    
    iLoadImage(&backButton , "pause.png");

    iLoadFramesFromSheet(ball, "ball.png" ,1,1);
    iChangeSpriteFrames(&fball,ball,1);

    for(int n=0;n<6;n++)
    {
        //iInitSprite(&players[n],0xFFFFFF);
        iLoadFramesFromSheet(player, "player.png" ,1,1);
        iChangeSpriteFrames(&players[n],player,1);
        if(n==0)
        {
            //iInitSprite(&gk[n],0xFFFFFF);
            iChangeSpriteFrames(&gk[n],player,1);
            iSetSpritePosition(&gk[n],gkpointer[n][0],gkpointer[n][1]);
        }

        //iInitSprite(&opponents[n],-1);
        iLoadFramesFromSheet(opponent, "opponent.png" ,1,1);
        iChangeSpriteFrames(&opponents[n],opponent,1);
        if(n==1) 
        {
            //iInitSprite(&gk[n],-1);
            iChangeSpriteFrames(&gk[n],opponent,1);
            iSetSpritePosition(&gk[n],gkpointer[n][0],gkpointer[n][1]);
        }
        iSetSpritePosition(&players[n],playerposition[1][n][0],playerposition[1][n][1]);
        iSetSpritePosition(&opponents[n],playerposition[0][n][0],playerposition[0][n][1]);

        
        iLoadFramesFromSheet(&pauseButton , "pause.png", 1,1);
        iChangeSpriteFrames(&pauseButtons, &pauseButton, 1);
    }
}

void resetvariables()
{
    memcpy(ballpointer,newball,4*sizeof(double));
    memcpy(gkpointer,newgk,4*sizeof(double));
    memcpy(playerposition, newplayerposition,2*6*4*sizeof(double)) ;
    activeplayer=5;ballstate=1;ballholder=5;
    gametime=0;
}

int playerOnTheWay(double x1,double y1,double x3,double y3,double x2,double y2,double r)
{
    if((abs((int)((y2-y1)*x3-(x2-x1)*y3+y1*(x2-x1)-x1*(y2-y1)))/ sqrt((y2-y1)*(y2-y1)+(x2-x1)*(x2-x1)))>3*r)
        return 0;
    else
    {
        if(((x3>=x1 && x2>=x3) || (x3<=x1 && x2<=x3)) && ((y3>=y1 && y2>=y3) || (y3<=y1 && y2<=y3)))
            return 1;
        else
            return -1;
    }
}

void choosePosition(int t,int m, double maxX, double minX, double maxY, double minY, double rangeX, double rangeY)//int accuracy, int urgency
{
    //if(ballstate==t*2-1 || ballstate==0)
    {
        //X axis movement
        if(ballpointer[0]>=maxX)
        {
            if(ballpointer[0]-rangeX>=maxX)
                playerposition[t][m][2]=maxX - rand() % dX;
            else
                playerposition[t][m][2]=maxX - rand() % (int)(rangeX-ballpointer[0]+maxX+1);
            
        }
        else if(ballpointer[0]<=minX)
        {
            if(ballpointer[0]+rangeX <= minX)
                playerposition[t][m][2]=minX + rand() % dX;
            else
                playerposition[t][m][2]=minX+ rand() % (int)(ballpointer[0]+rangeX-minX+1);
            
        }
        else
        {
            if(ballpointer[0]>=minX+2*(maxX-minX)/3)
                playerposition[t][m][2]=ballpointer[0] - rand() % (int)rangeX;
            else if(ballpointer[0]<= minX+(maxX-minX)/3)
                playerposition[t][m][2]=ballpointer[0] + rand() % (int)rangeX;
            else
                playerposition[t][m][2]=minX + rand() % (int) (maxX-minX);
        }

        //Y axis movement
        if(ballpointer[1]>=maxY)
        {
            if(ballpointer[1]-rangeY >= maxY)
                playerposition[t][m][3]=maxY - rand() % dY;
            else
                playerposition[t][m][3]=maxY- rand() % (int)(rangeY-ballpointer[1]+maxY+1);
            
        }
        else if(ballpointer[1]<=minY)
        {
            if(ballpointer[1]+rangeY <= minY)
                playerposition[t][m][3]=minY + rand() % dY;
            else
                playerposition[t][m][3]=minY+ rand() % (int)(ballpointer[1]+rangeY-minY+1);
            
        }
        else
        {
            if(ballstate==1)
            {
                if(ballpointer[1]+rangeY<=maxY)
                    playerposition[t][m][3]=ballpointer[1]+rand() % (int)(rangeY);
                else
                    playerposition[t][m][3]=ballpointer[1]+rand() % (int)(maxY-ballpointer[1]+1);
                
            }
            else if(ballstate==-1)
            {
                if(ballpointer[1]-rangeY>=minY)
                    playerposition[t][m][3]=ballpointer[1]- rand() % (int)(rangeY);
                else
                    playerposition[t][m][3]=ballpointer[1]-rand() % (int)(ballpointer[1]-minY+1);
                
            }
        }
    }
    /*else
    {
        if(ballpointer[0]==playerposition[!t][5-m][0])
            playerposition[t][m][2]=ballpointer[0];
        else if(ballpointer[0]<playerposition[!t][5-m][0])
        {
            if(ballpointer[0]<minX)
                playerposition[t][m][2]=playerposition[!t][5-m][0]- rand() % (int)(playerposition[!t][5-m][0]-minX+1);
            else if(ballpointer[0]>minX)
                playerposition[t][m][2]=playerposition[!t][5-m][0]+ rand() % (int)(playerposition[!t][5-m][0] - ballpointer[0] + 1);
            //playerposition[t][m][2]=ballpointer[0]+ rand() % (int)(playerposition[!t][5-m][0]-ballpointer[0]+1);
        }
        else if(ballpointer[0]>playerposition[!t][5-m][0])
        {
            if(ballpointer[0]>maxX)
                playerposition[t][m][2]=playerposition[!t][5-m][0]+ rand() % (int)(maxX-playerposition[!t][5-m][0]+1);
            else if(ballpointer[0]>maxX)
                playerposition[t][m][2]=playerposition[!t][5-m][0]+ rand() % (int)(ballpointer[0] - playerposition[!t][5-m][0]+1);
        }

        playerposition[t][m][3]=ballpointer[1]+(playerposition[t][m][2]-ballpointer[0])*(playerposition[!t][5-m][1]-ballpointer[1])/(playerposition[!t][5-m][0]-ballpointer[0]) ;//+ pow(-1,(rand()-rand()))*15 ;
    }*/

}

bool timetoChoosePosition(int t, int m)
{
    //if(playerposition[team][m][0]<playerposition[team][m][2]+dxy && playerposition[team][m][0]>playerposition[team][m][2]-dxy && playerposition[team][m][1]<playerposition[team][m][3]+dxy && playerposition[team][m][1]>playerposition[team][m][3]-dxy)
    if((playerposition[t][m][2]-playerposition[t][m][0])*(playerposition[t][m][2]-playerposition[t][m][0])+(playerposition[t][m][3]-playerposition[t][m][1])*(playerposition[t][m][3]-playerposition[t][m][1])<dxy*dxy)
        return 1;
    else
        return 0;
}

void chooseAndTakePosition()
{
    for(int j = 0; j < 6; j++)
    {
        if(activeplayer != j)
        {
            if (timetoChoosePosition(1,j))
            {
                if ((int)timer % (3000 + rand() % 3000) <= 10)
                    choosePosition(1,j,positionField[1][j][0],positionField[1][j][1],positionField[1][j][2],positionField[1][j][3],5*width/20,5*length/20); // team 1
            }
            else
            {
                if((playerposition[1][j][2]-playerposition[1][j][0])*(playerposition[1][j][2]-playerposition[1][j][0])+(playerposition[1][j][3]-playerposition[1][j][1])*(playerposition[1][j][3]-playerposition[1][j][1])>0)
                {
                    playerposition[1][j][0]+= (constSpeed) *(playerposition[1][j][2]-playerposition[1][j][0])/sqrt(1+(playerposition[1][j][2]-playerposition[1][j][0])*(playerposition[1][j][2]-playerposition[1][j][0])+(playerposition[1][j][3]-playerposition[1][j][1])*(playerposition[1][j][3]-playerposition[1][j][1]));
                    playerposition[1][j][1]+= (constSpeed) *(playerposition[1][j][3]-playerposition[1][j][1])/sqrt(1+(playerposition[1][j][2]-playerposition[1][j][0])*(playerposition[1][j][2]-playerposition[1][j][0])+(playerposition[1][j][3]-playerposition[1][j][1])*(playerposition[1][j][3]-playerposition[1][j][1]));
                }
            }
        }
        else
        {
            playerposition[1][j][2]=playerposition[1][j][0];
            playerposition[1][j][3]=playerposition[1][j][1];
        }
        if(activeplayeropp != j)
        {
            if (timetoChoosePosition(0,j))
            {
                if ((int)timer % (3000 + rand() % 3000) <= 100)
                    choosePosition(0,j,positionField[0][j][0],positionField[0][j][1],positionField[0][j][2],positionField[0][j][3],4*width/20,5*length/20); // team 0
            }
            else
            {
                if((playerposition[0][j][2]-playerposition[0][j][0])*(playerposition[0][j][2]-playerposition[0][j][0])+(playerposition[0][j][3]-playerposition[0][j][1])*(playerposition[0][j][3]-playerposition[0][j][1])>0)
                {
                    playerposition[0][j][0]+= constSpeed*(playerposition[0][j][2]-playerposition[0][j][0])/sqrt(1+(playerposition[0][j][2]-playerposition[0][j][0])*(playerposition[0][j][2]-playerposition[0][j][0])+(playerposition[0][j][3]-playerposition[0][j][1])*(playerposition[0][j][3]-playerposition[0][j][1]));
                    playerposition[0][j][1]+= constSpeed*(playerposition[0][j][3]-playerposition[0][j][1])/sqrt(1+(playerposition[0][j][2]-playerposition[0][j][0])*(playerposition[0][j][2]-playerposition[0][j][0])+(playerposition[0][j][3]-playerposition[0][j][1])*(playerposition[0][j][3]-playerposition[0][j][1])); 
                }
            }
        }
    }
}

void handlecollission()
{
    if(ballstate==0)
    {
        if(checkcollision(gkpointer[0][0],gkpointer[0][1],ballpointer[0],ballpointer[1],ballradius+playerradius))
            {
                ballstate=1;
                ballholder=-1;
                lastTouch=1;
            }
        else if(checkcollision(gkpointer[1][0],gkpointer[1][1],ballpointer[0],ballpointer[1],ballradius+playerradius))
            {
                ballstate=-1;
                ballholder=-1;
                lastTouch=-1;
            }
    }
    for(i=0;i<6;i++)
    {
        if(ballstate==0)
        {
            if(checkcollision(playerposition[1][i][0],playerposition[1][i][1],ballpointer[0],ballpointer[1],ballradius+playerradius))
            {
                ballstate=1;
                ballholder=i;
                lastTouch=1;
            }
            else if(checkcollision(playerposition[0][i][0],playerposition[0][i][1],ballpointer[0],ballpointer[1],ballradius+playerradius))
            {
                ballstate=-1;
                ballholder=i;
                lastTouch=-1;
            }
        }
        if(checkcollision(playerposition[1][i][0],playerposition[1][i][1],gkpointer[0][0],gkpointer[0][1],2*playerradius))
        {
            if(i==ballholder && ballstate==1)
            {
                ballstate=1;
                ballholder=-1;
            }
            if(playerposition[1][i][0]>gkpointer[0][0])
                playerposition[1][i][0]+=3;
            else if(playerposition[1][i][0]<gkpointer[0][0])
                playerposition[1][i][0]-=3;
            if(playerposition[1][i][1]>gkpointer[0][1])
                playerposition[1][i][1]+=3;
            else if(playerposition[1][i][1]<gkpointer[0][1])
                playerposition[1][i][1]-=3;
        }
        else if(checkcollision(playerposition[1][i][0],playerposition[1][i][1],gkpointer[1][0],gkpointer[1][1],2*playerradius))
        {
            if(i==ballholder && ballstate==1)
            {
                ballstate=-1;
                ballholder=-1;
            }
            if(playerposition[1][i][0]>gkpointer[1][0])
                playerposition[1][i][0]+=3;
            else if(playerposition[1][i][0]<gkpointer[1][0])
                playerposition[1][i][0]-=3;
            if(playerposition[1][i][1]>gkpointer[1][1])
                playerposition[1][i][1]+=3;
            else if(playerposition[1][i][1]<gkpointer[1][1])
                playerposition[1][i][1]-=3;
        }
        if(checkcollision(playerposition[0][i][0],playerposition[0][i][1],gkpointer[0][0],gkpointer[0][1],2*playerradius))
        {
            if(i==ballholder && ballstate==-1)
            {
                ballstate=1;
                ballholder=-1;
            }
            if(playerposition[0][i][0]>gkpointer[0][0])
                playerposition[0][i][0]+=3;
            else if(playerposition[0][i][0]<gkpointer[0][0])
                playerposition[0][i][0]-=3;
            if(playerposition[0][i][1]>gkpointer[0][1])
                playerposition[0][i][1]+=3;
            else if(playerposition[0][i][1]<gkpointer[0][1])
                playerposition[0][i][1]-=3;
        }
        else if(checkcollision(playerposition[0][i][0],playerposition[0][i][1],gkpointer[1][0],gkpointer[1][1],2*playerradius))
        {
            if(i==ballholder && ballstate==-1)
            {
                ballstate=-1;
                ballholder=-1;
            }
            if(playerposition[0][i][0]>gkpointer[1][0])
                playerposition[0][i][0]+=3;
            else if(playerposition[0][i][0]<gkpointer[1][0])
                playerposition[0][i][0]-=3;
            if(playerposition[0][i][1]>gkpointer[1][1])
                playerposition[0][i][1]+=3;
            else if(playerposition[0][i][1]<gkpointer[1][1])
                playerposition[0][i][1]-=3;
        }
        
        for(ii=0;ii<6;ii++)
        {
            if(checkcollision(playerposition[1][i][0],playerposition[1][i][1],playerposition[1][ii][0],playerposition[1][ii][1],2*playerradius))
            {
                if(ballstate==1)
                {
                    if(ballholder==i)
                        ballholder=ii;
                    else if(ballholder==ii)
                        ballholder=i;
                }
                if(playerposition[1][i][0]>playerposition[1][ii][0])
                {
                    playerposition[1][i][0]+=rand()% (int)collidepos;
                    playerposition[1][ii][0]-=rand()% (int)collidepos;
                }
                else if(playerposition[1][i][0]<playerposition[1][ii][0])
                {
                    playerposition[1][i][0]-=rand()% (int)collidepos;
                    playerposition[1][ii][0]+=rand()% (int)collidepos;
                }
                if(playerposition[1][i][1]>playerposition[1][ii][1])
                {
                    playerposition[1][i][1]+=rand()% (int)collidepos;
                    playerposition[1][ii][1]-=rand()% (int)collidepos;
                }
                else if(playerposition[1][i][1]<playerposition[1][ii][1])
                {
                    playerposition[1][i][1]-=rand()% (int)collidepos;
                    playerposition[1][ii][1]+=rand()% (int)collidepos;
                }
            }
            if(checkcollision(playerposition[0][i][0],playerposition[0][i][1],playerposition[0][ii][0],playerposition[0][ii][1],2*playerradius))
            {
                if(ballstate==-1)
                {
                    if(ballholder==i)
                        ballholder=ii;
                    else if(ballholder==ii)
                        ballholder=i;
                }
                if(playerposition[0][i][0]>playerposition[0][ii][0])
                {
                    playerposition[0][i][0]+=rand()% (int)collidepos;
                    playerposition[0][ii][0]-=rand()% (int)collidepos;
                }
                else if(playerposition[0][i][0]<playerposition[0][ii][0])
                {
                    playerposition[0][i][0]-=rand()% (int)collidepos;
                    playerposition[0][ii][0]+=rand()% (int)collidepos;
                }
                if(playerposition[0][i][1]>playerposition[0][ii][1])
                {
                    playerposition[0][i][1]+=rand()% (int)collidepos;
                    playerposition[0][ii][1]-=rand()% (int)collidepos;
                }
                else if(playerposition[0][i][1]<playerposition[0][ii][1])
                {
                    playerposition[0][i][1]-=rand()% (int)collidepos;
                    playerposition[0][ii][1]+=rand()% (int)collidepos;
                }
            }
            if(checkcollision(playerposition[1][i][0],playerposition[1][i][1],playerposition[0][ii][0],playerposition[0][ii][1],2*playerradius))
            {
                if(ballholder!=-1)
                {
                    if(ballstate==1 && ballholder==i)
                    {
                        ballstate=-1;
                        ballholder=ii;
                    }
                    else if(ballstate==-1 && ballholder==ii)
                    {
                        ballstate=1;
                        ballholder=i;
                    }
                }
                if(playerposition[1][i][0]>playerposition[0][ii][0])
                {
                    playerposition[1][i][0]+=rand()% (int)collidepos;
                    playerposition[0][ii][0]-=rand()% (int)collidepos;
                }
                else if(playerposition[1][i][0]<playerposition[0][ii][0])
                {
                    playerposition[1][i][0]-=rand()% (int)collidepos;
                    playerposition[0][ii][0]+=rand()% (int)collidepos;
                }
                if(playerposition[1][i][1]>playerposition[0][ii][1])
                {
                    playerposition[1][i][1]+=rand()% (int)collidepos;
                    playerposition[0][ii][1]-=rand()% (int)collidepos;
                }
                else if(playerposition[1][i][1]<playerposition[0][ii][1])
                {
                    playerposition[1][i][1]-=rand()% (int)collidepos;
                    playerposition[0][ii][1]+=rand()% (int)collidepos;
                }
            }
        }
    }
    if(ballstate!=0)
    {
        lastTouch=ballstate/abs(ballstate);
    }
}

void chooseactiveplayer()
{
    if(ballstate==1 || ballstate==2)
    {
        activeplayer=ballholder;
    }
    else if(ballstate==0 || ballstate==-1 || ballstate==-2)
    {
        helpingplayer=activeplayer==0?1:0;
        for(i=0;i<6;i++)
        {
            if(i==activeplayer)
                continue;
            if((ballpointer[0]-playerposition[1][helpingplayer][0])*(ballpointer[0]-playerposition[1][helpingplayer][0])+(ballpointer[1]-playerposition[1][helpingplayer][1])*(ballpointer[1]-playerposition[1][helpingplayer][1])>(ballpointer[0]-playerposition[1][i][0])*(ballpointer[0]-playerposition[1][i][0])+(ballpointer[1]-playerposition[1][i][1])*(ballpointer[1]-playerposition[1][i][1]))
                helpingplayer=i;
        }
        if(isKeyPressed('o'))
            activeplayer=helpingplayer++;
    }
    
}

void chooseopponentactiveplayer()
{
    if(ballstate==-1 || ballstate==-2)
    {
        activeplayeropp=ballholder;
    }
    else if(ballstate==0 || ballstate==1 || ballstate==2)
    {
        helpingplayeropp=activeplayeropp==0?1:0;
        for(i=0;i<6;i++)
        {
            if(i==activeplayeropp)
                continue;
            if((ballpointer[0]-playerposition[0][helpingplayeropp][0])*(ballpointer[0]-playerposition[0][helpingplayeropp][0])+(ballpointer[1]-playerposition[0][helpingplayeropp][1])*(ballpointer[1]-playerposition[0][helpingplayeropp][1])>(ballpointer[0]-playerposition[0][i][0])*(ballpointer[0]-playerposition[0][i][0])+(ballpointer[1]-playerposition[0][i][1])*(ballpointer[1]-playerposition[0][i][1]))
                helpingplayeropp=i;
        }
        if(isKeyPressed('c'))
            activeplayeropp=helpingplayeropp++;
    }
}

void drawScoreboard()
{
    char score_text[50];
    char time_text[20];
    int minutes = gametime / 60000; // Convert milliseconds to minutes
    int seconds = (gametime / 1000) % 60; // Convert milliseconds to seconds
    sprintf(score_text, "Pla %d - %d Opp", scoreplayer, scoreopp);
    sprintf(time_text, "Time: %02d:%02d", minutes, seconds);
    //iSetColor(0,0,255);
    //iFilledRectangle(0,length-gallary,100,gallary);
    iSetColor(255, 255, 255);
    iTextAdvanced(10, length - 30, score_text , .1, 2);
    iTextAdvanced(10, length - 15, time_text, .1, 2);
}

void throwCornerOutGoal()
{
    //throw
    if(ballpointer[0]<gallary-ballradius || ballpointer[0]>width-gallary+ballradius)
    {
        iDelay(2);
        if(ballstate!=0)
            playerposition[(lastTouch+1)/2][ballholder][0]+=((width/2-playerposition[(lastTouch+1)/2][ballholder][0])/(width/2-gallary))*100;
        ballstate=lastTouch==1?-2:2;
        ballholder=-lastTouch==1?activeplayer:activeplayeropp;
        playerposition[(-lastTouch+1)/2][ballholder][0]=ballpointer[0]>width/2?ballpointer[0]-ballradius:ballpointer[0]+ballradius;
        playerposition[(-lastTouch+1)/2][ballholder][1]=ballpointer[1];
    }
    //out or corner
    if((ballpointer[1]<gallary-ballradius || ballpointer[1]>length-gallary+ballradius))
    {
        if(ballpointer[0]<width/2-goalbar/2+ballradius/2 || ballpointer[0]>width/2+ goalbar/2-ballradius/2)
        {
            iDelay(2);
            if(ballstate!=0)
                playerposition[(lastTouch+1)/2][ballholder][1]+=((length/2-playerposition[(lastTouch+1)/2][ballholder][1])/(length/2-gallary))*200;
            if(ballpointer[1]<gallary-ballradius)
            {
                if(lastTouch==1)
                {
                    ballstate=-2;
                    ballholder=activeplayeropp==-1?helpingplayeropp:activeplayeropp;
                    playerposition[0][ballholder][0]=ballpointer[0]<width/2?gallary:width-gallary;
                    playerposition[0][ballholder][1]=gallary;
                }
                else if(lastTouch==-1)
                {
                    ballstate=1;
                    ballholder=-1;
                }
            }
            else if(ballpointer[1]>length-gallary+ballradius)
            {
                if(lastTouch==-1)
                {
                    ballstate=2;
                    ballholder=activeplayer==-1?helpingplayer:activeplayer;
                    playerposition[1][ballholder][0]=ballpointer[0]<width/2?gallary:width-gallary;
                    playerposition[1][ballholder][1]=length-gallary;
                }
                else if(lastTouch==1)
                {
                    ballstate=-1;
                    ballholder=-1;
                }
            }
        }
        //goal
        else if(ballpointer[0]>width/2-goalbar/2+ballradius/2 && ballpointer[0]<width/2+ goalbar/2-ballradius/2)
        {
            iDelay(3);
            memcpy(gkpointer,newgk,4*sizeof(double));
            memcpy(playerposition, newplayerposition,2*6*4*sizeof(double)) ;
            if(ballpointer[1]<gallary-ballradius)
            {
                scoreopp+=1;
                ballstate=2;ballholder=5;
            }
            else if(ballpointer[1]>length-gallary+ballradius)
            {
                scoreplayer+=1;
                playerposition[0][5][0]=width/2;
                playerposition[0][5][1]=length/2;
                playerposition[1][5][0]=width/2;
                playerposition[1][5][1]=4*length/10;
                ballstate=-2;ballholder=5;
            }
            memcpy(ballpointer,newball,4*sizeof(double));
        }
    }
}


void timeUpdater(){
    printf("%d  %.3lf  %d %d\n",fps,timer,sec,minit);
    if(fps!=0)
        timer=timer+(120.0/(fps));
    sec=timer/120;
    minit=timer/7200;
    if((page_number==1 || page_number==2) && !(ballstate==2 || ballstate==-2))
    {
        gametime+=1;
    }
}

void gkmoving(){
    if(ballholder!=-1 || (ballholder==-1 && ballstate==0))
    {
        if(ballpointer[1]<length/2)
        {
            if(gkpointer[0][0]-ballpointer[0]>0)
                gkpointer[0][0]=gkpointer[0][0]-gkspeed;
            else if(gkpointer[0][0]-ballpointer[0]<0)
                gkpointer[0][0]=gkpointer[0][0]+gkspeed;
        }
        if(ballpointer[1]>length/2)
        {
            if(gkpointer[1][0]-ballpointer[0]>0)
                gkpointer[1][0]=gkpointer[1][0]-gkspeed;
            else if(gkpointer[1][0]-ballpointer[0]<0)
                gkpointer[1][0]=gkpointer[1][0]+gkspeed;
        }
    }
    if(gkpointer[0][0]<width/2-goalbar/2)
        gkpointer[0][0]=width/2-goalbar/2;
    else if(gkpointer[0][0]>width/2+goalbar/2)
        gkpointer[0][0]=width/2+goalbar/2;
    if(gkpointer[1][0]<width/2-goalbar/2)
        gkpointer[1][0]=width/2-goalbar/2;
    else if(gkpointer[1][0]>width/2+goalbar/2)
        gkpointer[1][0]=width/2+goalbar/2;
    
}

void activepassing()
{
    if(ballstate==1 || ballstate==2)
    {
        if(isKeyPressed('p'))
            passedballspeed=passedballspeed;
        else if(isKeyPressed('l'))
            passedballspeed=shootballspeed;
        
        if(isKeyPressed('p') || isKeyPressed('l'))
        {
            ballpointer[2]=0;
            ballpointer[3]=0;
            varAcceleration[0]=0;
            varAcceleration[1]=0;
            if(isSpecialKeyPressed(GLUT_KEY_UP)){
                ballstate=0;
                if(isSpecialKeyPressed(GLUT_KEY_LEFT)){
                    ballpointer[0]-=playerradius*4/6;
                    ballpointer[1]+=playerradius*4/6;
                    ballpointer[2]=-passedballspeed/1.4;
                    ballpointer[3]=passedballspeed/1.4;
                    varAcceleration[0]=-acceleration/1.4;
                    varAcceleration[1]=acceleration/1.4;
                }
                else if(isSpecialKeyPressed(GLUT_KEY_RIGHT)){
                    ballpointer[0]+=playerradius*4/6;
                    ballpointer[1]+=playerradius*4/6;
                    ballpointer[2]=passedballspeed/1.4;
                    ballpointer[3]=passedballspeed/1.4;
                    varAcceleration[0]=acceleration/1.4;
                    varAcceleration[1]=acceleration/1.4;
                }
                else{
                    ballpointer[1]+=playerradius*10/6;
                    ballpointer[3]=passedballspeed;
                    varAcceleration[1]=acceleration;
                }
            }
            if(isSpecialKeyPressed( GLUT_KEY_DOWN )){
                ballstate=0;
                if(isSpecialKeyPressed(GLUT_KEY_LEFT)){
                    ballpointer[0]-=playerradius*4/6;
                    ballpointer[1]-=playerradius*4/6;
                    ballpointer[2]=-passedballspeed/1.4;
                    ballpointer[3]=-passedballspeed/1.4;
                    varAcceleration[0]=-acceleration/1.4;
                    varAcceleration[1]=-acceleration/1.4;
                }
                else if(isSpecialKeyPressed(GLUT_KEY_RIGHT)){
                    ballpointer[0]+=playerradius*4/6;
                    ballpointer[1]-=playerradius*4/6;
                    ballpointer[2]=passedballspeed/1.4;
                    ballpointer[3]=-passedballspeed/1.4;
                    varAcceleration[0]=acceleration/1.4;
                    varAcceleration[1]=acceleration/1.4;
                }
                else{
                    ballpointer[1]-=playerradius*10/6;
                    ballpointer[3]=-passedballspeed;
                    varAcceleration[1]=-acceleration;
                }
            }
            if(isSpecialKeyPressed( GLUT_KEY_RIGHT)){
                ballstate=0;
                if(isSpecialKeyPressed(GLUT_KEY_UP)){
                    ballpointer[0]+=playerradius*4/6;
                    ballpointer[1]+=playerradius*4/6;
                    ballpointer[2]=passedballspeed/1.4;
                    ballpointer[3]=passedballspeed/1.4;
                    varAcceleration[0]=acceleration/1.4;
                    varAcceleration[1]=acceleration/1.4;
                }
                else if(isSpecialKeyPressed(GLUT_KEY_DOWN)){
                    ballpointer[0]+=playerradius*4/6;
                    ballpointer[1]-=playerradius*4/6;
                    ballpointer[2]=passedballspeed/1.4;
                    ballpointer[3]=-passedballspeed/1.4;
                    varAcceleration[0]=acceleration/1.4;
                    varAcceleration[1]=-acceleration/1.4;
                }
                else{
                    ballpointer[0]+=playerradius*10/6;
                    ballpointer[2]=passedballspeed;
                    varAcceleration[0]=acceleration;
                }
            }
            if(isSpecialKeyPressed( GLUT_KEY_LEFT)){
                ballstate=0;
                if(isSpecialKeyPressed(GLUT_KEY_UP)){
                    ballpointer[0]-=playerradius*4/6;
                    ballpointer[1]+=playerradius*4/6;
                    ballpointer[2]=-passedballspeed/1.4;
                    ballpointer[3]=passedballspeed/1.4;
                    varAcceleration[0]=-acceleration/1.4;
                    varAcceleration[1]=acceleration/1.4;
                }
                else if(isSpecialKeyPressed(GLUT_KEY_DOWN)){
                    ballpointer[0]-=playerradius*4/6;
                    ballpointer[1]-=playerradius*4/6;
                    ballpointer[2]=-passedballspeed/1.4;
                    ballpointer[3]=-passedballspeed/1.4;
                    varAcceleration[0]=-acceleration/1.4;
                    varAcceleration[1]=-acceleration/1.4;
                }
                else{
                    ballpointer[0]-=ballradius*10/6;
                    ballpointer[2]=-passedballspeed;
                    varAcceleration[0]=-acceleration;
                }
            }
            }
    }
}

void opponentpassing()
{
    if(ballstate==-1 || ballstate==-2)
    {
        if(isKeyPressed('t'))
            passedballspeed=passedballspeed;
        else if(isKeyPressed('g'))
            passedballspeed=shootballspeed;
        
        if(isKeyPressed('t') || isKeyPressed('g'))
        {
            ballpointer[2]=0;
            ballpointer[3]=0;
            varAcceleration[0]=0;
            varAcceleration[1]=0;
            if(isKeyPressed('w')){
                ballstate=0;
                if(isKeyPressed('a')){
                    ballpointer[0]-=playerradius*4/6;
                    ballpointer[1]+=playerradius*4/6;
                    ballpointer[2]=-passedballspeed/1.4;
                    ballpointer[3]=passedballspeed/1.4;
                    varAcceleration[0]=-acceleration/1.4;
                    varAcceleration[1]=acceleration/1.4;
                }
                else if(isKeyPressed('d')){
                    ballpointer[0]+=playerradius*4/6;
                    ballpointer[1]+=playerradius*4/6;
                    ballpointer[2]=passedballspeed/1.4;
                    ballpointer[3]=passedballspeed/1.4;
                    varAcceleration[0]=acceleration/1.4;
                    varAcceleration[1]=acceleration/1.4;
                }
                else{
                    ballpointer[1]+=playerradius*10/6;
                    ballpointer[3]=passedballspeed;
                    varAcceleration[1]=acceleration;
                }
            }
            if(isKeyPressed( 's' )){
                ballstate=0;
                if(isKeyPressed('a')){
                    ballpointer[0]-=playerradius*4/6;
                    ballpointer[1]-=playerradius*4/6;
                    ballpointer[2]=-passedballspeed/1.4;
                    ballpointer[3]=-passedballspeed/1.4;
                    varAcceleration[0]=-acceleration/1.4;
                    varAcceleration[1]=-acceleration/1.4;
                }
                else if(isKeyPressed('d')){
                    ballpointer[0]+=playerradius*4/6;
                    ballpointer[1]-=playerradius*4/6;
                    ballpointer[2]=passedballspeed/1.4;
                    ballpointer[3]=-passedballspeed/1.4;
                    varAcceleration[0]=acceleration/1.4;
                    varAcceleration[1]=acceleration/1.4;
                }
                else{
                    ballpointer[1]-=playerradius*10/6;
                    ballpointer[3]=-passedballspeed;
                    varAcceleration[1]=-acceleration;
                }
            }
            if(isKeyPressed( 'd')){
                ballstate=0;
                if(isKeyPressed('w')){
                    ballpointer[0]+=playerradius*4/6;
                    ballpointer[1]+=playerradius*4/6;
                    ballpointer[2]=passedballspeed/1.4;
                    ballpointer[3]=passedballspeed/1.4;
                    varAcceleration[0]=acceleration/1.4;
                    varAcceleration[1]=acceleration/1.4;
                }
                else if(isKeyPressed('s')){
                    ballpointer[0]+=playerradius*4/6;
                    ballpointer[1]-=playerradius*4/6;
                    ballpointer[2]=passedballspeed/1.4;
                    ballpointer[3]=-passedballspeed/1.4;
                    varAcceleration[0]=acceleration/1.4;
                    varAcceleration[1]=-acceleration/1.4;
                }
                else{
                    ballpointer[0]+=playerradius*10/6;
                    ballpointer[2]=passedballspeed;
                    varAcceleration[0]=acceleration;
                }
            }
            if(isKeyPressed( 'a')){
                ballstate=0;
                if(isKeyPressed('w')){
                    ballpointer[0]-=playerradius*4/6;
                    ballpointer[1]+=playerradius*4/6;
                    ballpointer[2]=-passedballspeed/1.4;
                    ballpointer[3]=passedballspeed/1.4;
                    varAcceleration[0]=-acceleration/1.4;
                    varAcceleration[1]=acceleration/1.4;
                }
                else if(isKeyPressed('s')){
                    ballpointer[0]-=playerradius*4/6;
                    ballpointer[1]-=playerradius*4/6;
                    ballpointer[2]=-passedballspeed/1.4;
                    ballpointer[3]=-passedballspeed/1.4;
                    varAcceleration[0]=-acceleration/1.4;
                    varAcceleration[1]=-acceleration/1.4;
                }
                else{
                    ballpointer[0]-=ballradius*10/6;
                    ballpointer[2]=-passedballspeed;
                    varAcceleration[0]=-acceleration;
                }
            }
            }
    }
}

void activeplayermoveing()
{
    if(isKeyPressed('k'))
        speed=constSpeed*2;
    else
        speed=constSpeed;
    if((int)timer%25==0)chooseactiveplayer();
    if(activeplayer==-1)
    {
        if(isSpecialKeyPressed( GLUT_KEY_LEFT))
            gkpointer[0][0]=gkpointer[0][0]-gkspeed;
        else if(isSpecialKeyPressed( GLUT_KEY_RIGHT))
            gkpointer[0][0]=gkpointer[0][0]+gkspeed;
    }
    else if(isSpecialKeyPressed(GLUT_KEY_UP) || isSpecialKeyPressed( GLUT_KEY_DOWN ))
    {
        if(isSpecialKeyPressed(GLUT_KEY_UP)){
            if(isSpecialKeyPressed(GLUT_KEY_LEFT)){
                playerposition[1][activeplayer][0]-=speed/1.4;
                playerposition[1][activeplayer][1]+=speed/1.4;
            }
            else if(isSpecialKeyPressed(GLUT_KEY_RIGHT)){
                playerposition[1][activeplayer][0]+=speed/1.4;
                playerposition[1][activeplayer][1]+=speed/1.4;
            }
            else
                playerposition[1][activeplayer][1]+=speed;
        }
        if(isSpecialKeyPressed( GLUT_KEY_DOWN )){
            if(isSpecialKeyPressed(GLUT_KEY_LEFT)){
                playerposition[1][activeplayer][0]-=speed/1.4;
                playerposition[1][activeplayer][1]-=speed/1.4;
            }
            else if(isSpecialKeyPressed(GLUT_KEY_RIGHT)){
                playerposition[1][activeplayer][0]+=speed/1.4;
                playerposition[1][activeplayer][1]-=speed/1.4;
            }
            else
                playerposition[1][activeplayer][1]-=speed;
        }
    }
    else if(isSpecialKeyPressed( GLUT_KEY_RIGHT)){
        playerposition[1][activeplayer][0]+=speed;
    }
    else if(isSpecialKeyPressed( GLUT_KEY_LEFT)){
        playerposition[1][activeplayer][0]-=speed;
    }
}

void opponentplayermoveing()
{
    if(isKeyPressed('f'))
        speed=constSpeed*2;
    else
        speed=constSpeed;
    if((int)timer%25==0)chooseopponentactiveplayer();
    if(activeplayeropp==-1)
    {
        if(isKeyPressed( 'a'))
            gkpointer[1][0]=gkpointer[1][0]-gkspeed;
        else if(isKeyPressed( 'd'))
            gkpointer[1][0]=gkpointer[1][0]+gkspeed;
    }
    else if(isKeyPressed('w') || isKeyPressed( 's' ))
    {
        if(isKeyPressed('w')){
            if(isKeyPressed('a')){
                playerposition[0][activeplayeropp][0]-=speed/1.4;
                playerposition[0][activeplayeropp][1]+=speed/1.4;
            }
            else if(isKeyPressed('d')){
                playerposition[0][activeplayeropp][0]+=speed/1.4;
                playerposition[0][activeplayeropp][1]+=speed/1.4;
            }
            else
                playerposition[0][activeplayeropp][1]+=speed;
        }
        else if(isKeyPressed( 's' )){
            if(isKeyPressed('a')){
                playerposition[0][activeplayeropp][0]-=speed/1.4;
                playerposition[0][activeplayeropp][1]-=speed/1.4;
            }
            else if(isKeyPressed('d')){
                playerposition[0][activeplayeropp][0]+=speed/1.4;
                playerposition[0][activeplayeropp][1]-=speed/1.4;
            }
            else
                playerposition[0][activeplayeropp][1]-=speed;
        }
    }
    else if(isKeyPressed( 'd')){
        playerposition[0][activeplayeropp][0]+=speed;
    }
    else if(isKeyPressed( 'a')){
        playerposition[0][activeplayeropp][0]-=speed;
    }
}

void ballposition()
{
    if(ballstate==1 || ballstate==2)
    {
        if(ballholder==-1)
        {
            ballpointer[0]=gkpointer[0][0];
            ballpointer[1]=gkpointer[0][1];
        }
        else
        {
            ballpointer[0]=playerposition[1][ballholder][0];
            ballpointer[1]=playerposition[1][ballholder][1];
        }
    }
    else if(ballstate==-1 || ballstate==-2)
    {
        if (ballholder==-1)
        {
            ballpointer[0]=gkpointer[1][0];
            ballpointer[1]=gkpointer[1][1];
        }
        else
        {
            ballpointer[0]=playerposition[0][ballholder][0];
            ballpointer[1]=playerposition[0][ballholder][1];
        }
    }
    else if(ballstate==0)
    {
        if(!(ballpointer[2]==0 || (varAcceleration[0]>0 && ballpointer[2]>0) || (varAcceleration[0]<0 && ballpointer[2]<0)))
        {
            ballpointer[0]+=ballpointer[2];
            ballpointer[2]+=varAcceleration[0];
        }
        if(!(ballpointer[3]==0 || (varAcceleration[1]>0 && ballpointer[3]>0) || (varAcceleration[1]<0 && ballpointer[3]<0)))
        {
            ballpointer[1]+=ballpointer[3];
            ballpointer[3]+=varAcceleration[1];
        }
    }
}

void backbutton()
{
    iShowLoadedImage(5,50, &backButton);
}

void drawfield()
{
    iSetLineWidth(3);
    iSetColor(35,181,78);
    iFilledRectangle(0,0,width,gallary);
    iFilledRectangle(0,gallary+80,width,80);
    iFilledRectangle(0,gallary+3*80,width,80);
    iFilledRectangle(0,gallary+5*80,width,80);
    iFilledRectangle(0,gallary+7*80,width,80);
    iSetColor(45,232,100);
    iFilledRectangle(0,gallary+0*80,width,80);
    iFilledRectangle(0,gallary+2*80,width,80);
    iFilledRectangle(0,gallary+4*80,width,80);
    iFilledRectangle(0,gallary+6*80,width,80);
    iFilledRectangle(0,gallary+8*80,width,gallary);
    iSetColor(255,255,255);
    iRectangle(gallary,gallary,width-2*gallary,length-2*gallary);
    iLine(gallary,length/2,width-gallary,length/2);
    iFilledCircle(width/2,length/2,4);
    iCircle(width/2,length/2,80);
    iRectangle(width/2-130,gallary,130*2,130);
    iRectangle(width/2-130,length-gallary-130,130*2,130);
    iRectangle(width/2-80,gallary/2,160,gallary/2);
    iRectangle(width/2-80,length-gallary,160,gallary/2);
    //iPolygon({width/2-80,width/2-50,width/2+110,width/2+80},{gallary,gallary-20,gallary-20,gallary},4);
    iSetColor(255,0,0);
    iSetLineWidth(4);
    iLine(width/2-80,gallary,width/2+80,gallary);
    iLine(width/2-80,length-gallary,width/2+80,length-gallary);
    drawScoreboard();
}

void functioncaller()
{
    
    if((int)timer%10==0)
    {
        throwCornerOutGoal();
        activepassing();
        opponentpassing();
    }
    if((int)timer%5==0){
        if(!(ballstate==2 || ballstate==-2))
            chooseAndTakePosition();
        if(ballstate!=2)
            activeplayermoveing();
        if(ballstate!=-2)
            opponentplayermoveing();
        gkmoving();
        ballposition();
    }
    if((int)timer%3==0)
    {
        handlecollission();
    }
}

void chooselevel()
{

}

/*
function iDraw() is called again and again by the system.
*/
void iDraw()
{
    // place your drawing codes here
    timeUpdater();
    iClear();
    if(page_number==0)
    {
        if(timer<=coverpagetime)
            iShowImage(0,0,"opener.jpg");
        else
            iShowImage(0,0,"navigation.jpg");
    }
    else if(page_number==11)
    {
        iShowImage(0,0,"level.png");
        backbutton();
    }
    else if(page_number==12)
    {
        iShowImage(0,0,"navigator_2.jpg");

    }
    else if(page_number==1)
    {
        drawfield();
        chooselevel();
        functioncaller();
        spritepositionupdate();
        spriteshow();
    }
    else if(page_number==2)
    {
        drawfield();
        functioncaller();
        spritepositionupdate();
        spriteshow();
    }
    else if(page_number==3){
        penaltyActive=1;
        drawfield();
        drawPenaltyMarkers();// Draw penalty marks
        drawPenaltyScoreboard();
        meterUpdate(); // Update the penalty meter position
        drawMeter(); // Draw the penalty meter
        penaltyGK(); 
        penaltyScore(); 
    }

    /*else if(page_number==3)

    else if(page_number==4)

    else if(page_number==5)*/

    else if(page_number==6)
    {
        iShowImage(0,0,"exitpage.jpg");
        if(tracktime+75<=timer)
            exit(0);

    }
    iShowSpeed(10,10);
    char timertext[20];
    sprintf(timertext, "Timer: %lf", timer);
    iText(10,30,timertext);
    char fpss[20];
    sprintf(fpss,"fps %d",fps);
    iText(50,50,fpss);
}
/*
function iMouseMove() is called when the user moves the mouse.
(mx, my) is the position where the mouse pointer is.
*/
void iMouseMove(int mx, int my)
{
    // place your codes here
}

/*
function iMouseDrag() is called when the user presses and drags the mouse.
(mx, my) is the position where the mouse pointer is.
*/
void iMouseDrag(int mx, int my)
{
    // place your codes here
}

/*
function iMouse() is called when the user presses/releases the mouse.
(mx, my) is the position where the mouse pointer is.
*/
void iMouse(int button, int state, int mx, int my)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        // place your codes here
        if(page_number==0 && timer>coverpagetime)
        {
            if(mx>=134 && mx<=405)
            {
                if(my>=720-403  && my<=720-360)
                {
                    page_number=12;
                }
                else if(my>=720-450  && my<=720-408)
                {
                    page_number=2;
                }
                else if(my>=720-495  && my<=720-455)
                    page_number=3;
                else if(my>=720-542  && my<=720-502)
                    page_number=4;
                else if(my>=720-588  && my<=720-548)
                    page_number=5;
                else if(my>=720-635  && my<=720-594)
                    page_number=6;
            }
            tracktime=timer;
        }
        else if(page_number==11)
        {
            if(mx>63 && mx<472)
            {
                if(my<720-196 && my>720-263)
                    {level=1;page_number=1;resetvariables();}
                else if(my<720-304 && my>720-368)
                    {level=2;page_number=1;resetvariables();}
                else if(my<720-400 && my>720-460)
                    {level=3;page_number=1;resetvariables();}
            }
            else if(mx<65 && mx>5 && my<72 && my>50)
                page_number=0;
        }
        else if(page_number==12)
        {
            if(mx>22 && mx<243)
            {page_number=1;
            gametime=0;}
            else if(mx>281 && mx<520)
            {page_number=3;}

        }
        else if(page_number==3 && mx>20 && mx<width-20){
             targetX = mx;
    targetY = my;

    // only kick when ball is ready
    if (ballstate == 2 && penaltyActive)
    {
        // 1) compute meter geometry
        double fullH = penaltyMeterMaxY - penaltyMeterMinY;
        double mid   = (penaltyMeterMaxY + penaltyMeterMinY) * 0.5;
        // clamp penaltyMeterY just in case
        double y = fmin(fmax(penaltyMeterY, penaltyMeterMinY), penaltyMeterMaxY);

        // 2) offset from center, normalized [0..1]
        double offset = fabs(y - mid);
        double norm   = (offset * 2.0) / fullH;
        if (norm > 1.0) norm = 1.0;

        // 3) compute kick speed factor
        double powerFac = 2.5 - norm;           // 1 at center, 0 at edges
        double kickSpeed = ballSpeed * powerFac;

        // if at exact edge → guaranteed miss: set kickSpeed=0
        if (y <= penaltyMeterMinY || y >= penaltyMeterMaxY)
            kickSpeed = 0.0;

        // 4) compute direction vector
        double dx = targetX - playerposition[activeTeam][activeplayer][0];
        double dy = targetY - playerposition[activeTeam][activeplayer][1];
        double mag = sqrt(dx*dx + dy*dy);
        if (mag > 0 && kickSpeed > 0)
        {
            ballpointer[2] = (dx / mag) * kickSpeed;
            ballpointer[3] = (dy / mag) * kickSpeed;
        }
        else
        {
            // zero speed → immediate miss
            ballpointer[2] = ballpointer[3] = 0;
            scoreMark = -1;   
        }

        // 5) set up deceleration & launch
        varAcceleration[0] = -acceleration * (dx / (mag>0?mag:1));
        varAcceleration[1] = -acceleration * (dy / (mag>0?mag:1));
        ballpointer[0] = playerposition[activeTeam][activeplayer][0];
        ballpointer[1] = playerposition[activeTeam][activeplayer][1];
        iSetSpritePosition(&fball,
            ballpointer[0] - ballradius,
            ballpointer[1] - ballradius
        );
        ballstate = 0;
    }

        }
        else if(page_number==1 || page_number==2 || page_number==3)
        {
            if(mx<width && mx>width-20 && my<length && my>length-20)
            {
                page_number=0;
                penaltyActive=0;
                gametime+=timer-tracktime;
            }
        }
    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        // place your codes here
    }
}

/*
function iMouseWheel() is called when the user scrolls the mouse wheel.
dir = 1 for up, -1 for down.
*/
void iMouseWheel(int dir, int mx, int my)
{
    // place your code here
}

/*
function iKeyboard() is called whenever the user hits a key in keyboard.
key- holds the ASCII value of the key pressed.
*/
void iKeyboard(unsigned char key)
{
    switch (key)
    {
    case 'q':
        // do something with 'q'
        break;
    // place your codes for other keys here
    default:
        break;
    }
}

/*
function iSpecialKeyboard() is called whenver user hits special keys likefunction
keys, home, end, pg up, pg down, arraows etc. you have to use
appropriate constants to detect them. A list is:
GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11,
GLUT_KEY_F12, GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN,
GLUT_KEY_PAGE_UP, GLUT_KEY_PAGE_DOWN, GLUT_KEY_HOME, GLUT_KEY_END,
GLUT_KEY_INSERT */
void iSpecialKeyboard(unsigned char key)
{
    
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    // place your own initialization codes here.
    loadresources();
    //iSetTimer(1, timeUpdater);
    iInitialize(width, length, "2D Football");
    return 0;
}