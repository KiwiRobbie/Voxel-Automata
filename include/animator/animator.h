#include <iostream>
#include <cmath>
#include <map>


//Class for containt an animated float that uses linear interpolation 
class Animated
{
    std::map<int, float> keyframes;

public:
    void addFrame(int t, float v)
    {
        keyframes.insert(std::pair<int, float>(t, v));
        keyframes[t] = v;
    }

    void removeFrame(int t)
    {
        keyframes.erase(t);
    }

    void displayKeyframes()
    {
        for (std::map<int, float>::iterator it = keyframes.begin(); it != keyframes.end(); it++)
            std::cout << it->first << " => " << it->second << '\n';
    }

    float getFrame(int t)
    {   
        std::map<int, float>::iterator next_key_itt = keyframes.lower_bound(t);
        std::map<int, float>::iterator prev_key_itt = keyframes.lower_bound(t);

        if ( t != next_key_itt->first)
        {
            --prev_key_itt;
        }

        int prev_key = prev_key_itt->first;
        int next_key = next_key_itt->first;

        float prev_val = prev_key_itt->second;
        float next_val = next_key_itt->second;

        float D = (float)(next_key - prev_key);
        float d = (float)(t - prev_key) + !D;
        D += !D;
        return prev_val + d*(next_val-prev_val)/D;
    } 
    
    bool contains(int t)
    {
        return keyframes.count(t);
    }

    int size()
    {
        return keyframes.size();
    }
};

class Property
{
    Animated keys;
    float value;
    bool keyed = false;
    int end;
    std::string name;
    std::string index;
    unsigned int shaderID;
    float min;
    float max;
public:
    Property()
    {
    
    } 

    Property(const char* n, float d, unsigned int id, const char* i, int e, float mi, float ma)
    {
        name = n;
        value = d;
        shaderID = id;
        index = i;
        end = e;
        min = mi;
        max = ma;
    }

    void update(int t, int lt)
    {
        if (keyed && t != lt) { value = keys.getFrame(t); }

        if (keyed) { ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.8, 0.48, 0.1, 1.0)); }
        ImGui::SliderFloat(name.data(), &value, min, max);
        
        if (keyed) { ImGui::PopStyleColor(1); }
        
        
        
        if (keys.contains(t))
        {
            ImGui::SameLine();
            std::cout << value << ", " << keys.getFrame(t) << std::endl;
            if (value != keys.getFrame(t))
            {
                if (ImGui::Button((std::string("+##") + index).data()))
                {
                    keys.addFrame(t, value);
                }
            }
            else{
                if (ImGui::Button((std::string("-##") + index).data()))
                {
                    keys.removeFrame(t);
                }
            }
        }
        else
        {
            ImGui::SameLine();
            if (ImGui::Button((std::string("+##") + index).data()))
            {
                keys.addFrame(t, value);
                if (!keyed)
                {
                    keys.addFrame(0, value);
                    keys.addFrame(end, value);
                }
                keyed = true;
            }
            
        }
        glUniform1f(glGetUniformLocation(shaderID, index.data()), value);
    }

    void render(int t)
    {
        if (keyed)
        {
            value = keys.getFrame(t);
        }
        
        glUniform1f(glGetUniformLocation(shaderID, index.data()), value);
    }
};

class Scene
{
    std::map < std::string, Property > properties;

public:
    Scene()
    {

    }
    void addProperty(std::string i, Property p)
    {
        properties.insert(std::pair<std::string, Property>(i, p));
    }


    void update(int t, int lt)
    {
        std::map < std::string, Property >::iterator it;
        for (it = properties.begin(); it != properties.end(); it++)
        {
            it->second.update(t, lt);
        }
    }

    void render(int t)
    {
        std::map < std::string, Property >::iterator it;
        for (it = properties.begin(); it != properties.end(); it++)
        {
            it->second.render(t);
        }
    }

    Property getProperty(std::string i)
    {
        return properties[i];
    }

};