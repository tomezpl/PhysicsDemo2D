#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

int main()
{
	al_init();

	al_init_primitives_addon();

	ALLEGRO_DISPLAY* window = al_create_display(1280, 720);

	ALLEGRO_EVENT_QUEUE* evQueue = al_create_event_queue();
	al_register_event_source(evQueue, al_get_display_event_source(window));
	
	ALLEGRO_EVENT* ev = new ALLEGRO_EVENT();

	bool isRunning = true;
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

		al_draw_filled_circle(640, 360, 60, al_map_rgb(255, 0, 0));

		al_flip_display();
	}
}