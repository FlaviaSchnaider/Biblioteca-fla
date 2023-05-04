#include <iostream>
#include <string>
#include <time.h>
#include <ctime> // Para conseguir fazer a parte de mostrar a data atual
#include <Windows.h>
using namespace std;

// Trabalho feito por Flávia Schnaider e Helena Becker Piazera
const int MAX_RETIRADOS = 100;

#pragma region Structs

struct Data {
	int dia, mes, ano;
};

struct Autor {
	string nome, sobrenome;
};

struct Editora {
	string nome, local;
};

struct Retirado {
	int id = 0;
	Data retirada, devolucao;
	bool atraso = false;
};

struct Usuario {
	string nome;
	int id = 0;
	Retirado retirados[100];
};

struct Livro {
	string titulo, assunto;
	Autor autor;
	Editora editora;
	int publicacao, id = 0;
	bool disponivel = true;
	Data retirada;
};

struct Revista {
	string titulo, assunto;
	Editora editora;
	Data publicacao;
	int id = 0;
	bool disponivel = true;
};

Data ler_data() {
	Data d;
	do {
		cout << "\tDia: ";
		cin >> d.dia;
		cout << "\tMes: ";
		cin >> d.mes;
		cout << "\tAno: ";
		cin >> d.ano;
		cout << endl;
	} while (d.dia > 31 || d.mes < 1 || d.mes > 12 || d.ano < 1900 || d.ano > 2023);

	return d;
}

void mostrar_data(Data d) {
	if (d.dia < 10) {
		cout << "0";
	}
	cout << d.dia << "/";
	if (d.mes < 10) {
		cout << "0";
	}
	cout << d.mes << "/" << d.ano;
}

int dias_no_mes(int mes, int ano) {
	if (mes == 2) {
		if ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0)) {
			return 29; // ano bissexto
		}
		else {
			return 28; // ano não bissexto
		}
	}
	else if (mes == 4 || mes == 6 || mes == 9 || mes == 11) {
		return 30; // meses com 30 dias
	}
	else {
		return 31; // meses com 31 dias
	}
}

Data data_devolucao(Data data_retirada) {
	Data data_devolucao;
	int dias_no_mes_atual;

	data_devolucao.dia = data_retirada.dia + 7;
	data_devolucao.mes = data_retirada.mes;
	data_devolucao.ano = data_retirada.ano;

	dias_no_mes_atual = dias_no_mes(data_devolucao.mes, data_devolucao.ano);

	if (data_devolucao.dia > dias_no_mes_atual) {
		data_devolucao.dia -= dias_no_mes_atual;
		data_devolucao.mes++;
		if (data_devolucao.mes > 12) {
			data_devolucao.mes = 1;
			data_devolucao.ano++;
		}
	}
	return data_devolucao;
}

Autor ler_autor() {
	Autor autor;
	cout << "Nome do Autor: ";
	getline(cin, autor.nome);

	cout << "Sobrenome do Autor: ";
	getline(cin, autor.sobrenome);

	return autor;
}

Editora ler_editora() {
	Editora editora;
	cout << "Nome da Editora: ";
	getline(cin, editora.nome);

	cout << "Local da Editora: ";
	getline(cin, editora.local);

	return editora;
}

Usuario cadastrar_usuario() {
	Usuario u;
	cin.ignore();
	cout << "\nNome Completo: ";
	getline(cin, u.nome);

	return u;
}

Livro cadastrar_livro() {
	Livro l;
	cin.ignore();
	cout << "\nTitulo: ";
	getline(cin, l.titulo);

	l.autor = ler_autor();
	l.editora = ler_editora();

	cout << "Ano de publicacao: ";
	cin >> l.publicacao;

	while (l.publicacao < 0)
	{
		cout << "Digite um ano valido: ";
		cin >> l.publicacao;
	}

	cin.ignore();
	cout << "Assunto: ";
	getline(cin, l.assunto);

	return l;
}

Revista cadastrar_revista() {
	Revista r;
	cin.ignore();
	cout << "\nTitulo: ";
	getline(cin, r.titulo);

	r.editora = ler_editora();

	do {
		cout << "Mes de publicacao: ";
		cin >> r.publicacao.mes;
	} while (r.publicacao.mes < 1 || r.publicacao.mes > 12);
	do {
		cout << "Ano de publicacao: ";
		cin >> r.publicacao.ano;
	} while (r.publicacao.ano < 0);

	cin.ignore();
	cout << "Assunto: ";
	getline(cin, r.assunto);

	return r;
}

#pragma endregion Structs

#pragma region Funcoes
//Usado para posicionar o cursor em uma determinada posição da tela
void set_cursor(int x = 0, int y = 0) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(handle, pos);
}

//system("color 1"); também funciona vai de 1 a 15
void set_color(int color) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COLORREF cor;
	SetConsoleTextAttribute(handle, color);
}

//funcao para verificar se a escolha do usuario eh um numero valido
bool opcao_invalida(int opcao, int min_opcao, int max_opcao) {
	if (opcao < min_opcao || opcao > max_opcao) { //Define as opções mínimas e máximas de escolha para cada menu e submenu
		set_color(4);
		cout << "\nOpcao invalida. Tente novamente.\n" << endl;
		Sleep(500);
		set_color(7);
		return true;
	}
	else {
		return false;
	}
}

//quantidade de itens retirados por um usuario
int qtd_retiradas(Usuario usuario) {
	int soma = 0;
	for (int j = 0; j < MAX_RETIRADOS; j++) {
		if (usuario.retirados[j].id != 0) { //verifica a quantidade de itens já retirados pelo usuario
			soma++;
		}
	}
	return soma;
}

//verifica se ha atraso 
bool atraso(Usuario usuario) {
	for (int j = 0; j < MAX_RETIRADOS; j++) {
		if (usuario.retirados[j].atraso) { //nao permite retirar mais livros enquanto tiver um com atraso de devolucao
			cout << "\nVoce esta com um livro em atraso" << endl
				<< "Nao podera realizar mais retiradas enquanto nao regularizar as devolucoes" << endl;
			return true;
		}
	}
	return false;
}

void mostrar_data(int dia, int mes, int ano) {
	cout << dia << "/" << mes << "/" << ano;
}

void mostrar_autor(Autor a) {
	cout << a.nome << " " << a.sobrenome << endl;
}

void mostrar_editora(Editora ed) {
	cout << ed.nome << ", " << ed.local << endl;
}

void mostrar_livro(Livro a) {
	cout << "\nDados do livro:\n" << endl
		<< "\tID do livro: " << a.id << endl
		<< "\tTitulo: " << a.titulo << endl

		<< "\tAutor: ";
	mostrar_autor(a.autor);
	cout << "\tEditora: ";
	mostrar_editora(a.editora);

	cout << "\tAno de Publicacao: " << a.publicacao << endl
		<< "\tAssunto: " << a.assunto << endl;

	if (a.disponivel == true) {
		cout << "\tDisponivel\n";
	}
	else {
		cout << "\tIndisponivel\n";
	}
}

void mostrar_revista(Revista r) {
	cout << "\nDados da Revista:" << endl
		<< "\tID da revista: " << r.id << endl
		<< "\tTitulo: " << r.titulo << endl

		<< "\tEditora: ";
	mostrar_editora(r.editora);

	cout << "\tData da Publicacao " << r.publicacao.mes << "/" << r.publicacao.ano << endl;
	cout << "\tAssunto: " << r.assunto << endl;

	if (r.disponivel == true)
		cout << "\tDisponivel\n";
	else
		cout << "\tIndisponivel\n";
}

void mostrar_usuario(Usuario u) {
	cout << "\nDados do Usuario: " << endl
		<< "\tID do usuario: " << u.id << endl
		<< "\tNome Completo: " << u.nome << endl;
}

int gerar_ID(int num) {
	int id = 0;
	srand(time(NULL));
	if (num < 1100) {
		id = num + rand() % 900 + 1; //id pra livros
		return id;
	}
	if (num > 1100 && num < 2100) {
		id = num + rand() % 900 + 1; //id pra revistas
		return id;
	}
	if (num > 2100 && num < 3100) {
		id = num + rand() % 900 + 1; //id pra usuarios
		return id;
	}
}

//gera cadastros iniciais de titulos
void gerar_titulos(string lista_titulos[], string titulos_revistas[]) {
	const int tam = 5, tam2 = 10;
	string titulos[] = { "Tudo ou Nada", "Amanha eh outro dia", "Felizes para Sempre", "A Ultima Batalha", "Guerra dos Reis", "A Voz da Natureza", "Canto da Tempestade", "Maldicao Azul", "Harmonia em Vermelho", "Deuses em Caos" };
	bool flag = false;
	int aux[tam] = { 0 }, a = 0;

	srand(time(NULL));
	for (int i = 0; i < tam; i++) {
		a = rand() % tam2;
		if (i == 0) {
			lista_titulos[i] = titulos[a];
			aux[i] = a;
		}
		else if (i > 0) {
			flag = false;
			for (int j = 0; j < i; j++) {
				if (a == aux[j]) {
					flag = true; //verifica se o titulo de livro ja foi cadastrado uma vez
				}
			}
			if (flag) {
				i--; //se ja houver cadastro, nao permite cadastro igual
			}
			else if (flag == false) {
				lista_titulos[i] = titulos[a];
				aux[i] = a;
			}
		}
	}
	const int tam_r = 4, tam2_r = 10;
	int aux2[tam_r] = { 0 }, b = 0;
	string titulos_r[] = { "Vogue", "ELLE", "V Magazine", "Capricho", "Caras", "Veja SP", "A Sentinela", "Despertai", "Crusoé Revista Digital Semanal", "Manequim" };

	srand(time(NULL));
	for (int i = 0; i < tam_r; i++) {
		b = rand() % tam2_r;
		if (i == 0) {
			titulos_revistas[i] = titulos_r[b];
			aux2[i] = b;
		}
		else if (i > 0) {
			flag = false;
			for (int j = 0; j < i; j++) {
				if (b == aux2[j]) {
					flag = true; //verifica se o titulo de revista ja foi cadastrado uma vez
				}
			}
			if (flag) {
				i--; //se ja houver cadastro, nao permite cadastro igual
			}
			else if (flag == false) {
				titulos_revistas[i] = titulos_r[b];
				aux2[i] = b;
			}
		}
	}
}

//gera cadastros iniciais de autores
void gerar_autores(Autor lista_autores[]) {
	const int tam = 5;
	bool flag = false;
	string nomes[] = { "Maria", "Ana Carolina", "Davi", "Fernando", "Renato", "Gloria", "Erico", "Lais", "Juliano", "Rodrigo" };
	string sobrenomes[] = { "Moraes", "da Silva", "Pereira", "dos Santos", "Cruz", "Souza", "Lima", "Soares", "Zacarias", "Oliveira" };

	int aux[tam] = { 0 }, a = 0;
	srand(time(NULL));
	for (int i = 0; i < tam; i++) {
		a = rand() % 10;
		if (i == 0) {
			lista_autores[i].nome = nomes[a];
			aux[i] = a;
		}
		else if (i > 0) {
			flag = false;
			for (int j = 0; j < i; j++) {
				if (a == aux[j]) {
					flag = true; //verifica se o nome ja foi cadastrado uma vez
				}
			}
			if (flag) {
				i--; //se ja houver cadastro, nao permite cadastro igual de nomes
			}
			else if (flag == false) {
				lista_autores[i].nome = nomes[a]; //cadastra o nome 
				aux[i] = a;
			}
		}
	}
	int aux2[tam] = { 0 }, b = 0;
	srand(time(NULL));
	for (int i = 0; i < tam; i++) {
		b = rand() % 10;
		if (i == 0) {
			lista_autores[i].sobrenome = sobrenomes[b];
			aux2[i] = b;
		}
		else if (i > 0) {
			flag = false;
			for (int j = 0; j < i; j++) {
				if (b == aux2[j]) {
					flag = true; //verifica se o sobrenome ja foi cadastrado uma vez
				}
			}
			if (flag) {
				i--; //se ja houver cadastro, nao permite cadastro igual de sobrenomes
			}
			else if (flag == false) {
				lista_autores[i].sobrenome = sobrenomes[b]; //cadastra o sobrenome
				aux2[i] = b;
			}
		}
	}
}

//gera cadastros iniciais de editoras
void gerar_editoras(Editora lista_editoras[]) {
	const int tam = 9;
	bool flag = false;
	string nomes[] = { "Editora Plano", "Liberdade", "Recanto Editora", "Editora Arcanjo", "Hiperlivros", "Cometa", "Monumental", "Ponto&Virgula", "Sabedoria", "AlfaLetras", "SempreLer", "MundoPerdido" };
	string locais[] = { "Brasilia", "Porto Alegre", "Sao Paulo", "Rio de Janeiro", "Florianopolis", "Salvador", "Curitiba", "Santos", "Itajai", "Joinville", "Manaus", "Vitoria", "Belo Horizonte", "Palmas" };

	int aux[tam] = { 0 }, a = 0, aux2[tam] = { 0 }, b = 0;
	srand(time(NULL));
	for (int i = 0; i < tam; i++) {
		a = rand() % 12;
		if (i == 0) {
			lista_editoras[i].nome = nomes[a];
			aux[i] = a;
		}
		else if (i > 0) {
			flag = false;
			for (int j = 0; j < i; j++) {
				if (a == aux[j]) {
					flag = true; //verifica se o nome de editora ja foi cadastrado uma vez
				}
			}
			if (flag) {
				i--; //se ja houver cadastro, nao permite cadastro igual
			}
			else if (flag == false) {
				lista_editoras[i].nome = nomes[a];
				aux[i] = a;
			}
		}
	}
	srand(time(NULL));
	for (int i = 0; i < tam; i++) {
		b = rand() % 14;
		if (i == 0) {
			lista_editoras[i].local = locais[b];
			aux2[i] = b;
		}
		else if (i > 0) {
			flag = false;
			for (int j = 0; j < i; j++) {
				if (b == aux2[j]) {
					flag = true; //verifica se o local ja foi cadastrado uma vez
				}
			}
			if (flag) {
				i--; //se ja houver cadastro, nao permite cadastro igual
			}
			else if (flag == false) {
				lista_editoras[i].local = locais[b];
				aux2[i] = b;
			}
		}
	}
}

//gera cadastros iniciais de assuntos
void gerar_assuntos(string lista_assuntos[]) {
	const int tam = 9;
	string assuntos[] = { "Comedia", "Terror", "Romance", "Drama", "Ficcao Cientifica", "Suspense", "Horror", "Romance de Epoca", "Historia Real", "Historia", "Comedia Romantica", "Autoajuda", "Terror Psicologico", "Fantasia", "Religiao", "Poesia" };

	int aux[tam] = { 0 }, a = 0;
	srand(time(NULL));
	for (int i = 0; i < tam; i++) {
		a = rand() % 16;
		if (i == 0) {
			lista_assuntos[i] = assuntos[a];
			aux[i] = a;
		}
		else if (i > 0) {
			bool flag = false;
			for (int j = 0; j < i; j++) {
				if (a == aux[j]) {
					flag = true; //verifica se o assunto ja foi cadastrado uma vez
				}
			}
			if (flag) {
				i--; //se ja houver cadastro, nao permite cadastro igual
			}
			else if (flag == false) {
				lista_assuntos[i] = assuntos[a];
				aux[i] = a;
			}
		}
	}
}

//gera cadastros iniciais de anos de publicacao
void gerar_ano(int lista_ano[]) {
	int aux = 0;

	srand(time(NULL));
	for (int i = 0; i < 10; i++) {
		aux = rand() % 150;
		lista_ano[i] = 2023 - aux; //sorteia ano de publicação aleatorio
	}
}

//gera cadastros iniciais de mes de publicacao
void gerar_mes(int lista_mes[]) {
	int v[10] = { 0 }, aux = 0;

	srand(time(NULL));
	for (int i = 0; i < 10; i++) {
		aux = rand() % 12 + 1;
		if (i == 0) {
			lista_mes[i] = aux;
			v[i] = aux;
		}
		else if (i > 0) {
			if (aux != v[i - 1]) {
				lista_mes[i] = aux;
				v[i] = aux;
			}
			else {
				i--;
			}
		}
	}
}

// gera cadastros iniciais de livros
void gerar_livros(Livro& a, Editora b, Autor c, string assunto, string titulo, int ano, int id) {
	a.titulo = titulo;
	a.autor = c;
	a.editora = b;
	a.publicacao = ano;
	a.assunto = assunto;
	a.id = id;
}

void gerar_revistas(Revista& r, Editora b, string assunto, int ano, int mes, string titulo, int id) {
	r.titulo = titulo;
	r.editora = b;
	r.publicacao.mes = mes;
	r.publicacao.ano = ano;
	r.assunto = assunto;
	r.id = id;
}

void gerar_nomes(string lista_nomes[]) {
	const int tam = 3;
	string nomes[] = { "Pablo Jeremias", "Bernardo Junior", "Livia Luz", "Suzana Cruz", "Renata Silva", "Osvaldo Moreira", "Poliana Silva", "Julia Fernandes", "Cesar Neto" };

	int aux[tam] = { 0 }, a = 0;
	srand(time(NULL));
	for (int i = 0; i < tam; i++) {
		a = rand() % 9;
		if (i == 0) {
			lista_nomes[i] = nomes[a];
			aux[i] = a;
		}
		else if (i > 0) {
			bool flag = false;
			for (int j = 0; j < i; j++) {
				if (a == aux[j]) {
					flag = true; //verifica se o nome ja foi cadastrado uma vez
				}
			}
			if (flag) {
				i--; //se ja houver cadastro, nao permite cadastro igual
			}
			else if (flag == false) {
				lista_nomes[i] = nomes[a];
				aux[i] = a;
			}
		}
	}
}

void gerar_usuarios(Usuario& u, int id, string nome) {
	u.nome = nome;
	u.id = id;
}

// Função para verificar se um livro está disponível para empréstimo
bool livro_disponivel(Livro livro) {
	return !livro.disponivel;
}
#pragma endregion Funcoes

#pragma region Menu
void exibir_menu_principal() {
	cout << "\n[1] - Cadastro" << endl
		<< "[2] - Alteracao" << endl
		<< "[3] - Retirada" << endl
		<< "[4] - Consultar Acervo" << endl
		<< "[5] - Central do Usuario" << endl
		<< "[0] - Sair" << endl << endl;
	set_color(5);
	cout << "Digite uma opcao de menu: ";
	set_color(7);

}

void submenu_cadastro() {
	set_color(5);
	cout << "\n - - - - - CADASTRO - - - - -\n" << endl;
	set_color(7);
	cout << "[1] - Livro" << endl
		<< "[2] - Revista" << endl
		<< "[3] - Autor" << endl
		<< "[4] - Editora" << endl
		<< "[0] - Voltar" << endl << endl;
	set_color(5);
	cout << "Digite uma opcao de menu: ";
	set_color(7);
}

void submenu_alteracao() {
	set_color(5);
	cout << "\n - - - - - ALTERACAO - - - - -\n" << endl;
	set_color(7);
	cout << "[1] - Livro" << endl
		<< "[2] - Revista" << endl
		<< "[3] - Autor" << endl
		<< "[4] - Editora" << endl
		<< "[0] - Voltar" << endl << endl;
	set_color(5);
	cout << "Digite uma opcao de menu: ";
	set_color(7);
}

void submenu_retirada() {
	set_color(5);
	cout << "\n - - - - - RETIRADA - - - - -\n" << endl; //FALTA MUDAR O STATUS DE DISPONIVEL DO LIVRO OU REVISTA QUANDO FOR RETIRADO
	set_color(7);
	cout << "[1] - Livro" << endl
		<< "[2] - Revista" << endl
		<< "[0] - Voltar" << endl << endl;
	set_color(5);
	cout << "Digite uma opcao de menu: ";
	set_color(7);
}

void submenu_acervo() {
	set_color(5);
	cout << "\n - - - - - CONSULTAR ACERVO - - - - -\n" << endl;
	set_color(7);
	cout << "[1] - Pesquisa Filtrada" << endl
		<< "[2] - Listagem Completa" << endl
		<< "[0] - Voltar" << endl << endl;
	set_color(5);
	cout << "Digite uma opcao de menu: ";
	set_color(7);
}

void submenu_central_usuario() {
	set_color(5);
	cout << "\n - - - - - CENTRAL DO USUARIO - - - - -\n" << endl;
	set_color(7);
	cout << "[1] - Cadastro de Usuario" << endl
		<< "[2] - Usuarios com atraso de devolucao" << endl
		<< "[3] - Historico de retiradas" << endl
		<< "[4] - Listagem de Usuarios" << endl
		<< "[0] - Voltar" << endl << endl;
	set_color(5);
	cout << "Digite uma opcao de menu: ";
	set_color(7);
}

void submenu_filtrar_pesquisa() {
	set_color(5);
	cout << "\nDeseja filtrar sua pesquisa por: " << endl;
	set_color(7);
	cout << "\n[1] - Livros" << endl
		<< "[2] - Disponibilidade" << endl
		<< "[3] - Revistas" << endl
		<< "[4] - Autor" << endl
		<< "[5] - Editora" << endl
		<< "[6] - Assunto" << endl
		<< "[0] - Voltar" << endl << endl;
	set_color(5);
	cout << "\nDigite uma opcao de menu: ";
	set_color(7);
}
#pragma endregion  Menu

int main()
{
#pragma region Data atual
	int year, month, day;
	struct tm timeinfo;
	time_t now = time(0);

	localtime_s(&timeinfo, &now);
	year = timeinfo.tm_year + 1900;
	month = timeinfo.tm_mon + 1;
	day = timeinfo.tm_mday;

	Data data_atual = { day, month, year };

#pragma endregion Data atual

#pragma region listas
	//listas
	Autor lista_autores[100];
	Editora lista_editoras[100];
	string lista_assuntos[100], lista_titulos[100], titulos_revistas[100], lista_nomes[3];
	Livro lista_livros[100];
	Revista lista_revistas[100];
	Usuario lista_usuarios[100];
	int lista_ano[10], lista_mes[10];
#pragma endregion listas

#pragma region variaveis
	//outras variaveis
	int opcao, min_opcao, max_opcao, escolha;
	int cont_livros = 4, cont_revistas = 3, cont_autor = 4, cont_editora = 8, cont_assunto = 8, cont_usuario = 2, soma = 0;;
	char resposta;
	bool flag = false;
	Livro livro;
	Autor autor;
	Editora ed;
	string aux;
	Usuario usuario;
	Data data;
#pragma endregion variaveis

#pragma region gerar
	//gerar IDs iniciais
	int id_livro = gerar_ID(1001);
	int id_revista = gerar_ID(2001);
	int id_usuario = gerar_ID(3001);

	gerar_autores(lista_autores); //cadastro de 5 autores 
	gerar_editoras(lista_editoras); //cadastro de 9 editoras 
	gerar_assuntos(lista_assuntos); //cadastro de 15 assuntos 
	gerar_titulos(lista_titulos, titulos_revistas); //cadastro de 9 titulos
	gerar_ano(lista_ano); //cadastro de 10 anos aleatorios
	gerar_mes(lista_mes); //cadastro de 10 meses aleatorios
	gerar_nomes(lista_nomes); //cadastro de 10 meses aleatorios
#pragma endregion gerar

	for (int i = 0; i < 5; i++) { //cadastro de 5 livros aleatorios
		gerar_livros(lista_livros[i], lista_editoras[i], lista_autores[i], lista_assuntos[i], lista_titulos[i], lista_ano[i], id_livro);
		id_livro++;
	}

	for (int i = 0; i < 4; i++) { //cadastro de 4 revistas aleatorias
		gerar_revistas(lista_revistas[i], lista_editoras[i + 5], lista_assuntos[i + 5], lista_ano[i + 5], lista_mes[i], titulos_revistas[i], id_revista);
		id_revista++;
	}

	for (int i = 0; i < 3; i++) { //cadastro de 3 usuarios aleatorios
		gerar_usuarios(lista_usuarios[i], id_usuario, lista_nomes[i]);
		id_usuario++;
	}

	int escolha_menu_principal = 0;
	int escolha_submenu = 0;

	while (true)
	{
		do {
			set_color(5);
			cout << "\n- - - - - - - - BIBLIOTECA - - - - - - - - -" << endl;
			cout << "\t\t";
			mostrar_data(data_atual);
			set_color(7);
			cout << endl;
			exibir_menu_principal();
			cin >> escolha_menu_principal;
			system("cls");
		} while (opcao_invalida(escolha_menu_principal, 0, 5));

		// continua com a opção que o usuario escolheu
		switch (escolha_menu_principal)
		{
		case 0: // fecha o programa
			cout << "Volte sempre! " << endl;
			return 0;
			break;

		case 1: // cadastro de material
			do {
				submenu_cadastro();
				cin >> escolha_submenu;
				system("cls");

				switch (escolha_submenu) {
					// Casos para cada opção do submenu 1
				case 0:
					system("cls");
					break;

				case 1: //cadastrar livro
					do {
						system("cls");
						set_color(5);
						cout << "\n\tPreencha os dados abaixo: " << endl;
						set_color(7);

						cont_livros++;
						lista_livros[cont_livros] = cadastrar_livro();
						lista_livros[cont_livros].id = id_livro;
						id_livro++;
						cont_autor++;
						cont_editora++;

						//atualiza lista de autores e editoras tambem
						lista_autores[cont_autor].nome = lista_livros[cont_livros].autor.nome;
						lista_autores[cont_autor].sobrenome = lista_livros[cont_livros].autor.sobrenome;
						lista_editoras[cont_editora].nome = lista_livros[cont_livros].editora.nome;
						lista_editoras[cont_editora].local = lista_livros[cont_livros].editora.local;

						flag = false;
						for (int i = 0; i < cont_assunto + 1; i++) {
							if (lista_assuntos[i] == lista_livros[cont_livros].assunto) { //verifica se o assunto ja esta cadastrado
								flag = true;
							}
						}
						if (flag == false) { //se não estiver cadastrado, eh adicionado a lista
							cont_assunto++;
							lista_assuntos[cont_assunto] = lista_livros[cont_livros].assunto;
						}
						set_color(2);
						cout << "\n\tLivro cadastrado com sucesso!\n" << endl;
						set_color(7);
						cout << "Deseja cadastrar mais um livro (s/n)? ";
						cin >> resposta;
					} while (resposta == 's' || resposta == 'S'); //loop continua enquanto o usuario desejar cadastrar mais livros
					system("cls");
					break;

				case 2: //cadastrar revistas
					do {
						system("cls");
						set_color(5);
						cout << "\n\tPreencha os dados abaixo: " << endl;
						set_color(7);
						cont_revistas++;
						lista_revistas[cont_revistas] = cadastrar_revista();
						lista_revistas[cont_revistas].id = id_revista;
						id_revista++;
						cont_editora++;

						//atualiza lista de autores e editoras tambem
						lista_editoras[cont_editora].nome = lista_revistas[cont_revistas].editora.nome;
						lista_editoras[cont_editora].local = lista_revistas[cont_revistas].editora.local;

						flag = false;
						for (int i = 0; i < cont_assunto + 1; i++) {
							if (lista_assuntos[i] == lista_revistas[cont_revistas].assunto) { //verifica se o assunto ja esta cadastrado
								flag = true;
							}
						}
						if (flag == false) { //se não estiver cadastrado, eh adicionado a lista
							cont_assunto++;
							lista_assuntos[cont_assunto] = lista_revistas[cont_revistas].assunto;
						}

						set_color(2);
						cout << "\n\tRevista cadastrada com sucesso!\n" << endl;
						set_color(7);
						cout << "Deseja cadastrar mais um livro (s/n)? ";
						cin >> resposta;
					} while (resposta == 's' || resposta == 'S');  //continua enquanto for desejado cadastrar mais revistas

					system("cls");
					break;

				case 3: //cadastrar autor
					do {
						system("cls");
						set_color(5);
						cout << "\nLista atual de autores cadastrados: \n" << endl;
						set_color(7);

						for (int i = 0; i < cont_autor + 1; i++) {
							cout << "\t[" << i + 1 << "] ";
							mostrar_autor(lista_autores[i]); //mostra todos os autores cadastrados ate o momento
							cout << endl;
						}

						cont_autor++;
						cin.ignore();
						set_color(5);
						cout << "\nPreencha os dados abaixo: \n" << endl;
						set_color(7);
						autor = ler_autor();

						flag = false;
						for (int i = 0; i < cont_autor + 1; i++) { //verifica se ja existe cadastro deste autor
							if (lista_autores[cont_autor].nome == lista_autores[i].nome && lista_autores[cont_autor].sobrenome == lista_autores[i].sobrenome) {
								cout << "\nAutor ja cadastrado no sistema" << endl;
								flag = true;
							}
						}
						if (flag = false) {
							lista_autores[cont_autor].nome = autor.nome; //se nao tiver cadastro ainda, atualiza a lista de autores
							lista_autores[cont_autor].sobrenome = autor.sobrenome;

							set_color(2);
							cout << "\n\tAutor cadastrado com sucesso!" << endl;
							set_color(7);
						}
						cout << "\nDeseja realizar outro cadastro (s/n)? ";
						cin >> resposta;
					} while (resposta == 's' || resposta == 'S');

					system("cls");
					break;

				case 4: //cadastrar editora
					do {
						system("cls");
						set_color(5);
						cout << "\nLista atual de editoras cadastradas: \n" << endl;
						set_color(7);

						for (int i = 0; i < cont_editora + 1; i++) {
							cout << "\t[" << i + 1 << "] ";
							mostrar_editora(lista_editoras[i]); //mostra todas as editoras cadastradas ate o momento
							cout << endl;
						}

						cont_editora++;
						cin.ignore();
						set_color(5);
						cout << "\nPreencha os dados abaixo: \n" << endl;
						set_color(7);

						ed = ler_editora();

						flag = false;
						for (int i = 0; i < cont_editora + 1; i++) { //verifica se ja existe cadastro desta editora
							if (lista_editoras[cont_editora].nome == lista_editoras[i].nome && lista_editoras[cont_editora].local == lista_editoras[i].local) {
								cout << "\nEditora ja cadastrada no sistema" << endl;
								flag = true;
							}
						}
						if (flag = false) {
							lista_editoras[cont_editora].nome = ed.nome; //se nao tiver cadastro ainda, atualiza a lista de editoras
							lista_editoras[cont_editora].local = ed.local;

							set_color(2);
							cout << "\n\tEditora cadastrado com sucesso!" << endl;
							set_color(7);
						}
						cout << "\nDeseja realizar outro cadastro (s/n)? ";
						cin >> resposta;

					} while (resposta == 's' || resposta == 'S');
					system("cls");
					break;
				}
			} while (opcao_invalida(escolha_submenu, 0, 5));
			break;

		case 2: // alteracao de material
			do {
				submenu_alteracao();
				cin >> escolha_submenu;
				system("cls");

				switch (escolha_submenu) {
				case 0: //voltar para o menu
					system("cls");
					break;

				case 1://alterar livro
					system("cls");
					set_color(5);
					cout << "\nLivros cadastrados atualmente (id e titulo): \n" << endl;
					set_color(7);

					for (int i = 0; i < cont_livros + 1; i++) {
						cout << "\t[" << i + 1 << "] ";
						cout << lista_livros[i].titulo << " (" << lista_livros[i].id << ")\n";
					}
					set_color(7);
					set_color(5);
					cout << "\n\nEscolha a opcao que deseja alterar: ";
					set_color(7);

					cin >> escolha;

					usuario.id = lista_livros[escolha - 1].id;
					lista_livros[escolha - 1] = cadastrar_livro();
					lista_livros[escolha - 1].id = usuario.id;

					set_color(2);
					cout << "\n\tLivro alterado com sucesso!\n" << endl;
					set_color(7);

					//atualiza lista de autores e editoras tambem
					lista_autores[escolha - 1].nome = lista_livros[escolha - 1].autor.nome;
					lista_autores[escolha - 1].sobrenome = lista_livros[escolha - 1].autor.sobrenome;
					lista_editoras[escolha - 1].nome = lista_livros[escolha - 1].editora.nome;
					lista_editoras[escolha - 1].local = lista_livros[escolha - 1].editora.local;

					flag = false;
					for (int i = 0; i < cont_assunto + 1; i++) {
						if (lista_assuntos[i] == lista_livros[cont_livros].assunto) { //verifica se o assunto ja esta cadastrado
							flag = true;
						}
					}
					if (flag == false) { //se não estiver cadastrado, eh adicionado a lista
						cont_assunto++;
						lista_assuntos[cont_assunto] = lista_livros[cont_livros].assunto;
					}
					break;

				case 2:
					system("cls");
					set_color(5);
					cout << "\nRevistas cadastrados atualmente (id e titulo): \n" << endl;
					set_color(7);
					for (int i = 0; i < cont_revistas + 1; i++) {
						cout << "\t[" << i + 1 << "] ";
						cout << lista_revistas[i].titulo << " (" << lista_revistas[i].id << ")\n";
					}
					set_color(5);
					cout << "\n\nEscolha a opcao que deseja alterar: ";
					set_color(7);
					cin >> escolha;

					usuario.id = lista_revistas[escolha - 1].id;
					lista_revistas[escolha - 1] = cadastrar_revista();
					lista_revistas[escolha - 1].id = usuario.id;
					set_color(2);
					cout << "\n\tRevista alterada com sucesso!\n" << endl;
					set_color(7);
					//atualiza lista de autores e editoras tambem
					lista_editoras[escolha - 1].nome = lista_revistas[escolha - 1].editora.nome;
					lista_editoras[escolha - 1].local = lista_revistas[escolha - 1].editora.local;

					flag = false;
					for (int i = 0; i < cont_assunto + 1; i++) {
						if (lista_assuntos[i] == lista_revistas[cont_revistas].assunto) { //verifica se o assunto ja esta cadastrado
							flag = true;
						}
					}
					if (flag == false) { //se não estiver cadastrado, eh adicionado a lista
						cont_assunto++;
						lista_assuntos[cont_assunto] = lista_revistas[cont_revistas].assunto;
					}
					break;

				case 3: //alterar autor
					system("cls");
					set_color(5);
					cout << "\nLista atual de autores cadastrados: \n\n" << endl;
					set_color(7);
					for (int i = 0; i < cont_autor + 1; i++) {
						cout << "\t[" << i + 1 << "] ";
						mostrar_autor(lista_autores[i]);
					}
					set_color(5);
					cout << "\n\nEscolha a opcao que deseja alterar: ";
					set_color(7);
					cin >> escolha;

					cin.ignore();
					lista_autores[escolha - 1] = ler_autor();
					set_color(2);
					cout << "\n\tAutor alterado com sucesso!\n" << endl;
					set_color(7);

					//atualiza lista de livros tambem
					lista_livros[escolha - 1].autor.nome = lista_autores[escolha - 1].nome;
					lista_livros[escolha - 1].autor.sobrenome = lista_autores[escolha - 1].sobrenome;
					break;

				case 4: //alterar editora
					system("cls");
					set_color(5);
					cout << "\nLista atual de editoras cadastradas: \n\n" << endl;
					set_color(7);
					for (int i = 0; i < cont_editora + 1; i++) {
						cout << "\t[" << i + 1 << "] ";
						mostrar_editora(lista_editoras[i]);
					}
					set_color(5);
					cout << "\n\nEscolha a opcao que deseja alterar: ";
					set_color(7);
					cin >> escolha;

					cin.ignore();
					lista_editoras[escolha - 1] = ler_editora();
					set_color(2);
					cout << "\n\tAEditora alterada com sucesso!\n" << endl;
					set_color(7);

					//atualiza lista de livros tambem
					lista_livros[escolha - 1].editora.nome = lista_editoras[escolha - 1].nome;
					lista_livros[escolha - 1].editora.local = lista_editoras[escolha - 1].local;
					break;
				}
				system("pause");
			} while (opcao_invalida(escolha_submenu, 0, 4));
			system("cls");
			break;

		case 3: // retirada
			do {
				submenu_retirada();
				cin >> escolha_submenu;
				system("cls");
			} while (opcao_invalida(escolha_submenu, 0, 2));

			switch (escolha_submenu)
			{
			case 0:
				system("cls");
				break;

			case 1: // RETIRADA LIVROS
				system("cls");
				set_color(5);
				cout << "\nLista de Livros Disponiveis (Id - Titulo)\n" << endl;
				set_color(7);

				for (int i = 0; i < cont_livros; i++) {
					if (lista_livros[i].disponivel) {
						cout << "\t[" << lista_livros[i].id << "] - ";
						cout << lista_livros[i].titulo << endl;
					}
				}

				do {
					cout << "\nInforme o ID do livro: ";
					cin >> escolha;

					flag = false;
					for (int i = 0; i < cont_livros; i++) {
						if (lista_livros[i].id == escolha && lista_livros[i].disponivel) {
							flag = true;
							break;
						}
					}

					if (!flag) {
						set_color(4);
						cout << "\nLivro indisponivel para retirada." << endl;
						set_color(7);
						system("pause");
						system("cls");
						break;
					}

					if (flag == false) {
						set_color(4);
						cout << "\nId de Livro nao foi encontrada, verifique se digitou o ID corretamente!" << endl;
						set_color(7);
					}
				} while (flag == false);

				if (flag == true) {
					cout << "\nInforme seu ID de usuario: ";
					cin >> usuario.id;

					cout << "\nEntre com a data de retirada:\n";
					data = ler_data();

					// Permitir a retirada do livro
					soma = 0;
					flag = false;

					for (int i = 0; i < cont_usuario; i++) {
						if (lista_usuarios[i].id == usuario.id) {
							flag = true;

							if (atraso(lista_usuarios[i]) == false) {
								soma = qtd_retiradas(lista_usuarios[i]);
								lista_usuarios[i].retirados[soma].id = escolha;

								lista_usuarios[i].retirados[soma].retirada = data;
								lista_usuarios[i].retirados[soma].devolucao = data_devolucao(data);

								for (int j = 0; j < cont_livros; j++) {
									if (lista_livros[j].id == escolha) {
										lista_livros[j].disponivel = false;
										break;
									}
								}

								set_color(2);
								cout << "\nItem retirado com sucesso!" << endl;
								set_color(7);
								cout << "\tVoce tem 7 dias para realizar a leitura do material" << endl;
								cout << "\tData de devolucao: ";
								mostrar_data(lista_usuarios[i].retirados[soma].devolucao);
								cout << endl << endl;
							}
							else {
								break;
							}
						}
					}

					if (flag == false) {
						set_color(4);
						cout << "\nID de Usuario nao foi encontrado, verifique se ja realizou cadastro ou se digitou o ID corretamente!" << endl;
						set_color(7);
					}
				}

				system("pause");
				break;

			case 2: // RETIRADA REVISTAS
				system("cls");
				set_color(5);
				cout << "\nLista de Revistas Disponiveis (id e titulo): \n" << endl;
				set_color(7);

				for (int i = 0; i < cont_revistas + 1; i++) {
					if (lista_revistas[i].disponivel) {
						cout << "\t[" << lista_revistas[i].id << "] ";
						cout << lista_revistas[i].titulo << endl;
					}
				}
				do {
					cout << endl;
					cout << "\nInforme o ID da revista que deseja retirar: ";
					cin >> escolha;

					flag = false;
					for (int i = 0; i < cont_revistas + 1; i++) {
						if (lista_revistas[i].id == escolha && lista_revistas[i].disponivel) {
							flag = true;
							break;
						}
					}
					if (!flag) {
						set_color(4);
						cout << "\nRevista indisponivel para retirada." << endl;
						set_color(7);
						system("pause");
						system("cls");
						break;
					}

					if (flag == false) {
						set_color(4);
						cout << "\nId de Revista nao foi encontrada, verifique se digitou o ID corretamente!" << endl;
						set_color(7);
					}
				} while (flag == false);

				if (flag == true) {
					cout << "\nInforme seu ID de usuario: ";
					cin >> usuario.id;

					cout << "\nEntre com a data de retirada:\n";
					data = ler_data();

					soma = 0;
					flag = false;
					for (int i = 0; i < cont_usuario + 1; i++) {
						if (lista_usuarios[i].id == usuario.id) { //encontra na lista de usuarios cadastrados o usuario correspondente ao id informado
							flag = true;

							if (atraso(lista_usuarios[i]) == false) {
								soma = qtd_retiradas(lista_usuarios[i]);
								lista_usuarios[i].retirados[soma].id = escolha; //atualiza lista de retirados do usuario

								lista_usuarios[i].retirados[soma].retirada.dia = data.dia;
								lista_usuarios[i].retirados[soma].retirada.mes = data.mes;
								lista_usuarios[i].retirados[soma].retirada.ano = data.ano;
								lista_usuarios[i].retirados[soma].devolucao = data_devolucao(data);

								for (int i = 0; i < cont_revistas + 1; i++) {
									if (lista_revistas[i].id == escolha) {
										lista_revistas[i].disponivel = false; //passa a colocar o livro como indisponivel
									}
								}
								set_color(2);
								cout << "\nItem retirado com sucesso!" << endl;
								set_color(7);
								cout << "\tVoce tem 7 dias para realizar a leitura do material" << endl;
								cout << "\tData de devolucao: ";
								mostrar_data(lista_usuarios[i].retirados[soma].devolucao);
								cout << endl << endl;
							}
							else if (atraso(lista_usuarios[i])) {
								break;
							}
						}
					}

					if (flag == false) {
						set_color(4);
						cout << "\nID de Usuario nao foi encontrado, verifique se ja realizou cadastro ou se digitou a ID corretamente!" << endl;
						set_color(7);
					}
				}
				system("pause");
				break;
			}
			system("cls");
			break;

		case 4: // consultar acervo
			do {
				submenu_acervo();
				cin >> escolha_submenu;
				system("cls");
			} while (opcao_invalida(escolha_submenu, 0, 2));

			if (escolha_submenu == 0) {
				system("cls");
				break;
			}

			if (escolha_submenu == 1) {
				do {
					submenu_filtrar_pesquisa();
					cin >> escolha_submenu;
					system("cls");

					switch (escolha_submenu) {
					case 0: //voltar para o menu
						system("cls");
						break;

					case 1: //mostrar todos os livros
						set_color(5);
						cout << "\n - - - - - LISTAGEM DE LIVROS - - - - -" << endl;
						set_color(7);
						for (int i = 0; i < cont_livros; i++) {
							mostrar_livro(lista_livros[i]);
						}
						system("pause");
						break;

					case 2: // mostrar todos os livros e revistas disponiveis
						set_color(5);
						cout << "\nLista de livros disponiveis:\n" << endl;
						set_color(7);
						for (int i = 0; i < cont_livros + 1; i++) {
							if (lista_livros[i].disponivel) {
								mostrar_livro(lista_livros[i]);

							}
						}
						for (int i = 0; i < cont_livros + 1; i++) {
							if (lista_revistas[i].disponivel) {
								mostrar_revista(lista_revistas[i]);
							}
						}
						system("pause");
						break;

					case 3: //mostrar todas as revistas
						set_color(5);
						cout << "\n - - - - - LISTAGEM DE REVISTAS - - - - -" << endl;
						set_color(7);
						for (int i = 0; i < cont_livros + 1; i++) {
							mostrar_revista(lista_revistas[i]);
						}
						system("pause");
						break;

					case 4: //mostrar livros de um autor especifico
						set_color(5);
						cout << "\nLista atual de autores cadastrados: " << endl;
						set_color(7);
						for (int i = 0; i < cont_autor + 1; i++) {
							cout << "\t[" << i + 1 << "] ";
							mostrar_autor(lista_autores[i]);
						}
						cout << endl;
						cin.ignore();
						autor = ler_autor();

						flag = false;
						for (int i = 0; i < cont_livros + 1; i++) {
							if (lista_livros[i].autor.nome == autor.nome && lista_livros[i].autor.sobrenome == autor.sobrenome) { //verifica livros com o autor procurado
								mostrar_livro(lista_livros[i]);
								flag = true;
							}
						}
						if (flag == false) {
							set_color(4);
							cout << "\nNao existem livros cadastrados com este autor" << endl;
							set_color(7);
						}
						system("pause");
						break;

					case 5: //mostrar livros e revistas de uma editora
						set_color(5);
						cout << "\nLista atual de editoras cadastradas: " << endl;
						set_color(7);
						for (int i = 0; i < cont_editora + 1; i++) {
							cout << "\t[" << i + 1 << "] ";
							mostrar_editora(lista_editoras[i]);
						}
						cout << endl;
						cin.ignore();
						ed = ler_editora();

						flag = false;
						for (int i = 0; i < cont_livros + 1; i++) {
							if (lista_livros[i].editora.nome == ed.nome && lista_livros[i].editora.local == ed.local) { //verifica se existem livros da editora procurada
								mostrar_livro(lista_livros[i]);
								flag = true;
							}
							else if (lista_revistas[i].editora.nome == ed.nome && lista_revistas[i].editora.local == ed.local) { //verifica as revistas
								mostrar_revista(lista_revistas[i]);
								flag = true;
							}
						}
						if (flag == false) {
							set_color(4);
							cout << "\nNao existem livros ou revistas cadastrados com esta Editora" << endl;
							set_color(7);
						}
						system("pause");
						break;

					case 6: //mostrar livros e revistas de um assunto
						cin.ignore();
						set_color(5);
						cout << "Qual assunto deseja encontrar? ";
						set_color(7);
						getline(cin, aux);

						flag = false;
						for (int i = 0; i < cont_livros + 1; i++) {
							if (lista_livros[i].assunto == aux) { //verifica se existem livros relacionados ao assunto procurado
								mostrar_livro(lista_livros[i]);
								flag = true;
							}
							else if (lista_revistas[i].assunto == aux) { //verifica as revistas
								mostrar_revista(lista_revistas[i]);
								flag = true;
							}
						}
						if (flag == false) {
							set_color(4);
							cout << "\nNao existem livros ou revistas cadastrados com este assunto" << endl;
							set_color(7);
						}
						system("pause");
						break;
					}
					system("cls");
					break;
				} while (opcao_invalida(escolha_submenu, 0, 6));
			}

			else if (escolha_submenu == 2) { //lista de todos os livros e revistas
				cout << "\n - - - - - LISTAGEM COMPLETA - - - - -" << endl;
				for (int i = 0; i < cont_livros + 1; i++) {
					mostrar_livro(lista_livros[i]);
				}
				for (int i = 0; i < cont_revistas + 1; i++) {
					mostrar_revista(lista_revistas[i]);
				}
				cout << endl;
				system("pause");
			}
			system("cls");
			break;

		case 5: //informacoes relacionadas ao usuario
			do {
				submenu_central_usuario();
				cin >> escolha_submenu;
				system("cls");
			} while (opcao_invalida(escolha_submenu, 0, 4));

			switch (escolha_submenu)
			{
			case 0:
				system("cls");
				break;

			case 1:
				system("cls");
				set_color(5);
				cout << "\n - - - - - CADASTRE-SE - - - - -\n" << endl;
				set_color(7);

				usuario = cadastrar_usuario();
				cont_usuario++;

				flag = false;
				for (int i = 0; i <= cont_usuario; i++) {
					if (lista_usuarios[i].nome == usuario.nome) { // verifica se o usuário já está cadastrado
						cout << "\nUsuario ja cadastrado." << endl;
						cont_usuario--;
						flag = true;
						break;
					}
				}
				if (!flag) {
					lista_usuarios[cont_usuario].nome = usuario.nome;
					lista_usuarios[cont_usuario].id = id_usuario;
					cout << "Seu ID de usuario eh: " << lista_usuarios[cont_usuario].id;
					id_usuario++;
				}
				set_color(2);
				cout << "\nUsuario cadastrado com sucesso!" << endl;
				set_color(7);
				system("pause");
				system("cls");
				break;

			case 2:
				system("cls");
				set_color(4);
				cout << "- - - - - Usuarios com Atraso de Devolucao - - - - -\n\n";
				set_color(7);

				for (int i = 0; i < cont_usuario + 1; i++) {
					if (atraso(lista_usuarios[i])) {
						cout << "ID: " << lista_usuarios[i].id << endl;
						cout << "\nOla " << lista_usuarios[i].nome << endl << endl;
						cout << "\nLivros emprestados em atraso: " << endl;
						for (int j = 0; j < qtd_retiradas(lista_usuarios[i]); j++) {
							for (int k = 0; k < cont_livros; k++) {
								if (lista_usuarios[i].retirados[j].id == lista_livros[k].id) { // mostra livros emprestados em atraso
									mostrar_livro(lista_livros[k]);
									cout << "\tData de devolucao: ";
									mostrar_data(lista_usuarios[i].retirados[j].devolucao);
									cout << endl;
								}
							}
						}
						cout << endl;
					}
				}
				system("pause");
				system("cls");
				break;

			case 3:
				system("cls");
				set_color(5);
				cout << "- - - - - HISTORICO DE RETIRADA - - - - -\n" << endl;
				set_color(7);
				cout << "\nInforme o ID do usuario: ";
				cin >> usuario.id;

				soma = 0;
				flag = false;

				for (int i = 0; i < cont_usuario + 1; i++) {
					if (lista_usuarios[i].id == usuario.id) { // encontra na lista de usuarios cadastrados o usuario correspondente ao id informado
						flag = true;
						soma = qtd_retiradas(lista_usuarios[i]);
						cout << "\nHistorico de Retirada para " << lista_usuarios[i].nome << ": " << endl << endl;
						for (int j = 0; j < soma; j++) {
							for (int k = 0; k < cont_livros; k++) {
								if (lista_usuarios[i].retirados[j].id == lista_livros[k].id) { // mostra livros retirados
									mostrar_livro(lista_livros[k]);
									cout << "\tData de retirada: ";
									mostrar_data(lista_usuarios[i].retirados[j].retirada);
									cout << "\n\tData de devolucao: ";
									mostrar_data(lista_usuarios[i].retirados[j].devolucao);
									cout << endl;
								}
							}
							for (int k = 0; k < cont_revistas; k++) {
								if (lista_usuarios[i].retirados[j].id == lista_revistas[k].id) { //mostra revistas retiradas
									mostrar_revista(lista_revistas[k]);
									cout << "\n\tData de retirada: ";
									mostrar_data(lista_usuarios[i].retirados[j].retirada);
									//	cout << "\tData de devolucao: " << data_dev.dia << "/" << data_dev.mes << "/" << data_dev.ano << endl << endl;
									cout << endl;
								}
							}
						}
					}
				}

				if (flag == false) {
					set_color(4);
					cout << "\nID de Usuario nao foi encontrado, verifique existe cadastro ou se digitou o ID corretamente" << endl;
					set_color(7);
				}
				cout << endl;
				system("pause");
				system("cls");
				break;


			case 4: // Listagem de Usuarios
				system("cls");
				set_color(5);
				cout << "\t- - - - - LISTAGEM DE USUARIOS - - - - -" << endl;
				set_color(7);
				for (int i = 0; i < cont_usuario + 1; i++) {
					mostrar_usuario(lista_usuarios[i]);
				}
				cout << endl;
				system("pause");
				system("cls");
				break;
			}
			break;
		}
	}
	return 0;
}