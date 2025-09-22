#include "sistema.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <stdbool.h>

#define MAX_SENHA_ARQUIVO 100
#define MAX_USUARIO_ARQUIVO 50
#define MAX_PRODUTO_ARQUIVO 100



void info_estoqueVoltar()
{
    info_estoque();
    voltar();
}
void alterar_quantidade( char *nome_produto, int nova_quantidade)
{
    FILE *f = fopen("estoque.txt", "r");
    FILE *temp = fopen("temp_Alterado_Estoque.txt", "w");
    char linha[500];
    int lim = 50;
    char setor_guardado[lim];
    char nome_guardado[lim];
    int qtd_guardada;

    if (f == NULL || temp == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    int encontrado = 0;

    while (fgets(linha, sizeof(linha), f) != NULL)
    {

        sscanf(linha, "%49[^:]:%49[^:]:%d",setor_guardado, nome_guardado,&qtd_guardada) ;

        if (strcmp(nome_guardado,nome_produto) == 0)
        {
            // Atualiza a quantidade
            qtd_guardada = nova_quantidade;
            encontrado = 1;

        }

        fprintf(temp, "%s:%s:%d\n", setor_guardado, nome_guardado,qtd_guardada);
    }

    fclose(f);
    fclose(temp);

    remove("estoque.txt");
    rename("temp_Alterado_Estoque.txt", "estoque.txt");

    printf("\n\nO que deseja fazer?\n\n");
    printf("[1] Voltar  [2] Continuar\n");

    char escolha = getch();


    if (escolha != '2')
    {
        menu(usuario);
    }
    else
    {
        limpar_tela();
        modificar_estoque();
    }

    if (!encontrado)
    {
        printf("Produto não encontrado.\n");

    }

}
void add_estoque(char *setor,char *nome,int quantidade)
{
    FILE *fpont;
    char linhas[MAX_PRODUTO_ARQUIVO + 3];
    fpont = fopen("estoque.txt", "a");
    if (fpont == NULL)
    {
        perror("Nao foi possivel abrir/criar o arquivo de produtos para adicionar");
        exit(1);
    }
    fprintf(fpont, "%s:%s:%d\n", setor, nome, quantidade);
    fclose(fpont);
    telas_carregamento();
    printf("Produto %s foi adiccionado com sucesso",nome);

    voltar();
    getch();
}
void del_estoque(char *nome)
{
    FILE *fpont, *temp;
    char linhas[MAX_PRODUTO_ARQUIVO + 3];
    bool encontrado = false;

    fpont = fopen("estoque.txt", "r");
    temp = fopen("temp.txt", "w");

    if (fpont == NULL || temp == NULL)
    {
        perror("Erro ao abrir os arquivos.");
        exit(1);
    }

    while (fgets(linhas, sizeof(linhas), fpont) != NULL)
    {
        char setor_guardado[50], nome_guardado[50];
        int qtd_guardada;

        if (sscanf(linhas, "%49[^:]:%49[^:]:%d",setor_guardado, nome_guardado,&qtd_guardada) == 3)
        {

            if (strcmp(nome_guardado, nome) != 0)
            {
                fprintf(temp, "%s:%s:%d\n",setor_guardado, nome_guardado,qtd_guardada);
            }
            else
            {
                encontrado = true;
            }
        }
    }

    fclose(fpont);
    fclose(temp);

    remove("estoque.txt");
    rename("temp.txt", "estoque.txt");


    if (encontrado)
    {
        telas_carregamento();
        printf("\nProduto '%s' removido com sucesso.\n", nome);
    }
    else
    {
        telas_carregamento();
        printf("\nProduto '%s' nao encontrado no estoque.\n", nome);
    }
    voltar();
    getch();

}

void info_estoque()
{
    FILE *fpont;
    const int lim = 50;
    int contador = 1;
    char linhas[MAX_PRODUTO_ARQUIVO + 3];

    fpont = fopen("estoque.txt", "r");
    if (fpont == NULL)
    {
        perror("Nao foi possivel abrir o arquivo de estoque");
        exit(1);
    }

    printf("\tEstoque\n\n");

    while (fgets(linhas, sizeof(linhas), fpont) != NULL)
    {
        char setor_guardado[lim];
        char nome_guardado[lim];
        int qtd_guardada;

        if (sscanf(linhas, "%49[^:]:%49[^:]:%d",setor_guardado, nome_guardado,&qtd_guardada) == 3)
        {

            printf("[%d] Setor: %s\nNome: %s\nQuantidade: %d\n\n",contador, setor_guardado, nome_guardado, qtd_guardada);
            contador++;
        }

    }

    fclose(fpont);

}
void criar_lista_compras()
{
    FILE *fpont, *lista;
    char linhas[MAX_PRODUTO_ARQUIVO + 3];
    const int limite_minimo = 3;
    bool algum = false;

    fpont = fopen("estoque.txt", "r");
    lista = fopen("lista_compras.txt", "w");

    if (fpont == NULL || lista == NULL)
    {
        perror("Erro ao abrir os arquivos.");
        exit(1);
    }


    int total_produtos = 0;
    while (fgets(linhas, sizeof(linhas), fpont) != NULL)
    {
        total_produtos++;
    }

    if (total_produtos == 0)
    {
        printf("\nNenhum produto encontrado no estoque.\n");
        fclose(fpont);
        fclose(lista);
        voltar();
        return;
    }

    rewind(fpont); // volta ao início do arquivo


    int escolha, contador = 1;
    char setor_guardado[50], nome_guardado[50];
    int qtd_guardada;

    while (1)
    {

        info_estoque();

        printf("\n\nDigite os numeros dos produtos que deseja adicionar a lista (0 para finalizar):\n");

        printf("Escolha: ");
        scanf("%d", &escolha);

        if (escolha == 0) break;

        rewind(fpont);
        contador = 1;
        while (fgets(linhas, sizeof(linhas), fpont) != NULL)
        {
            if (sscanf(linhas, "%49[^:]:%49[^:]:%d", setor_guardado, nome_guardado, &qtd_guardada) == 3)
            {
                if (contador == escolha)
                {
                    fprintf(lista, "Produto: %s   Quantidade: %d\n",nome_guardado, qtd_guardada);
                    algum = true;
                    printf("\n\nProduto '%s' adicionado a lista.\n", nome_guardado);
                    Sleep(1000);
                    limpar_tela();
                    break;
                }
                contador++;
            }
        }
    }

    fclose(fpont);
    fclose(lista);

    if (algum)
    {
        //exibir_lista_compras();
        printf("\n\nLista de compras criada com sucesso em 'lista_compras.txt'\n");
        Sleep(1500);
    }
    else
    {
        printf("\nNenhum produto foi selecionado para a lista.\n");
    }

    voltar();
    getch();
}

void exibir_lista_compras()
{
    FILE *f ;

    char linhas[MAX_PRODUTO_ARQUIVO + 3];
    f = fopen("lista_compras.txt", "r");

    printf("\n\n             LISTA DE COMPRAS\n");
    while (fgets(linhas, sizeof(linhas), f) != NULL)
    {
        printf("\n%s",linhas);
    }

    fclose(f);
    voltar();
}

void cadastro_usuario(char *usuario, char *senha)
{

    FILE *fpont;
    char linhas[MAX_USUARIO_ARQUIVO + MAX_SENHA_ARQUIVO + 3];


    fpont = fopen("usuarios.txt", "r");
    if (fpont != NULL)
    {
        while (fgets(linhas, sizeof(linhas), fpont) != NULL)
        {
            char nome_guardado[MAX_USUARIO_ARQUIVO];
            if (sscanf(linhas, "%[^:]", nome_guardado) == 1)
            {
                if (strcmp(usuario, nome_guardado) == 0)
                {
                    printf("\n\nUsuario %s ja existe.\n", usuario);
                    fclose(fpont);
                    Sleep(2000);
                    cadastro();
                    return;
                }
            }
        }
        fclose(fpont);
    }

    fpont = fopen("usuarios.txt", "a");
    if (fpont == NULL)
    {
        perror("Nao foi possivel abrir/criar o arquivo de usuarios para cadastro");
        exit(1);
    }

    fprintf(fpont, "%s:%s\n", usuario, senha);
    fclose(fpont);

    telas_carregamento();
    printf("\nUsuario %s cadastrado com sucesso!\n", usuario);
}


void login_usuario(char *usuario_digitado, char *senha_digitada)
{
    FILE *fpont;
    const int lim = MAX_USUARIO_ARQUIVO + MAX_SENHA_ARQUIVO + 3;
    char linhas[lim];
    char ch;
    bool logged_in;
    bool senha_correta;


    fpont = fopen("usuarios.txt", "r");
    if (fpont == NULL)
    {
        printf("Arquivo de usuarios nao encontrado ou sem permissoes.\n");
        return;
    }

    while (fgets(linhas, sizeof(linhas), fpont) != NULL)
    {
        char nome_guardado[MAX_USUARIO_ARQUIVO];
        char senha_guardada[MAX_SENHA_ARQUIVO];

        if (sscanf(linhas, "%49[^:]:%99[^\n]", nome_guardado, senha_guardada) == 2)
        {
            if (strcmp(usuario_digitado, nome_guardado) == 0)
            {
                logged_in = !logged_in;
                if (strcmp(senha_digitada, senha_guardada) == 0)
                {
                    Sleep(500);
                    printf("\nLogin bem-sucedido!\n\n");
                    Sleep(1500);
                    fclose(fpont);
                    menu(nome_guardado);
                    return;
                }
                else
                {
                    printf("\nSenha incorreta para o usuario %s.\n", usuario_digitado);
                    logged_in = false;
                    break;
                }
            }else{
                printf("\nUsuario '%s' nao encontrado.\n", usuario_digitado);
                break;
            }
        }
    }

    if (!logged_in)
    {
        printf("\nO que gostaria de fazer?\n\n");
        printf("[1] Tentar novamente\t [2] Voltar");
        ch = getch();
        if(ch == '1')
        {
            login();
        }
        else
        {
            limpar_tela();
            tela_inicial();
        }
    }


    fclose(fpont);
}
