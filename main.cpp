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
*/

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
void RemoverArquivo();
void RemoverDiretorio();

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

//Pode ser reescrito de forma melhor.
void criar(){
    string fileName;
    cout << "-D para criar uma pasta. -A para criar um arquivo" << endl;
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

void inserir(){
    cout << "inserir" <<endl;
}

void listar(){
    WIN32_FIND_DATA fd;
    HANDLE hFind = FindFirstFile("*.*", &fd);
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


void apagar(){
    cout << "apagar" <<endl;
}

void renomear(){
    cout << "renomear" <<endl;
}

void mover(){
    cout << "mover" <<endl;
}

void deletar(){
    char c;
    //inserir aspas em volta das letras
    cout << "Digite D para remocao de diretorio ou A para remocao de arquivos: ";
    cin >> c;

    if(c != 'D' && c != 'A'){
        cout << "O valor digitado nao eh valido" << endl;
    }

    if(c=='D'){
        RemoverDiretorio();
        return;
    }

    RemoverArquivo();
}

void ajuda(){
    cout << "\n";
    cout << "| Comando  |               Descricao                | \n";
    cout << "| -------- | ---------------------------------------| \n";
    cout << "| CRIAR    | Abre o menu de criacao arquivos/pastas | \n";
    cout << "| INSERIR  | Insere texto em um arquivo             | \n";
    cout << "| LISTAR   | Lista os arquivos/diretorios           |\n";
    cout << "| APAGAR   | Apaga um arquivo ou diretorio          |\n"; //tem apagar
    cout << "| RENOMEAR | Renomeia um arquivo/diretorio          |\n";
    cout << "| MOVER    | Move um arquivo/diretorio              |\n";
    cout << "| DELETAR  | Deleta um arquivo/diretorio            |\n";// e tambem tem deletar ?
    cout << "| AJUDA    | Lista todos os comandos                |\n";
    cout << "| VER      | Imprime a versao do sistema            |\n";
    cout << "| DATA     | Imprime a data do sistema              |\n";
    cout << "| HORA     | Imprime a hora do sistema              |\n";
    cout << "| LIMPAR   | Limpa a tela                           |\n";
    cout << "| SAIR     | Finaliza o programa                    |\n";
}

void ver(){
    OSVERSIONINFO osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&osvi);

    cout << "Versao do Windows: " << osvi.dwMajorVersion << "." << osvi.dwMinorVersion << "." << osvi.dwBuildNumber << endl;
    cout << "Terminal GRR (c) creditado a Joao Gebara. Rafael Abreu. Rhamza Mourad" << endl;

}

void data(){
    SYSTEMTIME systime;
    string dt;
    GetSystemTime(&systime);
    
    dt = (systime.wDay < 10 ? "0" : "") + to_string(systime.wDay) + "/" + (systime.wMonth < 10 ? "0" : "") 
    + to_string(systime.wMonth) + "/" + to_string(systime.wYear);

    cout << dt << ", ";
    dataSemana();
}

void hora(){
    SYSTEMTIME st;
    GetLocalTime(&st); //Usa o fuso horario de DF, GetSystemTime() vai retornar o fuso horario dos EUA
    string hr = to_string(st.wHour) + ":" + to_string(st.wMinute) + ":" + to_string(st.wSecond) +
    "," + to_string(st.wMilliseconds) + ".";
    cout << "Hora atual: " << hr << endl;
}

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

void sair(){
    char c;
    cout << "Voce realmente deseja sair? (S/N):";
    cin >> c;
    if(c != 'S' && c != 'N'){
        cout << "Opcao invalida" << endl;
        sair(); //Recursão até um Input Valido
    }
    if(c=='N') return;

    ExitProcess(1);
}

//Auxiliares
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
bool criarArquivo(){
    string nomeArquivo;
    cout << "Titulo do arquivo: ";
    cin >> nomeArquivo;
    nomeArquivo += ".txt"; //OU O USUARIO DIGITA A EXTENSAO? SO APAGAR ESSA LINHA DAI
    
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


bool criarDiretorio(){
    string nomePasta;
    //string _dir = "C:\\";
    cout << "Titulo da pasta: ";
    cin >> nomePasta;
    //_dir+=nomePasta;

    if(CreateDirectory(nomePasta.c_str(),NULL)) return true;

    return false;
}

void RemoverExcecoesPastas(string foldername){
    if(foldername == "." || foldername == "..") return;
    if(foldername == ".git" || foldername == ".vscode") return;
    
    cout << "[DIR] " << foldername << endl;
}

void RemoverExcecoesArquivos(string filename){
    if(filename.substr(filename.find_last_of(".")+1) == "txt")
        cout << "[ARQ] " << filename << endl;
}

void RemoverArquivo(){
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

void RemoverDiretorio(){
    string foldername;
    bool HDir;
    cout << "Digite o nome do diretorio que deseja remover: ";
    cin >> foldername;

    HDir = RemoveDirectoryA(foldername.c_str());

    if(!HDir){
        cout << "Erro ao remover diretorio: " << GetLastError() << endl;
        return;
    }

    cout << "O diretorio foi removido com sucesso" << endl;
}