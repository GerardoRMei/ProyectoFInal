#include "Enemy.h"
#include "Turret.h"

bool Enemy::init()
{
    return true;
}

Enemy::Enemy() {

}

Enemy:: ~Enemy() {

}

void Enemy::gotHit(int i) {
        this->vida -= i;
    
}

int Enemy::getHealth() {
    return vida;
}

void Enemy::setHealth(int newHealth) {
    vida = newHealth;
}

bool Enemy::isKilled() {
    if (vida == 0) { return true; }

    if (vida > 0) { return false; }
}