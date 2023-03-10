#pragma once
#include <queue>
#include <memory>
#include "Entity.h"
#include "PathFinding.h"
#include "Utils.h"
#include "Weapon.h"
#include <cmath>


class GameCharacter : public Entity
{
public:
    GameCharacter(uint8_t maxHealth, int8_t health, uint8_t maxEnergy, int8_t energy, uint8_t priority) 
        : maxHealth(maxHealth), maxEnergy(maxEnergy), priority(priority), weapon(new Weapon(1,1,1))
    {
        setHealth(health); //fix health if greater than maxHealth
        setEnergy(energy); //fix energy if greater tha maxEnergy
    };

    virtual ~GameCharacter() = default;

    inline void render(sf::RenderWindow& window) override 
    {   
        Entity::render(window);
        weapon->render(window);    
    };
    
    //GETTER AND SETTER
    inline bool isSolid() const override { return true; };
    inline uint8_t getMaxHealth() const { return maxHealth; }; 
    inline uint8_t getMaxEnergy() const { return maxEnergy; };
    inline const int8_t& getHealth() const { return health; };
    inline const int8_t& getEnergy() const { return energy; };
    inline uint8_t getPriority() const { return priority; };
    inline std::unique_ptr<PathAlgorithm>& getMovementStrategy() { return movementStrategy; };
    inline Weapon& getWeapon() { return *weapon; };

    inline void setWeapon(const Weapon &newWeapon){ weapon.reset(new Weapon(newWeapon));};
    inline void setPos(const sf::Vector2<float>& pos) override  
    {   
        Entity::setPos(pos);
        weapon->setPos(pos); 
    };
    
    inline void setHealth(const int8_t newHealth) { health = std::max<int8_t>(std::min<int8_t>(newHealth, maxHealth), 0); }
    inline void setEnergy(const int8_t newEnergy) { energy = std::max<int8_t>(std::min<int8_t>(newEnergy, maxEnergy), 0); }
                                            
    //FUNCTIONS
    inline void turnReset(){ energy = maxEnergy; }; 
    
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
    int8_t health = 0;
    int8_t energy = 0;
    std::unique_ptr<PathAlgorithm> movementStrategy;
    std::unique_ptr<Weapon> weapon;
};


class Player : public GameCharacter
{
public:
    Player(uint8_t health, uint8_t energy, uint8_t filterColorR, uint8_t filterColorG, uint8_t filterColorB);

    virtual ~Player() {};

    inline const sf::Color& getFilterColor() const { return filterColor; };
    inline void setFilterColor(const sf::Color &color) {filterColor = color;};

    void serialize(Archive& fs) override 
    {
        GameCharacter::serialize(fs);
        fs.serialize(filterColor);
        uint8_t attack = weapon->getAttack();
        uint8_t cost = weapon->getCost();
        uint8_t range = weapon->getRange();
        bool hidden = weapon->isHidden();
        fs.serialize(attack);
        fs.serialize(cost);
        fs.serialize(range);
        fs.serialize(hidden);
        weapon.reset(new Weapon{attack, cost, range, hidden});
    }

    static Serializable* create() { return new Player{50,15,190,190,190}; };
private:
    static Register registration;
    sf::Color filterColor; 
};


class Melee : public GameCharacter
{
public:
    Melee(uint8_t health, uint8_t energy);

    virtual ~Melee() {};

    static Serializable* create() { return new Melee{30,5}; };
private:
    static Register registration;
};


class Bat : public GameCharacter
{
public:
    Bat(uint8_t health, uint8_t energy);

    virtual ~Bat() {};

    inline bool isSolid() const override { return false; };

    static Serializable* create() { return new Bat{10,10}; };
private:
    static Register registration;
};


class Ranged : public GameCharacter
{
public:
    Ranged(uint8_t health, uint8_t energy);

    virtual ~Ranged() {};

    void serialize(Archive& fs) override 
    {
        GameCharacter::serialize(fs);
        fs.serialize(animationTime);
    }

    static Serializable* create() { return new Ranged{5,15}; };
private:
    static Register registration;

    const float animationDuration = 0.f;
    float animationTime = 0.f;
};
