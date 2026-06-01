# TCC — Otimização de Sequenciamento de Produção

![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-%23008FBA.svg?style=for-the-badge&logo=cmake&logoColor=white)
![OR-Tools](https://img.shields.io/badge/OR--Tools-%231a73e8?style=for-the-badge&logo=google&logoColor=white)

### Barbosa Minerals | UFMG - Engenharia de Produção 2026
 
Implementação computacional do modelo de otimização desenvolvido no Trabalho de Conclusão de Curso **"Otimização da Alocação de Pedidos de Peças de Borracha no Setor de Mineração"**.
 

---
 
## Sobre o projeto
 
O modelo resolve o problema de sequenciamento da produção de peças de borracha da Barbosa Minerals, respeitando:
 
- Disponibilidade limitada de matrizes metálicas (recurso gargalo)
- Capacidade diária de operadores
- Prazos de entrega e prioridade de pedidos/clientes
- Minimização do total de atrasos ponderados
---
 
## Ferramentas utilizadas
 
- C++ - Linguagem de programação utilizada
- Google OR-Tools - Biblioteca de otimização
- CP-SAT - Solver de otimização
- CMake - Gerenciador de compilação
- CPM.cmake - Gerenciador de dependências
 
---
 
## Estrutura do código
 
O projeto foi desenvolvido em **C++ com Programação Orientada a Objetos** e com arquitetura em camadas (model, repository, optimizer) visando um código organizado e encapsulado, preparado para uma possível futura incorporação em um software desktop com interface gráfica em **Qt**.
 
```
tcc-20262/
├── src/
│   ├── main.cpp                        # Runner principal
│   │   
│   ├── model/
│   │   ├── order.h                     # Classe Pedido
│   │   ├── mold.h                      # Classe Matriz
│   │   ├── date.h                      # Classe Data
│   │   └── problem_data.h              # Classe para armazenar dados do problema (parâmetros)
│   │   
│   ├── repository/
│   │   ├── order_repository.h          # Repositório de pedidos
│   │   └── mold_repository.h           # Repositório de matrizes
│   │   
│   └── optimizer/
│       └── schedule_optimizer.h        # Modelo matemático e solver
│
└── tests/
    ├── main.cpp                        # Runner de testes
    ├── instances/
    │   └── test_instances.h            # Instâncias de teste
    └── results/
        └── tests.txt                   # Resultados dos testes já executados
```
 
---
 
## Resultados dos testes
 
Os resultados das instâncias de teste já executadas estão disponíveis em:
 
[`tests/results/tests.txt`](tests/results/tests.txt)
 
---
 
## Como compilar e executar
 
**Pré-requisitos:** CMake 3.24+, compilador C++20 (GCC 11+ ou Clang 13+).

1. Clonar o repositório
    ```bash
    git clone https://github.com/danilocamposdev/tcc-20262.git
    cd tcc-20262
    ```
 
2. Compilar o projeto (a primeira vez, por instalar e compilar o Google Or Tools e suas dependências, pode demorar vários minutos)
    ```bash
    cmake -S . -B build
    cmake --build build
    ```
3. Rodar os testes
    ```bash
    ./build/tcc-20262_tests
    # resultado salvo em tests/results/tests.txt
    ```
 
---
 
**Autores:** Danilo Teixeira Gonçalves Campos, Victória Dias de Almeida
 
**Orientador:** Prof. Dr. Maurício Cardoso
 
**Curso:** Engenharia de Produção — UFMG — 2026
 

