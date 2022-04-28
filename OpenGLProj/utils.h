
#pragma once
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>

// The namespace that holds all of the math	functions needs
namespace math
{

    static float Lerp(float cur, float dest, float strength) {
        return cur + (dest - cur) * strength;
    }


    static glm::vec3 Lerp(glm::vec3 currentRotation, glm::vec3 dest, float strength) {
        return currentRotation + (dest - currentRotation) * strength;
    }


    static int PingPong(int time, int length) {
        int L = 2 * length;
        int T = time % L;

        if (0 <= T && T < length)
            return T;
        else
            return L - T;
    }


    // Projection Calculation of a onto b
    static glm::vec3 ProjV3(const  glm::vec3 a, const  glm::vec3 b)
    {

        glm::vec3 bn = b / glm::length(b);
        return bn * glm::dot(a, bn);
    }

    // Angle between 2 Vector3 Objects
    static float AngleBetweenV3(const glm::vec3 a, const glm::vec3 b)
    {
        float angle = glm::dot(a, b);
        angle /= (glm::length(a) * glm::length(b));
        return angle = acosf(angle);
    }
}

//The namespace that holds all of the Algorithms needed for loading models
namespace algorithm
{
    // A test to see if P1 is on the same side as P2 of a line segment ab
    static bool SameSide(glm::vec3  p1, glm::vec3  p2, glm::vec3  a, glm::vec3  b)
    {
        glm::vec3 cp1 = glm::cross(b - a, p1 - a);
        glm::vec3 cp2 = glm::cross(b - a, p2 - a);

        if (glm::dot(cp1, cp2) >= 0)
            return true;
        else
            return false;
    }

    // Generate a cross produect normal for a triangle
    static glm::vec3 GenTriNormal(glm::vec3 t1, glm::vec3 t2, glm::vec3 t3)
    {
        glm::vec3 u = t2 - t1;
        glm::vec3 v = t3 - t1;

        glm::vec3 normal = glm::cross(u, v);

        return normal;
    }

    // Check to see if a Vector3 Point is within a 3 Vector3 Triangle
    static bool inTriangle(glm::vec3 point, glm::vec3 tri1, glm::vec3 tri2, glm::vec3 tri3)
    {
        // Test to see if it is within an infinite prism that the triangle outlines.
        bool within_tri_prisim = SameSide(point, tri1, tri2, tri3) && SameSide(point, tri2, tri1, tri3)
            && SameSide(point, tri3, tri1, tri2);

        // If it isn't it will never be on the triangle
        if (!within_tri_prisim)
            return false;

        // Calulate Triangle's Normal
        glm::vec3 n = GenTriNormal(tri1, tri2, tri3);

        // Project the point onto this normal
        glm::vec3 proj = math::ProjV3(point, n);

        // If the distance from the triangle to the point is 0
        //	it lies on the triangle
        if (glm::length(proj) == 0)
            return true;
        else
            return false;
    }

    // Split a String into a string array at a given token
    static void split(const std::string& in,
        std::vector<std::string>& out,
        std::string token)
    {
        out.clear();

        std::string temp;

        for (int i = 0; i < int(in.size()); i++)
        {
            std::string test = in.substr(i, token.size());

            if (test == token)
            {
                if (!temp.empty())
                {
                    out.push_back(temp);
                    temp.clear();
                    i += (int)token.size() - 1;
                }
                else
                {
                    out.push_back("");
                }
            }
            else if (i + token.size() >= in.size())
            {
                temp += in.substr(i, token.size());
                out.push_back(temp);
                break;
            }
            else
            {
                temp += in[i];
            }
        }
    }

    // Get tail of string after first token and possibly following spaces
    static std::string tail(const std::string& in)
    {
        size_t token_start = in.find_first_not_of(" \t");
        size_t space_start = in.find_first_of(" \t", token_start);
        size_t tail_start = in.find_first_not_of(" \t", space_start);
        size_t tail_end = in.find_last_not_of(" \t");
        if (tail_start != std::string::npos && tail_end != std::string::npos)
        {
            return in.substr(tail_start, tail_end - tail_start + 1);
        }
        else if (tail_start != std::string::npos)
        {
            return in.substr(tail_start);
        }
        return "";
    }

    // Get first token of string
    static std::string firstToken(const std::string& in)
    {
        if (!in.empty())
        {
            size_t token_start = in.find_first_not_of(" \t");
            size_t token_end = in.find_first_of(" \t", token_start);
            if (token_start != std::string::npos && token_end != std::string::npos)
            {
                return in.substr(token_start, token_end - token_start);
            }
            else if (token_start != std::string::npos)
            {
                return in.substr(token_start);
            }
        }
        return "";
    }

    // Get element at given index position
    template <class T>
    static const T& getElement(const std::vector<T>& elements, std::string& index)
    {
        int idx = std::stoi(index);
        if (idx < 0)
            idx = int(elements.size()) + idx;
        else
            idx--;
        return elements[idx];
    }
}


