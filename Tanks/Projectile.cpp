#include "Projectile.h"

Projectile::Projectile(Direction direction){
	d = direction;

	move_interval = std::chrono::milliseconds(50);
	last_move_time = std::chrono::steady_clock::now();
}

void Projectile::move() {
	auto now = std::chrono::steady_clock::now();
	if (now - last_move_time >= move_interval) {
		update_coordinates();
		last_move_time = now;
	}
}