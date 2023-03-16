#include "../hdr/AchievementManager.h"

AchievementManager AchievementManager::achievementManager;

void AchievementManager::update(const Achievement& achievement)
{
	achievementManager.completedAchievements.push_back(achievement);
}

Achievement AchievementManager::getAchievement()
{
	if (achievementManager.completedAchievements.empty())
		return Achievement("", "", 0);

	Achievement ach = achievementManager.completedAchievements.front();
	achievementManager.completedAchievements.pop_front();
	return ach;
}