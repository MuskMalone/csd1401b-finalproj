#include "projectiles.h"
static float Lifespan_count = 0;

entity_struct init_projectile(void) {
	Projectile Proj;
	Proj.speed = 500;
	return (entity_struct) {.projectile = Proj};
}
void set_projectile_values(Projectile* Proj, char Source, char type, int radius, Position Start_Pos, CP_Vector Direction_Vector) {
	Proj->type = type;			// 2 Projectile types. Static (For melee or exploding enemy) and Mobile (Ranged mobs)
	Proj->pos.x = Start_Pos.x;
	Proj->pos.y = Start_Pos.y;
	Proj->Direction = Direction_Vector;
	Proj->radius = radius;	   // default size 20
	
	Proj->source = Source;	  // The owner of the projectile. Prevents the projectile from attacking its owner
	Proj->Future_Pos = Proj->pos;
	Proj->toRebound_NextFrame = 'n';
	Proj->LifeSpan = 0.01;
}
void update_projectile(int index, Entity entities[], int wall_pos[GRID_ROWS][GRID_COLS]) {
	Projectile* proj = &(entities[index].projectile);
	//int red_rgb = proj->source == 'p' ? 0 : 255;
	//int blue_rgb = proj->source == 'p' ? 255 : 0;
	int to_Rebound = 0;
	CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
	
	if (proj->type == 'm') {
		if (proj->toRebound_NextFrame != 'n') {
			proj->pos = proj->Future_Pos;
			deflectprojectiles(proj->toRebound_NextFrame, index, entities);
		}
		else {
			moveEntity(&(proj->pos), proj->Direction.x * proj->speed, proj->Direction.y * proj->speed);
		}
		if(!Entities_Collision_Check(proj, index, entities)){
			Position Left_Edge_Wall = (Position){ -WALL_DIM , 0 };
			Position Right_Edge_Wall = (Position){ CP_System_GetWindowWidth() , 0 };
			Position Top_Edge_Wall = (Position){ 0,-WALL_DIM };
			Position Bottom_Edge_Wall = (Position){ 0 , CP_System_GetWindowHeight() };
			proj->toRebound_NextFrame = 'n';
		


			for (int i = 0; i < GRID_ROWS; ++i) {
				for (int j = 0; j < GRID_COLS; ++j) {
					if (wall_pos[i][j]) {
						if(!to_Rebound)
						if (Wall_Edge_Check(proj, (Position) { WALL_DIM* (float)j, WALL_DIM* (float)i }, WALL_DIM, WALL_DIM)) {
							to_Rebound = 1;
							break;
						}
					}
				}
			}
			if (!to_Rebound) {
				if (!Wall_Edge_Check(proj, Left_Edge_Wall, WALL_DIM, CP_System_GetWindowHeight())) {
					if (!Wall_Edge_Check(proj, Right_Edge_Wall, WALL_DIM, CP_System_GetWindowHeight())) {
						if (!Wall_Edge_Check(proj, Top_Edge_Wall, CP_System_GetWindowWidth(), WALL_DIM)) {
							Wall_Edge_Check(proj, Bottom_Edge_Wall, CP_System_GetWindowWidth(), WALL_DIM);
						}
					}
				}
			}	
		CP_Graphics_DrawCircle(proj->pos.x, proj->pos.y, proj->radius * 2);
		}
	}
	else {
		//Draw the static proj explosion animation here
		Entities_Collision_Check(proj, index, entities);
		Lifespan_count += CP_System_GetDt();
		if (Lifespan_count >= proj->LifeSpan) {	
			entities[index].type = entity_null;
		}

		
	}
}

void deflectprojectiles(char source,int index, Entity entities[]) {
	Projectile* proj = &(entities[index].projectile);

	if (proj->type == 'm') {
		if (source == 'x') {
			proj->Direction.x *= -1;
		}
		else if (source == 'y') {
			proj->Direction.y *= -1;
		}
		else {			
			proj->Direction = CP_Vector_Negate(proj->Direction);
			proj->source = source;
		}
	}
}

int Wall_Edge_Check(Projectile* proj, Position rect, float width, float height) {
	int collided = 0;
	for (int i = 5; i >0; i--) {
		Position Future_Pos = (Position){ proj->pos.x + (proj->Direction.x * (proj->speed / i) * CP_System_GetDt()) , proj->pos.y + (proj->Direction.y * (proj->speed / i) * CP_System_GetDt()) };
		if (collisionCircleRect(Future_Pos, proj->radius, rect, width, height)) {
			proj->Future_Pos = Future_Pos;
			collided = 1;
			break;
		}
	}
	if (collided){
		Position Rect_Center = (Position){rect.x+(width/2),rect.y + (height/2)};
		float y_diff = (abs(proj->Future_Pos.y - rect.y) < abs(proj->Future_Pos.y - (rect.y + height))) ? (proj->Future_Pos.y - rect.y) : (proj->Future_Pos.y - (rect.y + height));
		float x_diff = (abs(proj->Future_Pos.x - rect.x) < abs(proj->Future_Pos.x - (rect.x + width))) ? (proj->Future_Pos.x - rect.x) : (proj->Future_Pos.x - (rect.x + width));
		
		if (abs(y_diff) < abs(x_diff)) {
			proj->toRebound_NextFrame = 'y';
			int direction = (abs(proj->Future_Pos.y - rect.y) < abs(proj->Future_Pos.y - (rect.y + height))) ? -1 : 1;
			proj->Future_Pos.y = Rect_Center.y + direction * ((height / 2) + proj->radius);
		}
		else {
			proj->toRebound_NextFrame = 'x';
			int direction = (abs(proj->Future_Pos.x - rect.x) < abs(proj->Future_Pos.x - (rect.x + width))) ? -1 : 1;
			proj->Future_Pos.x = Rect_Center.x + direction * ((width / 2) + proj->radius);
		}
	}
	return collided;
}

int Entities_Collision_Check(Projectile* proj, int index, Entity entities[]){
	int Proj_Collided = 0;
	for (int i = 0; i < ENTITY_CAP; ++i) {
		if (entities[i].type == entity_null) continue;
		if (proj->source != 'p') {
			if (entities[i].type == entity_player) {
				if (collisionCircle(entities[i].player.pos, entities[i].player.diameter / 2, proj->pos, proj->radius)) {
					damage_player(&(entities[i].player));
					entities[index].type = entity_null;
					Proj_Collided = 1;
					break;
				}
			}

		}
		else {
			if (entities[i].type == entity_boss) {
				if (collisionCircle(entities[i].boss.pos, entities[i].boss.diameter / 2, proj->pos, proj->radius)) {
					damage_boss(&(entities[i].boss));
					entities[index].type = entity_null;
					Proj_Collided = 1;
					break;
				}
			}
			// Check if its Mob
		}
	}
	if (Proj_Collided) {
		//Draw the Mobile projectile destroy animation
	}
	return Proj_Collided;
}


