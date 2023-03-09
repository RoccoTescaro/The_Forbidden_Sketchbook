#pragma once
#include <unordered_map>
#include <typeinfo>
#include <functional>
#include <iostream>

class Archive;

class Serializable
{
public:
	virtual ~Serializable() = default;
	virtual void serialize(Archive& fs) = 0;
	virtual std::string getTypeId() const { return static_cast<std::string>(typeid(*this).name()); };
};

class Register
{
public:
	//the Register class allow leafs of Serializable abstract class to memorize itself in an unorderdmap.
	// Archive can then use it to create the correct object when loading a file (when a pointer object is serialized like Wall* 
	//it can after loading be deserialize as Tile*, Archive cannot determine tho from a Tile pointer if it was a Wall or a Hole
	//so it need to know the type of the object to create the correct one)

	Register(std::string id, std::function<Serializable* ()> create) { memory[id] = create; };

	inline static Serializable* getType(std::string id) { return  memory[id](); };
private:
	inline static std::unordered_map<std::string, std::function<Serializable* ()>> memory; 
};