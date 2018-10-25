#include <iostream>
#include <vector>
#include <stdio.h>
#include <algorithm>

using namespace std;

int numero_quadros; 

bool BuscaPaginas(int *array, int pagina){
	for(int i = 0; i < numero_quadros; i++){
		if(pagina == array[i]){
			return true;
		}
	}
	return false;
}

int BuscaPaginas2(vector <int> array, int pagina){ //usado parraya o OTIMO
	for(int i = 0; i < numero_quadros; i++){
		if(pagina == array[i]){
			return i;
		}
	}
	return -1; //se retornarray -1 significa que a pagina nao esta na memoriarrayia
}


vector <int> ReadFile(FILE *arquivo){
	arquivo = fopen("entrada.txt", "rt");
	int pAtual;

	vector<int> p;

	while(!feof(arquivo)){
		fscanf(arquivo, "%d", &pAtual);

		if(feof(arquivo)) 
			break;

		p.push_back(pAtual);
	}

	fclose(arquivo);

	numero_quadros = p[0];
	p.erase(p.begin());


	return p;

}

void FIFO(vector<int> listaPagina){
	int faltaPag = 0;
	int numero_paginas = listaPagina.size();
	int memoria[numero_quadros] = {};
	int contador = 0; //parraya simularray a fila circularray
	int z = 0;

	for(int x = 0; x < numero_quadros; x++){
		memoria[x] = -10;

	}


	if(numero_paginas == 0){
		cout << "Nao existem paginas" << endl;
	}

	for(int i = 0; i < numero_paginas; i++){
		if(!BuscaPaginas(memoria, listaPagina[i])){
			if(contador == numero_quadros){
				contador = 0;
			}
			memoria[contador] = listaPagina[i];
			faltaPag++;
			contador++; 
		}	
	}
	cout << "FIFO" << " " << faltaPag << endl;

}

void OTIMO(vector <int> listaPagina){
	int faltaPag = 0;
	int numero_paginas = listaPagina.size(); //tamanho da lista
	int memoria[numero_quadros] = {}; //arraymazena os valores que estao nos quadros
	int i = 0;

	for(int x = 0; x < numero_quadros; x++){
		memoria[x] = -10;

	}

	if(numero_paginas == 0){
		cout << "Nao existem paginas" << endl;
	}

	for(i = 0; i < numero_paginas; i++){
		if(faltaPag < numero_quadros && !BuscaPaginas(memoria, listaPagina[i])){
			memoria[faltaPag] = listaPagina[i];
			faltaPag++;
			continue;
		}else if(!BuscaPaginas(memoria, listaPagina[i])){
			vector <int> aux;

			for(int k = 0; k < numero_quadros; k++){
				aux.push_back(memoria[k]);
			}

			for(int j = i; j < numero_paginas; j++){
				int pos = BuscaPaginas2(aux, listaPagina[j]);
				if(pos >= 0 && aux.size() > 1){
					aux.erase(aux.begin()+pos);
				}
			}

			int pagRemoved = aux[0];
			for(int l = 0; l < numero_quadros; l++){
				if(memoria[l] == pagRemoved){
					memoria[l] = listaPagina[i];
				}
			}
			faltaPag++;
		}
	}
	
	cout << "OTIMO" << " " << faltaPag << endl;

}

void LRU(vector <int> listaPagina){
	int faltaPag = 0;
	int numero_paginas = listaPagina.size();
	int memoria[numero_quadros] = {};
	int contador = 0;
	int i;

	for(int x = 0; x < numero_quadros; x++){
		memoria[x] = -10;

	}

	if(numero_paginas == 0){
		cout << "Nao existem paginas" << endl;
	}

	for(i = 0; i < numero_paginas; i++){
		if(faltaPag < numero_quadros && !BuscaPaginas(memoria, listaPagina[i])){
			memoria[faltaPag] = listaPagina[i];
			faltaPag++;
			continue;
		}else if(!BuscaPaginas(memoria, listaPagina[i])){
			vector <int> aux;

			for(int k = 0; k < numero_quadros; k++){
				aux.push_back(memoria[k]);
			}

			for(int j = i; j > 0; j--){
				int pos = BuscaPaginas2(aux, listaPagina[j]);
				if(pos >= 0 && aux.size() > 1){
					aux.erase(aux.begin()+pos);
				}
			}

			int pagRemoved = aux[0];
			for(int l = 0; l < numero_quadros; l++){
				if(memoria[l] == pagRemoved){
					memoria[l] = listaPagina[i];
				}
			}
			faltaPag++;
		}
	}

	cout << "LRU" << " " << faltaPag << endl;

}



int main(){
	FILE *arquivo;

	vector <int> FIFO_SCHEDULER;
	vector <int> OTIMO_SCHEDULER;
	vector <int> LRU_SCHEDULER;
	

	FIFO_SCHEDULER = ReadFile(arquivo);
	OTIMO_SCHEDULER = ReadFile(arquivo);
	LRU_SCHEDULER = ReadFile(arquivo);
	

	FIFO(FIFO_SCHEDULER);
	OTIMO(OTIMO_SCHEDULER);
	LRU(LRU_SCHEDULER);

	return 0;
	
}
