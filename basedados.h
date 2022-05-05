#ifndef _BASEDADOS_H
#define _BASEDADOS_H

    extern char* FICHEIRO_ALUNOS;
    extern char* FICHEIRO_UCS;
    extern char* FICHEIRO_INSCRICOES;
    extern char* FICHEIRO_INSERE_ALUNOS_TEMP;
    extern char* FICHEIRO_INSERE_UCS_TEMP;
    extern char* FICHEIRO_INSERE_INSCRICOES_TEMP;
    extern char* FICHEIRO_ALTERA_ALUNOS_TEMP;
    extern char* FICHEIRO_ALTERA_UCS_TEMP;
    extern char* FICHEIRO_ALTERA_INSCRICOES_TEMP;
    extern char* FICHEIRO_REMOVE_ALUNOS_TEMP;
    extern char* FICHEIRO_REMOVE_UCS_TEMP;
    extern char* FICHEIRO_REMOVE_INSCRICOES_TEMP;

    struct Aluno{
        int numero;
        char nome[255];
        char pais[255];
    };
    struct UnidadeCurricular{
        int numero;
        char nome[255];
        int ano;
        int semestre;
        char area_cientifica[1];
        int ects;
    };
    struct Inscricao{
        int numero_de_aluno;
        int numero_de_unidade_curricular;
        int ano_letivo;
        int nota;
    };


    //funções de inserção
    void insere_aluno(struct Aluno aluno);
    void insere_unidade_curricular(struct UnidadeCurricular uc);
    void insere_inscricao(struct Inscricao insc);
    //função de consulta por ficheiro (de alunos, de ucs ou de inscrições)
    void listar_alunos();
    void listar_ucs();
    void listar_inscricoes();
    //funções de alteração
    void altera_aluno(struct Aluno aluno);
    void altera_unidade_curricular(struct UnidadeCurricular uc);
    void altera_inscricao(struct Inscricao nova_inscricao, int numero_uc_antiga);
    //funções de apagamento de dados
    void apaga_aluno(int numero_de_aluno);
    void apaga_uc(int numero_de_uc);
    void apaga_inscricao(int numero_de_aluno, int numero_de_uc);

    //funções de interação
    void introduzir_aluno();
    void introduzir_unidade_curricular();
    void inscrever_aluno_em_unidade_curricular();
    void alterar_aluno();
    void alterar_uc();
    void alterar_inscricao();

    //funções auxiliares
    int tamanho_aluno(struct Aluno al);
    int tamanho_uc(struct UnidadeCurricular uc);
    int tamanho_inscricao(struct Inscricao insc);
    int contar_digitos(int numero);
    char* busca_campo(char *linha, int num);
    char* int_para_string(int val);
    char **separa_palavras(const char *linha, char delim);
    int compara_casos(char* caso1, char* caso2);
    int busca_indice(char* str, char ch, int ocorrencia);
    int verifica_se_existe(int num, char* ficheiro, int elemento);
    void gravar_dados();    
    int obter_numero(char* frase, char* frase_loop);
    char* obter_nome(char* frase, char* frase_loop, int tamanho_maximo);
#endif