#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include <cmath>

#define GRAVITY 9.81f

class Vec2 
{
public:
	float x, y;
	Vec2(float X = .0f, float Y = .0f) { x = X; y = Y; }

	Vec2 operator+(Vec2 rh) { return Vec2(x + rh.x, y + rh.y); }
	Vec2 operator-() { return Vec2(-x, -y); }
	Vec2 operator-(Vec2 rh) { return Vec2(x - rh.x, y - rh.y); }
	Vec2 operator/(float rh) { return rh == .0f ? Vec2() : Vec2(x / rh, y / rh); }
	Vec2 operator*(float rh) { return Vec2(x * rh, y * rh); }
	void operator+=(Vec2 rh) { *this = *this + rh; }
	void operator-=(Vec2 rh) { *this += -rh; }
	void operator*=(float rh) { *this = *this * rh; }
	void operator/=(float rh) { *this = *this / rh; }

	float magnitude() { return sqrtf(x * x + y * y); }
	Vec2 normalised() { return *this / magnitude(); }
};

class RBDynamic 
{
public:
	RBDynamic(Vec2 Position = Vec2(), float Mass = 1.f) 
	{
		position = Position;
		mass = Mass;
		velocity = Vec2();
	}

	// Centre of mass position
	Vec2 position;
	float mass;
	Vec2 velocity;

	void fall(float time) 
	{
		velocity += Vec2(.0f, GRAVITY) * mass;

		position += velocity * time;
	}
};

int main()
{
	al_init();

	al_init_primitives_addon();

	ALLEGRO_DISPLAY* window = al_create_display(1280, 720);

	ALLEGRO_EVENT_QUEUE* evQueue = al_create_event_queue();
	al_register_event_source(evQueue, al_get_display_event_source(window));
	
	ALLEGRO_EVENT* ev = new ALLEGRO_EVENT();

	RBDynamic box = RBDynamic(Vec2(640.f, 360.f));
	float boxSize = 60.f;
	float boxHx = boxSize / 2.f; // Box half-extents

	bool isRunning = true;
	float frameStartTime = al_get_time();
	while (isRunning)
	{
		while (al_get_next_event(evQueue, ev))
		{
			if (ev && ev->type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			{
				isRunning = false;
			}
		}

		al_flush_event_queue(evQueue);

		al_clear_to_color(al_map_rgb(100, 149, 237));

		// I'M GONNA FUCKING FALL
		box.fall(al_get_time() - frameStartTime);
		al_draw_filled_rectangle(box.position.x - boxHx, box.position.y - boxHx, box.position.x + boxHx, box.position.y + boxHx, al_map_rgb(255, 0, 0));

		al_flip_display();

		frameStartTime = al_get_time();
	}
}