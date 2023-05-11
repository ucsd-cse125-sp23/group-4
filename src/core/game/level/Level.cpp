#include "core/game/level/Level.h"
#include "core/util/global.h"

void Level::tick() {
	std::vector<size_t> allIds = this->objects.getAllIds();
	for (size_t id : allIds)
	{
		PObject* obj = this->objects[id];
		obj->tick();
		if (obj->markedRemove())
		{
			this->objects.removeById(id);
		}
	}

	allIds = this->objects.getAllIds();
	std::vector<PObject*> collisions;
	for (size_t id : allIds)
	{
		PObject* self = objects[id];
		const CollisionBounds* selfBounds = self->getBounds();
		for (size_t id_other : allIds)
		{
			if (id == id_other)
				continue;
			PObject* obj = objects[id_other];
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
			collisions[ind]->onCollision(self);
			self->onCollision(collisions[ind]);
			collisions[ind] = collisions.back(); collisions.pop_back();
		}
	}

	for (size_t id : allIds)
	{
		PObject* self = objects[id];
		const CollisionBounds* selfBounds = self->getBounds();
		for (size_t id_other : allIds)
		{
			if (id == id_other)
				continue;
			PObject* obj = objects[id_other];
			switch (collisionTypeLUT[selfBounds->layer][obj->getBounds()->layer])
			{
			case CollisionType::NONE:
			case CollisionType::COLLISION:
				break;
			case CollisionType::TRIGGER:
				if (selfBounds->collides(obj->getBounds()))
				{
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
					self->onTrigger(obj);
					obj->onTrigger(self);
				}
				break;
			}
		}
	}

	this->age++;
}