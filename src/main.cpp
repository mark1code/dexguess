// TODO game loop: prompt -> check -> feedback -> repeat
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <random>

struct Pokemon{
    std::string name;
    std::string type1;
    std::string type2;
    int evolStage;
    bool fullEvol;
    int gen;
    int statTotal;
    std::string bestStat;
};

bool loadPokemonData(std::vector<Pokemon>& info){
    std::ifstream file("data/pokemon.csv");
    if(!file){
        std::cerr << "Error: couldn't open file" << std::endl;
        return false;
    }
    std::string line;
    std::getline(file, line);
    while (std::getline(file, line)){
        std::stringstream ss(line);
        std::string field;
        std::vector<std::string> fields;
        // split strings and return as vector
        while (std::getline(ss, field, ',')){
            fields.push_back(field);
        }
        
        Pokemon mon;
        mon.name = fields[0];
        mon.type1 = fields[1];
        mon.type2 = fields[2];
        mon.evolStage = std::stoi(fields[3]);
        mon.fullEvol = (fields[4] == "true");
        mon.gen = std::stoi(fields[5]);
        mon.statTotal = std::stoi(fields[6]);
        mon.bestStat = fields[7];
        info.push_back(mon);
    }
    return true;   
}

int main(){
    std::vector<Pokemon> pokeInfo;
    if(!loadPokemonData(pokeInfo)) return 1;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, pokeInfo.size()-1);
    int index = dist(gen);
    
    std::cout << pokeInfo[index].name << std::endl;
    std::cout << pokeInfo[index].type1<< std::endl;
    std::cout << pokeInfo[index].type2 << std::endl;
    std::cout << pokeInfo[index].evolStage << std::endl;
    std::cout << pokeInfo[index].fullEvol << std::endl;
    std::cout << pokeInfo[index].gen << std::endl;
    std::cout << pokeInfo[index].statTotal<< std::endl;
    std::cout << pokeInfo[index].bestStat << std::endl;
    return 0; 
}