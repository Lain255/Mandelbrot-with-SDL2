#include <SDL2/SDL.h>

float threshold = 4;

int mandelBrot(double a, double b, int n)
{
    double x = 0;
    double y = 0;
    double temp;

    for (int i = 1; i < n; i++) {
        temp = x;
        x = x*x - y*y + a;
        y = 2 * temp * y + b + i/n;
        if (threshold < x*x + y*y)
            return i;
    }
    return n;
}
    
int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_VIDEO);

    const int w = 800;
    const int h = 600;

    SDL_Window *window = SDL_CreateWindow(
        "Mandelbrot", 
        SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED, 
        w, 
        h,
        0
    );
    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, w, h);

    SDL_bool quit = SDL_FALSE;
    SDL_Event e;
    double Cx = 0;
    double Cy = 0;
    double zoom = 0.005;
    int maxIter = 20;
    unsigned char* pixels;
    int pitch;
    int binaryView = 1;
    int turbo = 1;
    
    const Uint8* keyboard = SDL_GetKeyboardState(NULL);

    while (!quit) 
    {
        while (SDL_PollEvent(&e)) {
            switch(e.type) {
            case SDL_QUIT:
                quit = SDL_TRUE;    
                break;
            case SDL_KEYDOWN:       
                if (e.key.keysym.sym == SDLK_t)
                    binaryView = !binaryView;
                break;
            }
        }
        quit = quit || keyboard[SDL_SCANCODE_ESCAPE];
        turbo = 1 + 4*keyboard[SDL_SCANCODE_LSHIFT];
        Cx += 2*turbo*zoom*(keyboard[SDL_SCANCODE_D] - keyboard[SDL_SCANCODE_A]);    
        Cy += 2*turbo*zoom*(keyboard[SDL_SCANCODE_S] - keyboard[SDL_SCANCODE_W]);
        zoom *= 1 - 0.05*turbo*(keyboard[SDL_SCANCODE_EQUALS] - keyboard[SDL_SCANCODE_MINUS]);
        maxIter += turbo*(keyboard[SDL_SCANCODE_E] - keyboard[SDL_SCANCODE_Q]);
        maxIter = maxIter > 0 ? maxIter : 1;
        threshold += 0.05*turbo*(keyboard[SDL_SCANCODE_X] - keyboard[SDL_SCANCODE_Z]);
        
        SDL_LockTexture(texture, NULL, (void**) &pixels, &pitch);
        for (int i = 0; i < w*h; i++) {
            
            double x = Cx + zoom * ((i % w) - w/2);
            double y = Cy + zoom * ((i / w) - h/2);

            double numIter = mandelBrot(x, y, maxIter);
            int inSet = numIter == maxIter;
            numIter *= (binaryView || inSet);
            double value = numIter/maxIter;

            int r = 255 * value*value*value;
            int g = 255 * inSet;
            int b = 255 * value;

            pixels[4*i + 0] = b;
            pixels[4*i + 1] = g;
            pixels[4*i + 2] = r;
        }
        SDL_UnlockTexture(texture);

        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);  //closes window
    SDL_Quit();
    return 0;
}