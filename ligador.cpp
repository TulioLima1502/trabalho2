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

#define TABELA 1

typedef struct tabela_uso
{
	string simbolo;
	int endereco;
} tabela_uso;

typedef struct tabela_definicoes
{
	string simbolo;
	int valor;
} tabela_definicoes;


bool file_exist(std::string fileName)
{
	fileName = fileName + ".txt" ; //corrigir e trocar pra .o
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
	string file_out = file_in + "out.txt"; //TODO criar nome de saída corretamente
	ofstream ofile(file_out);

	file_in = file_in + ".txt";	//todo corrigir formato
	std::ifstream infile(file_in);
	std::string line;
	string str;

	//While lê arquivo de entrada até o arquivo acabar
	while (std::getline(infile, line))
	{
		vector<string> token_vector = separate_tokens(line);

		vector<string>::iterator it = token_vector.begin();
		str = *it;

		if (!str.compare("T:"))
		{
			token_vector.erase(token_vector.begin());
			for (const auto &e : token_vector)
				ofile << e << " ";
			break;
		}
	}
}

void ligador2(string file_1, string file_2)
{
	vector<string> token_vector;
	vector<string>::iterator it;
	string line;
	string str;


	//***********************************
	//COPIA CABEÇALHO DO PRIMEIRO ARQUIVO
	//***********************************
	string file_in = file_1 + ".txt"; //todo corrigir formato	

	std::ifstream infile(file_in);
	std::getline(infile, line);
	std::getline(infile, line);

	//GERA FATOR DE RELOCAÇÃO
	token_vector = separate_tokens(line);
	it = token_vector.begin();
	it++;
	int fator_relocacao = stoi(*it) + 1; //fator de relocacao = arquivo + 1 ???


	//GERA MAPA DE RELOCAÇÃO 
	vector<int> mapa_relocacao_vector1;
	std::getline(infile, line);
	token_vector = separate_tokens(line);
	it = token_vector.begin();
	it ++;
	while (it != token_vector.end())
	{
		mapa_relocacao_vector1.push_back(stoi(*it));
		it++;
	}

	//COPIA TABELA DE USO
	vector<tabela_uso> uso_vector1;
	std::getline(infile, line);
	token_vector = separate_tokens(line);
	it = token_vector.begin();
	it++;
	while (it != token_vector.end())
	{
		tabela_uso temp2;
		temp2.simbolo = (*it);
		it++;
		temp2.endereco = stoi(*it);
		uso_vector1.push_back(temp2);
		it++;
	}

	//COPIA TABELA DE DEFINIÇÕES
	vector<tabela_definicoes> definicoes_vector1;
	std::getline(infile, line);
	token_vector = separate_tokens(line);
	it = token_vector.begin();
	it++;
	while (it != token_vector.end())
	{
		tabela_definicoes temp2;
		temp2.simbolo = (*it);
		it++;
		temp2.valor = stoi(*it);
		definicoes_vector1.push_back(temp2);
		it++;
	}

	infile.close();

	if(TABELA)
	{
		cout << "********************" << endl;
		cout << "TABELAS DO ARQUIVO 1" << endl;
		cout << "FATOR DE RELOCAÇÃO: " << fator_relocacao << endl;
		cout << "MAPA DE RELOCAÇÃO: ";
		for (vector<int>::iterator i = mapa_relocacao_vector1.begin(); i != mapa_relocacao_vector1.end(); i++)
		{
			cout << (*i) << " ";
		}
		cout << endl << "TABELA DE USO: ";
		for (vector<tabela_uso>::iterator i = uso_vector1.begin(); i != uso_vector1.end(); i++)
		{
			cout << "\n" << (*i).simbolo << " " << (*i).endereco;
		}
		cout << endl << "TABELA DE DEFINICOES: ";
		for (vector<tabela_definicoes>::iterator i = definicoes_vector1.begin(); i != definicoes_vector1.end(); i++)
		{
			cout << "\n" << (*i).simbolo << " " << (*i).valor;
		}
		cout << endl << endl;
	}

	//***********************************
	//COPIA CABEÇALHO DO SEGUNDO ARQUIVO
	//***********************************
	string file_in2 = file_2 + ".txt"; //todo corrigir formato	

	std::ifstream infile2(file_in2);
	std::getline(infile2, line);
	std::getline(infile2, line);

	//GERA MAPA DE RELOCAÇÃO 
	vector<int> mapa_relocacao_vector2;
	std::getline(infile2, line);
	token_vector = separate_tokens(line);
	it = token_vector.begin();
	it ++;
	while (it != token_vector.end())
	{
		mapa_relocacao_vector2.push_back(stoi(*it)+fator_relocacao);
		it++;
	}

	//COPIA TABELA DE USO
	vector<tabela_uso> uso_vector2;
	std::getline(infile2, line);
	token_vector = separate_tokens(line);
	it = token_vector.begin();
	it++;
	while (it != token_vector.end())
	{
		tabela_uso temp2;
		temp2.simbolo = (*it);
		it++;
		temp2.endereco = stoi(*it) + fator_relocacao;
		uso_vector2.push_back(temp2);
		it++;
	}

	//COPIA TABELA DE DEFINIÇÕES
	vector<tabela_definicoes> definicoes_vector2;
	std::getline(infile2, line);
	token_vector = separate_tokens(line);
	it = token_vector.begin();
	it++;
	while (it != token_vector.end())
	{
		tabela_definicoes temp2;
		temp2.simbolo = (*it);
		it++;
		temp2.valor = stoi(*it) + fator_relocacao;
		definicoes_vector2.push_back(temp2);
		it++;
	}

	infile2.close();

	if(TABELA)
	{
		cout << "\n\n\n********************" << endl;
		cout << "TABELAS DO ARQUIVO 2" << endl;
		cout << "MAPA DE RELOCAÇÃO: ";
		for (vector<int>::iterator i = mapa_relocacao_vector2.begin(); i != mapa_relocacao_vector2.end(); i++)
		{
			cout << (*i) << " ";
		}
		cout << endl << "TABELA DE USO: ";
		for (vector<tabela_uso>::iterator i = uso_vector2.begin(); i != uso_vector2.end(); i++)
		{
			cout << "\n" << (*i).simbolo << " " << (*i).endereco;
		}
		cout << endl << "TABELA DE DEFINICOES: ";
		for (vector<tabela_definicoes>::iterator i = definicoes_vector2.begin(); i != definicoes_vector2.end(); i++)
		{
			cout << "\n" << (*i).simbolo << " " << (*i).valor;
		}
		cout << endl << endl;
	}


	int pc = 0;
	string variavel;
	int is_uso =0;

	string file_out = "ligado.txt"; //TODO criar nome de saída corretamente
	ofstream ofile(file_out);
	//************************
	//COMEÇA A LIGAR ARQUIVO 1
	//************************
	std::ifstream infile3(file_in);
	while (std::getline(infile3, line))
	{
		cout << "entrou no while" << endl;
		token_vector = separate_tokens(line);
		it = token_vector.begin();
		str = *it;
		if (!str.compare("T:"))
		{
			it++;

			//PARTE DO CÓDIGO 
			while (it != token_vector.end())
			{
				str = *it;
				//PROCURA ENDEREÇO NA TABELA DE USO
				for (vector<tabela_uso>::iterator its = uso_vector1.begin(); its != uso_vector1.end(); its++)
				{
					if ((*its).endereco == pc)
					{	//É UMA VARIÁVEL DA TABELA DE USO
						is_uso =1;
						variavel = (*its).simbolo;
						for (vector<tabela_definicoes>::iterator itd = definicoes_vector2.begin(); itd != definicoes_vector2.end(); itd++)
						{
							if (!variavel.compare((*itd).simbolo))
							{
								//então eu achei qual a posiçao da tabela que tem o endereço que eu quero 
								//(*itd).valor esse é o valor que deve ir pro arquivo de saída
								ofile << (*itd).valor << " ";
							}
						}
					}
				}
				if (! is_uso)
				{
					ofile << *it << " ";	
				}
				is_uso = 0;
				it++;
				pc++;
			}
		}
	}
	infile3.close();

	//************************
	//COMEÇA A LIGAR ARQUIVO 2
	//************************
	std::ifstream infile4(file_in2);
	while (std::getline(infile4, line))
	{

	}


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
			cout << "\nERRO.\nArquivo "<< file_1 <<".o não existe nessa pasta!\n\n";
			return 0;
		}
		if (!file_exist(file_2))
		{
			cout << "\nERRO.\nArquivo "<<file_2 <<".o não existe nessa pasta!\n\n";
			return 0;
		}

		ligador2(file_1, file_2);

	}
	else
		cout << "Número de argumentos incorreto." << endl;

	return 0;
}


