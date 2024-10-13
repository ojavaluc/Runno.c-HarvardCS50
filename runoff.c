#include <cs50.h>
#include <stdio.h>
#include <string.h>

#define MAX_CANDIDATES 9
#define MAX_VOTERS 100

// Preferências dos eleitores, onde preferences[i][j] é o j-ésimo candidato preferido do i-ésimo eleitor
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidatos têm nome, contagem de votos, e eliminado status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array de candidatos
candidate candidates[MAX_CANDIDATES];

// Número de eleitores e candidatos
int voter_count;
int candidate_count;

// Funções
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Verifica uso correto
    if (argc < 2)
    {
        printf("Usage: ./runoff [candidate ...]\n");
        return 1;
    }

    // Inicializa candidatos
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %d\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    // Obtem número de eleitores
    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %d\n", MAX_VOTERS);
        return 3;
    }

    // Mantém controle das preferências de todos os eleitores
    for (int i = 0; i < voter_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %d: ", j + 1);

            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }
    }

    // Continua tabulando até que seja encontrado um vencedor
    while (true)
    {
        // Calcular votos considerando as preferências dos eleitores ainda não eliminados
        tabulate();

        // Verifica se há um vencedor
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Elimina o candidato (ou candidatos) com o menor número de votos
        int min = find_min();
        bool tie = is_tie(min);

        // Se há empate, todos os candidatos restantes vencem
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Elimina candidatos em min
        eliminate(min);

        // Reinicia contagem de votos
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Registrar voto se válido
bool vote(int voter, int rank, string name)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i].name, name) == 0)
        {
            preferences[voter][rank] = i;
            return true;
        }
    }
    return false;
}

// Tabular votos para candidatos não eliminados
void tabulate(void)
{
    for (int i = 0; i < voter_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            int candidate_index = preferences[i][j];
            if (!candidates[candidate_index].eliminated)
            {
                candidates[candidate_index].votes++;
                break;
            }
        }
    }
}

// Imprimir vencedor, se houver
bool print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes > voter_count / 2)
        {
            printf("%s\n", candidates[i].name);
            return true;
        }
    }
    return false;
}

// Retornar o número mínimo de votos
int find_min(void)
{
    int min_votes = MAX_VOTERS;
    for (int i = 0; i < candidate_count; i++)
    {
        if (!candidates[i].eliminated && candidates[i].votes < min_votes)
        {
            min_votes = candidates[i].votes;
        }
    }
    return min_votes;
}

// Verificar se a eleição resultou em empate
bool is_tie(int min)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (!candidates[i].eliminated && candidates[i].votes != min)
        {
            return false;
        }
    }
    return true;
}

// Eliminar o candidato com o menor número de votos
void eliminate(int min)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes == min)
        {
            candidates[i].eliminated = true;
        }
    }
}
