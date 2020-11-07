# Trabalho 2

Esse é a implementação do Trabalho 2 de Fundamentos de Sistemas Embarcados da Universidade de Brasília do semestre de 2020.1. O trabalho está definido [neste](https://gitlab.com/fse_fga/projetos/projeto-2) GitLab.

# Servidor Central

Essa pasta diz respeito ao servidor central que controla todos os servidores distribuidos. O papel desse servidor é enviar comandos e controlar os dispositivos nos servidores distribuidos via TCP/IP.


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

O Servidor central precisa ser executado primeiro:

```
┌───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│Temperatura: 0.00                                                                                                                  │
│Humidade: 0.00                                                                                                                     │
└───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘
┌─────────────────────────────────────────────────────────────────────┐┌─────────────────────────────────────────────────────────────┐
│Lampada da Cozinha                                          DESLIGADO││Erros:                                                       │
│Lampada da Sala                                             DESLIGADO││                                                             │
│Lampada da Quarto 1                                         DESLIGADO││                                                             │
│Lampada da Quarto 2                                         DESLIGADO││                                                             │
│Ar Condicionado do Quarto 1                                 DESLIGADO││                                                             │
│Ar Condicionado do Quarto 2                                 DESLIGADO││                                                             │
│Sair                                                                 ││                                                             │
│                                                                     ││                                                             │
└─────────────────────────────────────────────────────────────────────┘└─────────────────────────────────────────────────────────────┘

```
