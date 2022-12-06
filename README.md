# Entregáveis e avaliação.

Serão três itens a serem avaliados:

1. (Peso 3) O programa prog_exp

2. (Peso 3) Um relatório (em pdf) sobre os experimentos, com explicações, gráficos e conclusões

3. (Peso 2) O programa prog_print


Não serão considerados programas que contêm erros de compilação. O relatório será aceito apenas se o programa estiver executando. Os dois programas referentes aos itens 1 e 3 acima poderão ser codificados em C ou C++.

Notar ainda que o item 3, caso não esteja correto, pode impactar os resultados dos itens 1 e 2. Portanto, é muito importante que estejam atentos à correção da sua implementação.

## prog_exp

Para os experimentos, deve-se considerar sequências de inteiros, a serem lidos e inseridos na árvore. Como serão experimentos comparativos, para cada elemento de uma sequência de dados, deve-se realizar inserções nas árvores AVL e VP, medir o número de rotações nelas realizadas e o seu nível de balanceamento. O nível de balanceamento de cada nó i da árvore é dado por

```
B_i = Altura_Esquerda_i - Altura_Direita_i
```

O nível de balanceamento `NB` da árvore é dado pela soma de |`B_i`| (valor absoluto de `B_i`), para cada um de seus nós `i`, dividida pelo número de nós na árvore.

Supondo que o executável do programa que implementa o experimento é denominado prog_exp, a execução, em linha de comando, será realizada no momento da correção como:

```
./prog_exp < entrada.txt
```

onde entrada.txt representa o arquivo texto com uma sequência de valores inteiros, um por linha. Exemplos de arquivos de entrada (input1.txt e input2.txt) estão disponibilizados no AVA Moodle como anexos a esta descrição.

Em sua saída, o programa deverá exibir dois valores de NB, calculados para as árvores AVL e VP. Além disso, deve-se exibir o número médio de rotações, contabilizados como número total de rotações dividido pelo número total de nós inseridos.

### Compilar

```Powershell
    g++ src/prog_exp.cpp -o prog_exp
```

### Executar

```Powershell
    # PowerShell
    cmd.exe /c ".\prog_exp.exe < .\input\input1.txt"
    cmd.exe /c ".\prog_exp.exe < .\input\input2.txt"
```

## prog_print

Este programa deverá ler uma sequência de valores, tal como descrito na seção sobre experimentos, e deve imprimir os nós da árvore VP por nível, usando varredura em largura, da esquerda para a direita. Este item servirá para verificar a correção da implementação de árvore VP. A entrada a ser submetida nesta caso não deverá ser grande e o formato de execução é o mesmo da descrição anterior, ou seja

```
./prog_print < entrada.txt
```

### Compilar

```Powershell
    g++ src/prog_print.cpp -o prog_print
```

### Executar

```Powershell
    # PowerShell
    cmd.exe /c ".\prog_print.exe < .\input\input1.txt"
    cmd.exe /c ".\prog_print.exe < .\input\input2.txt"
```