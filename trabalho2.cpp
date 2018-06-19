//Codigo em C++ do Primeiro Trabalho de Software Basico

// Desenvolvedores: Túlio Mariano da Silva Lima e Débora Ferreira dos Santos

// Descrição do trabalho 1
// Objetivos: Fazer um programa que personifique o um montador fazendo uso de uma ou duas passagens
// além de realizar a expansão de macros e um pre-processamento

// Funcoes: Pre-processamento de arquivos .asm e retorna um arquivo .pre
// Expansão de macros de arquivos .asm e retorna um arquivo .mcr
// Montagem conforme o algoritmo de duas passagens e retorna um arquivo .o
// O CÓDIGO DA DUPLA ENCONTRA-SE DISPONÍVEL NO GITHUB, NO SEGUINTE REPOSITÓRIO:
//
// 		https://github.com/TulioLima1502/trabalho1
//
// PARA A CONFECÇÃO DO TRABALHO 1 FOI UTILIZADO O VISUAL STUDIO CODE (	diferente do visual studio community, mas igualmente gratuito)
//
// INSTRUÇÕES DE USO/COMPILAÇÃO
// É NECESSÁRIO DE UM COMPUTADOR COM LINUX (UBUNTU 16 e 18)
// É NECESSAŔIO TER OS PACOTES DE C++ PARA REALIZAR A COMPILAÇÃO E EXECUÇÃO PELA LINHA DE COMANDO
// É NECESSÁRIO REALIZAR A COMPILAÇÃO PARA SÓ EM SEGUIDA EXECUTAR
// O COMANDO DE COMPILAÇÃO É: g++ -std=c++11 trabalho1.cpp -o <nome do executável>
// DEPOIS DE COMPILADO BASTA EXECUTAR O PROGRAMA COM O COMANDO:
//
//				./<nome do executável> -x <nome do arquivo>.asm <nome do arquivo de saida>
//
// ** PARA CORRETO FUNCIONAMENTO É NECESSAŔIO TER UM ARQUIVO TEXTO COM EXTENSÃO .ASM NA MESMA PASTA DO PROGRAMA TRABALHO1.CPP

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

//DEFINIÇÃO DE STRUCTS
typedef struct tabela_simbolo
{
	string simbolo;
	int valor;
	int is_const;
	int valor_const;
} tabela_simbolo;

typedef struct tabela_instrucao
{
	string mnemonico;
	string opcode;
	int n_operando;
} tabela_instrucao;

typedef struct tabela_diretiva
{
	string mnemonico;
	int n_operando;
} tabela_diretiva;

//DEFINIÇÃO DAS TABELAS
vector<tabela_simbolo> tabela_simbolo_vector;

vector<tabela_instrucao> tabela_instrucao_vector;

vector<tabela_diretiva> tabela_diretiva_vector;

//VARIÁVEL GLOBAL AUXILIAR
int data = -1;
int data_pc = -1;

//INICIALIZAÇÃO DAS TABELAS
//*****TABELA DE INSTRUÇÕES
void inicia_tabela_instrucao()
{
	tabela_instrucao temp;

	temp.mnemonico = "ADD";
	temp.opcode = "01";
	temp.n_operando = 1;
	tabela_instrucao_vector.push_back(temp);

	temp.mnemonico = "SUB";
	temp.opcode = "02";
	temp.n_operando = 1;
	tabela_instrucao_vector.push_back(temp);

	temp.mnemonico = "MULT";
	temp.opcode = "03";
	temp.n_operando = 1;
	tabela_instrucao_vector.push_back(temp);

	temp.mnemonico = "DIV";
	temp.opcode = "04";
	temp.n_operando = 1;
	tabela_instrucao_vector.push_back(temp);

	temp.mnemonico = "JMP";
	temp.opcode = "05";
	temp.n_operando = 1;
	tabela_instrucao_vector.push_back(temp);

	temp.mnemonico = "JMPN";
	temp.opcode = "06";
	temp.n_operando = 1;
	tabela_instrucao_vector.push_back(temp);

	temp.mnemonico = "JMPP";
	temp.opcode = "07";
	temp.n_operando = 1;
	tabela_instrucao_vector.push_back(temp);

	temp.mnemonico = "JMPZ";
	temp.opcode = "08";
	temp.n_operando = 1;
	tabela_instrucao_vector.push_back(temp);

	temp.mnemonico = "COPY";
	temp.opcode = "09";
	temp.n_operando = 2;
	tabela_instrucao_vector.push_back(temp);

	temp.mnemonico = "LOAD";
	temp.opcode = "10";
	temp.n_operando = 1;
	tabela_instrucao_vector.push_back(temp);

	temp.mnemonico = "STORE";
	temp.opcode = "11";
	temp.n_operando = 1;
	tabela_instrucao_vector.push_back(temp);

	temp.mnemonico = "INPUT";
	temp.opcode = "12";
	temp.n_operando = 1;
	tabela_instrucao_vector.push_back(temp);

	temp.mnemonico = "OUTPUT";
	temp.opcode = "13";
	temp.n_operando = 1;
	tabela_instrucao_vector.push_back(temp);

	temp.mnemonico = "STOP";
	temp.opcode = "14";
	temp.n_operando = 0;
	tabela_instrucao_vector.push_back(temp);
}

void inicia_tabela_diretiva()
{
	tabela_diretiva temp2;

	temp2.mnemonico = "SECTION";
	temp2.n_operando = 1;
	tabela_diretiva_vector.push_back(temp2);

	temp2.mnemonico = "SPACE";
	temp2.n_operando = 1;
	tabela_diretiva_vector.push_back(temp2);

	temp2.mnemonico = "CONST";
	temp2.n_operando = 1;
	tabela_diretiva_vector.push_back(temp2);
}

bool file_exist(std::string fileName)
{
	//fileName = fileName + ".asm" ;
	std::ifstream infile(fileName);
	return infile.good();
}

string filtro_comentarios(string line)
{
	size_t poscom = line.find(";");
	int coluna;

	if (poscom != (line).npos)
	{
		coluna = poscom;
		line = line.substr(0, coluna);
	}
	return (line);
}

string formato_padrao(string line)
{
	size_t posspace;
	size_t postab = line.find("\t");

	while (postab != line.npos) //loop que tira tabs e troca por espacos.
	{
		if (postab == 0)
		{
			line.erase(line.begin() + postab);
		}
		else
		{
			line.replace(line.begin() + postab, line.begin() + postab + 1, " ");
		}
		postab = line.find("\t");
	}

	posspace = line.find(" "); // loop que tira espacos a mais.
	while (posspace != line.npos)
	{ // o formato padrao eh com todos argumentos
		if (posspace == 0)
		{ // com apenas um espaco entre eles.
			do
			{
				line.erase(line.begin() + posspace);
				posspace = line.find(" ");
			} while (posspace == 0);
		}
		else
		{
			posspace = line.find("  ");
			while (posspace != line.npos)
			{
				line.erase(line.begin() + posspace);
				posspace = line.find("  ");
			}
		}
	}
	if (line[line.size() - 1] == ' ')
	{ //retira espacos no final
		size_t posfinal = line.size() - 1;
		line.erase(line.begin() + posfinal);
	}

	if (line == "\n")
	{			   //se a linha se resumir a uma quebra de linha,
		line = ""; //a mesma eh descartada.
	}

	return (line);
}

//void lerarquivo(char* file_name,char* file_name2 ) {
int lerarquivo(std::string file_name, char *file_name2)
{
	string line;
	string nome, saida, mcr, pontoo;
	nome = file_name2;

	nome = nome.substr(0, nome.size());
	//cout << "nome: " << nome << endl;

	saida = nome.substr(0, nome.size()) + ".pre";
	//cout << "saida: " << saida << endl;
	mcr = nome.substr(0, nome.size()) + ".mcr";
	//cout << mcr << endl;
	pontoo = nome.substr(0, nome.size()) + ".o";

	const char *psaida = saida.c_str();
	const char *pmcr = mcr.c_str();
	const char *ppontoo = pontoo.c_str();

	//cout<<file_name<<endl;
	ifstream myfile(file_name);
	int achoutext = 0, achoudata = 0, errosection = 0, erro_ordem_section = 0;

	remove("auxiliar");
	remove("MNT");
	remove("MDT");
	remove("EQU");
	remove(psaida);
	remove(pmcr);
	remove("tabela_de_simbolos");
	remove(ppontoo);

	int lineachousection = 0;

	ofstream mfile("auxiliar", ios::app);
	if (myfile.is_open())
	{
		int linecounter = 0;
		while (getline(myfile, line))
		{
			linecounter++;
			line = filtro_comentarios(line);
			line = formato_padrao(line); // remove tabulacoes, espacos extras e quebras de linhas.
			if (mfile.is_open() && line != "")
			{
				for (int i = 0; i < line.size(); i++)
				{
					line[i] = toupper(line[i]);
				}
				//cout << line << endl;
				mfile << line << endl;
			}
			if (line.find("SECTION TEXT") == 0)
			{
				achoutext = 1;
				if (lineachousection == 0)
				{
					lineachousection = linecounter;
				}
			}
			if (line.find("SECTION DATA") == 0)
			{
				achoudata = 1;
				if (achoutext == 0)
				{
					erro_ordem_section = 1;
				}
				if (lineachousection == 0)
				{
					lineachousection = linecounter;
				}
			}
			if (line.find("SECTION") == 0)
			{
				size_t pos = line.find("SECTION");
				int tamlinha = line.size();
				string sectionerrada = line.substr(pos + 8, tamlinha);
				//cout << sectionerrada << endl;
				if ((sectionerrada.compare("DATA") != 0) && (sectionerrada.compare("TEXT") != 0))
				{
					errosection = 1;
				}
			}
		}
		//cout << "\n";
		myfile.close();
	}

	else
		cout << "\nArquivo nao pode ser aberto!!!\n\n";

	if (achoutext == 0)
	{
		cout << "ERRO, FALTOU A SECTION TEXT" << endl;
	}
	//if (achoudata == 0)
	//{
	//		cout << "ERRO, FALTOU A SECTION DATA" << endl;
	//	}
	if (errosection == 1)
	{
		cout << "ERRO na difinição do Section" << endl;
	}
	if (erro_ordem_section == 1)
	{
		cout << "ERRO, A DIRETIVA 'TEXT' DEVE SEMPRE VIR ANTES DA DIRETIVA 'DATA'" << endl;
	}

	return lineachousection;
}

int leargumentos(string argumentos)
{
	//return 0 caso não tenha nenhum argumento
	//return 1 caso tenha um argumento
	//return 2 caso tenha 2 argumentos
	//return 3 caso tenha 3 argumentos
	//return 4 caso tenha 4 argumentos
	//para cada retorno a função que chama essa ativa uma rotina diferente
	argumentos = argumentos.substr(1, argumentos.size());
	int i = 0, n = 0, m = 0, l = 0;
	//cout << "Chamou a função e está retornando o que foi recebido pela função:" << argumentos << endl;

	//se não encontrar a virgula verifica se tem o '&', se não tiver retorna 0
	//se encontrar entra em uma rotina que conta a quantidade de vírgulas que tem, para saber quantos argumentos tem.

	for (i = 0; argumentos[i] != '\0'; i++)
	{
		if (argumentos[i] == ',')
		{
			n = n + 1;
		}
	}

	for (i = 0; argumentos[i] != '\0'; i++)
	{
		if (argumentos[i] == '&')
		{
			m = m + 1;
		}
	}

	for (i = 0; argumentos[i] != '\0'; i++)
	{
		if (argumentos[i] == ' ')
		{
			l = l + 1;
		}
	}

	if (l > 0)
	{
		cout << "Os argumentos estão separados por espaço, favor retirar os espaços entre os argumentos...\nDefina os argumentos separado apenas por vígulas..." << endl;
		return 5;
	}
	//cout << "A expressão tem " << n << " vírgulas..." << endl;
	//então cria vários caso para resolver esse tipo de macros

	if (n == 0)
	{
		//cout << "A expressão tem " << m << " argumentos" << endl;
		if (m == 1)
		{
			return 1;
		}
		else
		{
			cout << "Aqui pode ter um erro aqui caso a MACRO precise de uma argumento, pois não existe declaração de '&' indicando o argumento" << endl;
		}
		return 0;
	}
	else if (n == 1)
	{
		//cout << "A expressão tem " << m << " argumentos" << endl;
		if (m == 2)
		{
			return 2;
		}
		else
		{
			cout << "Expressão da MACRO mal definida" << endl;
			return 5;
		}
	}
	else if (n == 2)
	{
		//cout << "A expressão tem " << m << " argumentos" << endl;
		if (m == 3)
		{
			return 3;
		}
		else
		{
			cout << "Expressão da MACRO mal definida" << endl;
			return 5;
		}
	}
	else if (n == 3)
	{
		//cout << "A expressão tem " << m << " argumentos" << endl;
		if (m == 4)
		{
			return 4;
		}
		else
		{
			cout << "Expressão da MACRO mal definida" << endl;
			return 5;
		}
	}
}

string troca_a_linha_macro(string line, string linha_aux, string linha_aux_macro) //falta só verificar se o ENDMACRO não existe no arquivo e causa um loop e o caso de chamada de macro dentro de outra
{

	string variavel, variavel_macro;
	//cout << "Entrou na função de troca de linha da macro" << endl;
	//cout << "A linha a ser analisada: " << line << endl;
	//cout << "A variável que será buscada: " << linha_aux << endl;
	//cout << "A variável que vai ser substituída: " << linha_aux_macro << endl;

	//procura pelo valor do primeiro argumento
	//cout << linha_aux_macro.substr(linha_aux_macro.find("&"), linha_aux_macro.find(",")) << endl;
	variavel = linha_aux_macro.substr(linha_aux_macro.find("&"), linha_aux_macro.find(","));
	linha_aux_macro = linha_aux_macro.substr(linha_aux_macro.find(",") + 1, linha_aux_macro.size());
	//procura pelo valor do primeiro argumento
	//cout << linha_aux.substr(0, linha_aux.find(",")) << endl;
	variavel_macro = linha_aux.substr(0, linha_aux.find(","));
	linha_aux = linha_aux.substr(linha_aux.find(",") + 1, linha_aux.size());

	if (line.find(variavel) != line.npos)
	{
		//cout << "Encontrou esse argumento na função interna a macro" << endl;
		//cout << variavel << endl;
		//cout << variavel_macro << endl;
		//cout << line << endl;
		line.replace(line.find(variavel), variavel.size(), variavel_macro);
		//cout << "Aqui está: " << line << endl;
	}

	//aqui passamos a linha, o argumento que deve ser mudado e o que vai substituir para uma função
	//essa função recebe a linha e analisa se tem que fazer a troca
	//se tiver que fazer a troca já troca e retorna a linha com o valor trocado
	//ao final de tudo escreve em a função no arquivo .mcr

	//procura pelo valor do segundo argumento
	//cout << linha_aux_macro.substr(linha_aux_macro.find("&"), linha_aux_macro.find(",")) << endl;
	//cout << linha_aux_macro.substr(linha_aux_macro.find("&")+1,linha_aux_macro.find(",")) << endl;
	variavel = linha_aux_macro.substr(linha_aux_macro.find("&"), linha_aux_macro.find(","));
	linha_aux_macro = linha_aux_macro.substr(linha_aux_macro.find(",") + 1, linha_aux_macro.size());
	//procura pelo valor do segundo argumento
	variavel_macro = linha_aux.substr(0, linha_aux.find(","));
	//cout << linha_aux.substr(0, linha_aux.find(",")) << endl;
	linha_aux = linha_aux.substr(linha_aux.find(",") + 1, linha_aux.size());

	if (line.find(variavel) != line.npos)
	{
		//cout << "Encontrou esse argumento na função interna a macro" << endl;
		//cout << variavel << endl;
		//cout << variavel_macro << endl;
		//cout << line << endl;
		line.replace(line.find(variavel), variavel.size(), variavel_macro);
		//cout << "Aqui está: " << line << endl;
	}

	//procura pelo valor do terceiro argumento
	//cout << linha_aux_macro.substr(linha_aux_macro.find("&"), linha_aux_macro.find(",")) << endl;
	variavel = linha_aux_macro.substr(linha_aux_macro.find("&"), linha_aux_macro.find(","));

	linha_aux_macro = linha_aux_macro.substr(linha_aux_macro.find(",") + 1, linha_aux_macro.size());
	//procura pelo valor do terceiro argumento
	variavel_macro = linha_aux.substr(0, linha_aux.find(","));

	//cout << linha_aux.substr(0, linha_aux.find(",")) << endl;
	linha_aux = linha_aux.substr(linha_aux.find(",") + 1, linha_aux.size());

	if (line.find(variavel) != line.npos)
	{
		//cout << "Encontrou esse argumento na função interna a macro" << endl;
		//cout << variavel << endl;
		//cout << variavel_macro << endl;
		//cout << line << endl;
		line.replace(line.find(variavel), variavel.size(), variavel_macro);
		//cout << "Aqui está: " << line << endl;
	}

	//procura pelo valor do quarto argumento
	//cout << linha_aux_macro.substr(linha_aux_macro.find("&"), linha_aux_macro.find("\n")) << endl;
	variavel = linha_aux_macro.substr(linha_aux_macro.find("&"), linha_aux_macro.find("\n"));
	//procura pelo valor do quarto argumento
	variavel_macro = linha_aux.substr(0, linha_aux.find("\n"));
	//cout << linha_aux.substr(0, linha_aux.find("\n")) << endl;

	if (line.find(variavel) != line.npos)
	{
		//cout << "Encontrou esse argumento na função interna a macro" << endl;
		//cout << variavel << endl;
		//cout << variavel_macro << endl;
		//cout << line << endl;
		line.replace(line.find(variavel), variavel.size(), variavel_macro);
		//cout << "Aqui está: " << line << endl;
	}

	return line;
}

int verifica_argumento_macro(string saida, string argumento)
{
	//cout << "Vamos analisar se essa linha contém uma macro....." << endl;
	int numero = 0;
	//ofstream mntfile("MNT", ios::app);
	//ofstream mdtfile("MDT", ios::app);

	ifstream meuarquivo(saida);
	string nome, line, comparar;
	nome = saida.substr(0, saida.size() - 4);
	ofstream menosm(nome.append(".mcr"), ios::app);
	//cout << nome << endl;

	string linha, mntbusca, mdtbusca, token, num_de_args, linha_aux, macro_achada, linha_aux_macro;
	int i, n = 0, m = 0, l = 0, contador = 0, flag, bandeira;
	string termina = "ENDMACRO";

	linha = argumento;

	for (i = 0; argumento[i] != '\0'; i++)
	{
		if (argumento[i] == ':')
		{
			n = n + 1;
		}
	}

	if (n == 1)
	{
		size_t doispontos = argumento.find(":");
		argumento = argumento.substr(doispontos + 2, argumento.size());
	}
	else if (n == 0)
	{
		//cout << argumento << endl;
	}
	argumento = argumento.substr(0, argumento.find(" "));
	//aqui começa a checar na tabela MNT se existe a macro

	ifstream mntfile("MNT");

	if (mntfile.is_open())
	{
		while (getline(mntfile, mntbusca))
		{

			token = argumento;
			size_t postab = mntbusca.find("\t");
			//cout << "Token aqui: " << token << endl;
			//cout << "mntbusca: " << mntbusca << endl;
			mdtbusca = mntbusca.substr(0, postab);
			//cout << "mdtbusca: " << mdtbusca << endl;
			//cout << mdtbusca.compare(token) << endl;
			//Se achou um possível membro das macros então pode expandir
			//analisa se tem mais de um argumento, se sim resolve eles aqui... caso contrário deixa como tava.

			if (mdtbusca.compare(token) == 0)
			{
				//agora é necessário descobrir quantos argumentos essa função tem
				cout << "Para descobrir a quantidade de argumentos é necessário usar a linha: " << mntbusca << endl;
				num_de_args = mntbusca.substr(mntbusca.find("\t") + 1, mntbusca.size());
				num_de_args = num_de_args.substr(0, 1);
				cout << num_de_args << endl;
				//assim eu tenho a quantidade de argumentos mínima que eu tenho que ter na chamada dessa linha
				n = 0;
				l = 0;
				//cout << linha << endl;
				linha = linha.substr(linha.find(" ") + 1, linha.size());
				for (i = 0; linha[i] != '\0'; i++)
				{
					if (linha[i] == ',')
					{
						n = n + 1;
					}
				}

				for (i = 0; linha[i] != '\0'; i++)
				{
					if (linha[i] == ' ')
					{
						l = l + 1;
					}
				}

				if (l > 0)
				{
					cout << "Os argumentos estão separados por espaço, favor retirar os espaços entre os argumentos...\nDefina os argumentos separado apenas por vígulas..." << endl;
					return 5;
				}

				//cout << n << "\t" << l << endl;

				if (n == 0)
				{
					if (num_de_args == "1")
					{
						numero = 1;
						//cout << "Acertou a quantidade de argumentos 1 ................................." << endl;
						//cout << "Aqui vamos repassar pelo arquivo para pegar a definição da macro e trocar os argumentos por números..." << endl;
						flag = 0;
						bandeira = 0;
						//cout << line << endl;
						//cout << line.substr(line.find("")+1,line.size()) << endl;
						//cout << token << ": MACRO" << endl;
						do
						{
							getline(meuarquivo, line);
							string busca = token + ": MACRO ";
							if (strstr(line.c_str(), busca.c_str()))
							{
								//cout << "entrou aqui" << endl;
								bandeira = 1;
							}
							if (meuarquivo.eof())
							{
								bandeira = 1;
							}
							//getline(meufile, line);
							//cout << line << endl;

						} while (bandeira == 0);
						//cout << "saiu" << endl;
						//cout << line << endl;
						while (line != termina)
						{
							//cout << line << endl;
							//se encontrar os : então entra no laço a procura pela macro
							size_t poscom = line.find("MACRO");
							if (poscom != line.npos)
							{
								comparar = line.substr(0, line.find(":"));
								if (comparar == mdtbusca)
								{
									//cout << line.substr(line.find("MACRO") + 6, line.size()) << endl;
									macro_achada = line.substr(line.find("MACRO") + 6, line.size());
									//cout << "encontrou a definição da macro desejada" << endl;
									flag = 1;
								}
							}
							else if (flag == 1)
							{
								//aqui nessa parte tem que ser feito a análise de cada argumento da macro
								//no caso são 4 argumentos, então que procurar em cada linha os argumentos que já temos
								//cout << line << endl;
								//cout << macro_achada << endl;
								//cout << linha << endl;
								linha_aux = linha;
								linha_aux_macro = macro_achada;

								string linha_retornada = troca_a_linha_macro(line, linha_aux, linha_aux_macro);

								//cout << "Retorno da Função: " << linha_retornada << endl;

								//aqui escreve no arquivo .MCR
								if (menosm.is_open())
								{
									menosm << linha_retornada << endl;
								}
								else
									cout << "Nao foi possivel abrir o arquivo .mcr! " << endl;

								//aqui passamos a linha, o argumento que deve ser mudado e o que vai substituir para uma função
								//essa função recebe a linha e analisa se tem que fazer a troca
								//se tiver que fazer a troca já troca e retorna a linha com o valor trocado
								//ao final de tudo escreve em a função no arquivo .mcr

								//uma vez que eu tenho cada um dos argumentos eu posso considerar que eles serão substituídos pelos originais da macro
								//mas antes disso é preciso relacionar cada argumento ao respctivo argumento

								//compara cada uma das variáveis das macros para então fazer a substituição
							}
							getline(meuarquivo, line);
						}
					}
					else
					{
						cout << "Erro na quantidade de argumentos 1" << endl;
					}
				}
				else if (n == 1)
				{
					if (num_de_args == "2")
					{
						numero = 2;
						//cout << "Acertou a quantidade de argumentos 2 ................................." << endl;
						//cout << "Aqui vamos repassar pelo arquivo para pegar a definição da macro e trocar os argumentos por números..." << endl;
						flag = 0;
						bandeira = 0;
						//cout << line << endl;
						//cout << line.substr(line.find("")+1,line.size()) << endl;
						//cout << token << ": MACRO" << endl;
						do
						{
							getline(meuarquivo, line);
							string busca = token + ": MACRO ";
							if (strstr(line.c_str(), busca.c_str()))
							{
								//cout << "entrou aqui" << endl;
								bandeira = 1;
							}
							if (meuarquivo.eof())
							{
								bandeira = 1;
							}
							//getline(meufile, line);
							//cout << line << endl;

						} while (bandeira == 0);

						//cout << line << endl;
						//cout << line.substr(line.find("")+1,line.size()) << endl;
						//cout << token << endl;
						while (line != termina)
						{
							//cout << line << endl;
							//se encontrar os : então entra no laço a procura pela macro
							size_t poscom = line.find("MACRO");
							if (poscom != line.npos)
							{
								comparar = line.substr(0, line.find(":"));
								if (comparar == mdtbusca)
								{
									//cout << line.substr(line.find("MACRO") + 6, line.size()) << endl;
									macro_achada = line.substr(line.find("MACRO") + 6, line.size());
									//cout << "encontrou a definição da macro desejada" << endl;
									flag = 1;
								}
							}
							else if (flag == 1)
							{
								//aqui nessa parte tem que ser feito a análise de cada argumento da macro
								//no caso são 4 argumentos, então que procurar em cada linha os argumentos que já temos
								//cout << line << endl;
								//cout << macro_achada << endl;
								//cout << linha << endl;
								linha_aux = linha;
								linha_aux_macro = macro_achada;

								string linha_retornada = troca_a_linha_macro(line, linha_aux, linha_aux_macro);

								//cout << "Retorno da Função: " << linha_retornada << endl;

								//aqui escreve no arquivo .MCR
								if (menosm.is_open())
								{
									menosm << linha_retornada << endl;
								}
								else
									cout << "Nao foi possivel abrir o arquivo .mcr! " << endl;

								//aqui passamos a linha, o argumento que deve ser mudado e o que vai substituir para uma função
								//essa função recebe a linha e analisa se tem que fazer a troca
								//se tiver que fazer a troca já troca e retorna a linha com o valor trocado
								//ao final de tudo escreve em a função no arquivo .mcr

								//uma vez que eu tenho cada um dos argumentos eu posso considerar que eles serão substituídos pelos originais da macro
								//mas antes disso é preciso relacionar cada argumento ao respctivo argumento

								//compara cada uma das variáveis das macros para então fazer a substituição
							}
							getline(meuarquivo, line);
						}
					}
					else
					{
						cout << "Erro na quantidade de argumentos 2" << endl;
					}
				}
				else if (n == 2)
				{
					if (num_de_args == "3")
					{
						numero = 3;
						//cout << "Acertou a quantidade de argumentos 3 ................................." << endl;
						//cout << "Aqui vamos repassar pelo arquivo para pegar a definição da macro e trocar os argumentos por números..." << endl;
						flag = 0;

						bandeira = 0;
						//cout << line << endl;
						//cout << line.substr(line.find("")+1,line.size()) << endl;
						//cout << token << ": MACRO" << endl;
						do
						{
							getline(meuarquivo, line);
							string busca = token + ": MACRO ";
							if (strstr(line.c_str(), busca.c_str()))
							{
								//cout << "entrou aqui" << endl;
								bandeira = 1;
							}
							if (meuarquivo.eof())
							{
								bandeira = 1;
							}
							//getline(meufile, line);
							//cout << line << endl;

						} while (bandeira == 0);

						while (line != termina)
						{
							//cout << line << endl;
							//se encontrar os : então entra no laço a procura pela macro
							size_t poscom = line.find("MACRO");
							if (poscom != line.npos)
							{
								comparar = line.substr(0, line.find(":"));
								if (comparar == mdtbusca)
								{
									//cout << line.substr(line.find("MACRO") + 6, line.size()) << endl;
									macro_achada = line.substr(line.find("MACRO") + 6, line.size());
									//cout << "encontrou a definição da macro desejada" << endl;
									flag = 1;
								}
							}
							else if (flag == 1)
							{
								//aqui nessa parte tem que ser feito a análise de cada argumento da macro
								//no caso são 4 argumentos, então que procurar em cada linha os argumentos que já temos
								//cout << line << endl;
								//cout << macro_achada << endl;
								//cout << linha << endl;
								linha_aux = linha;
								linha_aux_macro = macro_achada;

								string linha_retornada = troca_a_linha_macro(line, linha_aux, linha_aux_macro);

								//cout << "Retorno da Função: " << linha_retornada << endl;

								//aqui escreve no arquivo .MCR
								if (menosm.is_open())
								{
									menosm << linha_retornada << endl;
								}
								else
									cout << "Nao foi possivel abrir o arquivo .mcr! " << endl;

								//aqui passamos a linha, o argumento que deve ser mudado e o que vai substituir para uma função
								//essa função recebe a linha e analisa se tem que fazer a troca
								//se tiver que fazer a troca já troca e retorna a linha com o valor trocado
								//ao final de tudo escreve em a função no arquivo .mcr

								//uma vez que eu tenho cada um dos argumentos eu posso considerar que eles serão substituídos pelos originais da macro
								//mas antes disso é preciso relacionar cada argumento ao respctivo argumento

								//compara cada uma das variáveis das macros para então fazer a substituição
							}
							getline(meuarquivo, line);
						}
					}
					else
					{
						cout << "Erro na quantidade de argumentos 3" << endl;
					}
				}
				else if (n == 3)
				{
					if (num_de_args == "4")
					{
						numero = 4;
						//cout << "Acertou a quantidade de argumentos 4 ................................." << endl;
						//cout << "Aqui vamos repassar pelo arquivo para pegar a definição da macro e trocar os argumentos por números..." << endl;
						flag = 0;
						//cout << line << endl;
						//cout << line.substr(line.find("")+1,line.size()) << endl;
						//cout << token << endl;
						bandeira = 0;
						//cout << line << endl;
						//cout << line.substr(line.find("")+1,line.size()) << endl;
						//cout << token << ": MACRO" << endl;
						do
						{
							getline(meuarquivo, line);
							string busca = token + ": MACRO ";
							if (strstr(line.c_str(), busca.c_str()))
							{
								//cout << "entrou aqui" << endl;
								bandeira = 1;
							}
							if (meuarquivo.eof())
							{
								bandeira = 1;
							}
							//getline(meufile, line);
							//cout << line << endl;

						} while (bandeira == 0);

						while (line != termina)
						{
							//cout << line << endl;
							//se encontrar os : então entra no laço a procura pela macro
							size_t poscom = line.find("MACRO");
							if (poscom != line.npos)
							{
								comparar = line.substr(0, line.find(":"));
								if (comparar == mdtbusca)
								{
									//cout << line.substr(line.find("MACRO") + 6, line.size()) << endl;
									macro_achada = line.substr(line.find("MACRO") + 6, line.size());
									//cout << "encontrou a definição da macro desejada" << endl;
									flag = 1;
								}
							}
							else if (flag == 1)
							{
								//aqui nessa parte tem que ser feito a análise de cada argumento da macro
								//no caso são 4 argumentos, então que procurar em cada linha os argumentos que já temos
								//cout << line << endl;
								//cout << macro_achada << endl;
								//cout << linha << endl;
								linha_aux = linha;
								linha_aux_macro = macro_achada;

								string linha_retornada = troca_a_linha_macro(line, linha_aux, linha_aux_macro);

								//cout << "Retorno da Função: " << linha_retornada << endl;

								//aqui escreve no arquivo .MCR
								if (menosm.is_open())
								{
									menosm << linha_retornada << endl;
								}
								else
									cout << "Nao foi possivel abrir o arquivo .mcr! " << endl;

								//aqui passamos a linha, o argumento que deve ser mudado e o que vai substituir para uma função
								//essa função recebe a linha e analisa se tem que fazer a troca
								//se tiver que fazer a troca já troca e retorna a linha com o valor trocado
								//ao final de tudo escreve em a função no arquivo .mcr

								//uma vez que eu tenho cada um dos argumentos eu posso considerar que eles serão substituídos pelos originais da macro
								//mas antes disso é preciso relacionar cada argumento ao respctivo argumento

								//compara cada uma das variáveis das macros para então fazer a substituição
							}
							getline(meuarquivo, line);
						}
						// Abre o arquivo .pre e procura pela tag de macro: e então depois compara se é a macro que procuramos
						// cria um arquivo auxiliar para com o nome MDT_"nome da macro"
						// Edita esse arquivo toda vez que
					}
					else
					{
						cout << "Erro na quantidade de argumentos 4" << endl;
					}
				}
			}
		}
		//mdtfile.close();
		mntfile.close();
		menosm.close();
		meuarquivo.close();
		contador = 0;
	}
	else
		cout << "Nao foi possivel abrir o arquivo MNT para expandir macros!" << endl;

	return numero;
}

void expande_macro(char *file_name)
{
	string salto_um = ":\0", salto_dois = ": \0";
	cout << "Começando a fazer a expansão de macro do arquivo: ";
	string nome;
	cout << file_name << ".pre" << endl;
	nome = file_name;
	nome = nome.substr(0, nome.size());
	//cout << nome;

	string line, linha_aux, linha_auxdois, nomedamacro, nomeparam, valorparam, nomeequ, valorequ, token, mntbusca, mdtbusca, mdtline, linhabusca, comparando, linhafimacro, argumentos, auxiliar, quantidade_de_argumentos;

	string saida = nome.substr(0, nome.size()) + ".pre";

	ifstream meufile(saida);
	ofstream mntfile("MNT", ios::app);
	ofstream mdtfile("MDT", ios::app);
	ofstream menosm(nome.append(".mcr"), ios::app);

	string termina = "ENDMACRO";
	string SECTIONTEXT = "SECTION TEXT";
	string SECTIONDATA = "SECTION DATA";
	int marca_section = 0, marca_sectiondata = 0;

	int linhamdt = 0, tam, contador = 0, compara = 0, linhafim, inicio_argumentos = 0, num_argumentos = 0;

	if (meufile.is_open())
	{

		//cout << "ta aqui?" << endl;
		while (getline(meufile, line))
		{
			size_t posicao = line.find(":");
			if (posicao != line.npos)
			{
				linha_auxdois = line;
				linha_aux = line;
				linha_aux = linha_aux.substr(linha_aux.find(":"), linha_aux.size());

				if (linha_aux == salto_um)
				{
					cout << "Achou o problema" << endl;
					getline(meufile, line);
					cout << linha_aux << " " << line << endl;
					line = linha_auxdois + " " + line;
				}
				else if (linha_aux == salto_dois)
				{
					cout << "Achou o problema 2" << endl;
					getline(meufile, line);
					cout << linha_aux << " " << line << endl;
					line = linha_aux + " " + line;
				}
			}
			if (strstr(line.c_str(), SECTIONTEXT.c_str()))
			{
				marca_section = 1;
			}
			if (strstr(line.c_str(), SECTIONDATA.c_str()))
			{
				marca_sectiondata = 1;
			}
			size_t poscom = line.find("MACRO");
			if (poscom != line.npos)
			{
				if (marca_section != 1)
				{
					cout << "MACRO definida fora da secção text" << endl;
				}
				if (marca_sectiondata == 1)
				{
					cout << "MACRO definida dentro da secção data" << endl;
				}
				//cout << "\nTem uma MACRO aqui \n\n";
				//salva aqui o nome das MACROS definidas no código
				//Chama uma rotina para salvar em um arquivo o nome da macro, a quantidade de parametros e a linha que será adicionada na MDT
				//Pega a linha em que foi achada a macro, e encontra o nome que vem antes dos ':'
				poscom = line.find(":");
				//cout << "Mostrando a posição na linha que o nome macro está: " << line.find("MACRO") << endl;
				inicio_argumentos = line.find("MACRO") + 5;
				//cout << "Mostrando o inicio dos argumentos da macro: " << inicio_argumentos << endl;
				nomedamacro = line.substr(0, poscom);
				//cout << nomedamacro << endl;
				//cout << "Mostrando a quantidade de argumentos da MACRO:" << line.substr(inicio_argumentos, (line.size() - inicio_argumentos)) << endl;
				argumentos = line.substr(inicio_argumentos, (line.size() - inicio_argumentos));
				num_argumentos = 0;
				//aqui chama uma função para contar a quantidade de argumentos da macro 1,2,3 ou 4
				if (argumentos.size() > 0)
				{
					num_argumentos = leargumentos(argumentos);
					//cout << num_argumentos << endl;
				}

				//inicialização da variável que diz qual linha estará o escopo da macro
				//cout << nomedamacro << endl;
				if (mntfile.is_open())
				{
					mntfile << nomedamacro << "\t" << num_argumentos << "\t" << linhamdt << endl; //o nome desse arquivo é MNT(Macro Name Table)
					//cout << "Formato de argumentos que a macro aceita:" << line.substr(inicio_argumentos,(line.size()-inicio_argumentos)) << endl;
					//caso o valor dos argumentos não esteja correto então mostra um erro
					getline(meufile, line);

					do
					{
						mdtfile << line << endl;
						getline(meufile, line);
						//cout << line << endl;
						comparando = line;
						//cout << "Comparacao: " << comparando.compare(termina) << endl;
						linhamdt++;
					} while ((comparando.compare(termina) != 0));

					//cout << "\n      finalizou a MACRO      \n" << endl;
				}
				else
					cout << "\nArquivo nao pode ser aberto!!!\n\n";
				//getline(meufile, line);
				//Chama uma rotina para salvar em uma tabela o código da macro até o valor ENDMACRO
				//o nome desse arquivo é MDT(Macro Definition Table)
				//strcmp(pesq, agen[contador].nome) == 0
				//cout << line << endl;
			}
			else
			{
				// Se for uma chamada de macro, aqui ela sera expandida
				size_t posdelimit = line.find(" ");
				// Se nao tiver espacos, ou eh diretiva ou eh macro.
				// Testa aqui se eh macro (pela mnt)
				// Se nao for, tem que ser diretiva (verificar durante a montagem)
				//cout << "Teste 1" << endl;
				//cout << posdelimit << endl;
				//cout << line.npos << endl;
				if (posdelimit == line.npos)
				{
					ifstream mntfile("MNT");
					if (mntfile.is_open())
					{
						while (getline(mntfile, mntbusca))
						{
							token = line;
							size_t postab = mntbusca.find("\t");
							cout << "Token: " << token << endl;
							cout << "mntbusca: " << mntbusca << endl;
							mdtbusca = mntbusca.substr(0, postab);
							//cout << "mdtbusca: " << mdtbusca << endl;
							//cout << mdtbusca.compare(token) << endl;

							//Aqui tem que consultar a quantidade de argumentos para ver se é maior que 0, para então chamar uma rotina alternativa

							// Se entrar nesse if, achou o nome da macro na mnt
							// Vai agora entrar no if e ir na mdt.

							// Procura as macros, ainda nao expande mas ta quase la

							if (mdtbusca.compare(token) == 0)
							{
								ifstream mdtfile("MDT");
								auxiliar = mntbusca.substr(postab + 1, mntbusca.size());
								postab = auxiliar.find("\t");
								quantidade_de_argumentos = auxiliar.substr(0, postab);
								//cout << "Quantidade de argumentos:" << auxiliar.substr(0, postab) << endl;
								//cout << "Print a linha:" << line << endl;

								int ia = 0, na = 0, la = 0;
								for (ia = 0; line[ia] != '\0'; ia++)
								{
									if (line[ia] == ',')
									{
										na = na + 1;
									}
								}

								for (ia = 0; line[ia] != '\0'; ia++)
								{
									if (line[ia] == ' ')
									{
										la = la + 1;
									}
								}

								if (la > 0)
								{
									cout << "Os argumentos estão separados por espaço, favor retirar os espaços entre os argumentos...\nDefina os argumentos separado apenas por vígulas..." << endl;
								}

								if (na == 0)
								{
									if ((quantidade_de_argumentos == "0") && (la == 0))
									{
										cout << "Chamada de MACRO correta" << endl;
									}
									else if ((la == 1) && (quantidade_de_argumentos == "1"))
									{
										cout << "Chamada de MACRO correta" << endl;
									}
									else if ((quantidade_de_argumentos != "1") || (la < 1))
									{
										cout << "Erro na quantidade de argumentos.... Programa continuará mas o seu código '.mcr' terá erros" << endl;
									}
								}
								else if (na == 1)
								{
									if (quantidade_de_argumentos != "2")
									{
										cout << "Erro na quantidade de argumentos.... Programa continuará mas o seu código '.mcr' terá erros" << endl;
									}
								}
								else if (na == 2)
								{
									if (quantidade_de_argumentos != "3")
									{
										cout << "Erro na quantidade de argumentos.... Programa continuará mas o seu código '.mcr' terá erros" << endl;
									}
								}
								else if (na == 3)
								{
									if (quantidade_de_argumentos != "4")
									{
										cout << "Erro na quantidade de argumentos.... Programa continuará mas o seu código '.mcr' terá erros" << endl;
									}
								}

								if (getline(mntfile, linhafimacro))
								{
									int tam2 = linhafimacro.size();
									linhafimacro = linhafimacro.substr(tam2 - 1, tam2);
									//cout << "linha fim da macro:" << linhafimacro << endl;
									linhafim = stoi(linhafimacro);
								}

								else
								{
									linhafim = 100000;
								}

								tam = mntbusca.size();
								linhabusca = mntbusca.substr(tam - 1, tam);
								compara = stoi(linhabusca);
								//cout << linhabusca << endl;
								//cout << compara << endl;
								//cout << "linha fim : " << linhafim << endl;

								if (mdtfile.is_open())
								{

									while ((getline(mdtfile, mdtline)) && compara != contador)
									{
										contador++;
										//cout << "compara: " << compara << endl;
										//cout << "contador: " << contador << endl;
										//cout << "linha da mdt: " << mdtline << endl;
									}

									// o arquivo MDT esta exatamente na linha da macro chamada
									// linhafim tem o valor da ultima linha que eh pra ser copiada

									do
									{
										//cout << "entrou aqui!" << endl;
										if (menosm.is_open())
										{
											//cout << mdtline << endl;
											menosm << mdtline << endl;
											contador++;
											//cout << "contador: " << contador << endl;
											//cout << "linha fim: " << linhafim << endl;
										}
									} while (getline(mdtfile, mdtline) && contador != linhafim);
								}
								else
								{
									cout << "Nao foi possivel abrir o arquivo MDT para busca" << endl;
								}
							}
						}
						//cout << token << endl;
						//if (token == "STOP")
						//{ // esse if eh so pra colocar o STOP no .mcr. sem isso o stop nao entra.
						//cout << "entrou aqui!" << endl;
						//if (menosm.is_open())
						//{
						//	menosm << token << endl;
						//}
						//}
						mdtfile.close();
						mntfile.close();
						contador = 0;
					}
					else
						cout << "Nao foi possivel abrir o arquivo MNT para expandir macros!" << endl;
				}
				else
				{
					// Tem que chamar uma rotina aqui que olha se a linha é de uma macro com argumentos
					//cout << "Pode ser uma macro com argumentos:" << line << endl;
					//cria uma função que procura pelo rotulo da macro dentro da MNT
					if (verifica_argumento_macro(saida, line) == 0)
					{
						//caso ele não encontre a definição da macro ele continua o código a seguir
						// Se nao for definicao de macro nem chamada de macro, escreve direto no .mcr
						if (menosm.is_open())
						{
							menosm << line << endl;
						}
						else
							cout << "Nao foi possivel abrir o arquivo .mcr! " << endl;
					}
					else
					{
						cout << "Foi encontrado uma MACRO com argumentos e ela foi resolvida" << endl;
					}
				}
			}
			cout << line << endl;
			if (line == "STOP")
			{ // esse if eh so pra colocar o STOP no .mcr. sem isso o stop nao entra.
				//cout << "entrou aqui!" << endl;
				//if (menosm.is_open())
				//{
				if (menosm.is_open())
				{
					//cout << "if do stop" << endl;
					//cout << "line: " << line << endl;
					menosm << line << endl;
				}
				else
					cout << "Nao foi possivel abrir o arquivo .mcr! " << endl;

				//}
			}
		}
		//menosm.close();
	}
	menosm.close();
	mntfile.close();
	mdtfile.close();
	meufile.close();
}

void pre_procesamento(char *file_name, int lineachousection)
{

	cout << "Começando a fazer o pre processamento do arquivo: ";
	string nome;
	cout << file_name << ".asm" << endl;
	nome = file_name;
	nome = nome.substr(0, nome.size());
	//nome.append(".pre");
	//cout << nome;

	string line, nomedamacro, nomeparam, valorparam, nomeequ, valorequ;
	int fim, posequ, fimequ, remover, teste, erroequ = 0, marcador = 0;
	ifstream meufile("auxiliar");
	ofstream equfile("EQU", ios::app);
	ofstream saidafile(nome.append(".pre"), ios::app);

	if (meufile.is_open())
	{
		int linecounter = 0;
		while (getline(meufile, line))
		{

			if (line.find("SECTION TEXT") != line.npos)
			{
				marcador = 1;
			}

			linecounter++;
			if (line.find("EQU") != line.npos)
			{
				if (marcador == 1)
				{
					cout << "ERRO, EQU está definida dentro da SECTION TEXT" << endl;
				}
				//cout << "\nTem um EQU aqui\n";
				fim = line.size();

				valorparam = line[fim - 1];

				posequ = line.find(":");
				nomeparam = line.substr(0, posequ);

				//salva na tabela o nome e o valor do parametro
				if (equfile.is_open())
				{
					equfile << nomeparam + "\t";
					equfile << valorparam << endl;
				}
				else
					cout << "\nArquivo nao pode ser aberto EQU!!!\n\n";

				//Verifica se EQU vem depois de SECTION
				if (linecounter >= lineachousection)
				{
					//cout << "\nDiretiva EQU encontrada após a diretiva SECTION na linha " << linecounter << endl;
				}
			}
			else if (line.find("IF") != line.npos)
			{
				//cout << "\nTem um IF aqui\n";
				fim = line.size();
				size_t posif = line.find("IF");
				nomeparam = line.substr(posif + 3, fim);
				teste = nomeparam.size();
				nomeparam = line.substr(3, teste);
				//cout << line << endl;
				//cout << nomeparam << endl;
				//cout << nomeparam.size() << endl;
				ifstream equfile("EQU");
				if (equfile.is_open())
				{
					while (getline(equfile, line))
					{
						//le a linha ate o espaço depois disso
						posequ = line.find("\t");
						//depois pega o valor até o espaço e compara com o if que queremos
						nomeequ = line.substr(0, posequ);
						fimequ = line.size();
						valorequ = line[fimequ - 1];
						//line.substr(posequ,fim);
						//cout << nomeequ.size() << endl;
						//cout << valorequ.size() << endl;
						//olha na tabela de EQU procurando o valor que está logo após o IF(0 ou 1)
						if (nomeequ.compare(nomeparam) == 0)
						{
							//parametro existe na tabela e podemos verificar o seu valor
							//cout << nomeequ << endl;
							//cout << "aqui" <<endl;
							//cout << valorequ << endl;
							//verifica o valor de nomeequ
							if (valorequ == "1")
							{
								remover = 0;
								erroequ = 1;
								getline(meufile, line);
								saidafile << line << endl;
							}
							else if (valorequ == "0")
							{
								remover = 1;
								erroequ = 1;
								//cout << "Remover: " << remover << endl;
								getline(meufile, line);
							}
							//se for 1 adiciona a linha posterior ao if
							//se for 0 remove a linha posterior ao if
						}
					}
					if (erroequ == 0)
					{ //Retornando o erro para a resolução de EQU não válida
						//cout << "Valor de EQU válido para " << nomeparam << " não foi encontrado no arquivo, revise o código .asm." << endl;
						erroequ = 0;
					}
				}
				else
					cout << "\nArquivo EQU nao pode ser aberto!!!\n\n";
			}
			else
			{
				saidafile << line << endl;
			}
		}
		equfile.close();
		meufile.close();
		saidafile.close();
	}
}

int retorna_decimal(string hexadecimal)
{
	int x = (int)strtol(hexadecimal.c_str(), 0, 16);
	return x;
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

void lexer(std::vector<std::string> token_vector, int n_linha)
{
	for (vector<string>::iterator it = token_vector.begin(); it != token_vector.end(); ++it)
	{
		if ((*it).size() < 21)
		{
			string str = *it;
			if (isalpha(str.at(0)) || (str.at(0) == '_'))
			{
				for (string::iterator it3 = str.begin(); it3 != str.end(); ++it3)
				{
					if (!isdigit(*it3) && !isalpha(*it3) && !((*it3) == '_'))
					{
						if ((*it3) == ':')
						{
							//se for primeira string e ':' no final, então ok. Se não for isso erro
							if (!((it == token_vector.begin()) && ((*it).back() == (*it3))))
								printf("Erro léxico! \n Token inválido. Token deve ser composto por dígitos, letras ou underscore. \n Linha: %d.", n_linha);
						}
						else
						{
							printf("Erro léxico! \n Token inválido. Token deve ser composto por dígitos, letras ou underscore. \n Linha: %d.", n_linha);
							break;
						}
					}
				}
			}
			else
			{
				for (string::iterator it3 = str.begin(); it3 != str.end(); ++it3)
				{
					if (!isdigit(*it3))
					{
						if ((*it).find("0X") != 0)
						{
							printf("Erro léxico! \n Token inválido. Token deve ser iniciado por dígito ou underscore. \n Linha: %d.", n_linha);
							cout << "\n\nFoi aqui:  " << *it;
						}
						else
							break;
					}
				}
			}
		}
		else
			printf("Erro léxico! \n Token inválido. Token deve ter no máximo 20 caracteres. \n Linha: %d.", n_linha);
	}
}

void definir_label(string str, int n_address)
{
	tabela_simbolo temp;
	temp.simbolo = str;
	temp.valor = n_address;
	temp.is_const = 0;
	temp.valor_const = -1;
	tabela_simbolo_vector.push_back(temp);
}

void primeira_passagem(string file_in)
{
	cout << "Começando a fazer a primeira passagem no arquivo: ";
	cout << file_in << endl;

	//*******PRIMEIRA PASSAGEM*******
	std::ifstream infile(file_in);
	std::string line;
	string str;

	int n_linha = 1; //número da linha do programa
	int pc = 0;		 //número do endereço equivalente

	int simbolo_redefinido = 0;
	int found = 0;

	vector<string>::iterator it;

	//Cria arquivo intermediario
	//ofstream ofile("file_inter.txt");
	//While lê arquivo de entrada até o arquivo acabar
	while (std::getline(infile, line))
	{

		//ANÁLISE LÉXICA
		//separa os tokens e faz análise léxica
		vector<string> token_vector = separate_tokens(line);
		lexer(token_vector, n_linha);

		it = token_vector.begin();
		str = *it;
		//VERIFICA SE É LABEL
		found = 0;
		if (str.back() == ':') //procura no fim do primeiro token ':'
		{
			str.erase(std::prev(str.end())); //apaga o ':'
			if (token_vector.size())
			{
				//percorre toda a tabela de simbolos  comparando o token do arquivo com o simbolo definido na tabela
				for (vector<tabela_simbolo>::iterator it_s = tabela_simbolo_vector.begin(); it_s != tabela_simbolo_vector.end(); ++it_s)
					if (!str.compare((*it_s).simbolo)) //ja está definido na tabela
					{
						printf("Erro Semântico! \n Símbolo redefinido. \n Linha: %d \n", n_linha);
						simbolo_redefinido = 1;
					}
				if (!simbolo_redefinido)
				{
					//str.erase(std::prev(str.end()));
					definir_label(str, pc); //inclui arquivo na tabela
				}
			}
			if (token_vector.size() > 1) //evitar seg fault
				++it;					 //pega o proximo token da linha do arquivo
			str = *it;

			if (!str.compare("CONST"))
			{
				it--;
				for (vector<tabela_simbolo>::iterator it_s = tabela_simbolo_vector.begin(); it_s != tabela_simbolo_vector.end(); ++it_s)
				{

					str = *it;
					str.erase(std::prev(str.end()));
					if (!str.compare((*it_s).simbolo))
					{
						(*it_s).is_const = 1;
						it++;
						it++;

						if ((*it).find("0X") == 0)
							(*it_s).valor_const = retorna_decimal(*it);
						else
							(*it_s).valor_const = stoi(*it);

						it--;
						it--;
						break;
					}
				}
				it++;
				str = *it;
			}
		}
		//VERIFICA SE É INSTRUÇÃO
		for (vector<tabela_instrucao>::iterator it_i = tabela_instrucao_vector.begin(); it_i != tabela_instrucao_vector.end(); ++it_i)
		{
			if (!str.compare((*it_i).mnemonico))
			{ //se for uma instruçao ele atualiza o valor do PC e diz que ja encontrou, pra nao precisar procurar nas diretivas
				pc = pc + (*it_i).n_operando + 1;
				found = 1;
			}
		}
		//VERIFICA SE É DIRETIVA
		if (!found)
		{
			if (!str.compare("CONST"))
			{
				++pc;
			}
			else
			{
				if (!str.compare("SPACE"))
				{
					it++;
					if (it != token_vector.end())
					{ //atualiza pc de acordo com o que foi especificado na linah do arquivo
						pc = pc + stoi(*it);
						//cout << *it << endl;
						//cout << stoi(*it) << endl;
					}
					else //soma so um mesmo
						pc++;
				}
				else
				{
					if (!str.compare("SECTION"))
					{
						pc = pc;
						++it;
						str = *it;
						if (it != token_vector.end())
						{
							if (!str.compare("DATA"))
								data = n_linha;
							data_pc = pc;
						}
					}
					else
					{
						printf("Erro! \n Símbolo não definido. \n Linha: %d \n", n_linha);
						cout << "String: " << str << "\n\n\n";
					}
				}
			}
		}
		++n_linha;
	}
}

int procura_simbolo(vector<string>::iterator it)
{ //ße existir um tabela de simbolos, percorre ela toda procurando pelo simbolo. retorna -1 caso nao encontre na tabela
	if (tabela_simbolo_vector.size())
	{
		for (vector<tabela_simbolo>::iterator it_s = tabela_simbolo_vector.begin(); it_s != tabela_simbolo_vector.end(); ++it_s)
		{
			if (!(*it).compare((*it_s).simbolo))
			{
				return (*it_s).valor;
			}
		}
		return -1;
	}
	else
		return -1;
}

int procura_simbolo_const(vector<string>::iterator it)
{ //ße existir um tabela de simbolos, percorre ela toda procurando pelo simbolo. retorna -1 caso nao encontre na tabela
	if (tabela_simbolo_vector.size())
	{
		for (vector<tabela_simbolo>::iterator it_s = tabela_simbolo_vector.begin(); it_s != tabela_simbolo_vector.end(); ++it_s)
		{
			if (!(*it).compare((*it_s).simbolo))
			{
				return (*it_s).is_const;
			}
		}
		return -1;
	}
	else
		return -1;
}
int procura_simbolo_valor_const(vector<string>::iterator it)
{ //ße existir um tabela de simbolos, percorre ela toda procurando pelo simbolo. retorna -1 caso nao encontre na tabela
	if (tabela_simbolo_vector.size())
	{
		for (vector<tabela_simbolo>::iterator it_s = tabela_simbolo_vector.begin(); it_s != tabela_simbolo_vector.end(); ++it_s)
		{
			if (!(*it).compare((*it_s).simbolo))
			{
				return (*it_s).valor_const;
			}
		}
		return -1;
	}
	else
		return -1;
}

void segunda_passagem(string file_in, string file_out)
{
	cout << "Começando a fazer a segunda passagem no arquivo: ";
	cout << file_in << endl;

	//*******PRIMEIRA PASSAGEM*******
	std::ifstream infile(file_in);
	std::string line;
	string str;

	int n_linha = 1; //número da linha do programa
	int pc = 0;		 //número do endereço equivalente

	int found = 0;
	int symbol_value;

	vector<string>::iterator it;
	vector<string>::iterator it_end;

	vector<string> aux;
	//Cria arquivo intermediario
	ofstream ofile(file_out);
	//While lê arquivo de entrada até o arquivo acabar
	while (std::getline(infile, line))
	{

		//ANÁLISE LÉXICA
		vector<string> token_vector = separate_tokens(line);
		//lexer(token_vector, n_linha);

		it = token_vector.begin();
		it_end = token_vector.end();
		str = *it;
		//VERIFICA SE É LABEL
		if (str.back() == ':')
		{
			if (token_vector.size() > 1) //pega o proximo token
				++it;
			str = *it;
		}
		//VERIFICA SE É INSTRUÇÃO
		for (vector<tabela_instrucao>::iterator it_i = tabela_instrucao_vector.begin(); it_i != tabela_instrucao_vector.end(); ++it_i)
		{
			if (!str.compare((*it_i).mnemonico))
			{
				if ((!str.compare("JMP")) || (!str.compare("JMPZ")) || (!str.compare("JMPP")) || (!str.compare("JMPN")))
				{
					it++;
					if ((data_pc > -1) && (procura_simbolo(it) >= data_pc))
					{
						printf("Erro! \n Pulo para sessão inválida. \n Linha: %d \n", n_linha);
					}
					it--;
				}
				if ((!str.compare("STORE")) || (!str.compare("INPUT")))
				{
					it++;
					if (procura_simbolo_const(it))
						printf("Erro! \n Modificação de um valor constante. \n Linha: %d \n", n_linha);
					it--;
				}
				if (!str.compare("DIV"))
				{
					it++;
					if (procura_simbolo_const(it))
					{
						if (procura_simbolo_valor_const(it) == 0)
							printf("Erro! \n Divisão por constante igual a 0. \n Linha: %d \n", n_linha);
					}
					it--;
				}
				if (distance(it, it_end) != ((*it_i).n_operando + 1))
				{
					aux.push_back((*it_i).opcode);
					printf("Erro! \n Número de operandos da instrução errado. \n Linha: %d \n", n_linha);
					if (distance(it, it_end) > ((*it_i).n_operando + 1))
					{
						//tem mais operandos do que precisa

						//Laço pegando proximos tokens i vezes, em que i é o # de operandos da instrução.
						//STOP nem entra no laço (0 operandos)
						//COPY entra no laço 2 vezes
						//os demais entram no laço 1 vez.
						for (int i = 0; i < (*it_i).n_operando; i++)
						{
							it++; //pega proximo token
							symbol_value = procura_simbolo(it);
							if (symbol_value == -1)
							{
								printf("Erro! \n Símbolo não declarado. \n Linha: %d \n", n_linha);
								aux.push_back("ND");
							}
							else
							{
								if ((n_linha <= data) || (data == -1))
									aux.push_back(to_string(symbol_value)); //transforma o valor correspondente do simbolo pra string e coloca no vetor aux
								else
									printf("Erro Sintático! \n Instrução na sessão errada. \n Linha: %d \n", n_linha); //todo corrigir tipo de erro
							}
						}
					}
					else
					{
						for (int i = 0; i < (*it_i).n_operando; i++)
						{
							it++;						  //pega proximo token
							if (it != token_vector.end()) //copy com 1 argumento é o unico que entraria aqui
							{
								symbol_value = procura_simbolo(it);
								if (symbol_value == -1)
								{
									printf("Erro! \n Símbolo não declarado. \n Linha: %d \n", n_linha);
									aux.push_back("ND");
								}
								else
								{
									if ((n_linha <= data) || (data == -1))
										aux.push_back(to_string(symbol_value)); //transforma o valor correspondente do simbolo pra string e coloca no vetor aux
									else
										printf("Erro Sintático! \n Instrução na sessão errada. \n Linha: %d \n", n_linha); //todo corrigir tipo de erro
								}
							}
							else
							{
								aux.push_back("FA");
							}
						}
					}
				}
				else
				{
					aux.push_back((*it_i).opcode); //coloca o opcode no vetor aux
					for (int i = 0; i < (*it_i).n_operando; i++)
					{
						++it;
						symbol_value = procura_simbolo(it);
						if (symbol_value == -1)
						{
							printf("Erro! \n Símbolo não declarado. \n Linha: %d \n", n_linha);
							aux.push_back("ND");
						}
						else
						{
							if ((n_linha <= data) || (data == -1))
								aux.push_back(to_string(symbol_value)); //transforma o valor correspondente do simbolo pra string e coloca no vetor aux
							else
								printf("Erro Sintático! \n Instrução na sessão errada. \n Linha: %d \n", n_linha); //todo corrigir tipo de erro
						}
					}
				}
				found = 1;
			}
		}
		//VERIFICA SE É DIRETIVA
		if (!found)
		{
			if (!str.compare("CONST"))
			{
				if ((n_linha >= data) && (data != -1))
				{
					if (distance(it, it_end) != 2)
					{
						printf("Erro Sintático! \n Quantidade de operandos inválida. \n Linha: %d \n", n_linha);
					}
					else
					{
						++it;
						if ((*it).size() > 1)
						{
							if ((*it).find("0X") == 0)
							{
								aux.push_back(to_string(retorna_decimal(*it)));
							}
							else
								aux.push_back(*it);
						}
						else
							aux.push_back(*it);
					}
				}
				else
					printf("Erro Sintático! \n Diretiva CONST na sessão errada. \n Linha: %d \n", n_linha); //todo corrigir tipo de erro
			}
			else
			{
				if (!str.compare("SPACE"))
				{
					if ((n_linha >= data) && (data != -1))
					{
						++it;
						if (it != token_vector.end()) //verifica se tem algum operando na diretiva space
						{
							for (int i = 0; i < stoi(*it); i++) //loop reservando espaço até alcançar o valor do argumento
								aux.push_back("X");
						}
						else
						{
							aux.push_back("X");
						}
					}
					else
						printf("Erro Sintático! \n Diretiva SPACE na sessão errada. \n Linha: %d \n", n_linha); //todo corrigir tipo de erro
				}
				else
				{
					if (str.compare("SECTION"))
						printf("Erro! \n Instrução ou diretiva não identificada. \n Linha: %d \n", n_linha);
				}
			}
		}
		found = 0;
		//colocar o vetor aux no arquivo final
		for (const auto &e : aux)
			ofile << e << " ";
		//TODO retirar linha abaixo depois
		//ofile << endl;

		++n_linha;
		token_vector.clear();
		aux.clear();
	}
	infile.close();
	ofile.close();
}
void montagem(string filein, string fileout)
{
	inicia_tabela_diretiva();
	inicia_tabela_instrucao();
	primeira_passagem(filein);
	segunda_passagem(filein, fileout);
}

int main(int argc, char *argv[])
{
	//argc eh um inteiro com o numero de argumentos passados pela linha de comando
	//argv eh um vetor com os argumentos. argv[0] sempre sera o path do programa,
	//entao eh basicamente ignorado. por isso, o argc na verdade vai ser o numero
	//de argumentos mais um.

	string file_name;
	string file_in;
	int lineachousection;

	if(argc == 1){
		cout << "ERRO!!! Chamada errada de Programa, deve ter pelo menos 1 arquivo especificado na chamada do sistema..." << endl;
		return 0;
	}

	if ((string(argv[1]) == "-o")||(string(argv[1]) == "-p")||(string(argv[1]) == "-m"))
	{
		if (argc != 4)
		{
			cout << "\nERRO.\nNúmero de argumentos inválidos! " << endl;
			return 0;
		}

		file_name = argv[2]; // passar para learquivo(). eh o nome do arquivo .asm.
		file_in = file_name + ".asm";
		if (!file_exist(file_in))
		{
			cout << "\nERRO.\nArquivo não existe nessa pasta!\n\n";
			return 0;
		}

		//lerarquivo(argv[2],argv[3]);
		lineachousection = lerarquivo(file_in, argv[3]);

		if (string(argv[1]) == "-p")
		{

			pre_procesamento(argv[3], lineachousection);
		}
		else if (string(argv[1]) == "-m")
		{
			pre_procesamento(argv[3], lineachousection);
			expande_macro(argv[3]);
			// realiza a expansão das macros em um arquivo com extensão .mcr
			// pega o arquivo da etapa anterior, le o que está nele e executa a criação da MNT e da MDT
			// depois utiliza o algoritmo de expansão das macros que está na função de passagem zero
			// mostra esse arquivo com as macros expandidas

			//montagem();
			//codigo_objeto();
		}
		else if (string(argv[1]) == "-o")
		{
			pre_procesamento(argv[3], lineachousection);
			expande_macro(argv[3]);
			//FUNÇOES DA MONTAGEM
			string file_ = argv[3];
			file_in = file_ + ".mcr";
			string file_out = file_ + ".o"; //todo trocar pra '.o'
			montagem(file_in, file_out);
			//Realiza a montagem do código depois de expandir as macros
		}

		else
		{
			cout << "\nERRO.\nComando de execução inválido." << endl;
		}
	}else{
		if ((argc > 3)||(argc < 2))
		{
			cout << "\nERRO.\nNúmero de argumentos inválidos! " << endl;
			return 0;
		}

		if(argc == 2){
			cout << "Faz a saída do ligador para 1 arquivo" << endl;
			//Descobrir como fazer 1 arquivo do ligador
		}
		else if(argc == 3){
			
			cout << "Faz a saída do ligador para 2 arquivos" << endl;
			//Descobrir como fazer para 2 arquivos depois de ter feito para 1 arquivo
			file_name = argv[2]; // passar para learquivo(). eh o nome do arquivo .asm.
			file_in = file_name + ".asm";

			cout << argv[1] << endl;
			cout << file_name << endl;
			cout << file_in << endl;
		}

	}

	return 0;
}
