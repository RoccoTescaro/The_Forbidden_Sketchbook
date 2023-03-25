#pragma once
#include <memory>
#include "GameCharacter.h"
#include "Map.h"
#include "Utils.h"
#include "Achievement.h"

class TurnSystem : public Serializable
{
private:
    struct Action 
    {
        enum Type
        {
            Interact = 0,
			Move = 1
		};
        Type type;
        sf::Vector2<float> target;
    };

    struct Compare
    {
        bool operator()(const std::weak_ptr<GameCharacter>& p1, const std::weak_ptr<GameCharacter>& p2) const
        {
            //the gameCharacters in the queue might be expired due to death, so we need to check for that even to use .top() function
            return ((p1.expired() || p2.expired()) || p1.lock().get()->getPriority() <= p2.lock().get()->getPriority()); 
        }
    };

    using ActionQueue = std::queue<Action>;
    using TurnQueue = std::priority_queue<std::weak_ptr<GameCharacter>, std::vector<std::weak_ptr<GameCharacter>>, Compare>;
public:
    TurnSystem();

    void init(Hud* hud, std::shared_ptr<Map> map);
  
    void update(const float &dt); //update actor accordingly to actionQueue
    void turnBuild(sf::Vector2<float> target); //update actionQueue accordingly to input

    inline std::weak_ptr<GameCharacter> getActor() { return actor; };
    inline bool isPlayerTurn() const { return turnQueue.empty(); };  //Player has priority 0 so it will allways be the last one in the queue (no need for dynamic_cast)
       
    void serialize(Archive& arc); 
private:
    void newRound();
    void newTurn();
    
    static Serializable* create() { return new TurnSystem; };
    static Register registration;

    std::weak_ptr<Map> map;

    TurnQueue turnQueue;
    ActionQueue actionQueue;
    std::weak_ptr<GameCharacter> actor;

    std::unordered_map<std::string, Achievement> achievements; //could use an enum or/and a vector instead of a map
};