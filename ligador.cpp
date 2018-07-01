#include <iostream>
#include <istream>
#include <fstream>
#include <sstream>
#include <ostream>
#include <string>
#include <cstring>
#include <iostream>
#include <map>
#include <cstddef>
#include <vector>
#include <algorithm>

using namespace std;


bool file_exist(std::string fileName)
{
	//fileName = fileName + ".txt" ; //corrigir e trocar pra .o
	std::ifstream infile(fileName);
	return infile.good();
}

vector<string> separate_tokens(string line)
{
	stringstream stream_line(line);
	vector<string> token_vector;
	size_t prev = 0, pos;
	//Separa tokens por espaços ou vírgulas
	while (((pos = line.find_first_of(" ", prev)) != std::string::npos) || (pos = line.find_first_of(",", prev)) != std::string::npos)
	{
		if (pos > prev)
			token_vector.push_back(line.substr(prev, pos - prev));
		prev = pos + 1;
	}
	if (prev < line.length())
		token_vector.push_back(line.substr(prev, std::string::npos));

	return token_vector;
}

void ligador1(string file_in)
{
	std::ifstream infile(file_in);
	std::string line;
	string str;

	string file_out = "teste.txt"; //todo criar nome de saída 
	ofstream ofile(file_out);

	vector<string> aux;

	//While lê arquivo de entrada até o arquivo acabar
	while (std::getline(infile, line))
	{
		vector<string> token_vector = separate_tokens(line);

		vector<string>::iterator it = token_vector.begin();
		str = *it;

		if (!str.compare("T:"))
		{
			//começa realmente a ligação
			//TODO conferir se a lgação pra um arquivo é só copiar o texto mesmo
			token_vector.erase(token_vector.begin());
			for (const auto &e : token_vector)
				ofile << e << " ";
			break;
		}
	}
}

void ligador2(string file_1, string file_2)
{
	cout << "ligador2" <<endl;
}

int main(int argc, char *argv[])
{
	//argc eh um inteiro com o numero de argumentos passados pela linha de comando
	//argv eh um vetor com os argumentos. argv[0] sempre sera o path do programa,
	//entao eh basicamente ignorado. por isso, o argc na verdade vai ser o numero
	//de argumentos mais um.

	string file_1;

	if (argc == 2)
	{
		cout << "Faz a saída do ligador para 1 arquivo" << endl;
		file_1 = argv[1]; // passar para learquivo(). eh o nome do arquivo .asm.

		if (!file_exist(file_1))
		{
			cout << "\nERRO.\nArquivo não existe nessa pasta!\n\n";
			return 0;
		}

		ligador1(file_1);
	}
	else if (argc == 3)
	{
		cout << "Faz a saída do ligador para 2 arquivos" << endl;
		file_1 = argv[1];
		string file_2 = argv[2];

		if (!file_exist(file_1))
		{
			cout << "\nERRO.\nArquivo não existe nessa pasta!\n\n";
			return 0;
		}
		if (!file_exist(file_2))
		{
			cout << "\nERRO.\nArquivo não existe nessa pasta!\n\n";
			return 0;
		}

		ligador2(file_1, file_2);

	}
	else
		cout << "Número de argumentos incorreto." << endl;

	return 0;
}


