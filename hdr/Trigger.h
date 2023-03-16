#pragma once
#include <string>
#include <functional>
#include <memory>
#include "AchievementManager.h"

//It would be cool to store a function inside trigger which returns if task as been done (to update progress)
//and then call trigger.update indiscriminately from achievement manager, unfortunatly it's not always easy to define 
//that function becouse even with something like we did with button the state of things might change and the condition might not be true anymore

class Trigger 
{
public:
	Trigger(const Achievement& achievement) : achievement(achievement) {};

	//we don't need an attach, detach or notify function since we only have an observer and is a singleton

	void update()
	{
		achievement.progress++;
		if (achievement.progress == achievement.goal)
			AchievementManager::update(achievement);
	}
private:
	Achievement achievement;
};

