#include <iostream>
#include <string>
#include <random>
#include <time.h>
#include <list>
#include <algorithm>
#include <vector>
using namespace std;

random_device rd;
mt19937 gen(rd());
//NOtes: Generalize functions


// 1. Genetic Representation of the solution 
// Problem: Evolve a string to match a target string (e.g., "Supercalifragilisticexpialidocious")

string target = "Trippi";
string current = "";

const string characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890";

vector<string> population;

// 2. Initialization of the population
string generate_StringGenome(string current, int genome_length) {
	current.resize(target.size());
	string randomString = current;

	uniform_int_distribution<> chooseletter(0, characters.size());
	for (int i = 0; i < genome_length; i++) {
		randomString[i] = characters[chooseletter(gen)];
	}
	return randomString;
}

vector<string> generate_string_population(int genome_length, int population_size) {
	for (int i = 0; i < population_size; i++) {
		string genome = generate_StringGenome(current, genome_length);
		population.push_back(genome);
	}
	return population;
}

// 3. Fitness Function
int fitness_function(string genome) {
	if (genome.size() != target.size()) {
		return 0; // Invalid genome
	}

	int fitness = 0;
	for (int i = 0; i < genome.size(); i++) {
		if (genome[i] == target[i]) {
			fitness++;
		}
	}
	return fitness;
}


// 4. Selection of parents
vector<string> select_parents(vector<string> population) {
	int total_fitness = 0;
	vector<int> fitness_values;


	for (const string& genome : population) {
		int genome_fitness = fitness_function(genome);
		total_fitness += genome_fitness;
		fitness_values.push_back(genome_fitness);
	}

	if (total_fitness <= 0) return { population[0], population[1] };

	// Roulette Wheel Selection


	uniform_int_distribution<int> dis(0, total_fitness);
	int roulette_number = dis(gen);

	string parent1, parent2;

	int running_sum = 0;

	// Selection for Parent 1
	for (int i = 0; i < population.size(); ++i) {
		running_sum += fitness_values[i]; // Use the stored value
		if (running_sum >= roulette_number) {
			parent1 = population[i];
			break; // <--- CRITICAL: Stop once found!
		}
	}

	// Selection for Parent 2
	roulette_number = dis(gen);
	running_sum = 0;
	for (int i = 0; i < population.size(); ++i) {
		running_sum += fitness_values[i];
		if (running_sum >= roulette_number) {
			parent2 = population[i];
			break; // <--- CRITICAL: Stop once found!
		}
	}
	return { parent1, parent2 };


}

// 5. Crossover and Mutation

vector<string> crossover(string parent1, string parent2) {
	if	(parent1.size() != parent2.size()) {
		return {}; // Invalid parents
	}
	
	int crossover_point = rand() % parent1.size();
	string child1 = parent1.substr(0, crossover_point) + parent2.substr(crossover_point);
	string child2 = parent2.substr(0, crossover_point) + parent1.substr(crossover_point);
	return { child1, child2 };
}

string mutate(string genome, double mutation_rate) {
	for (int i = 0; i < genome.size(); i++) {
		if (((double)rand() / RAND_MAX) < mutation_rate) {
			genome[i] = characters[rand() % characters.size()];
		}
	}
	return { genome };
}

vector<string> run_evolution(int initial_population_size, int generation_limit, int fitness_limit, int mutation_rate) {
	
	population = generate_string_population(target.size(), initial_population_size);

	for (int g = 0; g < generation_limit; ++g) {

		int best_fitness = 0;
		string best_genome = "";
		for (const string& s : population) {
			int f = fitness_function(s);
			if (f > best_fitness) {
				best_fitness = f;
				best_genome = s;
			}
		}
		if (fitness_function(population[0]) >= fitness_limit ){
			cout << "Target string evolved: " << population[0] << endl;
			return population;
		}

		cout << "Gen " << g << " Best: " << best_genome << " (" << best_fitness << ")" << endl;

		if (best_fitness >= fitness_limit) {
			cout << "Target reached!" << endl;
			return population;
		}

		vector<string> new_population;
		while (new_population.size() < initial_population_size) {
			vector<string> parents = select_parents(population);

			vector<string> children = crossover(parents[0], parents[1]);

			for (string child : children) {
				string mutated_child = mutate(child, mutation_rate);
				new_population.push_back(mutated_child);
			}
		}
		population = new_population;

	}
	return population;
}

int main()
{
	
	cout << "Welcome to the String Evolution Genetic Algorithm!" << endl;
	cout << "Give me a string to evolve: ";
	cin >> target;
	
	cout << "Target String: " << target << endl;

	run_evolution(350, 1000, target.size(), 0.07);
}