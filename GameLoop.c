#ifndef GAMELOOP_H
#define GAMELOOP_H

#include "./Typedefs.h"
#include "FuncDefs.h"
#include "InitFuncs.h"

#include <stdlib.h>

void GameLoop(GameObject* gameObject) {
    al_start_timer(gameObject->timer);

    while (1)
    {
        al_wait_for_event(gameObject->eventQueue, gameObject->event);

        al_get_keyboard_state(gameObject->keyboardState);

        switch (gameObject->event->type)
        {
            case ALLEGRO_EVENT_TIMER:
                if (gameObject->key[ALLEGRO_KEY_ENTER])
                    if (!gameObject->gameRunning)
                        gameObject->gameRunning = true;

                if (!gameObject->gameRunning)
                    break;

                if (gameObject->key[ALLEGRO_KEY_LEFT])
                    if ((gameObject->rect->x1 - gameObject->rect->dx) > 0) {
                        gameObject->rect->x1 -= gameObject->rect->dx;
                        gameObject->rect->x2 -= gameObject->rect->dx;
                    }
                if (gameObject->key[ALLEGRO_KEY_RIGHT])
                    if ((gameObject->rect->x2 + gameObject->rect->dx) < 800) {
                        gameObject->rect->x1 += gameObject->rect->dx;
                        gameObject->rect->x2 += gameObject->rect->dx;
                    }

                if (gameObject->ball->isActive)
                    moveTheBall(gameObject);

                if (gameObject->key[ALLEGRO_KEY_ESCAPE])
                    gameObject->done = true;

                for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
                    gameObject->key[i] &= KEY_SEEN;

                gameObject->redraw = true;
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                gameObject->key[gameObject->event->keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
                break;
            case ALLEGRO_EVENT_KEY_UP:
                gameObject->key[gameObject->event->keyboard.keycode] &= KEY_RELEASED;
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                gameObject->done = true;
                break;
        } // end switch

        if (gameObject->done)
            break;

        if (gameObject->gameRunning) {
            if (gameObject->redraw && al_is_event_queue_empty(gameObject->eventQueue)) {

                char levelString[10];
                getLevelString(levelString, gameObject->currentLevel);

                char liveString[10];
                getLivesString(liveString, gameObject->ball->lives);

                char scoreString[12];
                getScoreString(scoreString, gameObject->score);

                gameObject->ball->isActive = 1;

                if (gameObject->ball->lives == 0) {
                    al_rest(2);

                    al_clear_to_color(al_map_rgb(0, 0, 0));
                    al_draw_text(gameObject->font, al_map_rgb(255, 255, 255),
                        SCREEN_WIDTH / 2 - 25, SCREEN_HEIGHT / 2, 0, "Game over.");
                    al_flip_display();
                    al_rest(5);
                    gameObject->done = true;
                    gameObject->gameRunning = false;
                }
                else {
                    al_clear_to_color(al_map_rgb(0, 0, 0));
                    drawBlocks(gameObject);
                    al_draw_filled_rectangle(gameObject->rect->x1, gameObject->rect->y1, gameObject->rect->x2, gameObject->rect->y2, WHITE_COLOR);
                    al_draw_filled_circle(gameObject->ball->cx, gameObject->ball->cy, gameObject->ball->radius, WHITE_COLOR);
                    al_draw_text(gameObject->font, WHITE_COLOR, 0, 0, 0, levelString);
                    al_draw_text(gameObject->font, WHITE_COLOR, 0, 10, 0, liveString);
                    al_draw_text(gameObject->font, WHITE_COLOR, 0, 20, 0, scoreString);
                    al_flip_display();

                    if (gameObject->levels->levelCleared) {
                        al_clear_to_color(al_map_rgb(0, 0, 0));
                        al_draw_text(gameObject->font, al_map_rgb(255, 255, 255),
                            SCREEN_WIDTH / 2 - 25, SCREEN_HEIGHT / 2, 0, "Level cleared.");
                        al_flip_display();
                        al_rest(2);
                        initLevel(gameObject);
                    }

                }
                gameObject->redraw = false;
            }
        }
        else { // game not running
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_text(gameObject->font, al_map_rgb(255, 255, 255),
                SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2, 0, "Press enter to start.");
            al_flip_display();
        }
    }
}

#endif // !GAMELOOP_H
