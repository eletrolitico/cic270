# Trabalho cic270
## Compilando no linux
basta ter instalado o gcc, o make e as bibliotecas glfw, glew, glm e openal e executar o comando 

    make

## Compilando no windows
Utilizar algo como o [MSYS 2](https://www.msys2.org/) ou [git for windows SDK](https://github.com/git-for-windows/build-extra/releases/latest) e instalar as bibliotecas, eu recomendo utilizar o git for windows SDK, pois já vem com um 'git' que funciona melhor no windows e um 'make' melhor que o 'mingw32-make'

    pacman -S mingw-w64-x86_64-glew mingw-w64-x86_64-glfw mingw-w64-x86_64-openal mingw-w64-x86_64-glm
E instalar o gcc e o make caso utilize somente o MSYS 2

    pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-make

Finalmente compilar

    make

Ou

    mingw32-make
Se estiver utilizando o MSYS 2
