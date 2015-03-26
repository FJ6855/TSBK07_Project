#ifndef RENDERER_H
#define RENDERER_H


class Renderer
{
public:
    Renderer();
    ~Renderer();

    void render();
    

private:
    void _initSystems();

    int _screenWidth;
    int _screenHeight;

};

#endif
