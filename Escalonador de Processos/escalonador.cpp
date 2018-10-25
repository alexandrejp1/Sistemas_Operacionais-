#include <iostream>
#include <stdio.h>
#include <vector>
#include <algorithm>

using namespace std;

#define QUANTUM 2  //usado no RR

struct Processo{
    int tempoChegada;
    int tempoPico;
    int tempoRestante; //somente usado no RR
};

vector <Processo> LeArquivo(FILE *arquivo){

    arquivo = fopen("entrada.txt", "rt"); //rt leitura
    int tempoChegada, tempoPico;

    vector <Processo> p;

    while(!feof(arquivo)){
        fscanf(arquivo,"%d %d", &tempoChegada, &tempoPico); 
        
        if(feof(arquivo)) break; //evitar a repetição da ult linha do arquivo 
        
        Processo auxiliar;
        auxiliar.tempoChegada = tempoChegada;
        auxiliar.tempoPico = tempoPico;
        auxiliar.tempoRestante = tempoPico;
        p.push_back(auxiliar);

    }

    //p.erase(p.end()-1); // apaga ultimo elemento do vector q ta sendo passado duas vezes 

    fclose(arquivo);    

    return p;
}


void FCFS(vector <Processo> listaProcessos){    
    float tempoRetorno = 0, tempoResposta = 0, tempoEspera = 0, tempoExec = 0;
    int tamLista = listaProcessos.size();

    for(int i=0 ; i<listaProcessos.size(); i++){
        Processo auxiliar = listaProcessos[i];
    
        if(auxiliar.tempoChegada > tempoExec){//caso a cpu fique ociosa sem nenhum processo, precisa-se atualizar o valor do processo que chegará
            tempoExec = auxiliar.tempoChegada;
        }

        tempoRetorno = tempoRetorno + tempoExec + auxiliar.tempoPico - auxiliar.tempoChegada;
        tempoResposta = tempoResposta + tempoExec - auxiliar.tempoChegada;//tempo de execução - tempo de chegada do processo em questão
        tempoEspera = tempoEspera + tempoExec - auxiliar.tempoChegada;//tempo execução - tempo de chegada do processo em questão
        tempoExec = tempoExec + auxiliar.tempoPico;//tempo em que a cpu esta em execução

    }

    float retornoMedio = (tempoRetorno/tamLista);
    float respostaMedia = (tempoResposta/tamLista);
    float esperaMedia = (tempoEspera/tamLista);

    cout << "FCFS " << retornoMedio << " " << respostaMedia << " " << esperaMedia << endl;    
   
}

bool ComparaPico(Processo x, Processo y){
    return(x.tempoPico <= y.tempoPico);
}

void SJF(vector <Processo> listaProcessos){
    float tempoEspera = 0, tempoRetorno = 0, tempoResposta = 0, tempoExec = 0;
    int tamLista = listaProcessos.size();
    int quantidadeProcessos = tamLista;

    vector <Processo> filaProntos;// Lista de processos prts pra ser executado

    while(quantidadeProcessos!=0){
        for(int i=0 ; i<listaProcessos.size() ; i++){
            if(listaProcessos[i].tempoChegada <= tempoExec){
                filaProntos.push_back(listaProcessos[i]);
                listaProcessos.erase(listaProcessos.begin()+i);
                i--;
            }
        }
        sort(filaProntos.begin(), filaProntos.end(), ComparaPico);

        Processo auxiliar = filaProntos[0];
        filaProntos.erase(filaProntos.begin());

   
        tempoResposta += tempoExec - auxiliar.tempoChegada;
        tempoEspera += tempoExec - auxiliar.tempoChegada;        
        tempoExec += auxiliar.tempoPico;  
        tempoRetorno += tempoExec - auxiliar.tempoChegada; //tempo de exec ja vai ser a soma de todos os picos até então  

        quantidadeProcessos--;
    }

    float retornoMedio = tempoRetorno / tamLista;
    float respostaMedia = tempoResposta / tamLista;
    float esperaMedia = tempoEspera / tamLista;

    cout << "SJF " << retornoMedio << " " << respostaMedia << " " << esperaMedia << endl;

}

void RR(vector <Processo> listaProcessos){
    float tempoEspera = 0, tempoRetorno = 0, tempoResposta = 0, tempoExec = 0;
    int tamLista = listaProcessos.size();
    int quantidadeProcessos = tamLista;
    int i;
    int tamanhoProntos;

    vector <Processo> filaProntos;

    Processo auxiliar;
    auxiliar.tempoChegada = 0; 
    auxiliar.tempoPico = 0;
    auxiliar.tempoRestante = 0;

    while(quantidadeProcessos!=0){
        for (i = 0; i < listaProcessos.size(); i++){
            if(listaProcessos[i].tempoChegada == tempoExec){//chegou no mesmo tempo que a execução estava, ja ta pronto
                filaProntos.push_back(listaProcessos[i]);
                listaProcessos.erase(listaProcessos.begin()+i);
                i--;
            }
            else if(listaProcessos[i].tempoChegada < tempoExec){//chegou mas esperou
                tempoEspera += tempoExec - listaProcessos[i].tempoChegada;//chegou antes do tempo de exec
                filaProntos.push_back(listaProcessos[i]);
                listaProcessos.erase(listaProcessos.begin()+i);
                i--;
            }
        }

        if(auxiliar.tempoRestante>0){ //se o processo ainda nao acabou sua execução
            filaProntos.push_back(auxiliar);
        }

        auxiliar = filaProntos[0];
        filaProntos.erase(filaProntos.begin());

        if(auxiliar.tempoRestante == auxiliar.tempoPico){ //primeira execucao do meu processo
            tempoResposta += tempoExec - auxiliar.tempoChegada;
        }

        tempoExec += QUANTUM;
        auxiliar.tempoRestante -=  QUANTUM;
        tamanhoProntos = filaProntos.size();
        tempoEspera += (QUANTUM * tamanhoProntos);//cada processo vai esperar 1 quantum caso ainda tenha restado

        if(auxiliar.tempoRestante<=0){//quando o proc encerra
            tempoExec += auxiliar.tempoRestante;
            tempoEspera += (auxiliar.tempoRestante * tamanhoProntos);//tirando o que nao foi esperado pelos procs
            tempoRetorno += tempoExec - auxiliar.tempoChegada; 
            quantidadeProcessos--;
        }


    }

    float retornoMedio = tempoRetorno / tamLista;
    float respostaMedia = tempoResposta / tamLista;
    float esperaMedia = tempoEspera / tamLista;
    
    //cout << tempoExec << endl;
    cout << "RR " << retornoMedio << " " << respostaMedia << " " << esperaMedia << endl;

}

int main(){
    FILE *arquivo;
    vector <Processo> a;
    vector <Processo> b;
    vector <Processo> c;

    a = LeArquivo(arquivo);
    b = LeArquivo(arquivo);
    c = LeArquivo(arquivo);
    
    FCFS(a);
    SJF(b);
    RR(c);
    
    //cout << a.size() << endl;
    return 0;
}
