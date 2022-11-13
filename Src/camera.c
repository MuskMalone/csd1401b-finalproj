#include "player.h"
#include "boss.h"
#include "mobs.h"
#include "projectiles.h"
#include "camera.h"
CP_Image Mobile_Proj_E;
CP_Image Mobile_Proj_P;
CP_Image player_projectile_sprites[MELEE_PROJECTILE_SPRITE_COUNT];
CP_Image enemy_projectile_sprites[MELEE_PROJECTILE_SPRITE_COUNT];

CP_Image melee_mob[MELEE_MOB_SPRITE_COUNT];
CP_Image explode_mob[MELEE_MOB_SPRITE_COUNT];
CP_Image range_mob[RANGE_MOB_SPRITE_COUNT];

void init_sprites(void) {
	Mobile_Proj_E = CP_Image_Load("./Assets/Tiles/Projectiles/Mobile_Proj_E.png");
	Mobile_Proj_P = CP_Image_Load("./Assets/Tiles/Projectiles/Mobile_Proj_P.png");
	player_projectile_sprites[0] = CP_Image_Load("./Assets/Tiles/Projectiles/Static_Proj_P1.png");
	player_projectile_sprites[1] = CP_Image_Load("./Assets/Tiles/Projectiles/Static_Proj_P2.png");
	player_projectile_sprites[2] = CP_Image_Load("./Assets/Tiles/Projectiles/Static_Proj_P3.png");
	player_projectile_sprites[3] = CP_Image_Load("./Assets/Tiles/Projectiles/Static_Proj_P4.png");
	player_projectile_sprites[4] = CP_Image_Load("./Assets/Tiles/Projectiles/Static_Proj_P5.png");
	player_projectile_sprites[5] = CP_Image_Load("./Assets/Tiles/Projectiles/Static_Proj_P6.png");

	enemy_projectile_sprites[0] = CP_Image_Load("./Assets/Tiles/Projectiles/Static_Proj_E1.png");
	enemy_projectile_sprites[1] = CP_Image_Load("./Assets/Tiles/Projectiles/Static_Proj_E2.png");
	enemy_projectile_sprites[2] = CP_Image_Load("./Assets/Tiles/Projectiles/Static_Proj_E3.png");
	enemy_projectile_sprites[3] = CP_Image_Load("./Assets/Tiles/Projectiles/Static_Proj_E4.png");
	enemy_projectile_sprites[4] = CP_Image_Load("./Assets/Tiles/Projectiles/Static_Proj_E5.png");
	enemy_projectile_sprites[5] = CP_Image_Load("./Assets/Tiles/Projectiles/Static_Proj_E6.png");

	melee_mob[0] = CP_Image_Load("./Assets/Tiles/Mobs/Melee/Slime_Idle1.png");
	melee_mob[1] = CP_Image_Load("./Assets/Tiles/Mobs/Melee/Slime_Idle2.png");

	explode_mob[0] = CP_Image_Load("./Assets/Tiles/Mobs/Explode/ExplodeSlime_Idle1.png");
	explode_mob[1] = CP_Image_Load("./Assets/Tiles/Mobs/Explode/ExplodeSlime_Idle2.png");

	//room tiles
	tile_list[1] = CP_Image_Load("./Assets/Tiles/tile_0012.png"); // rock floor
	tile_list[2] = CP_Image_Load("./Assets/Tiles/tile_0065.png"); // grave
	tile_list[3] = CP_Image_Load("./Assets/Tiles/tile_0074.png"); // anvil
	tile_list[4] = CP_Image_Load("./Assets/Tiles/tile_0082.png"); // barrel
	tile_list[5] = CP_Image_Load("./Assets/Tiles/TopWall.png");
	tile_list[6] = CP_Image_Load("./Assets/Tiles/BottomWall.png");
	tile_list[7] = CP_Image_Load("./Assets/Tiles/RightWall.png");
	tile_list[8] = CP_Image_Load("./Assets/Tiles/LeftWall.png");
}

void draw_all(Entity entities, int tile_map[GRID_ROWS][GRID_COLS]) {

}