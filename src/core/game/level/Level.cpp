#include "core/game/level/Level.h"

void Level::tick() {
	for (int i = 0; i < this->objects.size(); i++)
	{
		PObject* obj = this->objects[i];
		obj->tick();
		if (obj->markedRemove())
		{
			delete this->objects[i];
			this->objects[i] = this->objects.back();
			this->objects.pop_back();
			i--;
		}
	}

	std::vector<PObject*> collisions;
	for (int i = 0; i < objects.size(); i++)
	{
		PObject* self = objects[i];
		const CollisionBounds* selfBounds = self->getBounds();
		for (int j = i + 1; j < objects.size(); j++)
		{
			PObject* obj = objects[j];
			switch (collisionTypeLUT[selfBounds->layer][obj->getBounds()->layer])
			{
			case CollisionType::NONE:
			case CollisionType::TRIGGER:
				break;
			case CollisionType::COLLISION:
				collisions.push_back(obj);
				break;
			}
		}
		for (PObject* obj : this->environment->getCollisions())
		{
			switch (collisionTypeLUT[selfBounds->layer][obj->getBounds()->layer])
			{
			case CollisionType::NONE:
			case CollisionType::TRIGGER:
				break;
			case CollisionType::COLLISION:
				collisions.push_back(obj);
				break;
			}
		}

		while (!collisions.empty())
		{
			size_t ind = -1;
			vec4f minMTV = vec4f(0.0f, 0.0f, 0.0f, std::numeric_limits<float>::max());

			for (int k = 0; k < collisions.size(); k++)
			{
				vec4f mtv = selfBounds->mtv(collisions[k]->getBounds());
				if (mtv.w > 0)
				{
					if (mtv.w < minMTV.w)
					{
						ind = k;
						minMTV = mtv;
					}
				}
				else
				{
					collisions[k--] = collisions.back(); collisions.pop_back();
				}
			}
			if (collisions.empty() || ind == -1)
				break;

			vec3f norm = normalize(vec3f(minMTV));
			if (self->vel.y < 0 && minMTV.y / (abs(minMTV.x) + abs(minMTV.z)) > 0.05)
				self->onGround = true;
			if (collisions[ind]->isStatic())
			{
				//Shift object out
				//Set object velocity to only tangential
				//Apply friction on object tangent velocoty
				self->addPos(vec3f(minMTV) * (minMTV.w + 0.0001f));
				self->vel = tangent(self->vel, norm);
				self->vel -= self->vel * minMTV.w * collisions[ind]->getBounds()->friction;
			}
			else
			{
				if (collisions[ind]->vel.y < 0 && -minMTV.y > 0)
					collisions[ind]->onGround = true;
				//As above but halved;
				self->addPos(vec3f(minMTV) * (minMTV.w * 0.5f + 0.0001f));
				self->vel = tangent(self->vel, norm);
				self->vel -= self->vel * minMTV.w * collisions[ind]->getBounds()->friction * 0.5f;
				collisions[ind]->addPos(-vec3f(minMTV) * (minMTV.w * 0.5f + 0.001f));
				collisions[ind]->vel = tangent(collisions[ind]->vel, -norm);
				collisions[ind]->vel -= collisions[ind]->vel * minMTV.w * self->getBounds()->friction * 0.5f;
			}
			this->eventManager->fireCollisionEvent(self, collisions[ind]);
			collisions[ind]->onCollision(self);
			self->onCollision(collisions[ind]);
			collisions[ind] = collisions.back(); collisions.pop_back();
		}
	}

	for (int i = 0; i < objects.size(); i++)
	{
		PObject* self = objects[i];
		const CollisionBounds* selfBounds = self->getBounds();
		for (int j = i + 1; j < objects.size(); j++)
		{
			PObject* obj = objects[j];
			switch (collisionTypeLUT[selfBounds->layer][obj->getBounds()->layer])
			{
			case CollisionType::NONE:
			case CollisionType::COLLISION:
				break;
			case CollisionType::TRIGGER:
				if (selfBounds->collides(obj->getBounds()))
				{
					this->eventManager->fireTriggerEvent(self, obj);
					self->onTrigger(obj);
					obj->onTrigger(self);
				}
				break;
			}
		}
		for (PObject* obj : this->environment->getCollisions())
		{
			switch (collisionTypeLUT[selfBounds->layer][obj->getBounds()->layer])
			{
			case CollisionType::NONE:
			case CollisionType::COLLISION:
				break;
			case CollisionType::TRIGGER:
				if (selfBounds->collides(obj->getBounds()))
				{
					this->eventManager->fireTriggerEvent(self, obj);
					self->onTrigger(obj);
					obj->onTrigger(self);
				}
				break;
			}
		}
	}

	this->age++;
}