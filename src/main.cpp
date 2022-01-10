#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <string>
#include <iostream>

#include "rigidbody.h"
#include "springconstraint.h"

int main()
{
	al_init();

	al_init_primitives_addon();

	ALLEGRO_DISPLAY* window = al_create_display(1280, 720);

	ALLEGRO_EVENT_QUEUE* evQueue = al_create_event_queue();
	al_register_event_source(evQueue, al_get_display_event_source(window));
	
	ALLEGRO_EVENT* ev = new ALLEGRO_EVENT();

	RBDynamic box = RBDynamic(Point2d(640.f, 360.f), 1000.f, false, new RectCollider(Rect(0.f, 0.f, 60.f, 60.f)));
	RBDynamic springRoot = RBDynamic(Point2d(640.f, 240.f), 1.f, true, new RectCollider(Rect(0.f, 0.f, 60.f, 60.f)));

	SpringConstraint spring = SpringConstraint({ &springRoot, &box }, 300.f, 150.f);

	float boxSize = 60.f;
	float boxHx = boxSize / 2.f; // Box half-extents

	RectCollider platform = RectCollider(Rect(600, 180, 660, 1100));

	bool isRunning = true;
	float frameStartTime = al_get_time();

	float runningTime = 0.f;

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

		float deltaTime = al_get_time() - frameStartTime;
		runningTime += deltaTime;

		box.step(deltaTime, nullptr);
		springRoot.step(deltaTime, nullptr);
		spring.step(deltaTime);
		ALLEGRO_COLOR boxColour = box.overlap ? al_map_rgb(255, 0, 0) : al_map_rgb(255, 128, 32);
		RectCollider* boxRectCollider = reinterpret_cast<RectCollider*>(box.collider);
		Rect boxRect = boxRectCollider->bounds, box2Rect = reinterpret_cast<RectCollider*>(springRoot.collider)->bounds;
		al_draw_filled_rectangle(boxRect.left, boxRect.top, boxRect.right, boxRect.bottom, boxColour);
		al_draw_filled_rectangle(box2Rect.left, box2Rect.top, box2Rect.right, box2Rect.bottom, boxColour);
		if (runningTime < 15.f) 
		{
			//al_draw_filled_rectangle(platform.bounds.left, platform.bounds.top, platform.bounds.right, platform.bounds.bottom, al_map_rgb(0, 255, 0));
		}

		al_flip_display();

		frameStartTime = runningTime;
	}

	al_shutdown_primitives_addon();
	al_destroy_display(window);

	return 0;
}