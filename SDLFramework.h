#pragma once
#include <SDL.h>
#include <cmath>
#include <vector>
#include <array>

class SDLFramework {
public:
    // Contructor which initialize the parameters.
    SDLFramework(int width, int height) : m_width(width),m_height(height) {
        SDL_Init(SDL_INIT_VIDEO);       // Initializing SDL as Video
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
        SDL_CreateWindowAndRenderer(width, height, 0, &m_window, &m_renderer);
        SDL_SetRenderDrawColor(m_renderer, 255, 255,255, 0);      // setting draw color
        SDL_RenderClear(m_renderer);      // Clear the newly created m_window
        SDL_RenderPresent(m_renderer);    // Reflects the changes done in the
    }
    ~SDLFramework() {
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
    }
    void draw_point(float x, float y)
    {
        SDL_SetRenderDrawColor(m_renderer, 65, 100, 31, 255);
        SDL_RenderDrawPointF(m_renderer, -x+xcenter, -y+ycenter);
        SDL_RenderPresent(m_renderer);
    }
    void draw_rectangleF(const SDL_FRect* rect)
    {
        SDL_SetRenderDrawColor(m_renderer, 65, 100, 31, 255);
        SDL_RenderDrawRectF(m_renderer, rect);
        SDL_RenderPresent(m_renderer);
    }
    void draw_lineF(std::array<Uint8,4> color, float x1, float y1, float x2, float y2) {
        SDL_SetRenderDrawColor(m_renderer, color[0], color[1], color[2], color[3]);
        SDL_RenderDrawLineF(m_renderer, x1+ xcenter, -y1+ycenter, x2+ xcenter, -y2+ycenter);
        SDL_RenderPresent(m_renderer);
    }
    void draw_circle(std::array<Uint8, 4> color,int center_x, int center_y, int radius_) {
        SDL_SetRenderDrawColor(m_renderer, color[0],color[1],color[2],color[3]);
        for (int x = center_x - radius_; x <= center_x + radius_; x++) {
            for (int y = center_y - radius_; y <= center_y + radius_; y++) {
                if ((std::pow(center_y - y, 2) + std::pow(center_x - x, 2)) <=
                    std::pow(radius_, 2)) {
                    SDL_RenderDrawPoint(m_renderer, x+xcenter, -y+ycenter);
                }
            }
        }
        SDL_RenderPresent(m_renderer);
    }
    void draw_circle(int center_x, int center_y, int radius_) {
        for (int x = center_x - radius_; x <= center_x + radius_; x++) {
            for (int y = center_y - radius_; y <= center_y + radius_; y++) {
                if ((std::pow(center_y - y, 2) + std::pow(center_x - x, 2)) <=
                    std::pow(radius_, 2)) {
                    SDL_RenderDrawPoint(m_renderer, x + xcenter, y + ycenter);
                }
            }
        }
        SDL_RenderPresent(m_renderer);
    }
    void move_circle() {
        // Setting the color to be RED with 100% opaque (0% trasparent).

        SDL_Event event;    // Event variable
        while (!(event.type == SDL_QUIT)) {

            // Circle will go down!
            for (int i = 0; i < m_height; i++) {
                SDL_Delay(10);  // setting some Delay
                SDL_PollEvent(&event);  // Catching the poll event.
                if (event.type == SDL_QUIT) return;
                SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
                SDL_RenderClear(m_renderer);
                draw_circle(m_width / 2, i, 25);
            }

            // Circle will go up!
            for (int i = m_height; i > 0; i--) {
                SDL_Delay(10);  // setting some Delay
                SDL_PollEvent(&event);  // Catching the poll event.
                if (event.type == SDL_QUIT) return;
                SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
                SDL_RenderClear(m_renderer);
                draw_circle(m_width / 2, i, 25);
            }
        }

    }

private:
    int m_height;     
    int m_width;      
    int xcenter = m_width / 2;
    int ycenter = m_height / 2;
    SDL_Renderer* m_renderer = NULL;     
    SDL_Window* m_window = NULL;      
};

