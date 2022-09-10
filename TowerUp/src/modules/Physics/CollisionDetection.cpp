#include <tuple>

#include "CollisionDetection.h"
#include "CollisionData.h"
#include "AABB.h"
#include "Circle.h"
#include "../SFMLUtils.hpp"
#include "../CollisionLayer.h"
#include "../Engine2D/TileMap.h"
#include "../Entity/DynamicEntity.h"

sf::Vector2f ClosestPoint(sf::Vector2f aabbPosition, const AABB* aabb, sf::Vector2f pos)
{
    float hw = aabb->Width * 0.5f;
    float hh = aabb->Height * 0.5f;

	return sf::Vector2f
	{
		sf::clamp(pos.x, aabbPosition.x - hw, aabbPosition.x + hw),
		sf::clamp(pos.y, aabbPosition.y - hh, aabbPosition.y + hh)
	};
}

CollisionData CirclevsCircle(const Circle* c1, sf::Vector2f pos1, const Circle* c2, sf::Vector2f pos2)
{
    CollisionData data;

	sf::Vector2f dir = pos2 - pos1;
	float radiusSum = c1->Radius + c2->Radius;
	float distSquared = sf::lengthSquared(dir);

	if (distSquared > radiusSum * radiusSum)
		return data;

	float dist = 0.0f;
	if (distSquared != 0.0f)
	{
		dist = sqrt(distSquared);
		dir /= dist;
	}

	data.Collision = true;
	data.Normal = dir;
	data.Penetration = radiusSum - dist;

    return data;
}

std::tuple<float, sf::Vector2f> GetPenetrationAndNormal(sf::Vector2f pos1, const AABB* aabb, sf::Vector2f pos2)
{
	const sf::Vector2f Normals[4] = 
	{
		sf::Vector2f{ 1.0f, 0.0f},
		sf::Vector2f{-1.0f, 0.0f},
		sf::Vector2f{0.0f,  1.0f},
		sf::Vector2f{0.0f, -1.0f}
	};

	float xMax = pos2.x + aabb->Width * 0.5f;
	float xMin = pos2.x - aabb->Width * 0.5f;
	float yMax = pos2.y + aabb->Height * 0.5f;
	float yMin = pos2.y - aabb->Height * 0.5f;

	sf::Vector2f Overlaps[4] =
	{
		sf::Vector2f{pos1.x - xMin, 0.0f},
		sf::Vector2f{pos1.x - xMax, 0.0f},
		sf::Vector2f{0.0f, pos1.y - yMin},
		sf::Vector2f{0.0f, pos1.y - yMax}
	};

	float minOverlap = sf::dot(Overlaps[0], Normals[0]);
	sf::Vector2f minNormal = Normals[0];

	for(int i = 0;i < 3; ++i)
	{
		float newOverlap = sf::dot(Overlaps[i], Normals[i]);
		if(newOverlap < minOverlap)
		{
			minOverlap = newOverlap;
			minNormal = Normals[i];
		}
	}

	return std::make_tuple(minOverlap, minNormal);
}

CollisionData CirclevsAABB(const Circle* circle, sf::Vector2f pos1, const AABB* aabb, sf::Vector2f pos2)
{
    CollisionData data;

	sf::Vector2f closestPoint = ClosestPoint(pos2, aabb, pos1);

	if(closestPoint == pos1) //Circle is inside of the AABB	
	{
		auto tuple = GetPenetrationAndNormal(pos1, aabb, pos2);

		data.Collision = true;
		data.Penetration = std::get<0>(tuple) + circle->Radius;
		data.Normal = std::get<1>(tuple);

		return data;
	}

	sf::Vector2f dir = closestPoint - pos1;
	float distSquared = sf::lengthSquared(dir);

	if (distSquared > circle->Radius * circle->Radius) //Circle does not collide
		return data;

	float dist = 0.0f;
	if (distSquared != 0.0f)
	{
		dist = sqrt(distSquared);
		dir /= dist;
	}

	data.Collision = true;
	data.Normal = dir;
	data.Penetration = circle->Radius - dist;

    return data;
}

CollisionData AABBvsAABB(const AABB* aabb1, sf::Vector2f pos1, const AABB* aabb2, sf::Vector2f pos2)
{
    CollisionData data;

    float widthSum = aabb1->Width * 0.5f + aabb2->Width * 0.5f;
    float xDistance = std::abs(pos1.x - pos2.x);
    float xOverlap = widthSum - xDistance;

    if(xOverlap < 0.0f)
        return data;

    float heightSum = aabb1->Height * 0.5f + aabb2->Height * 0.5f;
    float yDistance = std::abs(pos1.y - pos2.y);
    float yOverlap = heightSum - yDistance;

    if(yOverlap < 0.0f)
        return data;

    data.Collision = true;
	if (xOverlap < yOverlap) //Normal points to the x axis
	{
		data.Penetration = xOverlap;
		data.Normal = (pos1.x < pos2.x) ? sf::Vector2f{ 1.0f, 0.0f } : sf::Vector2f{ -1.0f, 0.0f };
	}
	else                     //Normal points to the y axis
	{
		data.Penetration = yOverlap;
		data.Normal = (pos1.y < pos2.y) ? sf::Vector2f{ 0.0f, 1.0f } : sf::Vector2f{ 0.0f, -1.0f };
	}

    return data;
}


TileCollisionData GetAABBHorizontalTileCollisionData(const DynamicEntity* entity, const AABB* aabb, sf::Vector2f velocity, 
	float left, float top, const CollisionLayer& collisionLayer)
{
	constexpr float OFFSET = 0.1f;

	TileCollisionData data;

	int32_t minY = collisionLayer.GetScaledCoord(top);
    int32_t maxY = collisionLayer.GetScaledCoord(top + aabb->Height + 0.01f);

	if(velocity.x < 0)	//Collision from the right side of the tile
	{
		int32_t minX = collisionLayer.GetScaledCoord(left + velocity.x);
        int32_t maxX = collisionLayer.GetScaledCoord(left);

        for(int32_t y = minY; y <= maxY; ++y)
        {
            for(int32_t x = maxX; x >= minX; --x)
            {
                if(collisionLayer.Collides(x, y))
                {
					float penetration = (x + 1) * E2D::TileMap::TILE_SIZE - (entity->Position.x - aabb->Width * 0.5f);
					if(penetration > data.Penetration)
					{
						data.Collision = true;
						data.Normal = sf::Vector2f{ 1.0f, 0.0f };
						data.Penetration = penetration;
						data.TileType = collisionLayer.Get(x, y);
					}

					break;
                }
            }
        }
	}
	else if(velocity.x > 0) //Collision from the left side of the tile
	{
		int32_t minX = collisionLayer.GetScaledCoord(left + aabb->Width);
        int32_t maxX = collisionLayer.GetScaledCoord(left + aabb->Width + velocity.x);

        for(int32_t y = minY; y <= maxY; ++y)
        {
            for(int32_t x = minX; x <= maxX; ++x)
            {
                if(collisionLayer.Collides(x, y))
                {
					float penetration = (entity->Position.x + aabb->Width * 0.5f + OFFSET) - (x * E2D::TileMap::TILE_SIZE);
					if(penetration > data.Penetration)
					{
						data.Collision = true;
						data.Normal = sf::Vector2f{ -1.0f, 0.0f };
						data.Penetration = penetration;
						data.TileType = collisionLayer.Get(x, y);
					}
					
					break;
                }
            }
        }
	}

	return data;
}

TileCollisionData GetAABBVerticalTileCollisionData(const DynamicEntity* entity, const AABB* aabb, sf::Vector2f velocity, 
	float left, float top, const CollisionLayer& collisionLayer)
{
	constexpr float OFFSET = 0.1f;

	TileCollisionData data;

	int32_t minX = collisionLayer.GetScaledCoord(left);
    int32_t maxX = collisionLayer.GetScaledCoord(left + aabb->Width);

	if(velocity.y < 0) //Collision from the bottom side of the tile
    {
        int32_t minY = collisionLayer.GetScaledCoord(top + velocity.y);
        int32_t maxY = collisionLayer.GetScaledCoord(top);

        for(int32_t x = minX; x <= maxX; ++x)
        {
            for(int32_t y = maxY; y >= minY; --y)
            {
                if(collisionLayer.Collides(x, y))
                {
					float penetration = (y + 1) * E2D::TileMap::TILE_SIZE - (entity->Position.y - aabb->Height * 0.5f);
					if(penetration > data.Penetration)
					{
						data.Collision = true;
						data.Normal = sf::Vector2f{ 0.0f, 1.0f };
						data.Penetration = penetration;
						data.TileType = collisionLayer.Get(x, y);
					}

					break;
                }
            }
        }
    }
    else if(velocity.y > 0) //Collision from the top side of the tile
    {
        int32_t minY = collisionLayer.GetScaledCoord(top + aabb->Height);
        int32_t maxY = collisionLayer.GetScaledCoord(top + aabb->Height + velocity.y);

        for(int32_t x = minX; x <= maxX; ++x)
        {
            for(int32_t y = minY; y <= maxY; ++y)
            {
                if(collisionLayer.Collides(x, y))
                {
					float penetration = (entity->Position.y + aabb->Height * 0.5f + OFFSET) - (y * E2D::TileMap::TILE_SIZE);
					if(penetration > data.Penetration)
					{
						data.Collision = true;
						data.Normal = sf::Vector2f{ 0.0f, -1.0f };
						data.Penetration = penetration;
						data.TileType = collisionLayer.Get(x, y);
					}

					break;
                }
            }
        }
    }

	return data;
}

std::pair<TileCollisionData, TileCollisionData> EntityAABBvsCollisionLayer
	(const DynamicEntity* entity, const AABB* aabb, const CollisionLayer& collisionLayer)
{
	TileCollisionData horizontalCollision;
	TileCollisionData verticalCollision;

	auto data = std::make_pair(TileCollisionData{}, TileCollisionData{}); 

	sf::Vector2f lastPosition = entity->Position - entity->LastVelocity;

	float left = lastPosition.x - aabb->Width * 0.5f;
	float top =  lastPosition.y - aabb->Height * 0.5f;

	data.first = GetAABBHorizontalTileCollisionData(entity, aabb, entity->LastVelocity, left, top, collisionLayer);
	data.second = GetAABBVerticalTileCollisionData(entity, aabb, entity->LastVelocity, left, top, collisionLayer);

	if(!data.first.Collision && !data.second.Collision)
	{
		left = entity->Position.x - aabb->Width * 0.5f;
		top =  entity->Position.y - aabb->Height * 0.5f;

		data.second = GetAABBVerticalTileCollisionData(entity, aabb, entity->LastVelocity, left, top, collisionLayer);
	}

	return data;
}

TileCollisionData GetCircleHorizontalTileCollisionData
	(const DynamicEntity* entity, const Circle* circle, sf::Vector2f velocity, float left, float top, const CollisionLayer& collisionLayer)
{
	constexpr float OFFSET = 0.1f;

	TileCollisionData data;

	int32_t minY = collisionLayer.GetScaledCoord(top);
    int32_t maxY = collisionLayer.GetScaledCoord(top + circle->Radius + circle->Radius);

	if(velocity.x < 0)	//Collision from the right side of the tile
	{
		int32_t minX = collisionLayer.GetScaledCoord(left + velocity.x);
        int32_t maxX = collisionLayer.GetScaledCoord(left);

        for(int32_t y = minY; y <= maxY; ++y)
        {
            for(int32_t x = maxX; x >= minX; --x)
            {
                if(collisionLayer.Collides(x, y))
                {
					float penetration = (x + 1) * E2D::TileMap::TILE_SIZE - (entity->Position.x - circle->Radius);
					if(penetration > data.Penetration)
					{
						data.Collision = true;
						data.Normal = sf::Vector2f{ 1.0f, 0.0f };
						data.Penetration = penetration;
						data.TileType = collisionLayer.Get(x, y);
					}

					break;
                }
            }
        }
	}
	else if(velocity.x > 0) //Collision from the left side of the tile
	{
		int32_t minX = collisionLayer.GetScaledCoord(left + circle->Radius + circle->Radius );
        int32_t maxX = collisionLayer.GetScaledCoord(left + circle->Radius + circle->Radius + velocity.x);

        for(int32_t y = minY; y <= maxY; ++y)
        {
            for(int32_t x = minX; x <= maxX; ++x)
            {
                if(collisionLayer.Collides(x, y))
                {
					float penetration = (entity->Position.x + circle->Radius + OFFSET) - (x * E2D::TileMap::TILE_SIZE);
					if(penetration > data.Penetration)
					{
						data.Collision = true;
						data.Normal = sf::Vector2f{ -1.0f, 0.0f };
						data.Penetration = penetration;
						data.TileType = collisionLayer.Get(x, y);
					}

					break;
                }
            }
        }
	}

	return data;
}

TileCollisionData GetCircleVerticalTileCollisionData
	(const DynamicEntity* entity, const Circle* circle, sf::Vector2f velocity, float left, float top, const CollisionLayer& collisionLayer)
{
	constexpr float OFFSET = 0.1f;

	TileCollisionData data;

	int32_t minX = collisionLayer.GetScaledCoord(left);
    int32_t maxX = collisionLayer.GetScaledCoord(left + circle->Radius + circle->Radius);

	if(velocity.y < 0) //Collision from the bottom side of the tile
    {
        int32_t minY = collisionLayer.GetScaledCoord(top + velocity.y);
        int32_t maxY = collisionLayer.GetScaledCoord(top);

        for(int32_t x = minX; x <= maxX; ++x)
        {
            for(int32_t y = maxY; y >= minY; --y)
            {
                if(collisionLayer.Collides(x, y))
                {
					float penetration = (y + 1) * E2D::TileMap::TILE_SIZE - (entity->Position.y - circle->Radius);
					if(penetration > data.Penetration)
					{
						data.Collision = true;
						data.Normal = sf::Vector2f{ 0.0f, 1.0f };
						data.Penetration = penetration;
						data.TileType = collisionLayer.Get(x, y);
					}

					break;
                }
            }
        }
    }
    else if(velocity.y > 0) //Collision from the top side of the tile
    {
        int32_t minY = collisionLayer.GetScaledCoord(top + circle->Radius + circle->Radius);
        int32_t maxY = collisionLayer.GetScaledCoord(top + circle->Radius + circle->Radius + velocity.y);

        for(int32_t x = minX; x <= maxX; ++x)
        {
            for(int32_t y = minY; y <= maxY; ++y)
            {
                if(collisionLayer.Collides(x, y))
                {
					float penetration = (entity->Position.y + circle->Radius + OFFSET) - (y * E2D::TileMap::TILE_SIZE);
					if(penetration > data.Penetration)
					{
						data.Collision = true;
						data.Normal = sf::Vector2f{ 0.0f, -1.0f };
						data.Penetration = penetration;
						data.TileType = collisionLayer.Get(x, y);
					}

					break;
                }
            }
        }
    }

	return data;
}

std::pair<TileCollisionData, TileCollisionData> EntityCirclevsCollisionLayer
	(const DynamicEntity* entity, const Circle* circle, const CollisionLayer& collisionLayer)
{
	TileCollisionData horizontalCollision;
	TileCollisionData verticalCollision;

	auto data = std::make_pair(TileCollisionData{}, TileCollisionData{}); 

	sf::Vector2f lastPosition = entity->Position - entity->LastVelocity;

	float left = lastPosition.x - circle->Radius;
	float top =  lastPosition.y - circle->Radius;

	data.first = GetCircleHorizontalTileCollisionData(entity, circle, entity->LastVelocity, left, top, collisionLayer);
	data.second = GetCircleVerticalTileCollisionData(entity, circle, entity->LastVelocity, left, top, collisionLayer);

	if(!data.first.Collision && !data.second.Collision)
	{
		left = entity->Position.x - circle->Radius;
		top =  entity->Position.y - circle->Radius;

		data.second = GetCircleVerticalTileCollisionData(entity, circle, entity->LastVelocity, left, top, collisionLayer);
	}

	return data;
}