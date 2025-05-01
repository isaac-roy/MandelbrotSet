#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include "ComplexPlane.h"
#include <complex>
#include <cmath>

using namespace std;
using namespace sf;

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight)         // Eric
{
    m_vArray.setPrimitiveType(Points);
    m_vArray.resize(pixelWidth * pixelHeight);
    m_state = State::CALCULATING;
    m_mouseLocation = {0.f, 0.f};
    m_pixel_size = { pixelWidth, pixelHeight };
    m_plane_center = {0, 0};
    m_plane_size = { BASE_WIDTH, BASE_HEIGHT * m_aspectRatio };
    m_zoom_count = 0;
    m_aspectRatio = static_cast<float>(pixelHeight) / static_cast<float>(pixelWidth);
}

void ComplexPlane::draw(RenderTarget& target, RenderStates states) const            // Eric
{
    target.draw(m_vArray);
}

void ComplexPlane::updateRender()                       // Eric
{
    if(m_state == State::CALCULATING)
    {
        for(int j = 0; j < m_pixel_size.x; j++)
        {
            for(int i = 0; i < m_pixel_size.y; i++)
            {
                m_vArray[j + i * m_pixel_size.x].position = { (float)j, (float)i };
                Vector2f coord = ComplexPlane::mapPixelToCoords({ j, i });
                size_t numIterations = ComplexPlane::countIterations(coord);
                Uint8 r, g, b;
                ComplexPlane::iterationsToRGB(numIterations, r, g, b);
                m_vArray[j + i * m_pixel_size.x].color = { r,g,b };
            }
        }
        m_state = State::DISPLAYING;
    }
}

void ComplexPlane::zoomIn()             // Isaac
{
    m_zoom_count++;
    
    float xSize = BASE_WIDTH * pow(BASE_ZOOM, m_zoom_count);
    float ySize = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoom_count);

    m_plane_size = { xSize, ySize };

    m_state = State::CALCULATING;
}

void ComplexPlane::zoomOut()            // Isaac
{
    m_zoom_count--;
    
    float xSize = BASE_WIDTH * pow(BASE_ZOOM, m_zoom_count);
    float ySize = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoom_count);

    m_plane_size = { xSize, ySize };

    m_state = State::CALCULATING;
}

void ComplexPlane::setCenter(Vector2i mousePixel)           // Isaac
{
    m_plane_center = ComplexPlane::mapPixelToCoords(mousePixel);
    m_state = State::CALCULATING;
}

void ComplexPlane::setMouseLocation(Vector2i mousePixel)            // Isaac
{
    m_mouseLocation = ComplexPlane::mapPixelToCoords(mousePixel);
}

void ComplexPlane::loadText(Text& text)                     // Isaac
{
    stringstream ss;

    ss << "Mandelbrot Set\n";
    ss << "Cursor: (" << m_mouseLocation.x << ", " << m_mouseLocation.y << ")\n";
    ss << "Center: (" << m_plane_center.x << ", " << m_plane_center.y << ")\n";
    ss << "Left-click to Zoom in\n";
    ss << "Right-click to Zoom out\n";

    text.setString(ss.str());
}

size_t ComplexPlane::countIterations(Vector2f coord)            // Isaac
{
    // c = x + yi in the complex plane
    complex<float> c(coord.x, coord.y);

    // Start with z = 0
    complex<float> z = 0;

    // Count how many iterations it takes for |z| to exceed 2
    size_t iterations = 0;
    while (abs(z) < 2.0f && iterations < MAX_ITER)
    {
        z = z * z + c;
        iterations++;
    }

    return iterations;
}

void ComplexPlane::iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b)          // Eric
{
    // We're doing 8 colors since 8 x 8 = 64 and NUM_ITERATIONS = 64
    size_t regionSize = MAX_ITER / 8;

    if(count >= MAX_ITER)
    {
        // iteration 64 inclusive and above
        // hex: #000000
        r = 0;
        g = 0;
        b = 0;
    }
    else if(count < regionSize)
    {
        // iteration less than 8
        // hex: #0d0630
        r = 13;
        g = 6;
        b = 48;
    }
    else if(count < 2 * regionSize)
    {
        // iteration between 8 inclusive and 16 non-inclusive
        // hex: #18314f
        r = 24;
        g = 49;
        b = 79;
    }
    else if(count < 3 * regionSize)
    {
        // iteration between 16 inclusive and 24 non-inclusive
        // hex: #384e77
        r = 56;
        g = 78;
        b = 119;
    }
    else if(count < 4 * regionSize)
    {
        // iteration between 24 inclusive and 32 non-inclusive
        // hex: #8bbeb2
        r = 139;
        g = 190;
        b = 178;
    }
    else if(count < 5 * regionSize)
    {
        // iteration between 32 inclusive and 40 non-inclusive
        // hex: #e6f9af
        r = 230;
        g = 249;
        b = 175;
    }
    else if(count < 6 * regionSize)
    {
        // iteration between 40 inclusive and 48 non-inclusive
        // hex: #dc493a
        r = 220;
        g = 73;
        b = 58;
    }
    else if(count < 7 * regionSize)
    {
        // iteration between 48 inclusive and 56 non-inclusive
        // hex: #62466B
        r = 98;
        g = 70;
        b = 107;
    }
    else if(count <= 8 * regionSize)
    {
        // iteration between 56 inclusive and 64 non-inclusive
        // hex: #FCA17D
        r = 252;
        g = 162;
        b = 125;
    }
}

Vector2f ComplexPlane::mapPixelToCoords(Vector2i mousePixel)                // Eric
{
    Vector2f coords;

    float minX = m_plane_center.x - m_plane_size.x / 2.0f;
    float minY = m_plane_center.y - m_plane_size.y / 2.0f;

    coords.x = (static_cast<float>(mousePixel.x) / m_pixel_size.x) * m_plane_size.x + minX;
    coords.y = (static_cast<float>(mousePixel.y) / m_pixel_size.y) * m_plane_size.y + minY;
    
    return coords;
}
