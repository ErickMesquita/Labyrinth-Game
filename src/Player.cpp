//
//  Player.cpp
//  Zombie Arena
//

//#include "stdafx.h" // <- Visual Studio only
#include "player.hpp"
//#include "grid.h"


Player::Player() {
    m_abs_Speed = START_SPEED;
    m_Health = START_HEALTH;
    m_MaxHealth = START_HEALTH;

    m_Win = false;

    // Associate a texture with the sprite
    // !!Watch this space!!
    m_Texture = TextureHolder().GetTexture("Resources/graphics/player.png");
    m_Sprite.setTexture(m_Texture);

    // Set the origin of the sprite to the centre,
    // for smooth rotation
    m_Sprite.setOrigin(19, 18);
}

void Player::spawn(int scale, Vector2f resolution) {
    // Place the player in the middle of the arena
    m_Position.x = scale/2;
    m_Position.y = scale/2;
    m_Sprite.setPosition(m_Position);


    // Store the resolution for future use
    m_Resolution.x = resolution.x;
    m_Resolution.y = resolution.y;

    m_Win = false;
}

void Player::resetPlayerStats() {
    m_abs_Speed = START_SPEED;
    m_Health = START_HEALTH;
    m_MaxHealth = START_HEALTH;
}

Time Player::getLastHitTime() {
    return m_LastHit;
}

bool Player::hit(Time timeHit) {
    if (timeHit.asMilliseconds() - m_LastHit.asMilliseconds() > 200) { // 2 tenths of second
        m_LastHit = timeHit;
        m_Health -= 10;
        return true;
    } else {
        return false;
    }

}

FloatRect Player::getPosition() {
    return m_Sprite.getGlobalBounds();
}

Vector2f Player::getCenter() {
    return m_Position;
}

float Player::getRotation() {
    return m_Sprite.getRotation();
}

Sprite Player::getSprite() {
    return m_Sprite;
}

int Player::getHealth() {
    return m_Health;
}

bool Player::collisionHorizontal(FloatRect wall, FloatRect position, grid *g) {

    //Simple Rectangle-Rectangle collision detection
    /*
    if (playerPosition.intersects((*s).getGlobalBounds())){
        return true;
    }*/


    //Circle-Rectangle Collision Detection
    //Created by Jeffrey Thompson
    //Obtained from:  http://www.jeffreythompson.org/collision-detection/circle-rect.php

    // Circle's center
    int cx = position.left + position.width/2;
    int cy = position.top + position.height/2;

    //Wall's square:
    int rx = wall.left;
    int ry = wall.top;

    int rw = wall.width;
    int rh = wall.height;

    int testX = cx;
    int testY = cy;

    // Circle's center is inside rectangle
    if (wall.contains(cx, cy)) {
        return true;

    } else {
        if (cx < rx)         testX = rx;        // left edge
        else if (cx > rx+rw) testX = rx+rw;     // right edge
        else                 return false;      // if collision, it is not horizontal

        if (cy < ry)         testY = ry;        // top edge
        else if (cy > ry+rh) testY = ry+rh;     // bottom edge

        float distX = cx-testX;
        float distY = cy-testY;
        float distance = sqrt( (distX*distX) + (distY*distY) );

        if (distance <= position.width/2) {
            return true;
        }
    }
    return false;
}

bool Player::collisionVertical(FloatRect wall, FloatRect position, grid *g) {

    //Simple Rectangle-Rectangle collision detection
    /*
    if (playerPosition.intersects((*s).getGlobalBounds())){
        return true;
    }*/


    //Circle-Rectangle Collision Detection
    //Created by Jeffrey Thompson
    //Obtained from:  http://www.jeffreythompson.org/collision-detection/circle-rect.php

    // Circle's center
    int cx = position.left + position.width/2;
    int cy = position.top + position.height/2;

    //Wall's square:
    int rx = wall.left;
    int ry = wall.top;

    int rw = wall.width;
    int rh = wall.height;

    int testX = cx;
    int testY = cy;

    // Circle's center is inside rectangle
    if (wall.contains(cx, cy)) {
        return true;

    } else {
        if (cx < rx)         testX = rx;        // left edge
        else if (cx > rx+rw) testX = rx+rw;     // right edge

        if (cy < ry)         testY = ry;        // top edge
        else if (cy > ry+rh) testY = ry+rh;     // bottom edge
        else                 return false;      // if collision, it is not vertical

        float distX = cx-testX;
        float distY = cy-testY;
        float distance = sqrt( (distX*distX) + (distY*distY) );

        if (distance <= position.height/2) {
            return true;
        }
    }
    return false;
}

Vector2f Player::restrictMovement(FloatRect wall, FloatRect playerPosition, Vector2f movement, grid *g){ //player dimensions

    float x = movement.x;
    float y = movement.y;


    //add the x first - Check Horizontal only
    FloatRect next_playerPosition_H = FloatRect(playerPosition);
    next_playerPosition_H.left += x;

    if (collisionHorizontal(wall, next_playerPosition_H, g)) {
      x = 0;
    }

    //check the y movement
    FloatRect next_playerPosition_V = FloatRect(playerPosition);
    next_playerPosition_V.top += y;

    if (collisionVertical(wall, next_playerPosition_V, g)) {
      y = 0;
    }

    return Vector2f(x, y);
}

void Player::move(float elapsedTime, grid* g){

    // Read current speed
    Vector2f temp = Vector2f(this->m_Speed.x, this->m_Speed.y);

    // Normalize it
    float norm = float(sqrt((temp.x * temp.x) + (temp.y * temp.y)));
    if (norm != 0){
        temp /= norm;
    }

    // Multiply by absolute value of Speed
    temp *= this->m_abs_Speed;

    // Multiply by elapsed time
    temp *= elapsedTime;


    // Player's current tile coordinates: Center of rectangle/scale
    int x = (this->m_Position.x)/(g->getScale());
    int y = (this->m_Position.y)/(g->getScale());


    for (int i = -1; i <= 1; i++){
        if (i == 1 && x == g->getHW().x - 1){
            // do nothing
        }else if (i >= 0 && x == g->getHW().x){
            // do nothing
        }else if (x > g->getHW().x){
            // do nothing
        }else if (i == -1 && x == 0){
            // do nothing
        }else{
            for (int j = -1; j <= 1; j++){
                if (j == 1 && y == g->getHW().y - 1){
                    // do nothing
                }else if (j >= 0 && y == g->getHW().y){
                    // do nothing
                }else if (y > g->getHW().y){
                    // do nothing
                }else if (j == -1 && y == 0){
                    // do nothing
                }else{

                    for (std::vector<Sprite>::const_iterator s = g->nos[x + i][y + j]->spriteWalls->begin(); s != g->nos[x + i][y + j]->spriteWalls->end(); s++){

                        temp = restrictMovement((*s).getGlobalBounds(), this->getPosition(), temp, g);

                    }
                }
            }
       }
    }
    this->m_Position += temp;
    m_Sprite.setPosition(this->m_Position);

}


void Player::detectWin(grid *g){

    // Player's current tile coordinates: Center of rectangle/scale
    int x = (this->m_Position.x)/(g->getScale());
    int y = (this->m_Position.y)/(g->getScale());

    if (x == g->getHW().x - 1 && y == g->getHW().y - 1){ //if get to last tile
        this->m_Win = true;
    }

}

void Player::update(float elapsedTime, Vector2i mousePosition, grid* g) {

    move(elapsedTime, g);
    detectWin(g);


    /***************** Comment if deploying to Retina display******************/
    // Calculate the angle the player is facing - not Retina displays
    float angle = (atan2(mousePosition.y - m_Position.y,
                         mousePosition.x - m_Position.x)
                   * 180) / 3.141;


    /**************  QAD fix for problem with iMac Retina displays ************/
    /*
    // Calculate the angle the player is facing - Retina displays
    float angle = (atan2((mousePosition.y - m_Resolution.y) - m_Resolution.y / 2,
                         mousePosition.x - m_Resolution.x / 2)
                   * 180) / 3.141;
    */
    //m_Sprite.setRotation(angle);

}

void Player::upgradeSpeed() {
    // 20% speed upgrade
    m_abs_Speed += (START_SPEED * .2);
}

void Player::upgradeHealth() {
    // 20% max health upgrade
    m_MaxHealth += (START_HEALTH * .2);

}

void Player::increaseHealthLevel(int amount) {
    m_Health += amount;

    // But not beyond the maximum
    if (m_Health > m_MaxHealth) {
        m_Health = m_MaxHealth;
    }
}
