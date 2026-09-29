#include <stdio.h>
#include <string.h>


typedef struct {
	int id;
	char descricao[100];
	char responsavel[50];
	int status;
	int excluido;
}Tarefa;

#define ARQUIVO "tarefas.txt"

void listarTarefas() {
    FILE *arq = fopen(ARQUIVO, "rb");
    Tarefa tarefas[100];
    int total = 0;
    int i, j;

    if (arq == NULL) {
        printf("Nenhuma tarefa cadastrada ainda.\n");
        return;
    }

    // le todas as tarefas ativas para o vetor
    Tarefa t;
    while (fread(&t, sizeof(Tarefa), 1, arq) == 1) {
        if (t.excluido == 0) {
            tarefas[total] = t;
            total++;
        }
    }
    fclose(arq);

    if (total == 0) {
        printf("Nenhuma tarefa cadastrada ainda.\n");
        return;
    }

    // ordenacao por selecao (bubble sort) com dois criterios
    for (i = 0; i < total - 1; i++) {
        for (j = 0; j < total - 1 - i; j++) {
            int troca = 0;

            // criterio 1: pendente (status 0) vem antes de concluido (status 1)
            if (tarefas[j].status > tarefas[j+1].status) {
                troca = 1;
            }
            // criterio 2: se status igual, ID decrescente
            else if (tarefas[j].status == tarefas[j+1].status &&
                     tarefas[j].id < tarefas[j+1].id) {
                troca = 1;
            }

            if (troca) {
                Tarefa aux = tarefas[j];
                tarefas[j] = tarefas[j+1];
                tarefas[j+1] = aux;
            }
        }
    }

    // impressao
    printf("\n%-5s %-30s %-15s %-10s\n", "ID", "DESCRICAO", "RESP", "STATUS");
    for (i = 0; i < total; i++) {
        printf("%-5d %-30s %-15s %-10s\n",
               tarefas[i].id,
               tarefas[i].descricao,
               tarefas[i].responsavel,
               tarefas[i].status == 0 ? "PENDENTE" : "CONCLUIDO");
    }
}

void editarTarefa() {
    FILE *arq = fopen(ARQUIVO, "rb");
    Tarefa tarefas[100];
    int total = 0;
    int idBusca, encontrado = -1;
    int i;

    if (arq == NULL) {
        printf("Nenhuma tarefa cadastrada ainda.\n");
        return;
    }

    Tarefa t;
    while (fread(&t, sizeof(Tarefa), 1, arq) == 1) {
        tarefas[total] = t;
        total++;
    }
    fclose(arq);

    printf("Digite o ID da tarefa que deseja editar: ");
    scanf("%d", &idBusca);

    for (i = 0; i < total; i++) {
        if (tarefas[i].id == idBusca && tarefas[i].excluido == 0) {
            encontrado = i;
            break;
        }
    }

    if (encontrado == -1) {
        printf("Tarefa nao encontrada!\n");
        return;
    }

    printf("Nova descricao: ");
    scanf(" %[^\n]", tarefas[encontrado].descricao);

    printf("Novo responsavel: ");
    scanf(" %[^\n]", tarefas[encontrado].responsavel);

    printf("Status (0 = pendente, 1 = concluido): ");
    scanf("%d", &tarefas[encontrado].status);

    // reescreve o arquivo inteiro com os dados atualizados
    arq = fopen(ARQUIVO, "wb");
    for (i = 0; i < total; i++) {
        fwrite(&tarefas[i], sizeof(Tarefa), 1, arq);
    }
    fclose(arq);

    printf("Tarefa atualizada com sucesso!\n");
}

void excluirTarefa() {
    FILE *arq = fopen(ARQUIVO, "rb");
    Tarefa tarefas[100];
    int total = 0;
    int idBusca, encontrado = -1;
    int i;

    if (arq == NULL) {
        printf("Nenhuma tarefa cadastrada ainda.\n");
        return;
    }

    Tarefa t;
    while (fread(&t, sizeof(Tarefa), 1, arq) == 1) {
        tarefas[total] = t;
        total++;
    }
    fclose(arq);

    printf("Digite o ID da tarefa que deseja excluir: ");
    scanf("%d", &idBusca);

    for (i = 0; i < total; i++) {
        if (tarefas[i].id == idBusca && tarefas[i].excluido == 0) {
            encontrado = i;
            break;
        }
    }

    if (encontrado == -1) {
        printf("Tarefa nao encontrada!\n");
        return;
    }

    tarefas[encontrado].excluido = 1;

    // reescreve o arquivo inteiro com a tarefa marcada como excluida
    arq = fopen(ARQUIVO, "wb");
    for (i = 0; i < total; i++) {
        fwrite(&tarefas[i], sizeof(Tarefa), 1, arq);
    }
    fclose(arq);

    printf("Tarefa excluida com sucesso!\n");
}
int gerarProximoID(){
		FILE *arq = fopen(ARQUIVO, "rb");
		Tarefa t;
		int maiorId = 0;
		
		if (arq == NULL){
			
			return 1; 
		}
		
		while (fread(&t,sizeof(Tarefa), 1, arq) == 1){
			if (t.id > maiorId){
				maiorId = t.id;
			}
		}
	fclose(arq);
	return maiorId +1;
}
void novaTarefa(){
	FILE *arq = fopen(ARQUIVO, "ab");
	Tarefa t;
	
	if (arq == NULL){
		printf("Erro ao abrir o arquivo!\n");
		return;	
	}
	
	t.id = gerarProximoID();
	
	printf("\n ----Nova Tarefa----\n");
	printf("Descrição: \n");
	scanf(" %[^\n]", t.responsavel);
	
	printf("Responsavel: ");
	scanf(" %[^\n]", t.responsavel);
	
	t.status = 0;
	t.excluido = 0;
	
	fwrite(&t, sizeof(Tarefa), 1, arq);
	fclose(arq);
	
	printf("Tarefa cadastrada com sucesso! ID: %d\n", t.id);
}


int main(){
	int opcao;
	
	do{
		printf("\n= Meu titulo = \n");
		printf("1) Nova Tarefa\n");
		printf("2) Listar Tarefas\n");
		printf("3) Editar Tarefa\n");
		printf("4) Excluir Tarefa\n");
		printf("0) Sair\n");
		printf("Escolha uma opção\n");
		scanf(" %d", &opcao);
		
		switch(opcao){
			case 1:
				novaTarefa();
				break;
			case 2:
				listarTarefas();
				break;
			case 3:
				editarTarefa();
				break;
			case 4:
				excluirTarefa();
				break;
			case 0:
				printf("Saindo...\n");
				break;
			default:
				printf("Opção inválida!\n");
				
		}
		
	}while(opcao != 0);
	return 0;
	
	
}
