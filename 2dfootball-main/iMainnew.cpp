#include "iGraphics.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>



Sprite players[6], opponents[6], gk[2], fball, fields;
Image ball[1], player[1], opponent[1], field[1];
double length = 120 * 6, width = 90 * 6, gallary = 40, goalbar = 160;
long long int timer = 0, i, ii, gametime = 0, tracktime = 0;
int level, scoreplayer = 0, scoreopp = 0;
double speed = 0.6, gkspeed = 0.5, passedballspeed = 1, shootballspeed = 1.3, acceleration = -0.002, varAcceleration[2] = {0.1, 0.1};
double varFriction = .975;
double playerradius = 8, ballradius = 5, collidepos = 5;
double newball[4] = {width / 2, length / 2};
double gkpointer[2][2] = {{width / 2, gallary + playerradius}, {width / 2, length - gallary - playerradius}};
double newgk[2][2] = {{width / 2, gallary + playerradius}, {width / 2, length - gallary - playerradius}};
double playerposition[2][6][4] = {{{
                                       width / 2,
                                       length * 8 / 10,
                                       width / 2,
                                       length * 8 / 10,
                                   },
                                   {width / 6, length * 8 / 10, width / 6, length * 8 / 10},
                                   {width * 5 / 6, length * 8 / 10, width * 5 / 6, length * 8 / 10},
                                   {width * 2 / 6, length * 7 / 10, width * 2 / 6, length * 7 / 10},
                                   {width * 4 / 6, length * 7 / 10, width * 4 / 6, length * 7 / 10},
                                   {width / 2, length * 6 / 10, width / 2, length * 6 / 10}},
                                  {{width / 2, length * 2 / 10, width / 2, length * 2 / 10}, {width / 6, length * 2 / 10, width / 6, length * 2 / 10}, {width * 5 / 6, length * 2 / 10, width * 5 / 6, length * 2 / 10}, {width * 2 / 6, length * 7 / 20, width * 2 / 6, length * 7 / 20}, {width * 4 / 6, length * 7 / 20, width * 4 / 6, length * 7 / 20}, {width / 2, length / 2, width / 2, length / 2}}};
double newplayerposition[2][6][4] = {{{
                                          width / 2,
                                          length * 8 / 10,
                                          width / 2,
                                          length * 8 / 10,
                                      },
                                      {width / 6, length * 8 / 10, width / 6, length * 8 / 10},
                                      {width * 5 / 6, length * 8 / 10, width * 5 / 6, length * 8 / 10},
                                      {width * 2 / 6, length * 7 / 10, width * 2 / 6, length * 7 / 10},
                                      {width * 4 / 6, length * 7 / 10, width * 4 / 6, length * 7 / 10},
                                      {width / 2, length * 6 / 10, width / 2, length * 6 / 10}},
                                     {{width / 2, length * 2 / 10, width / 2, length * 2 / 10}, {width / 6, length * 2 / 10, width / 6, length * 2 / 10}, {width * 5 / 6, length * 2 / 10, width * 5 / 6, length * 2 / 10}, {width * 2 / 6, length * 7 / 20, width * 2 / 6, length * 7 / 20}, {width * 4 / 6, length * 7 / 20, width * 4 / 6, length * 7 / 20}, {width / 2, length / 2, width / 2, length / 2}}};
double ballpointer[4] = {width / 2, length / 2};
int activeplayer = 5, ballstate = 2, ballholder = activeplayer, activeplayeropp = 5, helpingplayer = 4, helpingplayeropp = 4, lastTouch = 1;
int page_number = 0, coverpagetime = 100;
double dxy = 30;
double positionField[2][6][4] = {{{4 * width / 7, 3 * width / 7, length - gallary, length / 2}, {10 * width / 20, gallary, length - gallary, 10 * length / 20}, {width - gallary, 10 * width / 20, length - gallary, 10 * length / 20}, {10 * width / 20, gallary, 10 * length / 20, gallary}, {width - gallary, 10 * width / 20, 10 * length / 20, gallary}, {10 * width / 20, 10 * width / 20, length / 2, gallary}}, {{4 * width / 7, 3 * width / 7, length / 2, gallary}, {10 * width / 20, gallary, 10 * length / 20, gallary}, {width - gallary, 10 * width / 20, 10 * length / 20, gallary}, {10 * width / 20, gallary, length - gallary, 10 * length / 20}, {width - gallary, 10 * width / 20, length - gallary, 10 * length / 20}, {10 * width / 20, 10 * width / 20, length - gallary, length / 2}}};
double range[2][6][2] = {{{7 * width / 20, 7 * width / 20}, {-7 * width / 20, 7 * width / 20}, {7 * width / 20, 7 * width / 20}, {-7 * width / 20, -7 * width / 20}, {7 * width / 20, -7 * width / 20}, {7 * width / 20, -7 * width / 20}}, {{7 * width / 20, -7 * width / 20}, {-7 * width / 20, -7 * width / 20}, {7 * width / 20, -7 * width / 20}, {-7 * width / 20, 7 * width / 20}, {7 * width / 20, 7 * width / 20}, {7 * width / 20, 7 * width / 20}}};
int dX = 30, dY = 50;
#define MAX_SHOTS 20

double  playerX[MAX_SHOTS];  
int     playerResult[MAX_SHOTS]={0};
int     playerCount = 0;

double  oppX[MAX_SHOTS];
int     oppResult[MAX_SHOTS]={0};
int     oppCount    = 0;
bool checkcollision(double x1, double y1, double x2, double y2, double r)
{
    if ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) <= r * r)
        return true;
    else
        return false;
}
/*
function iDraw() is called again and again by the system.
*/
//Penalty
int penaltyActive = 1;             // Flag to indicate penalty mode  
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
void drawScoreboard()
{
    char score_text[50];
    char time_text[20];
    int minutes = gametime / 60000;       // Convert milliseconds to minutes
    int seconds = (gametime / 1000) % 60; // Convert milliseconds to seconds
    sprintf(score_text, "Pla %d - %d Opp", scoreplayer, scoreopp);
    sprintf(time_text, "Time: %02d:%02d", minutes, seconds);
    // iSetColor(0,0,255);
    // iFilledRectangle(0,length-gallary,100,gallary);
    iSetColor(255, 255, 255);
    iTextAdvanced(10, length - 30, score_text, .1, 2);
    iTextAdvanced(10, length - 15, time_text, .1, 2);
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
void loadresources()
{
    iSetSpritePosition(&fields, 0, 0);
    iLoadFramesFromSheet(ball, "ball.png", 1, 1);
    iChangeSpriteFrames(&fball, ball, 1);

    for (int n = 0; n < 6; n++)
    {
        iLoadFramesFromSheet(player, "player.png", 1, 1);
        iChangeSpriteFrames(&players[n], player, 1);
        if (n == 1)
        {
            iChangeSpriteFrames(&gk[n], player, 1);
            iSetSpritePosition(&gk[n], gkpointer[n][0], gkpointer[n][1]);
        }
        iLoadFramesFromSheet(opponent, "opponent.png", 1, 1);
        iChangeSpriteFrames(&opponents[n], opponent, 1);
        if (n == 0)
        {
            iChangeSpriteFrames(&gk[n], opponent, 1);
            iSetSpritePosition(&gk[n], gkpointer[n][0], gkpointer[n][1]);
        }
        iSetSpritePosition(&players[n], playerposition[1][n][0], playerposition[1][n][1]);
        iSetSpritePosition(&opponents[n], playerposition[0][n][0], playerposition[0][n][1]);
    }
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
void drawfield()
{
    iSetLineWidth(3);
    iSetColor(35, 181, 78);
    iFilledRectangle(0, 0, width, gallary);
    iFilledRectangle(0, gallary + 80, width, 80);
    iFilledRectangle(0, gallary + 3 * 80, width, 80);
    iFilledRectangle(0, gallary + 5 * 80, width, 80);
    iFilledRectangle(0, gallary + 7 * 80, width, 80);
    iSetColor(45, 232, 100);
    iFilledRectangle(0, gallary + 0 * 80, width, 80);
    iFilledRectangle(0, gallary + 2 * 80, width, 80);
    iFilledRectangle(0, gallary + 4 * 80, width, 80);
    iFilledRectangle(0, gallary + 6 * 80, width, 80);
    iFilledRectangle(0, gallary + 8 * 80, width, gallary);
    iSetColor(255, 255, 255);
    iRectangle(gallary, gallary, width - 2 * gallary, length - 2 * gallary);
    iLine(gallary, length / 2, width - gallary, length / 2);
    iFilledCircle(width / 2, length / 2, 4);
    iCircle(width / 2, length / 2, 80);
    iRectangle(width / 2 - 130, gallary, 130 * 2, 130);
    iRectangle(width / 2 - 130, length - gallary - 130, 130 * 2, 130);
    iRectangle(width / 2 - 80, gallary / 2, 160, gallary / 2);
    iRectangle(width / 2 - 80, length - gallary, 160, gallary / 2);
    // iPolygon({width/2-80,width/2-50,width/2+110,width/2+80},{gallary,gallary-20,gallary-20,gallary},4);
    iSetColor(255, 0, 0);
    iSetLineWidth(4);
    iLine(width / 2 - 80, gallary, width / 2 + 80, gallary);
    iLine(width / 2 - 80, length - gallary, width / 2 + 80, length - gallary);
    drawScoreboard();
}
void iDraw()
{
    iClear();
    drawfield();
    drawPenaltyMarkers();// Draw penalty marks
    drawPenaltyScoreboard();
    if(penaltyActive){
        meterUpdate(); // Update the penalty meter position
        drawMeter(); // Draw the penalty meter
    penaltyGK(); 
     penaltyScore(); // Handle scoring logic during penalty  
    } 
else
{
    spritepositionupdate();
    spriteshow();
    gameresult();
}
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
        double powerFac = 1.5 - norm;           // 1 at center, 0 at edges
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
        exit(0);
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
    switch (key)
    {
    case GLUT_KEY_END:
        // do something
        break;
    // place your codes for other keys here
    default:
        break;
    }
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    loadresources();
    // place your own initialization codes here.
    iInitialize(540, 720, "2D Football");
    return 0;
}