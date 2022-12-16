#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>

std::string readShaderFile(std::string filepath)
{
    std::string res, line;
    std::ifstream fin(filepath);
    if (!fin.is_open())
    {
        std::cout << "file: " << filepath << " failed to open" << std::endl;
        exit(-1);
    }
    while (std::getline(fin, line))
    {
        res += line + '\n';
    }
    fin.close();
    return res;
}

void readOff(std::string filepath, std::vector<glm::vec3>& points, std::vector<glm::vec3>& colors)
{
    std::string line;
    std::ifstream fin(filepath);
    if (!fin.is_open())
    {
        std::cout << "open file: " << filepath << " failed" << std::endl;
        exit(-1);
    }

    std::vector<glm::vec3> vertexPosition, vertexColor; 
    fin >> line;  
    int vertexNum, faceNum, edgeNum;
    fin >> vertexNum >> faceNum >> edgeNum;
    for (int i = 0; i < vertexNum; i++)
    {
        float p1, p2, p3;
        fin >> p1 >> p2 >> p3;
        vertexPosition.push_back(glm::vec3(p1, p2, p3));
        vertexColor.push_back(glm::vec3(p1 + 0.5, p2 + 0.5, p3 + 0.5)); 
    }
    points.clear();
    colors.clear();
    for (int i = 0; i < faceNum; i++)
    {
        int n, index1, index2, index3;
        fin >> n >> index1 >> index2 >> index3;
        points.emplace_back(vertexPosition[index1]);
        points.emplace_back(vertexPosition[index2]);
        points.emplace_back(vertexPosition[index3]);
        colors.emplace_back(vertexColor[index1]);
        colors.emplace_back(vertexColor[index2]);
        colors.emplace_back(vertexColor[index3]);
    }
}
