#include <SFML/Graphics.hpp>
#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"
#include <iostream>
using namespace sf;
using namespace std;

Vector2f win_s = { 512+512,512+128 };
int canvas_size = 24;
int curr_canvas_size = 0;
int Cell_S = 21.333333;
Uint8 bg_color[4] = { 255,255,255,255 };
Uint8 pixel_color_main[4] = { 0,0,0,1 };
Uint8 pixel_color_erace[4] = { 1,1,1,1 };
float pixel_color_main_t[4] = { 0,0,0,1 };
float pixel_color_erace_t[4] = { 1,1,1,1 };

class Pixel
{
public:
    Pixel()
    {
        this->pixel.setPosition(0, 0);
        this->pixel.setSize(Vector2f{ float(Cell_S),float(Cell_S) });
        this->pixel.setFillColor({bg_color[0],bg_color[1], bg_color[2], bg_color[3],});
    }
    RectangleShape& get_pixel()
    {
        return pixel;
    }
    RectangleShape operator= (const RectangleShape& pix)
    {
        this->pixel.setFillColor(pix.getFillColor());
        this->pixel.setPosition(pix.getPosition());
        return pixel;
    }
private:
    RectangleShape pixel;
};
bool t1 = false;
bool t2 = false;
void UI()
{
    
    ImGui::SetNextWindowBgAlpha(0);
    ImGui::SetNextWindowPos(ImVec2(0, 512));
    ImGui::SetNextWindowSize(ImVec2(512, 128));
    ImGui::Begin("bottom window",NULL,7);
    ImGui::Checkbox("1", &t1);
    ImGui::Checkbox("2", &t2);
        ImGui::InputInt("Canvas Size", &canvas_size);
        if (canvas_size > curr_canvas_size) canvas_size = curr_canvas_size;
    ImGui::End();

    ImGui::SetNextWindowBgAlpha(0);
    ImGui::SetNextWindowPos(ImVec2(512, 0));
    ImGui::SetNextWindowSize(ImVec2(512, 512+128));
    ImGui::Begin("right window",NULL,7);
        ImGui::SetNextItemWidth(184);
        ImGui::ColorPicker4("Color Picker", pixel_color_main_t);
        
        ImGui::SetNextItemWidth(256);
        ImGui::BeginListBox(" ");
            ImGui::ColorEdit4("main color", pixel_color_main_t, 4);
            ImGui::ColorEdit4("erase color", pixel_color_erace_t, 0);
        ImGui::EndListBox();

    ImGui::End();

    //cout << int(pixel_color_main[0]) << ' ' << int(pixel_color_main[1]) << ' ' << int(pixel_color_main[2]) << ' ' << int(pixel_color_main[3]) << endl;

    pixel_color_main[0] = int(pixel_color_main_t[0] * 255);
    pixel_color_main[1] = int(pixel_color_main_t[1] * 255);
    pixel_color_main[2] = int(pixel_color_main_t[2] * 255);
    pixel_color_main[3] = int(pixel_color_main_t[3] * 255);
    
    pixel_color_erace[0] = int(pixel_color_erace_t[0] * 255);
    pixel_color_erace[1] = int(pixel_color_erace_t[1] * 255);
    pixel_color_erace[2] = int(pixel_color_erace_t[2] * 255);
    pixel_color_erace[3] = int(pixel_color_erace_t[3] * 255);

}
void hello_window()
{
    RenderWindow window(VideoMode(256,256), "Let's start");
    ImGui::SFML::Init(window);
    window.setFramerateLimit(60);

    Clock deltaClock;
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == Event::Closed)
                window.close();
        }
        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::SetNextWindowBgAlpha(0);
        ImGui::SetNextWindowPos(ImVec2(0,0));
        ImGui::SetNextWindowSize(ImVec2(256, 256));
        ImGui::Begin(" ",NULL,7);
        ImGui::InputInt("Canvas Size", &canvas_size);
        curr_canvas_size = canvas_size;
        Cell_S = 512 / canvas_size;
        if(ImGui::Button("Start"))
            window.close();
        ImGui::End();

        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }
    ImGui::SFML::Shutdown();

}
int main()
{
    hello_window();

    RenderWindow window(VideoMode(win_s.x, win_s.y), "Paint v3");
    ImGui::SFML::Init(window);
    window.setFramerateLimit(60);
    vector<vector<Pixel>> canvas;
    vector<Pixel> canvas_1;

    for (int i = 0; i < canvas_size; i++)
    {
        for (int j = 0; j < canvas_size; j++)
        {
            Pixel pix;
            canvas_1.push_back(pix);
        }
        canvas.push_back(canvas_1);
    }
    Clock deltaClock;
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == Event::Closed)
                window.close();
        }
        ImGui::SFML::Update(window, deltaClock.restart());
        Vector2i m_pos = Mouse::getPosition(window);
        
        UI();

        window.clear();

        for (int i = 0; i < canvas_size; i++)
        {
            for (int j = 0; j < canvas_size; j++)
            {
                /*if (m_pos.x >= j * Cell_S && m_pos.y >= i * Cell_S &&
                    m_pos.x < j*Cell_S+Cell_S&& m_pos.y < i*Cell_S+Cell_S)
                    canvas[j][i].get_pixel().setFillColor(Color::Black);
                else
                    canvas[j][i].get_pixel().setFillColor(Color::White);
                */
                //cout << ImGui::IsAnyItemFocused() << endl;
                if (int(m_pos.y / Cell_S) < canvas_size && int(m_pos.x / Cell_S) < canvas_size &&
                    int(m_pos.y / Cell_S) >= 0 && int(m_pos.x / Cell_S) >= 0 &&
                    !ImGui::IsAnyItemActive() &&
                    !ImGui::IsAnyItemHovered()
                    )
                {
                    if (Mouse::isButtonPressed(Mouse::Left))
                    {
                        canvas[int(m_pos.x / Cell_S)][int(m_pos.y / Cell_S)].get_pixel().setFillColor({ pixel_color_main[0], pixel_color_main[1], pixel_color_main[2], pixel_color_main[3]});
                    }
                    if (Mouse::isButtonPressed(Mouse::Right))
                    {
                        canvas[int(m_pos.x / Cell_S)][int(m_pos.y / Cell_S)].get_pixel().setFillColor({ pixel_color_erace[0],pixel_color_erace[1],pixel_color_erace[2],pixel_color_erace[3] });
                    }
                }

                canvas[j][i].get_pixel().setPosition(j * Cell_S, i * Cell_S);
                window.draw(canvas[j][i].get_pixel());
                
            }
        }
        ImGui::SFML::Render(window);
        window.display();
    }
    ImGui::SFML::Shutdown();
    return 0;
}