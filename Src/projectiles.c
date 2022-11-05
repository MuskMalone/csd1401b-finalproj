#include "projectiles.h"

Projectile init_projectile(char Source, Position Start, CP_Vector Direction,char type) {
	Projectile Proj;
	Proj.type = type;
	Proj.pos.x = Start.x;
	Proj.pos.y = Start.y;
	Proj.Direction = Direction;
	Proj.radius = 10.0f;
	Proj.speed = 1000;
	Proj.source = Source;
	Proj.toRebound_NextFrame = 'n';
	return Proj;
}
void update_projectile(int index, Entity entities[], int wall_pos[GRID_ROWS][GRID_COLS]) {
	Projectile* proj = &(entities[index].projectile);
	int red_rgb = proj->source == 'p' ? 0 : 255;
	int blue_rgb = proj->source == 'p' ? 255 : 0;
	CP_Settings_Fill(CP_Color_Create(red_rgb, 0, blue_rgb, 255));
	
	if (proj->type == 'r') {
		if (proj->toRebound_NextFrame != 'n') {
			proj->pos = proj->Future_Pos;
			deflectprojectiles(proj->toRebound_NextFrame, index, entities);
			
		}
		else {
			moveEntity(&(proj->pos), proj->Direction.x * proj->speed, proj->Direction.y * proj->speed);
		}

		Position Left_Edge_Wall = (Position){ 200 , 0 };
		Position Right_Edge_Wall = (Position){ CP_System_GetWindowWidth()- 200 , 0 };
		Position Top_Edge_Wall = (Position){ 0,200 };
		Position Bottom_Edge_Wall = (Position){ 0 , CP_System_GetWindowHeight()- 200 };
		
		proj->Future_Pos = (Position){ proj->pos.x+ (proj->Direction.x * proj->speed * CP_System_GetDt()) , proj->pos.y + (proj->Direction.y * proj->speed * CP_System_GetDt())};
		proj->toRebound_NextFrame = 'n';
		
		for (int i = 0; i < GRID_ROWS; ++i) {
			for (int j = 0; j < GRID_COLS; ++j) {
				if (wall_pos[i][j]) {
					if (Wall_Edge_Check(proj, proj->radius, (Position) { WALL_DIM* (float)j, WALL_DIM* (float)i }, WALL_DIM, WALL_DIM)) {
						break;
					}
				}
			}
		}

		if (!Wall_Edge_Check(proj, proj->radius, Left_Edge_Wall, WALL_DIM, CP_System_GetWindowHeight())) {
			if (!Wall_Edge_Check(proj, proj->radius, Right_Edge_Wall, WALL_DIM, CP_System_GetWindowHeight())) {
				if (!Wall_Edge_Check(proj, proj->radius, Top_Edge_Wall, CP_System_GetWindowWidth(), WALL_DIM)) {
					Wall_Edge_Check(proj, proj->radius, Bottom_Edge_Wall, CP_System_GetWindowWidth(), WALL_DIM);
				}
			}
		}
	}
	
	CP_Graphics_DrawCircle(proj->pos.x, proj->pos.y, proj->radius * 2);

}

void deflectprojectiles(char source,int index, Entity entities[]) {
	Projectile* proj = &(entities[index].projectile);

	if (proj->type == 'r') {
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


int Wall_Edge_Check(Projectile* proj,float radius, Position rect, float width, float height) {
	int collided = 0;
	for (int i = 5; i >0; i--) {
		Position Future_Pos = (Position){ proj->pos.x + (proj->Direction.x * (proj->speed / i) * CP_System_GetDt()) , proj->pos.y + (proj->Direction.y * (proj->speed / i) * CP_System_GetDt()) };
		if (collisionCircleRect(Future_Pos, radius, rect, width, height)) {
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
			proj->Future_Pos.y = Rect_Center.y + direction * ((height / 2) + radius);
		}
		else {
			proj->toRebound_NextFrame = 'x';
			int direction = (abs(proj->Future_Pos.x - rect.x) < abs(proj->Future_Pos.x - (rect.x + width))) ? -1 : 1;
			proj->Future_Pos.x = Rect_Center.x + direction * ((width / 2) + radius);
		}
		return(1);
	}
	else {
		return 0;
	}
}
