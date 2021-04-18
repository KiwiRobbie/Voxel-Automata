#include <map>
#include <cmath>
#include <glm/glm.hpp>

//Class for containt an animated float that uses linear interpolation 
class AnimatedVec4
{
    std::string vectorName;
    std::map<int, glm::vec4> keyframes;
    
public:

    AnimatedVec4()
    {
    
    }

    AnimatedVec4(std::string n, int end_frame)
    {
        addFrame(0,   glm::vec4( 1, 0, -3, 0));
        addFrame(end_frame/2,  glm::vec4( 0, 0, -3, 0));
        addFrame(end_frame, glm::vec4(-1, 0, -3, 0));
        vectorName = n;
    }

    void addFrame(int t, glm::vec4 v)
    {
        keyframes.insert(std::pair<int, glm::vec4>(t, v));
        keyframes[t] = v;
    }

    void removeFrame(int t)
    {
        keyframes.erase(t);
    }

    glm::vec4 getFrame(int f) 
    {
        int t = getNearest(f);
        std::tuple<int, int, int> corner = getCorner(t);
        glm::vec4 position = getPosition(corner, f);
        return position;
        
    }

    //void displayKeyframes()
    //{
    //    for (std::map<int, float>::iterator it = keyframes.begin(); it != keyframes.end(); it++)
    //        std::cout << it->first << " => " << it->second << '\n';
    //}

    int getNearest(int t)
    {
        std::map<int, glm::vec4>::iterator next_key_itt = keyframes.lower_bound(t);
        std::map<int, glm::vec4>::iterator prev_key_itt = keyframes.lower_bound(t);

        if (t != next_key_itt->first)
        {
            --prev_key_itt;
        }

        int prev_key = prev_key_itt->first;
        int next_key = next_key_itt->first;

        if ((t - prev_key) < (next_key - t))
        {
            return prev_key;
        }
        else
        {
            return next_key;
        }
    }

    std::tuple<int, int, int> getCorner(int t)
    {
        std::map<int, glm::vec4>::iterator next_key_itt = keyframes.lower_bound(t);
        std::map<int, glm::vec4>::iterator prev_key_itt = keyframes.lower_bound(t);

        if ( prev_key_itt != keyframes.begin() ) { prev_key_itt--; }
        
        if ( next_key_itt != --keyframes.end()   ) { next_key_itt++; }
        
        int prev_key = prev_key_itt->first;
        int next_key = next_key_itt->first;
        return std::tuple<int, int, int>(prev_key, t, next_key);
    }

    glm::vec4 getPosition(std::tuple<int, int, int> corner, int f)
    {
        glm::vec4 p1 = (keyframes[std::get<1>(corner)] + keyframes[std::get<0>(corner)])*0.5f;
        glm::vec4 p2 = (keyframes[std::get<1>(corner)] + keyframes[std::get<2>(corner)])*0.5f;

        float k1 = (float)(std::get<1>(corner) + std::get<0>(corner)) * 0.5f;
        float k2 = (float)(std::get<1>(corner) + std::get<2>(corner)) * 0.5f;

        float t = (float)(f - k1)/(k2-k1);
        glm::vec4 l1 = lerp( p1, keyframes[std::get<1>(corner)], t);
        glm::vec4 l2 = lerp( keyframes[std::get<1>(corner)], p2, t);

        return lerp(l1, l2, t);
    }

    glm::vec4 lerp(glm::vec4 x, glm::vec4 y, float t) 
    {
        return x * (1.f - t) + y * t;
    }

    glm::vec4 update(bool playback, int t, int lt, glm::vec4 input)
    {

        ImGui::Button(vectorName.data());
        ImGui::SameLine();

        if (ImGui::Button((std::string("+##") + vectorName).data()))
        {
            addFrame(t, input);
        }
        if (keyframes.count(t))
        {
            ImGui::SameLine();
            if (ImGui::Button((std::string("-##") + vectorName).data()))
            {
                removeFrame(t);
            }
        }

        float cX = input.x;
        float cY = input.y;
        float cZ = input.z;

        //Label and slider for x
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0, 1.0, 1.0, 1.0));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
        ImGui::Button(" X ");
        ImGui::PopStyleColor(1);
        ImGui::SameLine();
        ImGui::DragFloat((std::string("##X") + vectorName).data(), &cX, -1, 1);

        //Label and slider for y
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2, 0.7, 0.2, 1.0));
        ImGui::Button(" Y ");
        ImGui::PopStyleColor(1);
        ImGui::SameLine();
        ImGui::DragFloat((std::string("##Y") + vectorName).data(), &cY, -1, 1);

        //Label and slider for z
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1, 0.25, 0.8, 1.0));
        ImGui::Button(" Z ");
        ImGui::PopStyleColor(1);
        ImGui::SameLine();
        ImGui::DragFloat((std::string("##Z") + vectorName).data(), &cZ, -1, 1);
        ImGui::PopStyleColor(1);

        ImGui::NewLine();
        
        if(t != lt && playback)
        {
            return getFrame(t);
        }
        else 
        {
            return input;
        }

    }

};

