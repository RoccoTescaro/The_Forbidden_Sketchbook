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
	Register(std::string id, std::function<Serializable* ()> create) { memory[id] = create; };

	inline static Serializable* getType(std::string id) { return  memory[id](); };
private:
	inline static std::unordered_map<std::string, std::function<Serializable* ()>> memory;
};