#pragma once
#include <list>
#include <memory>
#include <string>
#include "Utils.h"


struct Achievement
{
	Achievement(std::string name, std::string description, uint8_t goal)
		: name(name), description(description), progress(0), goal(goal) {}

	std::string name;
	std::string description;
	uint8_t progress;
	uint8_t goal;
};

class AchievementManager //Observer Pattern
{
public:
	AchievementManager(const AchievementManager&) = delete;

	static void update(const Achievement& achievement);

	static Achievement getAchievement();
private:
	AchievementManager() = default;
	static AchievementManager achievementManager;

	std::list<Achievement> completedAchievements;
};