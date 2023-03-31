#include <iostream>
#include <windows.h>
#include <map>
#include <string>
#include <tchar.h>
#include <conio.h>

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
void setPromptTitle();

int main(){

    menuInicial();
    setPromptTitle();
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
void criar(){
    string fileName;
    cout << "-D para criar uma pasta. -A para criar um arquivo" << endl;
    cin >> fileName;

    if(fileName == "-D"){

    }
    else if(fileName == "-A"){

    }else{
        cout << "Insira um tipo valido";
        return;
    }
}
void inserir(){
    cout << "inserir" <<endl;
}
void listar(){
    cout << "listar" <<endl;
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
    cout << "deletar" <<endl;
}
void ajuda(){
    cout << "| Comando  | Descricao                    | \n";
    cout << "| -------- | ---------------------------- | \n";
    cout << "| CRIAR    | Cria um arquivo (-A) ou diretorio (-D) | \n";
    cout << "| INSERIR  | Insere texto em um arquivo             | \n";
    cout << "| LISTAR   | Lista os arquivos/diretorios           |\n";
    cout << "| APAGAR   | Apaga um arquivo ou diretorio          |\n";
    cout << "| RENOMEAR | Renomeia um arquivo/diretorio          |\n";
    cout << "| MOVER    | Move um arquivo/diretorio              |\n";
    cout << "| DELETAR  | Deleta um arquivo/diretorio            |\n";
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
    dt = to_string(systime.wDay) + "/" + (systime.wMonth < 10 ? "0" : "") + to_string(systime.wMonth) + "/" + to_string(systime.wYear);
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

void setPromptTitle(){
    //TCHAR oldTitle_lenght[MAX_PATH];
    //TCHAR newTitle_lenght[MAX_PATH];
    
    try{
        SetConsoleTitle(TEXT("MarcoSoft Whinderson"));
    }catch(exception){

    }
}