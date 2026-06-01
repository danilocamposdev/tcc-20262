#include <fstream>
#include <filesystem>
#include "../src/optimizer/allocation_optimizer.h"
#include "./instances/test_instances.h"


int main() {
	const std::string results_directory = "tests/results/";
	const std::string results_file = "tests.txt";
	const std::string results_absolute_path = results_directory + results_file;

	std::filesystem::create_directories(results_directory);
	std::ofstream file(results_absolute_path);

	if (!file.is_open()) {
		std::cerr << "Erro ao abrir arquivo de saida.\n";
		return 1;
	}

	std::vector<std::unique_ptr<ProblemData>> instances;

	instances.push_back(std::make_unique<ProblemData>(instance_01()));
	instances.push_back(std::make_unique<ProblemData>(instance_02()));
	instances.push_back(std::make_unique<ProblemData>(instance_03()));
	instances.push_back(std::make_unique<ProblemData>(instance_04()));
	instances.push_back(std::make_unique<ProblemData>(instance_05()));
	instances.push_back(std::make_unique<ProblemData>(instance_06()));
	instances.push_back(std::make_unique<ProblemData>(instance_07()));

	for (size_t i = 0; i< instances.size(); ++i) {
		file << "\n########### Test " << i+1 << " ############" << std::endl;
		AllocationOptimizer optimizer{*instances[i]};

		if (optimizer.solve()) {
			optimizer.print_results(file);
		} else {
			file << "Falha ao resolver instancia.\n" << std::endl;
		}
	}

	std::cout << "Resultados salvos em: " << std::filesystem::absolute(results_absolute_path) << "\n";

	return 0;
}
