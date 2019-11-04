# the-walking-threads
Trabalho de Programação Concorrente 2/2019

# História
Esse programa resolve um problema envolvendo threads e programação concorrente. A trama desenvolvida é uma situação fictícia de um apocalipse zumbi, um grupo de sobreviventes acaba ficando preso em um shopping abandonado. Com o intuito de sobreviverem, as pessoas do grupo disputam as porções de comida restantes. Entretanto, quando as porções acabam, matam algum dos sobreviventes restantes para gerar mais porções de comida.

Enquanto tentam sobreviver, um carro de resgate é acionado para buscar os sobreviventes no shopping. Este sai do Local Seguro (uma colônia com outros sobreviventes), vai até o shopping e espera os sobreviventes embarcarem para serem resgatados. Porém, o carro só consegue carregar 4 sobreviventes por vez. Dada esta limitação, existe uma preferência para quem será resgatado: crianças, depois mulheres e, por fim, homens.

Os sobreviventes e o carro serão threads e a comida o rescurso compartilhado. São utilizados recursos como condicionais e semáforos para a sincronização de ações e controle das threads.

# Para compilar e executar o programa execute os seguintes comandos no terminal:

```
"gcc -ansi -Wall -o walking walking.c -lpthread"

"./walking"
```

# Alunos:
```
AMANDA OLIVEIRA ALVES - 15/0116276
FILLYPE ALVES DO NASCIMENTO - 16/0070431
```
