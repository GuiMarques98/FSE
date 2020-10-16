# Trabalho 1

Esse é a implementação do Trabalho 1 de Fundamentos de Sistemas Embarcados da Universidade de Brasília do semestre de 2020.1. O trabalho está definido [neste](https://gitlab.com/fse_fga/projetos/projeto-1) GitLab.

### Compilação e Execução do programa

A compilação é feita de maneira simples, antes é necessário a instalação das ferramentas necessárias.

```sudo apt-get install make gcc```

Depois é necessário estar na mesma pasta onde o `Makefile` está e rodar o comando de compilação.

```make```

Para executar o programa.

```make run```

Para limpar os .o e o executável só rodar o comando:

```make clean```

### Execução do Programa

Quando o programa inicia ele mostrara. Digite qual a opção e insira a temperatura caso necessário. Caso o programa seja interrompido por qualquer motivo o sistema desligara tudo normalmente.

```
Por favor insira a forma que deseja ajustar a temperatura:
1 - Potenciometro
2 - Teclado
3 - Sair
```
