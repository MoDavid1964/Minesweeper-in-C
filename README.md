# MINEZ (in C)

<pre>
    ▀████▄     ▄███▀▀████▀▀███▄   ▀███▀▀███▀▀▀███ ███▀▀▀███         █      
      ████    ████    ██    ███▄    █    ██    ▀█ █▀   ███      ▀▄█████▄▀  
      █ ██   ▄█ ██    ██    █ ███   █    ██   █   ▀   ███       ██  █████  
      █  ██  █▀ ██    ██    █  ▀██▄ █    ██████      ███      ▀▀█████████▀▀
      █  ██▄█▀  ██    ██    █   ▀██▄█    ██   █  ▄  ███   ▄     ▀███████▀  
      █  ▀██▀   ██    ██    █     ███    ██     ▄█ ███   ▄█     ▀ ▀▀█▀▀ ▀  
    ▄███▄ ▀▀  ▄████▄▄████▄▄███▄    ██  ▄███████████████████         ▀      
</pre>

## 1 Introduction

![minez](README/minez.png)

Welcome to a rough implementation of minesweeper... in C!
Brought to you by MMMM.

## 2 Running the Program

### Windows 10

If you are using Windows 10 and have not migrated to using the modern Windows Terminal (you're still using conhost), then the following commands should suffice to run the program. Otherwise, refer to the commands for Windows 11; the Windows Terminal has a different set of features which don't allow our program to do some important stuff (like resizing the console).

```C
gcc main.c -o main
main run
```

### Windows 11

Fortunately, even after migrating to using the Windows Terminal, the legacy console (conhost) is still accessible when running it directly using the `conhost` command (or, alternatively, looking for it manually in your Windows folder). After the console appears, just follow the same commands outlined for Windows 10.

```
conhost
gcc main.c -o main
main run
```

### Unix-based

Need I say more?

```
gcc main.c -o main
./main run
```

> NOTE: If you do not specify the `run` parameter after calling the main executable, you will be greated with the following message:
> ![compilation-warning](README/compilation-warning.png)
> To actually run the program, you must specify the `run` parameter.
> 
> As an added feature, **one can also specify `run dev` instead**; this will run an alternative executable used for debugging.

## 3 Project Structure

## 4 Implementation Outline

## 5 Test Script

## 6 About the Authors 