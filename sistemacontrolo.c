#include <stdio.h>
#include "basedados.h"
#include <string.h>

/*As várias operações do sistema devem ter em conta as regras descritas abaixo, e apresentar mensagens de erro claras
 sempre que alguma opção não seja possível.
De acordo com o Regulamento da Oferta Formativa, cada estudante só pode estar inscrito a um máximo de 60 ECTS no 1º ano
 e 84 ECTS nos anos seguintes.
Cálculo do valor da propina – cada estudante pagará 15€ por cada ECTS,
 mais uma taxa de matrícula de 80€ se estiver a frequentar o 1º ano.
No caso de alunos residentes no estrangeiro, estes valores são agravados em 20%.
*/

int MAX_ECTS_PRIMEIRO_ANO = 60;
int MAX_ECTS_OUTROS_ANOS = 84;
int TAXA_POR_CADA_ECTS = 15;
int TAXA_DE_MATRICULA_DE_ALUNOS_DO_PRIMEIRO_ANO = 80;
float TAXA_DE_ALUNOS_NO_ESTRANGEIRO = 0.2f;


void calcular_propinas(int numero_aluno){
	FILE *file, *file_ucs, *file_inscs;
	int bufferLength = 255;
	char buffer[bufferLength];
	int count = 0;
    int ano_letivo = 0;
    int portugues = 1;
    char* numero = int_para_string(numero_aluno);

	file = fopen(FICHEIRO_ALUNOS, "r");
    
    while (fgets(buffer, bufferLength, file) != NULL) {
		char temp1[strlen(buffer)];
		strncpy(temp1, buffer, strlen(buffer));
		char temp2[strlen(buffer)];
		strncpy(temp2, buffer, strlen(buffer));

		if(compara_casos(busca_campo(temp1, 1), numero) == 0){
		    ++count;
            char* pais = busca_campo(temp2, 3);
            if(compara_casos(pais, "PT") == 0 || 
            compara_casos(pais, "Portugal") == 0 || 
            compara_casos(pais, "portugal") == 0 || 
            compara_casos(pais, "PORTUGAL") == 0 ){
                portugues = 0;
            }
            break;
        }
    }
	fclose(file);
	if(count == 0){
		printf("Aluno não encontrado na base de dados\n");
        return;
	}
    file_inscs = fopen(FICHEIRO_INSCRICOES, "r");

    int ucs[100] = { 0 };
    int index = 0;
    int skip = 1;
    int primeiro_elemento = 0;

    while (fgets(buffer, bufferLength, file_inscs) != NULL) {
		char temp1[strlen(buffer)];
		strncpy(temp1, buffer, strlen(buffer));
		char temp2[strlen(buffer)];
		strncpy(temp2, buffer, strlen(buffer));
		char temp3[strlen(buffer)];
		strncpy(temp3, buffer, strlen(buffer));
        //busca as UCs a que o aluno está inscrito e o ano letivo
		if(compara_casos(busca_campo(temp1, 1), numero) == 0){
            char* uc = busca_campo(temp2, 2);
            char* ano = busca_campo(temp3, 3);
            int uc_i = 0;
            int ano_i = 0;
            sscanf(uc, "%d", &uc_i);
            sscanf(ano, "%d", &ano_i);

            if(primeiro_elemento == 0){
                if(ano_i > ano_letivo)
                    ano_letivo = ano_i;
                ucs[index] =  uc_i;
                ++index;
                primeiro_elemento = 1;
            }
            else{
                //ve se a UC já está na lista de UCS
                skip = 1;
                for(int i = 0; i < index; i++){
                    if(ucs[i] == uc_i){
                        skip = 0;
                        break;
                    }
                }
                if(skip == 1){
                    if(ano_i > ano_letivo)
                        ano_letivo = ano_i;
                    ucs[index] =  uc_i;
                    ++index;
                }
            }
        }
    }
	fclose(file_inscs);
    if(index == 0){
        printf("O aluno não está inscrito a nenhuma unidade curricular, o total de ECTS é 0, portanto o custo das propinas é 0.\n\n");
        return;
    }

    file_ucs = fopen(FICHEIRO_UCS, "r");
    int ects = 0;

    while (fgets(buffer, bufferLength, file_ucs) != NULL) {
		char temp1[strlen(buffer)];
		strncpy(temp1, buffer, strlen(buffer));

        char* numero_uc = busca_campo(temp1, 1);

        for(int i = 0; i < index; i++){
            if(compara_casos(numero_uc, int_para_string(ucs[i])) == 0 ){
                char temp2[strlen(buffer)];
                strncpy(temp2, buffer, strlen(buffer));

                char* ects_c = busca_campo(temp2, 6);

                int ects_i = 0;
                sscanf(ects_c, "%d", &ects_i);
                //calcular quantidade de ECTS
                ects += ects_i;
            }
        }
    }
	fclose(file_ucs);

    int propinas = 0;

    if(ano_letivo == 1)
        propinas = ects * TAXA_POR_CADA_ECTS + TAXA_DE_MATRICULA_DE_ALUNOS_DO_PRIMEIRO_ANO;
    else
        propinas = ects * TAXA_POR_CADA_ECTS;

    if(portugues != 0){
        propinas += propinas * TAXA_DE_ALUNOS_NO_ESTRANGEIRO;
    }

    printf("O total de propinas do aluno é: %d€\n\n", propinas);
};
void aluno_terminou_curso(int numero_aluno){
    
    if(verifica_se_existe(numero_aluno, FICHEIRO_ALUNOS, 1) == 0){
		printf("Aluno não encontrado na base de dados\n");
		return;
	}

    FILE *file_inscs;
	int bufferLength = 255;
	char buffer[bufferLength];

    file_inscs = fopen(FICHEIRO_INSCRICOES, "r");

    while (fgets(buffer, bufferLength, file_inscs) != NULL) {
        //n aluno
		char temp1[strlen(buffer)];
		strncpy(temp1, buffer, strlen(buffer));
        //nota
		char temp2[strlen(buffer)];
		strncpy(temp2, buffer, strlen(buffer));

		if(compara_casos(busca_campo(temp1, 1), int_para_string(numero_aluno)) == 0){
            char* nota = busca_campo(temp2, 4);
            int nota_i = 0;
            sscanf(nota, "%d", &nota_i);
            //cadeira com nota não positiva
            if(nota_i < 10){                
                printf("O aluno ainda não terminou o curso.\n");
	            fclose(file_inscs);
                return;
            }
        }
    }
	fclose(file_inscs);

    printf("O aluno já terminou o curso.\n\n");

};
void calcular_media(int numero_aluno){

    if(verifica_se_existe(numero_aluno, FICHEIRO_ALUNOS, 1) == 0){
		printf("Aluno não encontrado na base de dados\n");
		return;
	}

    FILE *file_inscs;
	int bufferLength = 255;
    char buffer[bufferLength];

    int notas[100] = { 0 };
    int index = 0;

    file_inscs = fopen(FICHEIRO_INSCRICOES, "r");

    while (fgets(buffer, bufferLength, file_inscs) != NULL) {
        //n aluno
		char temp1[strlen(buffer)];
		strncpy(temp1, buffer, strlen(buffer));
        //n nota
		char temp2[strlen(buffer)];
		strncpy(temp2, buffer, strlen(buffer));

		if(compara_casos(busca_campo(temp1, 1), int_para_string(numero_aluno)) == 0){
            char* nota = busca_campo(temp2, 4);
            int nota_i = 0;
            sscanf(nota, "%d", &nota_i);
            notas[index] = nota_i;
            ++index;
        }
    }
	fclose(file_inscs);

    if(index == 0){
        printf("O aluno não está inscrito a nenhuma unidade curricular, a média final é 0.\n\n");
        return;
    }
    float media_final = 0;
    for(int i = 0; i < index; i++){
        media_final += (float)notas[i] / index;
    }
    printf("A média final é: %0.1f.\n", media_final);
};