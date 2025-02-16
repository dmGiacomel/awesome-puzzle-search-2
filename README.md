# awesome-puzzle-search
Simple library in C++ to apply search methods with various heuristics to solve the nm -1 puzzle and variants.





solver.cpp ====================================================================================

-------- para puzzles gerados a partir da seed ----------------
Uso -> ./solver <random_seed> <n_puzzles> <rounds_per_puzzle> <linhas> <colunas> <heuristica>
Ex: ./solver 42 100 10 4 4 man

-------- para puzzles setados manualmente -----------------
<random_seed> deve ter a string "manual". Nesse caso o programa lerá da entrada padrao puzzles
 embaralhados de tamanho <linhas> <colunas> a cada n_puzzle que for executar.

------------- para PDBS --------------------------
Antes da execucao da primeira busca, devem ser passados pela entrada padrao os tiles 
correspondentes ao pdb. O fim da lista de tiles deve ser indicado pelo caracter 'e'
Ex: ./solver 42 100 10 4 4 p
> 3 7 11 15 e
> ... rolo e confusao


--------------- para APDBS -----------------------
Antes da execucao da primeira busca, devem ser passados pela entrada padrao os tiles
correspondentes ao apdb. O fim de cada apdb deve ser indicado pelo caracter 'n'. O fim das
entradas dos APDBs deve ser indicado pelo caracter 'e'
Ex.: Ex: ./solver 42 100 10 4 4 p
> 3 7 t 11 15 t 1 2 6 e
> ... rolo e confusao
================================================================================================



No caso de heuristica ser a, dois campos a mais:



heuristicas: 
    a           -> APDB


    p           -> PDB
    man         -> manhattan
    mis         -> misplaced tile count 
    z           -> zero

