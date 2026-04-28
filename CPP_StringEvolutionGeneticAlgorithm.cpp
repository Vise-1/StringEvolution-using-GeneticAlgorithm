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

class Genome {
	private:
		string word;
		int fitness;

	public: 

	Genome() {
		word = "";
		fitness = 0;
	}

	Genome(string s) : word(s), fitness(0) {
		update_fitness();
	}
		
	void update_fitness() {
		fitness = 0;
		for (int i = 0; i < word.size(); i++) {
			if (word[i] == target[i]) {
				fitness++;
			}
		}
	}
	// setter
	void set_word(const string& new_word) {
		word = new_word;
		update_fitness();
	}

	// getter
	int get_fitness() const { return fitness; }
	string get_word() const { return word; }
};

const string characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890";
//we can add special characters as well above!

vector<Genome> population;

// 2. Initialization of the population
Genome generate_StringGenome() {
	string new_word = "";
	uniform_int_distribution<> chooseletterindex(0, characters.size());
	for (int i = 0; i < target.size(); i++) {
		new_word += characters[chooseletterindex(gen)];
	}
	return Genome(new_word);
}

vector<Genome> generate_string_population(int population_size) {
	for (int i = 0; i < population_size; i++) {
		Genome newgenome = generate_StringGenome();
		population.push_back(newgenome);
	}
	return population;
}

// 3. Fitness Function
// --> Already calculated in genome class, Higher match = higher fitness



// 4. Selection of parents
vector<Genome> select_parents(vector<Genome> population) {
	int total_fitness = 0;

	for (const Genome& genome : population) {
		total_fitness += genome.get_fitness();
	}

	if (total_fitness <= 0) return { population[0], population[1] };

	// Roulette Wheel Selection


	uniform_int_distribution<int> dis(0, total_fitness);
	int roulette_number = dis(gen);

	Genome parent1, parent2;

	int running_sum = 0;

	// Selection for Parent 1
	for (int i = 0; i < population.size(); ++i) {
		running_sum += population[i].get_fitness(); // getting fitness value form each genom in the population
		if (running_sum >= roulette_number) {
			parent1 = population[i];
			break; // <--- CRITICAL: Stop once found!
		}
	}

	// Selection for Parent 2
	roulette_number = dis(gen);
	running_sum = 0;
	for (int i = 0; i < population.size(); ++i) {
		running_sum += population[i].get_fitness();
		if (running_sum >= roulette_number) {
			parent2 = population[i];
			break; // <--- CRITICAL: Stop once found!
		}
	}
	return { parent1, parent2 };


}

// 5. Crossover and Mutation

vector<Genome> crossover(const Genome& parent1, const Genome& parent2) {
	if	(parent1.get_word().size() != parent2.get_word().size()) {
		return {}; // Invalid parents
	}
	if (parent1.get_word().size() != 0 & parent2.get_word().size() != 0) {
		int crossover_point = rand() % parent1.get_word().size();
		Genome child1;
		child1.set_word(parent1.get_word().substr(0, crossover_point) + parent2.get_word().substr(crossover_point));
		Genome child2;
		child2.set_word(parent2.get_word().substr(0, crossover_point) + parent1.get_word().substr(crossover_point));
		return { child1, child2 };
	}
}

Genome mutate(Genome genome, double mutation_rate) {
	for (int i = 0; i < genome.get_word().size(); i++) {
		if (((double)rand() / RAND_MAX) < mutation_rate) {
			string new_word = genome.get_word();
			new_word[i] = characters[rand() % characters.size()];
			genome.set_word(new_word);
		}
	}
	return genome;
}

vector<Genome> run_evolution(int initial_population_size, int generation_limit, int fitness_limit, int mutation_rate) {
	
	population = generate_string_population(initial_population_size);

	int timestart = 0, timeend = 0;
	for (int g = 0; g < generation_limit; ++g) {

		int best_fitness = 0;
		string best_genomeWord = "";
		for (const Genome& genome : population) {
			int fitness = genome.get_fitness();
			if (fitness > best_fitness) {
				best_fitness = fitness;
				best_genomeWord = genome.get_word();
			}
		}
		
		cout << "Gen " << g << " Best: " << best_genomeWord << " (" << best_fitness << ")" << endl;

		if (best_fitness >= fitness_limit) {
			cout << "Genome string evolved: " << best_genomeWord << endl;
			return population;
		}


		vector<Genome> new_population;
		while (new_population.size() < initial_population_size) {
			vector<Genome> parents = select_parents(population);
			vector<Genome> children = crossover(parents[0], parents[1]);

			for (Genome child : children) {
				Genome mutated_child = mutate(child, mutation_rate);
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

	int starttotal = clock();
	run_evolution(350, 1000, target.size(), 0.07);
	int endtotal = clock();
	double totaltime = double(endtotal - starttotal) / CLOCKS_PER_SEC;
	cout << "Total Evolution Time: " << totaltime << " seconds." << endl;

	cout << endl;cout << endl;
	cout << "Made by Sparsh (No AI generated code was used, except for minor bug fixes)" << endl;

	system("pause");
	return 0;
} 