#ifndef _SISTEMACONTROLO_H
#define _SISTEMACONTROLO_H

    extern int MAX_ECTS_PRIMEIRO_ANO;
    extern int MAX_ECTS_OUTROS_ANOS;
    extern int TAXA_POR_CADA_ECTS;
    extern int TAXA_DE_MATRICULA_DE_ALUNOS_DO_PRIMEIRO_ANO;
    extern float TAXA_DE_ALUNOS_NO_ESTRANGEIRO;


    void calcular_propinas(int numero_aluno);
    void aluno_terminou_curso(int numero_aluno);
    void calcular_media(int numero_aluno);

#endif