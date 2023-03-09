#pragma once
#include <queue>
#include <memory>
#include "Entity.h"
#include "PathFinding.h"
#include "Utils.h"
#include "Weapon.h"



class GameCharacter : public Entity //TODO fix constructors
{
private:

    using MovementStrategy = std::unique_ptr<PathAlgorithm>;
    using StepQueue = std::deque<sf::Vector2<float>>;
public:
    GameCharacter(uint8_t maxHealth, uint8_t health, uint8_t maxEnergy, uint8_t energy, uint8_t priority) 
        : maxHealth(maxHealth), maxEnergy(maxEnergy), health(health), energy(energy), priority(priority), weapon(5,3) {};

    //ENTITY
    inline void render(sf::RenderWindow& window) override 
    {   
        Entity::render(window);
        weapon.render(window);    
    };
    
    //GAMECHARACTER GET&SET
    inline bool isSolid() const override { return true; };
    inline uint8_t getMaxHealth() const { return maxHealth; }; //need to return as a reference and not by copy to allow hud auto update
    inline uint8_t getMaxEnergy() const { return maxEnergy; }; //need to return as a reference and not by copy to allow hud auto update
    inline const uint8_t& getHealth() const { return health; };
    inline const uint8_t& getEnergy() const { return energy; };
    inline uint8_t getPriority() const { return priority; };
    inline MovementStrategy& getMovementStrategy() { return movementStrategy; };
    inline Weapon& getWeapon() { return weapon; };

    inline void setPos(const sf::Vector2<float>& pos) override  
    {   
        Entity::setPos(pos);
        weapon.setPos(pos); 
    };
    
    inline void setHealth(const uint8_t newHealth) { health = newHealth > maxHealth ? 0 : newHealth; }
    inline void setEnergy(const uint8_t newEnergy) { energy = newEnergy > maxEnergy ? 0 : newEnergy; }
                                            
    //GAMECHARACTER FUNCTIONS
    inline void turnReset(){ energy=maxEnergy; }; 
    inline void serialize(Archive& fs) 
    {
        Entity::serialize(fs);
        fs.serialize(health);
        fs.serialize(energy);
    }

    virtual void move(Map &map, sf::Vector2<float> target, const float &dt); 
    void interact(Map &map, sf::Vector2<float> target, const float &dt) override; 

protected:
    
    const float speed = 200.f;
    const uint8_t maxHealth = 20;  
    const uint8_t maxEnergy = 20;
    const uint8_t priority = 0;
    uint8_t health = 0;
    uint8_t energy = 0;

    MovementStrategy movementStrategy;

    Weapon weapon; //std::unique_ptr<Weapon> weapon = nullptr;
};


class Player : public GameCharacter
{
public:
    Player(uint8_t health, uint8_t energy, uint8_t filterColorR, uint8_t filterColorG, uint8_t filterColorB);
    Player() : Player(100,0,255,255,255) {}; //should initialize by the list initialization the const memeber that we dont wont/cant serialize

    inline const sf::Color& getFilterColor() const { return filterColor; };
  
    void serialize(Archive& fs) override 
    {
        GameCharacter::serialize(fs);
        fs.serialize(filterColor);
    }

    static Serializable* create() { return new Player; };
private:
    static Register registration;
    sf::Color filterColor; 
};


class Melee : public GameCharacter
{
public:
    Melee(uint8_t health, uint8_t energy);

    void serialize(Archive& fs) override
    {
        GameCharacter::serialize(fs);
    }

    static Serializable* create() { return new Melee{100,100}; };
private:
    static Register registration;
};


class Bat : public GameCharacter
{
public:
    Bat(uint8_t health, uint8_t energy);

    inline bool isSolid() const override { return false; };

    void serialize(Archive& fs) override
    {
        GameCharacter::serialize(fs);
    }

    static Serializable* create() { return new Bat{100,100}; };
private:
    static Register registration;
};


class Ranged : public GameCharacter
{
public:
    Ranged(uint8_t health, uint8_t energy);

    void serialize(Archive& fs) override 
    {
        GameCharacter::serialize(fs);
        fs.serialize(animationTime);
    }

    static Serializable* create() { return new Ranged{100,100}; };
private:
    static Register registration;

    const float animationDuration = 0.f;
    float animationTime = 0.f;
};
