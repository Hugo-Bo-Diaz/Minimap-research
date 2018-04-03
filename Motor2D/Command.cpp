#include "Command.h"
#include "Unit.h"
#include "p2Log.h"
#include "j1EntityController.h"
#include "j1Map.h"

#define SPEED_CONSTANT 100


// BASE CLASSES: =========================

void Command::Execute(float dt)
{
	bool ret = false;

	switch (state)    
	{
	case TO_INIT: state = UPDATE;	ret = OnInit();			break;    // state change MUST happen before calling the method
	case UPDATE:					ret = OnUpdate(dt);		break;
	case TO_STOP: state = FINISHED;	ret = OnStop();			break;
	default:												break;
	}

	if (!ret) LOG("Error in command flow");
}

void Command::Stop()	{ state = TO_STOP; }
void Command::Restart() { OnStop(); state = TO_INIT; }


//		UNITS: =============================
// MOVETO

bool MoveTo::OnInit()
{
	next_step = { 0.0f,0.0f };
	if (path.empty()) 
	{
		iPoint pos = App->map->WorldToMap(unit->position.x, unit->position.y);
	}
	else Repath();    // if we have been already initalized and possess a path, repath

	return true;
}

bool MoveTo::OnUpdate(float dt)
{
	if (path.empty()) { Stop(); return true; }
	else {
		if (!waiting_for)
		{
			unit->position += next_step;
			unit->collider.x = unit->position.x; unit->collider.y = unit->position.y;

			iPoint unit_pos = App->map->WorldToMap(unit->position.x, unit->position.y);

			iPoint direction = path.front() - unit_pos;
			direction.Normalize();

			if (unit_pos == path.front())
				path.pop_front();
		}
	}

	if (!CheckCollisions()) Stop();

	return true;
}

bool MoveTo::OnStop()
{
	if (!path.empty())	path.clear();
	return true;
}

bool MoveTo::CheckCollisions()
{
	waiting_for = nullptr;
	std::vector<Entity*> collisions = App->entitycontroller->CheckCollidingWith(unit);

	if (!collisions.empty())
	{
		if (collisions.size() == 1)
		{
			if (collisions.front()->entity_type != UNIT)
			{
			}
			else {
				Unit* colliding_unit = (Unit*)collisions.front();
				if (colliding_unit->commands.empty())  return colliding_unit->Pushed(next_step);
				else
				{
					MoveTo * colliding_moveTo = nullptr;
					switch (colliding_unit->commands.front()->type)
					{
					case MOVETO:
						colliding_moveTo = (MoveTo*)colliding_unit->commands.front();
						if (!colliding_moveTo->waiting_for)
							if(colliding_moveTo->waiting_for != unit) waiting_for = colliding_unit;
						else {
							if (colliding_moveTo->next_step.Normalized() == next_step.Normalized().Negate())
							{
								if (colliding_unit->Pushed(next_step)) { colliding_unit->commands[1]->state = TO_INIT; return true; }
								else {
									if (unit->Pushed(colliding_moveTo->next_step)) { state = TO_INIT; return true; }
									else										   return false;
								}
							}
							else waiting_for = colliding_unit;
						}
						break;
					case ATTACK:
						state = TO_INIT;
						return unit->Pushed(next_step.Negate());
						break;
					default:
						return colliding_unit->Pushed(next_step);
						break;
					}
				}
			}
		}
		else {   // collisions.size() > 1
			//TODO;
		}
	}

	return true;
}

void MoveTo::Repath()
{
	iPoint pos = App->map->WorldToMap(unit->position.x, unit->position.y);

	std::list<iPoint> new_path;

}


// ATTACKING MOVE TO

bool AttackingMoveTo::OnUpdate(float dt) 
{
	Unit* enemy = unit->SearchNearestEnemy();
	if (enemy)
	{
		iPoint enemy_position = App->map->WorldToMap(enemy->position.x, enemy->position.y);
		iPoint pos = App->map->WorldToMap(unit->position.x, unit->position.y);

		if (pos.DistanceTo(enemy_position) < unit->line_of_sight)
		{
			Attack* new_attack_command = new Attack(unit);
			new_attack_command->enemy_position = enemy_position;
			new_attack_command->state = UPDATE;
			unit->commands.push_front(new_attack_command);

			state = TO_INIT;
			return true;
		}
	}

	unit->position += next_step;
	unit->collider.x = unit->position.x; unit->collider.y = unit->position.y;

	iPoint unit_pos = App->map->WorldToMap(unit->position.x, unit->position.y);

	if (unit_pos == path.front())
	{
		path.pop_front();
		if (path.empty()) { Stop(); return true; }
	}

	iPoint direction = path.front() - unit_pos;
	direction.Normalize();

	fPoint velocity = (direction * unit->speed * dt * SPEED_CONSTANT);
	next_step = velocity;

	return true;
}


// ATTACK

bool Attack::OnInit()
{
	Unit* enemy = unit->SearchNearestEnemy();
	if (enemy)
	{
		enemy_position = App->map->WorldToMap(enemy->position.x, enemy->position.y);
		iPoint pos     = App->map->WorldToMap(unit->position.x, unit->position.y);

		if (pos.DistanceTo(enemy_position) > unit->line_of_sight)	Stop();
	}
	else Stop();

	return true;
}

bool Attack::OnUpdate(float dt)
{
	iPoint pos = App->map->WorldToMap(unit->position.x, unit->position.y);

	if (pos.DistanceTo(enemy_position) <= unit->range)
	{
		// if(unit->current_animation = unit->animations[ATTACK_ANIMATION])
		//TODO
	}
	return true;
}

bool Attack::OnStop()
{
	// TODO
	return true;
}

// HOLD

bool Hold::OnInit()
{
	iPoint pos = App->map->WorldToMap(unit->position.x, unit->position.y);

	if (pos != held_position)
	{
		MoveTo* new_moveto_command = new MoveTo(unit, held_position);
		unit->commands.push_front(new_moveto_command);

		Restart();
	}
	return true;
}

bool Hold::OnUpdate(float dt)
{
	Unit* enemy = unit->SearchNearestEnemy();
	if (enemy)
	{
		iPoint enemy_position = App->map->WorldToMap(enemy->position.x, enemy->position.y);
		iPoint pos = App->map->WorldToMap(unit->position.x, unit->position.y);

		if (pos.DistanceTo(enemy_position) < unit->line_of_sight)
		{
			Attack* new_attack_command = new Attack(unit);
			new_attack_command->enemy_position = enemy_position;
			new_attack_command->state = UPDATE;
			unit->commands.push_front(new_attack_command);

			Restart();
		}
	}
	
	return true;
}


// PATROL

bool Patrol::OnInit()
{
	// TODO  (Check if the paths between points are possible: here or before creating the Patrol command?
	return true;
}

bool Patrol::OnUpdate(float dt) 
{
	current_point++;
	if (current_point == patrol_points.size()) current_point = 0;

	AttackingMoveTo* new_a_moveto_command = new AttackingMoveTo(unit, patrol_points[current_point]);
	unit->commands.push_front(new_a_moveto_command);

	return true;
}


//		SQUADS: =============================
// MOVETOSQUAD

bool MoveToSquad::OnInit()
{
	if (!unit->squad) { Stop(); return true; }
	else squad = unit->squad;

	iPoint map_pos = App->map->WorldToMap(unit->position.x, unit->position.y);

	return true;
}

bool MoveToSquad::OnUpdate(float dt)
{
	bool all_idle = true;

	return true;
}


bool MoveToSquad::ProcessPath(const std::list<iPoint>& path)
{
	std::vector<std::list<iPoint*>*> paths_list;

	return true;
}

// RESHAPE

bool ReshapeSquad::OnInit()
{
	if (!unit->squad) { Stop(); return true; }
	else squad = unit->squad;

}

bool ReshapeSquad::OnUpdate(float dt)
{
	bool all_idle = true;

	if (all_idle) 
		Stop();

	return true;
}
