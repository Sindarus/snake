
int detect(snake* s, direction c, field* map){
	int a=s->body[s->head].x;
	int b=s->body[s->head].y;
	if (c == UP) {
		return (map->f[a-1][b] == EMPTY);
	} else if (c == LEFT) {
		return (map->f[a][b-1] == EMPTY);
	} else if (c == RIGHT) {
		return (map->f[a][b+1] == EMPTY);
	} else if (c == DOWN) {
		return (map->f[a+1][b] == EMPTY);
	}
	else{return 1;}

}

void rngesus(snake* shlanga, field* map){
	int r;
	do {
		r = 1 + rand() % 4;
	} while (!(r == 1 && shlanga->dir != DOWN) &&
		!(r == 2 && shlanga->dir != RIGHT) &&
		!(r == 3 && shlanga->dir != LEFT) &&
		!(r == 4 && shlanga->dir != UP));
	if (r == 1 && shlanga->dir != DOWN) {
		if (move(shlanga, map, C_UP) < 0) {
			game_over(map, shlanga, M_WIN);
		}
	} else if (r == 2 && shlanga->dir != RIGHT) {
		if (move(shlanga, map, C_LEFT) < 0) {
			game_over(map, shlanga, M_WIN);
		}
	} else if (r == 3 && shlanga->dir != LEFT) {
		if (move(shlanga, map, C_RIGHT) < 0) {
			game_over(map, shlanga, M_WIN);
		}
	} else if (r == 4 && shlanga->dir != UP) {
		if (move(shlanga, map, C_DOWN) < 0) {
			game_over(map, shlanga, M_WIN);
		}
	}
}

void rngesus2(snake* shlanga, field* map){
	int r;
	do {
		r = 1 + rand() % 4;
	} while (!(r == 1 && shlanga->dir != DOWN) &&
		!(r == 2 && shlanga->dir != RIGHT) &&
		!(r == 3 && shlanga->dir != LEFT) &&
		!(r == 4 && shlanga->dir != UP));

	if (r == 1 && shlanga->dir != DOWN && detect(shlanga,UP,map)) {
		if (move(shlanga, map, C_UP) < 0) {
			game_over(map, shlanga, M_WIN);
		}
	} else if (r == 2 && shlanga->dir != RIGHT && detect(shlanga,RIGHT,map)) {
		if (move(shlanga, map, C_LEFT) < 0) {
			game_over(map, shlanga, M_WIN);
		}
	} else if (r == 3 && shlanga->dir != LEFT && detect(shlanga,LEFT,map)) {
		if (move(shlanga, map, C_RIGHT) < 0) {
			game_over(map, shlanga, M_WIN);
		}
	} else if (r == 4 && shlanga->dir != UP && detect(shlanga,DOWN,map)) {
		if (move(shlanga, map, C_DOWN) < 0) {
			game_over(map, shlanga, M_WIN);
		}
	}
}
