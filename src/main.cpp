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

typedef Vec2 Point2d;

class Rect
{
public:
	// Vertical coordinates for top and bottom edges, and horizontal coordinates for left and right edges.
	float top, left, bottom, right;

	Rect(float Top, float Left, float Bottom, float Right) { top = Top; left = Left; bottom = Bottom; right = Right; }
	Rect(Point2d topLeft = Point2d(), Point2d bottomRight = Point2d()) : Rect(topLeft.y, topLeft.x, bottomRight.y, bottomRight.x) { }

	// Is a Point inside the rectangle's bounds?
	bool inBounds(const Point2d& point)
	{
		return point.y >= top && point.x >= left && point.y <= bottom && point.x <= right;
	}

	inline Point2d topLeft() { return Point2d(left, top); }
	inline Point2d topRight() { return Point2d(right, top); }
	inline Point2d bottomLeft() { return Point2d(left, bottom); }
	inline Point2d bottomRight() { return Point2d(right, bottom); }

	// Is a Rect inside this Rect's bounds?
	bool inBounds(const Rect& rect)
	{
		return inBounds(topLeft()) || inBounds(topRight()) || inBounds(bottomLeft()) || inBounds(bottomRight());
	}
};

class RectCollider
{
public:
	Rect bounds;
	RectCollider(Rect Bounds = Rect()) { bounds = Bounds; }
};

class RBDynamic 
{
public:
	RBDynamic(Point2d Position = Point2d(), float Mass = 1.f) 
	{
		position = Position;
		mass = Mass;
		velocity = Vec2();
	}

	// Centre of mass position
	Point2d position;
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

	RBDynamic box = RBDynamic(Point2d(640.f, 360.f));
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