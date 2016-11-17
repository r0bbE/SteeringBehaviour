#include "Formation.h"
#include <iostream>

Formation::Formation()
{
	currIndex = 0; 

	//Moving related
	velocity.x = 0.0f;
	velocity.y = 0.0f;

	maxSpeed = 120.0f;
	steeringForce = 4.0f;
}

Formation::~Formation()
{
	for(auto i : offsets)
	{
		delete i;
	}
}

int Formation::registerSoldier()
{
	currIndex++;
	return currIndex - 1;
}

glm::vec2 Formation::GetPositionForIndex(int index)
{
	glm::vec2 offset(offsets.at(index)->getPosition().x, offsets.at(index)->getPosition().y);
	offset += position;

	return offset;
}

void Formation::SetPosition(glm::vec2 pos)
{
	position = pos;

}

void Formation::SetPath(Path* newpath)
{
	pathtofollow = newpath;
}

void Formation::Render(sf::RenderWindow* window)
{
	
}

void Formation::DebugDraw(sf::RenderWindow* window)
{
	for(int i = 0; i < offsets.size(); ++i)
	{
		sf::CircleShape tmp(*offsets[i]);
		tmp.setFillColor(sf::Color(40, 40, 120, 120));
		tmp.setPosition(GetPositionForIndex(i).x, GetPositionForIndex(i).y);
		//tmp.rotate(orientation);
		window->draw(tmp);
	}
}

void Formation::Move(sf::Time delta)
{
	glm::vec2 steering;

	//GetTarget
	target = pathtofollow->GetPoint(currentWaypoint);
	if(distance(target, position) < tolerance)
	{
		currentWaypoint++;
	}

 	steering = Seek(velocity, target);

	velocity = velocity + steering * delta.asSeconds() * 100.0f;
	velocity = truncate(velocity, maxSpeed);
	if ((velocity.x < -0.00001f || velocity.x > 0.00001f) && (velocity.y < -0.00001f || velocity.y > 0.00001f))
	{
		SetPosition(position + velocity * delta.asSeconds());
		SetRotation(atan2(normalize(velocity).y, normalize(velocity).x) * 180.0f / 3.1415926f);
	}
}

void Formation::SetRotation(float rot)
{
	orientation = rot;
}

glm::vec2 Formation::Seek(const glm::vec2 currentVelocity, glm::vec2 currentTarget)
{
	glm::vec2 desiredVelocity = glm::normalize(currentTarget - position) * maxSpeed;
	glm::vec2 newSteering = desiredVelocity - currentVelocity;

	newSteering = truncate(newSteering, steeringForce);

	return newSteering;
}

glm::vec2 Formation::truncate(glm::vec2 totrunc, float max) const
{
	float i = max / glm::length(totrunc);
	if (i > 1.0f)
	{
		i = 1.0f;
	}

	return totrunc * i;
}
