/*Alunos:
Vitor Kohara Guerra-211027349
Lorena  Gomes de Freitas Nascimento- 200040278
*/
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <numeric>
#include <cmath>
#include <algorithm>
#include <cctype>
using namespace std;
void cifra();
void decifra(string &criptograma, string &senha);
int key_size(string criptograma);
void freq_attack(string criptograma, int tamanho_senha, int idioma);
string filtro(string word);
vector<int> encontra_espacos(const string& mensagem, const string& trigama);

int main() {
    int opcao,tam_senha, idioma;
    string criptograma, senha; 

    cout << "Digite 1 para cifrar, 2 decifrar e 3 para realizar o ataque" << endl;
    cin >> opcao;
    // Limpa o buffer de entrada antes de usar getline
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    switch (opcao) {
        case 1:
            cifra();
            break;
        case 2:
            cout << "Insira o criptograma que deseja decifrar:" << endl; 
            getline(cin, criptograma); 
            cout << "Insira a senha:" << endl; 
            getline(cin, senha); 
            decifra(criptograma, senha);
            break;
        case 3:
            cout<<"Insira o criptograma que deseja decifrar:"<<endl;
            getline(cin,criptograma);
            tam_senha = key_size(criptograma);
            cout<<"Selecione o idioma: 1- Inglês e 2-Português"<<endl;
            cin>>idioma;
            freq_attack(criptograma,tam_senha,idioma);
            break;
        default:
            cout << "Opcao Invalida" << endl;
    }

    return 0;
}
string filtro(string word){//função para formatar strings para que tenham somente letras minúsculas
    int tamanho_criptograma = 0;
    string criptograma;

    tamanho_criptograma = word.size();
    for(int i = 0; i < tamanho_criptograma ; i++){//remoção dos caracteres que não são letras minúsculas
        if('a'<= word[i] && word[i] <= 'z'){
            criptograma.push_back(word[i]);
        }
    }
    return criptograma;
}
void cifra() {
    string mensagem, senha, criptograma;
    cout << "Insira a mensagem que deseja cifrar:" << endl;
    getline(cin, mensagem);
    cout << "Insira a senha:" << endl;
    getline(cin, senha);
    
    mensagem = filtro(mensagem); // Filtragem da mensagem
    int tam_mensagem = static_cast<int>(mensagem.size());
    int tam_senha = static_cast<int>(senha.size());
    vector<int> dec_m(tam_mensagem);
    vector<int> dec_cript(tam_mensagem);
    vector<int> dec_s(tam_senha);
    int z;
    unsigned int i;
    criptograma.resize(tam_mensagem);  // Redimensiona a string para o tamanho da mensagem

    for (i = 0; i < tam_senha; i++) {
        dec_s[i] = static_cast<int>(senha[i]) - 'a'; // 97
    }

    for (i = 0; i < tam_mensagem; i++) {
        dec_m[i] = static_cast<int>(mensagem[i]) - 'a';
        z = i % tam_senha;
        dec_cript[i] = (dec_m[i] + dec_s[z]) % 26; // Cifragem
        criptograma[i] = static_cast<char>(dec_cript[i] + 'a'); // Transformando de decimal para char
    }

    cout << "Criptograma:" << endl;
    cout << criptograma << endl;
}

void decifra(string &criptograma, string &senha) {
    string mensagem;
    criptograma = filtro(criptograma); // Filtragem do criptograma

    int tam_criptograma = static_cast<int>(criptograma.size());
    int tam_senha = static_cast<int>(senha.size());
    vector<int> dec_m(tam_criptograma);
    vector<int> dec_cript(tam_criptograma);
    vector<int> dec_s(tam_senha);
    int z;
    unsigned int i;
    mensagem.resize(tam_criptograma);  // Redimensiona a string para o tamanho do criptograma

    for (i = 0; i < tam_senha; i++) {
        dec_s[i] = static_cast<int>(senha[i]) - 'a'; // 97
    }

    for (i = 0; i < tam_criptograma; i++) {
        dec_cript[i] = static_cast<int>(criptograma[i]) - 'a'; // 97
        z = i % tam_senha;
        dec_m[i] = (dec_cript[i] - dec_s[z] + 26) % 26; // Decifragem
        mensagem[i] = static_cast<char>(dec_m[i] + 'a'); // Transformando de decimal para char
    }

    cout << "Mensagem:" << endl;
    cout << mensagem << endl;
}

vector<int> encontra_espacos(const string& mensagem, const string& trigama) { 
    vector<int> distancias;
    size_t pos_inicial = mensagem.find(trigama); 
    while (pos_inicial != string::npos) { // Enquanto não houver ocorrência do trigrama
        size_t pos_nova = mensagem.find(trigama, pos_inicial + 1);
        if (pos_nova != string::npos) { // Se ocorrência for válida
            distancias.push_back(static_cast<int>(pos_nova - pos_inicial)); // Calcula a distância e armazena no vetor de distâncias
            pos_inicial = pos_nova; // Reinicia a busca por ocorrência
        } else {
            break; // Caso o contrário para a busca 
        }
    }
    return distancias;
}

int key_size(string criptograma) {
    criptograma = filtro(criptograma); // Filtragem do criptograma
    int tamanho_criptograma = criptograma.size();
    int max_chave = 20; // Define um tamanho máximo de chave de 20 caracteres

    map<string, int> trigramas_contagem;
    vector<string> trigramas_repetidos;

    // Encontra trigramas repetidos
    for (size_t i = 0; i < tamanho_criptograma - 2; i++) {
        string trigama = criptograma.substr(i, 3); // Extrai cada sequência de três letras
        trigramas_contagem[trigama]++;
        if (trigramas_contagem[trigama] == 2) {
            trigramas_repetidos.push_back(trigama);
        }
    }

    // Remove duplicatas, garantindo que o trigrama repetido seja listado só uma vez
    sort(trigramas_repetidos.begin(), trigramas_repetidos.end());
    trigramas_repetidos.erase(unique(trigramas_repetidos.begin(), trigramas_repetidos.end()), trigramas_repetidos.end());

    // Calcula todas as distâncias
    vector<int> todas_dist;
    for (const auto& trigama : trigramas_repetidos) {
        vector<int> dist = encontra_espacos(criptograma, trigama);
        todas_dist.insert(todas_dist.end(), dist.begin(), dist.end());
    }

    // Calcula a frequência de cada possível tamanho de chave
    vector<pair<int, int>> ranking_espacos;
    for (int i = 2; i <= max_chave; i++) {
        int count = count_if(todas_dist.begin(), todas_dist.end(), [i](int dist) { return dist % i == 0; });
        ranking_espacos.push_back({i, count});
    }

    // Ordena para obter os três tamanhos mais frequentes
    sort(ranking_espacos.begin(), ranking_espacos.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
        return a.second > b.second;
    });

    // Seleciona as três chaves mais prováveis
    pair<int, int> chave1 = ranking_espacos[0];
    pair<int, int> chave2 = ranking_espacos[1];
    pair<int, int> chave3 = ranking_espacos[2];

    // Compara a frequência entre as três chaves mais prováveis
    if (chave3.first > chave1.first && chave3.second * 1.2 > chave1.second) {
        cout << "Tamanho da chave estimado: " << chave3.first << " (Terceira chave mais frequente)\n";
        return chave3.first;
    } else if (chave2.first > chave1.first && chave2.second * 1.2 > chave1.second) {
        cout << "Tamanho da chave estimado: " << chave2.first << " (Segunda chave mais frequente)\n";
        return chave2.first;
    } else {
        cout << "Tamanho da chave estimado: " << chave1.first << " (Primeira chave mais frequente)\n";
        return chave1.first;
    }
}





void freq_attack(string criptograma, int tamanho_senha, int lingua) {
    // Função que tenta descobrir a senha usando análise de frequência
    // de letras em inglês ou português

    int tamanho_criptograma = 0; // Armazena o tamanho do texto criptografado
    int k = 0;                   // Número de letras em cada grupo
    string senha;                // Senha resultante após ataque

    // Frequência de letras no idioma inglês
    map <char, float> freq_ingles {{'a',8.167},{'b',1.492},{'c',2.782},{'d',4.253},{'e',12.702},{'f',2.228},
    {'g',2.015},{'h',6.094},{'i',6.966}, {'j',0.153},{'k',0.772}, {'l',4.025},{'m',2.406},{'n',6.749},{'o',7.507},
    {'p',1.929},{'q',0.095},{'r',5.987},{'s',6.327},{'t',9.056},{'u',2.758},{'v',0.978},{'w',2.36},{'x',0.15},
    {'y',1.974},{'z',0.074}};

    // Frequência de letras no idioma português
    map <char, float> freq_port {{'a',14.63},{'b',1.04},{'c',3.88},{'d',4.99},{'e',12.57},{'f',1.02},
    {'g',1.30},{'h',1.28},{'i',6.18}, {'j',0.4},{'k',0.02}, {'l',2.78},{'m',4.74},{'n',5.05},{'o',10.73},
    {'p',2.52},{'q',1.20},{'r',6.53},{'s',7.81},{'t',4.34},{'u',4.63},{'v',1.67},{'w',0.01},{'x',0.21},
    {'y',0.01},{'z',0.47}};

    // Frequência de letras encontradas no criptograma
    map <char, float> freq_criptograma {{'a',0},{'b',0},{'c',0},{'d',0},{'e',0},{'f',0},
    {'g',0},{'h',0},{'i',0}, {'j',0},{'k',0}, {'l',0},{'m',0},{'n',0},{'o',0},
    {'p',0},{'q',0},{'r',0},{'s',0},{'t',0},{'u',0},{'v',0},{'w',0},{'x',0},
    {'y',0},{'z',0}};

    // Remove caracteres não alfabéticos e calcula o tamanho do criptograma
    criptograma = filtro(criptograma);
    tamanho_criptograma = criptograma.size();

    // Calcula o número de letras por grupo, sem considerar o resto da divisão
    k = tamanho_criptograma / tamanho_senha;

    // Converte a frequência em uma porcentagem
    float t = 100 / static_cast<float>(k);

    // Itera sobre cada posição da chave
    for (int i = 0; i < tamanho_senha; i++) {
        
        // Zera as frequências do map `freq_criptograma` para esta posição da chave
        for (int j = 97; j < 123; j++) {
            freq_criptograma.at(j) = 0;
        }

        // Calcula a frequência de cada letra na posição `i` da chave
        for (int j = 0; j < k; j++) {
            int posicao = (j * tamanho_senha) + i;
            freq_criptograma.at(criptograma[posicao]) += t;
        }

        senha.push_back(0);  // Placeholder para a letra da senha nesta posição
        float min = 999999;  // Inicializa `min` com um valor alto para armazenar o mínimo

        // Tenta cada letra do alfabeto como possível letra da chave
        for (int l = 0; l < 26; l++) {
            float dif_total = 0;
            int index = 0;  // Usado para iterar sobre as letras de forma circular

            // Calcula a diferença de frequência para todas as letras
            for (int m = 'a'; m <= 'z'; m++) {
                
                // Define `index` como `m + l`, voltando ao início do alfabeto se ultrapassar 'z'
                if (m + l > 'z') {
                    index = m + l - 26;
                } else {
                    index = m + l;
                }

                // Calcula a diferença total para a letra da chave em inglês ou português
                if (lingua == 1) {  // Idioma inglês
                    dif_total += abs(freq_ingles.at(m) - freq_criptograma.at(index));
                } else {  // Idioma português
                    dif_total += abs(freq_port.at(m) - freq_criptograma.at(index));
                }
            }

            // Atualiza a letra da senha se essa diferença for a menor até agora
            if (dif_total < min) {
                min = dif_total;
                senha[i] = 'a' + l;  // Define a letra correspondente para esta posição da senha
            }
        }
    }

    // Exibe a senha resultante
    cout << endl << "Senha = " << senha << endl;

    // Decifra o criptograma usando a senha descoberta
    decifra(criptograma, senha);
}
