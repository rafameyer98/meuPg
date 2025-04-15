#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h> 


#include <string.h>


#define USER "Rafaela"
#define PASS "6587"
#define MAX_CLIENTES 100
#define MAX_TENTATIVAS 3
#define TAM_SENHA 50
#define TAM_CNPJ 30
#define TAM_TEL 15
#define TAM_CEP 10
#define TAM_DATA 11 
#define TAM_NOME 50
#define TAM_EMAIL 50

#define TAM_CHAVE 32
#define ARQUIVO_DADOS "meuPg.txt"
#define ARQUIVO_LOG "registro.log"
#define CHAVE_CRIPTO "MaisdoMesmo5487981587" 
#define INT_MAX


typedef struct {
   
    char nome_responsavel[50];
    char cpf_responsavel[15];
    char cargo_responsavel[TAM_NOME];
    char cpf[15];
    int id;
  
    
    char nome_fantasia[50];
    char nome_empresa[50];
    char razao_social[50];
    char cnpj[TAM_CNPJ];
    char data_abertura[TAM_DATA];
    char inscricao_estadual[50];

    char nome[20];
    char telefone[TAM_TEL];
    char email [TAM_EMAIL];
    char site [TAM_EMAIL];

     
     char rua[TAM_NOME];
     char numero[10];
     char complemento[30];
     char bairro[30];
     char cidade[30];
     char estado[3];
     char cep[TAM_CEP];

    //relatorio mensal 
    int mes;
    int ano;
    int qtd_residuos;

    int ativo;
 
    float valor_mensal;
    float total_aportes;
    int menor_producao;
    int maior_producao;
    float total_residuos;

    char regiao;
    char data_ultima_atualizacao[11]; // DD/MM/AAAA
} Cliente;


   Cliente clientes[MAX_CLIENTES];  
    int total_clientes; 


   // Função XOR para criptografia/descriptografia
void cifrarDados(char *dado) {
    if (!dado) return;
    
    int chave_len = strlen(CHAVE_CRIPTO);
    for (int i = 0; dado[i] != '\0'; i++) {
        dado[i] ^= CHAVE_CRIPTO[i % chave_len];
    }
}

int salvarDados() {
    
    FILE *arquivo = fopen(ARQUIVO_DADOS, "wb");
    if (!arquivo) {
        perror("Erro ao abrir arquivo de dados");
        return 0;
    }

    // Criptografa
    for (int i = 0; i < total_clientes; i++) {
        cifrarDados(clientes[i].cpf);
        cifrarDados(clientes[i].cnpj);
        cifrarDados(clientes[i].telefone);
    }

    // Salva dados
    int salvos = fwrite(clientes, sizeof(Cliente), total_clientes, arquivo);
    fclose(arquivo);

    // Descriptografa para uso na memória
    for (int i = 0; i < total_clientes; i++) {
        cifrarDados(clientes[i].cpf);
        cifrarDados(clientes[i].cnpj);
        cifrarDados(clientes[i].telefone);
    }

    //  log
    FILE *log = fopen(ARQUIVO_LOG, "a");
    if (log) {
        fprintf(log, "Dados salvos em %s - Registros: %d\n", __DATE__, total_clientes);
        fclose(log);
    }

    return salvos == total_clientes;
}


void carregarDados() {
    FILE *arquivo = fopen(ARQUIVO_DADOS, "rb");
    if (!arquivo) {
        perror("Aviso: Arquivo de dados não encontrado");
        return;
    }

  
    total_clientes = fread(clientes, sizeof(Cliente), MAX_CLIENTES, arquivo);
    fclose(arquivo);

    for (int i = 0; i < total_clientes; i++) {
        cifrarDados(clientes[i].cpf);
        cifrarDados(clientes[i].cnpj);
        cifrarDados(clientes[i].telefone);
    }
}



bool validarEmail(const char *email) {
    int tem_arroba = 0, tem_ponto = 0;
    for(int i = 0; email[i]; i++) {
        if(email[i] == '@') tem_arroba = 1;
        if(email[i] == '.' && tem_arroba) tem_ponto = 1;
    }
    return tem_arroba && tem_ponto;
}

bool validarCNPJ(const char *cnpj) {
    // Verificação básica de formato
    if (cnpj[2] != '.' || cnpj[6] != '.' || 
        cnpj[10] != '/' || cnpj[15] != '-') {
        return false;
    } for (int i = 0; i < 18; i++) {
        // Pula a verificação nas posições dos separadores
        if (i == 2 || i == 6 || i == 10 || i == 15) continue;
        
        if (!isdigit(cnpj[i])) {
            return false;
        }
    }

    return true;
}

void formatarTelefone(char *tel) {
    char formatado[17];
    if(strlen(tel) == 11) {
        sprintf(formatado, "(%c%c) %c%c%c%c%c-%c%c%c%c", 
                tel[0], tel[1], tel[2], tel[3], tel[4], 
                tel[5], tel[6], tel[7], tel[8], tel[9], tel[10]);
        strcpy(tel, formatado);
    }
}

// ========== CADASTRO COMPLETO ==========
void cadastrarCliente() {
    if(total_clientes >= MAX_CLIENTES) {
        printf("\nLimite de clientes atingido!\n");
        return;
    }
    
    Cliente novo;
    clientes[total_clientes].id = total_clientes + 1;
    
    printf("\n=== CADASTRO COMPLETO DE CLIENTE ===\n");
    
    // Dados do responsável
    printf("\n[ DADOS DO RESPONSÁVEL ]\n");
    printf("Nome completo: ");
    fgets(novo.nome_responsavel, TAM_NOME, stdin);
    novo.nome_responsavel[strcspn(novo.nome_responsavel, "\n")] = '\0';
    
    printf("CPF (XXX.XXX.XXX-XX): ");
    fgets(novo.cpf_responsavel, 15, stdin);
    novo.cpf_responsavel[strcspn(novo.cpf_responsavel, "\n")] = '\0';
    
    printf("Cargo: ");
    fgets(novo.cargo_responsavel, TAM_NOME, stdin);
    novo.cargo_responsavel[strcspn(novo.cargo_responsavel, "\n")] = '\0';
    
    // Dados jurídicos
    printf("\n[ DADOS JURÍDICOS ]\n");
    printf("Razão Social: ");
    fgets(novo.razao_social, TAM_NOME, stdin);
    novo.razao_social[strcspn(novo.razao_social, "\n")] = '\0';
    
    printf("Nome Fantasia: ");
    fgets(novo.nome_fantasia, TAM_NOME, stdin);
    novo.nome_fantasia[strcspn(novo.nome_fantasia, "\n")] = '\0';

    printf("CNPJ (digite sem pontuação): ");
    fgets(novo.cnpj, sizeof(novo.cnpj), stdin);
    novo.cnpj[strcspn(novo.cnpj, "\n")] = '\0'; 
    

    printf("Inscrição Estadual: ");
    fgets(novo.inscricao_estadual, 20, stdin);
    novo.inscricao_estadual[strcspn(novo.inscricao_estadual, "\n")] = '\0';
    
    printf("Data de abertura (DD/MM/AAAA): ");
    fgets(novo.data_abertura, TAM_DATA, stdin);
    novo.data_abertura[strcspn(novo.data_abertura, "\n")] = '\0';
    
    // Contato
    printf("\n[ DADOS DE CONTATO ]\n");

    int c;
    while ((c = getchar()) != '\n' && c != EOF);  

    printf("Telefone (apenas números): ");
    fgets(novo.telefone, TAM_TEL, stdin);
    novo.telefone[strcspn(novo.telefone, "\n")] = '\0';
    formatarTelefone(novo.telefone);
    
    do {
        printf("E-mail: ");
        fgets(novo.email, TAM_EMAIL, stdin);
        novo.email[strcspn(novo.email, "\n")] = '\0';
    } while(!validarEmail(novo.email));
    
    printf("Site: ");
    fgets(novo.site, TAM_EMAIL, stdin);
    novo.site[strcspn(novo.site, "\n")] = '\0';
    
    // Endereço
    printf("\n[ ENDEREÇO ]\n");
    printf("Logradouro: ");
    fgets(novo.rua, TAM_NOME, stdin);
    novo.rua[strcspn(novo.rua, "\n")] = '\0';
    
    printf("Número: ");
    fgets(novo.numero, 10, stdin);
    novo.numero[strcspn(novo.numero, "\n")] = '\0';
    
    printf("Complemento: ");
    fgets(novo.complemento, 30, stdin);
    novo.complemento[strcspn(novo.complemento, "\n")] = '\0';
    
    printf("Bairro: ");
    fgets(novo.bairro, 30, stdin);
    novo.bairro[strcspn(novo.bairro, "\n")] = '\0';
    
    printf("Cidade: ");
    fgets(novo.cidade, 30, stdin);
    novo.cidade[strcspn(novo.cidade, "\n")] = '\0';
    
    printf("Estado (UF): ");
    fgets(novo.estado, 3, stdin);
    novo.estado[strcspn(novo.estado, "\n")] = '\0';
    
    printf("CEP (XXXXX-XXX): ");
    fgets(novo.cep, TAM_CEP, stdin);
    novo.cep[strcspn(novo.cep, "\n")] = '\0';
    
    // Dados operacionais
    printf("\n[ DADOS OPERACIONAIS ]\n");
    printf("Valor mensal estimado (R$): ");
    scanf("%f", &novo.valor_mensal);
    
    printf("Quantidade de resíduos (ton): ");
    scanf("%d", &novo.qtd_residuos);
    
    getchar(); 
    
    // Data
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(novo.data_ultima_atualizacao, "%02d/%02d/%04d", 
            tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);


            novo.id = total_clientes + 1;
            clientes[total_clientes] = novo;
            total_clientes++;
            printf("Cliente cadastrado com ID %d!\n", novo.id); // Confirmação
   
   salvarDados();
} 


void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void carregarClientes() {
    FILE *arquivo = fopen("clientes.dat", "rb");
    if (arquivo) {
        total_clientes = fread(clientes, sizeof(Cliente), MAX_CLIENTES, arquivo);
        fclose(arquivo);
    }
}

void salvarClientes() {
    FILE *arquivo = fopen("clientes.dat", "wb");
    if (arquivo) {
        fwrite(clientes, sizeof(Cliente), total_clientes, arquivo);
        fclose(arquivo);
    }
}
void listarClientes() {
    if(total_clientes ==0){
        printf("nenhum cliente encontrado!\n");
        return;
    }
    printf("\n=== LISTA DE CLIENTES ===\n");
    for(int i = 0; i < total_clientes; i++) {
        if(clientes[i].ativo) {
            printf("\nID: %d\n", clientes[i].id);
            printf("Nome: %s\n", clientes[i].nome);
            printf("CNPJ: %s\n", clientes[i].cnpj);
            printf("Telefone: %s\n", clientes[i].telefone);
            printf("E-mail: %s\n", clientes[i].email);
        }
    }
    printf("\nTotal: %d clientes\n", total_clientes);
}

void editarCliente() {
    int id;
    printf("\nDigite o ID do cliente a editar: ");
    scanf("%d", &id);
    limparBuffer();

    for(int i = 0; i < total_clientes; i++) {
        if(clientes[i].id == id && clientes[i].ativo) {
            printf("\nEditando cliente ID %d\n", id);
            
            printf("Novo nome (%s): ", clientes[i].nome);
            fgets(clientes[i].nome, 100, stdin);
            clientes[i].nome[strcspn(clientes[i].nome, "\n")] = '\0';

            printf("Novo CNPJ (%s): ", clientes[i].cnpj);
            fgets(clientes[i].cnpj, 20, stdin);
            clientes[i].cnpj[strcspn(clientes[i].cnpj, "\n")] = '\0';

            printf("Cliente atualizado com sucesso!\n");
            salvarClientes();
            return;
        }
    }
    printf("Cliente não encontrado!\n");

 
}

void excluirCliente() {
    int id;
    printf("\nDigite o ID do cliente a excluir: ");
    scanf("%d", &id);

    for(int i = 0; i < total_clientes; i++) {
        if(clientes[i].id == id && clientes[i].ativo) {
            clientes[i].ativo = 0; // Exclusão lógica
            salvarClientes();
            printf("Cliente marcado como inativo!\n");
            return;
        }
    }
    printf("Cliente não encontrado!\n");
    salvarDados();
} 
void analisarClientes() {
    printf("\n=== ANÁLISE DE CLIENTES ===\n");
    
    // Exemplo: Contagem por domínio de email
    int gmail = 0, outlook = 0, outros = 0;
    
    for(int i = 0; i < total_clientes; i++) {
        if(clientes[i].ativo) {
            if(strstr(clientes[i].email, "@gmail.com")) gmail++;
            else if(strstr(clientes[i].email, "@outlook.com")) outlook++;
            else outros++;
        }
    }
    
    printf("\nDistribuição de e-mails:\n");
    printf("Gmail: %d (%.1f%%)\n", gmail, (float)gmail/total_clientes*100);
    printf("Outlook: %d (%.1f%%)\n", outlook, (float)outlook/total_clientes*100);
    printf("Outros: %d (%.1f%%)\n", outros, (float)outros/total_clientes*100);
    
    printf("Relatorio global");


int menor = clientes[0].qtd_residuos;

for(int i = 1; i < total_clientes; i++) {
    if(clientes[i].qtd_residuos < menor) {
        menor = clientes[i].qtd_residuos;
    }
}
printf("\nClientes com MENOR producao (%d toneladas):\n", menor);
for(int i = 0; i < total_clientes; i++) {
    if(clientes[i].qtd_residuos == menor) {
        printf("- %s\n", clientes[i].nome_empresa);
    }
}


float total = 0;
for(int i = 0; i < total_clientes; i++) {
    total += clientes[i].valor_mensal * 6;
}
printf("\nAporte financeiro SEMESTRAL: R$ %.2f\n", total);


int total_residuos = 0;
for(int i = 0; i < total_clientes; i++) {
    total_residuos += clientes[i].qtd_residuos;
}
printf("\nVolume TOTAL de residuos: %d toneladas\n", total_residuos);

}

void menuClientes() {
    int opcao;
    do {
        printf("\n=== GESTÃO DE CLIENTES ===\n");
        printf("1. Listar todos\n");
        printf("2. Analisar dados\n");
        printf("3. Editar cliente\n");
        printf("4. Excluir cliente\n");
        printf("5. Cadastrar novo cliente\n");
        printf("0. Voltar\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch(opcao) {
            case 1: listarClientes(); break;
            case 2: analisarClientes(); break;
            case 3: editarCliente(); break;
            case 4: excluirCliente(); break;
            case 5:cadastrarCliente(); break;
            case 0: printf("voltando\n");break;
            default:printf("opcao invalida\n");
        }
    } while(opcao != 0);
}



Cliente clientes[MAX_CLIENTES];
int total_clientes = 0;


int fazerLogin() {
    char usuario[50], senha[50];
    int tentativas = MAX_TENTATIVAS;
    
    while(tentativas > 0) {
        printf("\n=== LOGIN ===\n");
        printf("Usuário: ");
        scanf("%49s", usuario);
        printf("Senha: ");
        scanf("%49s", senha);
        
        
        while(getchar() != '\n');
        
        if(strcmp(usuario, USER) == 0 && strcmp(senha, PASS) == 0) {
            printf("\nAcesso permitido!\n");
            return 1;
        }
        printf("\nCredenciais inválidas. Tentativas restantes: %d\n", --tentativas);
    }
    printf("\nAcesso bloqueado.\n");
    return 0;
}


void getDataAtual(char *output) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(output, "%02d_%02d_%04d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);
}
// ========== GESTÃO DE CLIENTES =====
//void c=====adastrarCliente() {
  //  if(total_clientes >= MAX_CLIENTES) {
     //   printf("\nLimite de clientes atingido!\n");
       // return;
    //}
    
  

void atualizarDadosMensais() {
    if(total_clientes == 0) {
        printf("\nNenhum cliente cadastrado!\n");
        return;
    }
    
    printf("\n=== ATUALIZAÇÃO MENSAL ===\n");
    
    for(int i = 0; i < total_clientes; i++) {
        printf("\nCliente %d: %s (CNPJ: %s)\n", i+1,  
               clientes[i].nome_empresa, clientes[i].cnpj);
        
        printf("Quantidade de resíduos (ton): ");
        scanf("%d", &clientes[i].qtd_residuos);
        
        printf("Valor estimado (R$): ");
        scanf("%f", &clientes[i].valor_mensal);
        
        getDataAtual(clientes[i].data_ultima_atualizacao);
    }
    printf("\nDados atualizados com sucesso!\n");

    salvarDados();
}

void gerarRelatorio() {
    if(total_clientes == 0) {
        printf("\nNenhum cliente cadastrado!\n");
        return;
    }

    char nome_arquivo[50];
    char data[11];
    getDataAtual(data);
    sprintf(nome_arquivo, "relatorio_%s.txt", data);

    FILE *arquivo = fopen(nome_arquivo, "w");
    if(!arquivo) {
        printf("\nErro ao criar relatório!\n");
        return;
    }

    fprintf(arquivo, "=== RELATÓRIO DE CLIENTES ===\n");
    fprintf(arquivo, "Data: %s\n", data);
    fprintf(arquivo, "Total de clientes: %d\n", total_clientes);
    fprintf(arquivo, "================================\n\n");

    // Relatórios Individualizados
    int total_semestral = 0;
    float total_aporte = 0.0;
    Cliente *menor_cliente = NULL;
    int menor_residuo = 100000000;

    for(int i = 0; i < total_clientes; i++) {
        fprintf(arquivo, "CLIENTE %d\n", i+1);
        fprintf(arquivo, "Nome: %s\n", clientes[i].nome_empresa);
        fprintf(arquivo, "CNPJ: %s\n", clientes[i].cnpj);
        fprintf(arquivo, "Última atualização: %s\n", clientes[i].data_ultima_atualizacao);
        fprintf(arquivo, "Resíduos (ton): %d\n", clientes[i].qtd_residuos);
        fprintf(arquivo, "Valor mensal (R$): %.2f\n", clientes[i].valor_mensal);

        // cálculo dos relatórios individualizados
        total_semestral += clientes[i].qtd_residuos;
        total_aporte += clientes[i].valor_mensal * 6;

        if (clientes[i].qtd_residuos < menor_residuo) {
            menor_residuo = clientes[i].qtd_residuos;
            menor_cliente = &clientes[i];
        }

        fprintf(arquivo, "Gasto semestral estimado (R$): %.2f\n", clientes[i].valor_mensal * 6);
        fprintf(arquivo, "------------------------------\n\n");
    }

    // Relatório Global
    fprintf(arquivo, "====== RELATÓRIO GLOBAL ======\n");
    fprintf(arquivo, "Total de resíduos tratados no semestre: %d toneladas\n", total_semestral);
    fprintf(arquivo, "Aporte financeiro semestral estimado: R$ %.2f\n", total_aporte);

    if (menor_cliente) {
        fprintf(arquivo, "Indústria com menor produção:\n");
        fprintf(arquivo, "Nome: %s\n", menor_cliente->nome_empresa);
        fprintf(arquivo, "CNPJ: %s\n", menor_cliente->cnpj);
        fprintf(arquivo, "Resíduos no semestre: %d toneladas\n", menor_cliente->qtd_residuos);
    }

    // Se quiser incluir regiões, adicione um campo `char regiao[50];` em Cliente
    // e uma lógica parecida com a explicada antes.

    fclose(arquivo);
    printf("\nRelatório gerado em '%s'\n", nome_arquivo);

    salvarDados();
}


// ========== MENU PRINCIPAL ==========
void menuPrincipal1() {
    int opcao;
    
    do {
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("1. Cadastrar cliente\n");
        printf("2. Atualizar dados mensais\n");
        printf("3. Gerar relatório\n");
        printf("4. Sair\n");
        printf("Escolha: ");
        
        if(scanf("%d", &opcao   ) != 1) {
            printf("Opção inválida!\n");
            while(getchar() != '\n');
            continue;
        }
        while(getchar() != '\n');
        
        switch(opcao) {
            case 1:
                menuClientes();
                break;
            case 2:
                atualizarDadosMensais();
                break;
            case 3:
                gerarRelatorio();
                break;
            case 4:
                printf("\nEncerrando sistema...\n");
                exit(0);
            default:
                printf("\nOpção inválida!\n");
        }
    } while(1);
}

int main() {
  
     if(!fazerLogin()) {
        printf("Acesso negado. Saindo...\n");
        return 1;
    }

    carregarDados();
    carregarClientes();

   
    printf("Debug: %d clientes carregados\n", total_clientes);
    for(int i = 0; i < total_clientes; i++) {
        printf("Cliente %d: ID %d\n", i, clientes[i].id);
    }

   
    int opcao;
    do {
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("1. Gerenciar clientes\n");
        printf("2. Cadastrar novo cliente\n");
        printf("3. Gerar relatórios\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        
        if(scanf("%d", &opcao) != 1) {
            limparBuffer();
            printf("Entrada inválida!\n");
            continue;
        }
        limparBuffer();

        switch(opcao) {
            case 1: 
                menuClientes(); 
                break;
            case 2: 
                cadastrarCliente();
                break;
            case 3: 
                gerarRelatorio();
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while(opcao != 0);


    salvarDados();
    salvarClientes();
    
    return 0;

}