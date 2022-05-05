#include <stdio.h>
#include <stdlib.h>
#include "interface.h"
#include "basedados.h"
#include "sistemacontrolo.h"
#include <string.h>
#include <unistd.h>

/*
A interface com o utilizador será baseada num menu de texto, com pelo menos as seguintes opções:
-Inscrever
-Anular
-Listar inscrições por UC e por aluno
-Calcular propinas de um aluno
-Gravar
-Sair
*/
void menu_principal(){
    limpar_ecra();
    printf("Escolha uma das opções listadas abaixo ao pressionar o numero associado a cada uma e depois enter.\n\n");
    printf("1 - Adicionar\n");
    printf("2 - Listar\n");
    printf("3 - Alterar dados\n");
    printf("4 - Apagar dados\n");
    printf("5 - Fazer cálculo\n");
    printf("6 - Gravar\n");
    printf("0 - Sair\n");

    switch (obter_escolha(6))
    {
        case 1:
            adicionar();
            break;
        case 2:
            listar();
            break;
        case 3:
            alterar();
            break;
        case 4:
            apagar();
            break;
        case 5:
            calcular();
            break;
        case 6:
            gravar();
            break;
        case 0:
            sair();
            break;
        default:
            sair();
            break;
    }
}

void adicionar(){
    printf("Escolha uma das opções listadas abaixo ao pressionar o numero associado a cada uma e depois enter.\n\n");
    printf("1 - Adicionar aluno\n");
    printf("2 - Adicionar Unidade Curricular\n");
    printf("3 - Inscrever aluno em Unidade Curricular\n");
    printf("4 - Voltar ao menu principal\n");
    printf("0 - Sair\n");

    switch(obter_escolha(4))
    {
        case 1:
            limpar_ecra();
            introduzir_aluno();
            break;
        case 2:
            limpar_ecra(); 
            introduzir_unidade_curricular();
            break;
        case 3:
            limpar_ecra();
            inscrever_aluno_em_unidade_curricular();
            break;
        case 4:
            limpar_ecra();
            menu_principal();
            break;
        case 0:
            limpar_ecra();
            sair();
            break;
        default:
            limpar_ecra();
            sair();
            break;
    } 
    adicionar();
};

void listar(){
    printf("Escolha uma das opções listadas abaixo ao pressionar o numero associado a cada uma e depois enter.\n\n");
    printf("1 - Listar alunos\n");
    printf("2 - Listar Unidades Curriculares\n");
    printf("3 - Listar inscrições\n");
    printf("4 - Voltar ao menu principal\n");
    printf("0 - Sair\n");

    switch(obter_escolha(4)){
        case 1:
            limpar_ecra();
            listar_alunos(FICHEIRO_ALUNOS);
            break;        
        case 2:
            limpar_ecra();
            listar_ucs(FICHEIRO_UCS);
            break;        
        case 3:
            limpar_ecra();
            listar_inscricoes(FICHEIRO_INSCRICOES);
            break;        
        case 4:
            limpar_ecra();
            menu_principal();
            break;
        case 0:
            limpar_ecra();
            sair();
            break;
        default:
            limpar_ecra();
            sair();
            break;        
    }
    listar();
};

void alterar(){
    printf("Escolha uma das opções listadas abaixo ao pressionar o numero associado a cada uma e depois enter.\n\n");
    printf("1 - Alterar dados de aluno\n");
    printf("2 - Alterar dados de Unidade Curricular\n");
    printf("3 - Alterar inscrição\n");
    printf("4 - Voltar ao menu principal\n");
    printf("0 - Sair\n");

    switch(obter_escolha(4)){
        case 1:
            limpar_ecra();
            alterar_aluno();
            break;        
        case 2:
            limpar_ecra();
            alterar_uc();
            break;        
        case 3:
            limpar_ecra();
            alterar_inscricao();
            break;        
        case 4:
            limpar_ecra();
            menu_principal();
            break;
        case 0:
            limpar_ecra();
            sair();
            break;
        default:
            limpar_ecra();
            sair();
            break;        
    }
    alterar();
};

void apagar(){
    printf("Escolha uma das opções listadas abaixo ao pressionar o numero associado a cada uma e depois enter.\n\n");
    printf("1 - Remover aluno\n");
    printf("2 - Remover Unidade Curricular\n");
    printf("3 - Remover Inscrição\n");
    printf("4 - Voltar ao menu principal\n");
    printf("0 - Sair\n");

    switch(obter_escolha(4)){
        case 1:
            apaga_aluno(obter_numero("Introduza o número do aluno a remover: ", "Tente um número válido: "));
            break;        
        case 2:
            apaga_uc(obter_numero("Introduza o número da unidade curricular a remover: ", "Tente um número válido: "));
            break;        
        case 3:
            apaga_inscricao(obter_numero("Introduza o número do aluno a remover: ", "Tente um número válido: "), obter_numero("Introduza o número da unidade curricular a remover: ", "Tente um número válido: "));
            break;        
        case 4:
            menu_principal();
            break;
        case 0:
            sair();
            break;
        default:
            sair();
            break;        
    }
    apagar();
};

void calcular(){
    printf("Escolha uma das opções listadas abaixo ao pressionar o numero associado a cada uma e depois enter.\n\n");
    printf("1 - Calcular propinas de um aluno\n");
    printf("2 - Determinar se aluno terminou o curso\n");
    printf("3 - Calcular média final de um aluno\n");
    printf("4 - Voltar ao menu principal\n");
    printf("0 - Sair\n");

    switch(obter_escolha(4)){
        case 1:
            calcular_propinas(obter_numero("Introduza o número do aluno: ", "Tente um número válido: "));
            break;        
        case 2:
            aluno_terminou_curso(obter_numero("Introduza o número do aluno: ", "Tente um número válido: "));
            break;        
        case 3:
            calcular_media(obter_numero("Introduza o número do aluno: ", "Tente um número válido: "));
            break;        
        case 4:
            menu_principal();
            break;
        case 0:
            sair();
            break;
        default:
            sair();
            break;        
    }
    calcular();
};

void gravar(){
    gravar_dados();
};

void sair(){
    //apaga ficheiros para descartar alterações
    if( access( FICHEIRO_ALTERA_ALUNOS_TEMP, F_OK ) == 0 ) 
		remove(FICHEIRO_ALTERA_ALUNOS_TEMP);
	
	if( access( FICHEIRO_ALTERA_UCS_TEMP, F_OK ) == 0 ) 
		remove(FICHEIRO_ALTERA_UCS_TEMP);
	
	if( access( FICHEIRO_ALTERA_INSCRICOES_TEMP, F_OK ) == 0 ) 
		remove(FICHEIRO_ALTERA_INSCRICOES_TEMP);
	
	if( access( FICHEIRO_INSERE_ALUNOS_TEMP, F_OK ) == 0 ) 
		remove(FICHEIRO_INSERE_ALUNOS_TEMP);
	
	if( access( FICHEIRO_INSERE_UCS_TEMP, F_OK ) == 0 ) 
		remove(FICHEIRO_INSERE_UCS_TEMP);
	
	if( access( FICHEIRO_INSERE_INSCRICOES_TEMP, F_OK ) == 0 ) 
		remove(FICHEIRO_INSERE_INSCRICOES_TEMP);
	
	if( access( FICHEIRO_REMOVE_ALUNOS_TEMP, F_OK ) == 0 ) 
		remove(FICHEIRO_REMOVE_ALUNOS_TEMP);
	
	if( access( FICHEIRO_REMOVE_UCS_TEMP, F_OK ) == 0 ) 
		remove(FICHEIRO_REMOVE_UCS_TEMP);
    

    printf("Tem a certeza que pretende sair? As alterações não gravadas irão ser descartadas.\n");
    printf("0 - Sim, quero sair.\n");
    printf("1 - Não, não quero sair.\n");

    switch (obter_escolha(1))
    {
    case 0:
        printf("Saiu da aplicação.\n");
        exit(0);
        break;
    case 1:
        menu_principal();    
    default:
        printf("Saiu da aplicação.\n");
        exit(0);
        break;
    }
};

void limpar_ecra(){
    system("clear"); 
}

int obter_escolha(int numero_de_escolhas){

    char escolha_c[50];
    int escolha = -1;
    int scan = 0;

    printf("\nFaça a sua escolha: ");
    fgets(escolha_c, 50, stdin);

    if(escolha_c != '\0'){
        scan = sscanf(escolha_c, "%d", &escolha);
    }
    while(escolha < 0 || escolha > numero_de_escolhas || scan != 1){
        printf("Tente uma escolha válida: ");
        fgets(escolha_c, 50, stdin);
        if(escolha_c != '\0'){
            scan = sscanf(escolha_c, "%d", &escolha);
        }
    }
    limpar_ecra();

    return escolha;
}
