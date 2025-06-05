#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <random>
#include <algorithm>

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

std::string cleanString(const std::string& str) {
    std::string cleaned = str;
    cleaned.erase(std::remove(cleaned.begin(), cleaned.end(), '\r'), cleaned.end());
    cleaned.erase(std::remove(cleaned.begin(), cleaned.end(), '\n'), cleaned.end());
    cleaned.erase(cleaned.begin(), std::find_if(cleaned.begin(), cleaned.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
    cleaned.erase(std::find_if(cleaned.rbegin(), cleaned.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), cleaned.end());
    return cleaned;
}


bool loadPokemonData(std::vector<Pokemon>& info){
    std::ifstream file("data/pokemon.csv");
    if(!file){
        std::cerr << "Error: couldn't open file" << std::endl;
        return false;
    }

    std::string line;
    std::getline(file, line); // Skip header

    while (std::getline(file, line)){
        std::stringstream ss(line);
        std::string field;
        std::vector<std::string> fields;

        while (std::getline(ss, field, ',')){
            fields.push_back(cleanString(field));
        }

        if (fields.size() < 8){
            std::cerr << "Missing attributes, skipping: " << line << std::endl;
            continue;
        }

        try {
            Pokemon mon;
            mon.name = fields[0];
            mon.type1 = fields[1];
            mon.type2 = fields[2];
            mon.evolStage = std::stoi(fields[3]);
            std::string fe = fields[4];
            std::transform(fe.begin(), fe.end(), fe.begin(), ::tolower);
            mon.fullEvol = (fe == "true");
            mon.gen = std::stoi(fields[5]);
            mon.statTotal = std::stoi(fields[6]);
            mon.bestStat = fields[7];

            info.push_back(mon);
        } catch (const std::exception& e) {
            std::cerr << "Data error, skipping: " << line << " (" << e.what() << ")" << std::endl;
        }
    }

    return true;
}


bool nameMatch(const std::string& a, const std::string& b){
    std::string cpyA = a;
    std::string cpyB = b;
    std::transform(cpyA.begin(), cpyA.end(), cpyA.begin(), ::tolower);
    std::transform(cpyB.begin(), cpyB.end(), cpyB.begin(), ::tolower);
    cpyB.erase(std::remove_if(cpyB.begin(), cpyB.end(), [](unsigned char c) { return std::isspace(c); }), cpyB.end());
    cpyA.erase(std::remove_if(cpyA.begin(), cpyA.end(), [](unsigned char c) { return std::isspace(c); }), cpyA.end());
    
    return cpyA == cpyB;
}

const Pokemon* findPokemonByName(const std::vector<Pokemon>& pokeList, const std::string& guess){
    for (const Pokemon& mon : pokeList){
        if (nameMatch(mon.name, guess)){
            return &mon;
        }
    }
    return nullptr;
}

bool playGame(const std::vector<Pokemon>& pokeInfo){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, pokeInfo.size()-1);
    int index = dist(gen);
    Pokemon target = pokeInfo[index];
    int attempts = 0;

    while(true){
        std::string guess;
        std::cout << "Guess the Pokémon: ";
        std::getline(std::cin, guess);
        attempts++;
        // answer
        //std::cout << pokeInfo[index].name << std::endl;
        
        if(nameMatch(guess, target.name)){
            break;
        } else if(guess == "exit")
        {
            std::cout << pokeInfo[index].name << std::endl;
            break;
        
        } else {
            std::cout << "Wrong. Try again." << "\n\n";
        }
        const Pokemon* guessmon = findPokemonByName(pokeInfo, guess);
        if (!guessmon){continue;}
        std::cout << "Type1: " << (target.type1 == guessmon->type1 ? guessmon->type1 + " ✅" : "❌") << std::endl;
        std::cout << "Type2: " << (target.type2 == guessmon->type2 ? guessmon->type2 + " ✅" : "❌") << std::endl;
        std::cout << "Evolution Stage: " << (target.evolStage == guessmon->evolStage ? std::to_string(guessmon->evolStage) + " ✅" : "❌") << std::endl;
        std::cout << "Fully Evolved: " << (target.fullEvol == guessmon->fullEvol ? (guessmon->fullEvol ? "true ✅" : "false ✅") : "❌") << std::endl;
        std::cout << "Generation: " << (target.gen == guessmon->gen ? std::to_string(guessmon->gen) + " ✅" : "❌") << std::endl;
        if (guessmon->statTotal == target.statTotal) {
            std::cout << "Stat Total: " << guessmon->statTotal << " ✅" << std::endl;
        } else if (guessmon->statTotal < target.statTotal) {
            std::cout << "Stat Total: Higher 🔼" << std::endl;
        } else {
            std::cout << "Stat Total: Lower 🔽" << std::endl;
        }
        std::cout << "Best Stat: " << (target.bestStat == guessmon->bestStat ? guessmon->bestStat + " ✅" : "❌") << "\n" << std::endl;
    }


    std::cout << "You win. It was " << pokeInfo[index].name << ".\n";
    std::cout << "Number of guesses: " << attempts << "\n\n";
    std::string again;
    std::cout << "Go again? [y/n]: ";
    std::getline(std::cin, again);
    return (!again.empty() && std::tolower(again[0]) == 'y');
}

int main(){
    std::vector<Pokemon> pokeInfo;
    if(!loadPokemonData(pokeInfo)) return 1;
    bool game = playGame(pokeInfo);
    while(game){
        game = playGame(pokeInfo);
    }

    return 0; 
} // é