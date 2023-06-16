#include <iostream>
#include <windows.h>
#include <filesystem>
#include <map>
#include <string>
#include <tchar.h>
#include <conio.h>
//#include <shlwapi.h>

/* DOCUMENTACAO OFICIAL DA BIBLIOTECA
https://learn.microsoft.com/en-us/windows/win32/api/

CODIGO FONTE:
https://github.com/Gebarito/Terminal-C-
*/

namespace fs = std::filesystem;
using namespace std;

// Menus 
void menuInicial();
void opcoes();

// Gerenciadores de comandos
void criar();
void inserir();
void listar();
void apagar();
void renomear();
void mover();
void deletar();
void ajuda();
void ver();
void data();
void hora();
void limpar();
void sair();

//Auxiliares
void dataSemana();
bool criarArquivo();
bool criarDiretorio();
void RemoverExcecoesPastas(string foldername);
void RemoverExcecoesArquivos(string filename);
void InserirTextoArquivo(string text, string filename);

int main(){
    menuInicial();
    SetConsoleTitle(TEXT("MarcoSoft Whinderson"));
    while(true){
        opcoes();
    }

    return 0;
}

//Menus
void menuInicial(){
    cout << "----- MarcoSoft Whinderson (c) -----"<< endl;
    cout << "Digite \"AJUDA\" para obter uma lista dos comandos" << endl;
}
/*
    mapeia cada funcao como uma string e chama quando o usuario digita a string mapeada
*/
void opcoes(){
    map<string, void(*)()> opt;

    opt["CRIAR"] = criar;
    opt["INSERIR"] = inserir;
    opt["LISTAR"] = listar;
    opt["APAGAR"] = apagar;
    opt["RENOMEAR"] = renomear;
    opt["MOVER"] = mover;
    opt["DELETAR"] = deletar;
    opt["AJUDA"] = ajuda;
    opt["VER"] = ver;
    opt["DATA"] = data;
    opt["HORA"] = hora;
    opt["LIMPAR"] = limpar;
    opt["SAIR"] = sair;

    string cmd;
    cout << "\nDigite um comando: ";
    cin >> cmd;

    if(opt.count(cmd)){
        opt[cmd]();
    }else{
        cout << "O comando " << cmd << " nao existe." << endl;
        cout << "Digite \"AJUDA\" para obter uma lista dos comandos" << endl;
    }

}

//Gerenciador de comandos
//menu de gerenciamento de criacao ele identifica se quer criar um diretorio ou arquivo
//e chama a funcao auxiliar responsavel.
void criar(){
    string fileName;
    cin >> fileName;

    if(fileName == "-D"){
        bool HDir = criarDiretorio();
        if(!HDir){
            cout << "Houve um erro ao criar o diretorio: " << GetLastError() << endl;
        }else{
            cout << "O diretorio foi criado com sucesso." << endl;
        }
    }
    else if(fileName == "-A"){
        bool HFile = criarArquivo();
        if(!HFile){
            cout << "Houve um erro ao criar o arquivo: " << GetLastError() << endl;
        }else{
            cout << "O arquivo foi criado com sucesso." << endl;
        }
    }else{
        cout << "Insira um tipo valido";
        return;
    }
}
//Pede o nome do arquivo e limpa o buffer para chamar a funcao auxiliar que ira inserir
void inserir(){
    string text, filename;
    
    cin >> filename;
    cin.ignore();

    cout << "Digite o texto que deseja inserir:"<<endl;
    getline(cin,text);

    InserirTextoArquivo(text, filename);
}
//WIN32API lista os arquivos e direitorios mas ignora os arquivos do projeto, isto é
//arquivos como main.cpp, .vscode, a.exe e gitgnore, para que haja clareza em quais arquivos
//o usuario criou
void listar(){
    WIN32_FIND_DATA fd;
    HANDLE hFind = FindFirstFile("*.*", &fd);
    
    cout << "Arquivos originais da pasta como main.cpp serao ignorados." << endl;
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                // Se for um diretório
                string parametro(fd.cFileName);
                RemoverExcecoesPastas(parametro);
            } else {
                // Se for um arquivo
                string parametro(fd.cFileName);
                RemoverExcecoesArquivos(parametro);
            }
        } while (FindNextFile(hFind, &fd) != 0);
        FindClose(hFind);
    }   
}

//Apaga um diretorio(pasta) usando a biblioteca filesystem, essa biblioteca 
//pode causar conflitos com alguns compiladores mais antigos
void apagar(){
    string foldername;
    bool HDir;
    cout << "Digite o nome do diretorio que deseja remover: ";
    cin >> foldername;

    try{
        fs::remove_all(foldername);
        cout << "Diretorio removido" << endl;
    }catch(fs::filesystem_error& e){
        cout << "Erro ao remover diretorio e seu conteudo. Erro: " << e.what() << endl;
    }
}
//renomeia um arquivo usando o move file ele "move" o arquivo para mesma pasta mas cm outro nome
void renomear()
{
    wstring caminho;
    wstring novoNome;

    wcin.ignore();
    wcout << L"Digite o caminho para o arquivo: ";
    getline(wcin, caminho);

    wcout << L"Digite o novo nome: ";
    getline(wcin, novoNome);

    if (MoveFileW(caminho.c_str(), novoNome.c_str())){
        wcout << L"Arquivo renomeado com sucesso" << endl;
    }
    else{
        wcout << L"Houve ao renomear o arquivo/diretorio" << endl;        
    }
}

//para esse metodo funcionar origem deve ser posto do seguinte jeito
//C:\\pasta\\objeto
//C:\\pasta\\outrapasta\\objeto
//O nome DEVE ser colocado ao final do destino
//Ele usa o filesystem que causa alguns conflitos com compiladores mais antigos e variados
//no entanto apresenta mais estabilidade para usar arquivos.
void mover() {
    string origem, destino;
    
    cout << "Digite o caminho de origem: ";
    cin >> origem;

    cout << "Digite o caminho de destino: ";
    cin >> destino;

    if (!filesystem::exists(origem)){
        cout << "O caminho leva a um arquivo/diretorio que nao existe." << endl;
        return;
    }

    try {
    filesystem::rename(origem, destino);
    cout << "Arquivo ou diretorio movido com sucesso." << endl;
    } catch (const filesystem::filesystem_error& e) {
        cout << "Erro ao mover o arquivo ou diretorio: " << e.what() << endl;
    }
}

//deleta o arquivo se ele esta no mesmo repositorio
//o .c_str() eh pra converter a string em LPCSTR parametro usado majoritariamente em metodos do
//win32 api
void deletar(){
    string filename;
    bool HFile;
    cout << "Digite o nome e a extensao do arquivo que deseja remover: ";
    cin >> filename;
    
    HFile = DeleteFileA(filename.c_str());

    if(!HFile){
        cout << "Erro ao remover arquivo: " << GetLastError() << endl;
        return;
    }

    cout << "O arquivo foi removido com sucesso." << endl;
}

void ajuda(){
    cout << "\n";
    cout << "| Comando  |               Descricao                | \n";
    cout << "| -------- | ---------------------------------------| \n";
    cout << "| CRIAR    | -A para criar um arquivo -D para pastas| \n";
    cout << "| INSERIR  | Insere texto em um arquivo             | \n";
    cout << "| LISTAR   | Lista os arquivos/diretorios           |\n";
    cout << "| APAGAR   | Apaga diretorio                        |\n"; 
    cout << "| RENOMEAR | Renomeia um arquivo/diretorio          |\n";
    cout << "| MOVER    | Move um arquivo/diretorio              |\n";
    cout << "| DELETAR  | Deleta um arquivo                      |\n";
    cout << "| AJUDA    | Lista todos os comandos                |\n";
    cout << "| VER      | Imprime a versao do sistema            |\n";
    cout << "| DATA     | Imprime a data do sistema              |\n";
    cout << "| HORA     | Imprime a hora do sistema              |\n";
    cout << "| LIMPAR   | Limpa a tela                           |\n";
    cout << "| SAIR     | Finaliza o programa                    |\n";
}

//Metodo oficial da documentacao WIN32 sobre como pegar a versao do windows
//OSVERSIONINFO eh uma struct da API feita exclusivamente para isso eh feito do mesmo jeito
//que no cmd por exemplo
void ver(){
    OSVERSIONINFO osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&osvi);

    cout << "Versao do Windows: " << osvi.dwMajorVersion << "." << osvi.dwMinorVersion << "." << osvi.dwBuildNumber << endl;
    cout << "Terminal GRR (c) creditado a Joao Gebara. Rafael Abreu. Rhamza Mourad" << endl;

}

//Usa o metodo GetSystemTime para capturar o tempo numa STRUCT DA API (systemtime)
//ela eh usa de IF em uma linha para concatenar de forma correta a data
//eh usado um metodo auxiliar para puxar o dia da semana
void data(){
    SYSTEMTIME systime;
    string dt;
    GetSystemTime(&systime);
    
    dt = (systime.wDay < 10 ? "0" : "") + to_string(systime.wDay) + "/" + (systime.wMonth < 10 ? "0" : "") 
    + to_string(systime.wMonth) + "/" + to_string(systime.wYear);

    cout << dt << ", ";
    dataSemana();
}

//Usa o mesmo sistema do void data() para capturar o horario so muda as variaveis da struct SYSTEMTIME
void hora(){
    SYSTEMTIME st;
    GetLocalTime(&st); //Usa o fuso horario de DF, GetSystemTime() vai retornar o fuso horario dos EUA
    string hr = to_string(st.wHour) + ":" + to_string(st.wMinute) + ":" + to_string(st.wSecond) +
    "," + to_string(st.wMilliseconds) + ".";
    cout << "Hora atual: " << hr << endl;
}

//Metodo oficial da WIN32 API para limpar a tela, o codigo pode ser encontrado na documentacao oficial
void limpar(){
    HANDLE hstdout;
    hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    SMALL_RECT scrollRect;
    COORD scrollTarget;
    CHAR_INFO fill;


    // Get the number of character cells in the current buffer.
    if (!GetConsoleScreenBufferInfo(hstdout, &csbi))
    {
        return;
    }

    // Scroll the rectangle of the entire buffer.
    scrollRect.Left = 0;
    scrollRect.Top = 0;
    scrollRect.Right = csbi.dwSize.X;
    scrollRect.Bottom = csbi.dwSize.Y;

    // Scroll it upwards off the top of the buffer with a magnitude of the entire height.
    scrollTarget.X = 0;
    scrollTarget.Y = (SHORT)(0 - csbi.dwSize.Y);

    // Fill with empty spaces with the buffer's default text attribute.
    fill.Char.UnicodeChar = TEXT(' ');
    fill.Attributes = csbi.wAttributes;

    // Do the scroll
    ScrollConsoleScreenBuffer(hstdout, &scrollRect, NULL, scrollTarget, &fill);

    // Move the cursor to the top left corner too.
    csbi.dwCursorPosition.X = 0;
    csbi.dwCursorPosition.Y = 0;

    SetConsoleCursorPosition(hstdout, csbi.dwCursorPosition);

}

//limpa o buffer e pergunta se quer sair
void sair(){
    cin.ignore();
    char c;
    cout << "Voce realmente deseja sair? (S/N):";
    cin >> c;
    
    if(c=='S' || c=='s') ExitProcess(1);
    
    return;
}

//Auxiliares
//tal como data e hora usa o system time, ele pega os dias da semana no systemtime
//ele usa um switch para imprimir o respectivo dia da semana
void dataSemana(){
    SYSTEMTIME st;
    GetSystemTime(&st);
    int dia = st.wDayOfWeek;
    switch(dia){
        case 0:
            cout << "Domingo." << endl;
            break;
        case 1:
            cout << "Segunda-Feira." << endl;
            break;
        case 2:
            cout << "Terca-Feira." << endl;
            break;
        case 3:
            cout << "Quarta-Feira." << endl;
            break;
        case 4:
            cout << "Quinta-Feira." << endl;
            break;
        case 5:
            cout << "Sexta-Feira." << endl;
            break;
        case 6:
            cout << "Sabado." << endl;
            break;
    }

}

//Cria o arquivo na pasta do programa
//cria arquivo usando o metodo createfile diretamente da win32 api, eh necessario digitar a API
bool criarArquivo(){
    string nomeArquivo;
    cout << "Titulo do arquivo: ";
    cin >> nomeArquivo;
    
    HANDLE Hfile = CreateFile(nomeArquivo.c_str(),    //.c_str() para converter para um ponteiro de char
                          GENERIC_WRITE | GENERIC_READ,
                          0,             
                          0,             
                          CREATE_ALWAYS, // SOBRESCREVE O ARQUIVO SE EXISTIR
                          FILE_ATTRIBUTE_NORMAL,
                          0);
    CloseHandle(Hfile);
    return Hfile;  
}

//auxiliar do metodo criar como o criarArquivo, ele cria uma pasta e retorna sefuncionou ou nao
bool criarDiretorio(){
    string nomePasta;
    cout << "Titulo da pasta: ";
    cin >> nomePasta;

    if(CreateDirectory(nomePasta.c_str(),NULL)) return true;

    return false;
}

//remove as pastas que ja vem com o projeto, como o .vscode da IDE que eu usei
void RemoverExcecoesPastas(string foldername){
    if(foldername == "." || foldername == "..") return;
    if(foldername == ".git" || foldername == ".vscode") return;
    
    cout << "[DIR] " << foldername << endl;
}

//exibe apenas os txt para que nao seja exibido o .MD ou .exe
void RemoverExcecoesArquivos(string filename){
    if(filename.substr(filename.find_last_of(".")+1) == "txt")
        cout << "[ARQ] " << filename << endl;
}

//auxiliar que insere texto dentro de um arquivo, auxiliar da INSERIR
void InserirTextoArquivo(string text, string filename){
    HANDLE file;
    DWORD bytes;
    
    file = CreateFileA(filename.c_str(),
                        GENERIC_WRITE,
                        0,
                        nullptr,
                        OPEN_ALWAYS,
                        FILE_ATTRIBUTE_NORMAL,
                        nullptr);

    if(file == INVALID_HANDLE_VALUE){
        cout << "Erro ao abrir o arquivo, certifique-se de que ele existe no diretório e que o nome dele está correto";
        return;
    }

    SetFilePointer(file, 0, nullptr, FILE_END);
    
    if(!WriteFile(file, text.c_str(), strlen(text.c_str()), &bytes, NULL))
        cout << "Erro ao abrir o arquivo" << endl;
    

    cout << "Se o arquivo nao existir um novo sera criado com o texto" << endl;
    CloseHandle(file);
}