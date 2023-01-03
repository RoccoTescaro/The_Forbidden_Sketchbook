#pragma once
#include <unordered_map>
#include <functional>

#define REGIST(Class) \
		public :\
		inline virtual std::string getTypeId() const override { return #Class; };\
		private:\
		inline static Serializable* create() { return new Class; };\
		inline static Register Class##Registration{#Class,Class::create}

class Serializable;

class Register
{
public:
	Register(const std::string& id, std::function<Serializable* ()> create) { memory[id] = create; };
	inline static Serializable* getType(std::string id) { return  memory[id](); };
private:
	inline static std::unordered_map<std::string, std::function<Serializable*(void)>> memory;
};

