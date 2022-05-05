#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "basedados.h"
#include "sistemacontrolo.h"

//O módulo de base de dados deverá apresentar as funcionalidade básicas de consulta, inserção, alteração e apagamento de dados (CRUD).


char* FICHEIRO_ALUNOS = "lista_de_alunos.txt";
char* FICHEIRO_UCS = "lista_de_unidades_curriculares.txt";
char* FICHEIRO_INSCRICOES = "lista_de_inscricoes.txt";
char* FICHEIRO_INSERE_ALUNOS_TEMP = "temp_lista_de_alunos_insere.txt";
char* FICHEIRO_INSERE_UCS_TEMP = "temp_lista_de_unidades_curriculares_insere.txt";
char* FICHEIRO_INSERE_INSCRICOES_TEMP = "temp_lista_de_inscricoes_insere.txt";
char* FICHEIRO_ALTERA_ALUNOS_TEMP = "temp_lista_de_alunos_altera.txt";
char* FICHEIRO_ALTERA_UCS_TEMP = "temp_lista_de_unidades_curriculares_altera.txt";
char* FICHEIRO_ALTERA_INSCRICOES_TEMP = "temp_lista_de_inscricoes_altera.txt";
char* FICHEIRO_REMOVE_ALUNOS_TEMP = "temp_lista_de_alunos_remove.txt";
char* FICHEIRO_REMOVE_UCS_TEMP = "temp_lista_de_unidades_curriculares_remove.txt";
char* FICHEIRO_REMOVE_INSCRICOES_TEMP = "temp_lista_de_inscricoes_remove.txt";

//funções de inserção
void insere_aluno(struct Aluno aluno){

	if(verifica_se_existe(aluno.numero, FICHEIRO_ALUNOS, 1) == 1){ //1 - true; 0 - false
		printf("Aluno já existe na base de dados, tente editar\n\n");
		return;
	}
	//se não existe, adiciona na temp
	FILE *temp = fopen(FICHEIRO_INSERE_ALUNOS_TEMP, "a");
	fprintf(temp, "%d;%s;%s;\n", aluno.numero, aluno.nome, aluno.pais);
	fclose(temp);

	printf("Aluno preparado para inserir.\n\n");
}
void introduzir_aluno(){
	struct Aluno al;

	al.numero = obter_numero("Introduza o número do aluno: ", "Tente um número válido: ");

	strcpy(al.nome, obter_nome("Introduza o nome do aluno: ", "Tente um nome válido: ", 255));

	strcpy(al.pais, obter_nome("Introduza o país do aluno: ", "Tente um país válido: ", 100));

	insere_aluno(al);
}
void insere_unidade_curricular(struct UnidadeCurricular uc){

	if(verifica_se_existe(uc.numero, FICHEIRO_UCS, 1) == 1){
		printf("Unidade Curricular já existe na base de dados, tente editar\n\n");
		return;
	}

	//se não existe, adiciona-o na temp
	FILE *temp = fopen(FICHEIRO_INSERE_UCS_TEMP, "a");
	fprintf(temp, "%d;%s;%d;%d;%s;%d;\n", uc.numero, uc.nome, uc.ano, uc.semestre, uc.area_cientifica, uc.ects);
	fclose(temp);

	printf("Unidade curricular preparada para inserir.\n\n");
}
void introduzir_unidade_curricular(){
	struct UnidadeCurricular ac;	

	ac.numero = obter_numero("Introduza o número da Unidade curricular: ", "Tente um número válido: ");

	strcpy(ac.nome, obter_nome("Introduza o nome da Unidade curricular: ", "Tente um nome válido: ", 255));

	ac.ano = obter_numero("Introduza o ano da Unidade curricular: ", "Tente um ano válido: ");

	ac.semestre = obter_numero("Introduza o semestre da Unidade curricular: ", "Tente um número válido: ");

	strcpy(ac.area_cientifica, obter_nome("Introduza o nome da área científica da Unidade curricular: ", "Tente apenas um caractere ou um caractere válido: ", 1));

	ac.ects = obter_numero("Introduza o o número de ECTS da Unidade curricular: ", "Tente um número válido: ");

	insere_unidade_curricular(ac);
}
void insere_inscricao(struct Inscricao insc){

	if(verifica_se_existe(insc.numero_de_aluno, FICHEIRO_ALUNOS, 1) == 0){
		printf("\nAluno não existe na base de dados, tente adicionar primeiro.\n\n");
		return;
	}

	if(verifica_se_existe(insc.numero_de_unidade_curricular, FICHEIRO_UCS, 1) == 0){
		printf("\nUnidade Curricular não existe na base de dados, tente adicionar primeiro.\n\n");
		return;
	}
	if(verifica_se_existe(insc.numero_de_aluno, FICHEIRO_INSCRICOES, 1) == 1 && verifica_se_existe(insc.numero_de_unidade_curricular, FICHEIRO_INSCRICOES, 2) == 1 )
	{
		printf("\nInscrição já existe na base de dados, tente editar primeiro.\n\n");
		return;
	}

	FILE *temp, *file_ucs;
	int bufferLength = 255;
	char buffer[bufferLength];

	int ucs[50] = { insc.numero_de_unidade_curricular };
	int index = 1;

	int ects = 0;
	int ect = 0;

	int ano = 1;
	int ano_scan = 0;
	
	file_ucs = fopen(FICHEIRO_UCS, "r");

	while (fgets(buffer, bufferLength, file_ucs) != NULL) {
		//buscar ects
		char temp1[strlen(buffer)];
		strncpy(temp1, buffer, strlen(buffer));
		char temp2[strlen(buffer)];
		strncpy(temp2, buffer, strlen(buffer));
		char temp3[strlen(buffer)];
		strncpy(temp3, buffer, strlen(buffer));

		for(int i = 0; i < index; i++){
			if(compara_casos(busca_campo(temp1, 1), int_para_string(ucs[i])) == 0
			){
				sscanf(busca_campo(temp2, 3), "%d", &ano_scan);
				sscanf(busca_campo(temp3, 6), "%d", &ect);
				ects += ect;
				if(ano_scan > ano)
					ano = ano_scan;
			}
			ect = 0;
			ano_scan = 0;
		}
	}

	fclose(file_ucs);

	if(ano == 1 && ects > MAX_ECTS_PRIMEIRO_ANO){
		printf("Falha a adicionar inscrição, máximo de ECTS ultrapassado. ECTS: %d. Max ECTS: %d\n\n", ects, MAX_ECTS_PRIMEIRO_ANO);
		return;
	} else if(ano != 1 && ects > MAX_ECTS_OUTROS_ANOS){
		printf("Falha a adicionar inscrição, máximo de ECTS ultrapassado. ECTS: %d. Max ECTS: %d\n\n", ects, MAX_ECTS_OUTROS_ANOS);
		return;
	}

	temp = fopen(FICHEIRO_INSERE_INSCRICOES_TEMP, "a");
	//se não existe, adiciona-o na temp
	fprintf(temp, "%d;%d;%d;%d;\n", insc.numero_de_aluno, insc.numero_de_unidade_curricular, insc.ano_letivo, insc.nota);
	fclose(temp);

	printf("Inscrição preparada para inserir.\n\n");
}
void inscrever_aluno_em_unidade_curricular(){
	struct Inscricao insc;

	insc.numero_de_aluno = obter_numero("Introduza o número do aluno a inscrever: ", "Tente um número válido: ");

	insc.numero_de_unidade_curricular = obter_numero("Introduza o número do Unidade curricular a inscrever: ", "Tente um número válido: ");

	insc.ano_letivo = obter_numero("Introduza o ano letivo do aluno a inscrever: ", "Tente um número válido: ");

	insc.nota = obter_numero("Introduza a nota (0 a 20) do aluno a inscrever: ", "Tente um número válido: ");

	while(insc.nota < 0 || insc.nota > 20){
		insc.nota = obter_numero("A nota tem de ser entre 0 e 20, tente de novo: ", "Tente um número válido: ");
	}

	insere_inscricao(insc);
}

//função de consulta por ficheiro (de alunos, de ucs ou de inscrições)

void listar_alunos(){
	FILE *file;
	int bufferLength = 255;
	char buffer[bufferLength];
	file = fopen(FICHEIRO_ALUNOS, "r");
	int count = 0;
	while (fgets(buffer, bufferLength, file) != NULL) {
		char temp1[strlen(buffer)];
		strncpy(temp1, buffer, strlen(buffer));
		char temp2[strlen(buffer)];
		strncpy(temp2, buffer, strlen(buffer));
		char temp3[strlen(buffer)];
		strncpy(temp3, buffer, strlen(buffer));

		char* campo1 = busca_campo(temp1, 1);
		char* campo2 = busca_campo(temp2, 2);
		char* campo3 = busca_campo(temp3, 3);
		printf("Número: %s, Nome: %s, País: %s\n", campo1, campo2, campo3);
		++count;
    }
	printf("\n");
	if(count == 0){
		printf("A base de dados não tem alunos\n");
	}
	fclose(file);
};
void listar_ucs(){
	FILE *file;
	int bufferLength = 255;
	char buffer[bufferLength];
	file = fopen(FICHEIRO_UCS, "r");
	int count = 0;
	while (fgets(buffer, bufferLength, file) != NULL) {	
		char temp1[strlen(buffer)];
		strncpy(temp1, buffer, strlen(buffer));
		char temp2[strlen(buffer)];
		strncpy(temp2, buffer, strlen(buffer));
		char temp3[strlen(buffer)];
		strncpy(temp3, buffer, strlen(buffer));
		char temp4[strlen(buffer)];
		strncpy(temp4, buffer, strlen(buffer));
		char temp5[strlen(buffer)];
		strncpy(temp5, buffer, strlen(buffer));
		char temp6[strlen(buffer)];
		strncpy(temp6, buffer, strlen(buffer));

		char* campo1 = busca_campo(temp1, 1);
		char* campo2 = busca_campo(temp2, 2);
		char* campo3 = busca_campo(temp3, 3);
		char* campo4 = busca_campo(temp4, 4);
		char* campo5 = busca_campo(temp5, 5);
		char* campo6 = busca_campo(temp6, 6);
		printf("Número: %s, Nome: %s, Ano: %s, Semeste: %s, Area cientifica: %s, ECTS: %s\n", campo1, campo2, campo3, campo4, campo5, campo6);
		++count;

    }
	printf("\n");

	if(count == 0){
		printf("A base de dados não tem unidades curriculares\n");
	}
	fclose(file);
};
void listar_inscricoes(){
	FILE *file;
	int bufferLength = 255;
	char buffer[bufferLength];
	file = fopen(FICHEIRO_INSCRICOES, "r");
	int count = 0;
	while (fgets(buffer, bufferLength, file) != NULL) {		
		char temp1[strlen(buffer)];
		strncpy(temp1, buffer, strlen(buffer));
		char temp2[strlen(buffer)];
		strncpy(temp2, buffer, strlen(buffer));
		char temp3[strlen(buffer)];
		strncpy(temp3, buffer, strlen(buffer));
		char temp4[strlen(buffer)];
		strncpy(temp4, buffer, strlen(buffer));

		char* campo1 = busca_campo(temp1, 1);
		char* campo2 = busca_campo(temp2, 2);
		char* campo3 = busca_campo(temp3, 3);
		char* campo4 = busca_campo(temp4, 4);
		printf("Numero de aluno: %s, Numero de UC: %s, Ano letivo: %s, Nota: %s\n", campo1, campo2, campo3, campo4);
		++count;
		
    }
	printf("\n");
	if(count == 0){
		printf("A base de dados não tem inscrições\n");
	}
	fclose(file);
};

//funções de alteração
void altera_aluno(struct Aluno aluno){
	FILE *file_read, *temp_file;
	int bufferLength = 255;
	char buffer[bufferLength];
	char numero_al[contar_digitos(aluno.numero)];
	sprintf(numero_al, "%d", aluno.numero);
	int aluno_existe = 0;

	file_read = fopen(FICHEIRO_ALUNOS, "r");
	temp_file = fopen(FICHEIRO_ALTERA_ALUNOS_TEMP, "a");

	while (fgets(buffer, bufferLength, file_read) != NULL) {
		int len = strlen(buffer);
		if (len > 0 && buffer[len-1] == '\n') buffer[len-1] = '\0';
		char temp[strlen(buffer)];
		strcpy(temp, buffer); 
		if( compara_casos(busca_campo(temp, 1), numero_al) == 0){
			//atualiza o aluno pretendido no ficheiro temporario
			aluno_existe = 1;
			fprintf(temp_file, "%d;%s;%s\n", aluno.numero, aluno.nome, aluno.pais);
			continue;
		}
		//copia dados para ficheiro temporário
		fprintf(temp_file, "%s\n", buffer);
    }

	fclose(file_read);
	fclose(temp_file);

	if(aluno_existe == 1){
		printf("Aluno preparado para atualizar.\n");
	} else{
		printf("Aluno não existe. Tente adicionar primeiro.\n");
	}
}
void altera_unidade_curricular(struct UnidadeCurricular uc){
	FILE *file_read, *temp_file;
	int bufferLength = 255;
	char buffer[bufferLength];
	char numero_uc[contar_digitos(uc.numero)];
	sprintf(numero_uc, "%d", uc.numero);
	int uc_existe = 0;
	file_read = fopen(FICHEIRO_UCS, "r");
	temp_file = fopen(FICHEIRO_ALTERA_UCS_TEMP, "a");

	while (fgets(buffer, bufferLength, file_read) != NULL) {
		int len = strlen(buffer);
		if (len > 0 && buffer[len-1] == '\n') buffer[len-1] = '\0';
		char temp[strlen(buffer)];
		strcpy(temp, buffer); 
		if( compara_casos(busca_campo(temp, 1), numero_uc) == 0){
			//atualiza a uc pretendida
			uc_existe = 1;
			fprintf(temp_file, "%d;%s;%d;%d;%s%d\n", uc.numero, uc.nome, uc.ano, uc.semestre, uc.area_cientifica, uc.ects);
			continue;
		}
		//copia dados para ficheiro temporário
		fprintf(temp_file, "%s\n", buffer);
    }

	fclose(file_read);
	fclose(temp_file);

	if(uc_existe == 1){
		printf("Unidade curricular preparada para atualizar.\n");
	} else{
		printf("Unidade curricular não existe. Tente adicionar primeiro.\n");
	}
}
void altera_inscricao(struct Inscricao nova_inscricao, int numero_uc_antiga){
	//ve se aluno existe na tabela de alunos
	if(verifica_se_existe(nova_inscricao.numero_de_aluno, FICHEIRO_ALUNOS, 1) == 0){
		printf("Falha a alterar inscrição. Aluno não existe.\n");
		return;
	}

	//ve se uc a alterar existe na tabela de ucs
	if(verifica_se_existe(numero_uc_antiga, FICHEIRO_UCS, 1) == 0){
		printf("Falha a alterar inscrição. Unidade curricular não existe.\n");
		return;
	}

	//ve se aluno e uc a alterar existem na tabela de inscrições
	if( verifica_se_existe(nova_inscricao.numero_de_aluno, FICHEIRO_INSCRICOES, 1) == 0 && 
		verifica_se_existe(nova_inscricao.numero_de_unidade_curricular, FICHEIRO_INSCRICOES, 2) == 0){
		printf("O aluno não tem inscrição na unidade curricular que pretende alterar.\n");
		return;		
	}

	//altera inscrição
	FILE *file_read, *temp_file;
	int bufferLength = 255;
	char buffer[bufferLength];
	int aluno_existe = 0;
	int uc_existe = 0;
	int existem = 0;

	file_read = fopen(FICHEIRO_INSCRICOES, "r");
	temp_file = fopen(FICHEIRO_ALTERA_INSCRICOES_TEMP, "a");

	while (fgets(buffer, bufferLength, file_read) != NULL) {
		int len = strlen(buffer);
		if (len > 0 && buffer[len-1] == '\n') buffer[len-1] = '\0';
		char temp[strlen(buffer)];
		strcpy(temp, buffer); 
		char* numero_de_aluno = busca_campo(temp, 1);
		char* numero_de_uc = busca_campo(temp, 2);
		aluno_existe = 0;
		uc_existe = 0;

		char numero_aluno[contar_digitos(nova_inscricao.numero_de_aluno)];
		sscanf(numero_aluno, "%d", &nova_inscricao.numero_de_aluno);

		//verifica se o numero de aluno está na linha corrente
		if( compara_casos(numero_de_aluno, numero_aluno) == 0)
			aluno_existe = 1;

		
		char numero_uc[contar_digitos(numero_uc_antiga)];
		sscanf(numero_uc, "%d", &numero_uc_antiga);

		//verifica se o numero da uc antiga está na linha corrente
		if( compara_casos(numero_de_uc, numero_uc) == 0)
			uc_existe = 1;
		
		//escreve alteração para ficheiro temporário
		if(aluno_existe == 1 && uc_existe == 1){
			fprintf(temp_file, "%d;%d;%d;%d\n", nova_inscricao.numero_de_aluno, nova_inscricao.numero_de_unidade_curricular, nova_inscricao.ano_letivo, nova_inscricao.nota);
			existem = 1;
		}
		else
			fprintf(temp_file, "%s", temp);
    }

	fclose(file_read);
	fclose(temp_file);

	if(existem == 1){
		printf("Inscrição preparada para atualizar.\n");
	} else{
		printf("Inscrição não existe. Tente adicionar primeiro.\n");
	}
}

//funções de apagamento de dados
void apaga_aluno(int numero_de_aluno){
	FILE *file_read, *temp_file;
	int bufferLength = 255;
	char buffer[bufferLength];
	char numero_al[contar_digitos(numero_de_aluno)];
	sprintf(numero_al, "%d", numero_de_aluno);
	int aluno_existe = 0;

	file_read = fopen(FICHEIRO_ALUNOS, "r");
	temp_file = fopen(FICHEIRO_REMOVE_ALUNOS_TEMP, "a");

	while (fgets(buffer, bufferLength, file_read) != NULL) {
		int len = strlen(buffer);
		if (len > 0 && buffer[len-1] == '\n') buffer[len-1] = '\0';
		char temp[strlen(buffer)];
		strcpy(temp, buffer); 
		if( compara_casos(busca_campo(temp, 1), numero_al) == 0){
			//remove o aluno pretendido
			aluno_existe = 1;
			fprintf(temp_file, "%s\n", buffer);
			break;
		}
    }

	fclose(file_read);
	fclose(temp_file);

	if(aluno_existe == 1){
		printf("Aluno preparado para ser apagado\n");
	} else{
		printf("Aluno não existe. Tente adicionar primeiro.\n");
	}
}
void apaga_uc(int numero_de_uc){
	FILE *file_read, *temp_file;
	int bufferLength = 255;
	char buffer[bufferLength];
	char numero_uc[contar_digitos(numero_de_uc)];
	sprintf(numero_uc, "%d", numero_de_uc);
	int uc_existe = 0;

	file_read = fopen(FICHEIRO_UCS, "r");
	temp_file = fopen(FICHEIRO_REMOVE_UCS_TEMP, "a");

	while (fgets(buffer, bufferLength, file_read) != NULL) {
		int len = strlen(buffer);
		if (len > 0 && buffer[len-1] == '\n') buffer[len-1] = '\0';
		char temp[strlen(buffer)];
		strcpy(temp, buffer); 
		if( compara_casos(busca_campo(temp, 1), numero_uc) == 0){
			//remove a uc pretendida
			uc_existe = 1;
			fprintf(temp_file, "%s\n", buffer);
			break;
		}
    }

	fclose(file_read);
	fclose(temp_file);

	if(uc_existe == 1){
		printf("Unidade curricular preparada para ser apagada\n");
	} else{
		printf("Unidade curricular não existe. Tente adicionar primeiro.\n");
	}
}
void apaga_inscricao(int numero_de_aluno, int numero_de_uc){
	FILE *file_read, *temp_file;
	int bufferLength = 255;
	char buffer[bufferLength];
	char numero_al[contar_digitos(numero_de_aluno)];
	char numero_uc[contar_digitos(numero_de_uc)];
	sprintf(numero_al, "%d", numero_de_aluno);
	sprintf(numero_uc, "%d", numero_de_uc);
	int existem = 0;

	file_read = fopen(FICHEIRO_INSCRICOES, "r");
	temp_file = fopen(FICHEIRO_REMOVE_INSCRICOES_TEMP, "a");

	while (fgets(buffer, bufferLength, file_read) != NULL) {
		int len = strlen(buffer);
		if (len > 0 && buffer[len-1] == '\n') buffer[len-1] = '\0';
		char temp[strlen(buffer)];
		char temp2[strlen(buffer)];
		strcpy(temp, buffer); 
		strcpy(temp2, buffer); 
		if( compara_casos(busca_campo(temp, 1), numero_al) == 0 && compara_casos(busca_campo(temp2, 1), numero_uc) == 0){
			//remove a inscrição pretendida
			existem = 1;
			fprintf(temp_file, "%s\n", buffer);
			break;
		}
    }

	fclose(file_read);
	fclose(temp_file);

	if(existem == 1){
		printf("Inscrição preparada para ser apagada.\n");
	} else{
		printf("Inscrição não existe. Tente adicionar primeiro.\n");
	}
}

void alterar_aluno(){
	struct Aluno al;
	al.numero = obter_numero("Introduza o número do aluno a modificar: ", "Tente um número válido: ");
	char* numero_c = int_para_string(al.numero);
	FILE *file;
	int bufferLength = 255;
	char buffer[bufferLength];
	file = fopen(FICHEIRO_ALUNOS, "r");
	int count = 0;
	while (fgets(buffer, bufferLength, file) != NULL) {
		char temp1[strlen(buffer)];
		strncpy(temp1, buffer, strlen(buffer));

		if(compara_casos(busca_campo(temp1, 1), numero_c) == 0){
			++count;
			break;
		}
    }
	fclose(file);
	if(count == 0){
		printf("Aluno não encontrado na base de dados...\n");
	} else{
		strcpy(al.nome, obter_nome("Introduza o nome do aluno: ", "Tente um nome válido: ", 255));

		strcpy(al.pais, obter_nome("Introduza o país do aluno: ", "Tente um país válido: ", 255));
		
		altera_aluno(al);
	}
}
void alterar_uc(){
	struct UnidadeCurricular uc;
	uc.numero = obter_numero("Introduza o número da unidade curricular a modificar (existente): ", "Tente um número válido: ");
	char* numero_c = int_para_string(uc.numero);

	FILE *file;
	int bufferLength = 255;
	char buffer[bufferLength];
	file = fopen(FICHEIRO_UCS, "r");
	int count = 0;
	while (fgets(buffer, bufferLength, file) != NULL) {
		char temp1[strlen(buffer)];
		strncpy(temp1, buffer, strlen(buffer));

		if(compara_casos(busca_campo(temp1, 1), numero_c) == 0){
			++count;
			break;
		}
    }
	fclose(file);
	if(count == 0){
		printf("Unidade curricular não encontrada na base de dados...\n");
	} else{
		strcpy(uc.nome, obter_nome("Introduza o nome da unidade curricular: ", "Tente um nome válido: ", 255));

		uc.ano = obter_numero("Introduza o ano da unidade curricular: ", "Tente um ano válido: ");

		uc.semestre = obter_numero("Introduza o semestre da unidade curricular: ", "Tente um número válido: ");

		strcpy(uc.area_cientifica, obter_nome("Introduza a área científica da unidade curricular: ", "Tente introduzir apenas um caractere: ", 1));
		
		uc.ects = obter_numero("Introduza o número de ECTS da unidade curricular: ", "Tente um número válido: ");

		altera_unidade_curricular(uc);
	}
}
void alterar_inscricao(){

	struct Inscricao insc;
	insc.numero_de_aluno = obter_numero("Introduza o número do aluno na inscrição (existente): ", "Tente um número de aluno válido: ");
	char* numero_aluno_c = int_para_string(insc.numero_de_aluno);

	FILE *file_a, *file_uc;
	int bufferLength = 255;
	char buffer[bufferLength];

	file_a = fopen(FICHEIRO_ALUNOS, "r");

	int count_alunos = 0;

	while (fgets(buffer, bufferLength, file_a) != NULL) {
		char temp1[strlen(buffer)];
		strncpy(temp1, buffer, strlen(buffer));

		if(compara_casos(busca_campo(temp1, 1), numero_aluno_c) == 0){
			++count_alunos;
			break;
		}
	}
	fclose(file_a);

	if(count_alunos == 0){
		printf("Aluno não encontrado na base de dados...\n");
		return;
	}

	int numero_de_unidade_curricular_existente = obter_numero("Introduza o número da unidade curricular na inscrição (existente): ", "Tente um número válido: ");
	char* numero_uc_c = int_para_string(numero_de_unidade_curricular_existente);	
	
	int count_ucs = 0;
	
	file_uc = fopen(FICHEIRO_UCS, "r");
	while (fgets(buffer, bufferLength, file_uc) != NULL) {
		char temp1[strlen(buffer)];
		strncpy(temp1, buffer, strlen(buffer));

		if(compara_casos(busca_campo(temp1, 1), numero_uc_c) == 0){
			++count_ucs;
			break;
		}
	}

	fclose(file_uc);
	
	if(count_ucs == 0){
		printf("Unidade curricular não encontrada na base de dados...\n");
	} else{
		
		insc.numero_de_unidade_curricular = obter_numero("Introduza o número da nova unidade curricular: ", "Tente um número válido: ");

		insc.ano_letivo = obter_numero("Introduza o ano letivo a modificar na inscrição: ", "Tente um ano válido: ");

		insc.nota = obter_numero("Introduza a nota a modificar na inscrição: ", "Tente um número válido: ");

		altera_inscricao(insc, numero_de_unidade_curricular_existente);
	}
}

//funções auxiliares
int tamanho_aluno(struct Aluno al){
	//contar os digitos do numero de aluno, do tamanho do nome e do pais (+2 para separar por pontos e vírgulas)
	return contar_digitos(al.numero) + strlen(al.nome) + strlen(al.pais) + 2;
}
int tamanho_uc(struct UnidadeCurricular uc){
	//contar os digitos do numero da uc, do ano, do semeste, das ects, do nome, da area cientifica (+5 para separar por pontos e vírgulas)
	return contar_digitos(uc.numero) + contar_digitos(uc.ano) + contar_digitos(uc.semestre) + contar_digitos(uc.ects) + strlen(uc.nome) + 1 + 5;
}
int tamanho_inscricao(struct Inscricao insc){
	//contar os digitos do numero de aluno, da uc, do ano letivo e da nota (+3 para separar por pontos e vírgulas)
	return contar_digitos(insc.numero_de_aluno) + contar_digitos(insc.numero_de_unidade_curricular) + contar_digitos(insc.ano_letivo) + contar_digitos(insc.nota) + 3;
}
int contar_digitos(int numero){
	int digitos_count = numero;
	int count = 0;
	while (digitos_count != 0){
    	digitos_count /= 10;
    	++count;
  	}
	return count;
}
char* busca_campo(char *linha, int num)
{
	char* tok;
    for (tok = strtok(linha, ";"); tok && *tok; tok = strtok(NULL, ";"))
    {
        if (!--num){
			char *pos;
			if ((pos = strchr(tok, '\n')) != NULL)
				*pos = '\0';
            return tok;
		}
    }
    return NULL;
}
char* int_para_string(int val){
	
	static char buf[32] = {0};
	
	int i = 30;
	
	for(; val && i ; --i, val /= 10){	
		buf[i] = "0123456789abcdef"[val % 10];
	}
	return &buf[i+1];
	
}
char **separa_palavras(const char *linha, char delim)
{
    char **buf = (char**)malloc(2 * sizeof *buf);
    size_t pos, n = 0, len = 0;
    int inquote = 0;

    if (!buf) {
        return NULL;
    }

    buf[n] = NULL;

    /* Trim leading whitespace on the first field */
    for (pos = 0; (linha[pos] && linha[pos] == ' ') || linha[pos] == '\t'; pos++) {
        ++pos;
    }

    for (;; pos++) {
        char ch = linha[pos];

        if (!inquote && len == 0 && ch == '"') {
            /* Starting a quoted field */
            inquote = 1;
        }
        else if (inquote && ch == '"') {
            if (linha[pos + 1] != '"') {
                /* Terminating a quoted field */
                inquote = 0;

                /* Trim trailing whitespace on the field */
                while ((linha[pos + 1] && linha[pos + 1] == ' ') || linha[pos + 1] == '\t') {
                    ++pos;
                }
            }
            else {
                /* The quote was escaped so it doesn't count */
                buf[n] = (char*)realloc(buf[n], (++len + 1) * sizeof *buf[n]);
                buf[n][len - 1] = ch;
            }
        }
        else if (!inquote && (ch == delim || (!ch || ch == '\r' || ch == '\n'))) {
            /* Finalize the current field and prepare for the next field */
            buf = (char**)realloc(buf, (++n + 1) * sizeof *buf);
            buf[n - 1][len] = '\0';
            buf[n] = NULL;
            len = 0;

            if (ch == delim) {
                /* Trim leading whitespace on the next field */
                while ((linha[pos + 1] && linha[pos + 1] == ' ') || linha[pos + 1] == '\t') {
                    ++pos;
                }
            }
            else {
                /* Unquoted line break means we're done */
                break;
            }
        }
        else {
            buf[n] = (char*)realloc(buf[n], (++len + 1) * sizeof *buf[n]);
            buf[n][len - 1] = ch;
        }
    }

    buf[n] = NULL;

    return buf;
}
int compara_casos(char* caso1, char* caso2){
	int existe = 0;
	if( strncmp(caso1, caso2, strlen(caso1)) == 0){
		++existe;
	}
	if( strncmp(caso1, caso2, strlen(caso2)) == 0){
		++existe;
	}
	if(existe == 2)
		return 0;
	return 1;
}
int busca_indice(char* str, char ch, int ocorrencia)
{
    int occur = 0;
 
    // Loop para encontrar o n-ésimo caractere
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == ch) {
            occur += 1;
        }
        if (occur == ocorrencia)
            return i;
    }
    return -1;
}
int verifica_se_existe(int num, char* ficheiro, int elemento){
	FILE *file;
	char buffer[255];

	file = fopen(ficheiro, "r");

	while(fgets(buffer, 255, file) != NULL)
	{
		char temp1[strlen(buffer)];
		strncpy(temp1, buffer, strlen(buffer));
		char temp2[strlen(buffer)];
		strncpy(temp2, buffer, strlen(buffer));
		char* campo1 = busca_campo(temp1, 1);
		char* campo2 = busca_campo(temp2, 2);


		if(elemento == 1){
			if( compara_casos(int_para_string(num), campo1) == 0){	
				fclose(file);		
				return 1; // existe
			}
		}
		else if(elemento == 2){
			if( compara_casos(int_para_string(num), campo2) == 0){		
				fclose(file);	
				return 1; // existe;
			}
		}
	}
	fclose(file);
	return 0; // não existe;
}

void gravar_dados(){

	FILE *file, *temp;
	int bufferLength = 255;
	char buffer[bufferLength];

	//alterações
	if( access( FICHEIRO_ALTERA_ALUNOS_TEMP, F_OK ) == 0 ) {
		file = fopen(FICHEIRO_ALUNOS, "w");
		temp = fopen(FICHEIRO_ALTERA_ALUNOS_TEMP, "r");

		while (fgets(buffer, bufferLength, temp) != NULL) {
			fprintf(file, "%s", buffer);
		}

		fclose(file);
		fclose(temp);
		remove(FICHEIRO_ALTERA_ALUNOS_TEMP);
	}
	if( access( FICHEIRO_ALTERA_UCS_TEMP, F_OK ) == 0 ) {
		file = fopen(FICHEIRO_UCS, "w");
		temp = fopen(FICHEIRO_ALTERA_UCS_TEMP, "r");

		while (fgets(buffer, bufferLength, temp) != NULL) {
			fprintf(file, "%s", buffer);
		}

		fclose(file);
		fclose(temp);
		remove(FICHEIRO_ALTERA_UCS_TEMP);
	}
	if( access( FICHEIRO_ALTERA_INSCRICOES_TEMP, F_OK ) == 0 ) {
		file = fopen(FICHEIRO_INSCRICOES, "w");
		temp = fopen(FICHEIRO_ALTERA_INSCRICOES_TEMP, "r");

		while (fgets(buffer, bufferLength, temp) != NULL) {
			fprintf(file, "%s", buffer);
		}
		
		fclose(file);
		fclose(temp);
		remove(FICHEIRO_ALTERA_INSCRICOES_TEMP);
	}

	//inserções
	if( access( FICHEIRO_INSERE_ALUNOS_TEMP, F_OK ) == 0 ) {
		file = fopen(FICHEIRO_ALUNOS, "a");
		temp = fopen(FICHEIRO_INSERE_ALUNOS_TEMP, "r");

		while (fgets(buffer, bufferLength, temp) != NULL) {
			fprintf(file, "%s", buffer);
		}

		fclose(file);
		fclose(temp);
		remove(FICHEIRO_INSERE_ALUNOS_TEMP);
	}
	if( access( FICHEIRO_INSERE_UCS_TEMP, F_OK ) == 0 ) {
		file = fopen(FICHEIRO_UCS, "a");
		temp = fopen(FICHEIRO_INSERE_UCS_TEMP, "r");

		while (fgets(buffer, bufferLength, temp) != NULL) {
			fprintf(file, "%s", buffer);
		}

		fclose(file);
		fclose(temp);
		remove(FICHEIRO_INSERE_UCS_TEMP);
	}
	if( access( FICHEIRO_INSERE_INSCRICOES_TEMP, F_OK ) == 0 ) {
		file = fopen(FICHEIRO_INSCRICOES, "a");
		temp = fopen(FICHEIRO_INSERE_INSCRICOES_TEMP, "r");

		while (fgets(buffer, bufferLength, temp) != NULL) {
			fprintf(file, "%s", buffer);
		}
		
		fclose(file);
		fclose(temp);
		remove(FICHEIRO_INSERE_INSCRICOES_TEMP);
	}
	
	FILE *file_remove;
	char buffer2[bufferLength];
	int must_remove = 0;

	//remoções
	if( access( FICHEIRO_REMOVE_ALUNOS_TEMP, F_OK ) == 0 ) {
		file = fopen(FICHEIRO_ALUNOS, "r");
		temp = fopen(FICHEIRO_REMOVE_ALUNOS_TEMP, "r");
		file_remove = fopen("temporario_para_remover.txt", "w");

		while (fgets(buffer, bufferLength, file) != NULL) {
			while (fgets(buffer2, bufferLength, temp) != NULL) {
				//se estiver na lista de remoções, nao o escrever
				if(compara_casos(buffer, buffer2) == 0){
					must_remove = 1;
					break;
				}			
			}
			if(must_remove == 0){
				fprintf(file_remove, "%s", buffer);
			} 
			must_remove = 0;
		}

		fclose(file);
		fclose(temp);
		fclose(file_remove);
		
		file = fopen(FICHEIRO_ALUNOS, "w");
		file_remove = fopen("temporario_para_remover.txt", "r");
		
		while (fgets(buffer, bufferLength, file_remove) != NULL) {
			fprintf(file, "%s", buffer);			
		}
		fclose(file);
		fclose(file_remove);

		remove("temporario_para_remover.txt");
		remove(FICHEIRO_REMOVE_ALUNOS_TEMP);

	}
	if( access( FICHEIRO_REMOVE_UCS_TEMP, F_OK ) == 0 ) {
		file = fopen(FICHEIRO_UCS, "r");
		temp = fopen(FICHEIRO_REMOVE_UCS_TEMP, "r");
		file_remove = fopen("temporario_para_remover.txt", "w");

		while (fgets(buffer, bufferLength, file) != NULL) {
			while (fgets(buffer2, bufferLength, temp) != NULL) {
				//se estiver na lista de remoções, nao o escrever
				if(compara_casos(buffer, buffer2) == 0){
					must_remove = 1;
					break;
				}			
			}
			if(must_remove == 0){
				fprintf(file_remove, "%s", buffer);
			} 
			must_remove = 0;
		}

		fclose(file);
		fclose(temp);
		fclose(file_remove);
		
		file = fopen(FICHEIRO_UCS, "w");
		file_remove = fopen("temporario_para_remover.txt", "r");
		
		while (fgets(buffer, bufferLength, file_remove) != NULL) {
			fprintf(file, "%s", buffer);			
		}
		fclose(file);
		fclose(file_remove);

		remove("temporario_para_remover.txt");
		remove(FICHEIRO_REMOVE_UCS_TEMP);
	}
	if( access( FICHEIRO_REMOVE_INSCRICOES_TEMP, F_OK ) == 0 ) {
		file = fopen(FICHEIRO_INSCRICOES, "r");
		temp = fopen(FICHEIRO_REMOVE_INSCRICOES_TEMP, "r");
		file_remove = fopen("temporario_para_remover.txt", "w");

		while (fgets(buffer, bufferLength, file) != NULL) {
			while (fgets(buffer2, bufferLength, temp) != NULL) {
				//se estiver na lista de remoções, nao o escrever
				if(compara_casos(buffer, buffer2) == 0){
					must_remove = 1;
					break;
				}			
			}
			if(must_remove == 0){
				fprintf(file_remove, "%s", buffer);
			} 
			must_remove = 0;
		}

		fclose(file);
		fclose(temp);
		fclose(file_remove);
		
		file = fopen(FICHEIRO_INSCRICOES, "w");
		file_remove = fopen("temporario_para_remover.txt", "r");
		
		while (fgets(buffer, bufferLength, file_remove) != NULL) {
			fprintf(file, "%s", buffer);			
		}
		fclose(file);
		fclose(file_remove);

		remove("temporario_para_remover.txt");
		remove(FICHEIRO_REMOVE_INSCRICOES_TEMP);
	}

	printf("Alterações gravadas.\n\n");
}

int obter_numero(char* frase, char* frase_loop){

    char escolha_c[50];
    int escolha = -1;
    int scan = 0;
	char* resultado;

    printf("%s", frase);
    resultado = fgets(escolha_c, 50, stdin);

    if(escolha_c != '\0'){
        scan = sscanf(resultado, "%d", &escolha);
    }

    while(scan != 1){
        printf("%s", frase_loop);
        resultado = fgets(escolha_c, 50, stdin);
        if(escolha_c != '\0'){
            scan = sscanf(resultado, "%d", &escolha);
        }
    }
	fflush(stdin);
    return escolha;
}

char* obter_nome(char* frase, char* frase_loop, int tamanho_maximo){
	
	char *arr = (char *)malloc(255*sizeof(char));
	char nome[255];
	char* resultado;
	
    printf("%s", frase);
    resultado = fgets(nome, 255, stdin);

    while(strlen(resultado) < 1 || strlen(resultado) > tamanho_maximo + 1 || nome[0] == '\0' || nome[0] == '\n'){
        printf("%s", frase_loop);
        resultado = fgets(nome, 255, stdin);
    }

	strcpy(arr, resultado);
	fflush(stdin);

	arr[strcspn(arr, "\n")] = 0;

    return arr;
}